#pragma once
#include "Item.h"

class Enchancer : public Item
{
public:
	Enchancer(const ItemCommonData& data, const json& j);
	float GetPer() { return per; }

private:
	Status_fundamental which_stat; //
	float per = 40.0f;
	int much =0; //얼마나 올려줄 거임? 

};

