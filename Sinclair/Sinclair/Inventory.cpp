#include "InvenTory.h"
#include "ResourceManager.h"
#include "ItemBank.h"
#include "CursorManager.h"
#include "InvenMem.h"
#include "Potion.h"
#include "Wearable.h"
//: UI_Object(MWP) //?ì„±?ë¡œ ?ì—­?€ ?¼ë‹¨ ?¤ì •??(Inven ?ê¸° ?ì—­ ë§ì„)
Inventory::Inventory() :UIWindow(UIWindowType::InventoryWindow, Vec2{ 1000,500 }, Vec2{ 1208,825 })  // Vec2{ 1097,766 }) 
{

    m_bound = { 0,0,1208,825 }; // ì´ˆê¸° ?„ì¹˜  

    windowPosition = { m_bound.x,m_bound.y };

    InitializeRegions();
  //  std::cout << "[Inventory] Regions ì´ˆê¸°???„ë£Œ" << std::endl;
    LoadUIBitmaps(); //ë©¤ë²„ë¡?ê°–ê³  ?ˆëŠ” ?«ê¸°????ë¶€ë¶?(?´ê±´ Inven?´ë‘ ê°™ì´??ê°€?¥ì„±???ˆìŒ) 
   // std::cout << "[Inventory] UI ë¹„íŠ¸ë§?ë¡œë”© ?„ë£Œ" << std::endl;

    InitializeSlots();
    std::cout << "[Inventory] ?¬ë¡¯ ì´ˆê¸°???„ë£Œ" << std::endl; // ???¬ê¸° ???˜ì˜¤ë©?ê·??ˆì—???°ì§„ ê±?

   LoadItemDatabase(Need_Moment::Gen_2);


   // LoadItemDatabase(Need_Moment::Adv);
    //LoadItemDatabase(Need_Moment::Syn);

    //LoadItemDatabase(Need_Moment::Gen_2);

    //LoadItemDatabase(Need_Moment::Gen_3);
    //LoadItemDatabase(Need_Moment::Gen_4);


    PackItem();

    dragState.isDragging = false; // ?œë˜ê·??íƒœ ì´ˆê¸°??

    //0. slot Init
    //1. ë¹„íŠ¸ë§?ë¡œë“œ
    //2. Item ?°ì´???£ê¸°

     //3. Update
     //4. Render

}


void Inventory::InitializeRegions()
{
    regions.resize(3); // 3ê°œì˜ ?ì—­ (4x8, 4x8, 4x8)

    //?€?´í? ë°??ì—­, ?„ì¹˜ë§?ì¡´ì¬?œë‹¤ê³??ê°?˜ì…ˆ 
    titleBarBounds = Rect(m_position.x, m_position.y, m_size.x, 42.0f);  //7+35

    //?«ê¸° ë²„íŠ¼ ?ì—­
    float closeButtonSize = 35.0f;
    closeButtonBounds = Rect(
        m_position.x + m_size.x - (closeButtonSize + 75), //14
        m_position.y + 30,      // 35¿´À½ ¿ø·¡
        closeButtonSize,
        closeButtonSize);

    //Inven ?„ì¹˜ 
    windowBackground.position = m_position;
    windowBackground.size = Vec2(m_size.x, m_size.y);
    windowBackground.srcRect = D2D1::RectF(
        0.0f, 0.0f,
        static_cast<float>(m_size.x),
        static_cast<float>(m_size.y)
    );

    //?«ê¸° ë²„íŠ¼ ?œë” ?„ì¹˜ 
    closeButton.position = Vec2(closeButtonBounds.x, closeButtonBounds.y);
    closeButton.size = Vec2(closeButtonBounds.width, closeButtonBounds.height);
    closeButton.srcRect = D2D1::RectF(0, 0, closeButtonSize, closeButtonSize); // ?«ê¸° ë²„íŠ¼ ?´ë?ì§€ ?¬ê¸°

    TitleBar.srcRect = D2D1::RectF(m_position.x, m_position.y, m_size.x, 42.0f);

    float slotSize = 74.0f;
    float padding_x = 13.0f;
    float padding_y = 12.0f;

    float totalSlotDimension_x = slotSize + padding_x;
    float totalSlotDimension_y = slotSize + padding_y;

    // windowBounds???´ë? ?ì—­??ê¸°ì??¼ë¡œ ë°°ì¹˜
    float currentRegionX = m_position.x + 68.0f; // ?ˆë„??ì¢Œì¸¡ ?¬ë°±
    RegionOffset.push_back({ currentRegionX , m_bound.y });
    // Region 0: ê¸°ë³¸ ?´ê¸ˆ ?ì—­ (4x8)
    regions[0].id = 0;
    regions[0].isEnabled = true;
    regions[0].gridSize = Vec2{ 4, 8 }; //4??8??
    regions[0].bounds = Rect(
        currentRegionX,
        m_bound.y + 64.0f, // ?ˆë„???ë‹¨ ?¬ë°± + ?€?´í?ë°?ê³µê°„
        regions[0].gridSize.x * totalSlotDimension_x,
        regions[0].gridSize.y * totalSlotDimension_y
    );
    currentRegionX += (regions[0].bounds.width) + 24.0f; // ?¤ìŒ ?ì—­ ?œì‘ X
    RegionOffset.push_back({ currentRegionX, m_bound.y });

    // Region 1: ì¤‘ê°„ ? ê¸ˆ ?ì—­ (4x8)
    regions[1].id = 1;
    regions[1].isEnabled = true; // ? ê¸ˆ
    regions[1].gridSize = Vec2{ 4, 8 };
    regions[1].bounds = Rect(
        currentRegionX,
        m_bound.y + 64.0f,
        regions[1].gridSize.x * totalSlotDimension_x,
        regions[1].gridSize.y * totalSlotDimension_y
    );
    currentRegionX += regions[1].bounds.width + 24.0f;
    RegionOffset.push_back({ currentRegionX, m_bound.y });

    // Region 2: ?¤ë¥¸ìª?? ê¸ˆ ?ì—­ (4x8)
    regions[2].id = 2;
    regions[2].isEnabled = true; // ? ê¸ˆ
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

                // ?¬ë¡¯ ?„ì¹˜???´ë‹¹ Region??boundsë¥?ê¸°ì??¼ë¡œ ê³„ì‚°
                float slotX = region.bounds.x + x * totalSlotDimension_x;
                float slotY = region.bounds.y + y * totalSlotDimension_y;

                slot.SetBounds(Rect(slotX, slotY, slotSize, slotSize));
                slot.isEnabled = region.isEnabled; // ?ì—­???œì„±???íƒœ???°ë¼ ?¬ë¡¯ ?œì„±???¤ì •
                slot.UpdateBackgroundBitmap(&controller); // ì´ˆê¸° ë°°ê²½ ?¤ì •
            }
        }
    }

    //UpdateSlotPositions();
}

