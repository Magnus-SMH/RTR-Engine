#include "RTR/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace RTR
{
	std::unique_ptr<RendererAPI> RendererAPI::Create()
	{
		//OpenGL only API for now
		RTR_CORE_INFO("OpenGL RendererAPI created");
		return std::make_unique<OpenGLRendererAPI>();
	}
}