#pragma once
#include "pch.h"
#include "AnimationClip.h" 
#include "AsepriteParser.h"
#include "Renderer.h"

using namespace std;

using wsg = std::wstring;
//using path = std::filesystem::path;
using path = std::filesystem::path;

//1. Scene단위로  asset불러오기 가능
//2. asset 재사용 가능
//3. 


struct Clip_Asset {
    std::string Name;
    std::string Ani_Name; // 없어질 예정 
    AnimationClip clip;
    std::vector<string> whichScene; //얘는 뭐. 있을듯 
};

class ResourceManager //Core Initalize 단계에서 Asset 업로드 하긴 함. 얘는 asset만 만들어서 Core Pointer 에 넘겨주기만 하면 되니깐 unorded_map 
{
    using AnimationClips = std::vector<std::pair<std::string, AnimationClip>>; //obj와 clip의 집합. 
private:
    ResourceManager() = default;
    ~ResourceManager() = default;
public:
    static ResourceManager& Get();

    //static ResourceManager& Get();
    void AssetLoad(static D2DRenderer& renderer, const std::string& directory);
    void LoadTexture(static D2DRenderer& renderer, wsg name, path path);

    ComPtr<ID2D1Bitmap1> GetTexture(const string& Info);
    void RegisterClip(const Clip_Asset& asset);
    std::vector<Clip_Asset> GetClips(const string& Info);

    void Clean();

    std::string GetAbsoluteResourcePathA();

private:
    AsepriteParser ap;
    std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> m_textures;
    std::wstring to_wstring_hash(const std::string& s);
    //두 번에 걸쳐서 값을 가져옴. Scene -> Name -> Asset 순으로

    std::unordered_map<std::string, Clip_Asset> allClipAssets;
    std::unordered_map<string, std::vector<std::string>> sceneToClipKeys;
};