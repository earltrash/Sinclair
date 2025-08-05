#pragma once
#include "pch.h"
#include "UIManager.h"
#include "Status.h" // UIWindowType �����÷����ؼ�.
#include "Item.h"


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
            m_ghostImagePos.x += 5;
            m_ghostImagePos.y += 5;
        }
    }

    void Render()
    {
        if (m_isDragging && m_draggedItem)
        {
            // �巡�� ���� ������ �̹��� ������ ����
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

    // �巡�� ����
    void StartItemDrag(Item& item, DragSource source)
    {
        m_isDragging = true;
        m_draggedItem = &item;
        m_dragSource = source;
        SetCursor(CursorType::Drag);
    }

    // �巡�� ����
    void EndItemDrag()
    {
        m_isDragging = false;
        m_draggedItem = nullptr;
        m_dragSource = DragSource::None;
        SetCursor(CursorType::Normal);
    }

    // �巡�� ������ Ȯ��
    bool IsDragging() const { return m_isDragging; }

    // �巡�� ���� ������ ��������
    Item* GetDraggedItem() const { return m_draggedItem; }

    // �巡�� ���� �ҽ� ��������
    DragSource GetDragSource() const { return m_dragSource; }

    // Ŀ�� Ÿ�� ����
    void SetCursor(CursorType type)
    {
        m_currentCursor = type;
        // ���� Ŀ�� �̹��� ���� ���� 
        // type�� ���� cursor �̹��� ����.
    }

    // ���� Ŀ�� Ÿ�� ��������
    CursorType GetCurrentCursor() const { return m_currentCursor; }

private:
		CursorManager() = default;
		~CursorManager() = default;

        CursorManager(const CursorManager&) = delete;
        CursorManager& operator=(const CursorManager&) = delete;

    // �巡�� ����
    bool m_isDragging = false;
    Item* m_draggedItem = nullptr;
    DragSource m_dragSource = DragSource::None;
    CursorType m_currentCursor = CursorType::Normal;
    Vec2 m_ghostImagePos;
};