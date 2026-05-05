#pragma once

#include "RTR/Renderer/RenderCommand.h"
#include "RTR/Renderer/VertexArray.h"
#include "RTR/Renderer/Shader.h"
#include "RTR/Renderer/Camera.h"

#include <memory>
#include <glm/glm.hpp>

namespace RTR
{
    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void OnWindowResize(uint32_t width, uint32_t height);

        static void BeginScene(const glm::mat4& viewProjection);
        static void Submit(
            const std::shared_ptr<Shader>& shader,
            const std::shared_ptr<VertexArray>& va,
            const glm::mat4& transform = glm::mat4(1.0f)
        );
        static void EndScene();
    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };
        static inline std::unique_ptr<SceneData> s_SceneData = nullptr;
    };
}