#pragma once

#include "RTR/Core/Window.h"
#include "RTR/Core/Base.h"
#include "RTR/Core/LayerStack.h"
#include "RTR/Core/Events.h"

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

	private:
		void OnEvent(Event& event);

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		ApplicationSpecification m_Spec;
#ifndef RTR_HEADLESS
		std::unique_ptr<Window> m_Window;
#endif
		LayerStack m_LayerStack;

		bool m_Running = true;
		bool m_Minimized = false;

		double m_LastFrameTime = 0.0;
		double m_TickAccumulator = 0.0;


		static Application* s_Instance;

	};
}