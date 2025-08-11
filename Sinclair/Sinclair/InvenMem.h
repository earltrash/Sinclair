// InventorySlot.h
#pragma once
#include "pch.h"
#include "SimpleMathHelper.h"
#include "Item.h"

#include "Renderer.h"

class ResourceManager;

using Vec2 = MYHelper::Vector2F;

struct Region
{
    int id;
    bool isEnabled;
    Rect bounds;
    Vec2 gridSize;
};

struct UIBitmap
{
    ID2D1Bitmap* bitmap;
    D2D_RECT_F srcRect;
    Vec2 position;
    Vec2 size;
    float opacity;

    UIBitmap() : bitmap(nullptr),
        srcRect{ 0,0,0,0 },
        position{ 0,0 },
        size{ 0,0 },
        opacity(1.0f) {}

};

template <class T>
inline void SafeRelease(T** ppT)
{
    if (*ppT != nullptr)
    {
        (*ppT)->Release();
        *ppT = nullptr;
    }
}

class SlotBitmapController
{
public:
    SlotBitmapController() = default;
    ~SlotBitmapController();

    ID2D1Bitmap* GetBitmap(const std::string& key);

    std::unordered_map<std::string, ID2D1Bitmap*> bitmaps;
};

class ItemDatabase
{
public:
    void AddItemData(std::unique_ptr<Item> itemData);
    Item* GetItemData(const std::string& itemId);
    std::unordered_map<std::string, std::unique_ptr<Item>>& GetMap();
    std::unique_ptr<Item> TakeItemData(const std::string& itemId);
    void ClearAllItems();



private:
    std::unordered_map<std::string, std::unique_ptr<Item>> itemDataMap;
};

struct ItemInstance
{
    std::string id;
    int count;

    ItemInstance(std::string id = "", int cnt = 0);
    bool IsEmpty() const;
};

class InventorySlot
{
public:
    ItemInstance item;
    bool isEnabled;

    UIBitmap backgroundBitmap;
    UIBitmap itemBitmap;
    UIBitmap overlayBitmap;

    Rect bounds;
    bool isHovered;
    bool isSelected;

    InventorySlot();

    void SetBounds(const Rect& rect);
    void UpdateItemBitmap(SlotBitmapController* controller, ItemDatabase* itemDB);
    void UpdateBackgroundBitmap(SlotBitmapController* bitmapManager);
    bool IsEmpty() const;
    void SetItem(std::string itemId, int count);
    void Clear();
};

struct DragState
{
    bool isDragging;
    InventorySlot* sourceSlot;
    ItemInstance draggedItem;
    Vec2 mousePos;
    UIBitmap dragBitmap;  // 드래그 중인 아이템 비트맵
};