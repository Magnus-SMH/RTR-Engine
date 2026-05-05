#pragma once

#include "RTR/Core/UUID.h"
#include "RTR/Core/SimState.h"

#include <glm/glm.hpp>
#include <vector>

namespace RTR
{
    struct SceneObject
    {
        glm::mat4 transform = glm::mat4(1.0f);
        UUID meshId;
    };

    class Scene
    {
    public:
        void ExtractRenderData(SimState& out) const;
        std::vector<SceneObject> objects;
    };
}