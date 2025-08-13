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

void EquipmentWindow::Update() // 이거는 Inven 때처럼 Update 말고 메시지 받을 때 하면 될 듯 
{
    for (auto& slot : m_slots)
    {
        slot.second->Update();
    }
}

void EquipmentWindow::Render()
{
    if (!m_isActive) return;

    // 렌더링 순서: 배경 → 타이틀바 → 슬롯들 → 장착된 아이템들 → 닫기 버튼
    RenderBackground();
    // RenderTitleBar();
    RenderSlots();
    RenderEquippedItems();
    RenderCloseButton();
}

bool EquipmentWindow::HandleMouseDown(Vec2 mousePos)
{
    if (!m_isActive) return false;

    // 2. 슬롯 영역 클릭 체크
    Wearable_part clickedSlot = GetSlotTypeAt(mousePos);
    if (clickedSlot != Wearable_part::UnKnown)
    {
        auto it = m_equippedItems.find(clickedSlot);
        if (it != m_equippedItems.end() && it->second != nullptr)
        {
            Item* equippedItem = it->second;
            CursorManager::Get().StartItemDrag(equippedItem->m_data.id, DragSource::Equipment);
            CursorManager::Get().SetDraggedItem(equippedItem);

            // 드래그 시작 시 아이템을 임시로 제거
            m_equippedItems[clickedSlot] = nullptr;
            return true;
        }
        return true;
    }
    // 창 내부 클릭 이벤트 처리 완료. 그래서 화면 최상단으로 올리기.
    if (IsInBounds(mousePos))
    {
        UIManager::Get().OpenWindow(m_windowType);
        return true;
    }

    // 영역 밖이면 체크안함.
    return false;
}

bool EquipmentWindow::HandleMouseUp(Vec2 mousePos)
{
    // 창이 비활성화 상태면 처리하지 않음
    if (!m_isActive) return false;

    //  현재 아이템을 드래그 중인지 확인
    if (CursorManager::Get().IsDragging())
    {
        // 드래그 중인 아이템과 소스 정보 가져오기
        Item* draggedItem = CursorManager::Get().GetDraggedItem();
        DragSource dragSource = CursorManager::Get().GetDragSource();

        // 슬롯 오브젝트의 blink effect 끄기
        //SendEventToComponent("STOP", draggedItem->m_data.wearablePart);

        // 마우스가 놓인 위치가 유효한 장비 슬롯인지 확인
        Wearable_part slotType = GetSlotTypeAt(mousePos);

        // 드래그 중인 아이템이 장착 가능한 아이템인지 확인 (dynamic_cast)
        Wearable* wearableItem = dynamic_cast<Wearable*>(draggedItem);

        // 유효한 슬롯에, 장착 가능한 아이템을 드롭했는지 조건 체크
        if (slotType != Wearable_part::UnKnown && wearableItem && IsItemTypeMatch(wearableItem->Getpart(), slotType))
        {
            //std::cout << "DEBUG: 장비 장착 조건 만족. 아이템을 장착합니다." << std::endl;
            // 장비 착용 성공
            
            Item* previousItem = GetEquippedItem(slotType); // 원래 슬롯에 있던 아이템
            m_equippedItems[slotType] = draggedItem;

            // 기존 아이템이 있었다면 인벤토리로 복구.
            if (previousItem)
            {
                auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
                inventoryWindow->AddItem(previousItem->m_data.id, 1);

                std::cout << "DEBUG: 기존 아이템이 있어 인벤토리로 복구시킴." << std::endl;
            }

            // 슬롯 전부 다시 가져와서 다시 계산해주기.
            if (auto* statWindow = dynamic_cast<StatWindow*>(UIManager::Get().GetWindow(UIWindowType::StatsWindow)))
            {
                statWindow->UpdateTotalStats();
            }

            // 드래그 종료.
            CursorManager::Get().EndItemDrag();
            return true;
        }
        // uimanager가 알아서 복구 처리.
        return false;
    }

    //  창 내부 클릭 시 최상단으로 올리기 (드래그 드롭 로직 이후에 처리)
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

        // 어떤 창 영역도 아니면 원래 위치로 복구
        if (source == DragSource::Equipment || source == DragSource::Inventory || source == DragSource::Enhancement)
        {
            // 인벤토리에서 온 아이템이므로 인벤토리로 복구
            auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
            if (inventoryWindow)
            {
                inventoryWindow->AddItem(draggedItem->m_data.id, 1);
                std::cout << "인벤토리 아이템을 인벤토리로 복구했습니다: " << draggedItem->m_data.name << std::endl;
            }
        }

    CursorManager::Get().EndItemDrag();
    return true;
}

