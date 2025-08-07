#include "CursorManager.h"
#include "ResourceManager.h"


void CursorManager::Update()
{
    if (m_isDragging)
    {
        m_ghostImagePos = InputManager::Get().GetMousePosition();
        // 약간의 오프셋으로 마우스 커서와 겹치지 않게
        m_ghostImagePos.x += 5;
        m_ghostImagePos.y += 5;
    }
}

void CursorManager::Render()
{
      // 드래그 중인 아이템 렌더링
    if (m_isDragging && m_draggedItem)
    {
        // 아이템 이미지 가져오고
        ID2D1Bitmap1* itemBitmap = ResourceManager::Get().Get_ItemBank().GetItemClip(m_draggedItem->m_data.id).atlas.Get();

        // 크기 계산
        D2D1_RECT_F destRect = D2D1::RectF(
            m_ghostImagePos.x - 16, m_ghostImagePos.y - 16,
            m_ghostImagePos.x + 16, m_ghostImagePos.y + 16
        );
        // 그리기.
        D2DRenderer::Get().DrawBitmap(itemBitmap, destRect);
    }
    RenderCursor();
  }

void CursorManager::RenderCursor()
{
    // 마우스 위치 가져오기
    Vec2 mousePos = InputManager::Get().GetMousePosition();
    // 커서 크기.
    D2D1_RECT_F destRect = D2D1::RectF(mousePos.x, mousePos.y, mousePos.x + 32, mousePos.y + 32);

    ComPtr<ID2D1Bitmap1> cursorBitmap = nullptr;

    // 현재 커서 타입에 따라 사용할 비트맵을 선택
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
        m_cursorNormal = ResourceManager::Get().Get_UIBank().Get_Image("커서0");
        m_cursorDown = ResourceManager::Get().Get_UIBank().Get_Image("커서1");
        // 비트맵 로드 후 포인터가 유효한지 확인
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

void CursorManager::StartItemDrag(std::string itemID, DragSource source)
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
    SetCursor(CursorType::Normal);
}

