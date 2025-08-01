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
	~ItemBank() = default; //���� �����ϰ� ���ǵ�, �Ƹ� ResourceManager.clean���� ���� �����شٰ� ������ų�� ����
public:
	void GetItem(string path); //json �о ���� load ��ų ���� -> �Ƹ� ��δ� �־���� �� �� 

	void GiveItem(Need_Moment Moment, ItemDatabase& InvenDatabase);



private:
	
	vector<unique_ptr<Item>> m_Delivered_Item; //Inven database�� ������ map  //id�� ���޵� ���� ����. 
	//���� move�� ����ٰŶ� ���� �� �ű� �ѵ�, move�� �����ִ� ��쵵 �ְ�, �� �� �� �а� �ٽ� �������� ��쵵 ���� �Ŷ�, �̰� �� ������ ���� �� ��, ���� �Ŀ� ó���� �κ��̶� ���� 

	unordered_map<Need_Moment, unique_ptr<Item>> m_Total_Item; //ó���� ���� �� �������� map

};
