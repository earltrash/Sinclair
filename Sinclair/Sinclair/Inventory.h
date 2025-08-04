#pragma once

#include "pch.h"

#include "SimpleMathHelper.h"
#include "InvenMem.h"

using Vec2 = MYHelper::Vector2F;


//�� ����� �ű�� 




class Inventory //: 
{
public:
    Inventory(Rect MWP) ; //�� ������Ʈ�� ��ü Rect, Object ������ ��� ������ ���� 

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

    std::string currentHoveredItemName; //HoveredItemName
    std::string currentHoveredItemDescription; //�� ������ �����.

    int currentHoveredItemCount;
    Vec2 tooltipPosition;
};

