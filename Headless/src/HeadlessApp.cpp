#include <RTR/RTR.h>

#include "layers/HeadlessTestLayer.h"

class Headless : public RTR::Application
{
public:
	explicit Headless(const RTR::ApplicationSpecification& spec)
		: RTR::Application(spec)
	{
		RTR_INFO("Headless created!");

		PushLayer(std::make_unique<RTR::ServerLayer>(
			GetAssetManager(),
			GetAssetLoader(),
			GetScene(),
			GetSimEventQueue()
		));

		PushLayer(std::make_unique<HeadlessTestLayer>(
			GetSimEventQueue()
		));

	}

	~Headless()
	{
	}

};

std::unique_ptr<RTR::Application> CreateApplication(RTR::ApplicationCommandLineArgs args)
{
	RTR::ApplicationSpecification spec;
	spec.Name = "Headless";
	spec.WorkingDirectory = ".";
	spec.CommandLineArgs = args;

	return std::make_unique<Headless>(spec);
}