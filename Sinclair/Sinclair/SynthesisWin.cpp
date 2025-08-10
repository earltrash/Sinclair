#include "SynthesisWin.h"
#include "UI_Renderer.h"
#include "ResourceManager.h"
#include "UIManager.h"
#include "CursorManager.h"

#include "GameManager.h"

SynthesisWin::SynthesisWin() :UIWindow(UIWindowType::SynthesisWindow, Vec2{ 0,0 }, Vec2{ 524,766 })
{
	//�������� ���� �˾Ƽ� �ǰ� 

	MemInit();
	MemBitmapLoad();
}

SynthesisWin::~SynthesisWin()
{

}

bool SynthesisWin::HandleMouseHover(Vec2 mousePos) //hover? 
{
	if (!m_isActive) return false;


	// �巡�� ���̸� â ��ġ ������Ʈ
	if (m_isDragging)
	{
		m_position = mousePos - m_dragOffset;

		// â ��ġ�� �ٲ�� ���� ��ġ�鵵 �ٽ� ���
		UpdatePosition();
		return true;
	}

	else 
	{
		// ���Կ� ���콺 ���� �� ���� ǥ��
		SynSlot whichSlot = SlotInit(mousePos);

		if (whichSlot != SynSlot::Nothing)
		{
			Item* Clicked_Item = m_slot_Item[whichSlot];
			if (Clicked_Item != nullptr)
			{
				CursorManager::Get().SetHoveredItem(Clicked_Item);
				Vec2 tooltipPos = mousePos + Vec2(10, 10);

				UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, tooltipPos); //��ġ �����Ű��, Ȱ��ȭ���� 
				return true;
			}
			else
				return true;

		}

		else/* if(whichSlot == SynSlot::Nothing)*/
		{
			CursorManager::Get().HoveredReleased(); //���� ���� 
			return false;
		}
	}
	

	CursorManager::Get().HoveredReleased(); //���� ���� 
	UIManager::Get().CloseWindow(UIWindowType::InventoryTooltip); //����
	return false;
}

bool SynthesisWin::HandleMouseDown(Vec2 mousePos) //������ �����̴� �� // slot position �ᱹ ��ġ���̶� 
{

	if (!m_isActive) return false;

	SynSlot whichSlot = SlotInit(mousePos);

	if (whichSlot != SynSlot::Nothing)
	{
		Item* Clicked_Item = m_slot_Item[whichSlot];

		if (Clicked_Item == nullptr)
			return false; // �� �����̸� �巡�� �� ��

		CursorManager::Get().StartItemDrag_NS(Clicked_Item->m_data.id, DragSource::Equipment);
		CursorManager::Get().SetDraggedItem(Clicked_Item);

		if (whichSlot == SynSlot::Result) //�������� �ִ� result ������ ������ �� ��ȯ��Ų�ٶ� 
		{
			m_slot_Item[SynSlot::Result] = nullptr;
			m_slot_Item[SynSlot::Slot1] = nullptr;
			m_slot_Item[SynSlot::Slot2] = nullptr;
		}
		else //�ƴ� ��쿡��, ��ġ ���� ���Կ� �ִ��� ������ 
		{
			m_slot_Item[whichSlot] = nullptr;
		}
		return true; // �巡�� ���� �ÿ��� ������ true ��ȯ
	}

	SynButton whichBut = ButtonInit(mousePos);

	// cancel ��ư Ŭ�� �� ���� ����.
	if (whichBut != SynButton::Nothing && whichBut == SynButton::Cancle)
	{

		for (auto& [type, pos] : m_slot)
		{
			Item* item = m_slot_Item[type];

			if (item != nullptr)
			{
				auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
				inventoryWindow->AddItem(item->m_data.id, 1); //�� ���Ÿ� �κ��� �������� 


			}
		}
		// cancel ������ �� �� ��ȯ. 
		m_slot_Item[SynSlot::Result] = nullptr;
		m_slot_Item[SynSlot::Slot1] = nullptr;
		m_slot_Item[SynSlot::Slot2] = nullptr;

		return true;
	}
	else if (whichBut != SynButton::Nothing && whichBut == SynButton::Syn)
	{
		//�� �ռ��̰��� 
		PerformSynthesis();
		return true;

	}

	if (IsInBounds(mousePos))
	{
		UIManager::Get().OpenWindow(m_windowType);
		return true;
	}


	return false;
}

