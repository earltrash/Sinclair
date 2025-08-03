#include "EquipmentWindow.h"
#include "InputManager.h"
#include "CursorManager.h"
#include "UIManager.h"
#include "Renderer.h"

void EquipmentWindow::Update()
{
    // 드래그 중이면 창 위치 업데이트
    if (m_isDragging)
    {
        Vec2 mousePos = InputManager::Get().GetMousePosition();
        m_position = mousePos - m_dragOffset;

        // 창 위치가 바뀌면 슬롯 위치들도 다시 계산
        UpdateSlotPositions();
    }
}

void EquipmentWindow::Render()
{
    if (!m_isActive) return;

    // 렌더링 순서: 배경 → 타이틀바 → 슬롯들 → 장착된 아이템들 → 닫기 버튼
    RenderBackground();
    RenderTitleBar();
    RenderSlots();
    RenderEquippedItems();
    RenderCloseButton();
}

bool EquipmentWindow::HandleMouseDown(Vec2 mousePos)
{
    // 1. 닫기 버튼 클릭 체크
    if (IsInCloseButton(mousePos))
    {
        UIManager::Get().CloseWindow(UIWindowType::EquipmentWindow);
        return true; // 처리 완료
    }

    // 2. 타이틀바 클릭 - 창 드래그 시작
    if (IsInTitleBar(mousePos))
    {
        m_isDragging = true;
        m_dragOffset = mousePos - m_position;
        return true; // 처리 완료
    }

    // 3. 슬롯 영역 클릭 체크
    Wearable_part clickedSlot = GetSlotTypeAt(mousePos);
    if (clickedSlot != Wearable_part::UnKnown)
    {
        // 현재 커서가 아이템을 드래그 중인가?
        if (CursorManager::Get().IsDragging())
        {
            // 드래그 중인 아이템 가져오기
            Item* draggedItem = CursorManager::Get().GetDraggedItem();

            if (draggedItem && CanEquipItem(draggedItem, clickedSlot))
            {
                // 장비 착용 가능
                EquipItem(draggedItem);
                CursorManager::Get().EndItemDrag();
                return true;
            }
            else
            {
                // 착용 불가능한 슬롯에 드롭 시도 - 드래그 취소
                CursorManager::Get().EndItemDrag();
                return true;
            }
        }
        else
        {
            // 드래그 중이 아닌 경우 - 해당 슬롯의 아이템을 드래그 시작
            Item* equippedItem = GetEquippedItem(clickedSlot);
            if (equippedItem)
            {
                CursorManager::Get().StartItemDrag(*equippedItem, DragSource::Equipment);
                // 실제 장비 해제는 마우스업에서 처리 (드래그가 성공적으로 완료되면)
                return true;
            }
        }
    }

    // 4. 창 영역 내의 다른 곳 클릭 - 입력을 소비 (다른 창으로 전달 방지)
    return true;
}

bool EquipmentWindow::HandleMouseUp(Vec2 mousePos)
{
    // 창 드래그 종료
    if (m_isDragging)
    {
        m_isDragging = false;
        return true;
    }

    // 아이템 드래그가 진행 중이고, 장비창 밖에서 마우스업 했다면
    if (CursorManager::Get().IsDragging() &&
        CursorManager::Get().GetDragSource() == DragSource::Equipment)
    {
        // 장비창 영역 밖이면 아이템을 인벤토리로 이동
        if (!IsInBounds(mousePos))
        {
            Item* draggedItem = CursorManager::Get().GetDraggedItem();
            if (draggedItem)
            {
                // 해당 슬롯에서 아이템 제거
                Wearable_part sourceSlot = draggedItem->GetWearablePart();
                UnequipItem(sourceSlot);

                // 인벤토리로 아이템 이동 
                //m_inventory->AddItem(draggedItem);
            }

            CursorManager::Get().EndItemDrag();
            return true;
        }
    }

    return false;
}

bool EquipmentWindow::HandleDoubleClick(Vec2 mousePos)
{
    // 슬롯 더블클릭 시 아이템 해제
    Wearable_part clickedSlot = GetSlotTypeAt(mousePos);
    if (clickedSlot != Wearable_part::UnKnown)
    {
        Item* item = UnequipItem(clickedSlot);
        if (item)
        {
            // 인벤토리로 아이템 반환
            //m_inventory->AddItem(item);
            return true;
        }
    }

    return false;
}

