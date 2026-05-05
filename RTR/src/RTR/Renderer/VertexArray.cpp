#include "RTR/Renderer/VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace RTR
{
    std::shared_ptr<VertexArray> VertexArray::Create()
    {
        return std::make_shared<OpenGLVertexArray>();
    }
}