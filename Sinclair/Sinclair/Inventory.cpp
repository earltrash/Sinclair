#include "InvenTory.h"
#include "ResourceManager.h"
#include "ItemBank.h"
#include "CursorManager.h"
#include "InvenMem.h"
#include "Potion.h"
#include "Wearable.h"
#include "Material.h"
//: UI_Object(MWP) //?ùÏÑ±?êÎ°ú ?ÅÏó≠?Ä ?ºÎã® ?§Ï†ï??(Inven ?êÍ∏∞ ?ÅÏó≠ ÎßêÏûÑ)
Inventory::Inventory() :UIWindow(UIWindowType::InventoryWindow, Vec2{ 0,0 }, Vec2{ 1208,825 })  // Vec2{ 1097,766 }) 
{

    m_bound = { 0,0,1208,825 }; // Ï¥àÍ∏∞ ?ÑÏπò  

    windowPosition = { m_bound.x,m_bound.y };

    InitializeRegions();
  //  std::cout << "[Inventory] Regions Ï¥àÍ∏∞???ÑÎ£å" << std::endl;
    LoadUIBitmaps(); //Î©§Î≤ÑÎ°?Í∞ñÍ≥† ?àÎäî ?´Í∏∞????Î∂ÄÎ∂?(?¥Í±¥ Inven?¥Îûë Í∞ôÏù¥??Í∞Ä?•ÏÑ±???àÏùå) 
   // std::cout << "[Inventory] UI ÎπÑÌä∏Îß?Î°úÎî© ?ÑÎ£å" << std::endl;

    InitializeSlots();
    std::cout << "[Inventory] ΩΩ∑‘ √ ±‚»≠ øœ∑·" << std::endl; // °Á ø©±‚ æ» ≥™ø¿∏È ±◊ æ»ø°º≠ ≈Õ¡¯ ∞≈

   LoadItemDatabase(Need_Moment::Gen_2);


   // LoadItemDatabase(Need_Moment::Adv);
    //LoadItemDatabase(Need_Moment::Syn);

    LoadItemDatabase(Need_Moment::Gen_2);

    //LoadItemDatabase(Need_Moment::Gen_3);
    //LoadItemDatabase(Need_Moment::Gen_4);


    PackItem();

    dragState.isDragging = false; // ?úÎûòÍ∑??ÅÌÉú Ï¥àÍ∏∞??

    //0. slot Init
    //1. ∫Ò∆Æ∏  ∑ŒµÂ
    //2. Item µ•¿Ã≈Õ ≥÷±‚

     //3. Update
     //4. Render

}


void Inventory::InitializeRegions()
{
    regions.resize(3); // 3∞≥¿« øµø™ (4x8, 4x8, 4x8)

    //≈∏¿Ã∆≤ πŸ øµø™, ¿ßƒ°∏∏ ¡∏¿Á«—¥Ÿ∞Ì ª˝∞¢«œº¿ 
    titleBarBounds = Rect(m_position.x, m_position.y, m_size.x, 42.0f);  //7+35

    //¥›±‚ πˆ∆∞ øµø™
    float closeButtonSize = 35.0f;
    closeButtonBounds = Rect(
        m_position.x + m_size.x - (closeButtonSize + 85), //14
        m_position.y + 30,      // 35ø¥¿Ω ø¯∑°
        closeButtonSize,
        closeButtonSize);

    //Inven ¿ßƒ° 
    windowBackground.position = m_position;
    windowBackground.size = Vec2(m_size.x, m_size.y);
    windowBackground.srcRect = D2D1::RectF(
        0.0f, 0.0f,
        static_cast<float>(m_size.x),
        static_cast<float>(m_size.y)
    );

    //¥›±‚ πˆ∆∞ ∑£¥ı ¿ßƒ° 
    closeButton.position = Vec2(closeButtonBounds.x, closeButtonBounds.y);
    closeButton.size = Vec2(closeButtonBounds.width, closeButtonBounds.height);
    closeButton.srcRect = D2D1::RectF(0, 0, closeButtonSize, closeButtonSize); // ?´Í∏∞ Î≤ÑÌäº ?¥Î?ÏßÄ ?¨Í∏∞

    TitleBar.srcRect = D2D1::RectF(m_position.x, m_position.y, m_size.x, 42.0f);

    float slotSize = 74.0f;
    float padding_x = 13.0f;
    float padding_y = 12.0f;

    float totalSlotDimension_x = slotSize + padding_x;
    float totalSlotDimension_y = slotSize + padding_y;

    // windowBounds¿« ≥ª∫Œ øµø™¿ª ±‚¡ÿ¿∏∑Œ πËƒ°
    float currentRegionX = m_position.x + 68.0f; // ¿©µµøÏ ¡¬√¯ ø©πÈ
    RegionOffset.push_back({ currentRegionX , m_bound.y });
    // Region 0: ±‚∫ª «ÿ±› øµø™ (4x8)
    regions[0].id = 0;
    regions[0].isEnabled = true;
    regions[0].gridSize = Vec2{ 4, 8 }; //4ø≠ 8«‡
    regions[0].bounds = Rect(
        currentRegionX,
        m_bound.y + 64.0f, // ¿©µµøÏ ªÛ¥‹ ø©πÈ + ≈∏¿Ã∆≤πŸ ∞¯∞£
        regions[0].gridSize.x * totalSlotDimension_x,
        regions[0].gridSize.y * totalSlotDimension_y
    );
    currentRegionX += (regions[0].bounds.width) + 24.0f; // ¥Ÿ¿Ω øµø™ Ω√¿€ X
    RegionOffset.push_back({ currentRegionX, m_bound.y });

    // Region 1: ¡ﬂ∞£ ¿·±› øµø™ (4x8)
    regions[1].id = 1;
    regions[1].isEnabled = true; // ¿·±›
    regions[1].gridSize = Vec2{ 4, 8 };
    regions[1].bounds = Rect(
        currentRegionX,
        m_bound.y + 64.0f,
        regions[1].gridSize.x * totalSlotDimension_x,
        regions[1].gridSize.y * totalSlotDimension_y
    );
    currentRegionX += regions[1].bounds.width + 24.0f;
    RegionOffset.push_back({ currentRegionX, m_bound.y });

    // Region 2: ø¿∏•¬  ¿·±› øµø™ (4x8)
    regions[2].id = 2;
    regions[2].isEnabled = true; // ¿·±›
    regions[2].gridSize = Vec2{ 4, 8 };
    regions[2].bounds = Rect(
        currentRegionX,
        m_bound.y + 64.0f,
        regions[2].gridSize.x * totalSlotDimension_x,
        regions[2].gridSize.y * totalSlotDimension_y
    );
}

