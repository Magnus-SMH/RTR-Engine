#include "RTR/Renderer/Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace RTR
{
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(const void* vertices, uint32_t size)
    {
        return std::make_shared<OpenGLVertexBuffer>(vertices, size);
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t count)
    {
        return std::make_shared<OpenGLIndexBuffer>(indices, count);
    }
}