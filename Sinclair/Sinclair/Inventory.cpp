#include "InvenTory.h"
#include "ResourceManager.h"
#include "ItemBank.h"
#include "CursorManager.h"
#include "InvenMem.h"
#include "Potion.h"

//: UI_Object(MWP) //�����ڷ� ������ �ϴ� ������ (Inven �ڱ� ���� ����)
Inventory::Inventory() :UIWindow(UIWindowType::InventoryWindow, Vec2{ 1000,500 }, Vec2{ 1208,825 })  // Vec2{ 1097,766 }) 
{

    m_bound = { 0,0,1208,825 }; // �ʱ� ��ġ  

    windowPosition = { m_bound.x,m_bound.y };

    InitializeRegions();
  //  std::cout << "[Inventory] Regions �ʱ�ȭ �Ϸ�" << std::endl;
    LoadUIBitmaps(); //����� ���� �ִ� �ݱ�� �� �κ� (�̰� Inven�̶� ������ ���ɼ��� ����) 
   // std::cout << "[Inventory] UI ��Ʈ�� �ε� �Ϸ�" << std::endl;

    InitializeSlots();
    std::cout << "[Inventory] ���� �ʱ�ȭ �Ϸ�" << std::endl; // �� ���� �� ������ �� �ȿ��� ���� ��

   LoadItemDatabase(Need_Moment::Syn);

    LoadItemDatabase(Need_Moment::Adv);
    //LoadItemDatabase(Need_Moment::Syn);

    //LoadItemDatabase(Need_Moment::Gen_2);

    //LoadItemDatabase(Need_Moment::Gen_3);
    //LoadItemDatabase(Need_Moment::Gen_4);


    PackItem();

    dragState.isDragging = false; // �巡�� ���� �ʱ�ȭ

    //0. slot Init
    //1. ��Ʈ�� �ε�
    //2. Item ������ �ֱ�

     //3. Update
     //4. Render

}


void Inventory::InitializeRegions()
{
    regions.resize(3); // 3���� ���� (4x8, 4x8, 4x8)

    //Ÿ��Ʋ �� ����, ��ġ�� �����Ѵٰ� �����ϼ� 
    titleBarBounds = Rect(m_position.x, m_position.y, m_size.x, 42.0f);  //7+35

    //�ݱ� ��ư ���� //86
    float closeButtonSize = 35.0f;
    closeButtonBounds = Rect(
        m_position.x + m_size.x - (closeButtonSize + 59), //14
        m_position.y + 7,
        closeButtonSize,
        closeButtonSize);

    //Inven ��ġ 
    windowBackground.position = m_position;
    windowBackground.size = Vec2(m_size.x, m_size.y);
    windowBackground.srcRect = D2D1::RectF(
        0.0f, 0.0f,
        static_cast<float>(m_size.x),
        static_cast<float>(m_size.y)
    );

    //�ݱ� ��ư ���� ��ġ 
    closeButton.position = Vec2(closeButtonBounds.x, closeButtonBounds.y);
    closeButton.size = Vec2(closeButtonBounds.width, closeButtonBounds.height);
    closeButton.srcRect = D2D1::RectF(0, 0, 27, 27); // �ݱ� ��ư �̹��� ũ��

    TitleBar.srcRect = D2D1::RectF(m_position.x, m_position.y, m_size.x, 42.0f);

    float slotSize = 74.0f;
    float padding_x = 13.0f;
    float padding_y = 12.0f;

    float totalSlotDimension_x = slotSize + padding_x;
    float totalSlotDimension_y = slotSize + padding_y;

    // windowBounds�� ���� ������ �������� ��ġ
    float currentRegionX = m_position.x + 68.0f; // ������ ���� ����
    RegionOffset.push_back({ currentRegionX , m_bound.y });
    // Region 0: �⺻ �ر� ���� (4x8)
    regions[0].id = 0;
    regions[0].isEnabled = true;
    regions[0].gridSize = Vec2{ 4, 8 }; //4�� 8��
    regions[0].bounds = Rect(
        currentRegionX,
        m_bound.y + 64.0f, // ������ ��� ���� + Ÿ��Ʋ�� ����
        regions[0].gridSize.x * totalSlotDimension_x,
        regions[0].gridSize.y * totalSlotDimension_y
    );
    currentRegionX += (regions[0].bounds.width) + 24.0f; // ���� ���� ���� X
    RegionOffset.push_back({ currentRegionX, m_bound.y });

    // Region 1: �߰� ��� ���� (4x8)
    regions[1].id = 1;
    regions[1].isEnabled = true; // ���
    regions[1].gridSize = Vec2{ 4, 8 };
    regions[1].bounds = Rect(
        currentRegionX,
        m_bound.y + 64.0f,
        regions[1].gridSize.x * totalSlotDimension_x,
        regions[1].gridSize.y * totalSlotDimension_y
    );
    currentRegionX += regions[1].bounds.width + 24.0f;
    RegionOffset.push_back({ currentRegionX, m_bound.y });

    // Region 2: ������ ��� ���� (4x8)
    regions[2].id = 2;
    regions[2].isEnabled = true; // ���
    regions[2].gridSize = Vec2{ 4, 8 };
    regions[2].bounds = Rect(
        currentRegionX,
        m_bound.y + 64.0f,
        regions[2].gridSize.x * totalSlotDimension_x,
        regions[2].gridSize.y * totalSlotDimension_y
    );
}

