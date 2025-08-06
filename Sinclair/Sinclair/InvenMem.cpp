#include "InvenMem.h"
#include "ResourceManager.h"

SlotBitmapController::~SlotBitmapController()
{
    for (auto& pair : bitmaps)
    {
        SafeRelease(&pair.second);
    }
    bitmaps.clear();
}

ID2D1Bitmap* SlotBitmapController::GetBitmap(const std::string& key)
{
    auto it = bitmaps.find(key);
    return it != bitmaps.end() ? it->second : nullptr;
}

void ItemDatabase::AddItemData(std::unique_ptr<Item> itemData)
{
    itemDataMap[itemData->m_data.id] = std::move(itemData);
}

Item* ItemDatabase::GetItemData(const std::string& itemId)
{
    auto it = itemDataMap.find(itemId);
    return it != itemDataMap.end() ? it->second.get() : nullptr;
}

std::unordered_map<std::string, std::unique_ptr<Item>>& ItemDatabase::GetMap()
{
    return itemDataMap;
}

void ItemDatabase::DeleItem(std::string id)
{
    auto it = std::find_if(itemDataMap.begin(), itemDataMap.end(),
        [&](auto& pair) { return pair.second->m_data.id == id; });

    if (it != itemDataMap.end()) {
        auto result = std::move(it->second);
        itemDataMap.erase(it);
    }
}

ItemInstance::ItemInstance(std::string id, int cnt) : id(id), count(cnt) {}

bool ItemInstance::IsEmpty() const { return id.empty() || count == 0; }

InventorySlot::InventorySlot() : isEnabled(true), isHovered(false), isSelected(false) {}

void InventorySlot::SetBounds(const Rect& rect)
{
    bounds = rect;

    backgroundBitmap.position = Vec2(rect.x, rect.y);
    backgroundBitmap.size = Vec2(rect.width, rect.height);

    float padding = 4.0f;
    itemBitmap.position = Vec2(rect.x + padding, rect.y + padding);
    itemBitmap.size = Vec2(rect.width - padding * 2, rect.height - padding * 2);

    overlayBitmap.position = Vec2(rect.x, rect.y);
    overlayBitmap.size = Vec2(rect.width, rect.height);
}

void InventorySlot::UpdateItemBitmap(SlotBitmapController* controller, ItemDatabase* itemDB)
{
   
    if (!item.IsEmpty())
    {
        Item* data = itemDB->GetItemData(item.id);
        if (data)
        {
            itemBitmap.bitmap = ResourceManager::Get().Get_ItemBank().GetItemClip(item.id).atlas.Get();
            itemBitmap.srcRect = ResourceManager::Get().Get_ItemBank().GetItemClip(item.id).srcRect;
            itemBitmap.opacity = 1.0f;


            std::cout << &itemBitmap.bitmap << endl;

        }
        else
        {
            itemBitmap.bitmap = nullptr;
        }
    }
    else
    {
        itemBitmap.bitmap = controller->GetBitmap("slot_normal");
        D2D1_SIZE_F size = itemBitmap.bitmap->GetSize();
        itemBitmap.srcRect = D2D1::RectF(0.0f, 0.0f, size.width, size.height);
        itemBitmap.opacity = 1.0f;
    }
}

void InventorySlot::UpdateBackgroundBitmap(SlotBitmapController* bitmapManager)
{
    std::string bgKey;

    if (!isEnabled) {
        bgKey = "slot_disabled";
    }
    else if (isHovered) {
        bgKey = "slot_hover";
    }
    else if (isSelected) {
        bgKey = "slot_selected";
    }
    else {
        bgKey = "slot_normal";
    }

   // std::cout << "½½·Ô ¹è°æ Å°: " << bgKey << std::endl;

    backgroundBitmap.bitmap = bitmapManager->GetBitmap(bgKey);
    D2D1_SIZE_F size = backgroundBitmap.bitmap->GetSize();
    backgroundBitmap.srcRect = D2D1::RectF(0.0f, 0.0f, size.width, size.height);
}

bool InventorySlot::IsEmpty() const { return item.IsEmpty(); }

void InventorySlot::SetItem(std::string itemId, int count)
{
    item.id = itemId;
    item.count = count;
}

void InventorySlot::Clear()
{
    item = ItemInstance();
}