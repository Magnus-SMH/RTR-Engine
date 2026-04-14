#pragma once

#include "Window.h"
#include "Base.h"

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
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class Application 
	{
	public:
		explicit Application(const ApplicationSpecification& spec);
		virtual ~Application();

		RTR_NON_COPYABLE(Application)

		void Run();
		void Close();
		
		static Application& Get();

	private:
		ApplicationSpecification m_Spec;

		bool m_Running = true;

		static Application* s_Instance;

	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}