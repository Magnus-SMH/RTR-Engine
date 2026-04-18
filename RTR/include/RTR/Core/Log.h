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
		inline static const std::shared_ptr<spdlog::logger>& GetCoreLogger() {return s_CoreLogger;}
		inline static const std::shared_ptr<spdlog::logger>& GetClientLogger() {return s_ClientLogger;}
#endif
	private:
#ifdef RTR_ENABLE_LOGGING
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
#endif
	};
}

#ifdef RTR_ENABLE_LOGGING

#define RTR_CORE_TRACE(...)    SPDLOG_LOGGER_TRACE(::RTR::Log::GetCoreLogger(), __VA_ARGS__)
#define RTR_CORE_INFO(...)     SPDLOG_LOGGER_INFO(::RTR::Log::GetCoreLogger(), __VA_ARGS__)
#define RTR_CORE_WARN(...)     SPDLOG_LOGGER_WARN(::RTR::Log::GetCoreLogger(), __VA_ARGS__)
#define RTR_CORE_ERROR(...)    SPDLOG_LOGGER_ERROR(::RTR::Log::GetCoreLogger(), __VA_ARGS__)
#define RTR_CORE_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(::RTR::Log::GetCoreLogger(), __VA_ARGS__)

#define RTR_TRACE(...)         SPDLOG_LOGGER_TRACE(::RTR::Log::GetClientLogger(), __VA_ARGS__)
#define RTR_INFO(...)          SPDLOG_LOGGER_INFO(::RTR::Log::GetClientLogger(), __VA_ARGS__)
#define RTR_WARN(...)          SPDLOG_LOGGER_WARN(::RTR::Log::GetClientLogger(), __VA_ARGS__)
#define RTR_ERROR(...)         SPDLOG_LOGGER_ERROR(::RTR::Log::GetClientLogger(), __VA_ARGS__)
#define RTR_CRITICAL(...)      SPDLOG_LOGGER_CRITICAL(::RTR::Log::GetClientLogger(), __VA_ARGS__)

#else

#define RTR_CORE_TRACE(...)
#define RTR_CORE_INFO(...)
#define RTR_CORE_WARN(...)
#define RTR_CORE_ERROR(...)
#define RTR_CORE_CRITICAL(...)

#define RTR_TRACE(...)
#define RTR_INFO(...)
#define RTR_WARN(...)
#define RTR_ERROR(...)
#define RTR_CRITICAL(...)

#endif