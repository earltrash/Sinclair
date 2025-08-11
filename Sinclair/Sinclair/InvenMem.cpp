#include "InvenMem.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "EffectComponent.h"

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

std::unique_ptr<Item> ItemDatabase::TakeItemData(const std::string& itemId)
{
    auto it = itemDataMap.find(itemId);
    if (it == itemDataMap.end()) return nullptr;

    auto ptr = std::move(it->second); // 소유권 이동
    itemDataMap.erase(it);
    return ptr;
}

void ItemDatabase::ClearAllItems()
{
    itemDataMap.clear();
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
            if (data->m_data.Momnet == Need_Moment::Fam3_a)
            {
                itemBitmap.item = data;
                if (data->GetRenderInfo()->GetBitmap() != nullptr)   itemBitmap.item->ComponentClear();
                auto info = itemBitmap.item->GetRenderInfo();
                info->SetBitmap(ResourceManager::Get().Get_ItemBank().GetItemClip(item.id)->atlas.Get());
                info->SetSrcRect(ResourceManager::Get().Get_ItemBank().GetItemClip(item.id)->srcRect);
                itemBitmap.item->GetTransform().SetScale({ 0.8f, 0.8f }); // render쪽에 position 설정값도 수정 필요
                ProcessItem(itemBitmap.item, {1.f, 1.f, 1.f, 1.f});
            }
            //else if (data->m_data.Momnet == Need_Moment::Fam4_a)      // fam4_a는 아직 없나봄 넣으면 throw발생
            //{
            //    itemBitmap.item = data;
            //    if (data->GetRenderInfo()->GetBitmap() != nullptr)   itemBitmap.item->ComponentClear();
            //    auto info = itemBitmap.item->GetRenderInfo();
            //    info->SetBitmap(ResourceManager::Get().Get_ItemBank().GetItemClip(item.id)->atlas.Get());
            //    info->SetSrcRect(ResourceManager::Get().Get_ItemBank().GetItemClip(item.id)->srcRect);
            //    itemBitmap.item->GetTransform().SetScale({ 0.8f, 0.8f }); // render쪽에 position 설정값도 수정 필요
            //    ProcessItem(itemBitmap.item, { 1.f, 215.f/255.f, 0.f, 1.f });
            //}
            else
            {
                itemBitmap.bitmap = ResourceManager::Get().Get_ItemBank().GetItemClip(item.id)->atlas.Get();
                itemBitmap.srcRect = ResourceManager::Get().Get_ItemBank().GetItemClip(item.id)->srcRect;
                itemBitmap.opacity = 1.0f;
            }
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

void InventorySlot::ProcessItem(Item* data, D2D1_VECTOR_4F color)
{
    auto info = data->GetRenderInfo();

    auto glowBm = ResourceManager::Get().GetTexture("rays");
    auto ro1 = data->AddComponent<Rotate3D_Effect>(info, 0.f, glowBm->GetSize().width / 2.f, glowBm->GetSize().width / 2.f, 0.f, 0.f, 0.1f, glowBm.Get());
    auto ro2 = data->AddComponent<Rotate3D_Effect>(info, 0.f, glowBm->GetSize().width / 2.f, glowBm->GetSize().width / 2.f, 0.f, 0.f, -0.1f, glowBm.Get());
    auto roComp = data->AddComponent<Composite_Effect>(info, ro1->GetEffect(), ro2->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
    auto scaleRo = data->AddComponent<Scale_Effect>(info, 128.f, 128.f, 1.8f, 1.8f, roComp->GetEffect());
    //auto colRo = data->AddComponent<Color_Effect>(info, 1.f, 1.f, 224.f / 255.f, 1.f, scaleRo->GetEffect());
    auto colRo = data->AddComponent<Color_Effect>(info, color.x, color.y, color.z, color.w, scaleRo->GetEffect());
    auto offset = data->AddComponent<Offset_Effect>(info, info->GetRenderInfo().srcRect.left, 0.f, colRo->GetEffect());

    auto shadow = data->AddComponent<Shadow_Effect>(info, 3.f, color.x, color.y, color.z, color.w, info->GetBitmap());
    auto shadowScale = data->AddComponent<Scale_Effect>(info, info->GetRenderInfo().srcRect.left + 128.f, info->GetSize().height / 2.f, 1.1f, 1.1f, shadow->GetEffect());
    auto shadRoComp = data->AddComponent<Composite_Effect>(info, shadowScale->GetEffect(), offset->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);

    auto fComp = data->AddComponent<Composite_Effect>(info, info->GetBitmap(), shadRoComp->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
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

   // std::cout << "슬롯 배경 키: " << bgKey << std::endl;

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
    itemBitmap.Clear();
}