#pragma once

#include <queue>
#include <mutex>
#include "RTR/Core/Events.h"

namespace RTR
{
	class EventQueue
	{
	public:
		void Push(Event event)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push(std::move(event));
		}

		void Flush(const std::function<void(Event&)>& executer)
		{
			std::queue<Event> tempQueue;
			{
				std::scoped_lock lock(m_Mutex);
				std::swap(tempQueue, m_Queue);
			}
			while (!tempQueue.empty())
			{
				executer(tempQueue.front());
				tempQueue.pop();
			}
		}

		bool Empty() const
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.empty();
		}

	private:
		mutable std::mutex m_Mutex;
		std::queue<Event> m_Queue;
	};
}