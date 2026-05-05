#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace RTR
{
	class VertexArray;

	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& va, uint32_t count = 0) = 0;

		static std::unique_ptr<RendererAPI> Create();
	};
}
