#include "InvenTory.h"
//: UI_Object(MWP) //�����ڷ� ������ �ϴ� ������ (Inven �ڱ� ���� ����)
Inventory::Inventory(Rect MWP) :m_bound(MWP)
{
    windowPosition = { m_bound.x,m_bound.y };

    InitializeRegions();
    LoadUIBitmaps(); //����� ���� �ִ� �ݱ�� �� �κ� (�̰� Inven�̶� ������ ���ɼ��� ����) 
    InitializeSlots();

    LoadItemDatabase(Need_Moment::Gen_2);
    PackItem();


    //database �� �� �־��ִ� �Ŷ�,Item���� �����ϴ� �Ŵ� �ϴ� ���߿�, �׳� ������ ����� Inven�� �־��ִ� ������ �ؾ� �� �� 
    // 
    //add �������� Scene �Ѱ����� ���� �ؾ��� �� // ������ü�� Factory���� �̷�� ���ϱ�. 
    //��ġ���� �ϴ� ������ ����, Bitmap�̶� �̷� �͵��� ResourceManager���� �Ѱ� �޾ƾ� �ϴϱ�. �̰� Scene ���� ���� �ϵ��� ��.
}


void Inventory::InitializeRegions()
{
    regions.resize(3); // 3���� ���� (4x8, 4x8, 4x8)

    //Ÿ��Ʋ �� ����
    titleBarBounds = Rect(m_bound.x, m_bound.y, m_bound.width, 30.0f);

    //�ݱ� ��ư ����
    float closeButtonSize = 22.0f;
    closeButtonBounds = Rect(
        windowPosition.x + m_bound.width - closeButtonSize - 5,
        windowPosition.y + 5,
        closeButtonSize,
        closeButtonSize);

    //Ivven ��ġ 
    windowBackground.position = windowPosition;
    windowBackground.size = Vec2(m_bound.width, m_bound.height);
    windowBackground.srcRect = D2D1::RectF(
        0.0f, 0.0f,
        static_cast<float>(m_bound.width),
        static_cast<float>(m_bound.height)
    );

    //�ݱ� ��ư ���� ��ġ 
    closeButton.position = Vec2(closeButtonBounds.x, closeButtonBounds.y);
    closeButton.size = Vec2(closeButtonBounds.width, closeButtonBounds.height);
    closeButton.srcRect = D2D1::RectF(0, 0, 27, 27); // �ݱ� ��ư �̹��� ũ��

    float slotSize = 48.0f;
    float padding = 4.0f;
    float totalSlotDimension = slotSize + padding;

    // windowBounds�� ���� ������ �������� ��ġ
    float currentRegionX = m_bound.x + 23.0f; // ������ ���� ����

    // Region 0: �⺻ �ر� ���� (4x8)
    regions[0].id = 0;
    regions[0].isEnabled = true;
    regions[0].gridSize = Vec2{ 4, 8 };
    regions[0].bounds = Rect(
        currentRegionX,
        m_bound.y + 50.0f, // ������ ��� ���� + Ÿ��Ʋ�� ����
        regions[0].gridSize.x * totalSlotDimension,
        regions[0].gridSize.y * totalSlotDimension
    );
    currentRegionX += (regions[0].bounds.width) + 23.0f; // ���� ���� ���� X

    // Region 1: �߰� ��� ���� (4x8)
    regions[1].id = 1;
    regions[1].isEnabled = false; // ���
    regions[1].gridSize = Vec2{ 4, 8 };
    regions[1].bounds = Rect(
        currentRegionX,
        m_bound.y + 50.0f,
        regions[1].gridSize.x * totalSlotDimension,
        regions[1].gridSize.y * totalSlotDimension
    );
    currentRegionX += regions[1].bounds.width + 15.0f;

    // Region 2: ������ ��� ���� (4x8)
    regions[2].id = 2;
    regions[2].isEnabled = false; // ���
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

                // ���� ��ġ�� �ش� Region�� bounds�� �������� ���
                float slotX = region.bounds.x + x * totalSlotDimension;
                float slotY = region.bounds.y + y * totalSlotDimension;

                slot.SetBounds(Rect(slotX, slotY, slotSize, slotSize));
                slot.isEnabled = region.isEnabled; // ������ Ȱ��ȭ ���¿� ���� ���� Ȱ��ȭ ����
                slot.UpdateBackgroundBitmap(&controller); // �ʱ� ��� ����
            }
        }
    }
}

bool Inventory::AddItem(string itemId, int count)
{
    const Item* itemData = m_itemDatabase.GetItemData(itemId);
    if (!itemData) return false;

    // �� ���� ã��
    for (auto& [key, slot] : slots)
    {
        if (!slot.isEnabled) continue; // ��Ȱ��ȭ�� ���Կ��� �߰� �Ұ�

        // ���� ������ �������̶�� ���� ���Կ� ��ġ�� �õ� (����)
        if (itemData->IsStackable() && slot.item.id == itemId && (slot.item.count + count) <= itemData->maxCount) //maxCount�� 10���� �ϱ� ��. 
        {
            slot.item.count += count;
            slot.UpdateItemBitmap(&controller, &m_itemDatabase); // ���� ������ ��Ʈ�� ������ ������, Ȥ�� �� ��� ȣ��
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

void Inventory::PackItem() //���� database�� �ִ� ��� Item�� Slot�� �־��� 
{

    for (const auto& [Id, Item] : m_itemDatabase.GetMap())
    {
        AddItem(Item->m_data.id, 1);
    }

}

void Inventory::SetDatabase(unique_ptr<ItemDatabase> database) //�ܺο��� ����� �־������/
{
    //LoadItemDatabase ������ �ϴ� ����� ��.

    //UI_Manager���� ��� Init ������ �𸣰ڴٸ�, Scene Index �ް�, Database�� �ִ� �Ŵ� 
    //GiveItem�� ���ؼ� ������ �ǰ����� ��� ���� �� �ϴ� ���� 1�� ���ڰ� 
  //  this->itemDatabase = std::move(database);
}

void Inventory::LoadUIBitmaps() 
{
    
    // +Inven�� ������ �ϴ� bitmap ������ �޾ƿ���

    






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

