#include <RTR/RTR.h>

class Sandbox : public RTR::Application
{
public:
	Sandbox()
	{
	}
	~Sandbox()
	{
	}

};

RTR::Application* RTR::CreateApplication()
{
	return new Sandbox();
}