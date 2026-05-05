#include "RTR/Core/UUID.h"

#include <random>

namespace RTR
{
	thread_local std::mt19937_64 s_Engine(std::random_device{}());
	thread_local std::uniform_int_distribution<uint64_t> s_Distribution{
		1,
		std::numeric_limits<uint64_t>::max()
	};

	UUID::UUID()
		: m_UUID(s_Distribution(s_Engine))
	{
	}
	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{
	}
}
