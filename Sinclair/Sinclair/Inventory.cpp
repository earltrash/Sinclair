#include "InvenTory.h"
#include "ResourceManager.h"
#include "ItemBank.h"
#

//: UI_Object(MWP) //생성자로 영역은 일단 설정함 (Inven 자기 영역 말임)
Inventory::Inventory() :UIWindow(UIWindowType::InventoryWindow, Vec2{ 0,0 }, Vec2{ 1097,766 }) 
{

    m_bound = { 0,0,1097,766 }; // 고정 

    windowPosition = { m_bound.x,m_bound.y };

    InitializeRegions();
    LoadUIBitmaps(); //멤버로 갖고 있는 닫기랑 윗 부분 (이건 Inven이랑 같이할 가능성도 있음) 
    InitializeSlots();

    LoadItemDatabase(Need_Moment::Gen_2);
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

    //타이틀 바 영역
    titleBarBounds = Rect(m_bound.x, m_bound.y, m_bound.width, 42.0f);

    //닫기 버튼 영역
    float closeButtonSize = 27.0f;
    closeButtonBounds = Rect(
        windowPosition.x + m_bound.width - (closeButtonSize +14),
        windowPosition.y + 7,
        closeButtonSize,
        closeButtonSize);

    //Inven 위치 
    windowBackground.position = windowPosition;
    windowBackground.size = Vec2(m_bound.width, m_bound.height);
    windowBackground.srcRect = D2D1::RectF(
        0.0f, 0.0f,
        static_cast<float>(m_bound.width),
        static_cast<float>(m_bound.height)
    );

    //닫기 버튼 랜더 위치 
    closeButton.position = Vec2(closeButtonBounds.x, closeButtonBounds.y);
    closeButton.size = Vec2(closeButtonBounds.width, closeButtonBounds.height);
    closeButton.srcRect = D2D1::RectF(0, 0, 27, 27); // 닫기 버튼 이미지 크기

    float slotSize = 74.0f;
    float padding_x = 13.0f;
    float padding_y = 12.0f;

    float totalSlotDimension_x = slotSize + padding_x;
    float totalSlotDimension_y = slotSize + padding_y;

    // windowBounds의 내부 영역을 기준으로 배치
    float currentRegionX = m_bound.x + 23.0f; // 윈도우 좌측 여백

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
    currentRegionX += (regions[0].bounds.width) + 23.0f; // 다음 영역 시작 X

    // Region 1: 중간 잠금 영역 (4x8)
    regions[1].id = 1;
    regions[1].isEnabled = false; // 잠금
    regions[1].gridSize = Vec2{ 4, 8 };
    regions[1].bounds = Rect(
        currentRegionX,
        m_bound.y + 64.0f,
        regions[1].gridSize.x * totalSlotDimension_x,
        regions[1].gridSize.y * totalSlotDimension_y
    );
    currentRegionX += regions[1].bounds.width + 15.0f;

    // Region 2: 오른쪽 잠금 영역 (4x8)
    regions[2].id = 2;
    regions[2].isEnabled = false; // 잠금
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

   
}

