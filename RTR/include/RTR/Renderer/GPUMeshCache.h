#pragma once

#include "RTR/Core/UUID.h"
#include "RTR/Assets/MeshData.h"
#include "RTR/Renderer/VertexArray.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

namespace RTR
{
    struct GPUSubMesh
    {
        std::shared_ptr<VertexArray> vertexArray;
        uint32_t indexCount;
        UUID materialID;
    };

    struct GPUMesh
    {
        UUID uuid;
        std::string name;
        std::vector<GPUSubMesh> subMeshes;
    };

    class GPUMeshCache
    {
    public:
        void Upload(const CPUMesh& cpuMesh);
        const GPUMesh* Get(UUID id) const;
        void Evict(UUID id);

    private:
        std::unordered_map<UUID, GPUMesh> m_Cache;
    };
}