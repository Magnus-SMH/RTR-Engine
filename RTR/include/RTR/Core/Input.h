#pragma once

#include "RTR/Core/InputState.h"
#include "RTR/Core/TripleBuffer.h"
#include "RTR/Core/Events.h"

namespace RTR
{
    class Input
    {
    public:
        static void OnEvent(Event& e);
        static void EndFrame();

        static void FetchSnapshot();

        static bool IsKeyDown(KeyCode key);
        static bool WasKeyPressed(KeyCode key);
        static bool WasKeyReleased(KeyCode key);

        static bool IsMouseDown(MouseCode button);
        static bool WasMousePressed(MouseCode button);
        static bool WasMouseReleased(MouseCode button);

        static glm::vec2 GetMousePosition();
        static glm::vec2 GetMouseDelta();

    private:
        static TripleBuffer<InputState> s_TripleBuffer;
        static InputState s_PrevSnapshot;
        static InputState s_WriterState;
    };
}
