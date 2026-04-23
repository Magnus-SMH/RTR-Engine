#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace RTR
{
	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		static std::unique_ptr<RendererAPI> Create();
	};
}