void Inventory::InitializeSlots()
{
    float slotSize = 74.0f;
    float padding_x = 13.0f;
    float padding_y = 12.0f;

    float totalSlotDimension_x = slotSize + padding_x;
    float totalSlotDimension_y = slotSize + padding_y;

    for (int regionId = 0; regionId < regions.size(); ++regionId)
    {
        const Region& region = regions[regionId];

        for (int y = 0; y < region.gridSize.y; ++y)
        {
            for (int x = 0; x < region.gridSize.x; ++x)
            {
                int slotIndex = y * region.gridSize.x + x;
                std::pair<int, int> key = { regionId, slotIndex };

                InventorySlot& slot = slots[key];

                // ΩΩ∑‘ ¿ßƒ°¥¬ «ÿ¥Á Region¿« bounds∏¶ ±‚¡ÿ¿∏∑Œ ∞ËªÍ
                float slotX = region.bounds.x + x * totalSlotDimension_x;
                float slotY = region.bounds.y + y * totalSlotDimension_y;

                slot.SetBounds(Rect(slotX, slotY, slotSize, slotSize));
                slot.isEnabled = region.isEnabled; // øµø™¿« »∞º∫»≠ ªÛ≈¬ø° µ˚∂Û ΩΩ∑‘ »∞º∫»≠ º≥¡§
                slot.UpdateBackgroundBitmap(&controller); // √ ±‚ πË∞Ê º≥¡§
            }
        }
    }

    //UpdateSlotPositions();
}

bool Inventory::AddItem(string itemId, int count)
{
    const Item* itemData = m_itemDatabase.GetItemData(itemId);
    if (!itemData) return false;

    // ∫Û ΩΩ∑‘ √£±‚
    for (auto& [key, slot] : slots)
    {
        if (!slot.isEnabled) continue; // ∫Ò»∞º∫»≠µ» ΩΩ∑‘ø°¥¬ √ﬂ∞° ∫“∞°

        // Ω∫≈√ ∞°¥…«— æ∆¿Ã≈€¿Ã∂Û∏È ±‚¡∏ ΩΩ∑‘ø° «’ƒ°±‚ Ω√µµ (øπΩ√)
        if (itemData->IsStackable() && slot.item.id == itemId && (slot.item.count + count) <= itemData->maxCount) //maxCount¥¬ 10¿∏∑Œ «œ±‰ «‘. 
        {
            slot.item.count += count;
            slot.UpdateItemBitmap(&controller, &m_itemDatabase); // ∞≥ºˆ ∫Ø∞Ê¿∫ ∫Ò∆Æ∏  ∫Ø∞Ê¿Ã æ¯¡ˆ∏∏, »§Ω√ ∏∏¶ ∞ÊøÏ »£√‚
            return true;
        }

        if (slot.IsEmpty()) {
            slot.SetItem(itemId, count);
            slot.UpdateItemBitmap(&controller, &m_itemDatabase);
            return true;
        }
    }
    return false;
}

void Inventory::UnlockRegion(int regionId)
{
    if (regionId >= 0 && regionId < regions.size())
    {
        regions[regionId].isEnabled = true;
        for (int y = 0; y < regions[regionId].gridSize.y; ++y)
        {
            for (int x = 0; x < regions[regionId].gridSize.x; ++x)
            {
                int slotIndex = y * regions[regionId].gridSize.x + x;
                std::pair<int, int> key = { regionId, slotIndex };
                if (slots.count(key))
                {
                    slots[key].isEnabled = true;
                    slots[key].UpdateBackgroundBitmap(&controller);
                }
            }
        }
    }
}

void Inventory::Update() //?ÖÎ†•Ï≤òÎ¶¨Î•?Î∞õÏ? ?§Ïùå???¥Ïïº?òÎäî Í±∞Ïûñ?? //Ï∞®Ìîº ?¥Í±∞???òÎ? ?ÜÏùÑ ???? ?á„Öá ?ÖÎ†• Ï≤òÎ¶¨ Î∞õÏúºÎ©??åÏïÑ??Î≥ÄÍ≤???Î∂ÄÎ∂ÑÏù¥?? 
{
    // effectÍ∞Ä ?ÑÏöî?òÎã§Î©??¨Í∏∞??Update ?òÎäî Í≤?ÎßûÏïÑ. 
    if (!m_isActive) return;
    // ?¥Ìéô??Ïª¥Ìè¨?åÌä∏ ?ÖÎç∞?¥Ìä∏
    for (const auto& [key, slot] : slots)
    {
        if (slot.itemBitmap.item == nullptr) continue;
        slot.itemBitmap.item->Update();
    }

    // Ï∞??ÑÏπòÍ∞Ä Î≥ÄÍ≤ΩÎê† ?åÎßà???Ä?¥Ì?Î∞?Î∞??´Í∏∞ Î≤ÑÌäº???ÑÏπò???ÖÎç∞?¥Ìä∏
    titleBarBounds = Rect(m_position.x, m_position.y, m_size.x, 42.0f);
    float closeButtonSize = 35.0f;
    closeButtonBounds = Rect(
        m_position.x + m_size.x - (closeButtonSize + 75),   // ø¯∑° 77
        m_position.y + 30,
        closeButtonSize,
        closeButtonSize);

    UpdateSlotPositions();

    // ¿©µµøÏ πË∞Ê ¿ÃπÃ¡ˆ ¿ßƒ° æ˜µ•¿Ã∆Æ
    windowBackground.position = m_position;
}

