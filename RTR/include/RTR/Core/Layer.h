#pragma once

#include "RTR/Core/Events.h"
#include "RTR/Core/Base.h"
#include "RTR/Core/SimState.h"

namespace RTR
{
	enum class LayerAffinity
	{
		Sim,     //server,physics,game logic
		Render,  //graphicsAPI rendering, ImGui
		Both
	};

	enum class LayerState
	{
		Detatched,
		Attached,
		Suspended,
	};

	class Layer
	{
	public:
		explicit Layer(std::string name = "Layer")
			: m_DebugName(std::move(name)) {
		}

		virtual ~Layer() = default;

		RTR_NON_COPYABLE(Layer);

		virtual LayerAffinity GetAffinity() const { return LayerAffinity::Both; }

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnSuspend(bool /*ReleaseResources*/) {}
		virtual void OnResume() {}

		virtual void OnPollEvents(float /*deltaTime*/) {}
		virtual void OnTick(float /*tickDelta*/) {}
		virtual void OnEvent(EventContext& /*ctx*/) {}
		virtual void OnRender(const SimState& /*snapshot*/, float /*deltaTime*/) {}
#ifndef RTR_HEADLESS
		virtual void OnImGuiRender() {}
#endif

		// Todo: implement
		virtual void Transition() {}
		virtual void Suspend() {}

		LayerState GetState() const { return m_State; }
		bool IsAttached() const { return m_State == LayerState::Attached; }
		bool IsSuspended() const { return m_State == LayerState::Suspended; }
		const std::string& GetName() const { return m_DebugName; }
	private:
		friend class LayerStack;
		void SetState(LayerState state) { m_State = state; }

		std::string m_DebugName;
		LayerState m_State = LayerState::Detatched;
	};
}