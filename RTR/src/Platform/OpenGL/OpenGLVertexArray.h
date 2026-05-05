#pragma once

#include "RTR/Renderer/VertexArray.h"

namespace RTR
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        void Bind() const override;
        void Unbind() const override;

        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vb) override;
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ib) override;

        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
    private:
        uint32_t m_RendererID;
        uint32_t m_NextBindingIndex = 0;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };
}