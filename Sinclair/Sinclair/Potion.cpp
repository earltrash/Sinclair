#include "Potion.h"



Potion::Potion(const ItemCommonData& data, const json& j) : Item(data)
{
    if (j.contains("much"))
        much = j["much"];

    else cout << "�� ������ �� �� ���µ���?" << endl;
    maxCount = 10;
}

