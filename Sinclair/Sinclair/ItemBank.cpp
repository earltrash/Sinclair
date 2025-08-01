#include "ItemBank.h"
#include "json.hpp"
///////////////////////////
#include <fstream>       // ifstream
#include <filesystem>    // std::filesystem
#include "UI_Renderer.h"

using json = nlohmann::json;


void ItemBank::GetItem(string path) // 한 번에 같이 할거면 Asset 경로도 필요할 듯 
{
    std::ifstream file(path); // json 있을 곳. 
    std::cout << "현재 경로: " << std::filesystem::current_path() << std::endl;

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
    //여기서 추가로 bitmap까지 넣을 거임 . 
     
    for (auto& [Moment, item] : m_Total_Item) //참조로 안 받아오면 큰일 나요 /UNIQUE_PTR이라 
    {
        //std::ifstream file(path); //JSON에 있는 iTEM의 NAME을 

        //item->m_data.name 이거랑 대조시켜서 mapping 시킬거임. 
        
        //item->AddComponent<UI_Renderer>() // 여기에 BITMAP 넣어줘서 ASSET 연결까지 해버리기 ㅇㅇ 
    }



    
}

//호출 시기는 Inven 초기화 할 때도 가능할 듯? ㅇㅇ 차피 ResourceManager 싱글톤이니깐.

void ItemBank::GiveItem(Need_Moment Moment, ItemDatabase& InvenDatabase) //일단 Scene 전환 특히 Ingame에서 전달해 줄 때는 이방식을 채택하겠다만, 합성 및 엔딩 따로 만듦
{
    for (auto& [moment, item] : m_Total_Item) //moment 랑 item 
    {
        if (Moment == moment) //그냥 포션 여러개를 만들어 버려? 어카지 .
        {
            m_Delivered_Item.push_back(std::move(item));
       }
    }

    //이제 인벤 데이터 베이스에 넣어줄 거임. ㅇㅇ 

    for (auto& s : m_Delivered_Item)
    {
        InvenDatabase.AddItemData(std::move(s));
    }

}
