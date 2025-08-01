#pragma once
#include "pch.h"
#include "UIManager.h"
#include "Status.h" // UIWindowType �����÷����ؼ�.



class CursorManager
{
public:
    // �̱���
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
            // �ణ�� ���������� ���콺 Ŀ���� ��ġ�� �ʰ�
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

    // �巡�� ����
    bool m_isDragging = false;
    //Item m_draggedItem;
    DragSource m_dragSource = DragSource::None;
    int m_sourceSlot = -1;
    Vec2 m_dragStartPos;
    Vec2 m_ghostImagePos;
};