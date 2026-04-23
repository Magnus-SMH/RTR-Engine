#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <format>

namespace RTR
{

	using KeyCode = uint16_t;
	using MouseCode = uint8_t;

	struct WindowMovedEvent { int32_t X, Y; };
	struct WindowCloseEvent {};
	struct WindowResizeEvent { uint32_t Width, Height; };
	struct WindowFocusEvent { bool Focused; };

	struct KeyPressedEvent { KeyCode  Key; bool IsRepeat = false; };
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

	class EventDispatcher
	{
	public:
		explicit EventDispatcher(Event& event)
			: m_Event(event) {
		}

		template<typename T, typename Func>
		bool Dispatch(Func&& func)
		{
			if (auto* e = std::get_if<T>(&m_Event)) {
				m_Handled = func(*e);
				return true;
			}
			return false;
		}
		bool IsHandled() const { return m_Handled; }

	private:
		Event& m_Event;
		bool   m_Handled = false;
	};

	struct EventContext
	{
		Event& Event;
		bool   Handled = false;

		bool IsMouseEvent() const
		{
			return std::holds_alternative<MouseMovedEvent>(Event) ||
				std::holds_alternative<MouseScrolledEvent>(Event) ||
				std::holds_alternative<MouseButtonPressedEvent>(Event) ||
				std::holds_alternative<MouseButtonReleasedEvent>(Event);
		}

		bool IsKeyEvent() const
		{
			return std::holds_alternative<KeyPressedEvent>(Event) ||
				std::holds_alternative<KeyReleasedEvent>(Event) ||
				std::holds_alternative<KeyTypedEvent>(Event);
		}
	};

}