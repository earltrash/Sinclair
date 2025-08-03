#pragma once
#include "pch.h"
#include "Item.h"
#include "SimpleMathHelper.h"
#include "Object.h"
#include "SpriteRenderer.h"
using Vec2 = MYHelper::Vector2F;

#include "UIWindow.h"

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
    Rect srcRect;                  // �ҽ� ���� (��������Ʈ ��Ʈ��)
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
        itemDataMap[itemData->m_data.name] = std::move(itemData);
    }

    Item* GetItemData(const std::string& itemId)  //id�� string�̴ϱ� �ϴ� �̷��� ���� 
    {
        auto it = itemDataMap.find(itemId);
        return it != itemDataMap.end() ? it->second.get() : nullptr;
    }
private:
    std::unordered_map<std::string, std::unique_ptr<Item>> itemDataMap; //�긦 ���� ������ ���� ����
};

struct ItemInstance //�ϸ� �������� ���ؼ� ���� ���� �ű�� �������� �� ��?
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
    Inventory(Rect MWP); //�� ������Ʈ�� ��ü Rect 

    void InitializeRegions();
    void InitializeSlots();
    bool AddItem(string itemId, int count = 1);
    void UnlockRegion(int regionId);
    void LoadUIBitmaps();
    void SetDatabase(unique_ptr<ItemDatabase> database); //
private:
    std::vector<Region> regions;
    std::map<std::pair<int, int>, InventorySlot> slots;  // [region][index] -> slot
    SlotBitmapController controller; //���� ��Ʈ�� �������ִ� ģ�� 
    unique_ptr<ItemDatabase> itemDatabase; //������ ���̽� 
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

            Item* data = itemDB->GetItemData(item.itemNM);
            if (data)
            {
                // 2. ��Ʈ�� ���� (������ ������)
                itemBitmap.bitmap = static_cast<Object*>(data)->GetComponent<SpriteRenderer>()->GetCurrentClip().GetBitmap();
                itemBitmap.srcRect = Rect(0, 0, 48, 48);  // ��ü ������ ��� (������. ���� �̹��� ����� ���� ����.)
                itemBitmap.opacity = 1.0f;
            }
            else
            {
                itemBitmap.bitmap = nullptr; // ������ �����Ͱ� ������ ��Ʈ���� ���
            }
        }
        else
        {
            // �� ������ ��� empty_slot_item ��Ʈ���� ���, �� ������ ���� �ϸ� ������ 

            itemBitmap.bitmap = controller->GetBitmap("slot_normal"); // �� ���Կ� �̹��� Ű
            itemBitmap.srcRect = Rect(0, 0, 48, 48); // �⺻ ũ��
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
        backgroundBitmap.srcRect = Rect(0, 0, 48, 48);  // ���� ũ�� (����)
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