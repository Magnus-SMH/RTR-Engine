#pragma once

namespace RTR
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void MakeCurrent() = 0;
		virtual void Release() = 0;

		//Factory to abstract away the platform-specific context creation in rtr_platform
		static std::unique_ptr<GraphicsContext> Create(void* window);
	};
}