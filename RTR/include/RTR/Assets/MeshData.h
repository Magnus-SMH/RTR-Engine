#pragma once

#include "RTR/Core/UUID.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace RTR
{
	struct SubMeshData
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<uint32_t> indices;

		UUID materialID;
	};

	struct CPUMesh
	{
		UUID uuid;
		std::string name;
		std::vector<SubMeshData> subMeshes;
	};
}