if(NOT RTR_HEADLESS)
    return()
endif()

add_executable(Headless ${RTR_WIN32_FLAG}
    src/HeadlessApp.cpp
    src/main.cpp
    src/layers/HeadlessTestLayer.cpp
)

target_link_libraries(Headless PRIVATE rtr_engine_core)

if(MSVC AND RTR_BUILD_MODE STREQUAL "DIST")
    target_link_options(Headless PRIVATE "/ENTRY:mainCRTStartup")

    target_link_options(Headless PRIVATE "/MAP")
    target_link_options(Headless PRIVATE "/MAPINFO:EXPORTS")
endif()

target_compile_features(Headless PRIVATE cxx_std_23)

set_target_properties(Headless PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Headless"
)

target_compile_definitions(Headless PRIVATE 
    RTR_ASSETS_DIR="${CMAKE_SOURCE_DIR}/Headless/assets"
)