# RTR-Engine
Real-Time 3D Only Renderer Engine/Sandbox

Inspiration from:  
* [TheCherno](https://www.youtube.com/@TheCherno):
* Game Engine series [(YT-Playlist)](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT)   -   [Github: Hazel](https://github.com/TheCherno/Hazel) 
* C++ Application Architecture - A Mini-Series. [(YT-Playlist)](https://www.youtube.com/playlist?list=PLlrATfBNZ98cpX2LuxLnLyLEmfD2FPpRA)   -   [Github: Architecture](https://github.com/TheCherno/Architecture)

* [OGLDEV](https://www.youtube.com/@OGLDEV):
* All OpenGl Tutorials [(YT-Playlist)](https://www.youtube.com/playlist?list=PLA0dXqQjCx0S04ntJKUftl6OaOgsiwHjA)   -     [Github](https://github.com/emeiri/ogldev)

# Unfinished Directory Structure
mostly empty placeholder files to plan the structure and to implement CMake and vckpg from the beginning.
```bash
RTR-Engine/
├── CMakeLists.txt
├── vcpkg.json
├── README.md
├── .gitignore
│
├── RTR/  # Static library
│  ├── CMakeLists.txt
│  ├── include/  # Public headers only — exposed to Sandbox via RTR.h
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
│  └── src/  # Private .cpp files + platform headers
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
├── Sandbox/  # Simple Executable
│  ├── CMakeLists.txt
│  └── src/
│    ├── SandboxApp.cpp
│    └── layers/
│
├── vendor/  # Thrid-party sources (imgui, stb, glad)
│  ├── CMakeLists.txt  # Manages vendor directories
│  ├── glad/
│  │  ├── include/
│  │  └── src/
│  ├── imgui/
│  └── stb/
│     └── stb_image.h/cpp
│
└── assets/ # To be moved. Dont want global assets
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

### Structure Ideas/goals:

* Sandbox uses RTR/include/RTR/RTR.h as public inferface
* RTR/src/RTR/ has api-agnostic logic, defines interfaces and uses: glm, spdlog
* RTR/src/Platform/ implements the interfaces and uses glad, glfw, stb
* RTR/src/Platform/ can only reach RTR/src/RTR in RendererAPI.cpp for the rendering API or Window.cpp

## Naming Convention im trying to follow:  
Allman brace style  

Everything is PascalCase, except:  

local var and function params is camelCase  

# Prefixes:  
m_   member var  
s_   static var  
k_   constant  
g_   global var  
T    emplate type param  

# Shader var:  
u_   uniform  
v_   varying  
a_   attribute  
o_   output  
r_   resource  

## Note to self:
* later, make RTR-Editor, split the root assests into engine and editor specific assets.
* add yaml-cpp into vcpkg
* Test framework (Catch2?)


Windows 11 x64 | MSVS | C++23 | Opengl 4.6 | GLFW | GLM | Glad 2 | imGui | GLTF | spdlog | enTT | tinygltf |
