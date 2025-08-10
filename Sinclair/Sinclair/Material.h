#pragma once
#include "Item.h"

class Material : public Item
{
public:
	Material(const ItemCommonData& data, const json& j);

	Material(const Material& other)
		: Item(other)
		, m_stat(other.m_stat)
	{
	}

	std::unique_ptr<Item> Clone() const override {
		return std::make_unique<Material>(*this); // ���� ����
	}

private:
	fundamentalStatus m_stat{};


};