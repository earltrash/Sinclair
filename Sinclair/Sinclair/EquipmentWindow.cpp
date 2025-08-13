#include "EquipmentWindow.h"
#include "InputManager.h"
#include "CursorManager.h"
#include "UIManager.h"
#include "Renderer.h"

#include "Wearable.h"

#include "EffectComponent.h"
#include "PlayComponent.h"

void EquipmentWindow::AddEffect(Object* slot)
{
    auto info = slot->GetRenderInfo();
    auto shadow = slot->AddComponent<Shadow_Effect>(info, 3.f, 1.f, 1.f, 1.f, 1.f, info->GetBitmap());
    auto scale = slot->AddComponent<Scale_Effect>(info, 37.f, 37.f, 1.1f, 1.1f, shadow->GetEffect());
    auto blink = slot->AddComponent<Blink_Effect>(info, 0.2f, 1.f, scale->GetEffect());
    auto comp = slot->AddComponent<Composite_Effect>(info, info->GetBitmap(), blink->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
}

void EquipmentWindow::Update() // �̰Ŵ� Inven ��ó�� Update ���� �޽��� ���� �� �ϸ� �� �� 
{
    for (auto& slot : m_slots)
    {
        slot.second->Update();
    }
}

void EquipmentWindow::Render()
{
    if (!m_isActive) return;

    // ������ ����: ��� �� Ÿ��Ʋ�� �� ���Ե� �� ������ �����۵� �� �ݱ� ��ư
    RenderBackground();
    // RenderTitleBar();
    RenderSlots();
    RenderEquippedItems();
    RenderCloseButton();
}

bool EquipmentWindow::HandleMouseDown(Vec2 mousePos)
{
    if (!m_isActive) return false;

    // 2. ���� ���� Ŭ�� üũ
    Wearable_part clickedSlot = GetSlotTypeAt(mousePos);
    if (clickedSlot != Wearable_part::UnKnown)
    {
        auto it = m_equippedItems.find(clickedSlot);
        if (it != m_equippedItems.end() && it->second != nullptr)
        {
            Item* equippedItem = it->second;
            CursorManager::Get().StartItemDrag(equippedItem->m_data.id, DragSource::Equipment);
            CursorManager::Get().SetDraggedItem(equippedItem);

            // �巡�� ���� �� �������� �ӽ÷� ����
            m_equippedItems[clickedSlot] = nullptr;
            return true;
        }
        return true;
    }
    // â ���� Ŭ�� �̺�Ʈ ó�� �Ϸ�. �׷��� ȭ�� �ֻ������ �ø���.
    if (IsInBounds(mousePos))
    {
        UIManager::Get().OpenWindow(m_windowType);
        return true;
    }

    // ���� ���̸� üũ����.
    return false;
}

bool EquipmentWindow::HandleMouseUp(Vec2 mousePos)
{
    // â�� ��Ȱ��ȭ ���¸� ó������ ����
    if (!m_isActive) return false;

    //  ���� �������� �巡�� ������ Ȯ��
    if (CursorManager::Get().IsDragging())
    {
        // �巡�� ���� �����۰� �ҽ� ���� ��������
        Item* draggedItem = CursorManager::Get().GetDraggedItem();
        DragSource dragSource = CursorManager::Get().GetDragSource();

        // ���� ������Ʈ�� blink effect ����
        //SendEventToComponent("STOP", draggedItem->m_data.wearablePart);

        // ���콺�� ���� ��ġ�� ��ȿ�� ��� �������� Ȯ��
        Wearable_part slotType = GetSlotTypeAt(mousePos);

        // �巡�� ���� �������� ���� ������ ���������� Ȯ�� (dynamic_cast)
        Wearable* wearableItem = dynamic_cast<Wearable*>(draggedItem);

        // ��ȿ�� ���Կ�, ���� ������ �������� ����ߴ��� ���� üũ
        if (slotType != Wearable_part::UnKnown && wearableItem && IsItemTypeMatch(wearableItem->Getpart(), slotType))
        {
            //std::cout << "DEBUG: ��� ���� ���� ����. �������� �����մϴ�." << std::endl;
            // ��� ���� ����
            
            Item* previousItem = GetEquippedItem(slotType); // ���� ���Կ� �ִ� ������
            m_equippedItems[slotType] = draggedItem;

            // ���� �������� �־��ٸ� �κ��丮�� ����.
            if (previousItem)
            {
                auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
                inventoryWindow->AddItem(previousItem->m_data.id, 1);

                std::cout << "DEBUG: ���� �������� �־� �κ��丮�� ������Ŵ." << std::endl;
            }

            // ���� ���� �ٽ� �����ͼ� �ٽ� ������ֱ�.
            if (auto* statWindow = dynamic_cast<StatWindow*>(UIManager::Get().GetWindow(UIWindowType::StatsWindow)))
            {
                statWindow->UpdateTotalStats();
            }

            // �巡�� ����.
            CursorManager::Get().EndItemDrag();
            return true;
        }
        // uimanager�� �˾Ƽ� ���� ó��.
        return false;
    }

    //  â ���� Ŭ�� �� �ֻ������ �ø��� (�巡�� ��� ���� ���Ŀ� ó��)
    if (IsInBounds(mousePos))
    {
        UIManager::Get().OpenWindow(m_windowType);
        return true;
    }

    return false;
}


bool EquipmentWindow::HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source)
{
    if (!draggedItem) return false;

        // � â ������ �ƴϸ� ���� ��ġ�� ����
        if (source == DragSource::Equipment || source == DragSource::Inventory || source == DragSource::Enhancement)
        {
            // �κ��丮���� �� �������̹Ƿ� �κ��丮�� ����
            auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
            if (inventoryWindow)
            {
                inventoryWindow->AddItem(draggedItem->m_data.id, 1);
                std::cout << "�κ��丮 �������� �κ��丮�� �����߽��ϴ�: " << draggedItem->m_data.name << std::endl;
            }
        }

    CursorManager::Get().EndItemDrag();
    return true;
}

