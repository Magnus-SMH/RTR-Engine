#pragma once

#include "RTR/Assets/AssetManager.h"
#ifndef RTR_HEADLESS
#include "RTR/Assets/MeshLoader.h"
#endif
#include "RTR/Assets/MeshData.h"

#include <thread>
#include <mutex>
#include <vector>
#include <atomic>

namespace RTR
{
    class AssetLoader
    {
    public:
        explicit AssetLoader(AssetManager& manager);
        ~AssetLoader();

#ifndef RTR_HEADLESS //For now, until i make a custom baked asset for servers
        void RequestMeshLoad(UUID uuid, const std::string& path);
#endif

        void ProcessUploadQueue(const std::function<void(const CPUMesh&)>& uploadFn);

        void Shutdown();

    private:
        AssetManager& m_AssetManager;

        std::mutex m_QueueMutex;
        std::vector<CPUMesh> m_UploadQueue;

        std::mutex m_ThreadMutex;
        std::vector<std::thread> m_Threads;
    };
}