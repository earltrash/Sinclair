#pragma once
#include "Item.h"

class Potion : public Item
{
public:
	Potion(const ItemCommonData& data, const json& j);

	Potion(const Potion& other)
		: Item(other)      // Item 복사 생성자 호출 → Object는 기본 생성
		, much(other.much) // 자기 멤버 복사
	{
		m_data.Momnet = (other.Momnet);
	}

	int GetMuch() { return much; }

	std::unique_ptr<Item> Clone() const override {
		return std::make_unique<Potion>(*this); // 복사 생성
	}

private:
	int much =0; //얼마나 올려줄 거임? 

};

