#include "ItemBank.h"
#include "json.hpp"
///////////////////////////
#include <fstream>       // ifstream
#include <filesystem>    // std::filesystem
#include "UI_Renderer.h"
#include "Renderer.h"
#include "Potion.h"
#include "Material.h"
#include "Wearable.h"
#include "Inventory.h"



namespace fs = std::filesystem;
using json = nlohmann::json;


void ItemBank::LoadItemStatus(const string& path) { //스텟 JSON - Item_S

    namespace fs = std::filesystem;
    fs::path base = fs::current_path();

#ifdef _DEBUG
    fs::path resourceFolder = base.parent_path() / "Item" / path;

#else NDEBUG 
    //fs::path resourceFolder = base.parent_path().parent_path() / "Item" / path;
    fs::path resourceFolder = base.parent_path()/ "Item" / path;

#endif
    


    int times = 0;

    for (const auto& entry : fs::recursive_directory_iterator(resourceFolder, fs::directory_options::skip_permission_denied)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".json")
            continue;

        std::ifstream file(entry.path());
        if (!file.is_open()) {
            std::cerr << "[ERROR] 파일 열기 실패: " << entry.path() << std::endl;
            continue;
        }

        json j;
        try {
            file >> j;
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] JSON 파싱 실패 (" << entry.path() << ") : " << e.what() << std::endl;
            continue;
        }

        if (!j.contains("items") || !j["items"].is_array()) {
            std::cerr << "[WARN] 'items' 배열 없음: " << entry.path() << std::endl;
            continue;
        }

        for (const auto& itemData : j["items"]) {
            std::string type = itemData["type"];
            std::string moment = itemData["Moment"];
            std::string Sound = itemData["sound"];

            ItemCommonData common = {
                itemData["id"],
                itemData["name"],
                itemData["description"],
                itemData["enchantable"],
                itemData["synthesizable"],
                StringToNM(moment),     
                StringToES(Sound)
            };


            std::unique_ptr<Item> item;
            times++;
            if (type == "Potion")
            {
                item = std::make_unique<Potion>(common, itemData);
                m_prototypes[itemData["id"]] = std::make_unique<Potion>(common, itemData);
            }

            else if (type == "Wearable")
            {
                item = std::make_unique<Wearable>(common, itemData);
                m_prototypes[itemData["id"]] = std::make_unique<Wearable>(common, itemData);

            }
            else if (type == "Material")
            {
                item = std::make_unique<Material>(common, itemData);
                m_prototypes[itemData["id"]] = std::make_unique<Material>(common, itemData);

            }
            else {
                std::cerr << "[WARN] Unknown item type: " << type << std::endl;
                continue;
            }
           // std::cout << moment << endl;


            m_S_Item.emplace(item->m_data.Momnet, std::move(item));
        }
    }

  //  std::cout << "아이템 크기는?" << " " << m_S_Item.size() << std::endl;
}

void ItemBank::LoadItemRect(const string& path) //Json과 Png는 이름이 같고 같은 경로에 두어야 함.
{
    namespace fs = std::filesystem;
    fs::path base = fs::current_path();


#ifdef _DEBUG
    fs::path resourceFolder = base.parent_path() / "Item" / path;

#else NDEBUG 
    // fs::path resourceFolder = base.parent_path().parent_path() / "Item" / path;
    fs::path resourceFolder = base.parent_path() / "Item" / path;

#endif
    //fs::path resourceFolder = base.parent_path() / "Item" / path;


   // fs::path resourceFolder = base.parent_path() /"Item" / path;
    int times = 0;

    for (const auto& entry : fs::recursive_directory_iterator(resourceFolder, fs::directory_options::skip_permission_denied)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".json")
            continue;

        std::ifstream file(entry.path());
        if (!file.is_open()) {
            std::cerr << "[ERROR] JSON 파일 열기 실패: " << entry.path() << "\n";
            continue;
        }

        json j;
        try {
            file >> j;
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] JSON 파싱 실패 (" << entry.path() << "): " << e.what() << "\n";
            continue;
        }

        if (!j.contains("Item") || !j["Item"].is_array()) {
            std::cerr << "[WARN] 'Item' 배열이 없음: " << entry.path() << "\n";
            continue;
        }

        for (const auto& itemData : j["Item"]) {
            if (!itemData.contains("name") || !itemData.contains("x") || !itemData.contains("y") ||
                !itemData.contains("width") || !itemData.contains("height")) {
                std::cerr << "[WARN] 필드 누락: " << entry.path() << "\n";
                continue;
            }

            std::string name = itemData["name"];
            float x = itemData["x"];
            float y = itemData["y"];
            float w = itemData["width"];
            float h = itemData["height"];

            D2D_RECT_F srcRect = D2D1::RectF(x, y, x + w, y + h);

            std::string atlasName = entry.path().stem().string();


            std::filesystem::path atlasPath = entry.path();
            atlasPath.replace_extension(".png");
           
            m_Atlas.emplace(name, ItemBitmapClip{ GetItemAtlas(atlasPath.string()) ,srcRect}); // name → rect 저장
            //std::cout << name << endl;
            times++;
        }
    }

   // std::cout << "매핑된 오브젝트 갯수"<<" " << m_Atlas.size() << endl;
   // std::cout << "아틀라스 종류의 개수" << " " << m_LoadedAtlases.size() << endl;
   // std::cout << times << endl;;
}

