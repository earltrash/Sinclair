#pragma once
#include "pch.h"
#include "UIManager.h"
#include "Status.h" 
#include "Item.h"
#include "ResourceManager.h"
#include "UI_Renderer.h"

class UIManager;

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

    void Update();

    void Render();
  
    void RenderCursor();
    void LoadCursorBitmaps();

    // ���콺 ���� ������Ʈ
    void UpdateMouseState(bool isPressed);

    // �巡�� ����
    void StartItemDrag(std::string itemid, DragSource source);

    // �巡�� ����
    void EndItemDrag();

    // �巡�� ������ Ȯ��
    bool IsDragging() const { return m_isDragging; }

    // �巡�� ���� ������ ��������
    Item* GetDraggedItem() const { return m_draggedItem; }
    void SetDraggedItem(Item* item) { m_draggedItem = item; }
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