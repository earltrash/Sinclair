#pragma once
#include "pch.h"
#include "UIManager.h"
#include "Status.h" 
#include "Item.h"
#include "ResourceManager.h"
#include "UI_Renderer.h"


class CursorManager : public UI_Renderer
{
public:
    // 싱글톤
    static CursorManager& Get()
    {
        static CursorManager instance;
        return instance;
    }

    void Initialize()
    {
        LoadCursorBitmaps();
    }

    void Update()
    {
        if (m_isDragging)
        {
            //m_ghostImagePos = InputManager::Get().GetMousePosition();
            // 약간의 오프셋으로 마우스 커서와 겹치지 않게
            m_ghostImagePos.x += 5;
            m_ghostImagePos.y += 5;
        }
    }

    //void Render()
    //{
    //    // 드래그 중인 아이템 렌더링
    //    if (m_isDragging && m_draggedItem)
    //    {
    //        if (m_draggedItem->GetBitmap())
    //        {
    //            D2D1_RECT_F destRect = D2D1::RectF(
    //                m_ghostImagePos.x - 16, m_ghostImagePos.y - 16,
    //                m_ghostImagePos.x + 16, m_ghostImagePos.y + 16
    //            );
    //            D2DRenderer::Get().DrawBitmap(m_draggedItem->GetBitmap(), destRect);
    //        }
    //    }
    //}

    void RenderCursor()
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
        case CursorType::Pressed:
            cursorBitmap = m_cursorDown;
            break;
        default:
            cursorBitmap = m_cursorNormal;
            break;
        }

        D2DRenderer::Get().DrawBitmap(cursorBitmap.Get(), destRect);
    }
    // 임시
    // 커서 비트맵 직접 로드
    void LoadCursorBitmaps()
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


    // 마우스 상태 업데이트
    void UpdateMouseState(bool isPressed)
    {
        m_isMousePressed = isPressed;

        SetCursor(isPressed ? CursorType::Pressed : CursorType::Normal);
    }

    // 드래그 시작
    void StartItemDrag(Item& item, DragSource source)
    {
        m_isDragging = true;
        m_draggedItem = &item;
        m_dragSource = source;
        SetCursor(CursorType::Drag);
    }

    // 드래그 종료
    void EndItemDrag()
    {
        m_isDragging = false;
        m_draggedItem = nullptr;
        m_dragSource = DragSource::None;
        SetCursor(CursorType::Normal);
    }

    // 드래그 중인지 확인
    bool IsDragging() const { return m_isDragging; }

    // 드래그 중인 아이템 가져오기
    Item* GetDraggedItem() const { return m_draggedItem; }

    // 드래그 시작 소스 가져오기
    DragSource GetDragSource() const { return m_dragSource; }

    // 상태 확인
    CursorType GetCurrentCursor() const { return m_currentCursor; }

    // 커서 타입 설정
    void SetCursor(CursorType type) { m_currentCursor = type; }

private:
    CursorManager() = default;
    ~CursorManager() = default;

    CursorManager(const CursorManager&) = delete;
    CursorManager& operator=(const CursorManager&) = delete;

    // 드래그 관련
    bool m_isDragging = false;
    bool m_isMousePressed = false;
    Item* m_draggedItem = nullptr;
    DragSource m_dragSource = DragSource::None;
    CursorType m_currentCursor = CursorType::Normal;
    Vec2 m_ghostImagePos;


    // 임시
    ComPtr<ID2D1Bitmap1> m_cursorNormal = nullptr;
    ComPtr<ID2D1Bitmap1> m_cursorDown= nullptr;
};