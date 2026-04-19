#pragma once

#include "RTR/Core/Log.h"
#include <filesystem>

#define RTR_NON_COPYABLE(type) \
	type(const type&) = delete; \
	type& operator=(const type&) = delete;

#ifdef RTR_ENABLE_ASSERTS

	#if defined(_MSC_VER)
		#define RTR_DEBUGBREAK() __debugbreak()
	#elif defined(__clang__) || defined(__GNUC__)
		#define RTR_DEBUGBREAK() __builtin_trap()
	#else
		#define RTR_DEBUGBREAK() std::abort()
	#endif

    #define RTR_CORE_ASSERT(check, ...) \
    do { \
		if (!(check)) { \
			RTR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
			RTR_DEBUGBREAK(); \
			std::abort(); \
		} \
	} while (0)
#else
	#define RTR_CORE_ASSERT(check, ...) \
    do { \
        if (!(check)) { std::abort(); } \
    } while (0)
#endif