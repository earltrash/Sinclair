#include "ItemBank.h"
#include "json.hpp"
///////////////////////////
#include <fstream>       // ifstream
#include <filesystem>    // std::filesystem
#include "UI_Renderer.h"

namespace fs = std::filesystem;
using json = nlohmann::json;


void ItemBank::LoadItemStatus(const string& path) {

    namespace fs = std::filesystem;
    fs::path base = fs::current_path();
    fs::path resourceFolder = base.parent_path() / path;

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

            ItemCommonData common = {
                itemData["id"],
                itemData["name"],
                itemData["description"],
                itemData["enchantable"],
                itemData["synthesizable"],
                StringToNM(moment)
            };

            std::unique_ptr<Item> item;

            if (type == "Potion")
                item = std::make_unique<Potion>(common, itemData);
            else if (type == "Wearable")
                item = std::make_unique<Wearable>(common, itemData);
            else if (type == "Material")
                item = std::make_unique<Material>(common, itemData);
            else {
                std::cerr << "[WARN] Unknown item type: " << type << std::endl;
                continue;
            }
           // std::cout << moment << endl;

            m_S_Item.emplace(item->m_data.Momnet, std::move(item));
        }
    }
}

void ItemBank::LoadItemRect(const string& path) //정확히는 Rect를 저장한 것임. //Atlas 경로를 어떻게 해야 할까 이건 고민좀 해봐야 할 듯.
{
    namespace fs = std::filesystem;
    fs::path base = fs::current_path();
    fs::path resourceFolder = base.parent_path() / path;

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
           
            m_Atlas.emplace(name, ItemBitmapClip{ GetItemAtlas(atlasName) ,srcRect}); // name → rect 저장
        }
    }
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

const ItemBitmapClip& ItemBank::GetItemClip(string name) //atlas가 여러개면 그것도 구분 해야 함.
{
    if (m_Atlas.find(name) != m_Atlas.end())
        return m_Atlas[name];
    else
    {
        std::cout << name << " " << "에 해당하는 값이 없는데요?" << std::endl;
    }

} 

void ItemBank::GiveItem(Need_Moment Moment, ItemDatabase& InvenDatabase) //일단 Scene 전환 특히 Ingame에서 전달해 줄 때는 이방식을 채택하겠다만, 합성 및 엔딩 따로 만듦
{
    for (auto it = m_S_Item.begin(); it != m_S_Item.end(); ) {
        if (it->first == Moment) {
            InvenDatabase.AddItemData(std::move(it->second));
            it = m_S_Item.erase(it); //key 자체는 인식 가능하니 erase 하는 거로 
        }
        else {
            ++it;
        }
    }

   

}

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


