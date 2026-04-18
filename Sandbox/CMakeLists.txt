if(RTR_HEADLESS)
    return()
endif()

add_executable(Sandbox
    src/SandboxApp.cpp
    src/main.cpp
)

target_compile_options(Sandbox PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_link_libraries(Sandbox PRIVATE rtr_engine_full)

target_compile_features(Sandbox PRIVATE cxx_std_23)

target_compile_options(Sandbox PRIVATE
$<$<CXX_COMPILER_ID:MSVC>:/W4>
$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

set_target_properties(Sandbox PROPERTIES
VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Sandbox"
)