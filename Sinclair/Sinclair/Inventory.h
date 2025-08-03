#pragma once
#include "pch.h"
#include "Item.h"
#include "SimpleMathHelper.h"
#include "Object.h"
#include "SpriteRenderer.h"
using Vec2 = MYHelper::Vector2F;

#include "UIWindow.h"

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
    Rect srcRect;                  // 소스 영역 (스프라이트 시트용)
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
        itemDataMap[itemData->m_data.name] = std::move(itemData);
    }

    Item* GetItemData(const std::string& itemId)  //id가 string이니깐 일단 이렇게 만듦 
    {
        auto it = itemDataMap.find(itemId);
        return it != itemDataMap.end() ? it->second.get() : nullptr;
    }
private:
    std::unordered_map<std::string, std::unique_ptr<Item>> itemDataMap; //얘를 직접 꺼내게 만들 던가
};

struct ItemInstance //하면 아이템을 통해서 읽을 값만 옮기는 형식으로 할 듯?
{
    std::string itemNM;
    int count;

    ItemInstance(std::string id = 0, int cnt = 0) : itemNM(id), count(cnt) {}
    bool IsEmpty() const { return itemNM.empty() || count == 0; }
};


class InventorySlot;
class Inventory
{
public:
    Inventory(Rect MWP); //이 오브젝트의 전체 Rect 

    void InitializeRegions();
    void InitializeSlots();
    bool AddItem(string itemId, int count = 1);
    void UnlockRegion(int regionId);
    void LoadUIBitmaps();
    void SetDatabase(unique_ptr<ItemDatabase> database); //
private:
    std::vector<Region> regions;
    std::map<std::pair<int, int>, InventorySlot> slots;  // [region][index] -> slot
    SlotBitmapController controller; //슬롯 비트맵 설정해주는 친구 
    unique_ptr<ItemDatabase> itemDatabase; //데이터 베이스 
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

            Item* data = itemDB->GetItemData(item.itemNM);
            if (data)
            {
                // 2. 비트맵 설정 (아이템 아이콘)
                itemBitmap.bitmap = static_cast<Object*>(data)->GetComponent<SpriteRenderer>()->GetCurrentClip().GetBitmap();
                itemBitmap.srcRect = Rect(0, 0, 48, 48);  // 전체 아이콘 사용 (예시임. 실제 이미지 사이즈에 따라 조절.)
                itemBitmap.opacity = 1.0f;
            }
            else
            {
                itemBitmap.bitmap = nullptr; // 아이템 데이터가 없으면 비트맵을 비움
            }
        }
        else
        {
            // 빈 슬롯일 경우 empty_slot_item 비트맵을 사용, 그 관리를 누가 하면 좋을까 

            itemBitmap.bitmap = controller->GetBitmap("slot_normal"); // 빈 슬롯용 이미지 키
            itemBitmap.srcRect = Rect(0, 0, 48, 48); // 기본 크기
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
        backgroundBitmap.srcRect = Rect(0, 0, 48, 48);  // 슬롯 크기 (예시)
    }


    bool IsEmpty() const { return item.IsEmpty(); }

    void SetItem(std::string itemId, int count)
    {
        item.itemNM = itemId;
        item.count = count;
    }

    void Clear()
    {
        item = ItemInstance();
    }
};