bool SynthesisWin::HandleMouseUp(Vec2 mousePos) //�������� ���� ó�� 
{
	if (!m_isActive) return false;

	//  ���� �������� �巡�� ������ Ȯ��
	if (!CursorManager::Get().IsDragging()) return false;
	
		// �巡�� ���� �����۰� �ҽ� ���� ��������
		Item* draggedItem = CursorManager::Get().GetDraggedItem();
		DragSource dragSource = CursorManager::Get().GetDragSource();
		SynSlot whichSlot = SlotInit(mousePos);


		if (whichSlot == SynSlot::Result && draggedItem != nullptr)
		{
			auto* inventoryWindow = dynamic_cast<Inventory*>(
				UIManager::Get().GetWindow(UIWindowType::InventoryWindow));

			// ���� ������ ����
			Item* previousItem = m_slot_Item[whichSlot];
			if (previousItem != nullptr) {
				inventoryWindow->AddItem(previousItem->m_data.id, 1);
			}

			// ����� �����۵� ����
			inventoryWindow->AddItem(draggedItem->m_data.id, 1);

			// ��� ���� ����
			m_slot_Item[whichSlot] = nullptr;

			CursorManager::Get().EndItemDrag();
			return true;
		}

		else if (whichSlot != SynSlot::Nothing && draggedItem != nullptr)
		{
			Item* previousItem = m_slot_Item[whichSlot];
			m_slot_Item[whichSlot] = draggedItem;

			if (previousItem)
			{
				auto* inventoryWindow = dynamic_cast<Inventory*>(
					UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
				inventoryWindow->AddItem(previousItem->m_data.id, 1);
			}

			CursorManager::Get().EndItemDrag();

			// ����� �������� �ٷ� hover ���·�
			CursorManager::Get().SetHoveredItem(draggedItem);
			UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, mousePos + Vec2(10, 10));

			return true;
		}

		


		else //�������� Slot�̰ų�, �ٸ� â�� ��쵵 ���� ����? 
		{
			CursorManager::Get().EndItemDrag();
			return HandleDropFailure(mousePos, draggedItem, dragSource);
		}
	

	CursorManager::Get().HoveredReleased();
	return false;
}

bool SynthesisWin::HandleDoubleClick(Vec2 mousePos) //swap ����? sort�� �ʿ��� �� ����. Inven�� ��Ȱ �Ǿ�� ���� �����ؾ� �� �� ���� 
{
	return false;
}

bool SynthesisWin::HandleMouseRight(Vec2 mousePos) //slot ��ġ�� mousepos -> 1 2 ��°�� ��쿡��. 
{
	
	SynSlot whichSlot = SlotInit(mousePos); //���콺 ������ Ŭ���� ���� 

	if (whichSlot != SynSlot::Nothing)
	{
		Item* Clicked_Item = m_slot_Item[whichSlot]; 

		if (Clicked_Item != nullptr)
		{
			//���� ���Կ� �ִ� 
			m_slot_Item[whichSlot] = nullptr;

			if (auto* inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow)))
			{
				inventory->AddItem(Clicked_Item->m_data.id, 1);

				return true;
			}
		}



	}

	return false;
}

void SynthesisWin::UpdatePosition()
{
	m_slot[SynSlot::Result] = { m_position.x + 226,m_position.y + 140 };
	m_slot[SynSlot::Slot1] = { m_position.x + 92 , m_position.y + 419 };
	m_slot[SynSlot::Slot2] = { m_position.x + 359 ,m_position.y + 419 };

	m_but[SynButton::Cancle] = { m_position.x + 52,m_position.y + 626 };
	m_but[SynButton::Syn] = { m_position.x + 286,m_position.y + 626 };


}

