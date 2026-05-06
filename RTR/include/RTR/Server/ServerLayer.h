#pragma once
#include "RTR/Core/Layer.h"

namespace RTR
{
    class AssetManager;
    class AssetLoader;
    class Scene;
    class EventQueue;

	class ServerLayer : public Layer
	{
	public:
		ServerLayer(
            AssetManager& assetManager,
            AssetLoader& assetLoader,
            Scene& scene,
            EventQueue& simEventQueue
            )
            : Layer("ServerLayer"),
            m_AssetManager(assetManager),
            m_AssetLoader(assetLoader),
            m_Scene(scene),
            m_SimEventQueue(simEventQueue)
        {
        }

		virtual ~ServerLayer() = default;

        LayerAffinity GetAffinity() const override { return LayerAffinity::Sim; }

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(EventContext ctx) override;
		void OnTick(float /*tickDelta*/) override;

    private:
        AssetManager& m_AssetManager;
        AssetLoader& m_AssetLoader;
        Scene& m_Scene;
        EventQueue& m_SimEventQueue;

	};
}