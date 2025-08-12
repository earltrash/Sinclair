#pragma once
#include "Item.h"

class Potion : public Item
{
public:
	Potion(const ItemCommonData& data, const json& j);

	Potion(const Potion& other)
		: Item(other)      // Item ���� ������ ȣ�� �� Object�� �⺻ ����
		, much(other.much) // �ڱ� ��� ����
	{
		m_data.Momnet = (other.Momnet);
	}

	int GetMuch() { return much; }

	std::unique_ptr<Item> Clone() const override {
		return std::make_unique<Potion>(*this); // ���� ����
	}

private:
	int much =0; //�󸶳� �÷��� ����? 

};

