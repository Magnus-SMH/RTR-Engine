#include <RTR/RTR.h>

class Headless : public RTR::Application
{
public:
	explicit Headless(const RTR::ApplicationSpecification& spec)
		: RTR::Application(spec)
	{
		RTR_INFO("Headless created!");
	}

	~Headless()
	{
	}

};

std::unique_ptr<RTR::Application> CreateApplication(RTR::ApplicationCommandLineArgs args)
{
	RTR::ApplicationSpecification spec;
	spec.Name = "Headless";
	spec.WorkingDirectory = "../Headless";
	spec.CommandLineArgs = args;

	return std::make_unique<Headless>(spec);
}