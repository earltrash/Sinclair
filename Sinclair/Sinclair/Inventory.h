#pragma once
#include "pch.h"
#include "Item.h"
#include "SimpleMathHelper.h"
#include "Object.h"
#include "ItemGlobal.h"
#include "ResourceManager.h"

using Vec2 = MYHelper::Vector2F;


//얘 어따가 옮길까 
template <class T>
inline void SafeRelease(T** ppT)
{
    if (*ppT != nullptr)
    {
        (*ppT)->Release();
        *ppT = nullptr;
    }
}
struct Region
{
    int id;
    bool isEnabled;
    Rect bounds; //top bottom left right (right-left)
    Vec2 gridSize;  // 4x8
};

struct UIBitmap
{
    ID2D1Bitmap* bitmap;           // Direct2D 비트맵
    D2D_RECT_F srcRect;                  // 소스 영역 (스프라이트 시트용)
    Vec2 position;                 // 렌더링 위치
    Vec2 size;                     // 렌더링 크기
    float opacity;                 // 투명도

    UIBitmap() : bitmap(nullptr), opacity(1.0f) {}
};

class SlotBitmapController
{
public:
    SlotBitmapController() = default; //비트맵에다가 값을 넣어주는 일을 Inventory에 해야함

    ~SlotBitmapController() {
        for (auto& pair : bitmaps)
        {
            SafeRelease(&pair.second);
        }
        bitmaps.clear();
    }

    ID2D1Bitmap* GetBitmap(const std::string& key)
    {
        auto it = bitmaps.find(key);
        return it != bitmaps.end() ? it->second : nullptr;
    }

    std::unordered_map<std::string, ID2D1Bitmap*> bitmaps; //slot용 bitmap 
};


class ItemDatabase //아이템 base -> Item을 그냥 말 그대로 갖고 있는?
{
public:
    void AddItemData(std::unique_ptr<Item> itemData)
    {
        itemDataMap[itemData->m_data.id] = std::move(itemData);
    }

    Item* GetItemData(const std::string& itemId)  //id가 string이니깐 일단 이렇게 만듦 
    {
        auto it = itemDataMap.find(itemId);
        return it != itemDataMap.end() ? it->second.get() : nullptr;
    }

    std::unordered_map<std::string, std::unique_ptr<Item>>& GetMap() {
        return itemDataMap;
    }

    void DeleItem(string id)
    {
        auto it = std::find_if(itemDataMap.begin(), itemDataMap.end(),
            [&](auto& pair) { return pair.second->m_data.id == id; });

        if (it != itemDataMap.end()) {
            auto result = std::move(it->second);
            itemDataMap.erase(it); 
        }
    }

private:
    std::unordered_map<std::string, std::unique_ptr<Item>> itemDataMap; //얘를 직접 꺼내게 만들 던가
};

struct ItemInstance //하면 아이템을 통해서 읽을 값만 옮기는 형식으로 할 듯?
{
    std::string id;
    int count;

    ItemInstance(std::string id = 0, int cnt = 0) : id(id), count(cnt) {}
    bool IsEmpty() const { return id.empty() || count == 0; }
};


class InventorySlot;

class Inventory //: 
{
public:
    Inventory(Rect MWP) ; //이 오브젝트의 전체 Rect, Object 형식이 어떻게 될지ㅣ 몰라서 

    //Initalize
    void InitializeRegions();
    void InitializeSlots();

    bool AddItem(string itemId, int count = 1);

    void LoadUIBitmaps();
    void LoadItemDatabase(Need_Moment Moment);

    void PackItem();
    void SetDatabase(unique_ptr<ItemDatabase> database); 

    // Scene 넘어갈 때 
    void UnlockRegion(int regionId);
    

private:
    std::vector<Region> regions;
    std::map<std::pair<int, int>, InventorySlot> slots;  // [region][index] -> slot
    SlotBitmapController controller; //슬롯 비트맵 설정해주는 친구 

    //unique_ptr<ItemDatabase> itemDatabase; //데이터 베이스, 만약에 
    ItemDatabase m_itemDatabase;
    Rect m_bound;

    UIBitmap windowBackground;//Window Bitmap;
    UIBitmap tooltipBackground;//tooltip image;
    UIBitmap closeButton;


