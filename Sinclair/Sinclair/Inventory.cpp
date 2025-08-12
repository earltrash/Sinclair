#include "InvenTory.h"
#include "ResourceManager.h"
#include "ItemBank.h"
#include "CursorManager.h"
#include "InvenMem.h"
#include "Potion.h"
#include "Wearable.h"
//: UI_Object(MWP) //?�성?�로 ?�역?� ?�단 ?�정??(Inven ?�기 ?�역 말임)
Inventory::Inventory() :UIWindow(UIWindowType::InventoryWindow, Vec2{ 1000,500 }, Vec2{ 1208,825 })  // Vec2{ 1097,766 }) 
{

    m_bound = { 0,0,1208,825 }; // 초기 ?�치  

    windowPosition = { m_bound.x,m_bound.y };

    InitializeRegions();
  //  std::cout << "[Inventory] Regions 초기???�료" << std::endl;
    LoadUIBitmaps(); //멤버�?갖고 ?�는 ?�기????부�?(?�건 Inven?�랑 같이??가?�성???�음) 
   // std::cout << "[Inventory] UI 비트�?로딩 ?�료" << std::endl;

    InitializeSlots();
    std::cout << "[Inventory] ?�롯 초기???�료" << std::endl; // ???�기 ???�오�?�??�에???�진 �?

   LoadItemDatabase(Need_Moment::Gen_2);


   // LoadItemDatabase(Need_Moment::Adv);
    //LoadItemDatabase(Need_Moment::Syn);

    //LoadItemDatabase(Need_Moment::Gen_2);

    //LoadItemDatabase(Need_Moment::Gen_3);
    //LoadItemDatabase(Need_Moment::Gen_4);


    PackItem();

    dragState.isDragging = false; // ?�래�??�태 초기??

    //0. slot Init
    //1. 비트�?로드
    //2. Item ?�이???�기

     //3. Update
     //4. Render

}


void Inventory::InitializeRegions()
{
    regions.resize(3); // 3개의 ?�역 (4x8, 4x8, 4x8)

    //?�?��? �??�역, ?�치�?존재?�다�??�각?�셈 
    titleBarBounds = Rect(m_position.x, m_position.y, m_size.x, 42.0f);  //7+35

    //?�기 버튼 ?�역
    float closeButtonSize = 35.0f;
    closeButtonBounds = Rect(
        m_position.x + m_size.x - (closeButtonSize + 75), //14
        m_position.y + 30,      // 35���� ����
        closeButtonSize,
        closeButtonSize);

    //Inven ?�치 
    windowBackground.position = m_position;
    windowBackground.size = Vec2(m_size.x, m_size.y);
    windowBackground.srcRect = D2D1::RectF(
        0.0f, 0.0f,
        static_cast<float>(m_size.x),
        static_cast<float>(m_size.y)
    );

    //?�기 버튼 ?�더 ?�치 
    closeButton.position = Vec2(closeButtonBounds.x, closeButtonBounds.y);
    closeButton.size = Vec2(closeButtonBounds.width, closeButtonBounds.height);
    closeButton.srcRect = D2D1::RectF(0, 0, closeButtonSize, closeButtonSize); // ?�기 버튼 ?��?지 ?�기

    TitleBar.srcRect = D2D1::RectF(m_position.x, m_position.y, m_size.x, 42.0f);

    float slotSize = 74.0f;
    float padding_x = 13.0f;
    float padding_y = 12.0f;

    float totalSlotDimension_x = slotSize + padding_x;
    float totalSlotDimension_y = slotSize + padding_y;

    // windowBounds???��? ?�역??기�??�로 배치
    float currentRegionX = m_position.x + 68.0f; // ?�도??좌측 ?�백
    RegionOffset.push_back({ currentRegionX , m_bound.y });
    // Region 0: 기본 ?�금 ?�역 (4x8)
    regions[0].id = 0;
    regions[0].isEnabled = true;
    regions[0].gridSize = Vec2{ 4, 8 }; //4??8??
    regions[0].bounds = Rect(
        currentRegionX,
        m_bound.y + 64.0f, // ?�도???�단 ?�백 + ?�?��?�?공간
        regions[0].gridSize.x * totalSlotDimension_x,
        regions[0].gridSize.y * totalSlotDimension_y
    );
    currentRegionX += (regions[0].bounds.width) + 24.0f; // ?�음 ?�역 ?�작 X
    RegionOffset.push_back({ currentRegionX, m_bound.y });

    // Region 1: 중간 ?�금 ?�역 (4x8)
    regions[1].id = 1;
    regions[1].isEnabled = true; // ?�금
    regions[1].gridSize = Vec2{ 4, 8 };
    regions[1].bounds = Rect(
        currentRegionX,
        m_bound.y + 64.0f,
        regions[1].gridSize.x * totalSlotDimension_x,
        regions[1].gridSize.y * totalSlotDimension_y
    );
    currentRegionX += regions[1].bounds.width + 24.0f;
    RegionOffset.push_back({ currentRegionX, m_bound.y });

    // Region 2: ?�른�??�금 ?�역 (4x8)
    regions[2].id = 2;
    regions[2].isEnabled = true; // ?�금
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

                // ?�롯 ?�치???�당 Region??bounds�?기�??�로 계산
                float slotX = region.bounds.x + x * totalSlotDimension_x;
                float slotY = region.bounds.y + y * totalSlotDimension_y;

                slot.SetBounds(Rect(slotX, slotY, slotSize, slotSize));
                slot.isEnabled = region.isEnabled; // ?�역???�성???�태???�라 ?�롯 ?�성???�정
                slot.UpdateBackgroundBitmap(&controller); // 초기 배경 ?�정
            }
        }
    }

    //UpdateSlotPositions();
}

