#include "CursorManager.h"
#include "ResourceManager.h"
#include "EffectComponent.h"

void CursorManager::Update()
{
    if (m_isDragging)
    {
        m_ghostImagePos = InputManager::Get().GetMousePosition();
        // �ణ�� ���������� ���콺 Ŀ���� ��ġ�� �ʰ�
        //m_ghostImagePos.x += 5;
        //m_ghostImagePos.y += 5;
    }
    //�巡�� ���� ������ ������Ʈ
    if (m_isDragging && m_draggedItem)
    {
        m_draggedItem->Update();

        // ���â�� ���� ������
        auto part = m_draggedItem->m_data.wearablePart;
        //bool isPopped = UIManager::Get().IsWindowActive(UIWindowType::EquipmentWindow);
        //if (!isPopped) return;
        //auto pWindow = UIManager::Get().GetWindow(UIWindowType::EquipmentWindow);
        if (UIManager::Get().IsWindowActive(UIWindowType::EquipmentWindow))
        {
            EquipmentWindow* win =
                dynamic_cast<EquipmentWindow*>(UIManager::Get().GetWindow(UIWindowType::EquipmentWindow));

            if (win->CanEquipItem(m_draggedItem, part));
            win->SendEventToComponent("PLAY", part);
        }
        m_stopDraggingItem = m_draggedItem;
    }
    if (!m_isDragging && m_stopDraggingItem != nullptr)
    {
        auto part = m_stopDraggingItem->m_data.wearablePart;
        if (UIManager::Get().IsWindowActive(UIWindowType::EquipmentWindow))
        {
            EquipmentWindow* win =
                dynamic_cast<EquipmentWindow*>(UIManager::Get().GetWindow(UIWindowType::EquipmentWindow));

            if (win->CanEquipItem(m_stopDraggingItem, part));
            win->SendEventToComponent("STOP", part);
        }
    }
}

void CursorManager::Render()
{
    // �巡�� ���� ������ ������
    if (m_isDragging && m_draggedItem)
    {
        m_draggedItem->GetTransform().SetScale({ 1.f, 1.f });
        m_draggedItem->GetTransform().SetPosition({ m_ghostImagePos.x - 128.f * 1.f,  m_ghostImagePos.y - 128.f * 1.f });
        auto info = m_draggedItem->GetRenderInfo();
        D2DRenderer::Get().DrawBitmap(info->GetRenderInfo());
    }
    //if (m_isDragging && m_draggedItem)
    //{
    //    Vec2 mousePos = InputManager::Get().GetMousePosition();
    //    // ������ �̹��� ��������
    //    const auto& itemClip = ResourceManager::Get().Get_ItemBank().GetItemClip(m_draggedItem->m_data.id);
    //    // ��Ʋ�󽺿��� �ش������ ��������.
    //    ID2D1Bitmap1* itemBitmap = itemClip->atlas.Get();
    //    // �ҽ� ũ�� ������.
    //    D2D1_RECT_F srcRect = itemClip->srcRect;
    //    // ũ�� ��� 
    //    D2D1_RECT_F destRect = D2D1::RectF(
    //        m_ghostImagePos.x - 28, m_ghostImagePos.y - 28,
    //        m_ghostImagePos.x + 28, m_ghostImagePos.y + 28
    //    );
    //    // �׸���.
    //    D2DRenderer::Get().DrawBitmap(
    //        static_cast<ID2D1Bitmap1*>(itemBitmap),
    //        destRect,
    //        srcRect, // �ҽ� �簢���� ����Ͽ� ��Ʋ���� Ư�� �κи� �׸��°�.
    //        0.8f
    //    );
    //}
    RenderCursor();
}

