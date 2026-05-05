#include "RTR/Server/ServerLayer.h"
#include "RTR/Core/Platform.h"
#include "RTR/Core/Application.h"
#include "RTR/Assets/AssetManager.h"
#include "RTR/Assets/AssetLoader.h"
#include "RTR/Scene/Scene.h"
#include "RTR/Core/EventQueue.h"
#include "RTR/Core/Events.h"

namespace RTR
{

    void ServerLayer::OnAttach()
    {
        RTR_CORE_INFO("ServerLayer attached");
    }

    void ServerLayer::OnDetach()
    {
    }

    void ServerLayer::OnEvent(EventContext ctx)
    {

        if (ctx.handled)
            return;

        EventDispatcher dispatcher(ctx.currentEvent);
        if (dispatcher.Dispatch<LoadAssetRequestEvent>(
            [this](const LoadAssetRequestEvent& event)
            {

                UUID uuid = m_AssetManager.RegisterAsset(event.path);
                m_AssetLoader.RequestMeshLoad(uuid, event.path);

                SceneObject obj;
                obj.meshId = uuid;
                obj.transform = glm::mat4(1.0f);
                m_Scene.objects.push_back(obj);

                m_RenderEventQueue.Push(LoadAssetAnswerEvent{ event.path, uuid });

                return true;
            }))
        {
            RTR_CORE_DEBUG("ServerLayer handled event");
            ctx.handled = true;
            return;
        }
    }

	void ServerLayer::OnTick(float /*tickDelta*/)
	{

	}
}