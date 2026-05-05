#include "TestLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void TestLayer::OnEvent(RTR::EventContext ctx)
{
	RTR::EventDispatcher dispatcher(ctx.currentEvent);

	if (dispatcher.Dispatch<RTR::LoadAssetAnswerEvent>(
		[this](const RTR::LoadAssetAnswerEvent& event)
		{
			RTR_INFO("TestLayer: asset '{}' registered as uuid {}", event.path, static_cast<uint64_t>(event.uuid));
			m_MeshId = event.uuid;
			return true;
		}))
	{
		ctx.handled = true;
		return;
	}
}

void TestLayer::OnImGuiRender()
{

}

void TestLayer::OnAttach()
{
	RTR_INFO("TestLayer attached");
}

void TestLayer::OnRender(const RTR::SimState& snapshot, float /*deltaTime*/)
{
	//until camera is written
	glm::mat4 projection = glm::perspective(
		glm::radians(45.0f),
		1280.0f / 720.0f,
		0.1f,             
		100.0f 
	);
	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 4.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	glm::mat4 viewProjection = projection * view;

	RTR::Renderer::BeginScene(viewProjection);

	static auto shader = RTR::Shader::Create(
		std::string(RTR_ASSETS_DIR) + "/shaders/glsl/basic.vert",
		std::string(RTR_ASSETS_DIR) + "/shaders/glsl/basic.frag"
	);


	for (const auto& proxy : snapshot.renderProxies)
	{
		auto* gpuMesh = m_GPUMeshCache.Get(proxy.uuid);
		if (!gpuMesh)
			continue;

		for (const auto& subMesh : gpuMesh->subMeshes)
		{
			RTR::Renderer::Submit(shader, subMesh.vertexArray, proxy.transform);
		}
	}
	RTR::Renderer::EndScene();
}

void TestLayer::OnTick(float /*tickDelta*/)
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
	////Testing Meshloading directly
	//static bool s_Loaded = false;
	//if (!s_Loaded)
	//{
	//	//std::string modelPath = std::string(RTR_ASSETS_DIR) + "/models/Brick/brick.gltf";
	//	std::string modelPath = std::string(RTR_ASSETS_DIR) + "/models/ABeautifulGame/ABeautifulGame.gltf";

	//	RTR::UUID testID(123);
	//	RTR::UUID testID2(123);

	//	auto mesh = RTR::MeshLoader::Load(modelPath, testID);
	//	auto mesh2 = RTR::MeshLoader::Load(modelPath, testID2);

	//	if (mesh)
	//	{
	//		RTR_INFO("Mesh loaded successfully! Name: {}", mesh->name);
	//		RTR_INFO("SubMeshes: {}", mesh->subMeshes.size());
	//	}
	//	s_Loaded = true;
	//}

	//Testing input polling
	if (RTR::Input::WasKeyPressed(RTR::Key::W))
		RTR_DEBUG("W was pressed!");
	if (RTR::Input::IsKeyDown(RTR::Key::W))
		RTR_DEBUG("W is down!");
	if (RTR::Input::WasKeyReleased(RTR::Key::W))
		RTR_DEBUG("W was released!");

	if (RTR::Input::WasKeyPressed(RTR::Key::D))
		RTR_DEBUG("D was pressed!");
	if (RTR::Input::IsKeyDown(RTR::Key::D))
		RTR_DEBUG("D is down!");
	if (RTR::Input::WasKeyReleased(RTR::Key::D))
		RTR_DEBUG("D was released!");

	if (RTR::Input::WasKeyPressed(RTR::Key::S))
		RTR_DEBUG("S was pressed!");
	if (RTR::Input::IsKeyDown(RTR::Key::S))
		RTR_DEBUG("S is down!");
	if (RTR::Input::WasKeyReleased(RTR::Key::S))
		RTR_DEBUG("S was released!");

	if (RTR::Input::WasKeyPressed(RTR::Key::A))
		RTR_DEBUG("A was pressed!");
	if (RTR::Input::IsKeyDown(RTR::Key::A))
		RTR_DEBUG("A is down!");
	if (RTR::Input::WasKeyReleased(RTR::Key::A))
		RTR_DEBUG("A was released!");

	//RTR_DEBUG("Mouse Position: {}, {}", RTR::Input::GetMousePosition().x, RTR::Input::GetMousePosition().y);
	//RTR_DEBUG("Mouse Delta: {}, {}", RTR::Input::GetMouseDelta().x, RTR::Input::GetMouseDelta().y);
}