bool EquipmentWindow::HandleDoubleClick(Vec2 mousePos)
{
    //// ���� ����Ŭ�� �� ������ ����
    //Wearable_part clickedSlot = GetSlotTypeAt(mousePos);
    //if (clickedSlot != Wearable_part::UnKnown)
    //{
    //    Item* item = UnequipItem(clickedSlot);
    //    if (item)
    //    {
    //        if (auto* inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow)))
    //        {
    //            inventory->AddItem(item->m_data.name, 1);
    //        }
    //        return true;
    //    }
    //}

    //return false;
    return false;
}

bool EquipmentWindow::HandleMouseHover(Vec2 mousePos)
{

    if (!m_isActive) return false;

    // ���콺�� ���â�� ��� �ȿ� �ִ��� ���� Ȯ��
    if (!IsInBounds(mousePos))
    {
        // ��� �ۿ� �ִٸ� ������ ��Ȱ��ȭ�ϰ� �Լ� ����
        CursorManager::Get().HoveredReleased();
        UIManager::Get().CloseWindow(UIWindowType::InventoryTooltip);
        return false;
    }

    if (m_isDragging)
    {
        m_position = mousePos - m_dragOffset;

        // â ��ġ�� �ٲ�� ���� ��ġ�鵵 �ٽ� ���
        UpdateSlotPositions();
    }


    // ���Կ� ���콺 ���� �� ���� ǥ��
    Wearable_part hoveredSlot = GetSlotTypeAt(mousePos);

    if (hoveredSlot != Wearable_part::UnKnown) //��� ��¥ �ִ� ��� 
    {
        Item* item = GetEquippedItem(hoveredSlot);
        if (item)
        {
            // ���� ǥ�� ��ġ�� ���콺 ������ ����
            Vec2 tooltipPos = mousePos + Vec2(10, 10);
            CursorManager::Get().SetHoveredItem(item);
            UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, tooltipPos); //Ȱ��ȭ ��, Item ���� ������� ��.

            return true;
        }
    }

    // ���콺�� ���â ��� ��, ���� �ٱ��� �ִ� ���
    // �Ǵ� ���Կ� ������ �������� ���� ���
    CursorManager::Get().HoveredReleased(); //���� ���� 
    UIManager::Get().CloseWindow(UIWindowType::InventoryTooltip); //����
    return true;
}

