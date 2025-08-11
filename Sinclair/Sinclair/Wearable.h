#pragma once
#include "Item.h"
#include "Status.h"
class Wearable : public Item
{
public:
	Wearable(const ItemCommonData& data, const json& j);

    Wearable(const Wearable& other)
        : Item(other)
        , m_stat(other.m_stat)
        , m_part(other.m_part)
        , Enchan_Count(other.Enchan_Count)
        , m_EnchanceResult(other.m_EnchanceResult)
    {
        m_data.wearablePart = m_part;
    }


 void SetStat(const Status_fundamental& statName, int value);
 int GetStat(const Status_fundamental& statName) const;
 void SetPart(const Wearable_part& Part);

 std::unique_ptr<Item> Clone() const override;

 Wearable_part Getpart();
 vector<EnchancerType>& GetEnchancResult();

 int GetEnchanCount() const { return Enchan_Count; }
 void UseEnhanceChance() { if (Enchan_Count > 0) --Enchan_Count; }
 void AddStat(const fundamentalStatus& statType);


 //위 둘은 ItemGlobal 참고 
private:
fundamentalStatus m_stat{}; //장비의 스텟 
Wearable_part m_part; //장비 중에서도 어떤 파츠인지

int Enchan_Count; //무기면 5, 갑옷 3번 
vector<EnchancerType> m_EnchanceResult;
};

