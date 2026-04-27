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

		LayerAffinity GetAffinity() const override { return LayerAffinity::Render; }

		void OnAttach() override;
		void OnDetach() override;

		void OnEvent(EventContext& ctx) override;

		void OnImGuiRender() override;

		void InitForRender(void* window);

		void Begin();
		void End();
	private:
		Application* m_App = nullptr;
	};
}
#endif