void Inventory::Render()
{
    // ¿Ã∞≈ æ»«œ∏È ∞Ëº” ±◊∑¡¡¸.

    if (!m_isActive) return;

    // 1. ¿©µµøÏ πË∞Ê ∑ª¥ı∏µ
    if (windowBackground.bitmap)
    {
        D2D1_RECT_F destRect = D2D1::RectF(
            m_position.x, m_position.y,
            m_position.x + m_size.x, m_position.y + m_size.y
        );
        D2D1_RECT_F srcRect = windowBackground.srcRect;

        D2DRenderer::Get().DrawBitmap(
            static_cast<ID2D1Bitmap1*>(windowBackground.bitmap),
            destRect,
            srcRect,
            windowBackground.opacity);
    }
    //std::cout << " slot.windowBackground:Opacity" << windowBackground.opacity << endl;


    // 2. ≈∏¿Ã∆≤πŸ π◊ ¥›±‚ πˆ∆∞ ∑ª¥ı∏µ
   // RenderTitleBar();
    RenderCloseButton();

    // 3. ∏µÁ ΩΩ∑‘ ∑ª¥ı∏µ
    for (const auto& [key, slot] : slots)
    {
        RenderSlot(slot);
    }

    //if (dragState.isDragging && dragState.dragBitmap.bitmap)
    //{
    //    D2D1_RECT_F dragDestRect = D2D1::RectF(
    //        dragState.mousePos.x - dragState.dragBitmap.size.x / 2.0f, // ∏∂øÏΩ∫ ¡ﬂæ”ø° ø¿µµ∑œ
    //        dragState.mousePos.y - dragState.dragBitmap.size.y / 2.0f,
    //        dragState.mousePos.x + dragState.dragBitmap.size.x / 2.0f,
    //        dragState.mousePos.y + dragState.dragBitmap.size.y / 2.0f
    //    );

    //    D2DRenderer::Get().DrawBitmap(static_cast<ID2D1Bitmap1*>(dragState.dragBitmap.bitmap),
    //        dragDestRect, dragState.dragBitmap.srcRect, 1.0f);
    //}

    // 5. ≈¯∆¡ ∑ª¥ı∏µ (∞°¿Â ∏∂¡ˆ∏∑ø° ∑ª¥ı∏µ«œø© ¥Ÿ∏• UI ¿ßø° «•Ω√)
   /* if (showTooltip)
    {
        RenderTooltip(renderTarget);
    }*/
}



bool Inventory::HandleMouseHover(Vec2 mousePos) 
{
    if (!m_isActive)
    {
        // showTooltip = false;
        return false;
    }

    // ¿©µµøÏ µÂ∑°±◊ ¡ﬂ¿œ ∂ß
    if (isWindowDragging) // 
    {
        float deltaX = mousePos.x - dragStartMousePos.x;
        float deltaY = mousePos.y - dragStartMousePos.y;

        windowPosition = Vec2(dragStartWindowPos.x + deltaX, dragStartWindowPos.y + deltaY);

        m_position = Vec2(dragStartWindowPos.x + deltaX, dragStartWindowPos.y + deltaY); //∫Œ∏«—≈◊ ≥—∞‹¡Ÿ ∞™. -> ªÁΩ« 

        // ¿©µµøÏ πŸøÓµÂ, ≈∏¿Ã∆≤πŸ, ¥›±‚ πˆ∆∞ πŸøÓµÂ æ˜µ•¿Ã∆Æ
        m_bound.x = windowPosition.x;
        m_bound.y = windowPosition.y;
        titleBarBounds.x = windowPosition.x;
        titleBarBounds.y = windowPosition.y;

        float closeButtonSize = 35;

        closeButtonBounds.x = windowPosition.x + m_bound.width - (closeButtonSize + 14);
        closeButtonBounds.y = windowPosition.y + 5;

        // ΩΩ∑‘µÈ¿« ¿ßƒ°µµ æ˜µ•¿Ã∆Æ«ÿæﬂ «‘
        UpdateSlotPositions();

        return true;
    }
    else // ¿©µµøÏ µÂ∑°±◊ ¡ﬂ¿Ã æ∆¥“ ∂ß∏∏ ΩΩ∑‘ »£πˆ π◊ ≈¯∆¡ √≥∏Æ
    {
        //showTooltip = false;
        InventorySlot* hoveredSlot = nullptr;

        // ΩΩ∑‘ »£πˆ ªÛ≈¬ æ˜µ•¿Ã∆Æ
        for (auto& [key, slot] : slots)
        {
            bool wasHovered = slot.isHovered;
            // ΩΩ∑‘¿« «ˆ¿Á ∑ª¥ı∏µ ¿ßƒ°(¿Ãµøµ» √¢ø° µ˚∂Û ¥ﬁ∂Û¡¸)∏¶ ±‚¡ÿ¿∏∑Œ Contains √º≈©
            // slot.bounds¥¬ ¿ÃπÃ UpdateSlotPositionsø° ¿««ÿ «ˆ¿Á √¢ ¿ßƒ°ø° ∏¬∞‘ æ˜µ•¿Ã∆Æµ  (»§¿∫ ¿ÃπÃ √ ±‚»≠ Ω√ ø√πŸ∏£∞‘ º≥¡§µ )

            slot.isHovered = slot.bounds.Contains(mousePos); //æ»ø° ¿÷¿∏∏È hovered ¿Œµ•, ø©±‚º≠ ªÁΩ« ∑ª¥ı∞° æ»µ«æﬂ «œ∞≈µÁ 

            if (wasHovered != slot.isHovered)
            {
                slot.UpdateBackgroundBitmap(&controller);
            }

            if (slot.isHovered && !slot.IsEmpty())  //-> 
            {
                hoveredSlot = &slot; // «ˆ¿Á ∏∂øÏΩ∫∞° ø√∂Û∞£ ΩΩ∑‘
            }
        }

        // ?¥ÌåÅ ?ïÎ≥¥ ?ÖÎç∞?¥Ìä∏
        if (hoveredSlot != nullptr)
        {
             Item* data = m_itemDatabase.GetItemData(hoveredSlot->item.id); // ±◊ ¿˙¿Âµ» æ∆¿Ã≈€ ¡§∫∏ ∞°¡Æø¿¥¬ ∞≈¿”. §∑§∑ 
             //Item* data = hoveredSlot->item.

            if (data)
            {
                CursorManager::Get().SetHoveredItem(data); 
                Vec2 tooltipPos = mousePos + Vec2(10, 10);

                UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, tooltipPos); //?ÑÏπò Î≥ÄÍ≤ΩÏãú?§Í≥†, ?úÏÑ±?îÍπåÏßÄ 
            }
            return true;

        }

        else //¿Ã¡¶¥¬ ππ ≥™∞£∞≈∞⁄¡“...
        {
            CursorManager::Get().HoveredReleased(); //√ﬂ¿˚ ±›¡ˆ 
            UIManager::Get().CloseWindow(UIWindowType::InventoryTooltip); //«ÿ¡¶
            return true;

        }
        return false;

    }

    // æ∆¿Ã≈€ µÂ∑°±◊ ¡ﬂ¿œ ∂ß (∏∂øÏΩ∫ ¿ßƒ° æ˜µ•¿Ã∆Æ)
    if (dragState.isDragging)
    {
        dragState.mousePos = mousePos;
    }
}

