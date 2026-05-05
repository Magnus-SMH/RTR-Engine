#include "RTR/Renderer/GPUMeshCache.h"

#include "RTR/Renderer/Buffer.h"

namespace RTR
{
    void GPUMeshCache::Upload(const CPUMesh& cpu)
    {
        GPUMesh gpu;
        gpu.uuid = cpu.uuid;
        gpu.name = cpu.name;

        for (const auto& sub : cpu.subMeshes)
        {
            const size_t vertexCount = sub.positions.size();

            //Interleaved  px py pz  nx ny nz  u v
            std::vector<float> interleaved;
            interleaved.reserve(vertexCount * 8);

            for (size_t i = 0; i < vertexCount; i++)
            {
                interleaved.push_back(sub.positions[i].x);
                interleaved.push_back(sub.positions[i].y);
                interleaved.push_back(sub.positions[i].z);

                if (i < sub.normals.size())
                {
                    interleaved.push_back(sub.normals[i].x);
                    interleaved.push_back(sub.normals[i].y);
                    interleaved.push_back(sub.normals[i].z);
                }

                if (i < sub.uvs.size())
                {
                    interleaved.push_back(sub.uvs[i].x);
                    interleaved.push_back(sub.uvs[i].y);
                }

            }

            auto vbo = VertexBuffer::Create(
                interleaved.data(),
                (uint32_t)(interleaved.size() * sizeof(float))
            );

            vbo->SetLayout(
                {
                    { ShaderDataType::Float3, "a_Position" },
                    { ShaderDataType::Float3, "a_Normal" },
                    { ShaderDataType::Float2, "a_TexCoord" }
                });

            auto vao = VertexArray::Create();
            vao->AddVertexBuffer(vbo);

            if (!sub.indices.empty())
            {
                auto ibo = IndexBuffer::Create( sub.indices.data(), (uint32_t)sub.indices.size() );
                vao->SetIndexBuffer(ibo);
            }

            gpu.subMeshes.push_back({
                vao,
                (uint32_t)sub.indices.size(),
                sub.materialID
                });
        }

        RTR_CORE_DEBUG("GPUMeshCache: uploaded '{}' ({} submeshes)", gpu.name, gpu.subMeshes.size());

        m_Cache[cpu.uuid] = std::move(gpu);
    }

    const GPUMesh* GPUMeshCache::Get(UUID uuid) const
    {
        auto mesh = m_Cache.find(uuid);
        return mesh != m_Cache.end() ? &mesh->second : nullptr;
    }

    void GPUMeshCache::Evict(UUID uuid)
    {
        m_Cache.erase(uuid);
    }
}