#pragma once

#include <string>
#include <cstdint>

namespace RTR
{
	class UUID
	{
	public:
		UUID();
		explicit UUID(uint64_t uuid);
		UUID(const UUID& other) = default;
		UUID& operator=(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }

		bool operator==(const UUID& other) const { return m_UUID == other.m_UUID; }
		bool operator!=(const UUID& other) const { return m_UUID != other.m_UUID; }

		static UUID Null() { return UUID(0); }
		bool IsNull() const { return m_UUID == 0; }

		std::string ToString() const { return std::to_string(m_UUID); }

	private:
		uint64_t m_UUID = 0;
	};
}

namespace std
{
	template<>
	struct hash<RTR::UUID>
	{
		size_t operator()(const RTR::UUID& uuid) const noexcept
		{
			return static_cast<size_t>(static_cast<uint64_t>(uuid));
		}
	};
}