bool Inventory::HandleMouseDown(Vec2 mousePos) //æÓ¬˜«« Inven ¿ßƒ° ≥ªø° ¿÷æÓæﬂ ¿Ã∞‘ √≥∏Æ∞° µ«¥¬ ∞≈¥œ±Ò 
{
    if (!m_isActive) return false;

    //Vec2 localPos = mousePos - m_position;

    InventorySlot* slot = GetSlotAt(mousePos);

    if (slot && !slot->IsEmpty() && slot->isEnabled)
    {

        // CursorManager∏¶ ≈Î«ÿ ¿¸ø™¿˚¿∏∑Œ æ∆¿Ã≈€ µÂ∑°±◊ Ω√¿€
        CursorManager::Get().StartItemDrag(slot->item.id, DragSource::Inventory, slot);
        CursorManager::Get().SetDraggedItem(m_itemDatabase.GetItemData(slot->item.id)); // Ω«¡¶ Item* ¿¸¥ﬁ -> id∏∏ ≥—∞‹º≠ πÆ¡¶∞° ª˝±‰µÌ. 

        InventorySlot* dragged_slot = CursorManager::Get().GetSourceSlot();

        if (dragged_slot != nullptr)
        {
            CursorManager::Get().SetItemCount(dragged_slot->item.count); //ƒ≠ø° ¿÷¥¯ æ∆¿Ã≈€ ∞πºˆ∏¶ ¿˙¿Â«ÿ ≥ı¿Ωø‰, ±◊≥… æ»¿¸«œ∞‘.. ¿œ¥‹ 
        }
        // æ∆¿Ã≈€ ¡§∫∏∏∏ CursorManagerø° ≥—±‚∞Ì ΩΩ∑‘ ∫ÒøÏ±‚.
        slot->Clear();
        slot->UpdateItemBitmap(&controller, &m_itemDatabase);

        return true; // ¿‘∑¬ √≥∏Æ øœ∑·
    }
    // øµø™ æ» ≈¨∏Ø Ω√ √÷ªÛ¥‹¿∏∑Œ.
    if (IsInInventoryBounds(mousePos))
    {
        std::cout << "πŸøÓµÂ øµø™ æ»¿Ã¥œ±Ó ¿‘∑¬√≥∏Æ«ﬂ∞Ì ≥°≥Ω¥Ÿ." << std::endl;
        UIManager::Get().OpenWindow(m_windowType);
        return true;
    }

    return false;
}



//bool Inventory::IsInBounds(const Vec2& screenPos) const
//{
//    // »≠∏È ¡¬«• -> ∑Œƒ√ ¡¬«• ∫Ø»Ø
//    Vec2 localPos = screenPos - m_position;
//
//    // µπˆ±◊ √‚∑¬
//    std::cout << "[IsInBounds]  ScreenPos: (" << screenPos.x << ", " << screenPos.y << ")\n";
//    std::cout << "              WindowPos: (" << m_position.x << ", " << m_position.y << ")\n";
//    std::cout << "              LocalPos:  (" << localPos.x << ", " << localPos.y << ")\n";
//    std::cout << "              Bound: L=" << m_bound.x << " T=" << m_bound.y << " x + width=" << m_bound.x + m_bound.width << " y + height=" << m_bound.y + m_bound.height << "\n";
//
//    bool inside = m_bound.Contains(localPos);
//
//    std::cout << "              Inside? " << (inside ? "YES" : "NO") << "\n\n";
//
//    return inside;
//}




