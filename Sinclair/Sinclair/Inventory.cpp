#include "InvenTory.h"

Inventory::Inventory(Rect MWP) //: UI_Object(MWP) //�����ڷ� ������ �ϴ� ������ (Inven �ڱ� ���� ����)
{
    InitializeRegions();
    LoadUIBitmaps(); //SlotBitmapController, ����� ���� �ִ� �ݱ�� �� �κ� (�̰� Inven�̶� ������ ���ɼ��� ����) 
    InitializeSlots();

    //database �� �� �־��ִ� �Ŷ�,Item���� �����ϴ� �Ŵ� �ϴ� ���߿�, �׳� ������ ����� Inven�� �־��ִ� ������ �ؾ� �� �� 
    // 
    //add �������� Scene �Ѱ����� ���� �ؾ��� �� // ������ü�� Factory���� �̷�� ���ϱ�. 
    //��ġ���� �ϴ� ������ ����, Bitmap�̶� �̷� �͵��� ResourceManager���� �Ѱ� �޾ƾ� �ϴϱ�. �̰� Scene ���� ���� �ϵ��� ��.
}


void Inventory::InitializeRegions()
{
    //regions.resize(3); // 3���� ���� (4x8, 4x8, 4x8)

    //float slotSize = 48.0f;
    //float padding = 4.0f;
    //float totalSlotDimension = slotSize + padding;

    //// windowBounds�� ���� ������ �������� ��ġ
    //float currentRegionX = m_bound.x + 23.0f; // ������ ���� ����

    //// Region 0: �⺻ �ر� ���� (4x8)
    //regions[0].id = 0;
    //regions[0].isEnabled = true;
    //regions[0].gridSize = Vec2{ 4, 8 };
    //regions[0].bounds = Rect(
    //    currentRegionX,
    //    m_bound.y + 50.0f, // ������ ��� ���� + Ÿ��Ʋ�� ����
    //    regions[0].gridSize.x * totalSlotDimension,
    //    regions[0].gridSize.y * totalSlotDimension
    //);
    //currentRegionX += (regions[0].bounds.width) + 23.0f; // ���� ���� ���� X

    //// Region 1: �߰� ��� ���� (4x8)
    //regions[1].id = 1;
    //regions[1].isEnabled = false; // ���
    //regions[1].gridSize = Vec2{ 4, 8 };
    //regions[1].bounds = Rect(
    //    currentRegionX,
    //    m_bound.y + 50.0f,
    //    regions[1].gridSize.x * totalSlotDimension,
    //    regions[1].gridSize.y * totalSlotDimension
    //);
    //currentRegionX += regions[1].bounds.width + 15.0f;

    //// Region 2: ������ ��� ���� (4x8)
    //regions[2].id = 2;
    //regions[2].isEnabled = false; // ���
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

                // ���� ��ġ�� �ش� Region�� bounds�� �������� ���
                float slotX = region.bounds.x + x * totalSlotDimension;
                float slotY = region.bounds.y + y * totalSlotDimension;

                slot.SetBounds(Rect(slotX, slotY, slotSize, slotSize));
                slot.isEnabled = region.isEnabled; // ������ Ȱ��ȭ ���¿� ���� ���� Ȱ��ȭ ����
                //slot.UpdateBackgroundBitmap(&controller); // �ʱ� ��� ����
            }
        }
    }
}

bool Inventory::AddItem(string itemId, int count)
{
    const Item* itemData = itemDatabase->GetItemData(itemId);
    if (!itemData) return false;

    // �� ���� ã��
    for (auto& [key, slot] : slots)
    {
        if (!slot.isEnabled) continue; // ��Ȱ��ȭ�� ���Կ��� �߰� �Ұ�

        // ���� ������ �������̶�� ���� ���Կ� ��ġ�� �õ� (����)
        if (itemData->IsStackable() && slot.item.itemNM == itemId && (slot.item.count + count) <= itemData->maxCount)
        {
            slot.item.count += count;
            slot.UpdateItemBitmap(&controller, itemDatabase.get()); // ���� ������ ��Ʈ�� ������ ������, Ȥ�� �� ��� ȣ��
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

void Inventory::SetDatabase(unique_ptr<ItemDatabase> database) //�ܺο��� ����� �־������/
{
    this->itemDatabase = std::move(database);
}



void Inventory::LoadUIBitmaps() //�̰� ���丮���׼� �ްų� �ƴϸ� �ܺο��� �޴°ŷ� ���ڰ�.
{
    //slotbitmapcontroller�� ������ �ϴ� bitmap load�ϱ�. 
    // +Inven�� ������ �ϴ� bitmap ������ �޾ƿ���

    //�̰Ŵ� factorY���� ui�� ���� bitmap ������ ���� ���� �ű⼭ �������� �ŷ� ����. 
    //�ƴ� Resourcemanager ���� 






}

