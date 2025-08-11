#pragma once
#include "pch.h"
#include "Status.h"
#include "Object.h"
#include "MouseInput.h"
//���� �Ľ̰� Ŭ���� ������ ����?
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
	//���콺 Ŭ���� �ް� �ҰŴϱ� 
	Item(ItemCommonData data) :m_data(data) {}; //stat�� �־�����, �� �޾ƿ��� �Ÿ� ��� �ϸ� ������
	virtual ~Item() = default;

	Item(const Item& other)
		: Object()  //  ��������� �ƴ϶� '�⺻����'���� ���� Object�� ���� ���
		, m_data(other.m_data)
		, Momnet(other.Momnet)
		, maxCount(other.maxCount)
	{}

	Item& operator=(const Item& other) {
		if (this != &other) {
			// Object ���� �ǵ帮�� ����
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
	Need_Moment Momnet; //ResourceManager�� mapping�� �� ��� �� �� ��.
	int maxCount = 0; //-> �̰� ���߿� �Ľ��� ��, potion�̸� maxCount �� 1�̻��̾�� ��. �̰� ���߿� �Ľ��� �� ���� ������.
};


