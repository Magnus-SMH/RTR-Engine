#pragma once

#include "RTR/Assets/MeshData.h"

#include <string>
#include <optional>

namespace RTR
{
	class MeshLoader
	{
	public:
		static std::optional<CPUMesh> Load(const std::string& path, UUID assignedId);
	};
}