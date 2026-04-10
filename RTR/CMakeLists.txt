file(GLOB_RECURSE RTR_SOURCES CONFIGURE_DEPENDS
    "src/RTR/*.cpp"
    "src/Platform/OpenGL/*.cpp"
)

add_library(RTR STATIC ${RTR_SOURCES})

target_include_directories(RTR
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform
)

target_link_libraries(RTR
    PUBLIC glm::glm
    PUBLIC glfw
    PUBLIC glad
    PUBLIC imgui
    PUBLIC stb
)

target_compile_definitions(RTR PUBLIC
    RTR_ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets/"
)