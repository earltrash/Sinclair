#include "EquipmentWindow.h"
#include "InputManager.h"
#include "CursorManager.h"
#include "UIManager.h"
#include "Renderer.h"

void EquipmentWindow::Update()
{
    // �巡�� ���̸� â ��ġ ������Ʈ
    if (m_isDragging)
    {
        Vec2 mousePos = InputManager::Get().GetMousePosition();
        m_position = mousePos - m_dragOffset;

        // â ��ġ�� �ٲ�� ���� ��ġ�鵵 �ٽ� ���
        UpdateSlotPositions();
    }
}

void EquipmentWindow::Render()
{
    if (!m_isActive) return;

    // ������ ����: ��� �� Ÿ��Ʋ�� �� ���Ե� �� ������ �����۵� �� �ݱ� ��ư
    RenderBackground();
    RenderTitleBar();
    RenderSlots();
    RenderEquippedItems();
    RenderCloseButton();
}

bool EquipmentWindow::HandleMouseDown(Vec2 mousePos)
{
    // 1. �ݱ� ��ư Ŭ�� üũ
    if (IsInCloseButton(mousePos))
    {
        UIManager::Get().CloseWindow(UIWindowType::EquipmentWindow);
        return true; // ó�� �Ϸ�
    }

    // 2. Ÿ��Ʋ�� Ŭ�� - â �巡�� ����
    if (IsInTitleBar(mousePos))
    {
        m_isDragging = true;
        m_dragOffset = mousePos - m_position;
        return true; // ó�� �Ϸ�
    }

    // 3. ���� ���� Ŭ�� üũ
    Wearable_part clickedSlot = GetSlotTypeAt(mousePos);
    if (clickedSlot != Wearable_part::UnKnown)
    {
        // ���� Ŀ���� �������� �巡�� ���ΰ�?
        if (CursorManager::Get().IsDragging())
        {
            // �巡�� ���� ������ ��������
            Item* draggedItem = CursorManager::Get().GetDraggedItem();

            if (draggedItem && CanEquipItem(draggedItem, clickedSlot))
            {
                // ��� ���� ����
                EquipItem(draggedItem);
                CursorManager::Get().EndItemDrag();
                return true;
            }
            else
            {
                // ���� �Ұ����� ���Կ� ��� �õ� - �巡�� ���
                CursorManager::Get().EndItemDrag();
                return true;
            }
        }
        else
        {
            // �巡�� ���� �ƴ� ��� - �ش� ������ �������� �巡�� ����
            Item* equippedItem = GetEquippedItem(clickedSlot);
            if (equippedItem)
            {
                CursorManager::Get().StartItemDrag(*equippedItem, DragSource::Equipment);
                // ���� ��� ������ ���콺������ ó�� (�巡�װ� ���������� �Ϸ�Ǹ�)
                return true;
            }
        }
    }

    // 4. â ���� ���� �ٸ� �� Ŭ�� - �Է��� �Һ� (�ٸ� â���� ���� ����)
    return true;
}

