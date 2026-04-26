#include "RTR/Core/Application.h"
#include "RTR/Core/TickClock.h"

#ifndef RTR_HEADLESS
	#include "RTR/Renderer/RenderCommand.h"
#endif

namespace RTR
{
	Application* Application::s_Instance = nullptr;

	static void SignalHandler(int signal)
	{
		if (signal == SIGINT || signal == SIGTERM)
		{
			Application::Get().Close();
		}
	}

	Application::Application(const ApplicationSpecification& spec)
		: m_Spec(spec)
	{
		RTR_CORE_ASSERT(!s_Instance, "Application already exists!");
		RTR_CORE_INFO("New Application: {0}", m_Spec.Name);

		s_Instance = this;

		std::signal(SIGINT, SignalHandler);
		std::signal(SIGTERM, SignalHandler);

		if (!m_Spec.WorkingDirectory.empty())
		{
			std::filesystem::path workDir = std::filesystem::absolute(m_Spec.WorkingDirectory);

			if (std::filesystem::exists(workDir))
				std::filesystem::current_path(workDir);
			else
				RTR_CORE_ERROR("Working directory doesnt exist: {0}", m_Spec.WorkingDirectory);
		}

#ifndef RTR_HEADLESS
		
		RTR_CORE_INFO("Window '{0}' ({1}x{2}) initialisation", spec.Window.Title, spec.Window.Width, spec.Window.Height);
		m_Window = Window::Create(spec.Window);	
		m_Window->SetEventCallback([this](Event event)
			{
				//Push events into OS event queue
				m_OSEventQueue.Push(std::move(event));
			});

		auto imgui = std::make_unique<ImGuiLayer>();
		m_ImGuiLayer = imgui.get();
		m_LayerStack.PushOverlay(std::move(imgui));

#else
		RTR_CORE_INFO("Running in headless mode.");
#endif
	}


	Application::~Application()
	{
		for (auto& layerPtr : m_LayerStack)
			layerPtr->OnDetach();
		s_Instance = nullptr;
	}

	void Application::PushLayer(std::unique_ptr<Layer> layer)
	{
		m_LayerStack.PushLayer(std::move(layer));
	}

	void Application::PushOverlay(std::unique_ptr<Layer> overlay)
	{
		m_LayerStack.PushOverlay(std::move(overlay));
	}

	void Application::OnEvent(Event& event)
	{
		EventContext ctx{ event };

		//Main thread handles window close/resize events
		EventDispatcher dispatcher(ctx.Event);
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& event) { return OnWindowClose(event); });
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event) { return OnWindowResize(event); });

		ctx.Handled = dispatcher.IsHandled();

		//iterates the layers from back to front
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			Layer& layer = **it;
			if (ctx.Handled) break;
			if (layer.IsAttached() && layer.GetAffinity() != LayerAffinity::Sim)
			{
				layer.OnEvent(ctx);
			}
		}

		if (!ctx.Handled && (ctx.IsMouseEvent() || ctx.IsKeyEvent()))
			m_SimEventQueue.Push(event);

	}


	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		RTR_CORE_INFO("WindowCloseEvent received! Shutting down.");
		m_Running.store(false, std::memory_order_release);
		return false;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		m_Minimized.store(event.Width == 0 || event.Height == 0, std::memory_order_release);
		return false;
	}



	void Application::Run()
	{
#ifdef RTR_HEADLESS
		HeadlessRun();
#else
		
		m_RenderThread = std::thread([this]() { RenderThreadRun(); });
		m_SimThread = std::thread([this]() { SimThreadRun(); });
		OSThreadRun();

		m_SimThread.join();
		m_RenderThread.join();
		RTR_CORE_INFO("Run() exited");
#endif
	}

