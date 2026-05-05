#include "Platform/OpenGL/OpenGLContext.h"

namespace RTR
{
	std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* window)
	{
		return std::make_unique<OpenGLContext>(window);
	}

	OpenGLContext::OpenGLContext(void* windowHandle)
		: m_WindowHandle(static_cast<GLFWwindow*>(windowHandle))
	{
		RTR_CORE_ASSERT(m_WindowHandle, "Window handle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGL(glfwGetProcAddress);
		RTR_CORE_ASSERT(status, "Failed to initialize Glad!");

		RTR_CORE_INFO("OpenGL Info:");
		RTR_CORE_INFO("  Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		RTR_CORE_INFO("  Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		RTR_CORE_INFO("  Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		glEnable(GL_DEPTH_TEST);

	}

	void OpenGLContext::MakeCurrent()
	{
		glfwMakeContextCurrent(m_WindowHandle);
	}

	void OpenGLContext::Release()
	{
		glfwMakeContextCurrent(nullptr);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}