void Inventory::InitializeSlots()
{
    float slotSize = 74.0f;
    float padding_x = 13.0f;
    float padding_y = 12.0f;

    float totalSlotDimension_x = slotSize + padding_x;
    float totalSlotDimension_y = slotSize + padding_y;

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
                float slotX = region.bounds.x + x * totalSlotDimension_x;
                float slotY = region.bounds.y + y * totalSlotDimension_y;

                slot.SetBounds(Rect(slotX, slotY, slotSize, slotSize));
                slot.isEnabled = region.isEnabled; // ������ Ȱ��ȭ ���¿� ���� ���� Ȱ��ȭ ����
                slot.UpdateBackgroundBitmap(&controller); // �ʱ� ��� ����
            }
        }
    }

    //UpdateSlotPositions();
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

void Inventory::Update() //�Է�ó���� ���� ������ �ؾ��ϴ� ���ݾ�? //���� �̰Ŵ� �ǹ� ���� �� ��? ���� �Է� ó�� ������ �˾Ƽ� ���� �� �κ��̶�. 
{
    // effect�� �ʿ��ϴٸ� ���⼭ Update �ϴ� �� �¾�. 
    if (!m_isActive) return;

    // â ��ġ�� ����� ������ Ÿ��Ʋ�� �� �ݱ� ��ư�� ��ġ�� ������Ʈ
    titleBarBounds = Rect(m_position.x, m_position.y, m_size.x, 42.0f);
    float closeButtonSize = 35.0f;
    closeButtonBounds = Rect(
        m_position.x + m_size.x - (closeButtonSize + 59),
        m_position.y + 7,
        closeButtonSize,
        closeButtonSize);

    UpdateSlotPositions();

    // ������ ��� �̹��� ��ġ ������Ʈ
    windowBackground.position = m_position;
}

void Inventory::Render()
{
    // 1. ������ ��� ������
    if (windowBackground.bitmap)
    {
        D2D1_RECT_F destRect = D2D1::RectF(
            m_position.x, m_position.y,
            m_position.x + m_size.x, m_position.y + m_size.y
        );
        D2D1_RECT_F srcRect = windowBackground.srcRect;

        D2DRenderer::Get().DrawBitmap(
            static_cast<ID2D1Bitmap1*>(windowBackground.bitmap),
            destRect,
            srcRect,
            windowBackground.opacity);
    }
    //std::cout << " slot.windowBackground:Opacity" << windowBackground.opacity << endl;


    // 2. Ÿ��Ʋ�� �� �ݱ� ��ư ������
   // RenderTitleBar();
    RenderCloseButton();

    // 3. ��� ���� ������
    for (const auto& [key, slot] : slots)
    {
        RenderSlot(slot);
    }

    //if (dragState.isDragging && dragState.dragBitmap.bitmap)
    //{
    //    D2D1_RECT_F dragDestRect = D2D1::RectF(
    //        dragState.mousePos.x - dragState.dragBitmap.size.x / 2.0f, // ���콺 �߾ӿ� ������
    //        dragState.mousePos.y - dragState.dragBitmap.size.y / 2.0f,
    //        dragState.mousePos.x + dragState.dragBitmap.size.x / 2.0f,
    //        dragState.mousePos.y + dragState.dragBitmap.size.y / 2.0f
    //    );

    //    D2DRenderer::Get().DrawBitmap(static_cast<ID2D1Bitmap1*>(dragState.dragBitmap.bitmap),
    //        dragDestRect, dragState.dragBitmap.srcRect, 1.0f);
    //}

    // 5. ���� ������ (���� �������� �������Ͽ� �ٸ� UI ���� ǥ��)
   /* if (showTooltip)
    {
        RenderTooltip(renderTarget);
    }*/
}



