#pragma once

#ifndef RTR_HEADLESS

#include "RTR/Core/Layer.h"

namespace RTR
{
	class Application;

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(EventContext& ctx) override;

		void OnImGuiRender() override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = true;
		Application* m_App = nullptr;
	};
}
#endif