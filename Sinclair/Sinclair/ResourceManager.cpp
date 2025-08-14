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

void ResourceManager::GameAssetLoad()
{
    //AnimatedAssetLoad(D2DRenderer::Get(), "Resource");
    //m_ItemBank.LoadItemStatus("Resource/Item"); //Status Only
    //m_ItemBank.LoadItemBitmap("") //얘는 Atlas 위치긴 한데 이거 얘기좀 해봐야 할 듯 . 

    m_UI_Bank.Load_UI_Image("UI"); // Single / Multi Bitmap
    m_UI_Bank.Load_UI_Image("Resource/UI"); // Single / Multi Bitmap
    m_ItemBank.LoadItemStatus("Item_S"); //Status Only
    m_ItemBank.LoadItemRect("Item_A"); //Atlas랑 정확히는 Item 별 srect 
    m_SoundBank.SoundAssetLoad("Sound");
}

void ResourceManager::AnimatedAssetLoad(static D2DRenderer& renderer, const std::string& directory)
{
    namespace fs = std::filesystem;
    fs::path base = fs::current_path();


#ifdef _DEBUG
    fs::path resourcePath = base.parent_path() / "Resource" / directory;

#else NDEBUG 
    //fs::path resourcePath = base.parent_path().parent_path() / "Resource" / directory;
    fs::path resourcePath = base.parent_path() / "Resource" / directory;

#endif
  //  fs::path resourcePath = base.parent_path() / "Resource" / directory;

    //fs::path resourcePath = base.parent_path() / "Resource";

    std::cout << "리소스 경로: " << resourcePath << std::endl;

    if (!fs::exists(resourcePath)) {
        std::cerr << "리소스 디렉터리를 찾을 수 없음: " << resourcePath << std::endl;
        return;
    }

    // recursive_directory_iterator를 직접 제어하기 위해 일반 for 루프로 변경
    auto it = fs::recursive_directory_iterator(resourcePath, fs::directory_options::skip_permission_denied);
    auto end = fs::end(it);

    for (; it != end; ++it)
    {
        const auto& entry = *it;

        // ▼▼▼▼▼ [수정된 부분] ▼▼▼▼▼
        // 현재 항목이 디렉토리이고, 이름이 '.'으로 시작하면 (예: .svn, .git)
        // 해당 디렉토리로의 재귀를 비활성화하고 다음 항목으로 넘어갑니다.
        if (entry.is_directory() && entry.path().filename().string().compare(0, 1, ".") == 0)
        {
            it.disable_recursion_pending();
            continue;
        }
        // ▲▲▲▲▲ [수정된 부분] ▲▲▲▲▲

        try {
            if (entry.is_regular_file()) {
                std::cout << "path: " << entry.path().string() << std::endl;

                if (entry.path().extension() == ".json") 
                {
                    std::string filename = entry.path().stem().string();
                    std::wstring fullPath = entry.path().wstring();

                    std::cout << "name: " << filename << std::endl;
                    wsg name = wsg(filename.begin(), filename.end());
                    LoadTexture(renderer, name, fullPath);
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "파일 처리 중 오류: " << entry.path().string() << " - " << e.what() << std::endl;
        }
    }
    //namespace fs = std::filesystem;
    //fs::path base = fs::current_path();
    //fs::path resourcePath = base.parent_path() / "Resource";

    //std::cout << "리소스 경로: " << resourcePath << std::endl;

    //try {
    //    for (const auto& entry : fs::recursive_directory_iterator(resourcePath, fs::directory_options::skip_permission_denied)) {
    //        try {
    //            if (entry.is_regular_file()) {
    //                std::cout << "path: " << entry.path().string() << std::endl;

    //                if (entry.path().extension() == ".json") {
    //                    std::string filename = entry.path().stem().string();
    //                    std::wstring fullPath = entry.path().wstring();

    //                    std::cout << "name: " << filename << std::endl;
    //                    wsg name = wsg(filename.begin(), filename.end());
    //                    LoadTexture(renderer, name, fullPath);
    //                }
    //            }
    //        }
    //        catch (const std::exception& e) {
    //            std::cerr << "파일 처리 중 오류: " << e.what() << std::endl;
    //        }
    //    }
    //}
    //catch (const fs::filesystem_error& e) {
    //    std::cerr << "디렉토리 순회 실패: " << e.what() << std::endl;
    //}

}


void ResourceManager::LoadTexture(static D2DRenderer& renderer, wsg name, path Path)
{
   
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

void ResourceManager::UI_AssetLoad(const string Path)
{
    m_UI_Bank.Load_UI_Image(Path); //m_UI_Bank 멤버에 들어감. 
}

ComPtr<ID2D1Bitmap1> ResourceManager::GetTexture(const string& Info)
{
    if (m_UI_Bank.Get_Image(Info) != nullptr) //Single bitmap -> 배경화면 같은 애들은 바로 가져올 수 있게.
        return m_UI_Bank.Get_Image(Info);

    else
    {
        std::cout << Info << " " << "에 해당하는 Bitmap이 없습니다" << endl;
        return nullptr;
    }

    //std::wstring wInfo(Info.begin(), Info.end());
    //auto it = m_textures.find(wInfo);
    //if (it != m_textures.end()) {
    //    return it->second;
    //}
    //return nullptr;
}

ComPtr<ID2D1Bitmap1> ResourceManager::GetTexture(const string& Info, const string& Info2)
{
    if (m_UI_Bank.Get_Image(Info, Info2) != nullptr) //Single bitmap -> 배경화면 같은 애들은 바로 가져올 수 있게.
    {
        //std::cout << Info << "_" << Info2 << "에 해당하는 Bitmap이 있습니다" << endl;
        return m_UI_Bank.Get_Image(Info, Info2);
    }

    else
    {
        std::cout << Info << "_" << Info2 << "에 해당하는 Bitmap이 없습니다" << endl;
        return nullptr;
    }

    //std::wstring wInfo(Info.begin(), Info.end());
    //auto it = m_textures.find(wInfo);
    //if (it != m_textures.end()) {
    //    return it->second;
    //}
    //return nullptr;
}

UI_Bank& ResourceManager::Get_UIBank()
{
    return m_UI_Bank;
}

ItemBank& ResourceManager::Get_ItemBank()
{
    return m_ItemBank;
}

TextBank& ResourceManager::Get_TextBank()
{
    return m_TextBank;
}



SoundBank& ResourceManager::Get_SoundBank()
{
    return m_SoundBank;
}




std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> ResourceManager::GetEndingBitmap(string id )
{
    namespace fs = std::filesystem;

    std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> result;




    // ending/<id> 경로 생성
    fs::path base = fs::current_path();


#ifdef _DEBUG
    fs::path resourcePath = base.parent_path() / "ending" / id;

#else NDEBUG 
    //fs::path resourcePath = base.parent_path().parent_path() / "ending" / id;
    fs::path resourcePath = base.parent_path() / "ending" / id;

#endif
    //  fs::path resourcePath = base.parent_path() / "Resource" / directory;




    fs::path folderPath = base.parent_path() / "ending" / id;

    if (!fs::exists(folderPath) || !fs::is_directory(folderPath))
    {
        std::cerr << "[ERROR] 폴더 없음: " << folderPath << std::endl;
        return result;
    }

    // 폴더 내 순회
    for (const auto& entry : fs::directory_iterator(folderPath, fs::directory_options::skip_permission_denied))
    {
        if (!entry.is_regular_file() || entry.path().extension() != ".png")
            continue;

        // 파일 이름(확장자 제거)
        std::string fileName = entry.path().stem().string();

        // 비트맵 로드
        Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;
        D2DRenderer::Get().CreateBitmapFromFile(entry.path().c_str(), bitmap.GetAddressOf());

        if (bitmap)
        {
            result.emplace(fileName, bitmap);
        }
        else
        {
            std::cerr << "[WARN] 비트맵 로드 실패: " << entry.path() << std::endl;
        }
    }

    return result;
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
    //m_ItemBank.clean();
    //m_UI_Bank.clean();

}

//std::string ResourceManager::GetAbsoluteResourcePathA()
//{
//    wchar_t exePath[MAX_PATH];
//    GetModuleFileNameW(nullptr, exePath, MAX_PATH);
//
//    std::filesystem::path path(exePath);
//    path = path.parent_path();   // exe 위치
//    path /= "Resource";          // 같은 폴더 내 Resource
//
//    return path.string();
//}


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






