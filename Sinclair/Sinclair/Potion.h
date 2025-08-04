#pragma once
#include "Item.h"

class Potion : public Item
{
public:
	Potion(const ItemCommonData& data, const json& j);

	int GetMuch() { return much; }

private:
	int much =0; //얼마나 올려줄 거임? 

};

