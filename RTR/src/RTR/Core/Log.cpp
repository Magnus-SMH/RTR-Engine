#include "RTR/Core/Log.h"

#ifdef RTR_ENABLE_LOGGING

namespace RTR
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		//spdlog::set_pattern("%^[%T] [%l] %n: %v%$	(%@)");
		spdlog::set_pattern("%^[%T] [%l] %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("RTR");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}
#else
namespace RTR
{
	void Log::Init() {}
}
#endif