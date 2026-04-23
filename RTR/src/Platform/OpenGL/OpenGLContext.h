#pragma once

#include "RTR/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace RTR
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(void* windowHandle);
		virtual ~OpenGLContext() = default;

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}