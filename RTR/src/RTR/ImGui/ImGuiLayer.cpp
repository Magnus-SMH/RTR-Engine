#include "RTR/ImGui/ImGuiLayer.h"

#ifndef RTR_HEADLESS

#include "RTR/Core/Application.h"
#include "RTR/Core/Window.h"

#include <imgui.h>
#include <implot.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

int FPS = 0;

struct CircularBufferGetter
{
	const float* buf;
	int offset;
	int size;
};

ImPlotPoint GetCircularValue(int idx, void* user_data)
{
	auto* data = static_cast<CircularBufferGetter*>(user_data);
	int i = (data->offset + idx) % data->size;
	return ImPlotPoint((double)idx, (double)data->buf[i]);
}

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
				static float tickTargetGraph[300] = { 0 };


				const EngineStats& stats = m_App->GetStats();
				float renderDeltaMs = stats.renderDeltaMs.load();
				float tickWorkTimeMs = stats.tickWorkTimeMs.load();
				float tickTargetMs = stats.tickTargetMs.load();


				//Frame graph
				frameGraph[offset] = renderDeltaMs;
				if (offset % 20 == 0)
				{
					FPS = renderDeltaMs > 0.0f ? static_cast<int>(1000.0f / renderDeltaMs) : 0;
				}

				ImGui::Text("Delta: %.2f ms  FPS: %d", renderDeltaMs, FPS);

				ImPlot::BeginPlot("##FrameGraph", ImVec2(-1, 100), ImPlotFlags_NoLegend | ImPlotFlags_NoInputs);
				ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, 0);
				ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, 50.0f, ImGuiCond_Always);

				CircularBufferGetter frameGetter = { frameGraph, offset, 300 };
				ImPlot::PlotLineG("Frame Time", GetCircularValue, &frameGetter, 300);

				ImPlot::EndPlot();

				//Tick graph
				tickGraph[offset] = tickWorkTimeMs;
				tickTargetGraph[offset] = tickTargetMs;

				ImGui::Text("Tick Work: %.2f ms  Target: %.2f ms", tickWorkTimeMs, tickTargetMs);
				ImPlot::BeginPlot("##TickGraph", ImVec2(-1, 100), ImPlotFlags_NoLegend | ImPlotFlags_NoInputs);
				ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, 0);
				ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, tickTargetMs * 1.1f, ImGuiCond_Always);

				CircularBufferGetter tickGetter = { tickGraph, offset, 300 };
				ImPlot::PlotLineG("Work Time", GetCircularValue, &tickGetter, 300);

				CircularBufferGetter tickTargetGetter = { tickTargetGraph, offset, 300 };
				ImPlot::PlotLineG("Target Time", GetCircularValue, &tickTargetGetter, 300);

				ImPlot::EndPlot();
				
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

	void ImGuiLayer::OnEvent(EventContext& ctx)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (ctx.IsMouseEvent() && io.WantCaptureMouse)
			ctx.Handled = true;

		if (ctx.IsKeyEvent() && io.WantCaptureKeyboard)
			ctx.Handled = true;
	}

	void ImGuiLayer::InitForRender(void* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();

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
		ImPlot::DestroyContext();
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