#pragma once
#include "Item.h"
#include "Status.h"
class Wearable : public Item
{
public:
	Wearable(const ItemCommonData& data, const json& j);

 void SetStat(const Status_fundamental& statName, int value);
 int GetStat(const Status_fundamental& statName) const;
 void SetPart(const Wearable_part& Part);

 Wearable_part Getpart();
 vector<EnchancerType>& GetEnchancResult();

 int GetEnchanCount() const { return Enchan_Count; }
 void UseEnhanceChance() { if (Enchan_Count > 0) --Enchan_Count; }
 void AddStat(fundamentalStatus statType, int value);


 //위 둘은 ItemGlobal 참고 
private:
fundamentalStatus m_stat{}; //장비의 스텟 
Wearable_part m_part; //장비 중에서도 어떤 파츠인지
int Enchan_Count; //무기면 5, 갑옷 3번 
vector<EnchancerType> m_EnchanceResult;
};