bool EquipmentWindow::HandleDoubleClick(Vec2 mousePos)
{
    //// 슬롯 더블클릭 시 아이템 해제
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

    // 마우스가 장비창의 경계 안에 있는지 먼저 확인
    if (!IsInBounds(mousePos))
    {
        // 경계 밖에 있다면 툴팁을 비활성화하고 함수 종료
        CursorManager::Get().HoveredReleased();
        UIManager::Get().CloseWindow(UIWindowType::InventoryTooltip);
        return false;
    }

    if (m_isDragging)
    {
        m_position = mousePos - m_dragOffset;

        // 창 위치가 바뀌면 슬롯 위치들도 다시 계산
        UpdateSlotPositions();
    }


    // 슬롯에 마우스 오버 시 툴팁 표시
    Wearable_part hoveredSlot = GetSlotTypeAt(mousePos);

    if (hoveredSlot != Wearable_part::UnKnown) //장비가 진짜 있는 경우 
    {
        Item* item = GetEquippedItem(hoveredSlot);
        if (item)
        {
            // 툴팁 표시 위치를 마우스 옆으로 조정
            Vec2 tooltipPos = mousePos + Vec2(10, 10);
            CursorManager::Get().SetHoveredItem(item);
            UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, tooltipPos); //활성화 후, Item 값을 보내줘야 함.

            return true;
        }
    }

    // 마우스가 장비창 경계 안, 슬롯 바깥에 있는 경우
    // 또는 슬롯에 있지만 아이템이 없는 경우
    CursorManager::Get().HoveredReleased(); //추적 금지 
    UIManager::Get().CloseWindow(UIWindowType::InventoryTooltip); //해제
    return true;
}

bool EquipmentWindow::HandleMouseRight(Vec2 mousePos)
{
    // 슬롯 더블클릭 시 아이템 해제
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
        // 슬롯 비우기
}