#ifndef RTR_HEADLESS

	void Application::OSThreadRun()
	{
		//Give render thread ownership of opengl context
		m_Window->GetGraphicsContext().Release();

		//Main thread loop
		while (m_Running.load(std::memory_order_acquire))
		{
			m_Window->PollEvents();
		}

		//Temp fix to ensure the renderer thread has finished before we destroy the window context
		while (!temp_RendererFinished.load(std::memory_order_acquire))
		{
			m_Window->PollEvents();
		}

		RTR_CORE_DEBUG("OS thread exiting");
	}



	void Application::SimThreadRun()
	{

		m_RenderReady.wait();

		TickClock clock(60.0, 5);

		m_Stats.tickTargetMs.store(clock.GetTickDelta() * 1000.0f, std::memory_order_relaxed);

		while (m_Running.load(std::memory_order_acquire))
		{
			m_SimEventQueue.Flush([this](Event& event)
				{
					EventContext ctx{ event };

					for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
					{
						Layer& layer = **it;
						if (ctx.Handled) break;
						if (layer.IsAttached() && layer.GetAffinity() != LayerAffinity::Render)
							layer.OnEvent(ctx);
					}
				});

			const int tickCount = clock.Update();
			for (int i = 0; i < tickCount; ++i)
			{
				const auto tickStart = std::chrono::steady_clock::now();

				for (auto& layerPtr : m_LayerStack)
				{
					Layer& layer = *layerPtr;
					if (layer.IsAttached() && layer.GetAffinity() != LayerAffinity::Render)
						layer.OnTick(clock.GetTickDelta());
				}


				SimState& snap = m_SimStateBuffer.GetWriteSlot();
				snap.tickIndex = clock.NextTickIndex();
				snap.tickDelta = clock.GetTickDelta();
				snap.simTimeSeconds = static_cast<double>(snap.tickIndex) * clock.GetTickDurationD();
				snap.isPaused = m_Paused.load(std::memory_order_relaxed);
				m_SimStateBuffer.Publish();

				const auto tickEnd = std::chrono::steady_clock::now();
				const float workMs = std::chrono::duration<float, std::milli>(tickEnd - tickStart).count();
				m_Stats.tickWorkTimeMs.store(workMs, std::memory_order_relaxed);
				m_Stats.tickIndex.store(snap.tickIndex, std::memory_order_relaxed);
			}
			if (tickCount == 0)
				clock.SleepUntilNextTick();
		}
		RTR_CORE_DEBUG("Sim thread exiting");

	}

	void Application::RenderThreadRun()
	{
		m_Window->GetGraphicsContext().MakeCurrent();
		RenderCommand::Init();

		m_ImGuiLayer->InitForRender(m_Window->GetNativeWindow());

		m_RenderReady.count_down();

		double startRenderTime = Platform::GetTime();

		while (m_Running.load(std::memory_order_acquire))
		{
			const double now = Platform::GetTime();
			const float renderDelta = static_cast<float>(now - startRenderTime);
			startRenderTime = now;
			m_Stats.renderDeltaMs.store(renderDelta * 1000.0f, std::memory_order_relaxed);

			m_OSEventQueue.Flush([this](Event& event) { OnEvent(event); });

			m_SimStateBuffer.Fetch();
			const SimState& snapshot = m_SimStateBuffer.Get();

			if (!m_Minimized.load(std::memory_order_acquire))
			{
				RenderCommand::SetClearColor({ 0,0,0,1 });
				RenderCommand::Clear();

				for (auto& layerPtr : m_LayerStack)
				{
					Layer& layer = *layerPtr;
					if (layer.IsAttached() && layer.GetAffinity() != LayerAffinity::Sim)
						layer.OnRender(snapshot, renderDelta);
				}

				m_ImGuiLayer->Begin();
				for (auto& layerPtr : m_LayerStack)
				{
					Layer& layer = *layerPtr;
					if (layer.IsAttached() && layer.GetAffinity() != LayerAffinity::Sim)
						layer.OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}
			m_Window->GetGraphicsContext().SwapBuffers();
		}
		//Temp fix!
		temp_RendererFinished.store(true, std::memory_order_release);
		RTR_CORE_DEBUG("Render thread exiting");
	}
#endif


#ifdef RTR_HEADLESS
	void Application::HeadlessRun()
	{
		TickClock clock(60.0, 5);

		m_Stats.tickTargetMs.store(clock.GetTickDelta() * 1000.0f, std::memory_order_relaxed);

		while (m_Running.load(std::memory_order_acquire))
		{
			const int tickCount = clock.Update();
			for (int i = 0; i < tickCount; ++i)
			{
				const auto tickStart = std::chrono::steady_clock::now();

				for (auto& layerPtr : m_LayerStack)
				{
					Layer& layer = *layerPtr;
					if (layer.IsAttached() && layer.GetAffinity() != LayerAffinity::Render)
						layer.OnTick(clock.GetTickDelta());
				}

				SimState& snap = m_SimStateBuffer.GetWriteSlot();
				snap.tickIndex = clock.NextTickIndex();
				snap.tickDelta = clock.GetTickDelta();
				snap.simTimeSeconds = static_cast<double>(snap.tickIndex) * clock.GetTickDurationD();
				snap.isPaused = m_Paused.load(std::memory_order_relaxed);
				m_SimStateBuffer.Publish();

				const auto tickEnd = std::chrono::steady_clock::now();
				const float workMs = std::chrono::duration<float, std::milli>(tickEnd - tickStart).count();
				m_Stats.tickWorkTimeMs.store(workMs, std::memory_order_relaxed);
				m_Stats.tickIndex.store(snap.tickIndex, std::memory_order_relaxed);
			}
			if (tickCount == 0)
				clock.SleepUntilNextTick();
		}
		RTR_CORE_INFO("HeadlessRun() exited.");
	}
#endif

	void Application::Close()
	{
		m_Running.store(false, std::memory_order_release);
	}

	Application& Application::Get()
	{
		RTR_CORE_ASSERT(s_Instance, "Application instance does not exist!");
		return *s_Instance;
	}
}