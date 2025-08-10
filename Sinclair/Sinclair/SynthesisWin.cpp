#include "SynthesisWin.h"
#include "UI_Renderer.h"
#include "ResourceManager.h"
#include "UIManager.h"
#include "CursorManager.h"

#include "GameManager.h"

SynthesisWin::SynthesisWin() :UIWindow(UIWindowType::SynthesisWindow, Vec2{ 0,0 }, Vec2{ 524,766 })
{
	//포지션은 ㅇㅇ 알아서 되고 

	MemInit();
	MemBitmapLoad();
}

SynthesisWin::~SynthesisWin()
{

}

bool SynthesisWin::HandleMouseHover(Vec2 mousePos) //hover? 
{
	if (!m_isActive) return false;


	// 드래그 중이면 창 위치 업데이트
	if (m_isDragging)
	{
		m_position = mousePos - m_dragOffset;

		// 창 위치가 바뀌면 슬롯 위치들도 다시 계산
		UpdatePosition();
		return true;
	}

	else 
	{
		// 슬롯에 마우스 오버 시 툴팁 표시
		SynSlot whichSlot = SlotInit(mousePos);

		if (whichSlot != SynSlot::Nothing)
		{
			Item* Clicked_Item = m_slot_Item[whichSlot];
			if (Clicked_Item != nullptr)
			{
				CursorManager::Get().SetHoveredItem(Clicked_Item);
				Vec2 tooltipPos = mousePos + Vec2(10, 10);

				UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, tooltipPos); //위치 변경시키고, 활성화까지 
				return true;
			}
			else
				return true;

		}

		else/* if(whichSlot == SynSlot::Nothing)*/
		{
			CursorManager::Get().HoveredReleased(); //추적 금지 
			return false;
		}
	}
	

	CursorManager::Get().HoveredReleased(); //추적 금지 
	UIManager::Get().CloseWindow(UIWindowType::InventoryTooltip); //해제
	return false;
}