ComPtr<ID2D1Bitmap1> ItemBank::GetItemAtlas(const string& path)
{
    if (m_LoadedAtlases.find(path) != m_LoadedAtlases.end())
        return m_LoadedAtlases[path];

    std::filesystem::path imagePath = path;
    imagePath.replace_extension(L".png");

    ComPtr<ID2D1Bitmap1> temp;
    D2DRenderer::Get().CreateBitmapFromFile(imagePath.c_str(), temp.GetAddressOf());

    m_LoadedAtlases[path] = temp;
    return temp;
}


//호출 시기는 Inven 초기화 할 때도 가능할 듯? ㅇㅇ 차피 ResourceManager 싱글톤이니깐.

const ItemBitmapClip* ItemBank::GetItemClip(string id) //atlas가 여러개면 그것도 구분 해야 함.
{
    if (auto it = m_Atlas.find(id); it != m_Atlas.end())
        return &it->second;
    else
    {
        std::cout << id << " " << "에 해당하는 값이 없는데요?" << std::endl;
        return nullptr;
    }

} 

void ItemBank::GiveItem(Need_Moment Moment, ItemDatabase& InvenDatabase) //일단 Scene 전환 특히 Ingame에서 전달해 줄 때는 이방식을 채택하겠다만, 합성 및 엔딩 따로 만듦
{
    for (auto it = m_S_Item.begin(); it != m_S_Item.end(); ) {
        if (it->first == Moment) {
            // 원본에서 복제본 생성
            auto newItem = CreateItem(it->second->m_data.id); //클론을 만들어서 보내줌 -> base는 변경 값 없음 
            if (newItem) {
                InvenDatabase.AddItemData(std::move(newItem));
            }
            ++it; // 원본은 지우지 않음
        }
        else {
            ++it;
        }
    }
}


// 확룰 계산해서 특정 세대 -> Need_Moment / 받아와야 하는 개수를 넣어서, 랜덤 -> 장비 RETURN 해주는 함수를 만들어야 할 듯 ? 



unique_ptr<Item> ItemBank::Get_Item_Status(string id) //합성에서 쓸 거면 필요. 혹은 그냥 
{
    auto it = std::find_if(m_S_Item.begin(), m_S_Item.end(),
        [&](auto& pair) { return pair.second->m_data.id == id; });

    if (it != m_S_Item.end()) {
        auto result = std::move(it->second);
        m_S_Item.erase(it); // key 자체는 인식 가능하니 erase 하는 거로 
        return result;
    }
}

void ItemBank::clean()
{
    m_S_Item.clear();
    m_Atlas.clear();
    m_LoadedAtlases.clear();
}

vector<string> ItemBank::GetItemIDsByMoment(Need_Moment moment)
{
    vector<string> result;

    for (auto& pair : m_S_Item)
    {
        if (pair.first == moment )
        {
            result.push_back(pair.second->m_data.id);
        }
    }

    return result;


}


std::unique_ptr<Item> ItemBank::CreateItem(const std::string& id) {
    auto it = m_prototypes.find(id);
    if (it != m_prototypes.end()) {
        return it->second->Clone(); // 복사본 리턴
    }
    return nullptr;
}