#pragma once

#include <memory>
#include <string>

namespace RTR
{
	struct WindowSpecification
	{
		std::string Title = "RTR Engine";
		uint32_t Width = 1280;
		uint32_t Height = 720;

	};

	class Window
	{
	public:
		virtual ~Window() = default;
		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

	private:
	};
}