bool SynthesisWin::HandleMouseDown(Vec2 mousePos) //아이템 움직이는 거 // slot position 결국 위치값이랑 
{

	if (!m_isActive) return false;

	SynSlot whichSlot = SlotInit(mousePos);

	if (whichSlot != SynSlot::Nothing)
	{
		Item* Clicked_Item = m_slot_Item[whichSlot];

		if (Clicked_Item == nullptr)
			return false; // 빈 슬롯이면 드래그 안 함

		CursorManager::Get().StartItemDrag_NS(Clicked_Item->m_data.id, DragSource::Equipment);
		CursorManager::Get().SetDraggedItem(Clicked_Item);

		if (whichSlot == SynSlot::Result) //아이템이 있는 result 슬롯을 누르면 다 반환시킨다라 
		{
			m_slot_Item[SynSlot::Result] = nullptr;
			m_slot_Item[SynSlot::Slot1] = nullptr;
			m_slot_Item[SynSlot::Slot2] = nullptr;
		}
		else //아닌 경우에는, 그치 기존 슬롯에 있던거 빼야지 
		{
			m_slot_Item[whichSlot] = nullptr;
		}
		return true; // 드래그 시작 시에는 무조건 true 반환
	}

	SynButton whichBut = ButtonInit(mousePos);

	// cancel 버튼 클릭 시 슬롯 정리.
	if (whichBut != SynButton::Nothing && whichBut == SynButton::Cancle)
	{

		for (auto& [type, pos] : m_slot)
		{
			Item* item = m_slot_Item[type];

			if (item != nullptr)
			{
				auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
				inventoryWindow->AddItem(item->m_data.id, 1); //안 쓸거면 인벤에 넣을래요 


			}
		}
		// cancel 누르면 둘 다 반환. 
		m_slot_Item[SynSlot::Result] = nullptr;
		m_slot_Item[SynSlot::Slot1] = nullptr;
		m_slot_Item[SynSlot::Slot2] = nullptr;

		return true;
	}
	else if (whichBut != SynButton::Nothing && whichBut == SynButton::Syn)
	{
		//뭐 합성이겠지 
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

bool SynthesisWin::HandleMouseUp(Vec2 mousePos) //내려놓을 때의 처리 
{
	if (!m_isActive) return false;

	//  현재 아이템을 드래그 중인지 확인
	if (!CursorManager::Get().IsDragging()) return false;
	
		// 드래그 중인 아이템과 소스 정보 가져오기
		Item* draggedItem = CursorManager::Get().GetDraggedItem();
		DragSource dragSource = CursorManager::Get().GetDragSource();
		SynSlot whichSlot = SlotInit(mousePos);


		if (whichSlot == SynSlot::Result && draggedItem != nullptr)
		{
			auto* inventoryWindow = dynamic_cast<Inventory*>(
				UIManager::Get().GetWindow(UIWindowType::InventoryWindow));

			// 기존 아이템 복귀
			Item* previousItem = m_slot_Item[whichSlot];
			if (previousItem != nullptr) {
				inventoryWindow->AddItem(previousItem->m_data.id, 1);
			}

			// 드롭한 아이템도 복귀
			inventoryWindow->AddItem(draggedItem->m_data.id, 1);

			// 결과 슬롯 비우기
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

			// 드롭한 아이템을 바로 hover 상태로
			CursorManager::Get().SetHoveredItem(draggedItem);
			UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, mousePos + Vec2(10, 10));

			return true;
		}

		


		else //부적절한 Slot이거나, 다른 창인 경우도 봐야 겠죠? 
		{
			CursorManager::Get().EndItemDrag();
			return HandleDropFailure(mousePos, draggedItem, dragSource);
		}
	

	CursorManager::Get().HoveredReleased();
	return false;
}

bool SynthesisWin::HandleDoubleClick(Vec2 mousePos) //swap 정도? sort도 필요할 거 같음. Inven이 비활 되었어도 접근 가능해야 할 듯 ㅇㅇ 
{
	return false;
}

bool SynthesisWin::HandleMouseRight(Vec2 mousePos) //slot 위치에 mousepos -> 1 2 번째인 경우에만. 
{
	
	SynSlot whichSlot = SlotInit(mousePos); //마우스 오른쪽 클릭된 슬롯 

	if (whichSlot != SynSlot::Nothing)
	{
		Item* Clicked_Item = m_slot_Item[whichSlot]; 

		if (Clicked_Item != nullptr)
		{
			//기존 슬롯에 있던 
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
		// 필요한 UI 비트맵들을 ResourceManager에서 로드하여 UI_Renderer에 설정
		// ResourceManager가 해당 키로 비트맵을 로드
		uiRenderer->SetBitmap("Syn_BG", ResourceManager::Get().GetTexture("SynBg"));
		uiRenderer->SetBitmap("Close_But", ResourceManager::Get().GetTexture("CloseButton"));

		uiRenderer->SetBitmap("Syn_Result", ResourceManager::Get().GetTexture("SynResult"));
		uiRenderer->SetBitmap("Syn_Slot", ResourceManager::Get().GetTexture("SlotNormal"));

		uiRenderer->SetBitmap("Cancle_But", ResourceManager::Get().GetTexture("CancleBut"));
		uiRenderer->SetBitmap("Syn_But", ResourceManager::Get().GetTexture("SynBut"));
	}
}

void SynthesisWin::MemInit() //0 1 2 -> 위치 넣어주고. 0은 제외하고 넣어주는 식으로 해야겠다.  
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

	if (uiRenderer) //배경
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

		for (const auto& [type, pos] : m_but) //그냥
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

	// 다른 창 내부에 있는지 확인
	bool isInOtherWindow = false;

	// 인벤토리 창 내부 확인
	UIWindow* inventoryWindow = UIManager::Get().GetWindow(UIWindowType::InventoryWindow);
	if (inventoryWindow && inventoryWindow->IsActive() && inventoryWindow->IsInBounds(mousePos))
	{
		isInOtherWindow = true;
	}

	// 장비 창 내부 확인
	UIWindow* equipmentWindow = UIManager::Get().GetWindow(UIWindowType::EquipmentWindow);
	if (equipmentWindow && equipmentWindow->IsActive() && equipmentWindow->IsInBounds(mousePos))
	{
		isInOtherWindow = true;
	}

	// 강화 창 내부 확인
	UIWindow* enhancementWindow = UIManager::Get().GetWindow(UIWindowType::EnhancementWindow);
	if (enhancementWindow && enhancementWindow->IsActive() && enhancementWindow->IsInBounds(mousePos))
	{
		isInOtherWindow = true;
	}

	// 다른 창 내부라면 해당 창에서 처리하도록 넘김
	if (isInOtherWindow)
	{
		return false;
	}

	// 어느 창 내부도 아니면 원래 위치로 복귀
	if (source == DragSource::Inventory)
	{
		// 인벤토리에서 온 아이템이라면 인벤토리로 복귀
		auto* inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
		if (inventory && inventory->AddItem(draggedItem->m_data.id, 1))
		{
			std::cout << "합성 불가능한 아이템을 인벤토리로 복귀: " << draggedItem->m_data.id << std::endl;
			return true;
		}
	}
	else if (source == DragSource::Equipment)
	{
		// 장비창에서 온 아이템이라면 장비창으로 복귀
		auto* equipment = dynamic_cast<EquipmentWindow*>(UIManager::Get().GetWindow(UIWindowType::EquipmentWindow));
		if (equipment)
		{
			// 장비창에 원래 슬롯이 있다면 해당 슬롯으로 복귀
			// 현재 CursorManager에서 원래 슬롯 정보를 관리하는지 확인 필요
			auto* inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
			if (inventory && inventory->AddItem(draggedItem->m_data.id, 1))
			{
				std::cout << "합성 불가능한 장비 아이템을 인벤토리로 이동: " << draggedItem->m_data.id << std::endl;
				return true;
			}
		}
	}
	else if (source == DragSource::Enhancement)
	{
		// 강화창에서 온 아이템이라면 인벤토리로 복귀
		auto* inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
		if (inventory && inventory->AddItem(draggedItem->m_data.id, 1))
		{
			std::cout << "합성 불가능한 강화 아이템을 인벤토리로 복귀: " << draggedItem->m_data.id << std::endl;
			return true;
		}
	}

	return false;
}

void SynthesisWin::PerformSynthesis()
{
	// 슬롯1과 슬롯2에 아이템이 있는지 확인
	Item* item1 = m_slot_Item[SynSlot::Slot1];
	Item* item2 = m_slot_Item[SynSlot::Slot2];

	if (!item1 || !item2)
	{
		std::cout << "합성을 위해서는 두 슬롯에 모두 아이템이 필요합니다." << std::endl;
		return;
	}

	// 두 아이템 모두 합성 가능한지 확인

	//이건 테이블로 확인 해야 함. 
	
	if (!item1->m_data.synthesizable || !item2->m_data.synthesizable)
	{
		std::cout << "합성 불가능한 아이템이 포함되어 있습니다." << std::endl;
		return;
	}

	std::cout << "합성 수행: " << item1->m_data.id << " + " << item2->m_data.id << std::endl;

	// 현재는 임시로 첫 번째 아이템을 결과로 사용


	string result = GameManager::Get().Synthesis(item1->m_data.id, item2->m_data.id);
	std::cout << "합성 결과: " << result;

	if (result != "F") //성공인 경우 
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
		//뭐 없긴 함 
	}

}

void SynthesisWin::ReturnItemToInventory()
{
		auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
		if (!inventoryWindow) return;

		// 합성창의 모든 슬롯에 있는 아이템들을 인벤토리로 반환

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

		std::cout << "합성창 닫기 전 모든 아이템 인벤토리로 반환 완료" << std::endl;
}

void SynthesisWin::Update()
{
	if (!m_isActive) return;
	//m_position = mousePos - m_dragOffset;
	UpdatePosition();

}

void SynthesisWin::Render() //배경 → 타이틀바 → 슬롯들 → 장착된 아이템들 → 닫기 버튼
{

	UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();

	if (uiRenderer) //배경
	{
		ID2D1Bitmap1* backgroundBitmap = uiRenderer->GetBitmap("Syn_BG").Get();
		if (backgroundBitmap)
		{
			D2D1_RECT_F destRect = { m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y };
			D2DRenderer::Get().DrawBitmap(backgroundBitmap, destRect);
		}

		float rightMargin = 47;  // 65.0f에서 47.0f로 변경
		Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 7 };  // +36에서 +7로 변경
		Vec2 closeButtonSize = { 35, 35 };  // 35x35에서 27x27로 변경
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

		for (const auto& [type, pos] : m_but) //그냥
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
