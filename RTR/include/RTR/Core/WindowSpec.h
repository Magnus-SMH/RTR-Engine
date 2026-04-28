#pragma once

#include <string>
#include <cstdint>

namespace RTR
{
	struct WindowSpecification
	{
		std::string Title = "RTR Engine";
		uint32_t Width = 1280;
		uint32_t Height = 720;
		uint32_t PosX = 0;
		uint32_t PosY = 0;
		bool VSync = true;
	};
}