void SynthesisWin::MemBitmapLoad()
{
	UI_Renderer* uiRenderer = AddComponent<UI_Renderer>();
	if (uiRenderer)
	{
		// �ʿ��� UI ��Ʈ�ʵ��� ResourceManager���� �ε��Ͽ� UI_Renderer�� ����
		// ResourceManager�� �ش� Ű�� ��Ʈ���� �ε�
		uiRenderer->SetBitmap("Syn_BG", ResourceManager::Get().GetTexture("SynBg"));
		uiRenderer->SetBitmap("Close_But", ResourceManager::Get().GetTexture("CloseButton"));

		uiRenderer->SetBitmap("Syn_Result", ResourceManager::Get().GetTexture("SynResult"));
		uiRenderer->SetBitmap("Syn_Slot", ResourceManager::Get().GetTexture("SlotNormal"));

		uiRenderer->SetBitmap("Cancle_But", ResourceManager::Get().GetTexture("CancleBut"));
		uiRenderer->SetBitmap("Syn_But", ResourceManager::Get().GetTexture("SynBut"));
	}
}

void SynthesisWin::MemInit() //0 1 2 -> ��ġ �־��ְ�. 0�� �����ϰ� �־��ִ� ������ �ؾ߰ڴ�.  
{
	m_slot[SynSlot::Result] = { 226 , 140 };
	m_slot[SynSlot::Slot1] = { 92 , 419 };
	m_slot[SynSlot::Slot2] = { 359 , 419 };

	m_but[SynButton::Cancle] = { 52,626 };
	m_but[SynButton::Syn] = { 286,626 };

}

void SynthesisWin::MemRender()
{
	UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();

	if (uiRenderer) //���
	{
		ID2D1Bitmap1* backgroundBitmap = uiRenderer->GetBitmap("Syn_BG").Get();
		if (backgroundBitmap)
		{
			D2D1_RECT_F destRect = { m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y };
			D2DRenderer::Get().DrawBitmap(backgroundBitmap, destRect);
		}

		float rightMargin = 65.0f;
		Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 36 };
		Vec2 closeButtonSize = { 35.0f, 35.0f };
		D2D1_RECT_F destRect = { closeButtonPos.x, closeButtonPos.y, closeButtonPos.x + closeButtonSize.x, closeButtonPos.y + closeButtonSize.y };


		ID2D1Bitmap1* closeButtonBitmap = uiRenderer->GetBitmap("Close_But").Get();

		if (closeButtonBitmap)
		{
			D2DRenderer::Get().DrawBitmap(closeButtonBitmap, destRect);
		}



		for (const auto& [type, pos] : m_slot)
		{
			Item* item = m_slot_Item[type];

			if (item != nullptr)
			{

				D2D1_RECT_F DEST = { pos.x, pos.y, SlotSize + pos.x , SlotSize + pos.y };

				D2DRenderer::Get().DrawBitmap(ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id).atlas.Get(),
					DEST, ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id).srcRect, 1);
			}
		}

		for (const auto& [type, pos] : m_but) //�׳�
		{
			ID2D1Bitmap1* closeButtonBitmap = (type == SynButton::Cancle) ? uiRenderer->GetBitmap("Cancle_But").Get() : uiRenderer->GetBitmap("Syn_But").Get();
			D2D1_RECT_F DEST = { pos.x,pos.y,pos.x + ButX,pos.y + ButY };
			D2DRenderer::Get().DrawBitmap(closeButtonBitmap, DEST);
		}


	}

}

SynButton SynthesisWin::ButtonInit(Vec2 mpos)
{
	for (const auto& [but, pos] : m_but)
	{
		if ((mpos.x >= pos.x && mpos.x <= pos.x + ButX &&
			mpos.y >= pos.y && mpos.y <= pos.y + ButY) == true)
		{
			return but;
		}
	}
	return SynButton::Nothing;

}

SynSlot SynthesisWin::SlotInit(Vec2 mpos)
{
	for (const auto& [slot, pos] : m_slot)
	{
		if ((mpos.x >= pos.x && mpos.x <= pos.x + SlotSize &&
			mpos.y >= pos.y && mpos.y <= pos.y + SlotSize) == true)
		{
			return slot;
		}
	}
	return SynSlot::Nothing;
}