bool Inventory::HandleMouseHover(Vec2 mousePos)
{
    if (!m_isActive)
    {
        // showTooltip = false;
        return false;
    }

    // ������ �巡�� ���� ��
    if (isWindowDragging) // 
    {
        float deltaX = mousePos.x - dragStartMousePos.x;
        float deltaY = mousePos.y - dragStartMousePos.y;

        windowPosition = Vec2(dragStartWindowPos.x + deltaX, dragStartWindowPos.y + deltaY);

        m_position = Vec2(dragStartWindowPos.x + deltaX, dragStartWindowPos.y + deltaY); //�θ����� �Ѱ��� ��. -> ��� 

        // ������ �ٿ��, Ÿ��Ʋ��, �ݱ� ��ư �ٿ�� ������Ʈ
        m_bound.x = windowPosition.x;
        m_bound.y = windowPosition.y;
        titleBarBounds.x = windowPosition.x;
        titleBarBounds.y = windowPosition.y;

        float closeButtonSize = 27.0f;

        closeButtonBounds.x = windowPosition.x + m_bound.width - (closeButtonSize + 14);
        closeButtonBounds.y = windowPosition.y + 5;

        // ���Ե��� ��ġ�� ������Ʈ�ؾ� ��
        UpdateSlotPositions();

        return true;
    }
    else // ������ �巡�� ���� �ƴ� ���� ���� ȣ�� �� ���� ó��
    {
        //showTooltip = false;
        InventorySlot* hoveredSlot = nullptr;

        // ���� ȣ�� ���� ������Ʈ
        for (auto& [key, slot] : slots)
        {
            bool wasHovered = slot.isHovered;
            // ������ ���� ������ ��ġ(�̵��� â�� ���� �޶���)�� �������� Contains üũ
            // slot.bounds�� �̹� UpdateSlotPositions�� ���� ���� â ��ġ�� �°� ������Ʈ�� (Ȥ�� �̹� �ʱ�ȭ �� �ùٸ��� ������)

            slot.isHovered = slot.bounds.Contains(mousePos); //�ȿ� ������ hovered �ε�, ���⼭ ��� ������ �ȵǾ� �ϰŵ� 

            if (wasHovered != slot.isHovered)
            {
                slot.UpdateBackgroundBitmap(&controller);
            }

            if (slot.isHovered && !slot.IsEmpty())  //-> 
            {
                hoveredSlot = &slot; // ���� ���콺�� �ö� ����
            }
        }

        // ���� ���� ������Ʈ
        if (hoveredSlot != nullptr)
        {
             Item* data = m_itemDatabase.GetItemData(hoveredSlot->item.id); // �� ����� ������ ���� �������� ����. ���� 
             //Item* data = hoveredSlot->item.


            if (data)
            {
                CursorManager::Get().SetHoveredItem(data); 
                Vec2 tooltipPos = mousePos + Vec2(10, 10);

                UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, tooltipPos); //��ġ �����Ű��, Ȱ��ȭ���� 
            }
            return true;

        }

        else //������ �� �����Ű���...
        {
            CursorManager::Get().HoveredReleased(); //���� ���� 
            UIManager::Get().CloseWindow(UIWindowType::InventoryTooltip); //����
            return true;

        }
        return false;

    }

    // ������ �巡�� ���� �� (���콺 ��ġ ������Ʈ)
    if (dragState.isDragging)
    {
        dragState.mousePos = mousePos;
    }
}

