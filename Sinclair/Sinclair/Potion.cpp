#include "Potion.h"



Potion::Potion(const ItemCommonData& data, const json& j) : Item(data)
{
    if (j.contains("much"))
        much = j["much"];

    else cout << "얘 가져야 할 게 없는데요?" << endl;
    maxCount = 10;
}

