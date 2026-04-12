#pragma once

namespace RTR
{
	class Application 
	{
		public:
		Application();
		virtual ~Application();

		void run();

	};

	Application* CreateApplication();
}