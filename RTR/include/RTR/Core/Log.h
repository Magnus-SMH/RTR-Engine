#pragma once

#ifdef RTR_ENABLE_LOGGING
	#include <spdlog/spdlog.h>
	#include <spdlog/fmt/ostr.h>
#endif

namespace RTR
{
	class Log
	{
	public:
		static void Init();
#ifdef RTR_ENABLE_LOGGING
		inline static spdlog::logger* GetCoreLogger() {return s_CoreLoggerPtr;}
		inline static spdlog::logger* GetClientLogger() {return s_ClientLoggerPtr;}
		enum class Level
		{
			Trace, Debug, Info, Warn, Error, Critical, Off
		};
		static void SetLevel(Level level);
#endif
	private:
#ifdef RTR_ENABLE_LOGGING
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

		static spdlog::logger* s_CoreLoggerPtr;
		static spdlog::logger* s_ClientLoggerPtr;
#endif
	};
}

#ifdef RTR_ENABLE_LOGGING

#define RTR_CORE_TRACE(...)    SPDLOG_LOGGER_TRACE(::RTR::Log::GetCoreLogger(), __VA_ARGS__)
#define RTR_CORE_DEBUG(...)    SPDLOG_LOGGER_DEBUG(::RTR::Log::GetCoreLogger(), __VA_ARGS__)
#define RTR_CORE_INFO(...)     SPDLOG_LOGGER_INFO(::RTR::Log::GetCoreLogger(), __VA_ARGS__)
#define RTR_CORE_WARN(...)     SPDLOG_LOGGER_WARN(::RTR::Log::GetCoreLogger(), __VA_ARGS__)
#define RTR_CORE_ERROR(...)    SPDLOG_LOGGER_ERROR(::RTR::Log::GetCoreLogger(), __VA_ARGS__)
#define RTR_CORE_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(::RTR::Log::GetCoreLogger(), __VA_ARGS__)
#define RTR_CORE_OFF(...)      SPDLOG_LOGGER_OFF(::RTR::Log::GetCoreLogger(), __VA_ARGS__)

#define RTR_TRACE(...)         SPDLOG_LOGGER_TRACE(::RTR::Log::GetClientLogger(), __VA_ARGS__)
#define RTR_DEBUG(...)         SPDLOG_LOGGER_DEBUG(::RTR::Log::GetClientLogger(), __VA_ARGS__)
#define RTR_INFO(...)          SPDLOG_LOGGER_INFO(::RTR::Log::GetClientLogger(), __VA_ARGS__)
#define RTR_WARN(...)          SPDLOG_LOGGER_WARN(::RTR::Log::GetClientLogger(), __VA_ARGS__)
#define RTR_ERROR(...)         SPDLOG_LOGGER_ERROR(::RTR::Log::GetClientLogger(), __VA_ARGS__)
#define RTR_CRITICAL(...)      SPDLOG_LOGGER_CRITICAL(::RTR::Log::GetClientLogger(), __VA_ARGS__)
#define RTR_OFF(...)           SPDLOG_LOGGER_OFF(::RTR::Log::GetClientLogger(), __VA_ARGS__)

#else

#define RTR_CORE_TRACE(...)
#define RTR_CORE_DEBUG(...)
#define RTR_CORE_INFO(...)
#define RTR_CORE_WARN(...)
#define RTR_CORE_ERROR(...)
#define RTR_CORE_CRITICAL(...)
#define RTR_CORE_OFF(...)

#define RTR_TRACE(...)
#define RTR_DEBUG(...)
#define RTR_INFO(...)
#define RTR_WARN(...)
#define RTR_ERROR(...)
#define RTR_CRITICAL(...)
#define RTR_OFF(...)

#endif