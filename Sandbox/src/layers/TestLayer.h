#pragma once
#include <RTR/RTR.h>

class TestLayer : public RTR::Layer
{
public:
    TestLayer(
        RTR::GPUMeshCache& gpuMeshCache,
        RTR::EventQueue& simEventQueue)
        : Layer("TestLayer"),
        m_GPUMeshCache(gpuMeshCache),
        m_SimEventQueue(simEventQueue)
    {
    }

    void OnAttach() override;
	void OnEvent(RTR::EventContext ctx) override;
	void OnImGuiRender() override;
	void OnTick(float /*tickDelta*/) override;
	void OnRender(const RTR::SimState& /*snapshot*/, float /*deltaTime*/) override;

private:
    RTR::GPUMeshCache& m_GPUMeshCache;
    RTR::EventQueue& m_SimEventQueue;

    RTR::UUID m_MeshId = RTR::UUID::Null();
};