bool Inventory::HandleMouseUp(Vec2 mousePos) //±◊ ≥ı¿∫ ¿ßƒ°ø° ¥Î«— øπø‹√≥∏Æ∏¶ «ÿæﬂ «‘. ¥©∞° Ω√¿€ «ﬂ¥¬¡ˆ æÀæ∆æﬂ «“ µÌ 
{
    if (!m_isActive) return false;

    if (CursorManager::Get().IsDragging())
    {
        // CursorManagerø°º≠ µÂ∑°±◊ ¡ﬂ¿Œ æ∆¿Ã≈€¿Ã ¿÷¥¬¡ˆ »Æ¿Œ
        InventorySlot* targetSlot = GetSlotAt(mousePos); // «ˆ¿Á ∏∂øÏΩ∫ ¿ßƒ°¿« ΩΩ∑‘¿ª ¥ŸΩ√ √£¿Ω
        Item* draggedItemData = CursorManager::Get().GetDraggedItem(); // CursorManagerø°º≠ µÂ∑°±◊ ¡ﬂ¿Œ æ∆¿Ã≈€ ∞°¡Æø¿±‚
        DragSource dragSource = CursorManager::Get().GetDragSource();

        // ∏∏æ‡ ¿Â∫Ò√¢ø°º≠ ø¬∞≈∏È Ω∫≈» æ˜µ•¿Ã∆Æ
        if (dragSource == DragSource::Equipment)
        {
            if (auto* statWindow = dynamic_cast<StatWindow*>(UIManager::Get().GetWindow(UIWindowType::StatsWindow)))
            {
                statWindow->UpdateTotalStats();
            }
        }

        bool placed = false;

        if (targetSlot && draggedItemData)
        {
            // Ω∫≈√ ∞°¥…«— æ∆¿Ã≈€¿Ã∂Û∏È «’ƒ°±‚ Ω√µµ
            if (draggedItemData->IsStackable() &&
                targetSlot->item.id == draggedItemData->m_data.id &&
                (targetSlot->item.count + 1) <= draggedItemData->maxCount) // count¥¬ CursorManagerø°º≠ ∞¸∏Æ«ÿæﬂ «‘
            {
                //std::cout << "Ω∫≈√ ∞°¥…. æ∆¿Ã≈€ «’ƒ•∞≈¿”." << std::endl; ¡∂∞«¿∫ ∏¬∞‘ µÈæÓø».
                targetSlot->item.count += 1; // µÂ∑°±◊µ» æ∆¿Ã≈€¿« Ω«¡¶ count∏¶ ¥ı«ÿæﬂ «‘
                targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
                placed = true;
            }
            else if (targetSlot->IsEmpty())
            {
                int Count = CursorManager::Get().GetItemCount(); //±‚¡∏¿∫ ±◊≥… databaseø°º≠ ∞°¡Æø¬ ∞≈∂Û 1∞™¿Ã π´¡∂∞« ≥™ø» §∑§∑ 

                // ∫Û ΩΩ∑‘ø° µÂ∑”
                targetSlot->SetItem(draggedItemData->m_data.id, Count); // count¥¬ CursorManagerø°º≠ ∞¸∏Æ«ÿæﬂ «‘
                targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
                placed = true;
            }
            else // ΩΩ∑‘¿Ã ∫ÒæÓ¿÷¡ˆ æ ∞Ì Ω∫≈√ ∫“∞°¥…«œ∞≈≥™ ¥Ÿ∏• æ∆¿Ã≈€¿Œ ∞ÊøÏ (±≥»Ø ∑Œ¡˜)
            {
                // ø¯∫ª ΩΩ∑‘ ¡§∫∏ ∞°¡Æø¿±‚
                InventorySlot* sourceSlot = CursorManager::Get().GetSourceSlot();

                if (sourceSlot)
                {
                    // «ˆ¿Á ≈∏∞Ÿ ΩΩ∑‘¿« æ∆¿Ã≈€¿ª ¿”Ω√ ¿˙¿Â
                    ItemInstance tempItem = targetSlot->item;

                    // ≈∏∞Ÿ ΩΩ∑‘ø° µÂ∑°±◊µ» æ∆¿Ã≈€ ≥ı±‚
                    int Count = CursorManager::Get().GetItemCount(); //±‚¡∏¿∫ ±◊≥… databaseø°º≠ ∞°¡Æø¬ ∞≈∂Û 1∞™¿Ã π´¡∂∞« ≥™ø» §∑§∑ 
                    targetSlot->SetItem(draggedItemData->m_data.id, Count);

                    std::cout << "∞πºˆø‰ " << Count << endl;

                    //targetSlot->SetItem(draggedItemData->m_data.id, 1);
                    targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);

                    // ø¯∫ª ΩΩ∑‘ø° ±≥»Øµ» æ∆¿Ã≈€ ≥ı±‚
                    if (dragSource == DragSource::Enhancement || dragSource == DragSource::Synthesis)
                    {
                        AddItem(draggedItemData->m_data.id, Count);
                    }
                    else
                    {
                        sourceSlot->SetItem(tempItem.id, tempItem.count);
                        sourceSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
                    }
                    placed = true;
                }
                else
                {
                    int Count = CursorManager::Get().GetItemCount(); //±‚¡∏¿∫ ±◊≥… databaseø°º≠ ∞°¡Æø¬ ∞≈∂Û 1∞™¿Ã π´¡∂∞« ≥™ø» §∑§∑ 
                    // ∫Û ΩΩ∑‘ø° √ﬂ∞°
                    ItemInstance tempItem = targetSlot->item;
                    targetSlot->SetItem(draggedItemData->m_data.id, Count);
                    targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
                    AddItem(tempItem.id, tempItem.count);

                    placed = true;
                }
            }
        }
        if (placed)
        {
            // æ∆¿Ã≈€¿ª º∫∞¯¿˚¿∏∑Œ ≥ıæ“¿∏π«∑Œ µÂ∑°±◊∏¶ øœ¿¸»˜ ¡æ∑·«œ∞Ì º∫∞¯¿ª π›»Ø
            CursorManager::Get().EndItemDrag();
            CursorManager::Get().RE_ItemCount();
            return true;
        }
        else
        {
            // ¿Ã∂ß∏∏ »£√‚«ÿæﬂ«‘.
            return false;
        }
    }
    if (IsInInventoryBounds(mousePos))
    {
        UIManager::Get().OpenWindow(m_windowType);
        return true;
    }

    return false;
}

bool Inventory::HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source)
{
    if (!draggedItem) return false;

    // µÂ∑°±◊ Ω√¿€¡°¿Ã ¿Œ∫•≈‰∏Æø¥¥¬¡ˆ »Æ¿Œ
    if (source == DragSource::Inventory)
    {
        InventorySlot* sourceSlot = CursorManager::Get().GetSourceSlot();
        if (sourceSlot)
        {
            // ø¯∫ª ΩΩ∑‘ø° µÂ∑°±◊«ﬂ¥¯ æ∆¿Ã≈€∞˙ '∞≥ºˆ'∏¶ ±◊¥Î∑Œ ∫πø¯
            sourceSlot->SetItem(draggedItem->m_data.id, CursorManager::Get().GetItemCount());
            sourceSlot->UpdateItemBitmap(&controller, &m_itemDatabase);


            ItemDrop(draggedItem);
            // µÂ∑°±◊ ¡æ∑· π◊ æ∆¿Ã≈€ ∞≥ºˆ √ ±‚»≠
            UIManager::Get().OpenWindow(m_windowType);
            CursorManager::Get().EndItemDrag();
            CursorManager::Get().RE_ItemCount(); // ∞≥ºˆ √ ±‚»≠
            return true; // √≥∏Æ øœ∑·
        }
    }
    // ∏µÁ √¢¿ª ∆Æ∑Á∑Œ πﬁ∞‘ «œ∏È ª˝±‚¥¬ πÆ¡¶¡°¿∫ π∫∞°ø‰?
    else //if (source == DragSource::Enhancement || source == DragSource::Equipment || source == DragSource::Synthesis) 
    {
        AddItem(draggedItem->m_data.id, 1);
        CursorManager::Get().EndItemDrag();
        CursorManager::Get().RE_ItemCount(); // ∞≥ºˆ √ ±‚»≠
        return true; // √≥∏Æ øœ∑·
    }

    // ¥Ÿ∏• √¢ø°º≠ ø¬ ∞ÊøÏ µÓ ¥Ÿ∏• Ω√≥™∏Æø¿¥¬ ø©±‚º≠ √≥∏Æ«œ¡ˆ æ ¿Ω
    return false;
}

bool Inventory::ConsumePendingPotion()
{
    if (!m_pendingPotionSlot) return false;

    // ΩΩ∑‘/æ∆¿Ã≈€ ¿Ø»øº∫ √º≈©
    if (m_pendingPotionSlot->IsEmpty()) { m_pendingPotionSlot = nullptr; return false; } 

    Item* data = m_itemDatabase.GetItemData(m_pendingPotionSlot->item.id);
    if (!data || dynamic_cast<Potion*>(data) == nullptr) { m_pendingPotionSlot = nullptr; return false; }

    // Ω∫≈√ 1 ∞®º“ °Ê 0¿Ã∏È ∫ÒøÏ±‚ °Ê ∫Ò∆Æ∏  ∞ªΩ≈
    m_pendingPotionSlot->item.count -= 1;
    if (m_pendingPotionSlot->item.count <= 0)
        m_pendingPotionSlot->Clear(); //∆˜¿Œ≈Õ¥¬ æ» æ¯æ⁄ 

    m_pendingPotionSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
    m_pendingPotionSlot = nullptr;
    return true;
}