bool EquipmentWindow::HandleMouseRight(Vec2 mousePos)
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
                inventory->AddItem(item->m_data.id, 1);

                return true;


            }

        }
    }

    return false;
}

void EquipmentWindow::ClearSlot(Wearable_part part)
{
    auto it = m_equippedItems.find(part);
    if (it != m_equippedItems.end() && it->second != nullptr)
    {
        m_equippedItems[part] = nullptr;
    }
        // ���� ����
}


void EquipmentWindow::EquipItem(Item* item) //Item �̸��� �ƴ϶�, Ÿ���� ���ؼ� �ؾ� �Կ� 
{
    Wearable* t_item = dynamic_cast<Wearable*>(item);
    if (item == nullptr) return;


    Wearable_part slotType = Wearable_part::UnKnown;

    if (t_item->Getpart() == Wearable_part::Helmet) slotType = Wearable_part::Helmet;
    else if (t_item->Getpart() == Wearable_part::Weapon) slotType = Wearable_part::Weapon;
    else if (t_item->Getpart() == Wearable_part::Cape) slotType = Wearable_part::Cape;
    else if (t_item->Getpart() == Wearable_part::Upper) slotType = Wearable_part::Upper;
    else if (t_item->Getpart() == Wearable_part::Under) slotType = Wearable_part::Under;
    else if (t_item->Getpart() == Wearable_part::Glove) slotType = Wearable_part::Glove;
    else if (t_item->Getpart() == Wearable_part::Neckless) slotType = Wearable_part::Neckless;
    else if (t_item->Getpart() == Wearable_part::Ring) slotType = Wearable_part::Ring;
    else if (t_item->Getpart() == Wearable_part::Shoes) slotType = Wearable_part::Shoes;

    /* if (item->m_data.name.find("Helmet") != std::string::npos) slotType = Wearable_part::Helmet;
     else if (item->m_data.name.find("Weapon") != std::string::npos) slotType = Wearable_part::Weapon;
     else if (item->m_data.name.find("Cape") != std::string::npos) slotType = Wearable_part::Cape;
     else if (item->m_data.name.find("Upper") != std::string::npos) slotType = Wearable_part::Upper;
     else if (item->m_data.name.find("Under") != std::string::npos) slotType = Wearable_part::Under;
     else if (item->m_data.name.find("Glove") != std::string::npos) slotType = Wearable_part::Glove;
     else if (item->m_data.name.find("Neckless") != std::string::npos) slotType = Wearable_part::Neckless;
     else if (item->m_data.name.find("EarRing") != std::string::npos) slotType = Wearable_part::Ring;
     else if (item->m_data.name.find("Shoes") != std::string::npos) slotType = Wearable_part::Shoes;*/

    if (slotType != Wearable_part::UnKnown)
    {
        // ������ ���� ���� �������� ������ �κ��丮�� ��ȯ
        Item* previousItem = m_equippedItems[slotType];
        if (previousItem)
        {
            if (auto* inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow)))
            {
                inventory->AddItem(previousItem->m_data.id, 1);
            }
        }

        // �� ������ ����
        m_equippedItems[slotType] = item;

        // ���� ������Ʈ
        // GameManager::Get().UpdatePlayerStats();
    }

    //if (!item) return;

    //Wearable* t_item = dynamic_cast<Wearable*>(item);
    //if (!t_item) return;

    //Wearable_part slotType = t_item->Getpart();

    //if (slotType != Wearable_part::UnKnown)
    //{
    //    // �� ������ ���� (���� �������� �̹� ó����)
    //    m_equippedItems[slotType] = item;
    //}
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

    if (!uiRenderer) return;

    for (const auto& [slotType, position] : m_slotPositions)
    {
        Vec2 screenPos = m_position + position; // â ��ġ�� ���� ���� ȭ�� ��ǥ
        Vec2 slotSize = m_slotSizes.at(slotType);
        //D2D1_RECT_F destRect = { screenPos.x, screenPos.y, screenPos.x + slotSize.x, screenPos.y + slotSize.y };

        // ������ ��� ��Ʈ���� slotType�� ���� �������� ���������� ����
        //std::string slotKey;
        switch (slotType)
        {
        case Wearable_part::Helmet:
            //slotKey = "icon_helmet_slot";
            m_slots[Wearable_part::Helmet]->GetTransform().SetPosition({ screenPos.x, screenPos.y });
            break;
        case Wearable_part::Weapon:
            //slotKey = "icon_weapon_slot";
            m_slots[Wearable_part::Weapon]->GetTransform().SetPosition({ screenPos.x, screenPos.y });
            break;
        case Wearable_part::Upper:
            //slotKey = "icon_armor_slot";
            m_slots[Wearable_part::Upper]->GetTransform().SetPosition({ screenPos.x, screenPos.y });
            break;
        case Wearable_part::Under:
            //slotKey = "icon_pants_slot";
            m_slots[Wearable_part::Under]->GetTransform().SetPosition({ screenPos.x, screenPos.y });
            break;
        case Wearable_part::Glove:
            //slotKey = "icon_glove_slot";
            m_slots[Wearable_part::Glove]->GetTransform().SetPosition({ screenPos.x, screenPos.y });
            break;
        case Wearable_part::Shoes:
            //slotKey = "icon_shoes_slot";
            m_slots[Wearable_part::Shoes]->GetTransform().SetPosition({ screenPos.x, screenPos.y });
            break;
        case Wearable_part::Neckless:
            //slotKey = "icon_necklace_slot";
            m_slots[Wearable_part::Neckless]->GetTransform().SetPosition({ screenPos.x, screenPos.y });
            break;
        case Wearable_part::Ring:
            //slotKey = "icon_earring_slot";
            m_slots[Wearable_part::Ring]->GetTransform().SetPosition({ screenPos.x, screenPos.y });
            break;
        case Wearable_part::Cape:
            //slotKey = "icon_cape_slot";
            m_slots[Wearable_part::Cape]->GetTransform().SetPosition({ screenPos.x, screenPos.y });
            break;
        default:
            //slotKey = "empty_slot"; // �⺻ ���� ��Ʈ��
            break;
        }

        //ID2D1Bitmap1* slotBitmap = uiRenderer->GetBitmap(slotKey).Get();
        //if (slotBitmap)
        //{
        //    // �ش��ϴ� Bitmap 
        //    D2DRenderer::Get().DrawBitmap(slotBitmap, destRect);
        //}

        // screenPos ����
        D2DRenderer::Get().DrawRectangle(screenPos.x, screenPos.y, screenPos.x + slotSize.x, screenPos.y + slotSize.y, D2D1::ColorF(D2D1::ColorF::Black));

        // screenPos ����
        //RenderSlotIcon(slotType, screenPos);
    }

    for (auto& slot : m_slots)
    {
        auto info = slot.second->GetRenderInfo()->GetRenderInfo();
        D2DRenderer::Get().DrawBitmap(info);
    }
}

