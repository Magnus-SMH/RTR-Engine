#pragma once
#include <RTR/RTR.h>

class TestLayer : public RTR::Layer
{
public:
	TestLayer() : Layer("TestLayer") {}

	void OnEvent(RTR::EventContext& ctx) override;
	void OnImGuiRender() override;

private:

};