#pragma once

#include "RTR/Core/TripleBuffer.h"

#include <cstdint>

namespace RTR
{
	struct SimState
	{
		double simTimeSeconds = 0.0;
		float tickDelta = 0.0f;
		uint64_t tickIndex = 0;
		bool isPaused = false;
	};
}