bool Inventory::HandleMouseDown(Vec2 mousePos) //������ Inven ��ġ ���� �־�� �̰� ó���� �Ǵ� �Ŵϱ� 
{
    if (!m_isActive) return false;

    InventorySlot* slot = GetSlotAt(mousePos);
    if (slot && !slot->IsEmpty() && slot->isEnabled)
    {
        // CursorManager�� ���� ���������� ������ �巡�� ����
        CursorManager::Get().StartItemDrag(slot->item.id, DragSource::Inventory, slot);
        CursorManager::Get().SetDraggedItem(m_itemDatabase.GetItemData(slot->item.id)); // ���� Item* ����

        // ������ ������ CursorManager�� �ѱ�� ���� ����.
        slot->Clear();
        slot->UpdateItemBitmap(&controller, &m_itemDatabase);

        return true; // �Է� ó�� �Ϸ�
    }
    // ���� �� Ŭ�� �� �ֻ������.
    if (IsInBounds(mousePos))
    {
        UIManager::Get().OpenWindow(m_windowType);
        return true;
    }

    return false;
}

bool Inventory::HandleMouseUp(Vec2 mousePos) //�� ���� ��ġ�� ���� ����ó���� �ؾ� ��. ���� ���� �ߴ��� �˾ƾ� �� �� 
{
    if (!m_isActive) return false;

    // CursorManager���� �巡�� ���� �������� �ִ��� Ȯ��
    if (!CursorManager::Get().IsDragging()) return false;

    InventorySlot* targetSlot = GetSlotAt(mousePos); // ���� ���콺 ��ġ�� ������ �ٽ� ã��
    Item* draggedItemData = CursorManager::Get().GetDraggedItem(); // CursorManager���� �巡�� ���� ������ ��������
    DragSource dragSource = CursorManager::Get().GetDragSource();

    bool placed = false;

    if (targetSlot && targetSlot->isEnabled && draggedItemData)
    {
        // ���� ������ �������̶�� ��ġ�� �õ�
        if (draggedItemData->IsStackable() &&
            targetSlot->item.id == draggedItemData->m_data.id &&
            (targetSlot->item.count + 1) <= draggedItemData->maxCount) // count�� CursorManager���� �����ؾ� ��
        {
            //std::cout << "���� ����. ������ ��ĥ����." << std::endl; ������ �°� ����.
            targetSlot->item.count += 1; // �巡�׵� �������� ���� count�� ���ؾ� ��
            targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
            placed = true;
        }
        else if (targetSlot->IsEmpty())
        {
            // �� ���Կ� ���
            targetSlot->SetItem(draggedItemData->m_data.id, 1); // count�� CursorManager���� �����ؾ� ��
            targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
            placed = true;
        }
        else // ������ ������� �ʰ� ���� �Ұ����ϰų� �ٸ� �������� ��� (��ȯ ����)
        {
            // ���� ���� ���� ��������
            InventorySlot* sourceSlot = CursorManager::Get().GetSourceSlot();

            if (sourceSlot)
            {
                // ���� Ÿ�� ������ �������� �ӽ� ����
                ItemInstance tempItem = targetSlot->item;

                // Ÿ�� ���Կ� �巡�׵� ������ ����
                targetSlot->SetItem(draggedItemData->m_data.id, 1);
                targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);

                // ���� ���Կ� ��ȯ�� ������ ����
                sourceSlot->SetItem(tempItem.id, tempItem.count);
                sourceSlot->UpdateItemBitmap(&controller, &m_itemDatabase);

                placed = true;
            }
            else
            {
                // �� ���Կ� �߰�
                ItemInstance tempItem = targetSlot->item;
                targetSlot->SetItem(draggedItemData->m_data.id, 1);
                targetSlot->UpdateItemBitmap(&controller, &m_itemDatabase);
                AddItem(tempItem.id, tempItem.count);
                placed = true;
            }
        }
    }
    if (IsInBounds(mousePos))
    {
        UIManager::Get().OpenWindow(m_windowType);
    }
    if (placed)
    {
        CursorManager::Get().EndItemDrag(); // ��� ���� �� �巡�� ����
        return true;
    }
    else
    {
        return HandleDropFailure(mousePos, draggedItemData, dragSource);
    }

    // ���� �� Ŭ�� �� �ֻ������.


    return placed;
}

