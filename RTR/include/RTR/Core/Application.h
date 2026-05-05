#pragma once

#ifndef RTR_HEADLESS
#include "RTR/Core/WindowSpec.h"
#include "RTR/ImGui/ImGuiLayer.h"
#include "RTR/Renderer/GPUMeshCache.h"
#endif

#include "RTR/Core/Base.h"
#include "RTR/Core/LayerStack.h"
#include "RTR/Core/Events.h"
#include "RTR/Core/EventQueue.h"
#include "RTR/Core/SimState.h"
#include "RTR/Core/TripleBuffer.h"
#include "RTR/Scene/Scene.h"
#include "RTR/Assets/AssetManager.h"
#include "RTR/Assets/AssetLoader.h"

#include <latch>
#include <thread>
#include <atomic>

int main(int argc, char** argv);

namespace RTR
{
	class ImGuiLayer;
	class Window;

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			if (index >= Count)
				return nullptr;	//Todo: Macro 
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "RTR Application";
		std::string WorkingDirectory;
#ifndef RTR_HEADLESS
		WindowSpecification Window;
#endif
		ApplicationCommandLineArgs CommandLineArgs;
	};

	struct EngineStats
	{
		//Server stats
		std::atomic<float> renderDeltaMs{ 0.0f };
		std::atomic<float> tickWorkTimeMs{ 0.0f };
		std::atomic<float> tickTargetMs{ 0.0f };
		std::atomic<uint64_t> tickIndex{ 0 };
	};

	class Application 
	{
		friend int ::main(int argc, char** argv);
	public:
		explicit Application(const ApplicationSpecification& spec = {});
		virtual ~Application();

		RTR_NON_COPYABLE(Application);

		void Close();

		void PushLayer(std::unique_ptr<Layer> layer);
		void PushOverlay(std::unique_ptr<Layer> overlay);

#ifndef RTR_HEADLESS
		Window& GetWindow() { return *m_Window; }
		const Window& GetWindow() const { return *m_Window; }

		GPUMeshCache* GetGPUMeshCache() { return m_GPUMeshCache.get(); }
#endif

		const ApplicationSpecification& GetSpec() const { return m_Spec; }
		static Application& Get();

		EngineStats& GetStats() { return m_Stats; }
		const EngineStats& GetStats() const { return m_Stats; }

		AssetManager& GetAssetManager() { return m_AssetManager; }
		AssetLoader& GetAssetLoader() { return m_AssetLoader; }
		Scene& GetScene() { return m_Scene; }

		EventQueue& GetRenderEventQueue() { return m_RenderEventQueue; }
		EventQueue& GetSimEventQueue() { return m_SimEventQueue; }

	private:

		void Run();

		ApplicationSpecification m_Spec;
		EngineStats m_Stats;
		LayerStack m_LayerStack;
		std::atomic<bool> m_Running{ true };
		std::atomic<bool> m_Paused{ false };

		//ImGui needs the poll events in OSThread to shutdown RendererThread cleanly
		std::atomic<bool> temp_RendererFinished{ false };

		static Application* s_Instance;

		TripleBuffer<SimState> m_SimStateBuffer;

		Scene m_Scene;

		AssetManager m_AssetManager;
		AssetLoader m_AssetLoader{ m_AssetManager };

#ifdef RTR_HEADLESS
		void HeadlessRun();
#else
		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		void SimThreadRun();
		void OSThreadRun();
		void RenderThreadRun();

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer = nullptr;

		std::unique_ptr<GPUMeshCache> m_GPUMeshCache;

		std::atomic<bool> m_Minimized{ false };

		std::latch m_RenderReady{ 1 };

		EventQueue m_RenderEventQueue;
		EventQueue m_SimEventQueue;
		
		std::thread m_SimThread;
		std::thread m_RenderThread;
#endif

	};
}