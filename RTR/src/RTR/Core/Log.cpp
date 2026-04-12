#include "RTR/Core/Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace RTR
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] [%l] %n: %v%$	(%@)");

		GetCoreLogger() = spdlog::stdout_color_mt("RTR");
		GetCoreLogger()->set_level(spdlog::level::trace);

		GetClientLogger() = spdlog::stdout_color_mt("APP");
		GetClientLogger()->set_level(spdlog::level::trace);
	}
}