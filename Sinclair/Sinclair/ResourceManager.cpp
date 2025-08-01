#include "pch.h"
#include "ResourceManager.h"
#include <filesystem>
#include "Renderer.h"

using namespace std;

ResourceManager& ResourceManager::Get()
{
    static ResourceManager resourcemanager;
    return resourcemanager;
}

void ResourceManager::AssetLoad(static D2DRenderer& renderer, const std::string& directory)
{
    namespace fs = std::filesystem;
    fs::path base = fs::current_path();
    fs::path resourcePath = base.parent_path() / "Resource";

    std::cout << "리소스 경로: " << resourcePath << std::endl;

    try {
        for (const auto& entry : fs::recursive_directory_iterator(resourcePath, fs::directory_options::skip_permission_denied)) {
            try {
                if (entry.is_regular_file()) {
                    std::cout << "path: " << entry.path().string() << std::endl;

                    if (entry.path().extension() == ".json") {
                        std::string filename = entry.path().stem().string();
                        std::wstring fullPath = entry.path().wstring();

                        std::cout << "name: " << filename << std::endl;
                        wsg name = wsg(filename.begin(), filename.end());
                        LoadTexture(renderer, name, fullPath);
                    }
                }
            }
            catch (const std::exception& e) {
                std::cerr << "파일 처리 중 오류: " << e.what() << std::endl;
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "디렉토리 순회 실패: " << e.what() << std::endl;
    }













}


void ResourceManager::LoadTexture(static D2DRenderer& renderer, wsg name, path Path)
{
   /* if (!renderer)
       /* std::wcout << "렌더러 없음" << std::endl;*/
    //디버깅 양식 변경해야 함.


    Clip_Asset clips = ap.Load(Path); // json 파싱은 string 기반
    std::filesystem::path imagePath = Path; // ← wstring 기반 path
    imagePath.replace_extension(L".png");

    std::wcout << "이미지 경로: " << imagePath << std::endl;

    if (!std::filesystem::exists(imagePath)) {
        std::wcerr << "[오류] 이미지 파일 존재하지 않음: " << imagePath << std::endl;
        return;
    }

    ComPtr<ID2D1Bitmap1> newBitmap;
    renderer.CreateBitmapFromFile(imagePath.c_str(), newBitmap.GetAddressOf());
    m_textures[name] = newBitmap;

    clips.clip.SetBitmap(newBitmap);
    RegisterClip(clips); //각 Scene마다 이렇게 하긴 함. 

}

ComPtr<ID2D1Bitmap1> ResourceManager::GetTexture(const string& Info)
{
    return nullptr;
}

std::vector<Clip_Asset> ResourceManager::GetClips(const string& Info)
{
    std::vector<Clip_Asset> Out;

    // Info에 해당하는 키 목록이 있는지 확인
    auto it = sceneToClipKeys.find(Info);
    if (it != sceneToClipKeys.end())
    {
        const std::vector<std::string>& keys = it->second;

        for (const auto& key : keys)
        {
            auto assetIt = allClipAssets.find(key);
            if (assetIt != allClipAssets.end())
            {
                Out.push_back(assetIt->second);
            }
            else
            {
                std::cout << "[경고] 키에 해당하는 애셋이 없음: " << key << std::endl;
            }
        }
    }

    return Out;


}

void ResourceManager::Clean()
{
    //일단 만들어는 둠... 혹여나 모르니깐 

}

std::string ResourceManager::GetAbsoluteResourcePathA()
{
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);

    std::filesystem::path path(exePath);
    path = path.parent_path();   // exe 위치
    path /= "Resource";          // 같은 폴더 내 Resource

    return path.string();
}







void ResourceManager::RegisterClip(const Clip_Asset& asset)
{
    std::string uniqueKey = asset.Name + "_" + std::to_string(std::hash<std::string>{}(asset.Ani_Name));
    allClipAssets[uniqueKey] = asset;

    for (std::string info : asset.whichScene) {
        sceneToClipKeys[info].push_back(uniqueKey);
    }
}

std::wstring ResourceManager::to_wstring_hash(const std::string& s)
{
    std::hash<std::string> hasher;
    return std::to_wstring(hasher(s));
}