void EquipmentWindow::EquipItem(Item* item) //Item 이름이 아니라, 타입을 비교해서 해야 함요 
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
        // 기존에 착용 중인 아이템이 있으면 인벤토리로 반환
        Item* previousItem = m_equippedItems[slotType];
        if (previousItem)
        {
            if (auto* inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow)))
            {
                inventory->AddItem(previousItem->m_data.id, 1);
            }
        }

        // 새 아이템 착용
        m_equippedItems[slotType] = item;

        // 스탯 업데이트
        // GameManager::Get().UpdatePlayerStats();
    }

    //if (!item) return;

    //Wearable* t_item = dynamic_cast<Wearable*>(item);
    //if (!t_item) return;

    //Wearable_part slotType = t_item->Getpart();

    //if (slotType != Wearable_part::UnKnown)
    //{
    //    // 새 아이템 착용 (기존 아이템은 이미 처리됨)
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
        Vec2 screenPos = m_position + position; // 창 위치를 더한 최종 화면 좌표
        Vec2 slotSize = m_slotSizes.at(slotType);
        //D2D1_RECT_F destRect = { screenPos.x, screenPos.y, screenPos.x + slotSize.x, screenPos.y + slotSize.y };

        // 슬롯의 배경 비트맵을 slotType에 따라 동적으로 가져오도록 수정
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
            //slotKey = "empty_slot"; // 기본 슬롯 비트맵
            break;
        }

        //ID2D1Bitmap1* slotBitmap = uiRenderer->GetBitmap(slotKey).Get();
        //if (slotBitmap)
        //{
        //    // 해당하는 Bitmap 
        //    D2DRenderer::Get().DrawBitmap(slotBitmap, destRect);
        //}

        // screenPos 기준
        D2DRenderer::Get().DrawRectangle(screenPos.x, screenPos.y, screenPos.x + slotSize.x, screenPos.y + slotSize.y, D2D1::ColorF(D2D1::ColorF::Black));

        // screenPos 기준
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
    // 착용된 아이템들 렌더링
    for (const auto& [slotType, item] : m_equippedItems)
    {
        if (item != nullptr)
        {
            Vec2 slotPos = m_slotPositions[slotType] + m_position;
            Vec2 slotSize = m_slotSizes[slotType];
            D2D1_RECT_F destRect = { slotPos.x, slotPos.y, slotPos.x + slotSize.x, slotPos.y + slotSize.y };

            ID2D1Bitmap1* itemBitmap = ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id)->atlas.Get();
            // GetTexture(item->GetImagePath()).Get(); // 나중에 처리.

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

            ////아이템 강화 수치 표시 (+1, +2 등)
            //if (item->GetEnhanceLevel() > 0) //이것도 추가 합세 
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
        m_equippedItems[slotType] = nullptr; // 슬롯 비우기

        // 슬롯에서 비웠으니까.
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
    // 모든 슬롯을 확인하여 마우스 위치에 해당하는 슬롯 찾기
    for (const auto& pair : m_slotPositions)
    {
        Wearable_part slotType = pair.first;
        Vec2 slotPos = pair.second;
        Vec2 slotSize = m_slotSizes.at(slotType);

        // 창 위치 + 슬롯 상대 위치를 더해 실제 화면상의 절대 좌표 계산
        float slot_left = windowPos.x + slotPos.x;
        float slot_top = windowPos.y + slotPos.y;
        float slot_right = slot_left + slotSize.x;
        float slot_bottom = slot_top + slotSize.y;

        // 마우스 위치가 슬롯 영역 내에 있는지 확인
        if (mousePos.x >= slot_left && mousePos.x <= slot_right &&
            mousePos.y >= slot_top && mousePos.y <= slot_bottom)
        {
            return slotType; // 일치하는 슬롯을 찾으면 해당 슬롯 타입을 반환
        }
    }

    // 일치하는 슬롯이 없으면 Unknown 반환
    return Wearable_part::UnKnown;
}

bool EquipmentWindow::CanEquipItem(Item* item, Wearable_part slotType) const
{

    Wearable* t_item = dynamic_cast<Wearable*>(item);
    if (!item) return false;

    // 아이템의 착용 부위와 슬롯 타입이 일치하는지 확인
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
    // 장비 슬롯 위치를 m_position을 기준으로 재계산
    m_slotPositions[Wearable_part::Helmet] = { m_position.x + 222, m_position.y + 138 };
    m_slotPositions[Wearable_part::Weapon] = { m_position.x + 74, m_position.y + 214 };
    m_slotPositions[Wearable_part::Upper] = { m_position.x + 222, m_position.y + 290 };
    m_slotPositions[Wearable_part::Glove] = { m_position.x + 74, m_position.y + 365 };
    m_slotPositions[Wearable_part::Under] = { m_position.x + 222, m_position.y + 442 };
    m_slotPositions[Wearable_part::Shoes] = { m_position.x + 370, m_position.y + 214 };
    m_slotPositions[Wearable_part::Cape] = { m_position.x + 370, m_position.y + 365 };
    m_slotPositions[Wearable_part::Ring] = { m_position.x + 370, m_position.y + 517 };
    m_slotPositions[Wearable_part::Neckless] = { m_position.x + 222, m_position.y + 593 };

    std::cout << "마우스 위치 받는 중" << std::endl;
}


void EquipmentWindow::RenderSlotIcon(Wearable_part slotType, Vec2 position)
{
    // 슬롯별 아이콘 렌더링 (어떤 장비가 들어가는지 표시)
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