set(RTR_SOURCES

src/RTR/Assets/AssetManager.cpp
src/RTR/Assets/MeshLoader.cpp
src/RTR/Assets/TextureLoader.cpp

src/RTR/Core/Layer.cpp
src/RTR/Core/LayerStack.cpp
src/RTR/Core/Log.cpp
src/RTR/Core/UUID.cpp
src/RTR/Core/Application.cpp
src/RTR/Core/Window.cpp

src/RTR/ImGui/ImGuiLayer.cpp

src/RTR/Renderer/Buffer.cpp
src/RTR/Renderer/Camera.cpp
src/RTR/Renderer/Framebuffer.cpp
src/RTR/Renderer/Material.cpp
src/RTR/Renderer/Mesh.cpp
src/RTR/Renderer/RenderAPI.cpp
src/RTR/Renderer/RenderCommand.cpp
src/RTR/Renderer/Renderer.cpp
src/RTR/Renderer/Renderer3D.cpp
src/RTR/Renderer/Shader.cpp
src/RTR/Renderer/Texture.cpp
src/RTR/Renderer/VertexArray.cpp

src/RTR/Scene/Scene.cpp
src/RTR/Scene/Entity.cpp

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

add_library(RTR STATIC ${RTR_SOURCES})

target_include_directories(RTR
PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform
)

target_link_libraries(RTR
PUBLIC glm::glm
PRIVATE glfw
PRIVATE glad
PRIVATE imgui
PRIVATE stb
)