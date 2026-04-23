#include "RTR/Core/Application.h"

#ifndef RTR_HEADLESS
	#include "RTR/Renderer/RenderCommand.h"
#endif

namespace RTR
{
	Application* Application::s_Instance = nullptr;
	static RTR::Application* s_AppInstance = nullptr;

	void SignalHandler(int signal)
	{
		if (signal == SIGINT || signal == SIGTERM)
		{
			RTR_CORE_INFO("Signal {0} received. Shutting down...", signal);
			if (s_AppInstance)
				s_AppInstance->Close();
		}
	}

	Application::Application(const ApplicationSpecification& spec)
		: m_Spec(spec)
	{
		RTR_CORE_ASSERT(!s_Instance, "Application already exists!");
		RTR_CORE_INFO("New Application: {0}", m_Spec.Name);

		s_Instance = this;
		s_AppInstance = this;

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
		m_Window->SetEventCallback([this](Event& event) { OnEvent(event); });

		//Renderer Init
		RenderCommand::Init();

		//ImGuiLayer Init
		auto imguiLayer = std::make_unique<ImGuiLayer>();
		m_ImGuiLayer = imguiLayer.get();
		m_LayerStack.PushOverlay(std::move(imguiLayer));

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

	//Simple tick system, for the time being in run, refactor out to src/network later
	void Application::Run()
	{
		//Swap buffers and poll events

		while (m_Running)
		{
			double time = Platform::GetTime();
			float deltaTime = static_cast<float>(time - m_LastFrameTime);
			deltaTime = std::clamp(deltaTime, 0.001f, 0.1f);
			m_LastFrameTime = time;
			m_DeltaTime = deltaTime;

			//Technical debt <3
			m_TickAccumulator += deltaTime;
			if (m_TickAccumulator >= m_TickTargetDelta)
			{
				m_TickAccumulator -= m_TickTargetDelta;
				double tickStart = Platform::GetTime();
				float tickDelta = static_cast<float>(m_TickTargetDelta);

				//RTR_CORE_TRACE("Tick: {0:.3f} ms", tickDelta * 1000.0f);

				for (auto& layer : m_LayerStack)
					if (layer->IsAttached())
						layer->OnTick(tickDelta);

				m_TickWorkTime = static_cast<float>(Platform::GetTime() - tickStart);
			}


#ifdef RTR_HEADLESS
			//Technical debt <3
			if (m_TickAccumulator < m_TickTargetDelta)
			{
				double sleepTime = m_TickTargetDelta - m_TickAccumulator;
				std::this_thread::sleep_for(
					std::chrono::duration<double>(sleepTime * 0.9)
				);
			}
#endif

#ifndef RTR_HEADLESS

			m_Window->OnUpdate();

			if (!m_Minimized)
			{
				RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
				RenderCommand::Clear();

				//Update layers
				for (auto& layerPtr : m_LayerStack)
					if (layerPtr->IsAttached())
						layerPtr->OnUpdate(deltaTime);

				//ImGui Rendering
				m_ImGuiLayer->Begin();
				for (auto& layer : m_LayerStack)
					if (layer->IsAttached())
						layer->OnImGuiRender();
				m_ImGuiLayer->End();
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