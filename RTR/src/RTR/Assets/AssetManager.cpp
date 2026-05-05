#include "RTR/Assets/AssetManager.h"

namespace RTR
{
	UUID AssetManager::RegisterAsset(const std::string& path)
	{
		std::scoped_lock lock(m_Mutex);

		auto asset = m_PathIndex.find(path);
		if (asset != m_PathIndex.end())
			return asset->second;

		AssetInfo info;
		info.uuid = UUID();
		info.sourcePath = path;
		info.state = AssetState::Unloaded;

		m_PathIndex[path] = info.uuid;
		m_Records[info.uuid] = info;

		RTR_CORE_DEBUG("AssetManager: registered '{}' -> {}", path, info.uuid.ToString());
		return info.uuid;
	}

	void AssetManager::MarkLoading(UUID uuid)
	{
		std::scoped_lock lock(m_Mutex);
		auto asset = m_Records.find(uuid);
		RTR_CORE_ASSERT(asset != m_Records.end(), "Unknown UUID");
		asset->second.state = AssetState::Loading;
	}

	void AssetManager::MarkReady(UUID uuid)
	{
		std::scoped_lock lock(m_Mutex);
		auto asset = m_Records.find(uuid);
		RTR_CORE_ASSERT(asset != m_Records.end(), "Unknown UUID");
		asset->second.state = AssetState::Ready;
	}

	void AssetManager::MarkFailed(UUID uuid)
	{
		std::scoped_lock lock(m_Mutex);
		auto asset = m_Records.find(uuid);
		RTR_CORE_ASSERT(asset != m_Records.end(), "Unknown UUID");
		asset->second.state = AssetState::Failed;
		RTR_CORE_ERROR("AssetManager: failed loading of '{}'", asset->second.sourcePath);
	}

	AssetState AssetManager::GetState(UUID uuid) const
	{
		std::scoped_lock lock(m_Mutex);
		auto asset = m_Records.find(uuid);
		return asset != m_Records.end() ? asset->second.state : AssetState::Failed;
	}

	const AssetInfo* AssetManager::getRecord(UUID uuid) const
	{
		std::scoped_lock lock(m_Mutex);
		auto asset = m_Records.find(uuid);
		return asset != m_Records.end() ? &asset->second : nullptr;
	}

	UUID AssetManager::GetIdForPath(const std::string& path) const
	{
		std::scoped_lock lock(m_Mutex);
		auto asset = m_PathIndex.find(path);
		return asset != m_PathIndex.end() ? asset->second : UUID::Null();
	}

}