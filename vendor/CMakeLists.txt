# glad — static library
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/gl.c)
    set_source_files_properties(glad/src/gl.c PROPERTIES LANGUAGE C)
    add_library(glad STATIC glad/src/gl.c)
    target_include_directories(glad PUBLIC glad/include)
else()
    message(WARNING "glad 2 not found")
    add_library(glad INTERFACE)
endif()

# stb — header only
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb)

# imgui
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp)
    add_library(imgui STATIC
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
    )
    target_include_directories(imgui PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    )
    target_link_libraries(imgui PRIVATE glfw glad)
else()
    message(WARNING "imgui not found")
endif()