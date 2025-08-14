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
    //m_ItemBank.LoadItemBitmap("") //��� Atlas ��ġ�� �ѵ� �̰� ����� �غ��� �� �� . 

    m_UI_Bank.Load_UI_Image("UI"); // Single / Multi Bitmap
    m_UI_Bank.Load_UI_Image("Resource/UI"); // Single / Multi Bitmap
    m_ItemBank.LoadItemStatus("Item_S"); //Status Only
    m_ItemBank.LoadItemRect("Item_A"); //Atlas�� ��Ȯ���� Item �� srect 
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

    std::cout << "���ҽ� ���: " << resourcePath << std::endl;

    if (!fs::exists(resourcePath)) {
        std::cerr << "���ҽ� ���͸��� ã�� �� ����: " << resourcePath << std::endl;
        return;
    }

    // recursive_directory_iterator�� ���� �����ϱ� ���� �Ϲ� for ������ ����
    auto it = fs::recursive_directory_iterator(resourcePath, fs::directory_options::skip_permission_denied);
    auto end = fs::end(it);

    for (; it != end; ++it)
    {
        const auto& entry = *it;

        // ������ [������ �κ�] ������
        // ���� �׸��� ���丮�̰�, �̸��� '.'���� �����ϸ� (��: .svn, .git)
        // �ش� ���丮���� ��͸� ��Ȱ��ȭ�ϰ� ���� �׸����� �Ѿ�ϴ�.
        if (entry.is_directory() && entry.path().filename().string().compare(0, 1, ".") == 0)
        {
            it.disable_recursion_pending();
            continue;
        }
        // ������ [������ �κ�] ������

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
            std::cerr << "���� ó�� �� ����: " << entry.path().string() << " - " << e.what() << std::endl;
        }
    }
    //namespace fs = std::filesystem;
    //fs::path base = fs::current_path();
    //fs::path resourcePath = base.parent_path() / "Resource";

    //std::cout << "���ҽ� ���: " << resourcePath << std::endl;

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
    //            std::cerr << "���� ó�� �� ����: " << e.what() << std::endl;
    //        }
    //    }
    //}
    //catch (const fs::filesystem_error& e) {
    //    std::cerr << "���丮 ��ȸ ����: " << e.what() << std::endl;
    //}

}


void ResourceManager::LoadTexture(static D2DRenderer& renderer, wsg name, path Path)
{
   
    Clip_Asset clips = ap.Load(Path); // json �Ľ��� string ���
    std::filesystem::path imagePath = Path; // �� wstring ��� path
    imagePath.replace_extension(L".png");

    std::wcout << "�̹��� ���: " << imagePath << std::endl;

    if (!std::filesystem::exists(imagePath)) {
        std::wcerr << "[����] �̹��� ���� �������� ����: " << imagePath << std::endl;
        return;
    }

    ComPtr<ID2D1Bitmap1> newBitmap;
    renderer.CreateBitmapFromFile(imagePath.c_str(), newBitmap.GetAddressOf());
    m_textures[name] = newBitmap;

    clips.clip.SetBitmap(newBitmap);
    RegisterClip(clips); //�� Scene���� �̷��� �ϱ� ��. 

}

void ResourceManager::UI_AssetLoad(const string Path)
{
    m_UI_Bank.Load_UI_Image(Path); //m_UI_Bank ����� ��. 
}

ComPtr<ID2D1Bitmap1> ResourceManager::GetTexture(const string& Info)
{
    if (m_UI_Bank.Get_Image(Info) != nullptr) //Single bitmap -> ���ȭ�� ���� �ֵ��� �ٷ� ������ �� �ְ�.
        return m_UI_Bank.Get_Image(Info);

    else
    {
        std::cout << Info << " " << "�� �ش��ϴ� Bitmap�� �����ϴ�" << endl;
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
    if (m_UI_Bank.Get_Image(Info, Info2) != nullptr) //Single bitmap -> ���ȭ�� ���� �ֵ��� �ٷ� ������ �� �ְ�.
    {
        //std::cout << Info << "_" << Info2 << "�� �ش��ϴ� Bitmap�� �ֽ��ϴ�" << endl;
        return m_UI_Bank.Get_Image(Info, Info2);
    }

    else
    {
        std::cout << Info << "_" << Info2 << "�� �ش��ϴ� Bitmap�� �����ϴ�" << endl;
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




    // ending/<id> ��� ����
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
        std::cerr << "[ERROR] ���� ����: " << folderPath << std::endl;
        return result;
    }

    // ���� �� ��ȸ
    for (const auto& entry : fs::directory_iterator(folderPath, fs::directory_options::skip_permission_denied))
    {
        if (!entry.is_regular_file() || entry.path().extension() != ".png")
            continue;

        // ���� �̸�(Ȯ���� ����)
        std::string fileName = entry.path().stem().string();

        // ��Ʈ�� �ε�
        Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;
        D2DRenderer::Get().CreateBitmapFromFile(entry.path().c_str(), bitmap.GetAddressOf());

        if (bitmap)
        {
            result.emplace(fileName, bitmap);
        }
        else
        {
            std::cerr << "[WARN] ��Ʈ�� �ε� ����: " << entry.path() << std::endl;
        }
    }

    return result;
}



std::vector<Clip_Asset> ResourceManager::GetClips(const string& Info)
{
    std::vector<Clip_Asset> Out;

    // Info�� �ش��ϴ� Ű ����� �ִ��� Ȯ��
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
                std::cout << "[���] Ű�� �ش��ϴ� �ּ��� ����: " << key << std::endl;
            }
        }
    }

    return Out;


}

void ResourceManager::Clean()
{
    //�ϴ� ������ ��... Ȥ���� �𸣴ϱ� 
    //m_ItemBank.clean();
    //m_UI_Bank.clean();

}

//std::string ResourceManager::GetAbsoluteResourcePathA()
//{
//    wchar_t exePath[MAX_PATH];
//    GetModuleFileNameW(nullptr, exePath, MAX_PATH);
//
//    std::filesystem::path path(exePath);
//    path = path.parent_path();   // exe ��ġ
//    path /= "Resource";          // ���� ���� �� Resource
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






