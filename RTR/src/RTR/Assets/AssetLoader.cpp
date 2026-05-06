#include "RTR/Assets/AssetLoader.h"



namespace RTR
{
    AssetLoader::AssetLoader(AssetManager& manager)
        : m_AssetManager(manager)
    {
    }

    AssetLoader::~AssetLoader()
    {
        Shutdown();
    }

#ifndef RTR_HEADLESS
    void AssetLoader::RequestMeshLoad(UUID uuid, const std::string& path)
    {
        m_AssetManager.MarkLoading(uuid);

        std::thread assetThread([this, uuid, path]()
            {
                auto result = MeshLoader::Load(path, uuid);

                if (result.has_value())
                {
                    {
                        std::scoped_lock lock(m_QueueMutex);
                        m_UploadQueue.push_back(std::move(*result));
                    }
                    m_AssetManager.MarkReady(uuid);
                }
                else
                {
                    m_AssetManager.MarkFailed(uuid);
                }
            });

        std::scoped_lock lock(m_ThreadMutex);
        m_Threads.push_back(std::move(assetThread));
    }
#endif

    void AssetLoader::ProcessUploadQueue(const std::function<void(const CPUMesh&)>& uploadFn)
    {
        std::vector<CPUMesh> toUpload;
        {
            std::scoped_lock lock(m_QueueMutex);
            toUpload.swap(m_UploadQueue);
        }

        for (const auto& mesh : toUpload)
            uploadFn(mesh);
    }

    void AssetLoader::Shutdown()
    {
        std::scoped_lock lock(m_ThreadMutex);
        for (auto& assetThread : m_Threads)
            if (assetThread.joinable()) assetThread.join();
        m_Threads.clear();
    }
}