void CursorManager::RenderCursor()
{
    // ���콺 ��ġ ��������
    Vec2 mousePos = InputManager::Get().GetMousePosition();
    // Ŀ�� ũ��.
    D2D1_RECT_F destRect = D2D1::RectF(mousePos.x, mousePos.y, mousePos.x + 32, mousePos.y + 32);

    ComPtr<ID2D1Bitmap1> cursorBitmap = nullptr;

    // ���� Ŀ�� Ÿ�Կ� ���� ����� ��Ʈ���� ����
    switch (m_currentCursor)
    {
    case CursorType::Normal:
        cursorBitmap = m_cursorNormal;
        break;
    case CursorType::Drag:
    case CursorType::Pressed:
        cursorBitmap = m_cursorDown;
        destRect = D2D1::RectF(mousePos.x, mousePos.y, mousePos.x + 18, mousePos.y + 23);
        break;
    default:
        cursorBitmap = m_cursorNormal;
        break;
    }

    D2DRenderer::Get().DrawBitmap(cursorBitmap.Get(), destRect);
}

void CursorManager::LoadCursorBitmaps()
{
    if (!m_cursorNormal)
    {
        m_cursorNormal = ResourceManager::Get().Get_UIBank().Get_Image("Ŀ��0");
        m_cursorDown = ResourceManager::Get().Get_UIBank().Get_Image("Ŀ��1");
        // ��Ʈ�� �ε� �� �����Ͱ� ��ȿ���� Ȯ��
        if (m_cursorNormal == nullptr)
        {
            std::cout << "m_cursorNormal is NULL after load." << std::endl;
        }
        else
        {
            std::cout << "m_cursorNormal is VALID." << std::endl;
        }
    }

    if (!m_cursorDown)
    {
        D2DRenderer::Get().CreateBitmapFromFile(L"UI\\cursordown.png", m_cursorDown.GetAddressOf());
        if (m_cursorDown == nullptr)
        {
            std::cout << "m_cursorDown is NULL after load." << std::endl;
        }
        else
        {
            std::cout << "m_cursorDown is VALID." << std::endl;
        }
    }
}

void CursorManager::UpdateMouseState(bool isPressed)
{
    m_isMousePressed = isPressed;

    SetCursor(isPressed ? CursorType::Pressed : CursorType::Normal);
}

void CursorManager::StartItemDrag(std::string itemID, DragSource source, InventorySlot* sourceSlot)
{
    m_isDragging = true;
    m_dragSource = source;
    m_sourceSlot = sourceSlot;
    SetCursor(CursorType::Drag);
}

void CursorManager::StartItemDrag_NS(std::string itemid, DragSource source)
{
    m_isDragging = true;
    m_dragSource = source;
    SetCursor(CursorType::Drag);
}

void CursorManager::EndItemDrag()
{
    m_isDragging = false;
    m_draggedItem = nullptr;
    m_dragSource = DragSource::None;
    m_sourceSlot = nullptr;
    SetCursor(CursorType::Normal);
}

void CursorManager::SetDraggedItem(Item* item)
{
    if (m_draggedItem == item)  return;
    m_draggedItem = item;
    m_draggedItem->ComponentClear();
    const auto& itemClip = ResourceManager::Get().Get_ItemBank().GetItemClip(m_draggedItem->m_data.id);
    // ��Ʋ�󽺿��� �ش������ ��������.
    ID2D1Bitmap1* itemBitmap = itemClip->atlas.Get();
    // �ҽ� ũ�� ������.
    D2D1_RECT_F srcRect = itemClip->srcRect;

    auto info = m_draggedItem->GetRenderInfo();
    info->SetBitmap(itemBitmap);
    info->SetSrcRect(srcRect);
    
    auto shadow = m_draggedItem->AddComponent<Shadow_Effect>(info, 3.f, 0.f, 0.f, 0.f, 1.f, itemBitmap);
    auto offset = m_draggedItem->AddComponent<Offset_Effect>(info, 5.f, 5.f, shadow->GetEffect());
    auto composite = m_draggedItem->AddComponent<Composite_Effect>(info, itemBitmap, offset->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
}

void CursorManager::HoveredReleased()
{
    { if (m_hoveredItem != nullptr) m_hoveredItem = nullptr; }
}
