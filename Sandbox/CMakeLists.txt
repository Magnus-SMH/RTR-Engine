if(RTR_HEADLESS)
    return()
endif()

add_executable(Sandbox ${RTR_WIN32_FLAG}
    src/SandboxApp.cpp
    src/main.cpp
    src/layers/TestLayer.cpp
)

target_compile_options(Sandbox PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
		/W4
		/WX
		/utf-8
		/permissive-
		>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
		-Wall
		-Wextra
		-Wpedantic
		-Werror
		-Wshadow
		>
)
target_link_libraries(Sandbox PRIVATE rtr_engine_full)

# makes .map file for dist builds
if(MSVC AND RTR_BUILD_MODE STREQUAL "DIST")
    target_link_options(Sandbox PRIVATE "/ENTRY:mainCRTStartup")

    target_link_options(Sandbox PRIVATE "/MAP")
    target_link_options(Sandbox PRIVATE "/MAPINFO:EXPORTS")
endif()

target_compile_features(Sandbox PRIVATE cxx_std_23)

set_target_properties(Sandbox PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Sandbox"
)