cmake_minimum_required(VERSION 3.20)
project(RTR-Engine CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_subdirectory(vendor)
add_subdirectory(RTR)
add_subdirectory(Sandbox)