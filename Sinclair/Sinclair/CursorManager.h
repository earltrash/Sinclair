#pragma once
#include "pch.h"
#include "UIManager.h"
#include "Status.h" // UIWindowType 가져올려고해서.



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
            m_ghostImagePos.x += 10;
            m_ghostImagePos.y += 10;
        }
    }

    void Render()
    {
       
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
private:
		CursorManager() = default;
		~CursorManager() = default;

    // 드래그 관련
    bool m_isDragging = false;
    //Item m_draggedItem;
    DragSource m_dragSource = DragSource::None;
    int m_sourceSlot = -1;
    Vec2 m_dragStartPos;
    Vec2 m_ghostImagePos;
};