bool SynthesisWin::HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source)
{
	if (!draggedItem) return false;

	// �ٸ� â ���ο� �ִ��� Ȯ��
	bool isInOtherWindow = false;

	// �κ��丮 â ���� Ȯ��
	UIWindow* inventoryWindow = UIManager::Get().GetWindow(UIWindowType::InventoryWindow);
	if (inventoryWindow && inventoryWindow->IsActive() && inventoryWindow->IsInBounds(mousePos))
	{
		isInOtherWindow = true;
	}

	// ��� â ���� Ȯ��
	UIWindow* equipmentWindow = UIManager::Get().GetWindow(UIWindowType::EquipmentWindow);
	if (equipmentWindow && equipmentWindow->IsActive() && equipmentWindow->IsInBounds(mousePos))
	{
		isInOtherWindow = true;
	}

	// ��ȭ â ���� Ȯ��
	UIWindow* enhancementWindow = UIManager::Get().GetWindow(UIWindowType::EnhancementWindow);
	if (enhancementWindow && enhancementWindow->IsActive() && enhancementWindow->IsInBounds(mousePos))
	{
		isInOtherWindow = true;
	}

	// �ٸ� â ���ζ�� �ش� â���� ó���ϵ��� �ѱ�
	if (isInOtherWindow)
	{
		return false;
	}

	// ��� â ���ε� �ƴϸ� ���� ��ġ�� ����
	if (source == DragSource::Inventory)
	{
		// �κ��丮���� �� �������̶�� �κ��丮�� ����
		auto* inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
		if (inventory && inventory->AddItem(draggedItem->m_data.id, 1))
		{
			std::cout << "�ռ� �Ұ����� �������� �κ��丮�� ����: " << draggedItem->m_data.id << std::endl;
			return true;
		}
	}
	else if (source == DragSource::Equipment)
	{
		// ���â���� �� �������̶�� ���â���� ����
		auto* equipment = dynamic_cast<EquipmentWindow*>(UIManager::Get().GetWindow(UIWindowType::EquipmentWindow));
		if (equipment)
		{
			// ���â�� ���� ������ �ִٸ� �ش� �������� ����
			// ���� CursorManager���� ���� ���� ������ �����ϴ��� Ȯ�� �ʿ�
			auto* inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
			if (inventory && inventory->AddItem(draggedItem->m_data.id, 1))
			{
				std::cout << "�ռ� �Ұ����� ��� �������� �κ��丮�� �̵�: " << draggedItem->m_data.id << std::endl;
				return true;
			}
		}
	}
	else if (source == DragSource::Enhancement)
	{
		// ��ȭâ���� �� �������̶�� �κ��丮�� ����
		auto* inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
		if (inventory && inventory->AddItem(draggedItem->m_data.id, 1))
		{
			std::cout << "�ռ� �Ұ����� ��ȭ �������� �κ��丮�� ����: " << draggedItem->m_data.id << std::endl;
			return true;
		}
	}

	return false;
}

void SynthesisWin::PerformSynthesis()
{
	// ����1�� ����2�� �������� �ִ��� Ȯ��
	Item* item1 = m_slot_Item[SynSlot::Slot1];
	Item* item2 = m_slot_Item[SynSlot::Slot2];

	if (!item1 || !item2)
	{
		std::cout << "�ռ��� ���ؼ��� �� ���Կ� ��� �������� �ʿ��մϴ�." << std::endl;
		return;
	}

	// �� ������ ��� �ռ� �������� Ȯ��

	//�̰� ���̺�� Ȯ�� �ؾ� ��. 
	
	if (!item1->m_data.synthesizable || !item2->m_data.synthesizable)
	{
		std::cout << "�ռ� �Ұ����� �������� ���ԵǾ� �ֽ��ϴ�." << std::endl;
		return;
	}

	std::cout << "�ռ� ����: " << item1->m_data.id << " + " << item2->m_data.id << std::endl;

	// ����� �ӽ÷� ù ��° �������� ����� ���


	string result = GameManager::Get().Synthesis(item1->m_data.id, item2->m_data.id);
	std::cout << "�ռ� ���: " << result;

	if (result != "F") //������ ��� 
	{
		unique_ptr<Item> resultitem  = ResourceManager::Get().Get_ItemBank().Get_Item_Status(result);

		Inventory* inven = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
		if (inven != nullptr)
		{
			inven->GetItemBase().AddItemData(std::move(resultitem));
			m_slot_Item[SynSlot::Result] = inven->GetItemBase().GetItemData(result);
			m_slot_Item[SynSlot::Slot1] = nullptr;
			m_slot_Item[SynSlot::Slot2] = nullptr;

		}

		
	}
	else
	{
		//�� ���� �� 
	}

}

