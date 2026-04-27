#include "RTR/ImGui/ImGuiLayer.h"

#ifndef RTR_HEADLESS

#include "RTR/Core/Application.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace RTR
{

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool showPerformance = true;
		static bool showInspector = true;

		ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Engine Debugger", nullptr, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("View"))
				{
					ImGui::MenuItem("Performance Metrics", nullptr, &showPerformance);
					ImGui::MenuItem("System Inspector", nullptr, &showInspector);
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			if (showPerformance)
			{
				ImGui::SeparatorText("Performance Metrics");

				static int offset = 0;
				static float frameGraph[300] = { 0 };
				static float tickGraph[300] = { 0 };

				const EngineStats& stats = m_App->GetStats();
				float renderDeltaMs = stats.renderDeltaMs.load();
				float tickWorkTimeMs = stats.tickWorkTimeMs.load();
				float tickTargetMs = stats.tickTargetMs.load();

				//Frame graph
				frameGraph[offset] = renderDeltaMs;
				char frameLabel[32];
				snprintf(frameLabel, sizeof(frameLabel), "Delta: %.2f ms", renderDeltaMs);
				ImGui::PlotLines("##FrameGraph", frameGraph, 300, offset, frameLabel, 0.0f, 50.0f, ImVec2(-1, 100));

				//Tick graph
				tickGraph[offset] = tickWorkTimeMs;
				char tickLabel[32];
				snprintf(tickLabel, sizeof(tickLabel), "Work: %.3f / Target: %.3f ms", tickWorkTimeMs, tickTargetMs);
				ImGui::PlotLines("##TickGraph", tickGraph, 300, offset, tickLabel, 0.0f, tickTargetMs + 1.0f, ImVec2(-1, 100));
				offset = (offset + 1) % 300;

			}


			if (showInspector)
			{
				ImGui::SeparatorText("System Inspector");

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

					//Checks compile logging level flag, removes the unavailable levels from the runtime levels
					const char* allLevels[] = { "Trace", "Debug", "Info", "Warn", "Error", "Critical", "Off" };
					int availableCount = 7 - SPDLOG_ACTIVE_LEVEL;
					static int currentLevelIdx = 0;
					ImGui::SetNextItemWidth(-FLT_MIN);
					if (ImGui::Combo("##LogLevel", &currentLevelIdx, &allLevels[SPDLOG_ACTIVE_LEVEL], availableCount))
					{
						Log::SetLevel(static_cast<Log::Level>(currentLevelIdx + SPDLOG_ACTIVE_LEVEL));
					}


					//ImGui input capture debug
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("ImGui");
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
		//Viewport broke with the multithreading. OS keeps event handeling and Renderer keeps Renderer context,
		//ImGui needs both on same thread to work. I think viewports can be implemented in the Editor at a later date.

		ImGui::StyleColorsDark();

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
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

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
	}
}
#endif