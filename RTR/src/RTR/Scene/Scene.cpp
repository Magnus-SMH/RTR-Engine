#include "RTR/Scene/Scene.h"

namespace RTR
{
    void Scene::ExtractRenderData(SimState& out) const
    {
        out.renderProxies.clear();
        for (const auto& obj : objects)
        {
            if (obj.meshId.IsNull()) continue;
            out.renderProxies.push_back({ obj.transform, obj.meshId });
        }
    }
}