#include "RTR/Assets/MeshLoader.h"

#define TINYGLTF_IMPLEMENTATION
//#define TINYGLTF_NO_STB_IMAGE
//#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include <tiny_gltf.h>

//Todo: https://www.khronos.org/files/gltf20-reference-guide.pdf

//Todo: Currently doing per mesh/submesh, later change to data-oriented design later by copying data directly into one buffer

namespace RTR
{
    template<typename T>
    static std::vector<T> ExtractAccessor(
        const tinygltf::Model& model,
        int accessorIndex)
    {
		const auto& accessor = model.accessors[accessorIndex];
		const auto& bufferView = model.bufferViews[accessor.bufferView];
		const auto& buffer = model.buffers[bufferView.buffer];

		const uint8_t* dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

        //component type size
        size_t componentSize = 0;
        switch (accessor.componentType)
        {
            case TINYGLTF_COMPONENT_TYPE_BYTE:
                componentSize = 1;
                break;
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                componentSize = 1;
                break;
            case TINYGLTF_COMPONENT_TYPE_SHORT:
                componentSize = 2;
                break;
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                componentSize = 2;
                break;
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                componentSize = 4;
                break;
            case TINYGLTF_COMPONENT_TYPE_FLOAT:
                componentSize = 4;
                break;
            default:
                RTR_CORE_ASSERT(false, "Unsupported component type");
                return {};
        }

        //number of components
        size_t componentCount = 0;
        switch(accessor.type)
        {
            case TINYGLTF_TYPE_SCALAR:
                componentCount = 1;
                break;
            case TINYGLTF_TYPE_VEC2:
                componentCount = 2;
                break;
            case TINYGLTF_TYPE_VEC3:
                componentCount = 3;
                break;
            case TINYGLTF_TYPE_VEC4:
                componentCount = 4;
                break;
            case TINYGLTF_TYPE_MAT2:
                componentCount = 4;
                break;
            case TINYGLTF_TYPE_MAT3:
                componentCount = 9;
                break;
            case TINYGLTF_TYPE_MAT4:
                componentCount = 16;
                break;
            default:
                RTR_CORE_ASSERT(false, "Unsupported type");
                return {};
		}

        size_t elementSize = componentCount * componentSize;
        RTR_CORE_ASSERT(bufferView.byteStride == 0 || bufferView.byteStride == elementSize, "Interleaved buffer views not supported yet");

        size_t stride = (bufferView.byteStride != 0) ? bufferView.byteStride : elementSize;

		std::vector<T> result;
		result.reserve(accessor.count);

        for (size_t i = 0; i < accessor.count; i++)
        {
			const uint8_t* elementPtr = dataPtr + i * stride;
			T value{};

			if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT)
            {
				RTR_CORE_ASSERT(sizeof(T) == elementSize, "Component type and accessor type size mismatch");
				memcpy(&value, elementPtr, sizeof(T));
            }
            else
            {
                float* dst = reinterpret_cast<float*>(&value);
                for (size_t j = 0; j < componentCount; j++)
                {
                    switch (accessor.componentType)
                    {
                    case TINYGLTF_COMPONENT_TYPE_BYTE:
                    {
                        dst[j] = reinterpret_cast<const int8_t*>(elementPtr)[j];
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                    {
                        dst[j] = reinterpret_cast<const uint8_t*>(elementPtr)[j];
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_SHORT:
                    {
                        dst[j] = reinterpret_cast<const int16_t*>(elementPtr)[j];
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                    {
                        dst[j] = reinterpret_cast<const uint16_t*>(elementPtr)[j];
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                    {
                        dst[j] = static_cast<float>(reinterpret_cast<const uint32_t*>(elementPtr)[j]);
                        break;
                    }
                    default:
                        RTR_CORE_ASSERT(false, "Unhandled component type in read loop");
                        return {};
                    }
                }
            }

            result.push_back(value);
        }

		return result;
    }

	//Only supports unsigned int indices for now, reduces drawcalls. Todo: support short and byte
    static std::vector<uint32_t> ExtractIndices(
        const tinygltf::Model& model,
        int accessorIndex)
    {
        const auto& accessor = model.accessors[accessorIndex];
        const auto& bufferView = model.bufferViews[accessor.bufferView];
        const auto& buffer = model.buffers[bufferView.buffer];

        const uint8_t* dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

		size_t stride = (bufferView.byteStride != 0) ? bufferView.byteStride : accessor.ByteStride(bufferView);

        std::vector<uint32_t> indices;
        indices.reserve(accessor.count);

        for (size_t i = 0; i < accessor.count; i++)
        {
			const uint8_t* elementPtr = dataPtr + i * stride;

            switch (accessor.componentType)
            {
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                {
                    uint8_t value = *reinterpret_cast<const uint8_t*>(elementPtr);
                    indices.push_back(static_cast<uint32_t>(value));
                    break;
				}
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                {
                    uint16_t value = *reinterpret_cast<const uint16_t*>(elementPtr);
                    indices.push_back(static_cast<uint32_t>(value));
                    break;
				}
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                {
                    uint32_t value = *reinterpret_cast<const uint32_t*>(elementPtr);
                    indices.push_back(value);
                    break;
                }
                default:
                    RTR_CORE_ASSERT(false, "Unsupported index component type");
					return {};
            }
        }
		return indices;
    }

    std::optional<CPUMesh> MeshLoader::Load(const std::string& path, UUID assignedId)
    {
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err, warn;

        bool wasLoaded = loader.LoadASCIIFromFile(&model, &err, &warn, path);

        if (!wasLoaded || model.meshes.empty())
        {
            RTR_CORE_ERROR("MeshLoader failed: {}", err);
            return std::nullopt;
        }

        if (!warn.empty())
        {
            RTR_CORE_WARN("MeshLoader warning: {}", warn);
        }

		CPUMesh result;
		result.uuid = assignedId;
		result.name = path;

        for (const auto& mesh : model.meshes)
        {
            for (const auto& primitive : mesh.primitives)
            {
				SubMeshData subMesh;

                RTR_CORE_ASSERT(primitive.mode == TINYGLTF_MODE_TRIANGLES, "Only triangle primitives are supported");

                //pos
                auto pos = primitive.attributes.find("POSITION");
                RTR_CORE_ASSERT(pos != primitive.attributes.end(), "Primitive has no Position attribute");
				subMesh.positions = ExtractAccessor<glm::vec3>(model, pos->second);

                //normal
				auto normal = primitive.attributes.find("NORMAL");
				if (normal != primitive.attributes.end())
				    subMesh.normals = ExtractAccessor<glm::vec3>(model, normal->second);

				//uv
				auto uv = primitive.attributes.find("TEXCOORD_0");
                if (uv != primitive.attributes.end())
					subMesh.uvs = ExtractAccessor<glm::vec2>(model, uv->second);

				//indices
				auto indices = primitive.indices;
                if (indices >= 0)
					subMesh.indices = ExtractIndices(model, indices);

				//Material
                if (primitive.material >= 0)
                {
                    const auto& materialName = model.materials[primitive.material].name;
					uint64_t hasedID = std::hash<std::string>{}(materialName);
					subMesh.materialID = UUID(hasedID);
                }             

                result.subMeshes.push_back(std::move(subMesh));
            }
        //RTR_CORE_DEBUG("Mesh name : {0}", mesh.name);
		}

        //Todo: remove debugging
        RTR_CORE_DEBUG("Mesh Loaded: {0}", result.name);
        RTR_CORE_DEBUG("Submeshes: {0}", result.subMeshes.size());

        for (size_t i = 0; i < result.subMeshes.size(); i++)
        {
            const auto& sm = result.subMeshes[i];
            RTR_CORE_DEBUG("Submesh {0}:", i);
            RTR_CORE_DEBUG("Positions: {0}", sm.positions.size());
            RTR_CORE_DEBUG("Normals: {0}", sm.normals.size());
            RTR_CORE_DEBUG("UVs: {0}", sm.uvs.size());
            RTR_CORE_DEBUG("Indices: {0}", sm.indices.size());
            RTR_CORE_DEBUG("Material ID: {0}", static_cast<uint64_t>(sm.materialID));
        }

		return result;
    }
}