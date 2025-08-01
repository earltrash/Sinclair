#include "InvenTory.h"

Inventory::Inventory(Rect MWP) //: UI_Object(MWP) //생성자로 영역은 일단 설정함 (Inven 자기 영역 말임)
{
    InitializeRegions();
    LoadUIBitmaps(); //SlotBitmapController, 멤버로 갖고 있는 닫기랑 윗 부분 (이건 Inven이랑 같이할 가능성도 있음) 
    InitializeSlots();

    //database 에 값 넣어주는 거랑,Item으로 연결하는 거는 일단 나중에, 그냥 별개로 만들고 Inven에 넣어주는 식으로 해야 할 듯 
    // 
    //add 아이템은 Scene 넘겨지고 나서 해야할 듯 // 생성자체는 Factory에서 이루어 지니깐. 
    //수치값만 일단 설정해 놓고, Bitmap이랑 이런 것들은 ResourceManager에서 넘겨 받아야 하니깐. 이건 Scene 들어가고 나서 하도록 함.
}


void Inventory::InitializeRegions()
{
    //regions.resize(3); // 3개의 영역 (4x8, 4x8, 4x8)

    //float slotSize = 48.0f;
    //float padding = 4.0f;
    //float totalSlotDimension = slotSize + padding;

    //// windowBounds의 내부 영역을 기준으로 배치
    //float currentRegionX = m_bound.x + 23.0f; // 윈도우 좌측 여백

    //// Region 0: 기본 해금 영역 (4x8)
    //regions[0].id = 0;
    //regions[0].isEnabled = true;
    //regions[0].gridSize = Vec2{ 4, 8 };
    //regions[0].bounds = Rect(
    //    currentRegionX,
    //    m_bound.y + 50.0f, // 윈도우 상단 여백 + 타이틀바 공간
    //    regions[0].gridSize.x * totalSlotDimension,
    //    regions[0].gridSize.y * totalSlotDimension
    //);
    //currentRegionX += (regions[0].bounds.width) + 23.0f; // 다음 영역 시작 X

    //// Region 1: 중간 잠금 영역 (4x8)
    //regions[1].id = 1;
    //regions[1].isEnabled = false; // 잠금
    //regions[1].gridSize = Vec2{ 4, 8 };
    //regions[1].bounds = Rect(
    //    currentRegionX,
    //    m_bound.y + 50.0f,
    //    regions[1].gridSize.x * totalSlotDimension,
    //    regions[1].gridSize.y * totalSlotDimension
    //);
    //currentRegionX += regions[1].bounds.width + 15.0f;

    //// Region 2: 오른쪽 잠금 영역 (4x8)
    //regions[2].id = 2;
    //regions[2].isEnabled = false; // 잠금
    //regions[2].gridSize = Vec2{ 4, 8 };
    //regions[2].bounds = Rect(
    //    currentRegionX,
    //    m_bound.y + 50.0f,
    //    regions[2].gridSize.x * totalSlotDimension,
    //    regions[2].gridSize.y * totalSlotDimension
    //);
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
                //slot.UpdateBackgroundBitmap(&controller); // 초기 배경 설정
            }
        }
    }
}

bool Inventory::AddItem(string itemId, int count)
{
    const Item* itemData = itemDatabase->GetItemData(itemId);
    if (!itemData) return false;

    // 빈 슬롯 찾기
    for (auto& [key, slot] : slots)
    {
        if (!slot.isEnabled) continue; // 비활성화된 슬롯에는 추가 불가

        // 스택 가능한 아이템이라면 기존 슬롯에 합치기 시도 (예시)
        if (itemData->IsStackable() && slot.item.itemNM == itemId && (slot.item.count + count) <= itemData->maxCount)
        {
            slot.item.count += count;
            slot.UpdateItemBitmap(&controller, itemDatabase.get()); // 개수 변경은 비트맵 변경이 없지만, 혹시 모를 경우 호출
            return true;
        }

        if (slot.IsEmpty()) {
            slot.SetItem(itemId, count);
            slot.UpdateItemBitmap(&controller, itemDatabase.get());
            return true;
        }
    }
    return false;
}

void Inventory::UnlockRegion(int regionId)
{
}

void Inventory::SetDatabase(unique_ptr<ItemDatabase> database) //외부에서 만들고 넣어버릴래/
{
    this->itemDatabase = std::move(database);
}



void Inventory::LoadUIBitmaps() //이건 팩토리한테서 받거나 아니면 외부에서 받는거로 하자고.
{
    //slotbitmapcontroller가 가져야 하는 bitmap load하기. 
    // +Inven이 가져야 하는 bitmap 정보를 받아오기

    //이거는 factorY에서 ui를 위한 bitmap 정보를 따로 빼서 거기서 가져오는 거로 하자. 
    //아님 Resourcemanager ㅇㅇ 






}

