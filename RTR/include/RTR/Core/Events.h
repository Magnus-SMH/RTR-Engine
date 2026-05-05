#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"
#include "RTR/Core/UUID.h"

#include <cstdint>
#include <string>
#include <variant>
#include <format>
#include <functional>

namespace RTR
{
	struct LoadAssetRequestEvent { std::string path; };
	struct LoadAssetAnswerEvent { std::string path; UUID uuid; };
	struct UnloadAssetRequestEvent { UUID uuid; };


	struct WindowMovedEvent { int32_t x, y; };
	struct WindowCloseEvent {};
	struct WindowResizeEvent { uint32_t width, height; };
	struct WindowFocusEvent { bool focused; };

	struct KeyPressedEvent { KeyCode  key; };
	struct KeyReleasedEvent { KeyCode key; };
	struct KeyTypedEvent { uint32_t codepoint; };

	struct MouseMovedEvent { float x, y; };
	struct MouseScrolledEvent { float offsetX, offsetY; };
	struct MouseButtonPressedEvent { MouseCode button; };
	struct MouseButtonReleasedEvent { MouseCode button; };

	using Event = std::variant <
		LoadAssetRequestEvent,
		LoadAssetAnswerEvent,
		UnloadAssetRequestEvent,


		WindowMovedEvent,
		WindowCloseEvent,
		WindowResizeEvent,
		WindowFocusEvent,

		KeyPressedEvent,
		KeyReleasedEvent,
		KeyTypedEvent,

		MouseMovedEvent,
		MouseScrolledEvent,
		MouseButtonPressedEvent,
		MouseButtonReleasedEvent
	>;

	using EventCallbackFn = std::function<void(Event&)>;

	class EventDispatcher
	{
	public:
		explicit EventDispatcher(Event& event)
			: m_Event(event) 
		{
		}

		template<typename T, typename Func>
		bool Dispatch(Func&& func)
		{
			if (auto* event = std::get_if<T>(&m_Event))
			{
				m_Handled = func(*event);
				return true;
			}
			return false;
		}
		bool IsHandled() const { return m_Handled; }

	private:
		Event& m_Event;
		bool m_Handled = false;
	};

	struct EventContext
	{
		Event currentEvent;
		bool handled = false;

		bool IsMouseEvent() const
		{
			return std::holds_alternative<MouseMovedEvent>(currentEvent) ||
				std::holds_alternative<MouseScrolledEvent>(currentEvent) ||
				std::holds_alternative<MouseButtonPressedEvent>(currentEvent) ||
				std::holds_alternative<MouseButtonReleasedEvent>(currentEvent);
		}

		bool IsKeyEvent() const
		{
			return std::holds_alternative<KeyPressedEvent>(currentEvent) ||
				std::holds_alternative<KeyReleasedEvent>(currentEvent) ||
				std::holds_alternative<KeyTypedEvent>(currentEvent);
		}
	};

}