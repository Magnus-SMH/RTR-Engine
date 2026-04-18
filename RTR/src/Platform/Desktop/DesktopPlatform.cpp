#include "RTR/Core/Platform.h"
#include <GLFW/glfw3.h>

namespace RTR
{
	double Platform::GetTime()
	{
		return glfwGetTime();
	}
}