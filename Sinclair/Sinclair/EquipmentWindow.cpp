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
        m_position = mousePos;

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
        return true; // ó�� �Ϸ�
    }

    // 3. ���� ���� Ŭ�� üũ
    Wearable_part clickedSlot = GetSlotTypeAt(mousePos);
    if (clickedSlot != Wearable_part::UnKnown)
    {
        // ���� Ŀ���� �������� �巡�� ������ üũ.
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
                // ���� ��� ������ ���콺������ ó�� 
                return true;
            }
        }
    }

    // 4. â ���� ���� �ٸ� �� Ŭ��. �׳� ������.
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
                // �巡�׵� �������� ���� Ÿ�� ã�Ƽ� ����
                for (auto& [slotType, item] : m_equippedItems)
                {
                    if (item == draggedItem)
                    {
                        UnequipItem(slotType);
                        break;
                    }
                }

                // UIManager�� ���� �κ��丮�� ������ �߰�
                if (auto* inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow)))
                {
                    inventory->AddItem(draggedItem->m_data.name, 1);
                }
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
            if (auto* inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow)))
            {
                inventory->AddItem(item->m_data.name, 1);
            }
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

    Wearable_part slotType = Wearable_part::UnKnown;
    if (item->m_data.name.find("Helmet") != std::string::npos) slotType = Wearable_part::Helmet;
    else if (item->m_data.name.find("Weapon") != std::string::npos) slotType = Wearable_part::Weapon;
    else if (item->m_data.name.find("Cape") != std::string::npos) slotType = Wearable_part::Cape;
    else if (item->m_data.name.find("Upper") != std::string::npos) slotType = Wearable_part::Upper;
    else if (item->m_data.name.find("Under") != std::string::npos) slotType = Wearable_part::Under;
    else if (item->m_data.name.find("Glove") != std::string::npos) slotType = Wearable_part::Glove;
    else if (item->m_data.name.find("Neckless") != std::string::npos) slotType = Wearable_part::Neckless;
    else if (item->m_data.name.find("EarRing") != std::string::npos) slotType = Wearable_part::Ring;
    else if (item->m_data.name.find("Shoes") != std::string::npos) slotType = Wearable_part::Shoes;

    if (slotType != Wearable_part::UnKnown)
    {
        // ������ ���� ���� �������� ������ �κ��丮�� ��ȯ
        Item* previousItem = m_equippedItems[slotType];
        if (previousItem)
        {
            if (auto* inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow)))
            {
                inventory->AddItem(previousItem->m_data.name, 1);
            }
        }

        // �� ������ ����
        m_equippedItems[slotType] = item;

        // ���� ������Ʈ
        // GameManager::Get().UpdatePlayerStats();
    }
}

void EquipmentWindow::RenderBackground()
{
    UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();
    if (uiRenderer)
    {
        ID2D1Bitmap1* backgroundBitmap = uiRenderer->GetBitmap("equipment_window_bg").Get();
        if (backgroundBitmap)
        {
            D2D1_RECT_F destRect = { m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y };
            D2DRenderer::Get().DrawBitmap(backgroundBitmap, destRect);
        }
        else
        {
            D2DRenderer::Get().DrawRectangle(m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y, D2D1::ColorF(D2D1::ColorF::DarkGray, 0.8f));
        }
    }
    else
    {
        D2DRenderer::Get().DrawRectangle(m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y, D2D1::ColorF(D2D1::ColorF::DarkGray, 0.8f));
    }
}

