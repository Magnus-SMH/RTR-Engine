#pragma once

#include <vector>

#include "RTR/Core/Layer.h"
#include "RTR/Core/Base.h"

namespace RTR
{
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack() = default;

		RTR_NON_COPYABLE(LayerStack);

		void PushLayer(std::unique_ptr<Layer> layer);
		void PushOverlay(std::unique_ptr<Layer> overlay);

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		void SuspendLayer(Layer* layer, bool releaseResources = false);
		void ResumeLayer(Layer* layer);

		auto begin() { return m_Layers.begin(); }
		auto end() { return m_Layers.end(); }
		auto rbegin() { return m_Layers.rbegin(); }
		auto rend() { return m_Layers.rend(); }

		auto begin() const { return m_Layers.cbegin(); }
		auto end() const { return m_Layers.cend(); }

	private:
		std::vector<std::unique_ptr<Layer>> m_Layers;
		size_t m_LayerInsertIndex = 0;
	};
}