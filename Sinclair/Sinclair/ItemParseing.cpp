
#include "Potion.h"
#include "Material.h"
#include "Wearable.h"
#include "json.hpp"
///////////////////////////
#include <fstream>       // ifstream
#include <filesystem>    // std::filesystem

using json = nlohmann::json;

//LoadItemsFromJson("../OBJ_Json/item.json");

void LoadItemsFromJson(const std::string& path)
{
    //enchancer 삭제 

    //std::ifstream file(path); //
    //std::cout << "현재 경로: " << std::filesystem::current_path() << std::endl;

    //json j;
    //file >> j;
    //int times = 0;
    //for (const auto& itemData : j["items"]) {

    //    std::string type = itemData["type"];
    //    std::string moment = itemData["Moment"];
    //    cout << moment << endl;
    //    ItemCommonData common = {
    //        itemData["id"],
    //        itemData["name"],
    //        itemData["description"],
    //        itemData["enchantable"],
    //        itemData["synthesizable"],
    //        (StringToNM(moment))
    //    };
    //    std::cout << j["description"] << std::endl;
    //    //디버깅이 어려워 질 수도 있겠는데, 만들어지는 순간에 타입이 결정됨.
    //    ObjFactory::Get().RegisterObject(common.name, [=]() -> std::unique_ptr<Object> {
    //        if (type == "Potion")
    //            return std::make_unique<Potion>(common, itemData);
    //        else if (type == "Enchancer")
    //            return std::make_unique<Enchancer>(common, itemData);
    //        else if (type == "Wearable")
    //            return std::make_unique<Wearable>(common, itemData);
    //        else if (type == "Material")
    //            return std::make_unique<Material>(common, itemData);
    //        else {
    //            std::cerr << "Unknown type: " << type << "\n";
    //            return nullptr;
    //        }
    //        });
    //}
}