bool EquipmentWindow::HandleMouseUp(Vec2 mousePos)
{
    // â �巡�� ����
    if (m_isDragging)
    {
        m_isDragging = false;
        return true;
    }

    // ������ �巡�װ� ���� ���̰�, ���â �ۿ��� ���콺�� �ߴٸ�
    if (CursorManager::Get().IsDragging() &&
        CursorManager::Get().GetDragSource() == DragSource::Equipment)
    {
        // ���â ���� ���̸� �������� �κ��丮�� �̵�
        if (!IsInBounds(mousePos))
        {
            Item* draggedItem = CursorManager::Get().GetDraggedItem();
            if (draggedItem)
            {
                // �ش� ���Կ��� ������ ����
                Wearable_part sourceSlot = draggedItem->GetWearablePart();
                UnequipItem(sourceSlot);

                // �κ��丮�� ������ �̵� 
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
    // ���� ����Ŭ�� �� ������ ����
    Wearable_part clickedSlot = GetSlotTypeAt(mousePos);
    if (clickedSlot != Wearable_part::UnKnown)
    {
        Item* item = UnequipItem(clickedSlot);
        if (item)
        {
            // �κ��丮�� ������ ��ȯ
            //m_inventory->AddItem(item);
            return true;
        }
    }

    return false;
}

bool EquipmentWindow::HandleMouseHover(Vec2 mousePos)
{
    // ���Կ� ���콺 ���� �� ���� ǥ��
    Wearable_part hoveredSlot = GetSlotTypeAt(mousePos);

    if (hoveredSlot != Wearable_part::UnKnown)
    {
        Item* item = GetEquippedItem(hoveredSlot);
        if (item)
        {
            // ���� ǥ�� ��ġ�� ���콺 ������ ����
            Vec2 tooltipPos = mousePos + Vec2(10, 10);
            UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, tooltipPos);
            return true;
        }
    }
    else
    {
        // ���콺�� ���� ������ ������ ���� �����
        UIManager::Get().HideTooltip(UIWindowType::InventoryTooltip);
    }

    return false;
}

void EquipmentWindow::EquipItem(Item* item)
{
    if (!item) return;

    Wearable_part slotType = item->GetWearablePart();

    // ������ ���� ���� �������� ������ ��ü
    Item* previousItem = m_equippedItems[slotType];
    if (previousItem)
    {
        // ���� �������� �κ��丮�� ��ȯ
        // InventoryManager::Get().AddItem(previousItem);
    }

    // �� ������ ����
    m_equippedItems[slotType] = item;

    // �÷��̾� ���� ������Ʈ
    // PlayerManager::Get().UpdateStats();

    OutputDebugStringA(("Equipped item to slot: " + std::to_string(static_cast<int>(slotType)) + "\n").c_str());
}

Item* EquipmentWindow::UnequipItem(Wearable_part slotType)
{
    auto it = m_equippedItems.find(slotType);
    if (it != m_equippedItems.end() && it->second != nullptr)
    {
        Item* item = it->second;
        m_equippedItems[slotType] = nullptr; // ���� ����

        // �÷��̾� ���� ������Ʈ
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
    // â ��� ������
    // Renderer::Get().DrawRectangle(m_position, m_size, Color::WindowBackground);
    // Renderer::Get().DrawImage("equipment_window_bg.png", m_position);

}

void EquipmentWindow::RenderTitleBar()
{
    // Ÿ��Ʋ�� ���
    Vec2 titleBarSize = { m_size.x, 42.0f };
    // Renderer::Get().DrawRectangle(m_position, titleBarSize, Color::TitleBar);

    // Ÿ��Ʋ �ؽ�Ʈ
    Vec2 textPos = m_position + Vec2(10, 10);
    // Renderer::Get().DrawText("Equipment", textPos, Color::White, FontSize::Medium);
}

void EquipmentWindow::RenderSlots()
{
    // ��� ���� ������
    for (const auto& [slotType, position] : m_slotPositions)
    {
        Vec2 slotSize = m_slotSizes[slotType];

        // ���� ��� (�� ���� �̹���)
        // Renderer::Get().DrawImage("empty_slot.png", position, slotSize);

        // ����׿� �׵θ�
        // Renderer::Get().DrawRectangleOutline(position, slotSize, Color::SlotBorder);

        // ���� Ÿ�Ժ� ������ (� ��� ������ ǥ��)
        RenderSlotIcon(slotType, position);
    }
}

void EquipmentWindow::RenderEquippedItems()
{
    // ����� �����۵� ������
    for (const auto& [slotType, item] : m_equippedItems)
    {
        if (item != nullptr)
        {
            Vec2 slotPos = m_slotPositions[slotType];
            Vec2 slotSize = m_slotSizes[slotType];

            // ������ �̹��� ������
            // Renderer::Get().DrawImage(item->GetImagePath(), slotPos, slotSize);

            // ������ ��ȭ ��ġ ǥ�� (+1, +2 ��)
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
    // �ݱ� ��ư ��ġ ��� (UIWindow�� IsInCloseButton�� ������ ����)
    float rightMargin = 47.0f;
    Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 7 };
    Vec2 closeButtonSize = { 27.0f, 27.0f };

    // �ݱ� ��ư �̹���
    // Renderer::Get().DrawImage("close_button.png", closeButtonPos, closeButtonSize);

    // ���콺 ���� ȿ��
    Vec2 mousePos = InputManager::Get().GetMousePosition();
    if (mousePos.x >= closeButtonPos.x && mousePos.x <= closeButtonPos.x + closeButtonSize.x &&
        mousePos.y >= closeButtonPos.y && mousePos.y <= closeButtonPos.y + closeButtonSize.y)
    {
        // ȣ�� ȿ��
        // Renderer::Get().DrawImage("close_button_hover.png", closeButtonPos, closeButtonSize);
    }
}

Wearable_part EquipmentWindow::GetSlotTypeAt(Vec2 mousePos) const
{
    // ��� ������ Ȯ���Ͽ� ���콺 ��ġ�� �ش��ϴ� ���� ã��
    for (const auto& [slotType, position] : m_slotPositions)
    {
        Vec2 slotSize = m_slotSizes.at(slotType);

        if (mousePos.x >= position.x && mousePos.x <= position.x + slotSize.x &&
            mousePos.y >= position.y && mousePos.y <= position.y + slotSize.y)
        {
            return slotType;
        }
    }

    return Wearable_part::UnKnown; // �ش��ϴ� ������ ����
}

bool EquipmentWindow::CanEquipItem(Item* item, Wearable_part slotType) const
{
    if (!item) return false;

    // �������� ���� ������ ���� Ÿ���� ��ġ�ϴ��� Ȯ��
    return item->GetWearablePart() == slotType;
}

void EquipmentWindow::UpdateSlotPositions()
{
    // â ��ġ�� �ٲ�� ���� ��ġ�鵵 ��������� ������Ʈ
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
    // ���Ժ� ������ ������ (� ��� ������ ǥ��)
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