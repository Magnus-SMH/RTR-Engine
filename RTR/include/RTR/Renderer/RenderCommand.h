#pragma once

#include "RTR/Renderer/RendererAPI.h"

namespace RTR
{
	class RenderCommand
	{
	public:
		static void Init()
		{
			s_API = RendererAPI::Create();
		}

		static void SetClearColor(const glm::vec4& color) { s_API->SetClearColor(color); }
		static void Clear() { s_API->Clear(); }

	private:
		static inline std::unique_ptr<RendererAPI> s_API = nullptr;
	};
}