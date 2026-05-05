#include "OpenGLVertexArray.h"
#include "OpenGLBuffer.h"

#include <glad/gl.h>

namespace RTR
{
    namespace
    {

        struct AttribSpec {
            GLint size;
            GLenum type;
            GLboolean normalized;
        };

        AttribSpec GetAttribSpec(ShaderDataType type)
        {
            switch (type)
            {
            case ShaderDataType::Float2: return { 2, GL_FLOAT, GL_FALSE };
            case ShaderDataType::Float3: return { 3, GL_FLOAT, GL_FALSE };
            case ShaderDataType::Float4: return { 4, GL_FLOAT, GL_FALSE };
            }
            return { 0, 0, 0 };
        }
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vb)
    {
        auto* OpenGLVb = static_cast<OpenGLVertexBuffer*>(vb.get());
        const auto& layout = OpenGLVb->GetLayout();

        glVertexArrayVertexBuffer(
            m_RendererID,
            m_NextBindingIndex,
            OpenGLVb->m_RendererID,
            0,
            layout.GetStride()
        );

        uint32_t attribIndex = 0;
        for (const auto& element : layout.GetElements())
        {
            auto spec = GetAttribSpec(element.type);

            glVertexArrayAttribFormat(
                m_RendererID,
                attribIndex,
                spec.size,
                spec.type,
                spec.normalized,
                element.offset
            );

            glVertexArrayAttribBinding(m_RendererID, attribIndex, m_NextBindingIndex);
            glEnableVertexArrayAttrib(m_RendererID, attribIndex);
            attribIndex++;
        }
        m_NextBindingIndex++;
    }

    void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ib)
    {
        auto* OpenGLIb = static_cast<OpenGLIndexBuffer*>(ib.get());

        glVertexArrayElementBuffer(m_RendererID, OpenGLIb->m_RendererID);
        m_IndexBuffer = ib;
    }
}