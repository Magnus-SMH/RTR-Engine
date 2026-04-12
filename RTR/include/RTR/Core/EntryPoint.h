#pragma once

#include "RTR/Core/Log.h"

extern RTR::Application* RTR::CreateApplication();

int main(int argc, char** argv)
{
	RTR::Log::Init();

	RTR_CORE_INFO("Initialized Log!");
	RTR_WARN("Initialized Log w/ warning!");


	auto app = RTR::CreateApplication();
	app->run();
	delete app;
}

