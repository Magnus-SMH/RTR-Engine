#pragma once

#ifdef RTR_ENABLE_LOGGING
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "RTR/Core/Log.h"
#endif

#include "RTR/Core/Base.h"
#include "RTR/Core/Platform.h"

#include <algorithm>
#include <memory>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <csignal>

#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tiny_gltf.h>
