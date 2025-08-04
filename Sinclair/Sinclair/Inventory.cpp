#include "InvenTory.h"
//: UI_Object(MWP) //생성자로 영역은 일단 설정함 (Inven 자기 영역 말임)
Inventory::Inventory(Rect MWP) :m_bound(MWP)
{
    windowPosition = { m_bound.x,m_bound.y };

    InitializeRegions();
    LoadUIBitmaps(); //멤버로 갖고 있는 닫기랑 윗 부분 (이건 Inven이랑 같이할 가능성도 있음) 
    InitializeSlots();

    LoadItemDatabase(Need_Moment::Gen_2);
    PackItem();


    //database 에 값 넣어주는 거랑,Item으로 연결하는 거는 일단 나중에, 그냥 별개로 만들고 Inven에 넣어주는 식으로 해야 할 듯 
    // 
    //add 아이템은 Scene 넘겨지고 나서 해야할 듯 // 생성자체는 Factory에서 이루어 지니깐. 
    //수치값만 일단 설정해 놓고, Bitmap이랑 이런 것들은 ResourceManager에서 넘겨 받아야 하니깐. 이건 Scene 들어가고 나서 하도록 함.
}


void Inventory::InitializeRegions()
{
    regions.resize(3); // 3개의 영역 (4x8, 4x8, 4x8)

    //타이틀 바 영역
    titleBarBounds = Rect(m_bound.x, m_bound.y, m_bound.width, 30.0f);

    //닫기 버튼 영역
    float closeButtonSize = 22.0f;
    closeButtonBounds = Rect(
        windowPosition.x + m_bound.width - closeButtonSize - 5,
        windowPosition.y + 5,
        closeButtonSize,
        closeButtonSize);

    //Ivven 위치 
    windowBackground.position = windowPosition;
    windowBackground.size = Vec2(m_bound.width, m_bound.height);
    windowBackground.srcRect = D2D1::RectF(
        0.0f, 0.0f,
        static_cast<float>(m_bound.width),
        static_cast<float>(m_bound.height)
    );

    //닫기 버튼 랜더 위치 
    closeButton.position = Vec2(closeButtonBounds.x, closeButtonBounds.y);
    closeButton.size = Vec2(closeButtonBounds.width, closeButtonBounds.height);
    closeButton.srcRect = D2D1::RectF(0, 0, 27, 27); // 닫기 버튼 이미지 크기

    float slotSize = 48.0f;
    float padding = 4.0f;
    float totalSlotDimension = slotSize + padding;

    // windowBounds의 내부 영역을 기준으로 배치
    float currentRegionX = m_bound.x + 23.0f; // 윈도우 좌측 여백

    // Region 0: 기본 해금 영역 (4x8)
    regions[0].id = 0;
    regions[0].isEnabled = true;
    regions[0].gridSize = Vec2{ 4, 8 };
    regions[0].bounds = Rect(
        currentRegionX,
        m_bound.y + 50.0f, // 윈도우 상단 여백 + 타이틀바 공간
        regions[0].gridSize.x * totalSlotDimension,
        regions[0].gridSize.y * totalSlotDimension
    );
    currentRegionX += (regions[0].bounds.width) + 23.0f; // 다음 영역 시작 X

    // Region 1: 중간 잠금 영역 (4x8)
    regions[1].id = 1;
    regions[1].isEnabled = false; // 잠금
    regions[1].gridSize = Vec2{ 4, 8 };
    regions[1].bounds = Rect(
        currentRegionX,
        m_bound.y + 50.0f,
        regions[1].gridSize.x * totalSlotDimension,
        regions[1].gridSize.y * totalSlotDimension
    );
    currentRegionX += regions[1].bounds.width + 15.0f;

    // Region 2: 오른쪽 잠금 영역 (4x8)
    regions[2].id = 2;
    regions[2].isEnabled = false; // 잠금
    regions[2].gridSize = Vec2{ 4, 8 };
    regions[2].bounds = Rect(
        currentRegionX,
        m_bound.y + 50.0f,
        regions[2].gridSize.x * totalSlotDimension,
        regions[2].gridSize.y * totalSlotDimension
    );
}

