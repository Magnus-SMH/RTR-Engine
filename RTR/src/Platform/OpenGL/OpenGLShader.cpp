#include "OpenGLShader.h"

#include <glad/gl.h>
#include <fstream>
#include <sstream>

namespace RTR
{

    std::shared_ptr<Shader> Shader::Create(const std::string& vertexPath,
        const std::string& fragmentPath)
    {
        return std::make_shared<OpenGLShader>(vertexPath, fragmentPath);
    }

    std::string OpenGLShader::ReadFile(const std::string& path)
    {
        std::ifstream file(path);
        RTR_CORE_ASSERT(file.is_open(), "Failed to open shader file: {}", path);

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    uint32_t OpenGLShader::CompileStage(uint32_t type, const std::string& source)
    {

        uint32_t shader = glCreateShader(type);

        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        int32_t success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            const char* stageName = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
            RTR_CORE_ERROR("{} Shader compilation failed: {}", stageName, infoLog);
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }

    OpenGLShader::OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath)
    {

        std::string vertexSource = ReadFile(vertexPath);
        std::string fragmentSource = ReadFile(fragmentPath);

        uint32_t vs = CompileStage(GL_VERTEX_SHADER, vertexSource);
        uint32_t fs = CompileStage(GL_FRAGMENT_SHADER, fragmentSource);
        RTR_CORE_ASSERT(vs && fs, "Shader stage compilation failed");


        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, vs);
        glAttachShader(m_RendererID, fs);
        glLinkProgram(m_RendererID);

        int32_t success;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(m_RendererID, 512, nullptr, infoLog);
            RTR_CORE_ERROR("Shader program linking failed: {}", infoLog);
            RTR_CORE_ASSERT(false, "Shader link failed");
        }

        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    int32_t OpenGLShader::GetUniformLocation(const std::string& name) const
    {
        auto it = m_UniformCache.find(name);
        if (it != m_UniformCache.end())
            return it->second;
        
        int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
        m_UniformCache[name] = location;
        return location;
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
    {
        int32_t loc = GetUniformLocation(name);
        if (loc != -1)
            glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]);

    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        int32_t loc = GetUniformLocation(name);
        if (loc != -1)
            glUniform4f(loc, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        int32_t loc = GetUniformLocation(name);
        if (loc != -1)
            glUniform3f(loc, value.x, value.y, value.z);
    }

    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
        int32_t loc = GetUniformLocation(name);
        if (loc != -1)
            glUniform1f(loc, value);
    }

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
        int32_t loc = GetUniformLocation(name);
        if (loc != -1)
            glUniform1i(loc, value);
    }
}