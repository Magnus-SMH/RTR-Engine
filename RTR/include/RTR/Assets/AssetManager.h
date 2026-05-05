#pragma once

#include "RTR/Core/UUID.h"

#include <string>
#include <unordered_map>
#include <mutex>

namespace RTR
{
	enum class AssetState
	{
		Unloaded,
		Loading,
		Ready,
		Failed
	};

	struct AssetInfo
	{
		UUID uuid;
		std::string sourcePath;
		AssetState state = AssetState::Unloaded;
	};

	class AssetManager
	{
	public:
		UUID RegisterAsset(const std::string& path);

		void MarkLoading(UUID uuid);
		void MarkReady(UUID uuid);
		void MarkFailed(UUID uuid);

		AssetState GetState(UUID uuid) const;
		const AssetInfo* getRecord(UUID uuid) const;

		UUID GetIdForPath(const std::string& path) const;
	private:
		mutable std::mutex m_Mutex;
		std::unordered_map<UUID, AssetInfo> m_Records;
		std::unordered_map<std::string, UUID> m_PathIndex;
	};
}