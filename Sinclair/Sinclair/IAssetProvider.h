#pragma once
#include "pch.h"
#include "ResourceManager.h"

class IAssetProvider //ResourceManager에 직접 접근 안하기 위함
{
public:
    virtual ComPtr<ID2D1Bitmap1> GetTexture(const  std::string& Info) = 0;
    virtual  std::vector<Clip_Asset> GetClips(const  std::string& Info) = 0;
    virtual ~IAssetProvider() {}
};