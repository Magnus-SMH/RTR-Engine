#glad
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/gl.c)
    message(FATAL_ERROR "glad 2 not found")
endif()

set_source_files_properties(glad/src/gl.c PROPERTIES LANGUAGE C)
add_library(glad STATIC glad/src/gl.c)
target_include_directories(glad PUBLIC glad/include)


#stb
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.cpp)
    message(FATAL_ERROR "stb_image.cpp not found")
endif()

add_library(stb STATIC ${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.cpp)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb)


#imgui (docking branch)
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/imgui.cpp)
    message(FATAL_ERROR "imgui not found")
endif()
add_library(imgui STATIC
    imgui-docking/imgui.cpp
    imgui-docking/imgui_draw.cpp
    imgui-docking/imgui_tables.cpp
    imgui-docking/imgui_widgets.cpp
    imgui-docking/imgui_demo.cpp
)
target_include_directories(imgui PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/backends
)

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/backends/imgui_impl_glfw.cpp)
    message(FATAL_ERROR "imgui glfw backend not found")
endif()

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/backends/imgui_impl_opengl3.cpp)
    message(FATAL_ERROR "imgui opengl backend not found")
endif()

add_library(rtr_imgui_backend STATIC
    imgui-docking/backends/imgui_impl_glfw.cpp
    imgui-docking/backends/imgui_impl_opengl3.cpp
)
target_link_libraries(rtr_imgui_backend
    PUBLIC imgui
    PRIVATE glfw
    PRIVATE glad
)

add_library(implot STATIC
    implot-master/implot.cpp
    implot-master/implot_items.cpp
)
target_include_directories(implot PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/implot-master
)
target_link_libraries(implot PUBLIC imgui)