bool EquipmentWindow::HandleMouseHover(Vec2 mousePos)
{
    // 슬롯에 마우스 오버 시 툴팁 표시
    Wearable_part hoveredSlot = GetSlotTypeAt(mousePos);

    if (hoveredSlot != Wearable_part::UnKnown)
    {
        Item* item = GetEquippedItem(hoveredSlot);
        if (item)
        {
            // 툴팁 표시 위치를 마우스 옆으로 조정
            Vec2 tooltipPos = mousePos + Vec2(10, 10);
            UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, tooltipPos);
            return true;
        }
    }
    else
    {
        // 마우스가 슬롯 밖으로 나가면 툴팁 숨기기
        UIManager::Get().HideTooltip(UIWindowType::InventoryTooltip);
    }

    return false;
}

void EquipmentWindow::EquipItem(Item* item)
{
    if (!item) return;

    Wearable_part slotType = item->GetWearablePart();

    // 기존에 착용 중인 아이템이 있으면 교체
    Item* previousItem = m_equippedItems[slotType];
    if (previousItem)
    {
        // 기존 아이템을 인벤토리로 반환
        // InventoryManager::Get().AddItem(previousItem);
    }

    // 새 아이템 착용
    m_equippedItems[slotType] = item;

    // 플레이어 스탯 업데이트
    // PlayerManager::Get().UpdateStats();

    OutputDebugStringA(("Equipped item to slot: " + std::to_string(static_cast<int>(slotType)) + "\n").c_str());
}

Item* EquipmentWindow::UnequipItem(Wearable_part slotType)
{
    auto it = m_equippedItems.find(slotType);
    if (it != m_equippedItems.end() && it->second != nullptr)
    {
        Item* item = it->second;
        m_equippedItems[slotType] = nullptr; // 슬롯 비우기

        // 플레이어 스탯 업데이트
        // PlayerManager::Get().UpdateStats();

        OutputDebugStringA(("Unequipped item from slot: " + std::to_string(static_cast<int>(slotType)) + "\n").c_str());
        return item;
    }

    return nullptr;
}

Item* EquipmentWindow::GetEquippedItem(Wearable_part slotType) const
{
    auto it = m_equippedItems.find(slotType);
    return (it != m_equippedItems.end()) ? it->second : nullptr;
}

void EquipmentWindow::RenderBackground()
{
    // 창 배경 렌더링
    // Renderer::Get().DrawRectangle(m_position, m_size, Color::WindowBackground);
    // Renderer::Get().DrawImage("equipment_window_bg.png", m_position);

}

void EquipmentWindow::RenderTitleBar()
{
    // 타이틀바 배경
    Vec2 titleBarSize = { m_size.x, 42.0f };
    // Renderer::Get().DrawRectangle(m_position, titleBarSize, Color::TitleBar);

    // 타이틀 텍스트
    Vec2 textPos = m_position + Vec2(10, 10);
    // Renderer::Get().DrawText("Equipment", textPos, Color::White, FontSize::Medium);
}

void EquipmentWindow::RenderSlots()
{
    // 모든 슬롯 렌더링
    for (const auto& [slotType, position] : m_slotPositions)
    {
        Vec2 slotSize = m_slotSizes[slotType];

        // 슬롯 배경 (빈 슬롯 이미지)
        // Renderer::Get().DrawImage("empty_slot.png", position, slotSize);

        // 디버그용 테두리
        // Renderer::Get().DrawRectangleOutline(position, slotSize, Color::SlotBorder);

        // 슬롯 타입별 아이콘 (어떤 장비가 들어가는지 표시)
        RenderSlotIcon(slotType, position);
    }
}

void EquipmentWindow::RenderEquippedItems()
{
    // 착용된 아이템들 렌더링
    for (const auto& [slotType, item] : m_equippedItems)
    {
        if (item != nullptr)
        {
            Vec2 slotPos = m_slotPositions[slotType];
            Vec2 slotSize = m_slotSizes[slotType];

            // 아이템 이미지 렌더링
            // Renderer::Get().DrawImage(item->GetImagePath(), slotPos, slotSize);

            // 아이템 강화 수치 표시 (+1, +2 등)
            if (item->GetEnhanceLevel() > 0)
            {
                Vec2 enhanceTextPos = slotPos + Vec2(slotSize.x - 20, 5);
                std::string enhanceText = "+" + std::to_string(item->GetEnhanceLevel());
                // Renderer::Get().DrawText(enhanceText, enhanceTextPos, Color::Yellow, FontSize::Small);
            }
        }
    }
}