void SynthesisWin::ReturnItemToInventory()
{
		auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
		if (!inventoryWindow) return;

		// �ռ�â�� ��� ���Կ� �ִ� �����۵��� �κ��丮�� ��ȯ

		if (m_slot_Item[SynSlot::Result] != nullptr)
		{
				inventoryWindow->AddItem(m_slot_Item[SynSlot::Result]->m_data.id, 1);
				m_slot_Item[SynSlot::Result] = nullptr;
		}

		if (m_slot_Item[SynSlot::Slot1] != nullptr)
		{
				inventoryWindow->AddItem(m_slot_Item[SynSlot::Slot1]->m_data.id, 1);
				m_slot_Item[SynSlot::Slot1] = nullptr;
		}

		if (m_slot_Item[SynSlot::Slot2] != nullptr)
		{
				inventoryWindow->AddItem(m_slot_Item[SynSlot::Slot2]->m_data.id, 1);
				m_slot_Item[SynSlot::Slot2] = nullptr;
		}

		std::cout << "�ռ�â �ݱ� �� ��� ������ �κ��丮�� ��ȯ �Ϸ�" << std::endl;
}

void SynthesisWin::Update()
{
	if (!m_isActive) return;
	//m_position = mousePos - m_dragOffset;
	UpdatePosition();

}

void SynthesisWin::Render() //��� �� Ÿ��Ʋ�� �� ���Ե� �� ������ �����۵� �� �ݱ� ��ư
{

	UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();

	if (uiRenderer) //���
	{
		ID2D1Bitmap1* backgroundBitmap = uiRenderer->GetBitmap("Syn_BG").Get();
		if (backgroundBitmap)
		{
			D2D1_RECT_F destRect = { m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y };
			D2DRenderer::Get().DrawBitmap(backgroundBitmap, destRect);
		}

		float rightMargin = 47;  // 65.0f���� 47.0f�� ����
		Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 7 };  // +36���� +7�� ����
		Vec2 closeButtonSize = { 35, 35 };  // 35x35���� 27x27�� ����
		D2D1_RECT_F destRect = { closeButtonPos.x, closeButtonPos.y, closeButtonPos.x + closeButtonSize.x, closeButtonPos.y + closeButtonSize.y };


		ID2D1Bitmap1* closeButtonBitmap = uiRenderer->GetBitmap("Close_But").Get();
		if (closeButtonBitmap)
		{
			D2DRenderer::Get().DrawBitmap(closeButtonBitmap, destRect);
		}



		for (const auto& [type, pos] : m_slot)
		{
			Item* item = m_slot_Item[type];

			D2D1_RECT_F DEST = { pos.x, pos.y, SlotSize + pos.x , SlotSize + pos.y };

			if (item != nullptr)
			{
				

				ID2D1Bitmap1* SlotBitmap = (type == SynSlot::Result) ? uiRenderer->GetBitmap("Syn_Result").Get() : uiRenderer->GetBitmap("Syn_Slot").Get();
				D2DRenderer::Get().DrawBitmap(SlotBitmap, DEST);

				D2DRenderer::Get().DrawBitmap(ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id).atlas.Get(),
					DEST, ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id).srcRect, 1);

			}
			else

			{
				ID2D1Bitmap1* SlotBitmap = (type == SynSlot::Result) ? uiRenderer->GetBitmap("Syn_Result").Get() : uiRenderer->GetBitmap("Syn_Slot").Get();
				D2DRenderer::Get().DrawBitmap(SlotBitmap, DEST);
			}


		}

		for (const auto& [type, pos] : m_but) //�׳�
		{
			ID2D1Bitmap1* ButtonBitmap = (type == SynButton::Cancle) ? uiRenderer->GetBitmap("Cancle_But").Get() : uiRenderer->GetBitmap("Syn_But").Get();
			D2D1_RECT_F DEST = { pos.x,pos.y,pos.x + ButX,pos.y + ButY };
			D2DRenderer::Get().DrawBitmap(ButtonBitmap, DEST);
		}


	}


}

UIWindowType SynthesisWin::GetType()
{
	return m_windowType;
}
