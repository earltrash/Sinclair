#pragma once

#include "pch.h"

#include "SimpleMathHelper.h"
#include "InvenMem.h"

#include "UIWindow.h"
using Vec2 = MYHelper::Vector2F;


//�� ����� �ű�� 

constexpr float SLOT_PADDING = 4.0f;


class Inventory : public UIWindow
{
public:
    //ũ��� 1097 *  766 // Rect {0,0 1097,766}
    //�� ������Ʈ�� ��ü Rect, Object ������ ��� ������ ���� 
    Inventory();

    //Initalize
    void InitializeRegions();
    void InitializeSlots();

    bool AddItem(string itemId, int count = 1);

    void LoadUIBitmaps();
    void LoadItemDatabase(Need_Moment Moment);

    void PackItem_withoutItem();
    void PackItem();
    void SetDatabase(unique_ptr<ItemDatabase> database);

    // Scene �Ѿ �� 
    void UnlockRegion(int regionId);


    void Update() override;
    void FixedUpdate(float dt) override
    {
        for (const auto& [key, slot] : slots)
        {
            if (slot.itemBitmap.item == nullptr) continue;
            slot.itemBitmap.item->FixedUpdate(dt);
        }
    }
    void Render() override;

    bool HandleMouseHover(Vec2 mousePos) override;
    bool HandleMouseDown(Vec2 mousePos) override;
    bool HandleMouseUp(Vec2 mousePos) override;
    bool HandleDoubleClick(Vec2 mousePos) override;
    bool HandleMouseRight(Vec2 mousePos) override;

    bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) override;
    bool ConsumePendingPotion();
    virtual void SetActivate(bool active) { m_isActive = active; }
    UIWindowType GetType() override;

    InventorySlot* GetSlotAt(const Vec2& pos);
    void RenderTitleBar();
    void RenderCloseButton();
    void RenderSlot(const InventorySlot& slot);

    ItemDatabase& GetItemBase();

    void UpdateSlotPositions();

    // ���� �ʱ�ȭ
    void ClearAllSlots();
private:

    InventorySlot* m_pendingPotionSlot = nullptr;


    std::vector<Region> regions;
    std::map<std::pair<int, int>, InventorySlot> slots;  // [region][index] -> slot
    SlotBitmapController controller; //���� ��Ʈ�� �������ִ� ģ�� 

    //unique_ptr<ItemDatabase> itemDatabase; //������ ���̽�, ���࿡ 
    ItemDatabase m_itemDatabase;
    Rect m_bound;

    UIBitmap windowBackground;//Window Bitmap;
    UIBitmap tooltipBackground;//tooltip image;
    UIBitmap closeButton;
    UIBitmap TitleBar;


    Rect titleBarBounds; // Ÿ��Ʋ�� ����
    Rect closeButtonBounds; // �ݱ� ��ư ����

    Vec2 windowPosition{ 0,0 };
    bool isWindowDragging = false;
    Vec2 dragStartMousePos{ 0,0 };
    Vec2 dragStartWindowPos{ 0,0 };

    std::string currentHoveredItemName; //HoveredItemName
    std::string currentHoveredItemDescription; //�� ������ �����.
    Vec2 tooltipPosition{ 0,0 };
    DragState dragState;
    int currentHoveredItemCount;

    vector<Vec2>RegionOffset;

};