bool Inventory::AddItem(string itemId, int count)
{
    const Item* itemData = m_itemDatabase.GetItemData(itemId);
    if (!itemData) return false;

    // ë¹??¬ë¡¯ ì°¾ê¸°
    for (auto& [key, slot] : slots)
    {
        if (!slot.isEnabled) continue; // ë¹„í™œ?±í™”???¬ë¡¯?ëŠ” ì¶”ê? ë¶ˆê?

        // ?¤íƒ ê°€?¥í•œ ?„ì´?œì´?¼ë©´ ê¸°ì¡´ ?¬ë¡¯???©ì¹˜ê¸??œë„ (?ˆì‹œ)
        if (itemData->IsStackable() && slot.item.id == itemId && (slot.item.count + count) <= itemData->maxCount) //maxCount??10?¼ë¡œ ?˜ê¸´ ?? 
        {
            slot.item.count += count;
            slot.UpdateItemBitmap(&controller, &m_itemDatabase); // ê°œìˆ˜ ë³€ê²½ì? ë¹„íŠ¸ë§?ë³€ê²½ì´ ?†ì?ë§? ?¹ì‹œ ëª¨ë? ê²½ìš° ?¸ì¶œ
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

void Inventory::Update() //?…ë ¥ì²˜ë¦¬ë¥?ë°›ì? ?¤ìŒ???´ì•¼?˜ëŠ” ê±°ì–?? //ì°¨í”¼ ?´ê±°???˜ë? ?†ì„ ???? ?‡ã…‡ ?…ë ¥ ì²˜ë¦¬ ë°›ìœ¼ë©??Œì•„??ë³€ê²???ë¶€ë¶„ì´?? 
{
    // effectê°€ ?„ìš”?˜ë‹¤ë©??¬ê¸°??Update ?˜ëŠ” ê²?ë§ì•„. 
    if (!m_isActive) return;
    // ?´í™??ì»´í¬?ŒíŠ¸ ?…ë°?´íŠ¸
    for (const auto& [key, slot] : slots)
    {
        if (slot.itemBitmap.item == nullptr) continue;
        slot.itemBitmap.item->Update();
    }

    // ì°??„ì¹˜ê°€ ë³€ê²½ë  ?Œë§ˆ???€?´í?ë°?ë°??«ê¸° ë²„íŠ¼???„ì¹˜???…ë°?´íŠ¸
    titleBarBounds = Rect(m_position.x, m_position.y, m_size.x, 42.0f);
    float closeButtonSize = 35.0f;
    closeButtonBounds = Rect(
        m_position.x + m_size.x - (closeButtonSize + 75),   // ¿ø·¡ 77
        m_position.y + 30,
        closeButtonSize,
        closeButtonSize);

    UpdateSlotPositions();

    // ?ˆë„??ë°°ê²½ ?´ë?ì§€ ?„ì¹˜ ?…ë°?´íŠ¸
    windowBackground.position = m_position;
}

void Inventory::Render()
{
    // ?´ê±° ?ˆí•˜ë©?ê³„ì† ê·¸ë ¤ì§?

    if (!m_isActive) return;

    // 1. ?ˆë„??ë°°ê²½ ?Œë”ë§?
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


    // 2. ?€?´í?ë°?ë°??«ê¸° ë²„íŠ¼ ?Œë”ë§?
   // RenderTitleBar();
    RenderCloseButton();

    // 3. ëª¨ë“  ?¬ë¡¯ ?Œë”ë§?
    for (const auto& [key, slot] : slots)
    {
        RenderSlot(slot);
    }

    //if (dragState.isDragging && dragState.dragBitmap.bitmap)
    //{
    //    D2D1_RECT_F dragDestRect = D2D1::RectF(
    //        dragState.mousePos.x - dragState.dragBitmap.size.x / 2.0f, // ë§ˆìš°??ì¤‘ì•™???¤ë„ë¡?
    //        dragState.mousePos.y - dragState.dragBitmap.size.y / 2.0f,
    //        dragState.mousePos.x + dragState.dragBitmap.size.x / 2.0f,
    //        dragState.mousePos.y + dragState.dragBitmap.size.y / 2.0f
    //    );

    //    D2DRenderer::Get().DrawBitmap(static_cast<ID2D1Bitmap1*>(dragState.dragBitmap.bitmap),
    //        dragDestRect, dragState.dragBitmap.srcRect, 1.0f);
    //}

    // 5. ?´íŒ ?Œë”ë§?(ê°€??ë§ˆì?ë§‰ì— ?Œë”ë§í•˜???¤ë¥¸ UI ?„ì— ?œì‹œ)
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

    // ?ˆë„???œë˜ê·?ì¤‘ì¼ ??
    if (isWindowDragging) // 
    {
        float deltaX = mousePos.x - dragStartMousePos.x;
        float deltaY = mousePos.y - dragStartMousePos.y;

        windowPosition = Vec2(dragStartWindowPos.x + deltaX, dragStartWindowPos.y + deltaY);

        m_position = Vec2(dragStartWindowPos.x + deltaX, dragStartWindowPos.y + deltaY); //ë¶€ëª¨í•œ???˜ê²¨ì¤?ê°? -> ?¬ì‹¤ 

        // ?ˆë„??ë°”ìš´?? ?€?´í?ë°? ?«ê¸° ë²„íŠ¼ ë°”ìš´???…ë°?´íŠ¸
        m_bound.x = windowPosition.x;
        m_bound.y = windowPosition.y;
        titleBarBounds.x = windowPosition.x;
        titleBarBounds.y = windowPosition.y;

        float closeButtonSize = 27.0f;

        closeButtonBounds.x = windowPosition.x + m_bound.width - (closeButtonSize + 14);
        closeButtonBounds.y = windowPosition.y + 5;

        // ?¬ë¡¯?¤ì˜ ?„ì¹˜???…ë°?´íŠ¸?´ì•¼ ??
        UpdateSlotPositions();

        return true;
    }
    else // ?ˆë„???œë˜ê·?ì¤‘ì´ ?„ë‹ ?Œë§Œ ?¬ë¡¯ ?¸ë²„ ë°??´íŒ ì²˜ë¦¬
    {
        //showTooltip = false;
        InventorySlot* hoveredSlot = nullptr;

        // ?¬ë¡¯ ?¸ë²„ ?íƒœ ?…ë°?´íŠ¸
        for (auto& [key, slot] : slots)
        {
            bool wasHovered = slot.isHovered;
            // ?¬ë¡¯???„ì¬ ?Œë”ë§??„ì¹˜(?´ë™??ì°½ì— ?°ë¼ ?¬ë¼ì§?ë¥?ê¸°ì??¼ë¡œ Contains ì²´í¬
            // slot.bounds???´ë? UpdateSlotPositions???˜í•´ ?„ì¬ ì°??„ì¹˜??ë§ê²Œ ?…ë°?´íŠ¸??(?¹ì? ?´ë? ì´ˆê¸°?????¬ë°”ë¥´ê²Œ ?¤ì •??

            slot.isHovered = slot.bounds.Contains(mousePos); //?ˆì— ?ˆìœ¼ë©?hovered ?¸ë°, ?¬ê¸°???¬ì‹¤ ?Œë”ê°€ ?ˆë˜???˜ê±°??

            if (wasHovered != slot.isHovered)
            {
                slot.UpdateBackgroundBitmap(&controller);
            }

            if (slot.isHovered && !slot.IsEmpty())  //-> 
            {
                hoveredSlot = &slot; // ?„ì¬ ë§ˆìš°?¤ê? ?¬ë¼ê°??¬ë¡¯
            }
        }

        // ?´íŒ ?•ë³´ ?…ë°?´íŠ¸
        if (hoveredSlot != nullptr)
        {
             Item* data = m_itemDatabase.GetItemData(hoveredSlot->item.id); // ê·??€?¥ëœ ?„ì´???•ë³´ ê°€?¸ì˜¤??ê±°ì„. ?‡ã…‡ 
             //Item* data = hoveredSlot->item.


            if (data)
            {
                CursorManager::Get().SetHoveredItem(data); 
                Vec2 tooltipPos = mousePos + Vec2(10, 10);

                UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, tooltipPos); //?„ì¹˜ ë³€ê²½ì‹œ?¤ê³ , ?œì„±?”ê¹Œì§€ 
            }
            return true;

        }

        else //?´ì œ??ë­??˜ê°„ê±°ê² ì£?..
        {
            CursorManager::Get().HoveredReleased(); //ì¶”ì  ê¸ˆì? 
            UIManager::Get().CloseWindow(UIWindowType::InventoryTooltip); //?´ì œ
            return true;

        }
        return false;

    }

    // ?„ì´???œë˜ê·?ì¤‘ì¼ ??(ë§ˆìš°???„ì¹˜ ?…ë°?´íŠ¸)
    if (dragState.isDragging)
    {
        dragState.mousePos = mousePos;
    }
}

bool Inventory::HandleMouseDown(Vec2 mousePos) //?´ì°¨??Inven ?„ì¹˜ ?´ì— ?ˆì–´???´ê²Œ ì²˜ë¦¬ê°€ ?˜ëŠ” ê±°ë‹ˆê¹?
{
    if (!m_isActive) return false;

    InventorySlot* slot = GetSlotAt(mousePos);
    if (slot && !slot->IsEmpty() && slot->isEnabled)
    {
        // CursorManagerë¥??µí•´ ?„ì—­?ìœ¼ë¡??„ì´???œë˜ê·??œì‘
        CursorManager::Get().StartItemDrag(slot->item.id, DragSource::Inventory, slot);
        CursorManager::Get().SetDraggedItem(m_itemDatabase.GetItemData(slot->item.id)); // ?¤ì œ Item* ?„ë‹¬

        // ?„ì´???•ë³´ë§?CursorManager???˜ê¸°ê³??¬ë¡¯ ë¹„ìš°ê¸?
        slot->Clear();
        slot->UpdateItemBitmap(&controller, &m_itemDatabase);

        return true; // ?…ë ¥ ì²˜ë¦¬ ?„ë£Œ
    }
    // ?ì—­ ???´ë¦­ ??ìµœìƒ?¨ìœ¼ë¡?
    if (IsInBounds(mousePos))
    {
        UIManager::Get().OpenWindow(m_windowType);
        return true;
    }

    return false;
}

bool Inventory::HandleMouseUp(Vec2 mousePos) //ê·??“ì? ?„ì¹˜???€???ˆì™¸ì²˜ë¦¬ë¥??´ì•¼ ?? ?„ê? ?œì‘ ?ˆëŠ”ì§€ ?Œì•„??????
{
    if (!m_isActive) return false;

    // CursorManager?ì„œ ?œë˜ê·?ì¤‘ì¸ ?„ì´?œì´ ?ˆëŠ”ì§€ ?•ì¸
    InventorySlot* targetSlot = GetSlotAt(mousePos); // ?„ì¬ ë§ˆìš°???„ì¹˜???¬ë¡¯???¤ì‹œ ì°¾ìŒ
    Item* draggedItemData = CursorManager::Get().GetDraggedItem(); // CursorManager?ì„œ ?œë˜ê·?ì¤‘ì¸ ?„ì´??ê°€?¸ì˜¤ê¸?
    DragSource dragSource = CursorManager::Get().GetDragSource();

    // ë§Œì•½ ?¥ë¹„ì°½ì—???¨ê±°ë©??¤íƒ¯ ?…ë°?´íŠ¸
    if (dragSource == DragSource::Equipment)
    {
        if (auto* statWindow = dynamic_cast<StatWindow*>(UIManager::Get().GetWindow(UIWindowType::StatsWindow)))
        {
            statWindow->UpdateTotalStats();
        }
    }

    bool placed = false;

    if (targetSlot && targetSlot->isEnabled && draggedItemData)
    {
        // ?¤íƒ ê°€?¥í•œ ?„ì´?œì´?¼ë©´ ?©ì¹˜ê¸??œë„
        if (draggedItemData->IsStackable() &&
            targetSlot->item.id == draggedItemData->m_data.id &&
            (targetSlot->item.count + 1) <= draggedItemData->maxCount) // count??CursorManager?ì„œ ê´€ë¦¬í•´????
        {
            //std::cout << "?¤íƒ ê°€?? ?„ì´???©ì¹ ê±°ì„." << std::endl; ì¡°ê±´?€ ë§ê²Œ ?¤ì–´??
            targetSlot->item.count += 1; // ?œë˜ê·¸ëœ ?„ì´?œì˜ ?¤ì œ countë¥??”í•´????
            targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
            placed = true;
        }
        else if (targetSlot->IsEmpty())
        {
            // ë¹??¬ë¡¯???œë¡­
            targetSlot->SetItem(draggedItemData->m_data.id, 1); // count??CursorManager?ì„œ ê´€ë¦¬í•´????
            targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
            placed = true;
        }
        else // ?¬ë¡¯??ë¹„ì–´?ˆì? ?Šê³  ?¤íƒ ë¶ˆê??¥í•˜ê±°ë‚˜ ?¤ë¥¸ ?„ì´?œì¸ ê²½ìš° (êµí™˜ ë¡œì§)
        {
            // ?ë³¸ ?¬ë¡¯ ?•ë³´ ê°€?¸ì˜¤ê¸?
            InventorySlot* sourceSlot = CursorManager::Get().GetSourceSlot();

            if (sourceSlot)
            {
                // ?„ì¬ ?€ê²??¬ë¡¯???„ì´?œì„ ?„ì‹œ ?€??
                ItemInstance tempItem = targetSlot->item;

                // ?€ê²??¬ë¡¯???œë˜ê·¸ëœ ?„ì´???“ê¸°
                targetSlot->SetItem(draggedItemData->m_data.id, 1);
                targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);

                // ?ë³¸ ?¬ë¡¯??êµí™˜???„ì´???“ê¸°
                sourceSlot->SetItem(tempItem.id, tempItem.count);
                sourceSlot->UpdateItemBitmap(&controller, &m_itemDatabase);

                placed = true;
            }
            else
            {
                // ë¹??¬ë¡¯??ì¶”ê?
                ItemInstance tempItem = targetSlot->item;
                targetSlot->SetItem(draggedItemData->m_data.id, 1);
                targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
                AddItem(tempItem.id, tempItem.count);
                placed = true;
            }
        }
    }
    if (IsInBounds(mousePos))
    {
        UIManager::Get().OpenWindow(m_windowType);
    }
    if (placed)
    {
        CursorManager::Get().EndItemDrag(); // ?œë¡­ ?±ê³µ ???œë˜ê·?ì¢…ë£Œ
        return true;
    }
    else
    {
        return HandleDropFailure(mousePos, draggedItemData, dragSource);
    }

    // ?ì—­ ???´ë¦­ ??ìµœìƒ?¨ìœ¼ë¡?


    return placed;
}

bool Inventory::HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source)
{
    if (!draggedItem) return false;

    // 1. ?¤ë¥¸ ì°½ë“¤???ì—­?¸ì? ?•ì¸
    bool isInOtherWindow = false;

    // ?¥ë¹„ì°??ì—­ ?•ì¸
    UIWindow* equipWindow = UIManager::Get().GetWindow(UIWindowType::EquipmentWindow);
    if (equipWindow && equipWindow->IsActive() && equipWindow->IsInBounds(mousePos))
    {
        isInOtherWindow = true;
    }

    // ê°•í™”ì°??ì—­ ?•ì¸ (?œì„±?”ë˜???ˆë‹¤ë©?
    UIWindow* enhanceWindow = UIManager::Get().GetWindow(UIWindowType::EnhancementWindow);
    if (enhanceWindow && enhanceWindow->IsActive() && enhanceWindow->IsInBounds(mousePos))
    {
        isInOtherWindow = true;
    }

    // ?©ì„±ì°??ì—­ ?•ì¸ (?œì„±?”ë˜???ˆë‹¤ë©?
    UIWindow* synthesisWindow = UIManager::Get().GetWindow(UIWindowType::SynthesisWindow);
    if (synthesisWindow && synthesisWindow->IsActive() && synthesisWindow->IsInBounds(mousePos))
    {
        isInOtherWindow = true;
    }

    // 2. ?¤ë¥¸ ì°??ì—­?´ë©´ ?´ë‹¹ ì°½ì—??ì²˜ë¦¬?˜ë„ë¡??˜ê? (?œë˜ê·??íƒœ ? ì?)
    if (isInOtherWindow)
    {
        return false; // ?¤ë¥¸ ì°½ì—??ì²˜ë¦¬?˜ë„ë¡??˜ê?
    }

    // 3. ?´ë–¤ ì°??ì—­???„ë‹ˆë©??ë˜ ?¸ë²¤? ë¦¬ë¡?ë³µêµ¬
    if (source == DragSource::Inventory)
    {
        // ?¸ë²¤? ë¦¬?ì„œ ?˜ì˜¨ ?„ì´?œì´ë¯€ë¡?ë¹??¬ë¡¯???¤ì‹œ ?£ê¸° ?œë„
        if (AddItem(draggedItem->m_data.id, 1))
        {
            std::cout << "?„ì´?œì„ ?¸ë²¤? ë¦¬ë¡?ë³µêµ¬?ˆìŠµ?ˆë‹¤: " << draggedItem->m_data.name << std::endl;
            CursorManager::Get().EndItemDrag();
            return true;
        }
        else
        {
            std::cout << "?¸ë²¤? ë¦¬ê°€ ê°€??ì°? ?„ì´??ë³µêµ¬ ?¤íŒ¨." << std::endl;
            // ?¸ë²¤? ë¦¬ê°€ ê°€??ì°?ê²½ìš°?ë„ ?œë˜ê·?ì¢…ë£Œ (?„ì´???Œì‹¤ ë°©ì?ë¥??„í•´ ë¡œê·¸ ì¶œë ¥)
            CursorManager::Get().EndItemDrag();
            return true;
        }
    }
    else if (source == DragSource::Equipment)
    {
        // ?¥ë¹„ì°½ì—???˜ì˜¨ ?„ì´?œì´ë¯€ë¡??¸ë²¤? ë¦¬ë¡??´ë™
        if (AddItem(draggedItem->m_data.id, 1))
        {
            std::cout << "?¥ë¹„ ?„ì´?œì„ ?¸ë²¤? ë¦¬ë¡??´ë™?ˆìŠµ?ˆë‹¤: " << draggedItem->m_data.name << std::endl;
            CursorManager::Get().EndItemDrag();
            return true;
        }
        else
        {
            std::cout << "?¸ë²¤? ë¦¬ê°€ ê°€??ì°? ?¥ë¹„ë¥??ë˜ ?¬ë¡¯?¼ë¡œ ë³µêµ¬?©ë‹ˆ??" << std::endl;
            // ?¸ë²¤? ë¦¬ê°€ ê°€??ì°¨ë©´ ?ë˜ ?¥ë¹„ ?¬ë¡¯?¼ë¡œ ë³µêµ¬?´ì•¼ ??
            // ?´ëŠ” EquipmentWindow?ì„œ ì²˜ë¦¬?´ì•¼ ??ë¡œì§
            return false;
        }
    }

    // ê¸°ë³¸?ìœ¼ë¡??œë˜ê·?ì¢…ë£Œ
    CursorManager::Get().EndItemDrag();
    return true;
}

