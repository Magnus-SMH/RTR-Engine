#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

#include <cstdint>
#include <string>
#include <variant>
#include <format>

namespace RTR
{

	struct WindowMovedEvent { int32_t X, Y; };
	struct WindowCloseEvent {};
	struct WindowResizeEvent { uint32_t Width, Height; };
	struct WindowFocusEvent { bool Focused; };

	struct KeyPressedEvent { KeyCode  Key; };
	struct KeyReleasedEvent { KeyCode Key; };
	struct KeyTypedEvent { uint32_t Codepoint; };

	struct MouseMovedEvent { float X, Y; };
	struct MouseScrolledEvent { float OffsetX, OffsetY; };
	struct MouseButtonPressedEvent { MouseCode Button; };
	struct MouseButtonReleasedEvent { MouseCode Button; };

	using Event = std::variant <
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
			if (auto* e = std::get_if<T>(&m_Event))
			{
				m_Handled = func(*e);
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
		Event& CurrentEvent;
		bool Handled = false;

		bool IsMouseEvent() const
		{
			return std::holds_alternative<MouseMovedEvent>(CurrentEvent) ||
				std::holds_alternative<MouseScrolledEvent>(CurrentEvent) ||
				std::holds_alternative<MouseButtonPressedEvent>(CurrentEvent) ||
				std::holds_alternative<MouseButtonReleasedEvent>(CurrentEvent);
		}

		bool IsKeyEvent() const
		{
			return std::holds_alternative<KeyPressedEvent>(CurrentEvent) ||
				std::holds_alternative<KeyReleasedEvent>(CurrentEvent) ||
				std::holds_alternative<KeyTypedEvent>(CurrentEvent);
		}
	};

}