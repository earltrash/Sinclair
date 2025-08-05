#pragma once
#include "pch.h"
#include "UIManager.h"
#include "Status.h" // UIWindowType 가져올려고해서.
#include "Item.h"


class CursorManager
{
public:
    // 싱글톤
    static CursorManager& Get()
    {
        static CursorManager instance;
        return instance;
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

    void Render()
    {
        if (m_isDragging && m_draggedItem)
        {
            // 드래그 중인 아이템 이미지 렌더링 로직
            // Renderer::DrawImage(m_draggedItem->GetImage(), m_ghostImagePos, ...);
        }
    }

    void RenderCursor()
    {
        //Vec2 mousePos = InputManager::Get().GetMousePosition();
        //switch (m_currentCursor)
        //{
        //case CursorType::Normal:
        //    RenderNormalCursor(mousePos);
        //    break;
        //case CursorType::Move:
        //    RenderMoveCursor(mousePos);
        //    break;
        //case CursorType::Pointer:
        //    RenderPointerCursor(mousePos);
        //    break;
        //case CursorType::Drag:
        //    RenderDragCursor(mousePos);
        //    break;
        //}
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

    // 커서 타입 설정
    void SetCursor(CursorType type)
    {
        m_currentCursor = type;
        // 실제 커서 이미지 변경 로직 
        // type에 따라서 cursor 이미지 변경.
    }

    // 현재 커서 타입 가져오기
    CursorType GetCurrentCursor() const { return m_currentCursor; }

private:
		CursorManager() = default;
		~CursorManager() = default;

        CursorManager(const CursorManager&) = delete;
        CursorManager& operator=(const CursorManager&) = delete;

    // 드래그 관련
    bool m_isDragging = false;
    Item* m_draggedItem = nullptr;
    DragSource m_dragSource = DragSource::None;
    CursorType m_currentCursor = CursorType::Normal;
    Vec2 m_ghostImagePos;
};