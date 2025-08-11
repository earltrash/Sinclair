#pragma once
#include "pch.h"
#include "ItemGlobal.h"
//#include "Inventory.h"
//#include "Item.h"
#include "Item.h"

using namespace  Microsoft::WRL;
using namespace std;

class ItemDatabase;
class Inventory;

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

	const ItemBitmapClip* GetItemClip(string name);
	void GiveItem(Need_Moment Moment, ItemDatabase& InvenDatabase);
	unique_ptr<Item> Get_Item_Status(string name);
	void clean();
	vector<string> GetItemIDsByMoment(Need_Moment moment);

	std::unique_ptr<Item> CreateItem(const std::string& id);

private:
	//���� ��� Map 
	unordered_multimap<Need_Moment, unique_ptr<Item>> m_S_Item; //ó���� ���� �� �������� map
	vector<shared_ptr<Item>> m_item;

	unordered_map<string, ItemBitmapClip> m_Atlas; // Item�� id�� �Ѱ��ָ� �ش��ϴ� Bitmap�� Rect�� ��ȯ�ϴ� map 

	std::unordered_map<std::string, std::unique_ptr<Item>> m_prototypes;

	//Load �� ���� �����. Ȥ�� ������ 
	unordered_map<std::string, ComPtr<ID2D1Bitmap1>> m_LoadedAtlases; //���ҽ� ĳ�ø� �Ƴ��� ���� map 
};
