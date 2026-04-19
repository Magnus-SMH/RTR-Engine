if(NOT RTR_HEADLESS)
    return()
endif()

add_executable(Headless ${RTR_WIN32_FLAG}
    src/HeadlessApp.cpp
    src/main.cpp
)

target_compile_options(Headless PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_link_libraries(Headless PRIVATE rtr_engine_core)

# makes .map file for dist builds
if(MSVC AND RTR_BUILD_MODE STREQUAL "DIST")
    target_link_options(Headless PRIVATE "/MAP")
    target_link_options(Headless PRIVATE "/MAPINFO:EXPORTS")
endif()

target_compile_features(Headless PRIVATE cxx_std_23)

target_compile_options(Headless PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

set_target_properties(Headless PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Headless"
)