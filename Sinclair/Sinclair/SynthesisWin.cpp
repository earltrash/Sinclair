#include "SynthesisWin.h"
#include "UI_Renderer.h"
#include "ResourceManager.h"
#include "UIManager.h"
#include "CursorManager.h"

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
	}

	// ���Կ� ���콺 ���� �� ���� ǥ��
	SynSlot whichSlot = SlotInit(mousePos);
	if (whichSlot != SynSlot::Nothing)
	{
		Item* Clicked_Item = m_slot_Item[whichSlot];
		// ���� ǥ�� ��ġ�� ���콺 ������ ����
		Vec2 tooltipPos = mousePos + Vec2(10, 10);
		UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, tooltipPos);
		return true;

	}
		
	else
	{
		// ���콺�� ���� ������ ������ ���� �����
		UIManager::Get().HideTooltip(UIWindowType::InventoryTooltip);
	}

	return false;
}

bool SynthesisWin::HandleMouseDown(Vec2 mousePos) //������ �����̴� �� // slot position �ᱹ ��ġ���̶� 
{

	if (!m_isActive) return false;

	SynSlot whichSlot = SlotInit(mousePos);
	if (whichSlot != SynSlot::Nothing)
	{
		Item* Clicked_Item = m_slot_Item[whichSlot];

			CursorManager::Get().StartItemDrag_NS(Clicked_Item->m_data.id , DragSource::Equipment);
			CursorManager::Get().SetDraggedItem(Clicked_Item);

			return true;
	}

	SynButton whichBut = ButtonInit(mousePos);
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

		return true;
	}
	else
	{
		//�� �ռ��̰��� 

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
	if (CursorManager::Get().IsDragging())
	{
		// �巡�� ���� �����۰� �ҽ� ���� ��������
		Item* draggedItem = CursorManager::Get().GetDraggedItem();
		DragSource dragSource = CursorManager::Get().GetDragSource();
		SynSlot whichSlot = SlotInit(mousePos);


		if (whichSlot != SynSlot::Nothing && draggedItem->m_data.synthesizable)
		{
			Item* previousItem = m_slot_Item[whichSlot]; // ���� ���Կ� �ִ� ������
			m_slot_Item[whichSlot] = draggedItem;

			// ���� �������� �־��ٸ� �κ��丮�� ����.
			if (previousItem)
			{
				auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
				inventoryWindow->AddItem(previousItem->m_data.id, 1);

				std::cout << "DEBUG: ���� �������� �־� �κ��丮�� ������Ŵ." << std::endl;
			}
			// �巡�� ����.
			CursorManager::Get().EndItemDrag();
			return true;
		}
		else //�������� Slot�̰ų�, �ٸ� â�� ��쵵 ���� ����? 
		{
			CursorManager::Get().EndItemDrag();

			return HandleDropFailure(mousePos, draggedItem, dragSource);

		}
	}

	//  â ���� Ŭ�� �� �ֻ������ �ø��� (�巡�� ��� ���� ���Ŀ� ó��)

	//if (IsInBounds(mousePos)) // -> ��� �� �ִ� ����?
	//{
	//	UIManager::Get().OpenWindow(m_windowType);
	//	return true;
	//}
	return false;
}

bool SynthesisWin::HandleDoubleClick(Vec2 mousePos) //swap ����? sort�� �ʿ��� �� ����. Inven�� ��Ȱ �Ǿ�� ���� �����ؾ� �� �� ���� 
{
	return false;
}

void SynthesisWin::UpdatePosition()
{
	m_slot[SynSlot::Result] = { m_position.x+ 226,m_position.y + 140 };
	m_slot[SynSlot::Slot1] = { m_position.x + 92 , m_position.y + 419 };
	m_slot[SynSlot::Slot2] = { m_position.x + 359 ,m_position.y + 419 };
	 
	m_but[SynButton::Cancle] = { m_position.x + 52,m_position.y + 626 };
	m_but[SynButton::Syn] = { m_position.x + 286,m_position.y + 626 };




	//for (auto& [type, relPos] : m_slot)
	//	m_slot[type] = m_position + relPos;

	//for (auto& [type, relPos] : m_but)
	//	m_but[type] = m_position + relPos;


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
	return false;
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

			D2D1_RECT_F DEST = { pos.x, pos.y, SlotSize + pos.x , SlotSize + pos.y };

			if (item != nullptr)
			{
				D2DRenderer::Get().DrawBitmap(ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id).atlas.Get(),
					DEST, ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id).srcRect, 1);
			}
			else
			
			{
				ID2D1Bitmap1* closeButtonBitmap = (type == SynSlot::Result) ? uiRenderer->GetBitmap("Syn_Result").Get() : uiRenderer->GetBitmap("Syn_Slot").Get();
				D2DRenderer::Get().DrawBitmap(closeButtonBitmap, DEST);
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

UIWindowType SynthesisWin::GetType()
{
	return m_windowType;
}