bool Inventory::HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source)
{
    if (!draggedItem) return false;

    // 1. �ٸ� â���� �������� Ȯ��
    bool isInOtherWindow = false;

    // ���â ���� Ȯ��
    UIWindow* equipWindow = UIManager::Get().GetWindow(UIWindowType::EquipmentWindow);
    if (equipWindow && equipWindow->IsActive() && equipWindow->IsInBounds(mousePos))
    {
        isInOtherWindow = true;
    }

    // ��ȭâ ���� Ȯ�� (Ȱ��ȭ�Ǿ� �ִٸ�)
    UIWindow* enhanceWindow = UIManager::Get().GetWindow(UIWindowType::EnhancementWindow);
    if (enhanceWindow && enhanceWindow->IsActive() && enhanceWindow->IsInBounds(mousePos))
    {
        isInOtherWindow = true;
    }

    // �ռ�â ���� Ȯ�� (Ȱ��ȭ�Ǿ� �ִٸ�)
    UIWindow* synthesisWindow = UIManager::Get().GetWindow(UIWindowType::SynthesisWindow);
    if (synthesisWindow && synthesisWindow->IsActive() && synthesisWindow->IsInBounds(mousePos))
    {
        isInOtherWindow = true;
    }

    // 2. �ٸ� â �����̸� �ش� â���� ó���ϵ��� �ѱ� (�巡�� ���� ����)
    if (isInOtherWindow)
    {
        return false; // �ٸ� â���� ó���ϵ��� �ѱ�
    }

    // 3. � â ������ �ƴϸ� ���� �κ��丮�� ����
    if (source == DragSource::Inventory)
    {
        // �κ��丮���� ���� �������̹Ƿ� �� ���Կ� �ٽ� �ֱ� �õ�
        if (AddItem(draggedItem->m_data.id, 1))
        {
            std::cout << "�������� �κ��丮�� �����߽��ϴ�: " << draggedItem->m_data.name << std::endl;
            CursorManager::Get().EndItemDrag();
            return true;
        }
        else
        {
            std::cout << "�κ��丮�� ���� ��. ������ ���� ����." << std::endl;
            // �κ��丮�� ���� �� ��쿡�� �巡�� ���� (������ �ҽ� ������ ���� �α� ���)
            CursorManager::Get().EndItemDrag();
            return true;
        }
    }
    else if (source == DragSource::Equipment)
    {
        // ���â���� ���� �������̹Ƿ� �κ��丮�� �̵�
        if (AddItem(draggedItem->m_data.id, 1))
        {
            std::cout << "��� �������� �κ��丮�� �̵��߽��ϴ�: " << draggedItem->m_data.name << std::endl;
            CursorManager::Get().EndItemDrag();
            return true;
        }
        else
        {
            std::cout << "�κ��丮�� ���� ��. ��� ���� �������� �����մϴ�." << std::endl;
            // �κ��丮�� ���� ���� ���� ��� �������� �����ؾ� ��
            // �̴� EquipmentWindow���� ó���ؾ� �� ����
            return false;
        }
    }

    // �⺻������ �巡�� ����
    CursorManager::Get().EndItemDrag();
    return true;
}


bool Inventory::HandleDoubleClick(Vec2 mousePos)
{
    

    return false;
}

bool Inventory::HandleMouseRight(Vec2 mousePos)
{
    InventorySlot* slot = GetSlotAt(mousePos);
    if (slot && !slot->IsEmpty())
    {
        Potion* item = dynamic_cast<Potion*>(m_itemDatabase.GetItemData(slot->item.id));
        if (item != nullptr)
        {
            int much  = item->GetMuch() -1 ;

            UIManager::Get().ShowPotionWindow(much); //�����ǵ� ���� ���ȴٰ� 

        }



    }


    return false;
}

UIWindowType Inventory::GetType()
{
    return m_windowType;
}

InventorySlot* Inventory::GetSlotAt(const Vec2& pos)
{
    if (!m_isActive || isWindowDragging) return nullptr; // ������ ��Ȱ�� �Ǵ� �巡�� �߿��� ���� Ŭ�� �Ұ�

    for (auto& [key, slot] : slots)
    {
        // ������ bounds�� �̹� UpdateSlotPositions�� ���� ���� â ��ġ�� �°� ������Ʈ�Ǿ� ����
        if (slot.bounds.Contains(pos))
        {
            return &slot;
        }
    }
    return nullptr;
}

