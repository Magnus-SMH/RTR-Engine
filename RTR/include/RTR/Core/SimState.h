#pragma once

#include "RTR/Core/UUID.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace RTR
{
	struct RenderProxy
	{
		glm::mat4 transform;
		UUID uuid;
	};

	struct SimState
	{
		uint64_t tickIndex = 0;
		double simTimeSeconds = 0.0;
		double tickDelta = 0.0;
		bool isPaused = false;

		std::vector<RenderProxy> renderProxies;
	};
}