bool Inventory::AddItem(string itemId, int count)
{
    const Item* itemData = m_itemDatabase.GetItemData(itemId);
    if (!itemData) return false;

    // �??�롯 찾기
    for (auto& [key, slot] : slots)
    {
        if (!slot.isEnabled) continue; // 비활?�화???�롯?�는 추�? 불�?

        // ?�택 가?�한 ?�이?�이?�면 기존 ?�롯???�치�??�도 (?�시)
        if (itemData->IsStackable() && slot.item.id == itemId && (slot.item.count + count) <= itemData->maxCount) //maxCount??10?�로 ?�긴 ?? 
        {
            slot.item.count += count;
            slot.UpdateItemBitmap(&controller, &m_itemDatabase); // 개수 변경�? 비트�?변경이 ?��?�? ?�시 모�? 경우 ?�출
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

void Inventory::Update() //?�력처리�?받�? ?�음???�야?�는 거잖?? //차피 ?�거???��? ?�을 ???? ?�ㅇ ?�력 처리 받으�??�아??변�???부분이?? 
{
    // effect가 ?�요?�다�??�기??Update ?�는 �?맞아. 
    if (!m_isActive) return;
    // ?�펙??컴포?�트 ?�데?�트
    for (const auto& [key, slot] : slots)
    {
        if (slot.itemBitmap.item == nullptr) continue;
        slot.itemBitmap.item->Update();
    }

    // �??�치가 변경될 ?�마???�?��?�?�??�기 버튼???�치???�데?�트
    titleBarBounds = Rect(m_position.x, m_position.y, m_size.x, 42.0f);
    float closeButtonSize = 35.0f;
    closeButtonBounds = Rect(
        m_position.x + m_size.x - (closeButtonSize + 75),   // ���� 77
        m_position.y + 30,
        closeButtonSize,
        closeButtonSize);

    UpdateSlotPositions();

    // ?�도??배경 ?��?지 ?�치 ?�데?�트
    windowBackground.position = m_position;
}

void Inventory::Render()
{
    // ?�거 ?�하�?계속 그려�?

    if (!m_isActive) return;

    // 1. ?�도??배경 ?�더�?
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


    // 2. ?�?��?�?�??�기 버튼 ?�더�?
   // RenderTitleBar();
    RenderCloseButton();

    // 3. 모든 ?�롯 ?�더�?
    for (const auto& [key, slot] : slots)
    {
        RenderSlot(slot);
    }

    //if (dragState.isDragging && dragState.dragBitmap.bitmap)
    //{
    //    D2D1_RECT_F dragDestRect = D2D1::RectF(
    //        dragState.mousePos.x - dragState.dragBitmap.size.x / 2.0f, // 마우??중앙???�도�?
    //        dragState.mousePos.y - dragState.dragBitmap.size.y / 2.0f,
    //        dragState.mousePos.x + dragState.dragBitmap.size.x / 2.0f,
    //        dragState.mousePos.y + dragState.dragBitmap.size.y / 2.0f
    //    );

    //    D2DRenderer::Get().DrawBitmap(static_cast<ID2D1Bitmap1*>(dragState.dragBitmap.bitmap),
    //        dragDestRect, dragState.dragBitmap.srcRect, 1.0f);
    //}

    // 5. ?�팁 ?�더�?(가??마�?막에 ?�더링하???�른 UI ?�에 ?�시)
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

    // ?�도???�래�?중일 ??
    if (isWindowDragging) // 
    {
        float deltaX = mousePos.x - dragStartMousePos.x;
        float deltaY = mousePos.y - dragStartMousePos.y;

        windowPosition = Vec2(dragStartWindowPos.x + deltaX, dragStartWindowPos.y + deltaY);

        m_position = Vec2(dragStartWindowPos.x + deltaX, dragStartWindowPos.y + deltaY); //부모한???�겨�?�? -> ?�실 

        // ?�도??바운?? ?�?��?�? ?�기 버튼 바운???�데?�트
        m_bound.x = windowPosition.x;
        m_bound.y = windowPosition.y;
        titleBarBounds.x = windowPosition.x;
        titleBarBounds.y = windowPosition.y;

        float closeButtonSize = 27.0f;

        closeButtonBounds.x = windowPosition.x + m_bound.width - (closeButtonSize + 14);
        closeButtonBounds.y = windowPosition.y + 5;

        // ?�롯?�의 ?�치???�데?�트?�야 ??
        UpdateSlotPositions();

        return true;
    }
    else // ?�도???�래�?중이 ?�닐 ?�만 ?�롯 ?�버 �??�팁 처리
    {
        //showTooltip = false;
        InventorySlot* hoveredSlot = nullptr;

        // ?�롯 ?�버 ?�태 ?�데?�트
        for (auto& [key, slot] : slots)
        {
            bool wasHovered = slot.isHovered;
            // ?�롯???�재 ?�더�??�치(?�동??창에 ?�라 ?�라�?�?기�??�로 Contains 체크
            // slot.bounds???��? UpdateSlotPositions???�해 ?�재 �??�치??맞게 ?�데?�트??(?��? ?��? 초기?????�바르게 ?�정??

            slot.isHovered = slot.bounds.Contains(mousePos); //?�에 ?�으�?hovered ?�데, ?�기???�실 ?�더가 ?�되???�거??

            if (wasHovered != slot.isHovered)
            {
                slot.UpdateBackgroundBitmap(&controller);
            }

            if (slot.isHovered && !slot.IsEmpty())  //-> 
            {
                hoveredSlot = &slot; // ?�재 마우?��? ?�라�??�롯
            }
        }

        // ?�팁 ?�보 ?�데?�트
        if (hoveredSlot != nullptr)
        {
             Item* data = m_itemDatabase.GetItemData(hoveredSlot->item.id); // �??�?�된 ?�이???�보 가?�오??거임. ?�ㅇ 
             //Item* data = hoveredSlot->item.


            if (data)
            {
                CursorManager::Get().SetHoveredItem(data); 
                Vec2 tooltipPos = mousePos + Vec2(10, 10);

                UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, tooltipPos); //?�치 변경시?�고, ?�성?�까지 
            }
            return true;

        }

        else //?�제??�??�간거겠�?..
        {
            CursorManager::Get().HoveredReleased(); //추적 금�? 
            UIManager::Get().CloseWindow(UIWindowType::InventoryTooltip); //?�제
            return true;

        }
        return false;

    }

    // ?�이???�래�?중일 ??(마우???�치 ?�데?�트)
    if (dragState.isDragging)
    {
        dragState.mousePos = mousePos;
    }
}

bool Inventory::HandleMouseDown(Vec2 mousePos) //?�차??Inven ?�치 ?�에 ?�어???�게 처리가 ?�는 거니�?
{
    if (!m_isActive) return false;

    InventorySlot* slot = GetSlotAt(mousePos);
    if (slot && !slot->IsEmpty() && slot->isEnabled)
    {
        // CursorManager�??�해 ?�역?�으�??�이???�래�??�작
        CursorManager::Get().StartItemDrag(slot->item.id, DragSource::Inventory, slot);
        CursorManager::Get().SetDraggedItem(m_itemDatabase.GetItemData(slot->item.id)); // ?�제 Item* ?�달

        // ?�이???�보�?CursorManager???�기�??�롯 비우�?
        slot->Clear();
        slot->UpdateItemBitmap(&controller, &m_itemDatabase);

        return true; // ?�력 처리 ?�료
    }
    // ?�역 ???�릭 ??최상?�으�?
    if (IsInBounds(mousePos))
    {
        UIManager::Get().OpenWindow(m_windowType);
        return true;
    }

    return false;
}

bool Inventory::HandleMouseUp(Vec2 mousePos) //�??��? ?�치???�???�외처리�??�야 ?? ?��? ?�작 ?�는지 ?�아??????
{
    if (!m_isActive) return false;

    // CursorManager?�서 ?�래�?중인 ?�이?�이 ?�는지 ?�인
    InventorySlot* targetSlot = GetSlotAt(mousePos); // ?�재 마우???�치???�롯???�시 찾음
    Item* draggedItemData = CursorManager::Get().GetDraggedItem(); // CursorManager?�서 ?�래�?중인 ?�이??가?�오�?
    DragSource dragSource = CursorManager::Get().GetDragSource();

    // 만약 ?�비창에???�거�??�탯 ?�데?�트
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
        // ?�택 가?�한 ?�이?�이?�면 ?�치�??�도
        if (draggedItemData->IsStackable() &&
            targetSlot->item.id == draggedItemData->m_data.id &&
            (targetSlot->item.count + 1) <= draggedItemData->maxCount) // count??CursorManager?�서 관리해????
        {
            //std::cout << "?�택 가?? ?�이???�칠거임." << std::endl; 조건?� 맞게 ?�어??
            targetSlot->item.count += 1; // ?�래그된 ?�이?�의 ?�제 count�??�해????
            targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
            placed = true;
        }
        else if (targetSlot->IsEmpty())
        {
            // �??�롯???�롭
            targetSlot->SetItem(draggedItemData->m_data.id, 1); // count??CursorManager?�서 관리해????
            targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
            placed = true;
        }
        else // ?�롯??비어?��? ?�고 ?�택 불�??�하거나 ?�른 ?�이?�인 경우 (교환 로직)
        {
            // ?�본 ?�롯 ?�보 가?�오�?
            InventorySlot* sourceSlot = CursorManager::Get().GetSourceSlot();

            if (sourceSlot)
            {
                // ?�재 ?��??�롯???�이?�을 ?�시 ?�??
                ItemInstance tempItem = targetSlot->item;

                // ?��??�롯???�래그된 ?�이???�기
                targetSlot->SetItem(draggedItemData->m_data.id, 1);
                targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);

                // ?�본 ?�롯??교환???�이???�기
                sourceSlot->SetItem(tempItem.id, tempItem.count);
                sourceSlot->UpdateItemBitmap(&controller, &m_itemDatabase);

                placed = true;
            }
            else
            {
                // �??�롯??추�?
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
        CursorManager::Get().EndItemDrag(); // ?�롭 ?�공 ???�래�?종료
        return true;
    }
    else
    {
        return HandleDropFailure(mousePos, draggedItemData, dragSource);
    }

    // ?�역 ???�릭 ??최상?�으�?


    return placed;
}

bool Inventory::HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source)
{
    if (!draggedItem) return false;

    // 1. ?�른 창들???�역?��? ?�인
    bool isInOtherWindow = false;

    // ?�비�??�역 ?�인
    UIWindow* equipWindow = UIManager::Get().GetWindow(UIWindowType::EquipmentWindow);
    if (equipWindow && equipWindow->IsActive() && equipWindow->IsInBounds(mousePos))
    {
        isInOtherWindow = true;
    }

    // 강화�??�역 ?�인 (?�성?�되???�다�?
    UIWindow* enhanceWindow = UIManager::Get().GetWindow(UIWindowType::EnhancementWindow);
    if (enhanceWindow && enhanceWindow->IsActive() && enhanceWindow->IsInBounds(mousePos))
    {
        isInOtherWindow = true;
    }

    // ?�성�??�역 ?�인 (?�성?�되???�다�?
    UIWindow* synthesisWindow = UIManager::Get().GetWindow(UIWindowType::SynthesisWindow);
    if (synthesisWindow && synthesisWindow->IsActive() && synthesisWindow->IsInBounds(mousePos))
    {
        isInOtherWindow = true;
    }

    // 2. ?�른 �??�역?�면 ?�당 창에??처리?�도�??��? (?�래�??�태 ?��?)
    if (isInOtherWindow)
    {
        return false; // ?�른 창에??처리?�도�??��?
    }

    // 3. ?�떤 �??�역???�니�??�래 ?�벤?�리�?복구
    if (source == DragSource::Inventory)
    {
        // ?�벤?�리?�서 ?�온 ?�이?�이므�?�??�롯???�시 ?�기 ?�도
        if (AddItem(draggedItem->m_data.id, 1))
        {
            std::cout << "?�이?�을 ?�벤?�리�?복구?�습?�다: " << draggedItem->m_data.name << std::endl;
            CursorManager::Get().EndItemDrag();
            return true;
        }
        else
        {
            std::cout << "?�벤?�리가 가??�? ?�이??복구 ?�패." << std::endl;
            // ?�벤?�리가 가??�?경우?�도 ?�래�?종료 (?�이???�실 방�?�??�해 로그 출력)
            CursorManager::Get().EndItemDrag();
            return true;
        }
    }
    else if (source == DragSource::Equipment)
    {
        // ?�비창에???�온 ?�이?�이므�??�벤?�리�??�동
        if (AddItem(draggedItem->m_data.id, 1))
        {
            std::cout << "?�비 ?�이?�을 ?�벤?�리�??�동?�습?�다: " << draggedItem->m_data.name << std::endl;
            CursorManager::Get().EndItemDrag();
            return true;
        }
        else
        {
            std::cout << "?�벤?�리가 가??�? ?�비�??�래 ?�롯?�로 복구?�니??" << std::endl;
            // ?�벤?�리가 가??차면 ?�래 ?�비 ?�롯?�로 복구?�야 ??
            // ?�는 EquipmentWindow?�서 처리?�야 ??로직
            return false;
        }
    }

    // 기본?�으�??�래�?종료
    CursorManager::Get().EndItemDrag();
    return true;
}

bool Inventory::ConsumePendingPotion()
{
    if (!m_pendingPotionSlot) return false;

    // ?�롯/?�이???�효??체크
    if (m_pendingPotionSlot->IsEmpty()) { m_pendingPotionSlot = nullptr; return false; } 

    Item* data = m_itemDatabase.GetItemData(m_pendingPotionSlot->item.id);
    if (!data || dynamic_cast<Potion*>(data) == nullptr) { m_pendingPotionSlot = nullptr; return false; }

    // ?�택 1 감소 ??0?�면 비우�???비트�?갱신
    m_pendingPotionSlot->item.count -= 1;
    if (m_pendingPotionSlot->item.count <= 0)
        m_pendingPotionSlot->Clear(); //?�인?�는 ???�앰 

    m_pendingPotionSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
    m_pendingPotionSlot = nullptr;
    return true;
}


bool Inventory::HandleDoubleClick(Vec2 mousePos)
{
    

    return false;
}

bool Inventory::HandleMouseRight(Vec2 mousePos) //?�용???�이?�의 ?�인?��? 받아?�???�애???�으�?진행?�야 ?�듯. 
{
    InventorySlot* slot = GetSlotAt(mousePos);

    if (slot && !slot->IsEmpty())
    {
        Potion* item = dynamic_cast<Potion*>(m_itemDatabase.GetItemData(slot->item.id));

        if (item != nullptr)
        {
            int much = item->GetMuch() - 1;
            m_pendingPotionSlot = slot; //  ���

            UIManager::Get().ShowPotionWindow(much); //�����ǵ� ���� ���ȴٰ� 

        }

        Wearable* wear = dynamic_cast<Wearable*>(m_itemDatabase.GetItemData(slot->item.id));
        if (wear != nullptr)
        {

            UIManager::Get().OpenWindow(UIWindowType::EquipmentWindow); //Ȱ��ȭ �õ� 

            auto EQUIPWIN = dynamic_cast<EquipmentWindow*>(
                UIManager::Get().GetWindow(UIWindowType::EquipmentWindow));

            if (EQUIPWIN != nullptr)
            {
                EQUIPWIN->EquipItem(wear);
                slot->Clear();
                slot->UpdateItemBitmap(&controller, &m_itemDatabase);
            }

            //����� ��쿡�� �����Ϸ��� ��. 

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
    if (!m_isActive || isWindowDragging) return nullptr; // ?�도??비활???�는 ?�래�?중에???�롯 ?�릭 불�?

    for (auto& [key, slot] : slots)
    {
        // ?�롯??bounds???��? UpdateSlotPositions???�해 ?�재 �??�치??맞게 ?�데?�트?�어 ?�음
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
        // 1. ?�?��?�??�역 ?�상 출력 (고정 ??브러???�용)
        ComPtr<ID2D1SolidColorBrush> darkBrush;
        context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkSlateGray, 0.7f), &darkBrush);

        D2D1_RECT_F rect = titleBarBounds.ToD2DRect();
        context->FillRectangle(rect, darkBrush.Get());


        D2DRenderer::Get().DrawBitmap(static_cast<ID2D1Bitmap1*>(TitleBar.bitmap), rect, TitleBar.srcRect, 1.0f);

    }
    // 2. ?�스??출력
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
        // �ݱ� ��ư ��ġ�� ���� â ��ġ�� ���� ������Ʈ
        float rightMargin = 75; // ���� 65
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
    // ?�롯 배경 ?�더�?
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

    // 비활?�화???�롯?� 빈slot?��?지 그림.
    if (!slot.isEnabled)
    {
        //std::cout << "비활?�화???�롯?��?�?배경�??�더링하�?종료" << std::endl;
        return;
    }

    // 2. ?�이???�이�??�더�?
    if (!slot.IsEmpty() && slot.itemBitmap.item != nullptr)
    {
        auto info = slot.itemBitmap.item->GetRenderInfo();
        slot.itemBitmap.item->SetPosition({ slot.itemBitmap.position.x - 128.f * 0.8f + 35.f, slot.itemBitmap.position.y - 128.f * 0.8f + 35.f });
        D2DRenderer::Get().DrawBitmap(info->GetRenderInfo());

        // 3. ?�이??개수 ?�스??
        if (slot.item.count > 1)
        {
            std::wstring countText = std::to_wstring(slot.item.count);
            // ?�스???�치�??�롯 ?�측 ?�단??배치 (?�시)
            D2D1_RECT_F textRect = D2D1::RectF(
                slot.bounds.x + slot.bounds.width - 30, // ?�측 ?�렬
                slot.bounds.y + slot.bounds.height - 20, // ?�단 ?�렬
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
            slot.itemBitmap.position.x, // itemBitmap.position?� SetBounds?�서 ?��? ?�롯 bounds??맞춰 ?�정??
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

          // 3. ?�이??개수 ?�스??
        if (slot.item.count > 1)
        {
            std::wstring countText = std::to_wstring(slot.item.count);
            // ?�스???�치�??�롯 ?�측 ?�단??배치 (?�시)
            D2D1_RECT_F textRect = D2D1::RectF(
                slot.bounds.x + slot.bounds.width - 30, // ?�측 ?�렬
                slot.bounds.y + slot.bounds.height - 20, // ?�단 ?�렬
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

void Inventory::UpdateSlotPositions() // -> widndow 기�??�로 ?�고 ?��? ?�아??
{
    float slotSize = 74.0f;
    float padding_x = 13.0f;
    float padding_y = 12.0f;

    /* float totalSlotDimension_x = windowPosition.x + slotSize + padding_x;
     float totalSlotDimension_y = windowPosition.y+ slotSize + padding_y;*/

    float totalSlotDimension_x = slotSize + padding_x;
    float totalSlotDimension_y = slotSize + padding_y;

    // Regions??bounds�?창의 ?�재 ?�치�?기�??�로 ?�데?�트
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

                    // ?�롯 ?�치???�당 Region??bounds�?기�??�로 계산 (regions.bounds??InitializeRegions?�서 windowPosition???�라 ?��? 결정??
                    //float slotX = region.bounds.x + x * totalSlotDimension_x;
                    //float slotY = region.bounds.y + y * totalSlotDimension_y;

                    //float slotX = m_bound.x + RegionOffset[regionId].x + x * totalSlotDimension_x;
                    //float slotY = m_bound.y + RegionOffset[regionId].y + y * totalSlotDimension_y+ 64.0f;

                    // region.bounds.x?� region.bounds.y�??�롯 ?�치 계산?�야??
                    float slotX = region.bounds.x + x * totalSlotDimension_x;
                    float slotY = region.bounds.y + y * totalSlotDimension_y;

                    slot.SetBounds(Rect(slotX, slotY, slotSize, slotSize));
                }
            }
        }
    }
}

// ?�롯?� 비워지?�데 ?�비창에 ?�는�??�떻�?꼬일지 모른?? 
// ?�단?� 만들기만?��? ?�무것도 추�? ?�했?�니�?
void Inventory::ClearAllSlots()
{
    // 모든 ?�롯 ?�회?�면??초기??
    for (auto& [key, slot] : slots)
    {
        // ?�롯 ?�용 ?�리??
        slot.Clear();

        // ?�이??비트�??�데?�트 (�??�롯?�로)
        slot.UpdateItemBitmap(&controller, &m_itemDatabase);

        // 배경 비트�??�데?�트 (기본 ?�태�?
        slot.UpdateBackgroundBitmap(&controller);
    }
    m_itemDatabase.ClearAllItems();

    std::cout << "[Inventory] 모든 ?�롯??초기?�되?�습?�다." << std::endl;
}

ItemDatabase& Inventory::GetItemBase()
{
    return m_itemDatabase;
}

void Inventory::PackItem() //?�재 database???�는 모든 Item??Slot???�어�?
{
    for (const auto& [Id, Item] : m_itemDatabase.GetMap())
    {
        //std::cout << Item->m_data.id << endl;
        AddItem(Item->m_data.id, 1);
    }
}

void Inventory::SetDatabase(unique_ptr<ItemDatabase> database) //?��??�서 만들�??�어버릴??
{
    //LoadItemDatabase 버전???�단 ?�기�???

    //UI_Manager?�서 ?�떻�?Init ?��???모르겠다�? Scene Index 받고, Database???�는 거는 
    //GiveItem???�해??받으�??��?지�??��? ?�긴 ???�단 버전 1�??�자�?
  //  this->itemDatabase = std::move(database);
}

void Inventory::LoadUIBitmaps()
{


    windowBackground.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("InvenBg").Get();
    tooltipBackground.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("SlotDisabled").Get(); //?�의
    tooltipBackground.srcRect = D2D1::RectF(0, 0, 234, 345); // ?�시. -> Tooltip 

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