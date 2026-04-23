#include <RTR/RTR.h>

#include "layers/TestLayer.h"

class Sandbox : public RTR::Application
{
public:
	explicit Sandbox(const RTR::ApplicationSpecification& spec)
		: RTR::Application(spec)
	{
		RTR_INFO("Sandbox created");
		PushLayer(std::make_unique<TestLayer>());
	}

	~Sandbox()
	{
	}

};

std::unique_ptr<RTR::Application> CreateApplication(RTR::ApplicationCommandLineArgs args)
{
	RTR::ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = ".";
	spec.CommandLineArgs = args;
	spec.Window.Width = 1280;
	spec.Window.Height = 720;
	spec.Window.VSync = true;

	return std::make_unique<Sandbox>(spec);
}