void Inventory::RenderTitleBar()
{
    auto& renderer = D2DRenderer::Get();
    auto context = renderer.GetContext();

    if (context)
    {
        // 1. Ÿ��Ʋ�� ���� ���� ��� (���� �� �귯�� ���)
        ComPtr<ID2D1SolidColorBrush> darkBrush;
        context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkSlateGray, 0.7f), &darkBrush);

        D2D1_RECT_F rect = titleBarBounds.ToD2DRect();
        context->FillRectangle(rect, darkBrush.Get());


        D2DRenderer::Get().DrawBitmap(static_cast<ID2D1Bitmap1*>(TitleBar.bitmap), rect, TitleBar.srcRect, 1.0f);

    }
    // 2. �ؽ�Ʈ ���
    auto textBrush = renderer.GetTBrush();
    auto textFormat = renderer.GetTFormat();

    if (textFormat && textBrush)
    {
        context->DrawText(
            L"Inventory",
            static_cast<UINT32>(wcslen(L"Inventory")),
            textFormat.Get(),
            D2D1::RectF(
                titleBarBounds.x + 10,
                titleBarBounds.y + 5,
                titleBarBounds.x + titleBarBounds.width - 10,
                titleBarBounds.y + titleBarBounds.height - 5
            ),
            textBrush.Get()
        );
    }
}

void Inventory::RenderCloseButton()
{
    if (closeButton.bitmap)
    {
        // �ݱ� ��ư ��ġ�� ���� â ��ġ�� ���� ������Ʈ
        float rightMargin = 47.0f;
        Vec2 currentCloseButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 7 };

        D2D1_RECT_F destRect = D2D1::RectF(
            currentCloseButtonPos.x, currentCloseButtonPos.y,
            currentCloseButtonPos.x + 27.0f, currentCloseButtonPos.y + 27.0f
        );
        D2D1_RECT_F srcRect = closeButton.srcRect;

        D2DRenderer::Get().DrawBitmap(
            static_cast<ID2D1Bitmap1*>(closeButton.bitmap),
            destRect,
            srcRect,
            closeButton.opacity);
    }
}

void Inventory::RenderSlot(const InventorySlot& slot)
{
    // ���� ��� ������
    if (slot.backgroundBitmap.bitmap)
    {
        D2D1_RECT_F destRect = D2D1::RectF(
            slot.bounds.x, slot.bounds.y,
            slot.bounds.x + slot.bounds.width,
            slot.bounds.y + slot.bounds.height
        );
        D2D1_RECT_F srcRect = slot.backgroundBitmap.srcRect;

        D2DRenderer::Get().DrawBitmap(
            static_cast<ID2D1Bitmap1*>(slot.backgroundBitmap.bitmap),
            destRect,
            srcRect,
            slot.backgroundBitmap.opacity);
    }

    // ��Ȱ��ȭ�� ������ ��slot�̹��� �׸�.
    if (!slot.isEnabled)
    {
        //std::cout << "��Ȱ��ȭ�� �����̹Ƿ� ��游 �������ϰ� ����" << std::endl;
        return;
    }

    // 2. ������ ������ ������
    if (!slot.IsEmpty() && slot.itemBitmap.bitmap)
    {
        D2D1_RECT_F itemDestRect = D2D1::RectF(
            slot.itemBitmap.position.x, // itemBitmap.position�� SetBounds���� �̹� ���� bounds�� ���� ������
            slot.itemBitmap.position.y,
            slot.itemBitmap.position.x + slot.itemBitmap.size.x,
            slot.itemBitmap.position.y + slot.itemBitmap.size.y
        );
        D2D1_RECT_F itemSrcRect = slot.itemBitmap.srcRect;

        D2DRenderer::Get().DrawBitmap(
            static_cast<ID2D1Bitmap1*>(slot.itemBitmap.bitmap),
            itemDestRect,
            itemSrcRect,
            slot.itemBitmap.opacity

        );
        //  std::cout << " slot.itemBitmap:Opacity" << slot.itemBitmap.opacity << endl;


          // 3. ������ ���� �ؽ�Ʈ
        if (slot.item.count > 1)
        {
            std::wstring countText = std::to_wstring(slot.item.count);
            // �ؽ�Ʈ ��ġ�� ���� ���� �ϴܿ� ��ġ (����)
            D2D1_RECT_F textRect = D2D1::RectF(
                slot.bounds.x + slot.bounds.width - 30, // ���� ����
                slot.bounds.y + slot.bounds.height - 20, // �ϴ� ����
                slot.bounds.x + slot.bounds.width,
                slot.bounds.y + slot.bounds.height
            );
            D2DRenderer::Get().GetContext()->DrawText(
                countText.c_str(),
                static_cast<UINT32>(countText.length()),
                D2DRenderer::Get().GetTFormat().Get(),
                &textRect,
                D2DRenderer::Get().GetTBrush().Get()
            );
        }
    }
}

