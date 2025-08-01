#pragma once
#include "pch.h"
#include "Status.h"
#include "Object.h"

//���� �Ľ̰� Ŭ���� ������ ����?
#include "json.hpp"
#include "ItemGlobal.h"
using namespace std;

using json = nlohmann::json;

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp)) // �׳� ������ ���� �;���...

struct ItemCommonData {
	std::string id;
	std::string name;
	std::string description;
	bool enchantable;
	bool synthesizable;
	Need_Moment Momnet;
};

class Item : public Object //
{
public:
	//���콺 Ŭ���� �ް� �ҰŴϱ� 
	Item(ItemCommonData data) :m_data(data) {}; //stat�� �־�����, �� �޾ƿ��� �Ÿ� ��� �ϸ� ������
	virtual ~Item() = default;

	const bool IsStackable() const {
		return maxCount > 1;
	}

public:  
	ItemCommonData m_data{};
	Need_Moment Momnet; //ResourceManager�� mapping�� �� ��� �� �� ��.
	int maxCount = 0; //-> �̰� ���߿� �Ľ��� ��, potion�̸� maxCount �� 1�̻��̾�� ��. �̰� ���߿� �Ľ��� �� ���� ������.
};


