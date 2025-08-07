#pragma once
#include "pch.h"
#include "UIManager.h"
#include "Status.h" 
#include "Item.h"
#include "ResourceManager.h"
#include "UI_Renderer.h"
#include "InvenMem.h"

class UIManager;

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

    void Update();

    void Render();

    void RenderCursor();
    void LoadCursorBitmaps();

    // 마우스 상태 업데이트
    void UpdateMouseState(bool isPressed);

    // 드래그 시작
    void StartItemDrag(std::string itemid, DragSource source, InventorySlot* sourceSlot = nullptr);

    // 드래그 종료
    void EndItemDrag();

    // 드래그 중인지 확인
    bool IsDragging() const { return m_isDragging; }

    // 드래그 중인 아이템 가져오기
    Item* GetDraggedItem() const { return m_draggedItem; }
    void SetDraggedItem(Item* item) { m_draggedItem = item; }
    // 드래그 시작 소스 가져오기
    DragSource GetDragSource() const { return m_dragSource; }

    // 상태 확인
    CursorType GetCurrentCursor() const { return m_currentCursor; }

    // 커서 타입 설정
    void SetCursor(CursorType type) { m_currentCursor = type; }

    // 원본 슬롯 정보 가져오기
    InventorySlot* GetSourceSlot() const { return m_sourceSlot; }

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
    InventorySlot* m_sourceSlot = nullptr;

    Vec2 m_ghostImagePos;
    ComPtr<ID2D1Bitmap1> m_ghostImage = nullptr;

    // 임시
    ComPtr<ID2D1Bitmap1> m_cursorNormal = nullptr;
    ComPtr<ID2D1Bitmap1> m_cursorDown = nullptr;
};