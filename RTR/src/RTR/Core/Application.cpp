#include "RTR/Core/Application.h"

#include <iostream>
#include <cassert>
#include <filesystem>

#include "RTR/Core/Log.h"


namespace RTR
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& spec)
		: m_Spec(spec)
	{
		assert(!s_Instance);
		s_Instance = this;

		RTR_CORE_INFO("Application name: {0}", m_Spec.Name);

		if (!m_Spec.WorkingDirectory.empty())
		{
			std::filesystem::path workDir(m_Spec.WorkingDirectory);

			if (std::filesystem::exists(workDir))
			{
				std::filesystem::current_path(workDir);
			}
			else
			{
				RTR_ERROR("Working directory doesnt exist: {0}", m_Spec.WorkingDirectory);
			}
		}
	}


	Application::~Application()
	{
		s_Instance = nullptr;
	}

	void Application::Run()
	{
		while (m_Running)
		{

		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	Application& Application::Get()
	{
		assert(s_Instance);
		return *s_Instance;
	}
}