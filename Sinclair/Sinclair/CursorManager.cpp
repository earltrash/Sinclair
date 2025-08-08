#include "CursorManager.h"
#include "ResourceManager.h"


void CursorManager::Update()
{
    if (m_isDragging)
    {
        m_ghostImagePos = InputManager::Get().GetMousePosition();
        // �ణ�� ���������� ���콺 Ŀ���� ��ġ�� �ʰ�
        m_ghostImagePos.x += 5;
        m_ghostImagePos.y += 5;
    }
}

void CursorManager::Render()
{
    // �巡�� ���� ������ ������
    if (m_isDragging && m_draggedItem)
    {
        Vec2 mousePos = InputManager::Get().GetMousePosition();
        // ������ �̹��� ��������
        const auto& itemClip = ResourceManager::Get().Get_ItemBank().GetItemClip(m_draggedItem->m_data.id);
        // ��Ʋ�󽺿��� �ش������ ��������.
        ID2D1Bitmap1* itemBitmap = itemClip.atlas.Get();
        // �ҽ� ũ�� ������.
        D2D1_RECT_F srcRect = itemClip.srcRect;
        // ũ�� ��� 
        D2D1_RECT_F destRect = D2D1::RectF(
            m_ghostImagePos.x - 28, m_ghostImagePos.y - 28,
            m_ghostImagePos.x + 28, m_ghostImagePos.y + 28
        );
        // �׸���.
        D2DRenderer::Get().DrawBitmap(
            static_cast<ID2D1Bitmap1*>(itemBitmap),
            destRect,
            srcRect, // �ҽ� �簢���� ����Ͽ� ��Ʋ���� Ư�� �κи� �׸��°�.
            0.8f
        );
    }
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


void CursorManager::StartItemDrag_NS(std::string itemID, DragSource source)
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