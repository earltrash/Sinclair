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
    // �̱���
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
            // �ణ�� ���������� ���콺 Ŀ���� ��ġ�� �ʰ�
            m_ghostImagePos.x += 5;
            m_ghostImagePos.y += 5;
        }
    }

    //void Render()
    //{
    //    // �巡�� ���� ������ ������
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
        case CursorType::Pressed:
            cursorBitmap = m_cursorDown;
            break;
        default:
            cursorBitmap = m_cursorNormal;
            break;
        }

        D2DRenderer::Get().DrawBitmap(cursorBitmap.Get(), destRect);
    }
    // �ӽ�
    // Ŀ�� ��Ʈ�� ���� �ε�
    void LoadCursorBitmaps()
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


    // ���콺 ���� ������Ʈ
    void UpdateMouseState(bool isPressed)
    {
        m_isMousePressed = isPressed;

        SetCursor(isPressed ? CursorType::Pressed : CursorType::Normal);
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

    // ���� Ȯ��
    CursorType GetCurrentCursor() const { return m_currentCursor; }

    // Ŀ�� Ÿ�� ����
    void SetCursor(CursorType type) { m_currentCursor = type; }

private:
    CursorManager() = default;
    ~CursorManager() = default;

    CursorManager(const CursorManager&) = delete;
    CursorManager& operator=(const CursorManager&) = delete;

    // �巡�� ����
    bool m_isDragging = false;
    bool m_isMousePressed = false;
    Item* m_draggedItem = nullptr;
    DragSource m_dragSource = DragSource::None;
    CursorType m_currentCursor = CursorType::Normal;
    Vec2 m_ghostImagePos;


    // �ӽ�
    ComPtr<ID2D1Bitmap1> m_cursorNormal = nullptr;
    ComPtr<ID2D1Bitmap1> m_cursorDown= nullptr;
};