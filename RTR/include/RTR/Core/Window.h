#pragma once

#include <memory>
#include <string>
#include <functional>
#include <cstdint>

#include "RTR/Core/Events.h"
#include "RTR/Core/Base.h"
#include "RTR/Renderer/GraphicsContext.h"



namespace RTR
{
	
	struct WindowSpecification
	{
		std::string Title = "RTR Engine";
		uint32_t Width = 1280;
		uint32_t Height = 720;
		uint32_t PosX = 0;
		uint32_t PosY = 0;
		bool VSync = true;
	};

	class Window
	{
	public:
		virtual ~Window() = default;

		RTR_NON_COPYABLE(Window);

		virtual void PollEvents() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual int32_t GetPosX() const = 0;
		virtual int32_t GetPosY() const = 0;

		virtual void SetEventCallback(EventCallbackFn callback) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		// Window Context for Renderer API
		virtual void* GetNativeWindow() const = 0;
		virtual GraphicsContext& GetGraphicsContext() = 0;

		//Using factory function to not include platform specific headers
		static std::unique_ptr<Window> Create(const WindowSpecification& spec = {});
	protected:
		Window() = default;
	};
}
