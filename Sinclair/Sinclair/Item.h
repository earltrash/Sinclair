#pragma once
#include "pch.h"
#include "Status.h"
#include "Object.h"

//밑은 파싱과 클래스 오염을 방지?
#include "json.hpp"
#include "ItemGlobal.h"
using namespace std;

using json = nlohmann::json;

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp)) // 그냥 멋지게 쓰고 싶었다...

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
	//마우스 클릭도 받게 할거니깐 
	Item(ItemCommonData data) :m_data(data) {}; //stat을 넣어주자, 값 받아오는 거를 어떻게 하면 좋을까
	virtual ~Item() = default;

	const bool IsStackable() const {
		return maxCount > 1;
	}

public:  
	ItemCommonData m_data{};
	Need_Moment Momnet; //ResourceManager랑 mapping할 때 사용 할 거 임.
	int maxCount = 0; //-> 이거 나중에 파싱할 때, potion이면 maxCount 가 1이상이어야 함. 이거 나중에 파싱할 때 따로 설정함.
};