void Inventory::Render()
{
    // 1. 윈도우 배경 렌더링
    if (windowBackground.bitmap)
    {
        D2D1_RECT_F destRect = D2D1::RectF(
            m_bound.x, m_bound.y,
            m_bound.x + m_bound.width, m_bound.y + m_bound.height
        );
        D2D1_RECT_F srcRect = windowBackground.srcRect;

        D2DRenderer::Get().DrawBitmap(
            static_cast<ID2D1Bitmap1*>(windowBackground.bitmap),
            destRect,
            srcRect,
            windowBackground.opacity);
    }

    // 2. 타이틀바 및 닫기 버튼 렌더링
    RenderTitleBar();
    RenderCloseButton();

    // 3. 모든 슬롯 렌더링
    for (const auto& [key, slot] : slots)
    {
        RenderSlot(slot);
    }

    if (dragState.isDragging && dragState.dragBitmap.bitmap)
    {
        D2D1_RECT_F dragDestRect = D2D1::RectF(
            dragState.mousePos.x - dragState.dragBitmap.size.x / 2.0f, // 마우스 중앙에 오도록
            dragState.mousePos.y - dragState.dragBitmap.size.y / 2.0f,
            dragState.mousePos.x + dragState.dragBitmap.size.x / 2.0f,
            dragState.mousePos.y + dragState.dragBitmap.size.y / 2.0f
        );

        D2DRenderer::Get().DrawBitmap(static_cast<ID2D1Bitmap1*>(dragState.dragBitmap.bitmap),
            dragDestRect, dragState.dragBitmap.srcRect, 0.8f);
    }

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
        return;
    }

    // 윈도우 드래그 중일 때
    if (isWindowDragging)
    {
        float deltaX = mousePos.x - dragStartMousePos.x;
        float deltaY = mousePos.y - dragStartMousePos.y;
        windowPosition = Vec2(dragStartWindowPos.x + deltaX, dragStartWindowPos.y + deltaY);

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
            slot.isHovered = slot.bounds.Contains(mousePos);

            if (wasHovered != slot.isHovered)
            {
                slot.UpdateBackgroundBitmap(&controller);
            }

            if (slot.isHovered && !slot.IsEmpty())
            {
                hoveredSlot = &slot; // 현재 마우스가 올라간 슬롯
            }
        }

        // 툴팁 정보 업데이트
        if (hoveredSlot)
        {
            const Item* data = m_itemDatabase.GetItemData(hoveredSlot->item.id);
            if (data)
            {
                currentHoveredItemName = data->m_data.name;
                currentHoveredItemDescription = data->m_data.description;
                currentHoveredItemCount = hoveredSlot->item.count;
                // 툴팁 위치는 마우스 커서 옆으로 조정 (창이 이동해도 상대적으로 유지)
                tooltipPosition = Vec2(mousePos.x + 15, mousePos.y + 15);
               // showTooltip = true;
            }
        }
    }

    // 아이템 드래그 중일 때 (마우스 위치 업데이트)
    if (dragState.isDragging)
    {
        dragState.mousePos = mousePos;
    }
}

bool Inventory::HandleMouseDown(Vec2 mousePos)
{
    if (!m_isActive) return;

    // 닫기 버튼 클릭 처리
    if (closeButtonBounds.Contains(mousePos))
    {
        m_isActive = false; // 창 비활성화
       // showTooltip = false; // 툴팁 숨김
        return;
    }

    // 타이틀바 드래그 시작
    if (titleBarBounds.Contains(mousePos))
    {
        isWindowDragging = true;
        dragStartMousePos = mousePos;
        dragStartWindowPos = windowPosition;
        return; // 타이틀바 드래그 시작 시 다른 클릭 이벤트는 무시
    }

    // 슬롯 드래그 시작 (기존 로직)
    InventorySlot* slot = GetSlotAt(mousePos); // 현재 창 위치를 고려하여 슬롯 가져오기
    if (slot && !slot->IsEmpty() && slot->isEnabled)
    {
       dragState.isDragging = true;
        dragState.sourceSlot = slot;
        dragState.draggedItem = slot->item;
        dragState.mousePos = mousePos;

        const Item* itemData = m_itemDatabase.GetItemData(slot->item.id); //
        if (itemData)
        {
            //Itembank로부터 Item id를 받아서 해당 item에 해당하는 atlas의 영역을 보내 줘야 함. 
            dragState.dragBitmap.bitmap = ResourceManager::Get().Get_ItemBank().GetItemClip(slot->item.id).atlas.Get(); //id -> database에서 / Itembank에서 아틀라스 받아와야 함. 
            dragState.dragBitmap.srcRect = ResourceManager::Get().Get_ItemBank().GetItemClip(slot->item.id).srcRect;
            dragState.dragBitmap.size = Vec2(48, 48);
        }

        slot->Clear();
        slot->UpdateItemBitmap(&controller, &m_itemDatabase);
    }
}

