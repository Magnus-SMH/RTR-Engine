#include "RTR/ImGui/ImGuiLayer.h"

#ifndef RTR_HEADLESS

#include "RTR/Core/Application.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace RTR
{

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	void ImGuiLayer::OnImGuiRender()
	{
		// 1. Setup a Main "Dashboard" Window
		ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Engine Debugger", nullptr, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("View"))
				{
					// Logic to toggle sub-windows could go here
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			// 2. Performance Section (Using a Collapsing Header)
			if (ImGui::CollapsingHeader("Performance Metrics", ImGuiTreeNodeFlags_DefaultOpen))
			{
				static int offset = 0;

 				static float frameGraph[300] = { 0 };
 				static float tickGraph[300] = { 0 };


				const EngineStats& stats = m_App->GetStats();
				float renderDeltaMs = stats.renderDeltaMs.load();
				float tickWorkTimeMs = stats.tickWorkTimeMs.load();
				float tickTargetMs = stats.tickTargetMs.load();

				// Frame graph
				frameGraph[offset] = renderDeltaMs;
				char frameLabel[32];
				snprintf(frameLabel, sizeof(frameLabel), "Delta: %.2f ms", renderDeltaMs);
				ImGui::PlotLines("##FrameGraph", frameGraph, 300, offset, frameLabel, 0.0f, 50.0f, ImVec2(-1, 100));

				// Tick graph
				tickGraph[offset] = tickWorkTimeMs;
				char tickLabel[32];
				snprintf(tickLabel, sizeof(tickLabel), "Work: %.3f / Target: %.3f ms", tickWorkTimeMs, tickTargetMs);
				ImGui::PlotLines("##TickGraph", tickGraph, 300, offset, tickLabel, 0.0f, tickTargetMs + 1.0f, ImVec2(-1, 100));
				offset = (offset + 1) % 300;

			}

			ImGui::Separator();

			if (ImGui::CollapsingHeader("System Inspector", ImGuiTreeNodeFlags_DefaultOpen))
			{
				static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;

				if (ImGui::BeginTable("SystemsTable", 2, flags))
				{
					ImGui::TableSetupColumn("System");
					ImGui::TableSetupColumn("Settings / Status");
					ImGui::TableHeadersRow();


					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Logging");
					ImGui::TableSetColumnIndex(1);

					const char* allLevels[] = { "Trace", "Debug", "Info", "Warn", "Error", "Critical", "Off" };
					int availableCount = 7 - SPDLOG_ACTIVE_LEVEL;
					static int currentLevelIdx = 0;
					ImGui::SetNextItemWidth(-FLT_MIN);
					if (ImGui::Combo("##LogLevel", &currentLevelIdx, &allLevels[SPDLOG_ACTIVE_LEVEL], availableCount))
					{
						Log::SetLevel(static_cast<Log::Level>(currentLevelIdx + SPDLOG_ACTIVE_LEVEL));
					}



					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Input Capture");
					ImGui::TableSetColumnIndex(1);

					ImGuiIO& io = ImGui::GetIO();
					ImGui::Text("Mouse %s", io.WantCaptureMouse ? "True" : "False");
					ImGui::Text("Keyboard %s", io.WantCaptureKeyboard ? "True" : "False");
					ImGui::EndTable();
				}
			}
 		}
 		ImGui::End();
	}

	void ImGuiLayer::OnAttach()
	{
		m_App = &Application::Get();
	}

	void ImGuiLayer::InitForRender(void* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
 		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
 		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
// 		Viewport broke with the multithreading. Fix: Override imgui events with the engines event system
// 		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window);

		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplGlfw_NewFrame();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
	}

	//
	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(
			static_cast<float>(app.GetWindow().GetWidth()),
			static_cast<float>(app.GetWindow().GetHeight())
		);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup);
		}
	}
}
#endif