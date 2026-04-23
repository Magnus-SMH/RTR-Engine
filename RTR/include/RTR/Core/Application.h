#pragma once

#include "RTR/Core/Window.h"
#include "RTR/Core/Base.h"
#include "RTR/Core/LayerStack.h"
#include "RTR/Core/Events.h"

#include "RTR/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace RTR
{
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			if (index >= Count)
				return nullptr;	//Todo: Macro 
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "RTR Application";
		std::string WorkingDirectory;
		WindowSpecification Window;
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class Application 
	{
	public:
		explicit Application(const ApplicationSpecification& spec = {});
		virtual ~Application();

		RTR_NON_COPYABLE(Application);

		void Run();
		void Close();

		void PushLayer(std::unique_ptr<Layer> layer);
		void PushOverlay(std::unique_ptr<Layer> overlay);

#ifndef RTR_HEADLESS
		Window& GetWindow() { return *m_Window; }
		const Window& GetWindow() const { return *m_Window; }
#endif

		const ApplicationSpecification& GetSpec() const { return m_Spec; }
		
		static Application& Get();

		inline float GetDeltaTime() const { return m_DeltaTime; }
		inline float GetTickWorkTime() const { return m_TickWorkTime; }
		inline float GetTickTarget() const { return static_cast<float>(m_TickTargetDeltaMs); }

	private:
		void OnEvent(Event& event);

		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);
		bool OnWindowMoved(WindowMovedEvent& event);

		ApplicationSpecification m_Spec;
#ifndef RTR_HEADLESS
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer = nullptr;
#endif
		LayerStack m_LayerStack;

		bool m_Running = true;
		bool m_Minimized = false;

		double m_LastFrameTime = 0.0;
		double m_TickAccumulator = 0.0;
		float m_DeltaTime = 0.0f;
		float m_TickWorkTime = 0.0f;
		const double m_TickRate = 64.0;
		const double m_TickTargetDelta = 1.0 / m_TickRate;
		const double m_TickTargetDeltaMs = (1.0 / m_TickRate) * 1000.0;

		static Application* s_Instance;

	};
}