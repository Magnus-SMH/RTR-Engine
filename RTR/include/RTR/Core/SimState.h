#pragma once

#include <array>
#include <atomic>

#include <cstdint>

namespace RTR
{
	struct SimState
	{
		double   simTimeSeconds = 0.0;
		float    tickDelta = 0.0f;
		uint64_t tickIndex = 0;
		bool     isPaused = false;
	};

	template<typename T>
	class TripleBuffer
	{
	public:
		TripleBuffer()
			: m_Back(1 | NewData),
			m_WriteSlot(0),
			m_ReadSlot(2)
		{
		}

		T& GetWriteSlot() noexcept { return m_Slots[m_WriteSlot]; }

		void Publish() noexcept
		{
			const int desired = m_WriteSlot | NewData;
			const int oldBack = m_Back.exchange(desired, std::memory_order_acq_rel);
			m_WriteSlot = oldBack & SlotMask;
		}

		bool Fetch() noexcept
		{
			int back = m_Back.load(std::memory_order_acquire);
			if (!(back & NewData)) return false;

			const int desired = m_ReadSlot;
			while (!m_Back.compare_exchange_weak(
				back, desired,
				std::memory_order_acq_rel,
				std::memory_order_acquire))
			{
				if (!(back & NewData)) return false;
			}
			m_ReadSlot = back & SlotMask;
			return true;
		}

		const T& Get() const noexcept { return m_Slots[m_ReadSlot]; }

	private:
		static constexpr int NewData = (1 << 2);
		static constexpr int SlotMask = ~NewData;

		std::array<T, 3> m_Slots{};
		std::atomic<int> m_Back;
		int m_WriteSlot;
		int m_ReadSlot;
	};
}