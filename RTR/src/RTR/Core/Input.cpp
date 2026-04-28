#include "RTR/Core/Input.h"

namespace RTR
{
    TripleBuffer<InputState> Input::s_TripleBuffer;
    InputState Input::s_PrevSnapshot{};
    InputState Input::s_WriterState{};

    void Input::OnEvent(Event& event)
    {
        if (auto* key = std::get_if<KeyPressedEvent>(&event))
            s_WriterState.Keys.set(static_cast<size_t>(key->Key));

        else if (auto* key = std::get_if<KeyReleasedEvent>(&event))
            s_WriterState.Keys.reset(static_cast<size_t>(key->Key));

        else if (auto* mouse = std::get_if<MouseButtonPressedEvent>(&event))
            s_WriterState.MouseButtons.set(static_cast<size_t>(mouse->Button));

        else if (auto* mouse = std::get_if<MouseButtonReleasedEvent>(&event))
            s_WriterState.MouseButtons.reset(static_cast<size_t>(mouse->Button));

        else if (auto* move = std::get_if<MouseMovedEvent>(&event))
            s_WriterState.MousePos = { move->X, move->Y };
    }

    void Input::EndFrame()
    {
        s_TripleBuffer.GetWriteSlot() = s_WriterState;
        s_TripleBuffer.Publish();
    }

    void Input::FetchSnapshot()
    {
        s_PrevSnapshot = s_TripleBuffer.Get();
        s_TripleBuffer.ForceFetch();
	}

    bool Input::IsKeyDown(KeyCode key)
    {
		return s_TripleBuffer.Get().Keys[key];
    }

    bool Input::WasKeyPressed(KeyCode key)
    {
		return s_TripleBuffer.Get().Keys[key] && !s_PrevSnapshot.Keys[key];
    }

    bool Input::WasKeyReleased(KeyCode key)
    {
		return !s_TripleBuffer.Get().Keys[key] && s_PrevSnapshot.Keys[key];
    }

    bool Input::IsMouseDown(MouseCode button)
    {
    	return s_TripleBuffer.Get().MouseButtons[button];
    }

    bool Input::WasMousePressed(MouseCode button)
    {
        return s_TripleBuffer.Get().MouseButtons[button] && !s_PrevSnapshot.MouseButtons[button];
    }

    bool Input::WasMouseReleased(MouseCode button)
    {
        return !s_TripleBuffer.Get().MouseButtons[button] && s_PrevSnapshot.MouseButtons[button];
    }

    glm::vec2 Input::GetMousePosition()
    {
		return s_TripleBuffer.Get().MousePos;
    }

    glm::vec2 Input::GetMouseDelta()
    {
		return s_TripleBuffer.Get().MousePos - s_PrevSnapshot.MousePos;
    }
}