void EquipmentWindow::RenderEquippedItems()
{
    // ����� �����۵� ������
    for (const auto& [slotType, item] : m_equippedItems)
    {
        if (item != nullptr)
        {
            Vec2 slotPos = m_slotPositions[slotType] + m_position;
            Vec2 slotSize = m_slotSizes[slotType];
            D2D1_RECT_F destRect = { slotPos.x, slotPos.y, slotPos.x + slotSize.x, slotPos.y + slotSize.y };

            ID2D1Bitmap1* itemBitmap = ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id)->atlas.Get();
            // GetTexture(item->GetImagePath()).Get(); // ���߿� ó��.

            if (itemBitmap)
            {
                D2D_RECT_F SR = ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id)->srcRect;
                SR.left = SR.left + 86.f;
                SR.top = SR.top + 86.f;
                SR.right = SR.right - 86.f;
                SR.bottom = SR.bottom - 86.f;
                //D2D1_RECT_F srcRect = { 0, 0, (float)itemBitmap->GetSize().width, (float)itemBitmap->GetSize().height };
                D2DRenderer::Get().DrawBitmap(itemBitmap, destRect, SR);
            }

            ////������ ��ȭ ��ġ ǥ�� (+1, +2 ��)
            //if (item->GetEnhanceLevel() > 0) //�̰͵� �߰� �ռ� 
            //{
            //    Vec2 enhanceTextPos = slotPos + Vec2(slotSize.x - 20, 5);
            //    std::string enhanceText = "+" + std::to_string(item->GetEnhanceLevel());
            //    D2DRenderer::Get().DrawMessage(std::wstring(enhanceText.begin(), enhanceText.end()).c_str(), enhanceTextPos.x, enhanceTextPos.y, 20, 15, D2D1::ColorF(D2D1::ColorF::Yellow));
            //}
        }
    }
}