bool Inventory::HandleMouseUp(Vec2 mousePos)
{
    if (!m_isActive) return;

    // 윈도우 드래그 종료
    if (isWindowDragging)
    {
        isWindowDragging = false;
        return;
    }

    // 아이템 드래그 종료
    if (!dragState.isDragging) return;

    bool placed = false;
    InventorySlot* targetSlot = GetSlotAt(mousePos); // 현재 마우스 위치의 슬롯을 다시 찾음

    if (targetSlot && targetSlot->isEnabled)
    {
        // 스택 가능한 아이템이라면 합치기 시도
        const Item* draggedItemData = m_itemDatabase.GetItemData(dragState.draggedItem.id);
        if (draggedItemData && draggedItemData->IsStackable() &&
            targetSlot->item.id == dragState.draggedItem.id &&
            (targetSlot->item.count + dragState.draggedItem.count) <= draggedItemData->maxCount)
        {
            targetSlot->item.count += dragState.draggedItem.count;
            targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
            placed = true;
        }
        else if (targetSlot->IsEmpty())
        {
            // 빈 슬롯에 드롭
            targetSlot->item = dragState.draggedItem;
            targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
            placed = true;
        }
        else
        {
            // 다른 아이템이 있는 슬롯에 드롭 (교환 로직 추가 가능)
            // 현재는 그냥 원래 위치로 되돌림
        }
    }

    // 배치 실패시 원래 위치로 복귀
    if (!placed)
    {
        dragState.sourceSlot->item = dragState.draggedItem;
        dragState.sourceSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
    }

    dragState.isDragging = false;
}

bool Inventory::HandleDoubleClick(Vec2 mousePos)
{
    return false;
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
    D2D1_RECT_F destRect = closeButtonBounds.ToD2DRect();
    D2D1_RECT_F srcRect = closeButton.srcRect;
    D2DRenderer::Get().DrawBitmap(static_cast<ID2D1Bitmap1*>(closeButton.bitmap), destRect, srcRect, closeButton.opacity);
}

void Inventory::RenderSlot(const InventorySlot& slot)
{
    if (slot.backgroundBitmap.bitmap)
    {
        std::cout << "배경 비트맵 렌더링 시도" << std::endl;

        D2D1_RECT_F destRect = slot.bounds.ToD2DRect();
        D2D1_RECT_F srcRect = slot.backgroundBitmap.srcRect;
        D2DRenderer::Get().DrawBitmap
        (
            static_cast<ID2D1Bitmap1*>(slot.backgroundBitmap.bitmap),
            destRect,
            srcRect,
            slot.backgroundBitmap.opacity);

        std::cout << "배경 비트맵 렌더링 완료" << std::endl;
    }

    // 비활성화된 슬롯은 빈slot이미지 그림.
    if (!slot.isEnabled)
    {
        std::cout << "비활성화된 슬롯이므로 배경만 렌더링하고 종료" << std::endl;
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

        // 3. 아이템 개수 텍스트
        if (slot.item.count >1)
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

void Inventory::UpdateSlotPositions()
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

                if (slots.count(key))
                {
                    InventorySlot& slot = slots[key];

                    // 슬롯 위치는 해당 Region의 bounds를 기준으로 계산 (regions.bounds는 InitializeRegions에서 windowPosition에 따라 이미 결정됨)
                    float slotX = region.bounds.x + x * totalSlotDimension_x;
                    float slotY = region.bounds.y + y * totalSlotDimension_y;

                    slot.SetBounds(Rect(slotX, slotY, slotSize, slotSize));
                }
            }
        }
    }
}

void Inventory::PackItem() //현재 database에 있는 모든 Item을 Slot에 넣어줌 
{

    for (const auto& [Id, Item] : m_itemDatabase.GetMap())
    {
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
    

    windowBackground.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("InvenBG").Get();
    tooltipBackground.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("ToolTipBG").Get();
    tooltipBackground.srcRect = D2D1::RectF(0, 0, 234, 345); // 예시. -> Tooltip 
    closeButton.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("InvenClose").Get();
    

    controller.bitmaps.emplace("slot_disabled",
        ResourceManager::Get().Get_UIBank().Get_Image("Slotdisabled").Get());
    controller.bitmaps.emplace("slot_hover",
        ResourceManager::Get().Get_UIBank().Get_Image("slot_hover").Get());
    controller.bitmaps.emplace("slot_selected",
        ResourceManager::Get().Get_UIBank().Get_Image("slot_selected").Get());
    controller.bitmaps.emplace("slot_normal",
        ResourceManager::Get().Get_UIBank().Get_Image("slot_normal").Get());

}

void Inventory::LoadItemDatabase(Need_Moment Moment)
{
    ResourceManager::Get().Get_ItemBank().GiveItem(Moment, m_itemDatabase);
}

