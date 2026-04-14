#pragma once

#include "RTR/Core/Log.h"

extern RTR::Application* RTR::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	RTR::Log::Init();

	auto app = RTR::CreateApplication({ argc, argv });
	app->Run();
	delete app;
}

