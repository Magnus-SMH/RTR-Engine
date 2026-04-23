#pragma once
#include "RTR/Renderer/RendererAPI.h"

namespace RTR
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;
	};
}