void EquipmentWindow::RenderTitleBar()
{
    UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();
    Vec2 titleBarSize = { m_size.x, 42.0f };

    if (uiRenderer)
    {
        ID2D1Bitmap1* titleBarBitmap = uiRenderer->GetBitmap("title_bar_bg").Get();
        D2D1_RECT_F destRect = { m_position.x, m_position.y, m_position.x + titleBarSize.x, m_position.y + titleBarSize.y };
        if (titleBarBitmap)
        {
            D2DRenderer::Get().DrawBitmap(titleBarBitmap, destRect);
        }
        else
        {
            D2DRenderer::Get().DrawRectangle(m_position.x, m_position.y, m_position.x + titleBarSize.x, m_position.y + titleBarSize.y, D2D1::ColorF(D2D1::ColorF::SteelBlue, 1.0f));
        }
    }
    else
    {
        D2DRenderer::Get().DrawRectangle(m_position.x, m_position.y, m_position.x + titleBarSize.x, m_position.y + titleBarSize.y, D2D1::ColorF(D2D1::ColorF::SteelBlue, 1.0f));
    }

    Vec2 textPos = m_position + Vec2(10, 10);
    D2DRenderer::Get().DrawMessage(L"Equipment", textPos.x, textPos.y, 100, 20, D2D1::ColorF(D2D1::ColorF::White));
}

void EquipmentWindow::RenderSlots()
{
    UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();

    for (const auto& [slotType, position] : m_slotPositions)
    {
        Vec2 slotSize = m_slotSizes[slotType];
        D2D1_RECT_F destRect = { position.x, position.y, position.x + slotSize.x, position.y + slotSize.y };

        if (uiRenderer)
        {
            ID2D1Bitmap1* slotBackgroundBitmap = uiRenderer->GetBitmap("empty_slot").Get(); // .Get() �߰�
            if (slotBackgroundBitmap)
            {
                D2DRenderer::Get().DrawBitmap(slotBackgroundBitmap, destRect);
            }
            else
            {
                D2DRenderer::Get().DrawRectangle(position.x, position.y, position.x + slotSize.x, position.y + slotSize.y, D2D1::ColorF(D2D1::ColorF::LightGray, 0.5f));
            }
        }
        else
        {
            D2DRenderer::Get().DrawRectangle(position.x, position.y, position.x + slotSize.x, position.y + slotSize.y, D2D1::ColorF(D2D1::ColorF::LightGray, 0.5f));
        }

        D2DRenderer::Get().DrawRectangle(position.x, position.y, position.x + slotSize.x, position.y + slotSize.y, D2D1::ColorF(D2D1::ColorF::Black));

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
            D2D1_RECT_F destRect = { slotPos.x, slotPos.y, slotPos.x + slotSize.x, slotPos.y + slotSize.y };

            //ID2D1Bitmap1* itemBitmap = ResourceManager::Get().GetTexture(item->GetImagePath()).Get(); // ���߿� ó��.

            /*if (itemBitmap)
            {
                D2D1_RECT_F srcRect = { 0, 0, (float)itemBitmap->GetSize().width, (float)itemBitmap->GetSize().height };
                D2DRenderer::Get().DrawBitmap(itemBitmap, destRect, srcRect);
            }
            else
            {
                D2DRenderer::Get().DrawCircle(slotPos.x + slotSize.x / 2, slotPos.y + slotSize.y / 2, slotSize.x / 3, D2D1::ColorF(D2D1::ColorF::Green));
            }*/

            // ������ ��ȭ ��ġ ǥ�� (+1, +2 ��)
            // if (item->GetEnhanceLevel() > 0)
            // {
            //     Vec2 enhanceTextPos = slotPos + Vec2(slotSize.x - 20, 5);
            //     std::string enhanceText = "+" + std::to_string(item->GetEnhanceLevel());
            //     D2DRenderer::Get().DrawMessage(std::wstring(enhanceText.begin(), enhanceText.end()).c_str(), enhanceTextPos.x, enhanceTextPos.y, 20, 15, D2D1::ColorF(D2D1::ColorF::Yellow));
            // }
        }
    }
}

