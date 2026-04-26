#include "Platform/Desktop/GLFWWindow.h"
#include "RTR/Renderer/GraphicsContext.h"

#include <backends/imgui_impl_glfw.h>

namespace RTR
{
	static uint8_t s_GLFWWindowCount = 0;

	std::unique_ptr<Window> Window::Create(const WindowSpecification& spec)
	{
		return std::make_unique<GLFWWindow>(spec);
	}


	GLFWWindow::GLFWWindow(const WindowSpecification& spec)
	{
		Init(spec);
	}

	GLFWWindow::~GLFWWindow()
	{
		Shutdown();
	}

	void GLFWWindow::Init(const WindowSpecification& spec)
	{
		m_Data.Title = spec.Title;
		m_Data.Width = spec.Width;
		m_Data.Height = spec.Height;
		m_Data.PosX = spec.PosX;
		m_Data.PosY = spec.PosY;

		if (s_GLFWWindowCount == 0)
		{
			if (!glfwInit())
			{
				throw std::runtime_error("Failed to initialize GLFW!");
			}
			RTR_CORE_INFO("Initialized GLFW");
		}

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		RTR_CORE_ASSERT(m_Window, "Failed to create GLFW window!");

		++s_GLFWWindowCount;

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(spec.VSync);
		RegisterCallbacks();

		RTR_CORE_INFO("Created window '{}' ({}x{})", spec.Title, spec.Width, spec.Height);
	}

	void GLFWWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;
		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
			RTR_CORE_INFO("Terminated GLFW");
		}
	}

	void GLFWWindow::PollEvents()
	{
		glfwPollEvents();
	}

	void GLFWWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}

	void GLFWWindow::RegisterCallbacks()
	{
		glfwSetWindowPosCallback(m_Window,
			[](GLFWwindow* window, int xpos, int ypos)
			{
				RTR_CORE_TRACE("GLFWWindow Move callback! x={} y={}", xpos, ypos);

				if (xpos == -32000 || ypos == -32000) return;

				auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				data.PosX = xpos;
				data.PosY = ypos;

				Event e = WindowMovedEvent{ xpos, ypos };
				data.EventCallback(e);
			});

		glfwSetWindowSizeCallback(m_Window,
			[](GLFWwindow* window, int width, int height)
			{
				RTR_CORE_TRACE("GLFWWindow Resize callback! width={} height={}", width, height);

				auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				data.Width = static_cast<uint32_t>(width);
				data.Height = static_cast<uint32_t>(height);

				Event e = WindowResizeEvent{ data.Width, data.Height };
				data.EventCallback(e);
			});

		glfwSetWindowCloseCallback(m_Window,
		[](GLFWwindow* window)
		{
			RTR_CORE_TRACE("GLFWWindow Close callback!");

			auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			Event e = WindowCloseEvent{};
			data.EventCallback(e);
		});
		glfwSetWindowFocusCallback(m_Window,
			[](GLFWwindow* window, int focused)
			{
				RTR_CORE_TRACE("GLFWWindow Focus callback! focused={}", focused);

				auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				Event e = WindowFocusEvent{ focused == GLFW_TRUE };
				data.EventCallback(e);
			});

		glfwSetKeyCallback(m_Window,
		[](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			//ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
			RTR_CORE_TRACE("GLFWWindow Key callback! key={} action={}", key, action);

			auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				Event e = KeyPressedEvent{ static_cast<KeyCode>(key), false };
				data.EventCallback(e);
				break;
			}
			case GLFW_RELEASE:
			{
				Event e = KeyReleasedEvent{ static_cast<KeyCode>(key) };
				data.EventCallback(e);
				break;
			}
			case GLFW_REPEAT:
			{
				Event e = KeyPressedEvent{ static_cast<KeyCode>(key), true };
				data.EventCallback(e);
				break;
			}
			}
		});

		glfwSetCharCallback(m_Window,
		[](GLFWwindow* window, unsigned int codepoint)
		{
			//ImGui_ImplGlfw_CharCallback(window, codepoint);
			RTR_CORE_TRACE("GLFWWindow Char callback! codepoint={}", codepoint);

			auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			Event e = KeyTypedEvent{ codepoint };
			data.EventCallback(e);
		});

		glfwSetMouseButtonCallback(m_Window,
		[](GLFWwindow* window, int button, int action, int mods)
		{
			//ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
			auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			RTR_CORE_TRACE("GLFW GLFWWINDOW.CPP MouseButton callback! button={} action={}", button, action);

			switch (action)
			{
			case GLFW_PRESS:
			{
				Event e = MouseButtonPressedEvent{ static_cast<MouseCode>(button) };
				data.EventCallback(e);
				break;
			}
			case GLFW_RELEASE:
			{
				Event e = MouseButtonReleasedEvent{ static_cast<MouseCode>(button) };
				data.EventCallback(e);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Window,
		[](GLFWwindow* window, double xOffset, double yOffset)
		{
			//ImGui_ImplGlfw_ScrollCallback(window, xOffset, yOffset);
			RTR_CORE_TRACE("GLFW GLFWWINDOW.CPP Scroll callback! xOffset={} yOffset={}", xOffset, yOffset);

			auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			Event e = MouseScrolledEvent{
				static_cast<float>(xOffset),
				static_cast<float>(yOffset)
			};
			data.EventCallback(e);
		});

		glfwSetCursorPosCallback(m_Window,
		[](GLFWwindow* window, double x, double y)
		{
			//ImGui_ImplGlfw_CursorPosCallback(window, x, y);
			// RTR_CORE_TRACE("GLFW GLFWWINDOW.CPP CursorPos callback! x={} y={}", x, y);

			auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			Event e = MouseMovedEvent{
				static_cast<float>(x),
				static_cast<float>(y)
			};
			data.EventCallback(e);
		});
	}
}