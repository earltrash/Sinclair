#pragma once
#include "pch.h"
#include "IAssetProvider.h"
#include "ResourceManager.h"
#include "Scene.h"

class AssetProvider : public IAssetProvider
{
public:
    explicit AssetProvider(std::shared_ptr<ResourceManager> rm)
        : m_manager(rm) {
    }

    ~AssetProvider() = default;

protected:

    ComPtr<ID2D1Bitmap1> GetTexture(const  std::string& Info) override {
        return m_manager->GetTexture(Info); //stage도 같이 넣을 수 있음. 
    }

    std::vector<Clip_Asset> GetClips(const std::string& Info) override {
        return m_manager->GetClips(Info);
    }


private:
    std::shared_ptr<ResourceManager> m_manager;
};