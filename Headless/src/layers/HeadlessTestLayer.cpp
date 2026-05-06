#include "HeadlessTestLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void HeadlessTestLayer::OnEvent(RTR::EventContext ctx)
{
	RTR::EventDispatcher dispatcher(ctx.currentEvent);

	if (dispatcher.Dispatch<RTR::LoadAssetAnswerEvent>(
		[this](const RTR::LoadAssetAnswerEvent& event)
		{
			RTR_INFO("HeadlessTestLayer: asset '{}' registered as uuid {}", event.path, static_cast<uint64_t>(event.uuid));
			m_MeshId = event.uuid;
			return true;
		}))
	{
		ctx.handled = true;
		return;
	}
}

void HeadlessTestLayer::OnAttach()
{
	RTR_INFO("TestLayer attached");
}

void HeadlessTestLayer::OnDetach()
{
	RTR_INFO("TestLayer detached");
}

void HeadlessTestLayer::OnTick(float /*tickDelta*/)
{

	static bool requested = false;
	if (!requested)
	{
		requested = true;

		RTR_TRACE("requesting");

		std::string modelPath = std::string(RTR_ASSETS_DIR) + "/models/Brick/brick.gltf";
		//std::string modelPath = std::string(RTR_ASSETS_DIR) + "/models/ABeautifulGame/ABeautifulGame.gltf";

		m_SimEventQueue.Push(RTR::LoadAssetRequestEvent{ modelPath });

	}

}