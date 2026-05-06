#pragma once
#include <RTR/RTR.h>

class HeadlessTestLayer : public RTR::Layer
{
public:
    HeadlessTestLayer( RTR::EventQueue& simEventQueue)
        : Layer("HeadlessTestLayer"), m_SimEventQueue(simEventQueue)
    {
    }

    void OnAttach() override;
    void OnDetach() override;
	void OnEvent(RTR::EventContext ctx) override;
	void OnTick(float /*tickDelta*/) override;

private:
    RTR::EventQueue& m_SimEventQueue;

    RTR::UUID m_MeshId = RTR::UUID::Null();
};