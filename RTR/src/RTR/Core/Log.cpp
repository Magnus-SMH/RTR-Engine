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
		spdlog::init_thread_pool(8192, 1);
		auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

		s_CoreLogger = std::make_shared<spdlog::async_logger>(
			"RTR",
			sink,
			spdlog::thread_pool(),
			spdlog::async_overflow_policy::block // block if queue is full (safe)
		);
		s_CoreLoggerPtr = s_CoreLogger.get();
		s_CoreLogger->set_level(static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));
		s_CoreLogger->flush_on(spdlog::level::warn);
		s_CoreLogger->set_pattern("%^[%T] [%l] %n: %v%$");

		s_ClientLogger = std::make_shared<spdlog::async_logger>(
			"APP",
			sink,
			spdlog::thread_pool(),
			spdlog::async_overflow_policy::block
		);
		s_ClientLoggerPtr = s_ClientLogger.get();
		s_ClientLogger->set_level(static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));
		s_ClientLogger->flush_on(spdlog::level::warn);
		s_ClientLogger->set_pattern("%^[%T] [%l] %n: %v%$");
	}

	void Log::SetLevel(Level level)
	{
		auto spdLevel = static_cast<spdlog::level::level_enum>(level);
		s_CoreLogger->set_level(spdLevel);
		s_ClientLogger->set_level(spdLevel);
	}

	void Log::Shutdown()
	{
		RTR_CORE_INFO("spdlog shutdown");
		s_CoreLogger.reset();
		s_ClientLogger.reset();
		s_CoreLoggerPtr = nullptr;
		s_ClientLoggerPtr = nullptr;
		spdlog::shutdown();
	}
}
#else
namespace RTR
{
	void Log::Init() {}
}
#endif