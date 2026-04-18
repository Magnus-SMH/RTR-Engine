cmake_minimum_required(VERSION 3.20)
project(RTR-Engine CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(BUILD_SHARED_LIBS OFF)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)


include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT error)

set(RTR_BUILD_MODE "DEBUG" CACHE STRING "Build mode")
option(RTR_HEADLESS "Headless server build" OFF)

if(RTR_HEADLESS)
    add_compile_definitions(RTR_HEADLESS)
endif()

if (RTR_BUILD_MODE STREQUAL "DIST")
	if(result)
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
	endif()
elseif(RTR_BUILD_MODE STREQUAL "DEV")
	add_compile_definitions(RTR_ENABLE_LOGGING)
	add_compile_definitions(SPDLOG_ACTIVE_LEVEL=3) #warn
elseif(RTR_BUILD_MODE STREQUAL "DEBUG")
	add_compile_definitions(RTR_ENABLE_ASSERTS)
	add_compile_definitions(RTR_ENABLE_LOGGING)
	add_compile_definitions(SPDLOG_ACTIVE_LEVEL=0) #trace
endif()

add_subdirectory(vendor)
add_subdirectory(RTR)
add_subdirectory(Sandbox)
add_subdirectory(Headless)



# C:/vcpkg/vcpkg.exe x-update-baseline --add-initial-baseline for updating vcpkg baseline