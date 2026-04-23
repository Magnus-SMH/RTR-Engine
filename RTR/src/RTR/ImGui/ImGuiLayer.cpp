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
				float deltaTime = m_App->GetDeltaTime();
				static float frameGraph[300] = { 0 };
				static int offset = 0;
				frameGraph[offset] = deltaTime * 1000.0f;
				
				float tickWorkTime = m_App->GetTickWorkTime() * 1000.0f;
				static float tickGraph[300] = { 0 };
				tickGraph[offset] = tickWorkTime;

 				const float tickTarget = m_App->GetTickTarget();


				char frameLabel[32];
				sprintf(frameLabel, "Avg: %.2f ms", deltaTime * 1000.0f);
				ImGui::PlotLines("##FrameGraph", frameGraph, 300, offset, frameLabel, 0.0f, 50.0f, ImVec2(-1, 100));


				char tickLabel[32];
				snprintf(tickLabel, sizeof(tickLabel), "Work: %.3f / Target: %.3f", tickWorkTime, tickTarget);
				ImGui::PlotLines("##TickGraph", tickGraph, 300, offset, tickLabel, 0.0f, (tickTarget + 1.0f), ImVec2(-1, 100));


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
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, false);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(EventContext& ctx)
	{
		if (!m_BlockEvents) return;

		ImGuiIO& io = ImGui::GetIO();

		// Consume mouse/keyboard events if ImGui wants them
		if (ctx.IsMouseEvent() && io.WantCaptureMouse)
		{
			if (!std::holds_alternative<MouseMovedEvent>(ctx.Event))
				RTR_CORE_DEBUG("Mouse event captured by ImGui");
			ctx.Handled = true;
		}

		if (ctx.IsKeyEvent() && io.WantCaptureKeyboard)
		{
			RTR_CORE_DEBUG("Key event captured by ImGui");
			ctx.Handled = true;
		}
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