add_library(rtr_assets_common STATIC
    src/RTR/Assets/AssetManager.cpp
    src/RTR/Assets/MeshLoader.cpp

)
target_link_libraries(rtr_assets_common
    PUBLIC rtr_core
)
target_include_directories(rtr_assets_common
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)


add_library(rtr_assets_client STATIC
    src/RTR/Assets/TextureLoader.cpp
)
target_link_libraries(rtr_assets_client
    PUBLIC rtr_assets_common
    PRIVATE stb
)
target_include_directories(rtr_assets_client
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)


set(RTR_CORE_SOURCES
src/RTR/Core/Layer.cpp
src/RTR/Core/LayerStack.cpp
src/RTR/Core/Log.cpp
src/RTR/Core/UUID.cpp
src/RTR/Core/Application.cpp
)
add_library(rtr_core STATIC ${RTR_CORE_SOURCES})
if(NOT RTR_HEADLESS)
    target_sources(rtr_core PRIVATE src/RTR/Core/Window.cpp)
endif()

target_link_libraries(rtr_core
    PUBLIC spdlog::spdlog
)
target_include_directories(rtr_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)



set(RTR_IMGUI_SOURCES
src/RTR/ImGui/ImGuiLayer.cpp
)
add_library(rtr_imgui STATIC ${RTR_IMGUI_SOURCES})

target_link_libraries(rtr_imgui
    PUBLIC rtr_core
    PRIVATE imgui
    PRIVATE rtr_imgui_backend
)
target_include_directories(rtr_imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)



set(RTR_RENDERER_SOURCES
src/RTR/Renderer/Buffer.cpp
src/RTR/Renderer/Camera.cpp
src/RTR/Renderer/Framebuffer.cpp
src/RTR/Renderer/Material.cpp
src/RTR/Renderer/Mesh.cpp
src/RTR/Renderer/RendererAPI.cpp
src/RTR/Renderer/RenderCommand.cpp
src/RTR/Renderer/Renderer.cpp
src/RTR/Renderer/Renderer3D.cpp
src/RTR/Renderer/Shader.cpp
src/RTR/Renderer/Texture.cpp
src/RTR/Renderer/VertexArray.cpp
)
add_library(rtr_renderer STATIC ${RTR_RENDERER_SOURCES})

target_link_libraries(rtr_renderer
    PUBLIC rtr_core
    PUBLIC glm::glm
    PRIVATE rtr_platform
)
target_include_directories(rtr_renderer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


set(RTR_SCENE_SOURCES
src/RTR/Scene/Scene.cpp
src/RTR/Scene/Entity.cpp
)
add_library(rtr_scene STATIC ${RTR_SCENE_SOURCES})

target_link_libraries(rtr_scene
    PUBLIC rtr_core
    PRIVATE EnTT::EnTT
)
target_include_directories(rtr_scene PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)



set(RTR_PLATFORM_SOURCES
src/Platform/OpenGL/OpenGLTexture.cpp
src/Platform/OpenGL/OpenGLVertexArray.cpp
src/Platform/OpenGL/OpenGLBuffer.cpp
src/Platform/OpenGL/OpenGLContext.cpp
src/Platform/OpenGL/OpenGLDebug.cpp
src/Platform/OpenGL/OpenGLFramebuffer.cpp
src/Platform/OpenGL/OpenGLRendererAPI.cpp
src/Platform/OpenGL/OpenGLShader.cpp
)
add_library(rtr_platform STATIC
    ${RTR_PLATFORM_SOURCES}
)
if(RTR_HEADLESS)
target_sources(rtr_platform PRIVATE
    src/Platform/Headless/HeadlessPlatform.cpp
)
target_compile_definitions(rtr_platform PUBLIC RTR_HEADLESS)
else()
target_sources(rtr_platform PRIVATE
    src/Platform/Desktop/DesktopPlatform.cpp
    src/Platform/Desktop/GLFWWindow.cpp
)
endif()
target_link_libraries(rtr_platform
    PRIVATE glfw
    PRIVATE glad
    PRIVATE rtr_core
)
target_include_directories(rtr_platform PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform
    $<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>
)


add_library(rtr_engine_core INTERFACE)

target_include_directories(rtr_engine_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(rtr_engine_core INTERFACE
    rtr_core
    rtr_scene
    rtr_assets_common
    rtr_platform
)

add_library(rtr_engine_full INTERFACE)

target_link_libraries(rtr_engine_full INTERFACE
    rtr_engine_core
    rtr_assets_client
    rtr_renderer
    rtr_imgui
)