void Inventory::InitializeSlots()
{
    float slotSize = 48.0f;
    float padding = 4.0f;
    float totalSlotDimension = slotSize + padding;

    for (int regionId = 0; regionId < regions.size(); ++regionId)
    {
        const Region& region = regions[regionId];

        for (int y = 0; y < region.gridSize.y; ++y)
        {
            for (int x = 0; x < region.gridSize.x; ++x)
            {
                int slotIndex = y * region.gridSize.x + x;
                std::pair<int, int> key = { regionId, slotIndex };

                InventorySlot& slot = slots[key];

                // 슬롯 위치는 해당 Region의 bounds를 기준으로 계산
                float slotX = region.bounds.x + x * totalSlotDimension;
                float slotY = region.bounds.y + y * totalSlotDimension;

                slot.SetBounds(Rect(slotX, slotY, slotSize, slotSize));
                slot.isEnabled = region.isEnabled; // 영역의 활성화 상태에 따라 슬롯 활성화 설정
                slot.UpdateBackgroundBitmap(&controller); // 초기 배경 설정
            }
        }
    }
}

bool Inventory::AddItem(string itemId, int count)
{
    const Item* itemData = m_itemDatabase.GetItemData(itemId);
    if (!itemData) return false;

    // 빈 슬롯 찾기
    for (auto& [key, slot] : slots)
    {
        if (!slot.isEnabled) continue; // 비활성화된 슬롯에는 추가 불가

        // 스택 가능한 아이템이라면 기존 슬롯에 합치기 시도 (예시)
        if (itemData->IsStackable() && slot.item.id == itemId && (slot.item.count + count) <= itemData->maxCount) //maxCount는 10으로 하긴 함. 
        {
            slot.item.count += count;
            slot.UpdateItemBitmap(&controller, &m_itemDatabase); // 개수 변경은 비트맵 변경이 없지만, 혹시 모를 경우 호출
            return true;
        }

        if (slot.IsEmpty()) {
            slot.SetItem(itemId, count);
            slot.UpdateItemBitmap(&controller, &m_itemDatabase);
            return true;
        }
    }
    return false;
}

void Inventory::UnlockRegion(int regionId)
{
    if (regionId >= 0 && regionId < regions.size())
    {
        regions[regionId].isEnabled = true;
        for (int y = 0; y < regions[regionId].gridSize.y; ++y)
        {
            for (int x = 0; x < regions[regionId].gridSize.x; ++x)
            {
                int slotIndex = y * regions[regionId].gridSize.x + x;
                std::pair<int, int> key = { regionId, slotIndex };
                if (slots.count(key))
                {
                    slots[key].isEnabled = true;
                    slots[key].UpdateBackgroundBitmap(&controller);
                }
            }
        }
    }
}

void Inventory::PackItem() //현재 database에 있는 모든 Item을 Slot에 넣어줌 
{

    for (const auto& [Id, Item] : m_itemDatabase.GetMap())
    {
        AddItem(Item->m_data.id, 1);
    }

}

void Inventory::SetDatabase(unique_ptr<ItemDatabase> database) //외부에서 만들고 넣어버릴래/
{
    //LoadItemDatabase 버전을 일단 쓰기로 함.

    //UI_Manager에서 어떻게 Init 할지는 모르겠다만, Scene Index 받고, Database에 넣는 거는 
    //GiveItem을 통해서 받으면 되가지고 상관 없긴 해 일단 버전 1로 두자고 
  //  this->itemDatabase = std::move(database);
}

void Inventory::LoadUIBitmaps() 
{
    
    // +Inven이 가져야 하는 bitmap 정보를 받아오기

    






    controller.bitmaps.emplace("slot_disabled",
        ResourceManager::Get().Get_UIBank().Get_Image("Slotdisabled"));
    controller.bitmaps.emplace("slot_hover",
        ResourceManager::Get().Get_UIBank().Get_Image("slot_hover"));
    controller.bitmaps.emplace("slot_selected",
        ResourceManager::Get().Get_UIBank().Get_Image("slot_selected"));
    controller.bitmaps.emplace("slot_normal",
        ResourceManager::Get().Get_UIBank().Get_Image("slot_normal"));

}

void Inventory::LoadItemDatabase(Need_Moment Moment)
{
    ResourceManager::Get().Get_ItemBank().GiveItem(Moment, m_itemDatabase);
}

