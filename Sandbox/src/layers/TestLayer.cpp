#include "TestLayer.h"

void TestLayer::OnEvent(RTR::EventContext& /*ctx*/)
{
}

void TestLayer::OnImGuiRender()
{

}

void TestLayer::OnTick(float /*tickDelta*/)
{
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