    Rect titleBarBounds; // 타이틀바 영역
    Rect closeButtonBounds; // 닫기 버튼 영역

    Vec2 windowPosition{ 0,0 };

    std::string currentHoveredItemName; //HoveredItemName
    std::string currentHoveredItemDescription; //현 아이템 설명글.

    int currentHoveredItemCount;
    Vec2 tooltipPosition;
};

class InventorySlot
{
public:
    ItemInstance item;             // 실제 아이템 데이터 (ID, 개수)
    bool isEnabled;

    UIBitmap backgroundBitmap;     // 슬롯 배경 비트맵
    UIBitmap itemBitmap;           // 아이템 아이콘 비트맵 -> 아이템에서 등록하거나 아이템에서 가져오거나
    UIBitmap overlayBitmap;        // 호버/선택 효과 비트맵

    // UI용
    Rect bounds;
    bool isHovered;
    bool isSelected;

    InventorySlot() : isEnabled(true), isHovered(false), isSelected(false) {}

    void SetBounds(const Rect& rect)
    {
        bounds = rect;

        // 배경 비트맵 설정
        backgroundBitmap.position = Vec2(rect.x, rect.y);
        backgroundBitmap.size = Vec2(rect.width, rect.height);

        // 아이템 비트맵 설정 (패딩 적용)
        float padding = 4.0f;
        itemBitmap.position = Vec2(rect.x + padding, rect.y + padding);
        itemBitmap.size = Vec2(rect.width - padding * 2, rect.height - padding * 2);

        // 오버레이 설정
        overlayBitmap.position = Vec2(rect.x, rect.y);
        overlayBitmap.size = Vec2(rect.width, rect.height);
    }

    // === 핵심 연결 함수 ===
    void UpdateItemBitmap(SlotBitmapController* controller, ItemDatabase* itemDB)
    {

        if (!item.IsEmpty())
        {

            Item* data = itemDB->GetItemData(item.id);
            if (data)
            {
                // 2. 비트맵 설정 (아이템 아이콘) //그 Item에서 등록된 Rect에서 가져오는 방식으로 진행 해야함. 
                //itemBitmap.bitmap = static_cast<Object*>(data)->GetComponent<SpriteRenderer>()->GetCurrentClip().GetBitmap();

                itemBitmap.bitmap = ResourceManager::Get().Get_ItemBank().GetItemClip(item.id).atlas.Get();
                itemBitmap.srcRect = ResourceManager::Get().Get_ItemBank().GetItemClip(item.id).srcRect;  // 전체 아이콘 사용 (예시임. 실제 이미지 사이즈에 따라 조절.)
                itemBitmap.opacity = 1.0f;
            }
            else
            {
                itemBitmap.bitmap = nullptr; // 아이템 데이터가 없으면 비트맵을 비움
            }
        }
        else
        {
            // 빈 슬롯일 경우 empty_slot_item 비트맵을 사용

            itemBitmap.bitmap = controller->GetBitmap("slot_normal"); // 빈 슬롯용 이미지 키
            D2D1_SIZE_F size = itemBitmap.bitmap->GetSize();
            itemBitmap.srcRect = D2D1::RectF(0.0f, 0.0f, size.width, size.height);
            itemBitmap.opacity = 1.0f;
        }
    }

    void UpdateBackgroundBitmap(SlotBitmapController* bitmapManager)
    {
        std::string bgKey;

        if (!isEnabled)
        {
            bgKey = "slot_disabled";
        }
        else if (isHovered)
        {
            bgKey = "slot_hover";
        }
        else if (isSelected)
        {
            bgKey = "slot_selected";
        }
        else
        {
            bgKey = "slot_normal";
        }

        std::cout << "슬롯 배경 키: " << bgKey << std::endl;

        backgroundBitmap.bitmap = bitmapManager->GetBitmap(bgKey);
        D2D1_SIZE_F size = backgroundBitmap.bitmap->GetSize();
        backgroundBitmap.srcRect = D2D1::RectF(0.0f, 0.0f, size.width, size.height);
    }


    bool IsEmpty() const { return item.IsEmpty(); }

    void SetItem(std::string itemId, int count)
    {
        item.id = itemId;
        item.count = count;
    }

    void Clear()
    {
        item = ItemInstance();
    }
};