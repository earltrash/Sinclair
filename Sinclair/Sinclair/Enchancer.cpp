
#include "Enchancer.h"


Enchancer::Enchancer(const ItemCommonData& data, const json& j) : Item(data)
{
    //�ϴ� �̰� �������ӿ� 
    if (j.contains("much"))
        much = j["much"];
    else cout << "�� �־�� �� �� ���µ���?" << endl;

    if (j.contains("stat"))
    {
        string stat = j["stat"];
        which_stat = StringToF(stat);
    }
    else cout << "�� ������ ���о�Ȥ���" << endl;



}
