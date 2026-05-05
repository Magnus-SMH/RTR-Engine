#pragma once

#include "RTR/Renderer/Buffer.h"

namespace RTR
{
    class OpenGLVertexArray;

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(const void* vertices, uint32_t size);
        ~OpenGLVertexBuffer();

        void Bind() const override;
        void Unbind() const override;

        void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
        const BufferLayout& GetLayout() const override { return m_Layout; }
    private:
        friend class OpenGLVertexArray;

        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };


    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer();

        void Bind() const override;
        void Unbind() const override;

        uint32_t GetCount() const override { return m_Count; }
    private:
        friend class OpenGLVertexArray;

        uint32_t m_RendererID;
        uint32_t m_Count;
    };
}