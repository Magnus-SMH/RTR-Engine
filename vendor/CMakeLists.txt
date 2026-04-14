# glad — static library
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/gl.c)
    message(FATAL_ERROR "glad 2 not found")
endif()

set_source_files_properties(glad/src/gl.c PROPERTIES LANGUAGE C)
add_library(glad STATIC glad/src/gl.c)
target_include_directories(glad PUBLIC glad/include)


# stb — static lib
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.cpp)
    message(FATAL_ERROR "stb_image.cpp not found")
endif()

add_library(stb STATIC ${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.cpp)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb)


# imgui
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp)
    message(FATAL_ERROR "imgui not found")
endif()
add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
 #   imgui/backends/imgui_impl_glfw.cpp
 #   imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
#    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)
# target_link_libraries(imgui PRIVATE glfw glad)

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp)
    message(FATAL_ERROR "imgui glfw backend not found")
endif()

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp)
    message(FATAL_ERROR "imgui opengl backend not found")
endif()

add_library(rtr_imgui_backend STATIC
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(rtr_imgui_backend PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

target_link_libraries(rtr_imgui_backend
    PUBLIC imgui
    PRIVATE glfw glad
)
