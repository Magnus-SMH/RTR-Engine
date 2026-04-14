#pragma once

#include <spdlog/spdlog.h>
#include <memory>

namespace RTR
{
	class Log
	{
	public:
		static void Init();

		inline static const std::shared_ptr<spdlog::logger>& GetCoreLogger() {return s_CoreLogger;}
		inline static const std::shared_ptr<spdlog::logger>& GetClientLogger() {return s_ClientLogger;}
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}


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