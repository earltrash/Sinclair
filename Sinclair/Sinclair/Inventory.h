#pragma once
#include "pch.h"
#include "Item.h"
#include "SimpleMathHelper.h"
#include "Object.h"
#include "ItemGlobal.h"
#include "ResourceManager.h"

using Vec2 = MYHelper::Vector2F;


//�� ����� �ű�� 
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
    ID2D1Bitmap* bitmap;           // Direct2D ��Ʈ��
    D2D_RECT_F srcRect;                  // �ҽ� ���� (��������Ʈ ��Ʈ��)
    Vec2 position;                 // ������ ��ġ
    Vec2 size;                     // ������ ũ��
    float opacity;                 // ����

    UIBitmap() : bitmap(nullptr), opacity(1.0f) {}
};

class SlotBitmapController
{
public:
    SlotBitmapController() = default; //��Ʈ�ʿ��ٰ� ���� �־��ִ� ���� Inventory�� �ؾ���

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

    std::unordered_map<std::string, ID2D1Bitmap*> bitmaps; //slot�� bitmap 
};


class ItemDatabase //������ base -> Item�� �׳� �� �״�� ���� �ִ�?
{
public:
    void AddItemData(std::unique_ptr<Item> itemData)
    {
        itemDataMap[itemData->m_data.id] = std::move(itemData);
    }

    Item* GetItemData(const std::string& itemId)  //id�� string�̴ϱ� �ϴ� �̷��� ���� 
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
    std::unordered_map<std::string, std::unique_ptr<Item>> itemDataMap; //�긦 ���� ������ ���� ����
};

struct ItemInstance //�ϸ� �������� ���ؼ� ���� ���� �ű�� �������� �� ��?
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
    Inventory(Rect MWP) ; //�� ������Ʈ�� ��ü Rect, Object ������ ��� ������ ���� 

    //Initalize
    void InitializeRegions();
    void InitializeSlots();

    bool AddItem(string itemId, int count = 1);

    void LoadUIBitmaps();
    void LoadItemDatabase(Need_Moment Moment);

    void PackItem();
    void SetDatabase(unique_ptr<ItemDatabase> database); 

    // Scene �Ѿ �� 
    void UnlockRegion(int regionId);
    

private:
    std::vector<Region> regions;
    std::map<std::pair<int, int>, InventorySlot> slots;  // [region][index] -> slot
    SlotBitmapController controller; //���� ��Ʈ�� �������ִ� ģ�� 

    //unique_ptr<ItemDatabase> itemDatabase; //������ ���̽�, ���࿡ 
    ItemDatabase m_itemDatabase;
    Rect m_bound;

    UIBitmap windowBackground;//Window Bitmap;
    UIBitmap tooltipBackground;//tooltip image;
    UIBitmap closeButton;


    Rect titleBarBounds; // Ÿ��Ʋ�� ����
    Rect closeButtonBounds; // �ݱ� ��ư ����

    Vec2 windowPosition{ 0,0 };

    std::string currentHoveredItemName; //HoveredItemName
    std::string currentHoveredItemDescription; //�� ������ �����.

    int currentHoveredItemCount;
    Vec2 tooltipPosition;
};

class InventorySlot
{
public:
    ItemInstance item;             // ���� ������ ������ (ID, ����)
    bool isEnabled;

    UIBitmap backgroundBitmap;     // ���� ��� ��Ʈ��
    UIBitmap itemBitmap;           // ������ ������ ��Ʈ�� -> �����ۿ��� ����ϰų� �����ۿ��� �������ų�
    UIBitmap overlayBitmap;        // ȣ��/���� ȿ�� ��Ʈ��

    // UI��
    Rect bounds;
    bool isHovered;
    bool isSelected;

    InventorySlot() : isEnabled(true), isHovered(false), isSelected(false) {}

    void SetBounds(const Rect& rect)
    {
        bounds = rect;

        // ��� ��Ʈ�� ����
        backgroundBitmap.position = Vec2(rect.x, rect.y);
        backgroundBitmap.size = Vec2(rect.width, rect.height);

        // ������ ��Ʈ�� ���� (�е� ����)
        float padding = 4.0f;
        itemBitmap.position = Vec2(rect.x + padding, rect.y + padding);
        itemBitmap.size = Vec2(rect.width - padding * 2, rect.height - padding * 2);

        // �������� ����
        overlayBitmap.position = Vec2(rect.x, rect.y);
        overlayBitmap.size = Vec2(rect.width, rect.height);
    }

    // === �ٽ� ���� �Լ� ===
    void UpdateItemBitmap(SlotBitmapController* controller, ItemDatabase* itemDB)
    {

        if (!item.IsEmpty())
        {

            Item* data = itemDB->GetItemData(item.id);
            if (data)
            {
                // 2. ��Ʈ�� ���� (������ ������) //�� Item���� ��ϵ� Rect���� �������� ������� ���� �ؾ���. 
                //itemBitmap.bitmap = static_cast<Object*>(data)->GetComponent<SpriteRenderer>()->GetCurrentClip().GetBitmap();

                itemBitmap.bitmap = ResourceManager::Get().Get_ItemBank().GetItemClip(item.id).atlas.Get();
                itemBitmap.srcRect = ResourceManager::Get().Get_ItemBank().GetItemClip(item.id).srcRect;  // ��ü ������ ��� (������. ���� �̹��� ����� ���� ����.)
                itemBitmap.opacity = 1.0f;
            }
            else
            {
                itemBitmap.bitmap = nullptr; // ������ �����Ͱ� ������ ��Ʈ���� ���
            }
        }
        else
        {
            // �� ������ ��� empty_slot_item ��Ʈ���� ���

            itemBitmap.bitmap = controller->GetBitmap("slot_normal"); // �� ���Կ� �̹��� Ű
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

        std::cout << "���� ��� Ű: " << bgKey << std::endl;

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