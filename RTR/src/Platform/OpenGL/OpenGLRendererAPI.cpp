#include "OpenGL/OpenGLRendererAPI.h"

#include "RTR/Renderer/VertexArray.h"

#include <glad/gl.h>

namespace RTR
{
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& va, uint32_t count)
	{
		uint32_t c = count ? count : va->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, c, GL_UNSIGNED_INT, nullptr);
	}
}