void EquipmentWindow::RenderCloseButton()
{
    UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();

    float rightMargin = 65;
    Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 35 };
    Vec2 closeButtonSize = { 35, 35 };
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

        // ���Կ��� ������ϱ�.
        if (auto* statWindow = dynamic_cast<StatWindow*>(UIManager::Get().GetWindow(UIWindowType::StatsWindow)))
        {
            statWindow->UpdateTotalStats();
        }
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
    Vec2 windowPos = GetPosition();
    // ��� ������ Ȯ���Ͽ� ���콺 ��ġ�� �ش��ϴ� ���� ã��
    for (const auto& pair : m_slotPositions)
    {
        Wearable_part slotType = pair.first;
        Vec2 slotPos = pair.second;
        Vec2 slotSize = m_slotSizes.at(slotType);

        // â ��ġ + ���� ��� ��ġ�� ���� ���� ȭ����� ���� ��ǥ ���
        float slot_left = windowPos.x + slotPos.x;
        float slot_top = windowPos.y + slotPos.y;
        float slot_right = slot_left + slotSize.x;
        float slot_bottom = slot_top + slotSize.y;

        // ���콺 ��ġ�� ���� ���� ���� �ִ��� Ȯ��
        if (mousePos.x >= slot_left && mousePos.x <= slot_right &&
            mousePos.y >= slot_top && mousePos.y <= slot_bottom)
        {
            return slotType; // ��ġ�ϴ� ������ ã���� �ش� ���� Ÿ���� ��ȯ
        }
    }

    // ��ġ�ϴ� ������ ������ Unknown ��ȯ
    return Wearable_part::UnKnown;
}

bool EquipmentWindow::CanEquipItem(Item* item, Wearable_part slotType) const
{

    Wearable* t_item = dynamic_cast<Wearable*>(item);
    if (!item) return false;

    // �������� ���� ������ ���� Ÿ���� ��ġ�ϴ��� Ȯ��
    if (t_item->Getpart() == Wearable_part::Helmet) return true;
    else if (t_item->Getpart() == Wearable_part::Weapon) return true;
    else if (t_item->Getpart() == Wearable_part::Cape) return true;
    else if (t_item->Getpart() == Wearable_part::Upper)return true;
    else if (t_item->Getpart() == Wearable_part::Under) return true;
    else if (t_item->Getpart() == Wearable_part::Glove) return true;
    else if (t_item->Getpart() == Wearable_part::Neckless) return true;
    else if (t_item->Getpart() == Wearable_part::Ring) return true;
    else if (t_item->Getpart() == Wearable_part::Shoes) return true;

    return false;
}

