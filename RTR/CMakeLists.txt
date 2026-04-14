find_package(spdlog CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

set(RTR_ASSETS_SOURCES
src/RTR/Assets/AssetManager.cpp
src/RTR/Assets/MeshLoader.cpp
src/RTR/Assets/TextureLoader.cpp
)
add_library(rtr_assets STATIC ${RTR_ASSETS_SOURCES})
target_link_libraries(rtr_assets
    PUBLIC rtr_core
)
target_include_directories(rtr_assets
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)




set(RTR_CORE_SOURCES
src/RTR/Core/Layer.cpp
src/RTR/Core/LayerStack.cpp
src/RTR/Core/Log.cpp
src/RTR/Core/UUID.cpp
src/RTR/Core/Application.cpp
src/RTR/Core/Window.cpp
)
add_library(rtr_core STATIC ${RTR_CORE_SOURCES})

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
    PUBLIC imgui
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
src/Platform/Desktop/GLFWWindow.cpp
)
add_library(rtr_platform STATIC
    ${RTR_PLATFORM_SOURCES}
)
target_link_libraries(rtr_platform
    PRIVATE glfw
    PRIVATE glad
)
target_include_directories(rtr_platform PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform
)


add_library(rtr_engine INTERFACE)

target_include_directories(rtr_engine INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(rtr_engine INTERFACE
    rtr_core
    rtr_renderer
    rtr_scene
    rtr_assets
    rtr_imgui
)