void EquipmentWindow::RenderCloseButton()
{
    UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();

    float rightMargin = 47.0f;
    Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 7 };
    Vec2 closeButtonSize = { 27.0f, 27.0f };
    D2D1_RECT_F destRect = { closeButtonPos.x, closeButtonPos.y, closeButtonPos.x + closeButtonSize.x, closeButtonPos.y + closeButtonSize.y };

    if (uiRenderer)
    {
        ID2D1Bitmap1* closeButtonBitmap = uiRenderer->GetBitmap("close_button").Get();
        if (closeButtonBitmap)
        {
            D2DRenderer::Get().DrawBitmap(closeButtonBitmap, destRect);
        }
        else
        {
            D2DRenderer::Get().DrawRectangle(closeButtonPos.x, closeButtonPos.y, closeButtonPos.x + closeButtonSize.x, closeButtonPos.y + closeButtonSize.y, D2D1::ColorF(D2D1::ColorF::Red));
        }

        Vec2 mousePos = InputManager::Get().GetMousePosition();
        if (mousePos.x >= closeButtonPos.x && mousePos.x <= closeButtonPos.x + closeButtonSize.x &&
            mousePos.y >= closeButtonPos.y && mousePos.y <= closeButtonPos.y + closeButtonSize.y)
        {
            ID2D1Bitmap1* closeButtonHoverBitmap = uiRenderer->GetBitmap("close_button_hover").Get();
            if (closeButtonHoverBitmap)
            {
                D2DRenderer::Get().DrawBitmap(closeButtonHoverBitmap, destRect);
            }
            else
            {
                D2DRenderer::Get().DrawRectangle(closeButtonPos.x, closeButtonPos.y, closeButtonPos.x + closeButtonSize.x, closeButtonPos.y + closeButtonSize.y, D2D1::ColorF(D2D1::ColorF::Blue, 0.5f));
            }
        }
    }
    else
    {
        D2DRenderer::Get().DrawRectangle(closeButtonPos.x, closeButtonPos.y, closeButtonPos.x + closeButtonSize.x, closeButtonPos.y + closeButtonSize.y, D2D1::ColorF(D2D1::ColorF::Red));
        Vec2 mousePos = InputManager::Get().GetMousePosition();
        if (mousePos.x >= closeButtonPos.x && mousePos.x <= closeButtonPos.x + closeButtonSize.x &&
            mousePos.y >= closeButtonPos.y && mousePos.y <= closeButtonPos.y + closeButtonSize.y)
        {
            D2DRenderer::Get().DrawRectangle(closeButtonPos.x, closeButtonPos.y, closeButtonPos.x + closeButtonSize.x, closeButtonPos.y + closeButtonSize.y, D2D1::ColorF(D2D1::ColorF::Blue, 0.5f));
        }
    }
}

Item* EquipmentWindow::UnequipItem(Wearable_part slotType)
{
    auto it = m_equippedItems.find(slotType);
    if (it != m_equippedItems.end() && it->second != nullptr)
    {
        Item* item = it->second;
        m_equippedItems[slotType] = nullptr; // ���� ����

        // ���� ������Ʈ
        // GameManager::Get().UpdateStats();

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
    if (item->m_data.name.find("Helmet") != std::string::npos && slotType == Wearable_part::Helmet) return true;
    else if (item->m_data.name.find("Weapon") != std::string::npos && slotType == Wearable_part::Weapon) return true;
    else if (item->m_data.name.find("Cape") != std::string::npos && slotType == Wearable_part::Cape) return true;
    else if (item->m_data.name.find("Upper") != std::string::npos && slotType == Wearable_part::Upper) return true;
    else if (item->m_data.name.find("Under") != std::string::npos && slotType == Wearable_part::Under) return true;
    else if (item->m_data.name.find("Glove") != std::string::npos && slotType == Wearable_part::Glove) return true;
    else if (item->m_data.name.find("Neckless") != std::string::npos && slotType == Wearable_part::Neckless) return true;
    else if (item->m_data.name.find("EarRing") != std::string::npos && slotType == Wearable_part::Ring) return true;
    else if (item->m_data.name.find("Shoes") != std::string::npos && slotType == Wearable_part::Shoes) return true;

    return false;
}

void EquipmentWindow::UpdateSlotPositions()
{
    // â ��ġ�� �ٲ�� ���� ��ġ�鵵 ��������� ������Ʈ
    m_slotPositions[Wearable_part::Glove] = m_position + Vec2(222, 138);
    m_slotPositions[Wearable_part::Helmet] = m_position + Vec2(222, 290);
    m_slotPositions[Wearable_part::Ring] = m_position + Vec2(222, 442);
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
    case Wearable_part::Ring:
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