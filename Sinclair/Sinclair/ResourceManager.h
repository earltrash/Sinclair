#pragma once
#include "pch.h"
#include "AnimationClip.h" 
#include "AsepriteParser.h"
#include "Renderer.h"


#include "UI_Bank.h"
#include "ItemBank.h"
#include "TextBank.h"


using namespace std;

using wsg = std::wstring;
//using path = std::filesystem::path;
using path = std::filesystem::path;

//1. Scene������  asset�ҷ����� ����
//2. asset ���� ����
//3. 


struct Clip_Asset {
    std::string Name;
    std::string Ani_Name; // ������ ���� 
    AnimationClip clip;
    std::vector<string> whichScene; //��� ��. ������ 
};

class ResourceManager //Core Initalize �ܰ迡�� Asset ���ε� �ϱ� ��. ��� asset�� ���� Core Pointer �� �Ѱ��ֱ⸸ �ϸ� �Ǵϱ� unorded_map 
{
    using AnimationClips = std::vector<std::pair<std::string, AnimationClip>>; //obj�� clip�� ����. 
private:
    ResourceManager() = default;
    ~ResourceManager() = default;
public:
    static ResourceManager& Get();

    //static ResourceManager& Get();

    void GameAssetLoad();


    void AnimatedAssetLoad(static D2DRenderer& renderer, const std::string& directory);
    void LoadTexture(static D2DRenderer& renderer, wsg name, path path);
    void RegisterClip(const Clip_Asset& asset);
    std::vector<Clip_Asset> GetClips(const string& Info);


public:
    void UI_AssetLoad(const string Path);
    ComPtr<ID2D1Bitmap1> GetTexture(const string& Info);
    ComPtr<ID2D1Bitmap1> GetTexture(const string& Info, const string& Info2);
    
    UI_Bank& Get_UIBank();
    ItemBank& Get_ItemBank();
    TextBank& Get_TextBank();
   
public:
    void Clean();
    //��� ����  std::string GetAbsoluteResourcePathA(); 

private:
    AsepriteParser ap;
    std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> m_textures;
    std::wstring to_wstring_hash(const std::string& s);

    //���� ���� ���� �κ��� �и�. 
    std::unordered_map<std::string, Clip_Asset> allClipAssets;
    std::unordered_map<string, std::vector<std::string>> sceneToClipKeys;



    ItemBank m_ItemBank;
    UI_Bank m_UI_Bank;
    TextBank m_TextBank;
    // AsepriteParser -> �Ľ��ϴ� �� 
};