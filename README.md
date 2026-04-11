# RTR-Engine
Real-Time 3D Only Renderer Engine/Sandbox

Inspiration from TheCherno Hazel but in 3D

# Directory Structure
Unfinished File structure
```bash
RTR-Engine/
├── CMakeLists.txt
├── vcpkg.json (glfw3, glm)
├── README.md
├── .gitignore
│
├── RTR/  # Static library
│  ├── CMakeLists.txt
│  ├── include/  # Public headers only — exposed to Sandbox
│  │  └── RTR/
│  │     ├── RTR.h  # Public API gateway
│  │     ├── Core/
│  │     │  ├── Application.h
│  │     │  ├── EntryPoint.h
│  │     │  ├── Window.h
│  │     │  ├── LayerStack.h
│  │     │  ├── Layer.h
│  │     │  ├── Input.h
│  │     │  ├── Log.h
│  │     │  └── UUID.h
│  │     ├── Renderer/
│  │     │  ├── Renderer.h
│  │     │  ├── RenderCommand.h
│  │     │  ├── RendererAPI.h
│  │     │  ├── Shader.h
│  │     │  ├── Buffer.h
│  │     │  ├── VertexArray.h
│  │     │  ├── Texture.h
│  │     │  ├── Framebuffer.h
│  │     │  ├── Mesh.h
│  │     │  ├── Material.h
│  │     │  ├── Renderer3D.h
│  │     │  └── Camera.h
│  │     ├── Scene/
│  │     │  ├── Scene.h
│  │     │  ├── Entity.h
│  │     │  └── Components.h
│  │     ├── Assets/
│  │     │  ├── AssetManager.h
│  │     │  ├── MeshLoader.h
│  │     │  └── TextureLoader.h
│  │     └── ImGui/
│  │        └── ImGuiLayer.h
│  └── src/  # Private - .cpp files + platform headers
│     ├── RTR/  # API-Agnostic
│     │  ├── Core/ 
│     │  │  ├── Application.cpp
│     │  │  ├── LayerStack.cpp
│     │  │  ├── Log.cpp
│     │  │  ├── Layer.cpp
│     │  │  ├── UUID.cpp
│     │  │  └── Window.cpp
│     │  ├── Renderer/  
│     │  │  ├── Renderer.cpp
│     │  │  ├── RendererAPI.cpp
│     │  │  ├── RenderCommand.cpp
│     │  │  ├── Shader.cpp
│     │  │  ├── Buffer.cpp
│     │  │  ├── VertexArray.cpp
│     │  │  ├── Texture.cpp
│     │  │  ├── Framebuffer.cpp
│     │  │  ├── Mesh.cpp
│     │  │  ├── Material.cpp
│     │  │  ├── Renderer3D.cpp
│     │  │  └── Camera.cpp
│     │  ├── Scene/   
│     │  │  ├── Scene.cpp
│     │  │  └── Entity.cpp
│     │  ├── Assets/   
│     │  │  ├── AssetManager.cpp
│     │  │  ├── MeshLoader.cpp
│     │  │  └── TextureLoader.cpp
│     │  └── ImGui/
│     │     └── ImGuiLayer.cpp
│     └── Platform/
│        ├── OpenGL/ 
│        │  ├── OpenGLContext.h/cpp
│        │  ├── OpenGLRendererAPI.h/cpp
│        │  ├── OpenGLShader.h/cpp
│        │  ├── OpenGLBuffer.h/cpp
│        │  ├── OpenGLVertexArray.h/cpp
│        │  ├── OpenGLTexture.h/cpp
│        │  ├── OpenGLFramebuffer.h/cpp
│        │  └── OpenGLDebug.h/cpp
│        ├── Vulkan/
│        │  └── .gitkeep
│        └── Desktop/
│           └── GLFWWindow.h/cpp
│
├── Sandbox/  # Executable
│  ├── CMakeLists.txt
│  └── src/
│    ├── SandboxApp.cpp
│    └── layers/
│
├── vendor/  # Thrid-party source (imgui, stb, glad)
│  ├── CMakeLists.txt  # Manages vendor directories
│  ├── glad/
│  │  ├── include/
│  │  └── src/
│  ├── imgui/
│  └── stb/
│     └── stb_image.h/cpp
│
└── assets/ 
   ├── shaders/
   │  ├── OpenGl/  # GLSL
   │  │  └── .gitkeep
   │  └── Vulkan/  # SPIR-V
   │     └── .gitkeep
   ├── models/  #(.gltf .bin)
   │  └── .gitkeep
   └── textures/
      └── .gitkeep
```

### Structure Ideas/goals

* Sandbox uses RTR/include/RTR/RTR.h as public inferface
* RTR/src/RTR/ has api-agnostic logic, defines interfaces and uses: glm, spdlog
* RTR/src/Platform/ implements the interfaces and uses glad, glfw, stb
* RTR/src/Platform/ can only reach RTR/src/RTR in RendererAPI.cpp for the rendering API or Window.cpp
