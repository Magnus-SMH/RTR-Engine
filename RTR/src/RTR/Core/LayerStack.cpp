#include "RTR/Core/LayerStack.h"

#include <algorithm>
#include <memory>

namespace RTR
{
	void LayerStack::PushLayer(std::unique_ptr<Layer> layer)
	{
		layer->SetState(LayerState::Attached);
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, std::move(layer));
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(std::unique_ptr<Layer> overlay)
	{
		overlay->SetState(LayerState::Attached);
		m_Layers.emplace_back(std::move(overlay));
	}

	void LayerStack::SuspendLayer(Layer* layer, bool releaseResources)
	{
		auto it = std::find_if(
			m_Layers.begin(),
			m_Layers.begin() + m_LayerInsertIndex,
			[layer](const std::unique_ptr<Layer>& ptr) { return ptr.get() == layer; });

		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			(*it)->SetState(LayerState::Suspended);
			(*it)->OnSuspend(releaseResources);
		}
	}

	void LayerStack::ResumeLayer(Layer* layer)
	{
		auto it = std::find_if(
			m_Layers.begin(),
			m_Layers.begin() + m_LayerInsertIndex,
			[layer](const std::unique_ptr<Layer>& ptr) { return ptr.get() == layer; });

		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			(*it)->SetState(LayerState::Attached);
			(*it)->OnResume();
		}
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find_if(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex,
			[layer](const std::unique_ptr<Layer>& ptr) {
				return ptr.get() == layer;
			});

		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			(*it)->OnDetach();
			(*it)->SetState(LayerState::Detatched);
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find_if(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(),
			[overlay](const std::unique_ptr<Layer>& ptr) {
				return ptr.get() == overlay;
			});

		if (it != m_Layers.end())
		{
			(*it)->OnDetach();
			(*it)->SetState(LayerState::Detatched);
			m_Layers.erase(it);
		}
	}

}