void EquipmentWindow::RenderCloseButton()
{
    // 닫기 버튼 위치 계산 (UIWindow의 IsInCloseButton과 동일한 로직)
    float rightMargin = 47.0f;
    Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 7 };
    Vec2 closeButtonSize = { 27.0f, 27.0f };

    // 닫기 버튼 이미지
    // Renderer::Get().DrawImage("close_button.png", closeButtonPos, closeButtonSize);

    // 마우스 오버 효과
    Vec2 mousePos = InputManager::Get().GetMousePosition();
    if (mousePos.x >= closeButtonPos.x && mousePos.x <= closeButtonPos.x + closeButtonSize.x &&
        mousePos.y >= closeButtonPos.y && mousePos.y <= closeButtonPos.y + closeButtonSize.y)
    {
        // 호버 효과
        // Renderer::Get().DrawImage("close_button_hover.png", closeButtonPos, closeButtonSize);
    }
}

Wearable_part EquipmentWindow::GetSlotTypeAt(Vec2 mousePos) const
{
    // 모든 슬롯을 확인하여 마우스 위치에 해당하는 슬롯 찾기
    for (const auto& [slotType, position] : m_slotPositions)
    {
        Vec2 slotSize = m_slotSizes.at(slotType);

        if (mousePos.x >= position.x && mousePos.x <= position.x + slotSize.x &&
            mousePos.y >= position.y && mousePos.y <= position.y + slotSize.y)
        {
            return slotType;
        }
    }

    return Wearable_part::UnKnown; // 해당하는 슬롯이 없음
}

bool EquipmentWindow::CanEquipItem(Item* item, Wearable_part slotType) const
{
    if (!item) return false;

    // 아이템의 착용 부위와 슬롯 타입이 일치하는지 확인
    return item->GetWearablePart() == slotType;
}

void EquipmentWindow::UpdateSlotPositions()
{
    // 창 위치가 바뀌면 슬롯 위치들도 상대적으로 업데이트
    m_slotPositions[Wearable_part::Glove] = m_position + Vec2(222, 138);
    m_slotPositions[Wearable_part::Helmet] = m_position + Vec2(222, 290);
    m_slotPositions[Wearable_part::EarRing] = m_position + Vec2(222, 442);
    m_slotPositions[Wearable_part::Upper] = m_position + Vec2(222, 593);
    m_slotPositions[Wearable_part::Under] = m_position + Vec2(74, 214);
    m_slotPositions[Wearable_part::Shoes] = m_position + Vec2(74, 365);
    m_slotPositions[Wearable_part::Weapon] = m_position + Vec2(74, 214);
    m_slotPositions[Wearable_part::Neckless] = m_position + Vec2(370, 214);
    m_slotPositions[Wearable_part::Cape] = m_position + Vec2(370, 365);
}

void EquipmentWindow::RenderSlotIcon(Wearable_part slotType, Vec2 position)
{
    // 슬롯별 아이콘 렌더링 (어떤 장비가 들어가는지 표시)
    std::string iconPath;

    switch (slotType)
    {
    case Wearable_part::Helmet:
        iconPath = "icon_helmet_slot.png";
        break;
    case Wearable_part::Weapon:
        iconPath = "icon_weapon_slot.png";
        break;
    case Wearable_part::Upper:
        iconPath = "icon_armor_slot.png";
        break;
    case Wearable_part::Under:
        iconPath = "icon_pants_slot.png";
        break;
    case Wearable_part::Glove:
        iconPath = "icon_glove_slot.png";
        break;
    case Wearable_part::Shoes:
        iconPath = "icon_shoes_slot.png";
        break;
    case Wearable_part::Neckless:
        iconPath = "icon_necklace_slot.png";
        break;
    case Wearable_part::EarRing:
        iconPath = "icon_earring_slot.png";
        break;
    case Wearable_part::Cape:
        iconPath = "icon_cape_slot.png";
        break;
    }

    if (!iconPath.empty())
    {
        // Renderer::Get().DrawImage(iconPath, position, m_slotSizes.at(slotType));
    }
}