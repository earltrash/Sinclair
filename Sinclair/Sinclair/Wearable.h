#pragma once
#include "Item.h"

class Wearable : public Item
{
public:
	Wearable(const ItemCommonData& data, const json& j);

 void SetStat(const Status_fundamental& statName, int value);
 int GetStat(const Status_fundamental& statName) const;
 void SetPart(const Wearable_part& Part);



 //�� ���� ItemGlobal ���� 
private:
fundamentalStatus m_stat{}; //����� ���� 
Wearable_part m_part; //��� �߿����� � ��������
int Enchan_Count;
};

