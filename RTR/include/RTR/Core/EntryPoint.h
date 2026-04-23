#pragma once

#include "RTR/Core/Application.h"
#include "RTR/Core/Log.h"

extern std::unique_ptr<RTR::Application> CreateApplication(RTR::ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
#ifdef RTR_ENABLE_LOGGING
	RTR::Log::Init();
	RTR_CORE_INFO("Engine starting");
#endif
	{
		auto app = CreateApplication({ argc, argv });
		app->Run();
	}
	RTR_CORE_INFO("Engine shutdown");
}

