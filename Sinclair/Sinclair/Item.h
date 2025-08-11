#pragma once
#include "pch.h"
#include "Status.h"
#include "Object.h"
#include "MouseInput.h"
//밑은 파싱과 클래스 오염을 방지?
#include "json.hpp"
#include "ItemGlobal.h"
using namespace std;

using json = nlohmann::json;

struct ItemCommonData {
	std::string id;
	std::string name;
	std::string description;
	bool enchantable;
	bool synthesizable;
	Need_Moment Momnet;
	Wearable_part wearablePart;
};

class Item : public Object //
{
public:
	//마우스 클릭도 받게 할거니깐 
	Item(ItemCommonData data) :m_data(data) {}; //stat을 넣어주자, 값 받아오는 거를 어떻게 하면 좋을까
	virtual ~Item() = default;

	Item(const Item& other)
		: Object()  //  복사생성이 아니라 '기본생성'으로 기저 Object를 새로 깐다
		, m_data(other.m_data)
		, Momnet(other.Momnet)
		, maxCount(other.maxCount)
	{}

	Item& operator=(const Item& other) {
		if (this != &other) {
			// Object 쪽은 건드리지 않음
			m_data = other.m_data;
			Momnet = other.Momnet;
			maxCount = other.maxCount;
		}
		return *this;
	}



	virtual std::unique_ptr<Item> Clone() const = 0;

	const bool IsStackable() const {
		return maxCount > 1;
	}

public:  
	ItemCommonData m_data{};
	Need_Moment Momnet; //ResourceManager랑 mapping할 때 사용 할 거 임.
	int maxCount = 0; //-> 이거 나중에 파싱할 때, potion이면 maxCount 가 1이상이어야 함. 이거 나중에 파싱할 때 따로 설정함.
};


