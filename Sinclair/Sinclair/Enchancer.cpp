
#include "Enchancer.h"


Enchancer::Enchancer(const ItemCommonData& data, const json& j) : Item(data)
{
    //일단 이거 디버깅용임요 
    if (j.contains("much"))
        much = j["much"];
    else cout << "얘 있어야 할 게 없는데요?" << endl;

    if (j.contains("stat"))
    {
        string stat = j["stat"];
        which_stat = StringToF(stat);
    }
    else cout << "얘 스텟을 못읽어옴ㅇㅇ" << endl;



}
