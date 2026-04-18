#pragma once

#include <GLFW/glfw3.h>

#include "RTR/Core/Window.h"

namespace RTR
{
	class GLFWWindow : public Window
	{
	public:
		explicit GLFWWindow(const WindowSpecification& spec);
		virtual ~GLFWWindow();

		void OnUpdate() override;

		uint32_t GetWidth() const override { return m_Data.Width; }
		uint32_t GetHeight() const override { return m_Data.Height; }

		void SetEventCallback(EventCallbackFn EventCallback) override { m_Data.EventCallback = std::move(EventCallback); }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override { return m_Data.VSync; }
		void* GetNativeWindow() const override { return m_Window; }

	private:
		void Init(const WindowSpecification& spec);
		void Shutdown();	
		void RegisterCallbacks();

		GLFWwindow* m_Window = nullptr;

		struct WindowData
		{
			std::string Title;
			uint32_t Width = 0;
			uint32_t Height = 0;
			bool VSync = true;
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}