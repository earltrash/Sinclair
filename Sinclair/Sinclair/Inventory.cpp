#include "InvenTory.h"
#include "ResourceManager.h"
#include "ItemBank.h"
#include "CursorManager.h"
#include "InvenMem.h"
#include "Potion.h"

//: UI_Object(MWP) //생성자로 영역은 일단 설정함 (Inven 자기 영역 말임)
Inventory::Inventory() :UIWindow(UIWindowType::InventoryWindow, Vec2{ 1000,500 }, Vec2{ 1208,825 })  // Vec2{ 1097,766 }) 
{

    m_bound = { 0,0,1208,825 }; // 초기 위치  

    windowPosition = { m_bound.x,m_bound.y };

    InitializeRegions();
  //  std::cout << "[Inventory] Regions 초기화 완료" << std::endl;
    LoadUIBitmaps(); //멤버로 갖고 있는 닫기랑 윗 부분 (이건 Inven이랑 같이할 가능성도 있음) 
   // std::cout << "[Inventory] UI 비트맵 로딩 완료" << std::endl;

    InitializeSlots();
    std::cout << "[Inventory] 슬롯 초기화 완료" << std::endl; // ← 여기 안 나오면 그 안에서 터진 거

   LoadItemDatabase(Need_Moment::Syn);

    LoadItemDatabase(Need_Moment::Adv);
    //LoadItemDatabase(Need_Moment::Syn);

    //LoadItemDatabase(Need_Moment::Gen_2);

    //LoadItemDatabase(Need_Moment::Gen_3);
    //LoadItemDatabase(Need_Moment::Gen_4);


    PackItem();

    dragState.isDragging = false; // 드래그 상태 초기화

    //0. slot Init
    //1. 비트맵 로드
    //2. Item 데이터 넣기

     //3. Update
     //4. Render

}


void Inventory::InitializeRegions()
{
    regions.resize(3); // 3개의 영역 (4x8, 4x8, 4x8)

    //타이틀 바 영역, 위치만 존재한다고 생각하셈 
    titleBarBounds = Rect(m_position.x, m_position.y, m_size.x, 42.0f);  //7+35

    //닫기 버튼 영역 //86
    float closeButtonSize = 35.0f;
    closeButtonBounds = Rect(
        m_position.x + m_size.x - (closeButtonSize + 59), //14
        m_position.y + 7,
        closeButtonSize,
        closeButtonSize);

    //Inven 위치 
    windowBackground.position = m_position;
    windowBackground.size = Vec2(m_size.x, m_size.y);
    windowBackground.srcRect = D2D1::RectF(
        0.0f, 0.0f,
        static_cast<float>(m_size.x),
        static_cast<float>(m_size.y)
    );

    //닫기 버튼 랜더 위치 
    closeButton.position = Vec2(closeButtonBounds.x, closeButtonBounds.y);
    closeButton.size = Vec2(closeButtonBounds.width, closeButtonBounds.height);
    closeButton.srcRect = D2D1::RectF(0, 0, 27, 27); // 닫기 버튼 이미지 크기

    TitleBar.srcRect = D2D1::RectF(m_position.x, m_position.y, m_size.x, 42.0f);

    float slotSize = 74.0f;
    float padding_x = 13.0f;
    float padding_y = 12.0f;

    float totalSlotDimension_x = slotSize + padding_x;
    float totalSlotDimension_y = slotSize + padding_y;

    // windowBounds의 내부 영역을 기준으로 배치
    float currentRegionX = m_position.x + 68.0f; // 윈도우 좌측 여백
    RegionOffset.push_back({ currentRegionX , m_bound.y });
    // Region 0: 기본 해금 영역 (4x8)
    regions[0].id = 0;
    regions[0].isEnabled = true;
    regions[0].gridSize = Vec2{ 4, 8 }; //4열 8행
    regions[0].bounds = Rect(
        currentRegionX,
        m_bound.y + 64.0f, // 윈도우 상단 여백 + 타이틀바 공간
        regions[0].gridSize.x * totalSlotDimension_x,
        regions[0].gridSize.y * totalSlotDimension_y
    );
    currentRegionX += (regions[0].bounds.width) + 24.0f; // 다음 영역 시작 X
    RegionOffset.push_back({ currentRegionX, m_bound.y });

    // Region 1: 중간 잠금 영역 (4x8)
    regions[1].id = 1;
    regions[1].isEnabled = true; // 잠금
    regions[1].gridSize = Vec2{ 4, 8 };
    regions[1].bounds = Rect(
        currentRegionX,
        m_bound.y + 64.0f,
        regions[1].gridSize.x * totalSlotDimension_x,
        regions[1].gridSize.y * totalSlotDimension_y
    );
    currentRegionX += regions[1].bounds.width + 24.0f;
    RegionOffset.push_back({ currentRegionX, m_bound.y });

    // Region 2: 오른쪽 잠금 영역 (4x8)
    regions[2].id = 2;
    regions[2].isEnabled = true; // 잠금
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

                // 슬롯 위치는 해당 Region의 bounds를 기준으로 계산
                float slotX = region.bounds.x + x * totalSlotDimension_x;
                float slotY = region.bounds.y + y * totalSlotDimension_y;

                slot.SetBounds(Rect(slotX, slotY, slotSize, slotSize));
                slot.isEnabled = region.isEnabled; // 영역의 활성화 상태에 따라 슬롯 활성화 설정
                slot.UpdateBackgroundBitmap(&controller); // 초기 배경 설정
            }
        }
    }

    //UpdateSlotPositions();
}

