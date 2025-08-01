#pragma once
#include "pch.h"
#include "Item.h"
#include "Potion.h"
#include "Material.h"
#include "Wearable.h"
#include "ItemGlobal.h"
#include "Inventory.h"

using namespace std;
class ItemBank
{
public :
	ItemBank() = default;
	~ItemBank() = default; //에셋 정리하고 갈건데, 아마 ResourceManager.clean에서 여기 접근해다가 정리시킬듯 ㅇㅇ
public:
	void GetItem(string path); //json 읽어서 파일 load 시킬 거임 -> 아마 경로는 넣어줘야 할 듯 

	void GiveItem(Need_Moment Moment, ItemDatabase& InvenDatabase);



private:
	
	vector<unique_ptr<Item>> m_Delivered_Item; //Inven database에 보내줄 map  //id로 전달될 거임 ㅇㅇ. 
	//차피 move로 비워줄거라 재사용 할 거긴 한데, move로 보내주는 경우도 있고, 한 번 쓱 읽고 다시 가져오는 경우도 생길 거라, 이건 더 생각해 봐야 할 듯, 모험 후에 처리할 부분이라 ㅇㅇ 

	unordered_map<Need_Moment, unique_ptr<Item>> m_Total_Item; //처음에 받을 총 아이템의 map

};
