#pragma once

#include "RTR/Core/KeyCodes.h"
#include "RTR/Core/MouseCodes.h"

#include <bitset>
#include <glm/glm.hpp>

namespace RTR
{
    struct InputState
    {
        std::bitset<512> Keys;
        std::bitset<512> PrevKeys;

        std::bitset<8> MouseButtons;
        std::bitset<8> PrevMouseButtons;

        glm::vec2 MousePos{ 0.0f };
        glm::vec2 PrevMousePos{ 0.0f };
        glm::vec2 MouseDelta{ 0.0f };
    };
}