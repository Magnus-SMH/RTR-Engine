#include "RTR/Core/Log.h"

#ifdef RTR_ENABLE_LOGGING

namespace RTR
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	spdlog::logger* Log::s_CoreLoggerPtr = nullptr;
	spdlog::logger* Log::s_ClientLoggerPtr = nullptr;

	void Log::Init()
	{
		//spdlog::set_pattern("%^[%T] [%l] %n: %v%$	(%@)");
		spdlog::set_pattern("%^[%T] [%l] %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("RTR");
		s_CoreLoggerPtr = s_CoreLogger.get();
		s_CoreLogger->set_level(static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));
		s_CoreLogger->flush_on(spdlog::level::warn);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLoggerPtr = s_ClientLogger.get();
		s_ClientLogger->set_level(static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));
		s_ClientLogger->flush_on(spdlog::level::warn);
	}

	void Log::SetLevel(Level level)
	{
		auto spdLevel = static_cast<spdlog::level::level_enum>(level);
		s_CoreLogger->set_level(spdLevel);
		s_ClientLogger->set_level(spdLevel);
	}
}
#else
namespace RTR
{
	void Log::Init() {}
}
#endif