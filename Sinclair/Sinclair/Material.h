#pragma once
#include "Item.h"

class Material : public Item
{
public:
	Material(const ItemCommonData& data, const json& j);
	

private:
	fundamentalStatus m_stat{};


};