void EquipmentWindow::UpdateSlotPositions()
{
    // ��� ���� ��ġ�� m_position�� �������� ����
    m_slotPositions[Wearable_part::Helmet] = { m_position.x + 222, m_position.y + 138 };
    m_slotPositions[Wearable_part::Weapon] = { m_position.x + 74, m_position.y + 214 };
    m_slotPositions[Wearable_part::Upper] = { m_position.x + 222, m_position.y + 290 };
    m_slotPositions[Wearable_part::Glove] = { m_position.x + 74, m_position.y + 365 };
    m_slotPositions[Wearable_part::Under] = { m_position.x + 222, m_position.y + 442 };
    m_slotPositions[Wearable_part::Shoes] = { m_position.x + 370, m_position.y + 214 };
    m_slotPositions[Wearable_part::Cape] = { m_position.x + 370, m_position.y + 365 };
    m_slotPositions[Wearable_part::Ring] = { m_position.x + 370, m_position.y + 517 };
    m_slotPositions[Wearable_part::Neckless] = { m_position.x + 222, m_position.y + 593 };

    std::cout << "���콺 ��ġ �޴� ��" << std::endl;
}


void EquipmentWindow::RenderSlotIcon(Wearable_part slotType, Vec2 position)
{
    // ���Ժ� ������ ������ (� ��� ������ ǥ��)
    std::string kEY;

    switch (slotType)
    {
    case Wearable_part::Helmet:
        kEY = "icon_helmet_slot";
        break;
    case Wearable_part::Weapon:
        kEY = "icon_weapon_slot";
        break;
    case Wearable_part::Upper:
        kEY = "icon_armor_slot";
        break;
    case Wearable_part::Under:
        kEY = "icon_pants_slot";
        break;
    case Wearable_part::Glove:
        kEY = "icon_glove_slot";
        break;
    case Wearable_part::Shoes:
        kEY = "icon_shoes_slot";
        break;
    case Wearable_part::Neckless:
        kEY = "icon_necklace_slot";
        break;
    case Wearable_part::Ring:
        kEY = "icon_earring_slot";
        break;
    case Wearable_part::Cape:
        kEY = "icon_cape_slot";
        break;
    }

    if (!kEY.empty())
    {

        D2D1_SIZE_F  size = GetComponent<UI_Renderer>()->GetBitmap(kEY).Get()->GetSize();

        D2D1_RECT_F rect = {
      0.0f,           // left
      0.0f,           // top
      size.width,     // right
      size.height     // bottom
        };



        float x = m_slotSizes.at(slotType).x;
        float y = m_slotSizes.at(slotType).y;

        D2D1_RECT_F desrect = { x, y, x + size.width, y + size.height };

        // D2DRenderer::Get().DrawImage(iconPath, position, m_slotSizes.at(slotType));
        D2DRenderer::Get().DrawBitmap(GetComponent<UI_Renderer>()->GetBitmap(kEY).Get(), desrect, rect, 1.0f);
    }
}

bool EquipmentWindow::IsItemTypeMatch(Wearable_part itemType, Wearable_part slotType)
{
    return itemType == slotType;
}

bool EquipmentWindow::IsItemTypeMatch(const std::string& itemId, Wearable_part slotType) const
{
    Item* itemData = m_itemDatabase->GetItemData(itemId);

    m_itemBank.get()->GiveItem(itemData->Momnet, *m_itemDatabase);

    Wearable* wearableItem = dynamic_cast<Wearable*>(itemData);

    if (itemData && wearableItem->Getpart() == slotType)
    {
        return true;
    }

    return false;
}