bool Inventory::ConsumePendingPotion()
{
    if (!m_pendingPotionSlot) return false;

    // ?¬ë¡¯/?„ì´??? íš¨??ì²´í¬
    if (m_pendingPotionSlot->IsEmpty()) { m_pendingPotionSlot = nullptr; return false; } 

    Item* data = m_itemDatabase.GetItemData(m_pendingPotionSlot->item.id);
    if (!data || dynamic_cast<Potion*>(data) == nullptr) { m_pendingPotionSlot = nullptr; return false; }

    // ?¤íƒ 1 ê°ì†Œ ??0?´ë©´ ë¹„ìš°ê¸???ë¹„íŠ¸ë§?ê°±ì‹ 
    m_pendingPotionSlot->item.count -= 1;
    if (m_pendingPotionSlot->item.count <= 0)
        m_pendingPotionSlot->Clear(); //?¬ì¸?°ëŠ” ???†ì•° 

    m_pendingPotionSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
    m_pendingPotionSlot = nullptr;
    return true;
}


bool Inventory::HandleDoubleClick(Vec2 mousePos)
{
    

    return false;
}

bool Inventory::HandleMouseRight(Vec2 mousePos) //?¬ìš©???„ì´?œì˜ ?¬ì¸?°ë? ë°›ì•„?€???†ì• ???ìœ¼ë¡?ì§„í–‰?´ì•¼ ? ë“¯. 
{
    InventorySlot* slot = GetSlotAt(mousePos);

    if (slot && !slot->IsEmpty())
    {
        Potion* item = dynamic_cast<Potion*>(m_itemDatabase.GetItemData(slot->item.id));

        if (item != nullptr)
        {
            int much = item->GetMuch() - 1;
            m_pendingPotionSlot = slot; //  ±â¾ï

            UIManager::Get().ShowPotionWindow(much); //Æ÷Áö¼Çµµ ¸ÂÃç ¹ö·È´Ù°í 

        }

        Wearable* wear = dynamic_cast<Wearable*>(m_itemDatabase.GetItemData(slot->item.id));
        if (wear != nullptr)
        {

            UIManager::Get().OpenWindow(UIWindowType::EquipmentWindow); //È°¼ºÈ­ ½Ãµµ 

            auto EQUIPWIN = dynamic_cast<EquipmentWindow*>(
                UIManager::Get().GetWindow(UIWindowType::EquipmentWindow));

            if (EQUIPWIN != nullptr)
            {
                EQUIPWIN->EquipItem(wear);
                slot->Clear();
                slot->UpdateItemBitmap(&controller, &m_itemDatabase);
            }

            //ÀåºñÀÎ °æ¿ì¿¡´Â ÀåÂøÇÏ·Á°í ÇÔ. 

        }
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
    if (!m_isActive || isWindowDragging) return nullptr; // ?ˆë„??ë¹„í™œ???ëŠ” ?œë˜ê·?ì¤‘ì—???¬ë¡¯ ?´ë¦­ ë¶ˆê?

    for (auto& [key, slot] : slots)
    {
        // ?¬ë¡¯??bounds???´ë? UpdateSlotPositions???˜í•´ ?„ì¬ ì°??„ì¹˜??ë§ê²Œ ?…ë°?´íŠ¸?˜ì–´ ?ˆìŒ
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
        // 1. ?€?´í?ë°??ì—­ ?‰ìƒ ì¶œë ¥ (ê³ ì • ??ë¸ŒëŸ¬???¬ìš©)
        ComPtr<ID2D1SolidColorBrush> darkBrush;
        context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkSlateGray, 0.7f), &darkBrush);

        D2D1_RECT_F rect = titleBarBounds.ToD2DRect();
        context->FillRectangle(rect, darkBrush.Get());


        D2DRenderer::Get().DrawBitmap(static_cast<ID2D1Bitmap1*>(TitleBar.bitmap), rect, TitleBar.srcRect, 1.0f);

    }
    // 2. ?ìŠ¤??ì¶œë ¥
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
        // ´İ±â ¹öÆ° À§Ä¡¸¦ ÇöÀç Ã¢ À§Ä¡¿¡ µû¶ó ¾÷µ¥ÀÌÆ®
        float rightMargin = 75; // ¿ø·¡ 65
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
    // ?¬ë¡¯ ë°°ê²½ ?Œë”ë§?
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

    // ë¹„í™œ?±í™”???¬ë¡¯?€ ë¹ˆslot?´ë?ì§€ ê·¸ë¦¼.
    if (!slot.isEnabled)
    {
        //std::cout << "ë¹„í™œ?±í™”???¬ë¡¯?´ë?ë¡?ë°°ê²½ë§??Œë”ë§í•˜ê³?ì¢…ë£Œ" << std::endl;
        return;
    }

    // 2. ?„ì´???„ì´ì½??Œë”ë§?
    if (!slot.IsEmpty() && slot.itemBitmap.item != nullptr)
    {
        auto info = slot.itemBitmap.item->GetRenderInfo();
        slot.itemBitmap.item->SetPosition({ slot.itemBitmap.position.x - 128.f * 0.8f + 35.f, slot.itemBitmap.position.y - 128.f * 0.8f + 35.f });
        D2DRenderer::Get().DrawBitmap(info->GetRenderInfo());

        // 3. ?„ì´??ê°œìˆ˜ ?ìŠ¤??
        if (slot.item.count > 1)
        {
            std::wstring countText = std::to_wstring(slot.item.count);
            // ?ìŠ¤???„ì¹˜ë¥??¬ë¡¯ ?°ì¸¡ ?˜ë‹¨??ë°°ì¹˜ (?ˆì‹œ)
            D2D1_RECT_F textRect = D2D1::RectF(
                slot.bounds.x + slot.bounds.width - 30, // ?°ì¸¡ ?•ë ¬
                slot.bounds.y + slot.bounds.height - 20, // ?˜ë‹¨ ?•ë ¬
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
            slot.itemBitmap.position.x, // itemBitmap.position?€ SetBounds?ì„œ ?´ë? ?¬ë¡¯ bounds??ë§ì¶° ?¤ì •??
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

          // 3. ?„ì´??ê°œìˆ˜ ?ìŠ¤??
        if (slot.item.count > 1)
        {
            std::wstring countText = std::to_wstring(slot.item.count);
            // ?ìŠ¤???„ì¹˜ë¥??¬ë¡¯ ?°ì¸¡ ?˜ë‹¨??ë°°ì¹˜ (?ˆì‹œ)
            D2D1_RECT_F textRect = D2D1::RectF(
                slot.bounds.x + slot.bounds.width - 30, // ?°ì¸¡ ?•ë ¬
                slot.bounds.y + slot.bounds.height - 20, // ?˜ë‹¨ ?•ë ¬
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

void Inventory::UpdateSlotPositions() // -> widndow ê¸°ì??¼ë¡œ ?˜ê³  ?ˆì? ?Šì•„??
{
    float slotSize = 74.0f;
    float padding_x = 13.0f;
    float padding_y = 12.0f;

    /* float totalSlotDimension_x = windowPosition.x + slotSize + padding_x;
     float totalSlotDimension_y = windowPosition.y+ slotSize + padding_y;*/

    float totalSlotDimension_x = slotSize + padding_x;
    float totalSlotDimension_y = slotSize + padding_y;

    // Regions??boundsë¥?ì°½ì˜ ?„ì¬ ?„ì¹˜ë¥?ê¸°ì??¼ë¡œ ?…ë°?´íŠ¸
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

                    // ?¬ë¡¯ ?„ì¹˜???´ë‹¹ Region??boundsë¥?ê¸°ì??¼ë¡œ ê³„ì‚° (regions.bounds??InitializeRegions?ì„œ windowPosition???°ë¼ ?´ë? ê²°ì •??
                    //float slotX = region.bounds.x + x * totalSlotDimension_x;
                    //float slotY = region.bounds.y + y * totalSlotDimension_y;

                    //float slotX = m_bound.x + RegionOffset[regionId].x + x * totalSlotDimension_x;
                    //float slotY = m_bound.y + RegionOffset[regionId].y + y * totalSlotDimension_y+ 64.0f;

                    // region.bounds.x?€ region.bounds.yë¡??¬ë¡¯ ?„ì¹˜ ê³„ì‚°?´ì•¼??
                    float slotX = region.bounds.x + x * totalSlotDimension_x;
                    float slotY = region.bounds.y + y * totalSlotDimension_y;

                    slot.SetBounds(Rect(slotX, slotY, slotSize, slotSize));
                }
            }
        }
    }
}

// ?¬ë¡¯?€ ë¹„ì›Œì§€?”ë° ?¥ë¹„ì°½ì— ?ˆëŠ”ê²??´ë–»ê²?ê¼¬ì¼ì§€ ëª¨ë¥¸?? 
// ?¼ë‹¨?€ ë§Œë“¤ê¸°ë§Œ?ˆì? ?„ë¬´ê²ƒë„ ì¶”ê? ?ˆí–ˆ?¼ë‹ˆê¹?
void Inventory::ClearAllSlots()
{
    // ëª¨ë“  ?¬ë¡¯ ?œíšŒ?˜ë©´??ì´ˆê¸°??
    for (auto& [key, slot] : slots)
    {
        // ?¬ë¡¯ ?´ìš© ?´ë¦¬??
        slot.Clear();

        // ?„ì´??ë¹„íŠ¸ë§??…ë°?´íŠ¸ (ë¹??¬ë¡¯?¼ë¡œ)
        slot.UpdateItemBitmap(&controller, &m_itemDatabase);

        // ë°°ê²½ ë¹„íŠ¸ë§??…ë°?´íŠ¸ (ê¸°ë³¸ ?íƒœë¡?
        slot.UpdateBackgroundBitmap(&controller);
    }
    m_itemDatabase.ClearAllItems();

    std::cout << "[Inventory] ëª¨ë“  ?¬ë¡¯??ì´ˆê¸°?”ë˜?ˆìŠµ?ˆë‹¤." << std::endl;
}

ItemDatabase& Inventory::GetItemBase()
{
    return m_itemDatabase;
}

void Inventory::PackItem() //?„ì¬ database???ˆëŠ” ëª¨ë“  Item??Slot???£ì–´ì¤?
{
    for (const auto& [Id, Item] : m_itemDatabase.GetMap())
    {
        //std::cout << Item->m_data.id << endl;
        AddItem(Item->m_data.id, 1);
    }
}

void Inventory::SetDatabase(unique_ptr<ItemDatabase> database) //?¸ë??ì„œ ë§Œë“¤ê³??£ì–´ë²„ë¦´??
{
    //LoadItemDatabase ë²„ì „???¼ë‹¨ ?°ê¸°ë¡???

    //UI_Manager?ì„œ ?´ë–»ê²?Init ? ì???ëª¨ë¥´ê² ë‹¤ë§? Scene Index ë°›ê³ , Database???£ëŠ” ê±°ëŠ” 
    //GiveItem???µí•´??ë°›ìœ¼ë©??˜ê?ì§€ê³??ê? ?†ê¸´ ???¼ë‹¨ ë²„ì „ 1ë¡??ìê³?
  //  this->itemDatabase = std::move(database);
}

void Inventory::LoadUIBitmaps()
{


    windowBackground.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("InvenBg").Get();
    tooltipBackground.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("SlotDisabled").Get(); //?„ì˜
    tooltipBackground.srcRect = D2D1::RectF(0, 0, 234, 345); // ?ˆì‹œ. -> Tooltip 

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


//