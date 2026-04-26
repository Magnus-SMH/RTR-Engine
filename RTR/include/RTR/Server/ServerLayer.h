#pragma once
#include "RTR/Core/Layer.h"
#include "RTR/Core/TickClock.h"

namespace RTR
{
	class ServerLayer : public Layer
	{
	public:
		ServerLayer();
		virtual ~ServerLayer() = default;

		void OnAttach() override { RTR_CORE_INFO("ServerLayer attached"); }
		void OnDetach() override { RTR_CORE_INFO("ServerLayer detached"); }
		void OnTick(float tickDelta) override
		{
		}
	};
}