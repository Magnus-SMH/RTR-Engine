add_executable(Sandbox src/SandboxApp.cpp)

target_link_libraries(Sandbox PRIVATE RTR)

set_target_properties(Sandbox PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)