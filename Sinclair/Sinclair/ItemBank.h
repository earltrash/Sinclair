#pragma once
#include "pch.h"
#include "Item.h"
#include "Potion.h"
#include "Material.h"
#include "Wearable.h"
#include "ItemGlobal.h"
#include "Inventory.h"

using namespace  Microsoft::WRL;
using namespace std;

struct ItemBitmapClip {
	ComPtr<ID2D1Bitmap1> atlas;   // � atlas���� (���� ���� ���)
	D2D_RECT_F srcRect;          // atlas �� Ŭ�� ��ġ
};

class ItemBank
{
public :
	ItemBank() = default;
	~ItemBank() = default; //���� �����ϰ� ���ǵ�, �Ƹ� ResourceManager.clean���� ���� �����شٰ� ������ų�� ����
public:
	void LoadItemStatus(const string& path); //json �о ���� load ��ų ���� -> �Ƹ� ��δ� �־���� �� �� 
	void LoadItemRect(const string& path); //json ->rect
	ComPtr<ID2D1Bitmap1> GetItemAtlas(const string& path); //Png
public:

	const ItemBitmapClip& GetItemClip(string name);
	void GiveItem(Need_Moment Moment, ItemDatabase& InvenDatabase);
	unique_ptr<Item> Get_Item_Status(string name);

private:
	//���� ��� Map 
	unordered_multimap<Need_Moment, unique_ptr<Item>> m_S_Item; //ó���� ���� �� �������� map
	unordered_map<string, ItemBitmapClip> m_Atlas; // Item�� id�� �Ѱ��ָ� �ش��ϴ� Bitmap�� Rect�� ��ȯ�ϴ� map 


	//Load �� ���� �����. Ȥ�� ������ 
	unordered_map<std::string, ComPtr<ID2D1Bitmap1>> m_LoadedAtlases; //���ҽ� ĳ�ø� �Ƴ��� ���� map 
};