bool Inventory::HandleDoubleClick(Vec2 mousePos)
{
    

    return false;
}

bool Inventory::HandleMouseRight(Vec2 mousePos) //ªÁøÎ«— æ∆¿Ã≈€¿« ∆˜¿Œ≈Õ∏¶ πﬁæ∆øÕº≠ æ¯æ÷¥¬ Ωƒ¿∏∑Œ ¡¯«‡«ÿæﬂ «“µÌ. 
{
    InventorySlot* slot = GetSlotAt(mousePos);

    if (slot && !slot->IsEmpty())
    {
        // æ∆¿Ã≈€ µ•¿Ã≈Õ∫£¿ÃΩ∫ø°º≠ æ∆¿Ã≈€ ∆˜¿Œ≈Õ ∞°¡Æø¿±‚
        Item* itemData = m_itemDatabase.GetItemData(slot->item.id);

        if (Material* material = dynamic_cast<Material*>(itemData))
        {
            std::cout << "«ÿ¥Á æ∆¿Ã≈€¿∫ ¿Â∫Ò ¬¯øÎ¿Ã ∫“∞°¥… «’¥œ¥Ÿ.\n";
            return false;
        }
        else if (Potion* potion = dynamic_cast<Potion*>(itemData))
        {
            // ∆˜º« √≥∏Æ ∑Œ¡˜
            int much = potion->GetMuch() - 1;
            m_pendingPotionSlot = slot;
            UIManager::Get().ShowPotionWindow(much);
        }
        else
        {
            // ∆˜º«¿ª ≥ π´ ∏π¿Ã ≈¨∏Ø«œ∏È ø¿∑˘∞° ≈Õ¡¸ ±◊∞≈ «ÿ∞·«ﬂ¿Ω 
            Wearable* wear = dynamic_cast<Wearable*>(itemData);

            //
            wear->m_data.wearablePart;
            // ¬¯øÎ æ∆¿Ã≈€ √≥∏Æ ∑Œ¡˜
            UIManager::Get().OpenWindow(UIWindowType::EquipmentWindow);
            auto equipWindow = dynamic_cast<EquipmentWindow*>(UIManager::Get().GetWindow(UIWindowType::EquipmentWindow));

            if (equipWindow)
            {
                slot->Clear();
                ItemDrop(wear);
                equipWindow->EquipItem(wear);
            }

            if (auto* statWindow = dynamic_cast<StatWindow*>(UIManager::Get().GetWindow(UIWindowType::StatsWindow)))
            {
                statWindow->UpdateTotalStats();
            }
        }
        return true;
    }
    if (IsInBounds(mousePos))
    {
        return true;
    }

    return false;
}


  


UIWindowType Inventory::GetType()
{
    return m_windowType;
}

InventorySlot* Inventory::GetSlotAt(const Vec2& pos)
{
    if (!m_isActive || isWindowDragging) return nullptr; // ¿©µµøÏ ∫Ò»∞º∫ ∂«¥¬ µÂ∑°±◊ ¡ﬂø°¥¬ ΩΩ∑‘ ≈¨∏Ø ∫“∞°

    for (auto& [key, slot] : slots)
    {
        // ΩΩ∑‘¿« bounds¥¬ ¿ÃπÃ UpdateSlotPositionsø° ¿««ÿ «ˆ¿Á √¢ ¿ßƒ°ø° ∏¬∞‘ æ˜µ•¿Ã∆Æµ«æÓ ¿÷¿Ω
        if (slot.bounds.Contains(pos))
        {
            return &slot;
        }
    }
    return nullptr;
}

void Inventory::RenderTitleBar()
{
    auto& renderer = D2DRenderer::Get();
    auto context = renderer.GetContext();

    if (context)
    {
        // 1. ≈∏¿Ã∆≤πŸ øµø™ ªˆªÛ √‚∑¬ (∞Ì¡§ ªˆ ∫Í∑ØΩ√ ªÁøÎ)
        ComPtr<ID2D1SolidColorBrush> darkBrush;
        context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkSlateGray, 0.7f), &darkBrush);

        D2D1_RECT_F rect = titleBarBounds.ToD2DRect();
        context->FillRectangle(rect, darkBrush.Get());


        D2DRenderer::Get().DrawBitmap(static_cast<ID2D1Bitmap1*>(TitleBar.bitmap), rect, TitleBar.srcRect, 1.0f);

    }
    // 2. ≈ÿΩ∫∆Æ √‚∑¬
    auto textBrush = renderer.GetTBrush();
    auto textFormat = renderer.GetTFormat();

    if (textFormat && textBrush)
    {
        context->DrawText(
            L"Inventory",
            static_cast<UINT32>(wcslen(L"Inventory")),
            textFormat.Get(),
            D2D1::RectF(
                titleBarBounds.x + 10,
                titleBarBounds.y + 5,
                titleBarBounds.x + titleBarBounds.width - 10,
                titleBarBounds.y + titleBarBounds.height - 5
            ),
            textBrush.Get()
        );
    }
}

void Inventory::RenderCloseButton()
{
    if (closeButton.bitmap)
    {
        // ¥›±‚ πˆ∆∞ ¿ßƒ°∏¶ «ˆ¿Á √¢ ¿ßƒ°ø° µ˚∂Û æ˜µ•¿Ã∆Æ
        float rightMargin = 85; // ø¯∑° 65
        Vec2 currentCloseButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 30 };

        D2D1_RECT_F destRect = D2D1::RectF(
            currentCloseButtonPos.x, currentCloseButtonPos.y,
            currentCloseButtonPos.x + 35, currentCloseButtonPos.y + 35
        );
        D2D1_RECT_F srcRect = closeButton.srcRect;

        D2DRenderer::Get().DrawBitmap(
            static_cast<ID2D1Bitmap1*>(closeButton.bitmap),
            destRect,
            srcRect,
            closeButton.opacity);
    }
}

