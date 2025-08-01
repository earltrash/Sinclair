#include "ItemBank.h"
#include "json.hpp"
///////////////////////////
#include <fstream>       // ifstream
#include <filesystem>    // std::filesystem
#include "UI_Renderer.h"

using json = nlohmann::json;


void ItemBank::GetItem(string path) // �� ���� ���� �ҰŸ� Asset ��ε� �ʿ��� �� 
{
    std::ifstream file(path); // json ���� ��. 
    std::cout << "���� ���: " << std::filesystem::current_path() << std::endl;

    json j;
    file >> j;
    int times = 0;
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

        std::cout << j["description"] << std::endl;

        std::unique_ptr<Item> item;

        if (type == "Potion")
            item = std::make_unique<Potion>(common, itemData);
        /*else if (type == "Enchancer")
            func = std::make_unique<Potion>(common, itemData);*/
        else if (type == "Wearable")
            item = std::make_unique<Potion>(common, itemData);
        else if (type == "Material")
            item = std::make_unique<Potion>(common, itemData);
        else
        {
            std::cerr << "Unknown type: " << type << "\n";
            return;
        }

        m_Total_Item.emplace(item->m_data.Momnet, std::move(item)); 
    }
    //���⼭ �߰��� bitmap���� ���� ���� . 
     
    for (auto& [Moment, item] : m_Total_Item) //������ �� �޾ƿ��� ū�� ���� /UNIQUE_PTR�̶� 
    {
        //std::ifstream file(path); //JSON�� �ִ� iTEM�� NAME�� 

        //item->m_data.name �̰Ŷ� �������Ѽ� mapping ��ų����. 
        
        //item->AddComponent<UI_Renderer>() // ���⿡ BITMAP �־��༭ ASSET ������� �ع����� ���� 
    }



    
}

//ȣ�� �ñ�� Inven �ʱ�ȭ �� ���� ������ ��? ���� ���� ResourceManager �̱����̴ϱ�.

void ItemBank::GiveItem(Need_Moment Moment, ItemDatabase& InvenDatabase) //�ϴ� Scene ��ȯ Ư�� Ingame���� ������ �� ���� �̹���� ä���ϰڴٸ�, �ռ� �� ���� ���� ����
{
    for (auto& [moment, item] : m_Total_Item) //moment �� item 
    {
        if (Moment == moment) //�׳� ���� �������� ����� ����? ��ī�� .
        {
            m_Delivered_Item.push_back(std::move(item));
       }
    }

    //���� �κ� ������ ���̽��� �־��� ����. ���� 

    for (auto& s : m_Delivered_Item)
    {
        InvenDatabase.AddItemData(std::move(s));
    }

}
