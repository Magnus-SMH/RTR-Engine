#pragma once
#include "RTR/Renderer/Shader.h"
#include <unordered_map>
namespace RTR
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath);
        ~OpenGLShader();

        void Bind() const override;
        void Unbind() const override;

        void SetMat4(const std::string& name, const glm::mat4& matrix) override;
        void SetFloat4(const std::string& name, const glm::vec4& value) override;
        void SetFloat3(const std::string& name, const glm::vec3& value) override;
        void SetFloat(const std::string& name, float value) override;
        void SetInt(const std::string& name, int value) override;

    private:

        std::string ReadFile(const std::string& path);

        uint32_t CompileStage(uint32_t type, const std::string& source);

        int32_t GetUniformLocation(const std::string& name) const;
        uint32_t m_RendererID = 0;

        mutable std::unordered_map<std::string, int32_t> m_UniformCache;
    };
}