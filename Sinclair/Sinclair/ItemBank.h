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
	ComPtr<ID2D1Bitmap1> atlas;   // 어떤 atlas인지 (여러 개일 경우)
	D2D_RECT_F srcRect;          // atlas 내 클립 위치
};

class ItemBank
{
public :
	ItemBank() = default;
	~ItemBank() = default; //에셋 정리하고 갈건데, 아마 ResourceManager.clean에서 여기 접근해다가 정리시킬듯 ㅇㅇ
public:
	void LoadItemStatus(const string& path); //json 읽어서 파일 load 시킬 거임 -> 아마 경로는 넣어줘야 할 듯 
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
	//실제 사용 Map 
	unordered_multimap<Need_Moment, unique_ptr<Item>> m_S_Item; //처음에 받을 총 아이템의 map
	vector<shared_ptr<Item>> m_item;

	unordered_map<string, ItemBitmapClip> m_Atlas; // Item의 id를 넘겨주면 해당하는 Bitmap의 Rect를 반환하는 map 

	std::unordered_map<std::string, std::unique_ptr<Item>> m_prototypes;

	//Load 할 떄만 사용함. 혹은 디버깅용 
	unordered_map<std::string, ComPtr<ID2D1Bitmap1>> m_LoadedAtlases; //리소스 캐시를 아끼기 위한 map 
};
