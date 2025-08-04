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
            std::cerr << "[ERROR] ���� ���� ����: " << entry.path() << std::endl;
            continue;
        }

        json j;
        try {
            file >> j;
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] JSON �Ľ� ���� (" << entry.path() << ") : " << e.what() << std::endl;
            continue;
        }

        if (!j.contains("items") || !j["items"].is_array()) {
            std::cerr << "[WARN] 'items' �迭 ����: " << entry.path() << std::endl;
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

void ItemBank::LoadItemRect(const string& path) //��Ȯ���� Rect�� ������ ����. //Atlas ��θ� ��� �ؾ� �ұ� �̰� ����� �غ��� �� ��.
{
    namespace fs = std::filesystem;
    fs::path base = fs::current_path();
    fs::path resourceFolder = base.parent_path() / path;

    for (const auto& entry : fs::recursive_directory_iterator(resourceFolder, fs::directory_options::skip_permission_denied)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".json")
            continue;

        std::ifstream file(entry.path());
        if (!file.is_open()) {
            std::cerr << "[ERROR] JSON ���� ���� ����: " << entry.path() << "\n";
            continue;
        }

        json j;
        try {
            file >> j;
        }
        catch (const std::exception& e) {
            std::cerr << "[EXCEPTION] JSON �Ľ� ���� (" << entry.path() << "): " << e.what() << "\n";
            continue;
        }

        if (!j.contains("Item") || !j["Item"].is_array()) {
            std::cerr << "[WARN] 'Item' �迭�� ����: " << entry.path() << "\n";
            continue;
        }

        for (const auto& itemData : j["Item"]) {
            if (!itemData.contains("name") || !itemData.contains("x") || !itemData.contains("y") ||
                !itemData.contains("width") || !itemData.contains("height")) {
                std::cerr << "[WARN] �ʵ� ����: " << entry.path() << "\n";
                continue;
            }

            std::string name = itemData["name"];
            float x = itemData["x"];
            float y = itemData["y"];
            float w = itemData["width"];
            float h = itemData["height"];

            D2D_RECT_F srcRect = D2D1::RectF(x, y, x + w, y + h);

            std::string atlasName = entry.path().stem().string();
           
            m_Atlas.emplace(name, ItemBitmapClip{ GetItemAtlas(atlasName) ,srcRect}); // name �� rect ����
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


//ȣ�� �ñ�� Inven �ʱ�ȭ �� ���� ������ ��? ���� ���� ResourceManager �̱����̴ϱ�.

const ItemBitmapClip& ItemBank::GetItemClip(string name) //atlas�� �������� �װ͵� ���� �ؾ� ��.
{
    if (m_Atlas.find(name) != m_Atlas.end())
        return m_Atlas[name];
    else
    {
        std::cout << name << " " << "�� �ش��ϴ� ���� ���µ���?" << std::endl;
    }

} 

void ItemBank::GiveItem(Need_Moment Moment, ItemDatabase& InvenDatabase) //�ϴ� Scene ��ȯ Ư�� Ingame���� ������ �� ���� �̹���� ä���ϰڴٸ�, �ռ� �� ���� ���� ����
{
    for (auto it = m_S_Item.begin(); it != m_S_Item.end(); ) {
        if (it->first == Moment) {
            InvenDatabase.AddItemData(std::move(it->second));
            it = m_S_Item.erase(it); //key ��ü�� �ν� �����ϴ� erase �ϴ� �ŷ� 
        }
        else {
            ++it;
        }
    }

   

}

unique_ptr<Item> ItemBank::Get_Item_Status(string id) //�ռ����� �� �Ÿ� �ʿ�. Ȥ�� �׳� 
{
    auto it = std::find_if(m_S_Item.begin(), m_S_Item.end(),
        [&](auto& pair) { return pair.second->m_data.id == id; });

    if (it != m_S_Item.end()) {
        auto result = std::move(it->second);
        m_S_Item.erase(it); // key ��ü�� �ν� �����ϴ� erase �ϴ� �ŷ� 
        return result;
    }

}