void Inventory::RenderSlot(const InventorySlot& slot)
{
    // ΩΩ∑‘ πË∞Ê ∑ª¥ı∏µ
    if (slot.backgroundBitmap.bitmap)
    {
        D2D1_RECT_F destRect = D2D1::RectF(
            slot.bounds.x, slot.bounds.y,
            slot.bounds.x + slot.bounds.width,
            slot.bounds.y + slot.bounds.height
        );
        D2D1_RECT_F srcRect = slot.backgroundBitmap.srcRect;

        D2DRenderer::Get().DrawBitmap(
            static_cast<ID2D1Bitmap1*>(slot.backgroundBitmap.bitmap),
            destRect,
            srcRect,
            slot.backgroundBitmap.opacity);
    }

    // ∫Ò»∞º∫»≠µ» ΩΩ∑‘¿∫ ∫Ûslot¿ÃπÃ¡ˆ ±◊∏≤.
    if (!slot.isEnabled)
    {
        //std::cout << "∫Ò»∞º∫»≠µ» ΩΩ∑‘¿Ãπ«∑Œ πË∞Ê∏∏ ∑ª¥ı∏µ«œ∞Ì ¡æ∑·" << std::endl;
        return;
    }

    // 2. ?ÑÏù¥???ÑÏù¥ÏΩ??åÎçîÎß?
    if (!slot.IsEmpty() && slot.itemBitmap.item != nullptr)
    {
        auto info = slot.itemBitmap.item->GetRenderInfo();
        slot.itemBitmap.item->SetPosition({ slot.itemBitmap.position.x - 128.f * 0.8f + 35.f, slot.itemBitmap.position.y - 128.f * 0.8f + 35.f });
        D2DRenderer::Get().DrawBitmap(info->GetRenderInfo());

        // 3. ?ÑÏù¥??Í∞úÏàò ?çÏä§??
        if (slot.item.count > 1)
        {
            std::wstring countText = std::to_wstring(slot.item.count);
            // ?çÏä§???ÑÏπòÎ•??¨Î°Ø ?∞Ï∏° ?òÎã®??Î∞∞Ïπò (?àÏãú)
            D2D1_RECT_F textRect = D2D1::RectF(
                slot.bounds.x + slot.bounds.width - 30, // ?∞Ï∏° ?ïÎ†¨
                slot.bounds.y + slot.bounds.height - 20, // ?òÎã® ?ïÎ†¨
                slot.bounds.x + slot.bounds.width,
                slot.bounds.y + slot.bounds.height
            );
            D2DRenderer::Get().GetContext()->DrawText(
                countText.c_str(),
                static_cast<UINT32>(countText.length()),
                D2DRenderer::Get().GetTFormat().Get(),
                &textRect,
                D2DRenderer::Get().GetTBrush().Get()
            );
        }
    }
    else if (!slot.IsEmpty() && slot.itemBitmap.bitmap)
    {
        D2D1_RECT_F itemDestRect = D2D1::RectF(
            slot.itemBitmap.position.x, // itemBitmap.position¿∫ SetBoundsø°º≠ ¿ÃπÃ ΩΩ∑‘ boundsø° ∏¬√Á º≥¡§µ 
            slot.itemBitmap.position.y,
            slot.itemBitmap.position.x + slot.itemBitmap.size.x,
            slot.itemBitmap.position.y + slot.itemBitmap.size.y
        );
        D2D1_RECT_F itemSrcRect = slot.itemBitmap.srcRect;
        itemSrcRect.left = itemSrcRect.left + 85.f;
        itemSrcRect.top = itemSrcRect.top + 85.f;
        itemSrcRect.right = itemSrcRect.right - 85.f;
        itemSrcRect.bottom = itemSrcRect.bottom - 85.f;

        D2DRenderer::Get().DrawBitmap(
            static_cast<ID2D1Bitmap1*>(slot.itemBitmap.bitmap),
            itemDestRect,
            itemSrcRect,
            slot.itemBitmap.opacity

        );
        //  std::cout << " slot.itemBitmap:Opacity" << slot.itemBitmap.opacity << endl;


          // 3. æ∆¿Ã≈€ ∞≥ºˆ ≈ÿΩ∫∆Æ
        if (slot.item.count > 1)
        {
            std::wstring countText = std::to_wstring(slot.item.count);
            // ≈ÿΩ∫∆Æ ¿ßƒ°∏¶ ΩΩ∑‘ øÏ√¯ «œ¥‹ø° πËƒ° (øπΩ√)
            D2D1_RECT_F textRect = D2D1::RectF(
                slot.bounds.x + slot.bounds.width - 30, // øÏ√¯ ¡§∑ƒ
                slot.bounds.y + slot.bounds.height - 20, // «œ¥‹ ¡§∑ƒ
                slot.bounds.x + slot.bounds.width,
                slot.bounds.y + slot.bounds.height
            );
            D2DRenderer::Get().GetContext()->DrawText(
                countText.c_str(),
                static_cast<UINT32>(countText.length()),
                D2DRenderer::Get().GetTFormat().Get(),
                &textRect,
                D2DRenderer::Get().GetTBrush().Get()
            );
        }
    }
}

void Inventory::UpdateSlotPositions() // -> widndow ±‚¡ÿ¿∏∑Œ µ«∞Ì ¿÷¡ˆ æ æ∆ø‰ 
{
    float slotSize = 74.0f;
    float padding_x = 13.0f;
    float padding_y = 12.0f;

    /* float totalSlotDimension_x = windowPosition.x + slotSize + padding_x;
     float totalSlotDimension_y = windowPosition.y+ slotSize + padding_y;*/

    float totalSlotDimension_x = slotSize + padding_x;
    float totalSlotDimension_y = slotSize + padding_y;

    // Regions¿« bounds∏¶ √¢¿« «ˆ¿Á ¿ßƒ°∏¶ ±‚¡ÿ¿∏∑Œ æ˜µ•¿Ã∆Æ
    float currentRegionX = m_position.x + 68.0f;
    regions[0].bounds = Rect(
        currentRegionX,
        m_position.y + 64.0f,
        regions[0].gridSize.x * totalSlotDimension_x,
        regions[0].gridSize.y * totalSlotDimension_y
    );
    currentRegionX += (regions[0].bounds.width) + 24.0f;
    regions[1].bounds = Rect(
        currentRegionX,
        m_position.y + 64.0f,
        regions[1].gridSize.x * totalSlotDimension_x,
        regions[1].gridSize.y * totalSlotDimension_y
    );
    currentRegionX += regions[1].bounds.width + 24.0f;
    regions[2].bounds = Rect(
        currentRegionX,
        m_position.y + 64.0f,
        regions[2].gridSize.x * totalSlotDimension_x,
        regions[2].gridSize.y * totalSlotDimension_y
    );

    for (int regionId = 0; regionId < regions.size(); ++regionId)
    {
        const Region& region = regions[regionId];

        for (int y = 0; y < region.gridSize.y; ++y)
        {
            for (int x = 0; x < region.gridSize.x; ++x)
            {
                int slotIndex = y * region.gridSize.x + x;
                std::pair<int, int> key = { regionId, slotIndex };

                if (slots.count(key))
                {
                    InventorySlot& slot = slots[key];

                    
                    //float slotX = region.bounds.x + x * totalSlotDimension_x;
                    //float slotY = region.bounds.y + y * totalSlotDimension_y;

                    //float slotX = m_bound.x + RegionOffset[regionId].x + x * totalSlotDimension_x;
                    //float slotY = m_bound.y + RegionOffset[regionId].y + y * totalSlotDimension_y+ 64.0f;

                    // region.bounds.xøÕ region.bounds.y∑Œ ΩΩ∑‘ ¿ßƒ° ∞ËªÍ«ÿæﬂ«‘.
                    float slotX = region.bounds.x + x * totalSlotDimension_x;
                    float slotY = region.bounds.y + y * totalSlotDimension_y;

                    slot.SetBounds(Rect(slotX, slotY, slotSize, slotSize));
                }
            }
        }
    }
}

