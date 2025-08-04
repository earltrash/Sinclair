#pragma once

#include "pch.h"

#include "SimpleMathHelper.h"
#include "InvenMem.h"

using Vec2 = MYHelper::Vector2F;


//얘 어따가 옮길까 




class Inventory //: 
{
public:
    Inventory(Rect MWP) ; //이 오브젝트의 전체 Rect, Object 형식이 어떻게 될지ㅣ 몰라서 

    //Initalize
    void InitializeRegions();
    void InitializeSlots();

    bool AddItem(string itemId, int count = 1);

    void LoadUIBitmaps();
    void LoadItemDatabase(Need_Moment Moment);

    void PackItem();
    void SetDatabase(unique_ptr<ItemDatabase> database); 

    // Scene 넘어갈 때 
    void UnlockRegion(int regionId);
    

private:
    std::vector<Region> regions;
    std::map<std::pair<int, int>, InventorySlot> slots;  // [region][index] -> slot
    SlotBitmapController controller; //슬롯 비트맵 설정해주는 친구 

    //unique_ptr<ItemDatabase> itemDatabase; //데이터 베이스, 만약에 
    ItemDatabase m_itemDatabase;
    Rect m_bound;

    UIBitmap windowBackground;//Window Bitmap;
    UIBitmap tooltipBackground;//tooltip image;
    UIBitmap closeButton;


    Rect titleBarBounds; // 타이틀바 영역
    Rect closeButtonBounds; // 닫기 버튼 영역

    Vec2 windowPosition{ 0,0 };

    std::string currentHoveredItemName; //HoveredItemName
    std::string currentHoveredItemDescription; //현 아이템 설명글.

    int currentHoveredItemCount;
    Vec2 tooltipPosition;
};

