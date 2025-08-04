#pragma once

#include "pch.h"

#include "SimpleMathHelper.h"
#include "InvenMem.h"

#include "UIWindow.h"
using Vec2 = MYHelper::Vector2F;


//�� ����� �ű�� 




class Inventory : public UIWindow
{
public:
    //ũ��� 1097 *  766 // Rect {0,0 1097,766}
    Inventory(); //�� ������Ʈ�� ��ü Rect, Object ������ ��� ������ ���� 

    //Initalize
    void InitializeRegions();
    void InitializeSlots();

    bool AddItem(string itemId, int count = 1);

    void LoadUIBitmaps();
    void LoadItemDatabase(Need_Moment Moment);

    void PackItem();
    void SetDatabase(unique_ptr<ItemDatabase> database); 

    // Scene �Ѿ �� 
    void UnlockRegion(int regionId);

    void Update() override;
    void Render() override;

     bool HandleMouseHover(Vec2 mousePos) override;
     bool HandleMouseDown(Vec2 mousePos) override;
     bool HandleMouseUp(Vec2 mousePos) override;
     bool HandleDoubleClick(Vec2 mousePos) override;

     virtual void SetActivate(bool active) { m_isActive = active; }
     virtual UIWindowType GetType() = 0;



     InventorySlot* GetSlotAt(const Vec2& pos);
    void RenderTitleBar();
    void RenderCloseButton();
    void RenderSlot(const InventorySlot& slot);

    void UpdateSlotPositions();


private:
    std::vector<Region> regions;
    std::map<std::pair<int, int>, InventorySlot> slots;  // [region][index] -> slot
    SlotBitmapController controller; //���� ��Ʈ�� �������ִ� ģ�� 

    //unique_ptr<ItemDatabase> itemDatabase; //������ ���̽�, ���࿡ 
    ItemDatabase m_itemDatabase;
    Rect m_bound;

    UIBitmap windowBackground;//Window Bitmap;
    UIBitmap tooltipBackground;//tooltip image;
    UIBitmap closeButton;


    Rect titleBarBounds; // Ÿ��Ʋ�� ����
    Rect closeButtonBounds; // �ݱ� ��ư ����

    Vec2 windowPosition{ 0,0 };
    bool isWindowDragging;
    Vec2 dragStartMousePos;
    Vec2 dragStartWindowPos;



    std::string currentHoveredItemName; //HoveredItemName
    std::string currentHoveredItemDescription; //�� ������ �����.
    Vec2 tooltipPosition;
    DragState dragState;
    int currentHoveredItemCount;
   
};

