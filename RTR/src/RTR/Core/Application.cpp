#include "RTR/Core/Application.h"

namespace RTR
{
	Application* Application::s_Instance = nullptr;
	static RTR::Application* s_AppInstance = nullptr;

	void SignalHandler(int signal)
	{
		if (signal == SIGINT || signal == SIGTERM)
		{
			RTR_CORE_WARN("Signal {0} received. Shutting down...", signal);
			if (s_AppInstance)
				s_AppInstance->Close();
		}
	}

	Application::Application(const ApplicationSpecification& spec)
		: m_Spec(spec)
	{
		RTR_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		s_AppInstance = this;

		std::signal(SIGINT, SignalHandler);
		std::signal(SIGTERM, SignalHandler);

		RTR_CORE_INFO("Application name: {0}", m_Spec.Name);

		if (!m_Spec.WorkingDirectory.empty())
		{
			std::filesystem::path workDir = std::filesystem::absolute(m_Spec.WorkingDirectory);

			if (std::filesystem::exists(workDir))
				std::filesystem::current_path(workDir);
			else
				RTR_CORE_ERROR("Working directory doesnt exist: {0}", m_Spec.WorkingDirectory);
		}

#ifndef RTR_HEADLESS
		
		//Window Init
		m_Window = Window::Create(spec.Window);
		m_Window->SetEventCallback([this](Event& event) { OnEvent(event); });
		RTR_CORE_INFO("Window Created!  {}x{}", spec.Window.Width, spec.Window.Height);
#else
		RTR_CORE_WARN("Running in headless mode.");
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
		layer->OnAttach();
		m_LayerStack.PushLayer(std::move(layer));
	}

	void Application::PushOverlay(std::unique_ptr<Layer> overlay)
	{
		overlay->OnAttach();
		m_LayerStack.PushOverlay(std::move(overlay));
	}

	void Application::OnEvent(Event& event)
	{
		EventContext ctx{ event };

		EventDispatcher dispatcher(ctx.Event);
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& event) { return OnWindowClose(event); });
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event) { return OnWindowResize(event); });

		ctx.Handled = dispatcher.IsHandled();

		//iterates the layers from back to front
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (ctx.Handled) break;
			if ((*it)->IsAttached())
				(*it)->OnEvent(ctx);
		}

	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		RTR_CORE_INFO("WindowCloseEvent received! Shutting down.");
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		if (event.Width == 0 || event.Height == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		return false;
	}

	// Simple tick system, for the time being in run, refactor out to src/network later
	void Application::Run()
	{

		constexpr double TICK_RATE = 64.0;
		constexpr double TICK_INTERVAL = 1.0 / TICK_RATE;

		while (m_Running)
		{
			double time = Platform::GetTime();
			float deltaTime = static_cast<float>(time - m_LastFrameTime);
			deltaTime = std::clamp(deltaTime, 0.001f, 0.1f);
			m_LastFrameTime = time;


			m_TickAccumulator += deltaTime;
			if (m_TickAccumulator >= TICK_INTERVAL)
			{
				m_TickAccumulator -= TICK_INTERVAL;
				float tickDelta = static_cast<float>(TICK_INTERVAL);

				//RTR_CORE_TRACE("Tick: {0:.3f} ms", tickDelta * 1000.0f);

				for (auto& layer : m_LayerStack)
					if (layer->IsAttached())
						layer->OnTick(tickDelta);
			}
#ifdef RTR_HEADLESS
			if (m_TickAccumulator < TICK_INTERVAL)
			{
				double sleepTime = TICK_INTERVAL - m_TickAccumulator;
				std::this_thread::sleep_for(
					std::chrono::duration<double>(sleepTime * 0.9)
				);
			}
#endif

#ifndef RTR_HEADLESS
			if (!m_Minimized)
			{
				for (auto& layerPtr : m_LayerStack)
					if (layerPtr->IsAttached())
						layerPtr->OnUpdate(deltaTime);
			
				for (auto& layerPtr : m_LayerStack)
					if (layerPtr->IsAttached())
						layerPtr->OnImGuiRender();

				m_Window->OnUpdate();
			}
#endif
		}
		RTR_CORE_INFO("Run() exited");
	}

	void Application::Close()
	{
		m_Running = false;
	}

	Application& Application::Get()
	{
		RTR_CORE_ASSERT(s_Instance, "Application instance does not exist!");
		return *s_Instance;
	}
}