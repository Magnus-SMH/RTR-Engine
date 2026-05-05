#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>

namespace RTR
{
    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
        virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
        virtual void SetFloat(const std::string& name, float value) = 0;
        virtual void SetInt(const std::string& name, int value) = 0;


        static std::shared_ptr<Shader> Create(const std::string& vertexPath, const std::string& fragmentPath);
    };
}