#include <RTR/RTR.h>

class Sandbox : public RTR::Application
{
public:
	explicit Sandbox(const RTR::ApplicationSpecification& spec)
		: RTR::Application(spec)
	{
		RTR_CRITICAL("Sandbox created!");
	}

	~Sandbox()
	{
	}

};

RTR::Application* RTR::CreateApplication(RTR::ApplicationCommandLineArgs args)
{
	RTR::ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../Sandbox";
	spec.CommandLineArgs = args;
	return new Sandbox(spec);
}