bool Inventory::AddItem(string itemId, int count)
{
    const Item* itemData = m_itemDatabase.GetItemData(itemId);
    if (!itemData) return false;

    // 빈 슬롯 찾기
    for (auto& [key, slot] : slots)
    {
        if (!slot.isEnabled) continue; // 비활성화된 슬롯에는 추가 불가

        // 스택 가능한 아이템이라면 기존 슬롯에 합치기 시도 (예시)
        if (itemData->IsStackable() && slot.item.id == itemId && (slot.item.count + count) <= itemData->maxCount) //maxCount는 10으로 하긴 함. 
        {
            slot.item.count += count;
            slot.UpdateItemBitmap(&controller, &m_itemDatabase); // 개수 변경은 비트맵 변경이 없지만, 혹시 모를 경우 호출
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

void Inventory::Update() //입력처리를 받은 다음에 해야하는 거잖아? //차피 이거는 의미 없을 듯 함? ㅇㅇ 입력 처리 받으면 알아서 변경 될 부분이라. 
{
    // effect가 필요하다면 여기서 Update 하는 게 맞아. 
    if (!m_isActive) return;

    // 창 위치가 변경될 때마다 타이틀바 및 닫기 버튼의 위치도 업데이트
    titleBarBounds = Rect(m_position.x, m_position.y, m_size.x, 42.0f);
    float closeButtonSize = 35.0f;
    closeButtonBounds = Rect(
        m_position.x + m_size.x - (closeButtonSize + 59),
        m_position.y + 7,
        closeButtonSize,
        closeButtonSize);

    UpdateSlotPositions();

    // 윈도우 배경 이미지 위치 업데이트
    windowBackground.position = m_position;
}

void Inventory::Render()
{
    // 1. 윈도우 배경 렌더링
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


    // 2. 타이틀바 및 닫기 버튼 렌더링
   // RenderTitleBar();
    RenderCloseButton();

    // 3. 모든 슬롯 렌더링
    for (const auto& [key, slot] : slots)
    {
        RenderSlot(slot);
    }

    //if (dragState.isDragging && dragState.dragBitmap.bitmap)
    //{
    //    D2D1_RECT_F dragDestRect = D2D1::RectF(
    //        dragState.mousePos.x - dragState.dragBitmap.size.x / 2.0f, // 마우스 중앙에 오도록
    //        dragState.mousePos.y - dragState.dragBitmap.size.y / 2.0f,
    //        dragState.mousePos.x + dragState.dragBitmap.size.x / 2.0f,
    //        dragState.mousePos.y + dragState.dragBitmap.size.y / 2.0f
    //    );

    //    D2DRenderer::Get().DrawBitmap(static_cast<ID2D1Bitmap1*>(dragState.dragBitmap.bitmap),
    //        dragDestRect, dragState.dragBitmap.srcRect, 1.0f);
    //}

    // 5. 툴팁 렌더링 (가장 마지막에 렌더링하여 다른 UI 위에 표시)
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

    // 윈도우 드래그 중일 때
    if (isWindowDragging) // 
    {
        float deltaX = mousePos.x - dragStartMousePos.x;
        float deltaY = mousePos.y - dragStartMousePos.y;

        windowPosition = Vec2(dragStartWindowPos.x + deltaX, dragStartWindowPos.y + deltaY);

        m_position = Vec2(dragStartWindowPos.x + deltaX, dragStartWindowPos.y + deltaY); //부모한테 넘겨줄 값. -> 사실 

        // 윈도우 바운드, 타이틀바, 닫기 버튼 바운드 업데이트
        m_bound.x = windowPosition.x;
        m_bound.y = windowPosition.y;
        titleBarBounds.x = windowPosition.x;
        titleBarBounds.y = windowPosition.y;

        float closeButtonSize = 27.0f;

        closeButtonBounds.x = windowPosition.x + m_bound.width - (closeButtonSize + 14);
        closeButtonBounds.y = windowPosition.y + 5;

        // 슬롯들의 위치도 업데이트해야 함
        UpdateSlotPositions();

        return true;
    }
    else // 윈도우 드래그 중이 아닐 때만 슬롯 호버 및 툴팁 처리
    {
        //showTooltip = false;
        InventorySlot* hoveredSlot = nullptr;

        // 슬롯 호버 상태 업데이트
        for (auto& [key, slot] : slots)
        {
            bool wasHovered = slot.isHovered;
            // 슬롯의 현재 렌더링 위치(이동된 창에 따라 달라짐)를 기준으로 Contains 체크
            // slot.bounds는 이미 UpdateSlotPositions에 의해 현재 창 위치에 맞게 업데이트됨 (혹은 이미 초기화 시 올바르게 설정됨)

            slot.isHovered = slot.bounds.Contains(mousePos); //안에 있으면 hovered 인데, 여기서 사실 렌더가 안되야 하거든 

            if (wasHovered != slot.isHovered)
            {
                slot.UpdateBackgroundBitmap(&controller);
            }

            if (slot.isHovered && !slot.IsEmpty())  //-> 
            {
                hoveredSlot = &slot; // 현재 마우스가 올라간 슬롯
            }
        }

        // 툴팁 정보 업데이트
        if (hoveredSlot != nullptr)
        {
             Item* data = m_itemDatabase.GetItemData(hoveredSlot->item.id); // 그 저장된 아이템 정보 가져오는 거임. ㅇㅇ 
             //Item* data = hoveredSlot->item.


            if (data)
            {
                CursorManager::Get().SetHoveredItem(data); 
                Vec2 tooltipPos = mousePos + Vec2(10, 10);

                UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, tooltipPos); //위치 변경시키고, 활성화까지 
            }
            return true;

        }

        else //이제는 뭐 나간거겠죠...
        {
            CursorManager::Get().HoveredReleased(); //추적 금지 
            UIManager::Get().CloseWindow(UIWindowType::InventoryTooltip); //해제
            return true;

        }
        return false;

    }

    // 아이템 드래그 중일 때 (마우스 위치 업데이트)
    if (dragState.isDragging)
    {
        dragState.mousePos = mousePos;
    }
}

bool Inventory::HandleMouseDown(Vec2 mousePos) //어차피 Inven 위치 내에 있어야 이게 처리가 되는 거니깐 
{
    if (!m_isActive) return false;

    InventorySlot* slot = GetSlotAt(mousePos);
    if (slot && !slot->IsEmpty() && slot->isEnabled)
    {
        // CursorManager를 통해 전역적으로 아이템 드래그 시작
        CursorManager::Get().StartItemDrag(slot->item.id, DragSource::Inventory, slot);
        CursorManager::Get().SetDraggedItem(m_itemDatabase.GetItemData(slot->item.id)); // 실제 Item* 전달

        // 아이템 정보만 CursorManager에 넘기고 슬롯 비우기.
        slot->Clear();
        slot->UpdateItemBitmap(&controller, &m_itemDatabase);

        return true; // 입력 처리 완료
    }
    // 영역 안 클릭 시 최상단으로.
    if (IsInBounds(mousePos))
    {
        UIManager::Get().OpenWindow(m_windowType);
        return true;
    }

    return false;
}

bool Inventory::HandleMouseUp(Vec2 mousePos) //그 놓은 위치에 대한 예외처리를 해야 함. 누가 시작 했는지 알아야 할 듯 
{
    if (!m_isActive) return false;

    // CursorManager에서 드래그 중인 아이템이 있는지 확인
    if (!CursorManager::Get().IsDragging()) return false;

    InventorySlot* targetSlot = GetSlotAt(mousePos); // 현재 마우스 위치의 슬롯을 다시 찾음
    Item* draggedItemData = CursorManager::Get().GetDraggedItem(); // CursorManager에서 드래그 중인 아이템 가져오기
    DragSource dragSource = CursorManager::Get().GetDragSource();

    bool placed = false;

    if (targetSlot && targetSlot->isEnabled && draggedItemData)
    {
        // 스택 가능한 아이템이라면 합치기 시도
        if (draggedItemData->IsStackable() &&
            targetSlot->item.id == draggedItemData->m_data.id &&
            (targetSlot->item.count + 1) <= draggedItemData->maxCount) // count는 CursorManager에서 관리해야 함
        {
            //std::cout << "스택 가능. 아이템 합칠거임." << std::endl; 조건은 맞게 들어옴.
            targetSlot->item.count += 1; // 드래그된 아이템의 실제 count를 더해야 함
            targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
            placed = true;
        }
        else if (targetSlot->IsEmpty())
        {
            // 빈 슬롯에 드롭
            targetSlot->SetItem(draggedItemData->m_data.id, 1); // count는 CursorManager에서 관리해야 함
            targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
            placed = true;
        }
        else // 슬롯이 비어있지 않고 스택 불가능하거나 다른 아이템인 경우 (교환 로직)
        {
            // 원본 슬롯 정보 가져오기
            InventorySlot* sourceSlot = CursorManager::Get().GetSourceSlot();

            if (sourceSlot)
            {
                // 현재 타겟 슬롯의 아이템을 임시 저장
                ItemInstance tempItem = targetSlot->item;

                // 타겟 슬롯에 드래그된 아이템 놓기
                targetSlot->SetItem(draggedItemData->m_data.id, 1);
                targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);

                // 원본 슬롯에 교환된 아이템 놓기
                sourceSlot->SetItem(tempItem.id, tempItem.count);
                sourceSlot->UpdateItemBitmap(&controller, &m_itemDatabase);

                placed = true;
            }
            else
            {
                // 빈 슬롯에 추가
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
        CursorManager::Get().EndItemDrag(); // 드롭 성공 시 드래그 종료
        return true;
    }
    else
    {
        return HandleDropFailure(mousePos, draggedItemData, dragSource);
    }

    // 영역 안 클릭 시 최상단으로.


    return placed;
}

bool Inventory::HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source)
{
    if (!draggedItem) return false;

    // 1. 다른 창들의 영역인지 확인
    bool isInOtherWindow = false;

    // 장비창 영역 확인
    UIWindow* equipWindow = UIManager::Get().GetWindow(UIWindowType::EquipmentWindow);
    if (equipWindow && equipWindow->IsActive() && equipWindow->IsInBounds(mousePos))
    {
        isInOtherWindow = true;
    }

    // 강화창 영역 확인 (활성화되어 있다면)
    UIWindow* enhanceWindow = UIManager::Get().GetWindow(UIWindowType::EnhancementWindow);
    if (enhanceWindow && enhanceWindow->IsActive() && enhanceWindow->IsInBounds(mousePos))
    {
        isInOtherWindow = true;
    }

    // 합성창 영역 확인 (활성화되어 있다면)
    UIWindow* synthesisWindow = UIManager::Get().GetWindow(UIWindowType::SynthesisWindow);
    if (synthesisWindow && synthesisWindow->IsActive() && synthesisWindow->IsInBounds(mousePos))
    {
        isInOtherWindow = true;
    }

    // 2. 다른 창 영역이면 해당 창에서 처리하도록 넘김 (드래그 상태 유지)
    if (isInOtherWindow)
    {
        return false; // 다른 창에서 처리하도록 넘김
    }

    // 3. 어떤 창 영역도 아니면 원래 인벤토리로 복구
    if (source == DragSource::Inventory)
    {
        // 인벤토리에서 나온 아이템이므로 빈 슬롯에 다시 넣기 시도
        if (AddItem(draggedItem->m_data.id, 1))
        {
            std::cout << "아이템을 인벤토리로 복구했습니다: " << draggedItem->m_data.name << std::endl;
            CursorManager::Get().EndItemDrag();
            return true;
        }
        else
        {
            std::cout << "인벤토리가 가득 참. 아이템 복구 실패." << std::endl;
            // 인벤토리가 가득 찬 경우에도 드래그 종료 (아이템 소실 방지를 위해 로그 출력)
            CursorManager::Get().EndItemDrag();
            return true;
        }
    }
    else if (source == DragSource::Equipment)
    {
        // 장비창에서 나온 아이템이므로 인벤토리로 이동
        if (AddItem(draggedItem->m_data.id, 1))
        {
            std::cout << "장비 아이템을 인벤토리로 이동했습니다: " << draggedItem->m_data.name << std::endl;
            CursorManager::Get().EndItemDrag();
            return true;
        }
        else
        {
            std::cout << "인벤토리가 가득 참. 장비를 원래 슬롯으로 복구합니다." << std::endl;
            // 인벤토리가 가득 차면 원래 장비 슬롯으로 복구해야 함
            // 이는 EquipmentWindow에서 처리해야 할 로직
            return false;
        }
    }

    // 기본적으로 드래그 종료
    CursorManager::Get().EndItemDrag();
    return true;
}


bool Inventory::HandleDoubleClick(Vec2 mousePos)
{
    

    return false;
}

bool Inventory::HandleMouseRight(Vec2 mousePos)
{
    InventorySlot* slot = GetSlotAt(mousePos);
    if (slot && !slot->IsEmpty())
    {
        Potion* item = dynamic_cast<Potion*>(m_itemDatabase.GetItemData(slot->item.id));
        if (item != nullptr)
        {
            int much  = item->GetMuch() -1 ;

            UIManager::Get().ShowPotionWindow(much); //포지션도 맞춰 버렸다고 

        }



    }


    return false;
}

UIWindowType Inventory::GetType()
{
    return m_windowType;
}

InventorySlot* Inventory::GetSlotAt(const Vec2& pos)
{
    if (!m_isActive || isWindowDragging) return nullptr; // 윈도우 비활성 또는 드래그 중에는 슬롯 클릭 불가

    for (auto& [key, slot] : slots)
    {
        // 슬롯의 bounds는 이미 UpdateSlotPositions에 의해 현재 창 위치에 맞게 업데이트되어 있음
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
        // 1. 타이틀바 영역 색상 출력 (고정 색 브러시 사용)
        ComPtr<ID2D1SolidColorBrush> darkBrush;
        context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkSlateGray, 0.7f), &darkBrush);

        D2D1_RECT_F rect = titleBarBounds.ToD2DRect();
        context->FillRectangle(rect, darkBrush.Get());


        D2DRenderer::Get().DrawBitmap(static_cast<ID2D1Bitmap1*>(TitleBar.bitmap), rect, TitleBar.srcRect, 1.0f);

    }
    // 2. 텍스트 출력
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
        // 닫기 버튼 위치를 현재 창 위치에 따라 업데이트
        float rightMargin = 47.0f;
        Vec2 currentCloseButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 7 };

        D2D1_RECT_F destRect = D2D1::RectF(
            currentCloseButtonPos.x, currentCloseButtonPos.y,
            currentCloseButtonPos.x + 27.0f, currentCloseButtonPos.y + 27.0f
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
    // 슬롯 배경 렌더링
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

    // 비활성화된 슬롯은 빈slot이미지 그림.
    if (!slot.isEnabled)
    {
        //std::cout << "비활성화된 슬롯이므로 배경만 렌더링하고 종료" << std::endl;
        return;
    }

    // 2. 아이템 아이콘 렌더링
    if (!slot.IsEmpty() && slot.itemBitmap.bitmap)
    {
        D2D1_RECT_F itemDestRect = D2D1::RectF(
            slot.itemBitmap.position.x, // itemBitmap.position은 SetBounds에서 이미 슬롯 bounds에 맞춰 설정됨
            slot.itemBitmap.position.y,
            slot.itemBitmap.position.x + slot.itemBitmap.size.x,
            slot.itemBitmap.position.y + slot.itemBitmap.size.y
        );
        D2D1_RECT_F itemSrcRect = slot.itemBitmap.srcRect;

        D2DRenderer::Get().DrawBitmap(
            static_cast<ID2D1Bitmap1*>(slot.itemBitmap.bitmap),
            itemDestRect,
            itemSrcRect,
            slot.itemBitmap.opacity

        );
        //  std::cout << " slot.itemBitmap:Opacity" << slot.itemBitmap.opacity << endl;


          // 3. 아이템 개수 텍스트
        if (slot.item.count > 1)
        {
            std::wstring countText = std::to_wstring(slot.item.count);
            // 텍스트 위치를 슬롯 우측 하단에 배치 (예시)
            D2D1_RECT_F textRect = D2D1::RectF(
                slot.bounds.x + slot.bounds.width - 30, // 우측 정렬
                slot.bounds.y + slot.bounds.height - 20, // 하단 정렬
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

void Inventory::UpdateSlotPositions() // -> widndow 기준으로 되고 있지 않아요 
{
    float slotSize = 74.0f;
    float padding_x = 13.0f;
    float padding_y = 12.0f;

    /* float totalSlotDimension_x = windowPosition.x + slotSize + padding_x;
     float totalSlotDimension_y = windowPosition.y+ slotSize + padding_y;*/

    float totalSlotDimension_x = slotSize + padding_x;
    float totalSlotDimension_y = slotSize + padding_y;

    // Regions의 bounds를 창의 현재 위치를 기준으로 업데이트
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

                    // 슬롯 위치는 해당 Region의 bounds를 기준으로 계산 (regions.bounds는 InitializeRegions에서 windowPosition에 따라 이미 결정됨)
                    //float slotX = region.bounds.x + x * totalSlotDimension_x;
                    //float slotY = region.bounds.y + y * totalSlotDimension_y;

                    //float slotX = m_bound.x + RegionOffset[regionId].x + x * totalSlotDimension_x;
                    //float slotY = m_bound.y + RegionOffset[regionId].y + y * totalSlotDimension_y+ 64.0f;

                    // region.bounds.x와 region.bounds.y로 슬롯 위치 계산해야함.
                    float slotX = region.bounds.x + x * totalSlotDimension_x;
                    float slotY = region.bounds.y + y * totalSlotDimension_y;

                    slot.SetBounds(Rect(slotX, slotY, slotSize, slotSize));
                }
            }
        }
    }
}

ItemDatabase& Inventory::GetItemBase()
{
    return m_itemDatabase;
}

void Inventory::PackItem() //현재 database에 있는 모든 Item을 Slot에 넣어줌 
{

    for (const auto& [Id, Item] : m_itemDatabase.GetMap())
    {
        //std::cout << Item->m_data.id << endl;
        AddItem(Item->m_data.id, 1);
    }

}

void Inventory::SetDatabase(unique_ptr<ItemDatabase> database) //외부에서 만들고 넣어버릴래/
{
    //LoadItemDatabase 버전을 일단 쓰기로 함.

    //UI_Manager에서 어떻게 Init 할지는 모르겠다만, Scene Index 받고, Database에 넣는 거는 
    //GiveItem을 통해서 받으면 되가지고 상관 없긴 해 일단 버전 1로 두자고 
  //  this->itemDatabase = std::move(database);
}

void Inventory::LoadUIBitmaps()
{


    windowBackground.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("InvenBg").Get();
    tooltipBackground.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("SlotDisabled").Get(); //임의
    tooltipBackground.srcRect = D2D1::RectF(0, 0, 234, 345); // 예시. -> Tooltip 

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