// ΩΩ∑‘¿∫ ∫Òøˆ¡ˆ¥¬µ• ¿Â∫Ò√¢ø° ¿÷¥¬∞‘ æÓ∂ª∞‘ ≤ø¿œ¡ˆ ∏∏•¥Ÿ. 
// ¿œ¥‹¿∫ ∏∏µÈ±‚∏∏«ﬂ¡ˆ æ∆π´∞Õµµ √ﬂ∞° æ»«ﬂ¿∏¥œ±Ó.
void Inventory::ClearAllSlots()
{
    // ∏µÁ ΩΩ∑‘ º¯»∏«œ∏Èº≠ √ ±‚»≠
    for (auto& [key, slot] : slots)
    {
        // ΩΩ∑‘ ≥ªøÎ ≈¨∏ÆæÓ
        slot.Clear();

        // æ∆¿Ã≈€ ∫Ò∆Æ∏  æ˜µ•¿Ã∆Æ (∫Û ΩΩ∑‘¿∏∑Œ)
        slot.UpdateItemBitmap(&controller, &m_itemDatabase);

        // πË∞Ê ∫Ò∆Æ∏  æ˜µ•¿Ã∆Æ (±‚∫ª ªÛ≈¬∑Œ)
        slot.UpdateBackgroundBitmap(&controller);
    }
    m_itemDatabase.ClearAllItems();

    std::cout << "[Inventory] ∏µÁ ΩΩ∑‘¿Ã √ ±‚»≠µ«æ˙Ω¿¥œ¥Ÿ." << std::endl;
}

bool Inventory::IsInInventoryBounds(const Vec2& screenPos) const
{
    bool inside = screenPos.x >= m_position.x && screenPos.x <= m_position.x + m_size.x &&
        screenPos.y >= m_position.y && screenPos.y <= m_position.y + m_size.y;

    // µπˆ±◊ √‚∑¬
    std::cout << "[IsInInventoryBounds] ScreenPos: (" << screenPos.x << ", " << screenPos.y << ")\n";
    std::cout << "                     WindowPos: (" << m_position.x << ", " << m_position.y << ")\n";
    std::cout << "                     WindowSize: (" << m_size.x << ", " << m_size.y << ")\n";
    std::cout << "                     Inside? " << (inside ? "YES" : "NO") << "\n\n";

    return inside;
}

ItemDatabase& Inventory::GetItemBase()
{
    return m_itemDatabase;
}

void Inventory::PackItem() 
{
    for (const auto& [Id, Item] : m_itemDatabase.GetMap())
    {
        //std::cout << Item->m_data.id << endl;
        AddItem(Item->m_data.id, 1);
    }
}


void Inventory::PackItem_withoutItem()
{
    for (const auto& [Id, Item] : m_itemDatabase.GetMap())
    {
        if(dynamic_cast<Potion*>(Item.get()) !=nullptr)
        AddItem(Item->m_data.id, 1);
    }
}



void Inventory::SetDatabase(unique_ptr<ItemDatabase> database) //?∏Î??êÏÑú ÎßåÎì§Í≥??£Ïñ¥Î≤ÑÎ¶¥??
{
    //LoadItemDatabase πˆ¿¸¿ª ¿œ¥‹ æ≤±‚∑Œ «‘.

    //UI_Managerø°º≠ æÓ∂ª∞‘ Init «“¡ˆ¥¬ ∏∏£∞⁄¥Ÿ∏∏, Scene Index πﬁ∞Ì, Databaseø° ≥÷¥¬ ∞≈¥¬ 
    //GiveItem¿ª ≈Î«ÿº≠ πﬁ¿∏∏È µ«∞°¡ˆ∞Ì ªÛ∞¸ æ¯±‰ «ÿ ¿œ¥‹ πˆ¿¸ 1∑Œ µŒ¿⁄∞Ì 
  //  this->itemDatabase = std::move(database);
}

void Inventory::LoadUIBitmaps()
{


    windowBackground.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("InvenBg").Get();
    tooltipBackground.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("SlotDisabled").Get(); //¿”¿«
    tooltipBackground.srcRect = D2D1::RectF(0, 0, 234, 345); // øπΩ√. -> Tooltip 

    closeButton.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("CloseButton").Get();
    //TitleBar.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("TitleBar").Get();

    controller.bitmaps.emplace("slot_disabled",
        ResourceManager::Get().Get_UIBank().Get_Image("SlotDisabled").Get());
    controller.bitmaps.emplace("slot_hover",
        ResourceManager::Get().Get_UIBank().Get_Image("SlotNormal").Get());
    controller.bitmaps.emplace("slot_selected",
        ResourceManager::Get().Get_UIBank().Get_Image("SlotNormal").Get());
    controller.bitmaps.emplace("slot_normal",
        ResourceManager::Get().Get_UIBank().Get_Image("SlotNormal").Get());

}

void Inventory::LoadItemDatabase(Need_Moment Moment)
{
    ResourceManager::Get().Get_ItemBank().GiveItem(Moment, m_itemDatabase);
}