void Inventory::UpdateSlotPositions() // -> widndow �������� �ǰ� ���� �ʾƿ� 
{
    float slotSize = 74.0f;
    float padding_x = 13.0f;
    float padding_y = 12.0f;

    /* float totalSlotDimension_x = windowPosition.x + slotSize + padding_x;
     float totalSlotDimension_y = windowPosition.y+ slotSize + padding_y;*/

    float totalSlotDimension_x = slotSize + padding_x;
    float totalSlotDimension_y = slotSize + padding_y;

    // Regions�� bounds�� â�� ���� ��ġ�� �������� ������Ʈ
    float currentRegionX = m_position.x + 68.0f;
    regions[0].bounds = Rect(
        currentRegionX,
        m_position.y + 64.0f,
        regions[0].gridSize.x * totalSlotDimension_x,
        regions[0].gridSize.y * totalSlotDimension_y
    );
    currentRegionX += (regions[0].bounds.width) + 24.0f;
    regions[1].bounds = Rect(
        currentRegionX,
        m_position.y + 64.0f,
        regions[1].gridSize.x * totalSlotDimension_x,
        regions[1].gridSize.y * totalSlotDimension_y
    );
    currentRegionX += regions[1].bounds.width + 24.0f;
    regions[2].bounds = Rect(
        currentRegionX,
        m_position.y + 64.0f,
        regions[2].gridSize.x * totalSlotDimension_x,
        regions[2].gridSize.y * totalSlotDimension_y
    );

    for (int regionId = 0; regionId < regions.size(); ++regionId)
    {
        const Region& region = regions[regionId];

        for (int y = 0; y < region.gridSize.y; ++y)
        {
            for (int x = 0; x < region.gridSize.x; ++x)
            {
                int slotIndex = y * region.gridSize.x + x;
                std::pair<int, int> key = { regionId, slotIndex };

                if (slots.count(key))
                {
                    InventorySlot& slot = slots[key];

                    // ���� ��ġ�� �ش� Region�� bounds�� �������� ��� (regions.bounds�� InitializeRegions���� windowPosition�� ���� �̹� ������)
                    //float slotX = region.bounds.x + x * totalSlotDimension_x;
                    //float slotY = region.bounds.y + y * totalSlotDimension_y;

                    //float slotX = m_bound.x + RegionOffset[regionId].x + x * totalSlotDimension_x;
                    //float slotY = m_bound.y + RegionOffset[regionId].y + y * totalSlotDimension_y+ 64.0f;

                    // region.bounds.x�� region.bounds.y�� ���� ��ġ ����ؾ���.
                    float slotX = region.bounds.x + x * totalSlotDimension_x;
                    float slotY = region.bounds.y + y * totalSlotDimension_y;

                    slot.SetBounds(Rect(slotX, slotY, slotSize, slotSize));
                }
            }
        }
    }
}

ItemDatabase& Inventory::GetItemBase()
{
    return m_itemDatabase;
}

void Inventory::PackItem() //���� database�� �ִ� ��� Item�� Slot�� �־��� 
{

    for (const auto& [Id, Item] : m_itemDatabase.GetMap())
    {
        //std::cout << Item->m_data.id << endl;
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


    windowBackground.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("InvenBg").Get();
    tooltipBackground.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("SlotDisabled").Get(); //����
    tooltipBackground.srcRect = D2D1::RectF(0, 0, 234, 345); // ����. -> Tooltip 

    closeButton.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("CloseButton").Get();
    //TitleBar.bitmap = ResourceManager::Get().Get_UIBank().Get_Image("TitleBar").Get();

    controller.bitmaps.emplace("slot_disabled",
        ResourceManager::Get().Get_UIBank().Get_Image("SlotDisabled").Get());
    controller.bitmaps.emplace("slot_hover",
        ResourceManager::Get().Get_UIBank().Get_Image("SlotNormal").Get());
    controller.bitmaps.emplace("slot_selected",
        ResourceManager::Get().Get_UIBank().Get_Image("SlotNormal").Get());
    controller.bitmaps.emplace("slot_normal",
        ResourceManager::Get().Get_UIBank().Get_Image("SlotNormal").Get());

}

void Inventory::LoadItemDatabase(Need_Moment Moment)
{
    ResourceManager::Get().Get_ItemBank().GiveItem(Moment, m_itemDatabase);
}