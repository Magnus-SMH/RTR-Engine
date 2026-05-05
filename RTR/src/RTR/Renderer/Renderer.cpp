#include "RTR/Renderer/Renderer.h"

#include <glm/gtc/type_ptr.hpp>

namespace RTR
{
    void Renderer::Init()
    {
        RenderCommand::Init();
    }

    void Renderer::Shutdown() {}

    void Renderer::BeginScene(const glm::mat4& viewProjection)
    {
        s_SceneData = std::make_unique<SceneData>();
        s_SceneData->ViewProjectionMatrix = viewProjection;
    }

    void Renderer::EndScene()
    {
        s_SceneData.reset();
    }

    void Renderer::Submit(
        const std::shared_ptr<Shader>& shader,        
        const std::shared_ptr<VertexArray>& va,
        const glm::mat4& transform)
    {
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetMat4("u_Transform", transform);
        va->Bind();
        RenderCommand::DrawIndexed(va);
    }
}