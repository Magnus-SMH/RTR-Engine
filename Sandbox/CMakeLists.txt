add_executable(Sandbox

src/SandboxApp.cpp

#src/layers/.cpp
)

if (MSVC)
    target_compile_options(Sandbox PRIVATE /utf-8)
endif()

target_link_libraries(Sandbox PRIVATE RTR)

target_compile_features(Sandbox PRIVATE cxx_std_23)

target_compile_options(Sandbox PRIVATE
$<$<CXX_COMPILER_ID:MSVC>:/W4>
$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

set_target_properties(Sandbox PROPERTIES
VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)