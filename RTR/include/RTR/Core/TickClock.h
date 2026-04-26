#pragma once

#include <chrono>
#include <cstdint>
#include <thread>

namespace RTR
{
	class TickClock
	{
	public:
		using Clock = std::chrono::steady_clock;

		explicit TickClock(double targetHz = 60.0, int maxCatchup = 5) noexcept
			: m_TickDuration(1.0 / targetHz),
			m_MaxCatchup(maxCatchup),
			m_Accumulator(0.0),
			m_LastTime(Clock::now()),
			m_TickIndex(0)
		{
		}

		int Update() noexcept
		{
			const Clock::time_point now = Clock::now();
			const double dt = std::chrono::duration<double>(now - m_LastTime).count();
			m_LastTime = now;
			m_Accumulator += dt;

			const double maxAccum = m_TickDuration * static_cast<double>(m_MaxCatchup);
			if (m_Accumulator > maxAccum)
				m_Accumulator = maxAccum;

			int ticks = 0;
			while (m_Accumulator >= m_TickDuration && ticks < m_MaxCatchup)
			{
				m_Accumulator -= m_TickDuration;
				++ticks;
			}
			return ticks;
		}

		void SleepUntilNextTick() const noexcept
		{
			const double remaining = m_TickDuration - m_Accumulator;
			if (remaining > 0.001)
			{
				const auto sleepFor = std::chrono::duration<double>(remaining * 0.8);
				std::this_thread::sleep_for(sleepFor);
			}
		}

		float GetTickDelta() const noexcept { return static_cast<float>(m_TickDuration); }
		double GetTickDurationD() const noexcept { return m_TickDuration; }
		double TimeUntilNextTick() const noexcept { return m_TickDuration - m_Accumulator; }
		uint64_t NextTickIndex()  noexcept { return m_TickIndex++; }
		uint64_t GetTickIndex()   const noexcept { return m_TickIndex; }

	private:
		double m_TickDuration;
		int m_MaxCatchup;
		double m_Accumulator;
		Clock::time_point m_LastTime;
		uint64_t m_TickIndex;
	};
}