#include "SynthesisWin.h"
#include "UI_Renderer.h"
#include "ResourceManager.h"
#include "UIManager.h"
#include "CursorManager.h"

#include "GameManager_2.h"

SynthesisWin::SynthesisWin() :UIWindow(UIWindowType::SynthesisWindow, Vec2{ 0,0 }, Vec2{ 524,766 })
{
	//�������� ���� �˾Ƽ� �ǰ� 

	MemInit();
	MemBitmapLoad();
	InitializeObj();
}

SynthesisWin::~SynthesisWin()
{

}

bool SynthesisWin::HandleMouseHover(Vec2 mousePos) //hover? 
{
	if (!m_isActive) return false;

	MSG msg{};
	msg.message = WM_MOUSEMOVE;

	// ���, �ռ� ��ư �̹��� ��ȭ
	auto bType = ButtonInit(mousePos);
	if (bType != SynButton::Nothing)
	{
		auto btCompo = m_buttons[bType]->GetComponent<ButtonComponent>();
		btCompo->SetCollision(true);
		btCompo->Worked(msg);
	}
	else
	{
		for (auto& [tyep, bt] : m_buttons)
		{
			auto btCompo = bt->GetComponent<ButtonComponent>();
			btCompo->SetCollision(false);
			btCompo->Worked(msg);
		}
	}

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

	MSG msg{};
	msg.message = WM_LBUTTONDOWN;

	// ���, �ռ� ��ư �̹��� ��ȭ
	auto bType = ButtonInit(mousePos);
	if (bType != SynButton::Nothing)
	{
		auto btCompo = m_buttons[bType]->GetComponent<ButtonComponent>();
		btCompo->SetCollision(true);
		btCompo->Worked(msg);
	}
	else
	{
		for (auto& [tyep, bt] : m_buttons)
		{
			auto btCompo = bt->GetComponent<ButtonComponent>();
			btCompo->SetCollision(false);
			btCompo->Worked(msg);
		}
	}

	SynSlot whichSlot = SlotInit(mousePos);

	if (whichSlot != SynSlot::Nothing)
	{
		Item* Clicked_Item = m_slot_Item[whichSlot];

		if (Clicked_Item == nullptr)
			return false; // �� �����̸� �巡�� �� ��

		CursorManager::Get().StartItemDrag_NS(Clicked_Item->m_data.id, DragSource::Synthesis);
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
		SettingWindow* SETWin = dynamic_cast<SettingWindow*>(UIManager::Get().GetWindow(UIWindowType::SettingsWindow));
		if (SETWin)
		{
			float val = SETWin->GetSFXValue();
			SoundManager::Instance().PlaySFX("SY", val);
		}

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

	MSG msg{};
	msg.message = WM_LBUTTONUP;

	// ���, �ռ� ��ư �̹��� ��ȭ
	auto bType = ButtonInit(mousePos);
	if (bType != SynButton::Nothing)
	{
		auto btCompo = m_buttons[bType]->GetComponent<ButtonComponent>();
		btCompo->SetCollision(true);
		btCompo->Worked(msg);
	}
	else
	{
		for (auto& [tyep, bt] : m_buttons)
		{
			auto btCompo = bt->GetComponent<ButtonComponent>();
			btCompo->SetCollision(false);
			btCompo->Worked(msg);
		}
	}

	//  ���� �������� �巡�� ������ Ȯ��
	if (!CursorManager::Get().IsDragging()) return false;

	// �巡�� ���� �����۰� �ҽ� ���� ��������
	Item* draggedItem = CursorManager::Get().GetDraggedItem();
	DragSource dragSource = CursorManager::Get().GetDragSource();
	SynSlot whichSlot = SlotInit(mousePos);

	// Result ���Կ� ���
	if (whichSlot == SynSlot::Result && draggedItem != nullptr)
	{
			return false; // UIManager���� ���� ó��
	}
	// �Ϲ� ���Կ� ����ϴ� ���
	else if (whichSlot != SynSlot::Nothing && draggedItem != nullptr)
	{
		// �ռ� �Ұ��� �������� ���
		if (!draggedItem->m_data.synthesizable)
		{
			return false;
		}
		Item* previousItem = m_slot_Item[whichSlot];
		//ItemInitialize(draggedItem);
		m_slot_Item[whichSlot] = draggedItem;
		ItemDrop(draggedItem);

		if (previousItem)
		{
			auto* inventoryWindow = dynamic_cast<Inventory*>(
				UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
			ItemDrop(previousItem);
			inventoryWindow->AddItem(previousItem->m_data.id, 1);
		}

		CursorManager::Get().EndItemDrag();

		// ����� �������� �ٷ� hover ���·�
		CursorManager::Get().SetHoveredItem(draggedItem);
		UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, mousePos + Vec2(10, 10));

		return true;
	}
	// �ռ�â ���� ������ �� ������ ����� ���
	else if (IsInBounds(mousePos) && draggedItem != nullptr)
	{
			return false;
	}
	
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
				ItemDrop(Clicked_Item);
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

void SynthesisWin::InitializeObj()
{
	// ��� ��ư
	auto cancleBM = ResourceManager::Get().GetTexture("CancleBut");
	auto cancle = make_unique<Object>();
	auto cInfo = cancle->GetRenderInfo();
	cInfo->SetBitmap(cancleBM.Get());
	auto cbutton = cancle->AddComponent<ButtonComponent>(cInfo);
	cbutton->BitmapPush("normal", cancleBM);
	cbutton->BitmapPush("hover", cancleBM);
	cbutton->BitmapPush("pressed", cancleBM);
	cbutton->BitmapPush("disabled", cancleBM);
	
	m_buttons.emplace(SynButton::Cancle, std::move(cancle));

	// �ռ� ��ư
	auto synBM = ResourceManager::Get().GetTexture("SynBut");
	auto syn = make_unique<Object>();;
	auto sInfo = syn->GetRenderInfo();
	sInfo->SetBitmap(synBM.Get());
	auto sbutton = syn->AddComponent<ButtonComponent>(sInfo);
	sbutton->BitmapPush("normal", synBM);
	sbutton->BitmapPush("hover", synBM);
	sbutton->BitmapPush("pressed", synBM);
	sbutton->BitmapPush("disabled", synBM);

	m_buttons.emplace(SynButton::Syn, std::move(syn));
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

				D2DRenderer::Get().DrawBitmap(ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id)->atlas.Get(),
					DEST, ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id)->srcRect, 1);
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

	// �ҽ����� ������ ��ġ�� ����
	auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
	if (!inventoryWindow) return false;

	// �巡�� �ҽ��� �ռ�â�� ��� �κ��丮�� ����
	if (source == DragSource::Equipment || source == DragSource::Inventory || source == DragSource::Enhancement || source == DragSource::Synthesis)
	{
		ItemDrop(draggedItem);

		if (inventoryWindow->AddItem(draggedItem->m_data.id, 1))
		{
			std::cout << "�ռ� �Ұ����� �������� �κ��丮�� ����: " << draggedItem->m_data.id << std::endl;
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
		SettingWindow* SETWin = dynamic_cast<SettingWindow*>(UIManager::Get().GetWindow(UIWindowType::SettingsWindow));
		if (SETWin)
		{
			float val = SETWin->GetSFXValue();
			SoundManager::Instance().PlaySFX("SS", val);
		}
		unique_ptr<Item> resultitem = ResourceManager::Get().Get_ItemBank().Get_Item_Status(result);

		//resultitem.get()->AddComponent<> ���⼭ �ǵ鿩�� �ѹ� ���� ��Ű�� ���ֹ����°� ������.
		Inventory* inven = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
		if (inven != nullptr)
		{
			inven->GetItemBase().AddItemData(std::move(resultitem));
			m_slot_Item[SynSlot::Result] = inven->GetItemBase().GetItemData(result);
			//ItemInitialize(m_slot_Item[SynSlot::Result]);
			m_slot_Item[SynSlot::Slot1] = nullptr;
			m_slot_Item[SynSlot::Slot2] = nullptr;
		}


	}
	else
	{
		SettingWindow* SETWin = dynamic_cast<SettingWindow*>(UIManager::Get().GetWindow(UIWindowType::SettingsWindow));
		if (SETWin)
		{
			float val = SETWin->GetSFXValue();
			SoundManager::Instance().PlaySFX("SN", val);
		}

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

void SynthesisWin::ItemInitialize(Item* item)
{
	item->ComponentClear();
	auto rayBM = ResourceManager::Get().GetTexture("treasureRay");
	auto glowBM = ResourceManager::Get().GetTexture("basicglow");
	auto runeBM = ResourceManager::Get().GetTexture("runeword02");
	//auto comBM1 = ResourceManager::Get().GetTexture("readyattac00");
	//auto comBM2 = ResourceManager::Get().GetTexture("readyattac01");
	//auto comBM3 = ResourceManager::Get().GetTexture("readyattac02");
	//auto comBM4 = ResourceManager::Get().GetTexture("readyattac03");
	//auto comBM5 = ResourceManager::Get().GetTexture("readyattac04");
	//auto comBM6 = ResourceManager::Get().GetTexture("readyattac05");
	//auto comBM7 = ResourceManager::Get().GetTexture("readyattac06");
	//auto comBM8 = ResourceManager::Get().GetTexture("readyattac07");
	//auto comBM9 = ResourceManager::Get().GetTexture("readyattac08");
	//auto comBM10 = ResourceManager::Get().GetTexture("readyattac09");
	//auto comBM11 = ResourceManager::Get().GetTexture("readyattac10");
	//auto comBM12 = ResourceManager::Get().GetTexture("readyattac11");
	//auto comBM13 = ResourceManager::Get().GetTexture("readyattac12");
	//auto comBM14 = ResourceManager::Get().GetTexture("readyattac13");
	//auto comBM15 = ResourceManager::Get().GetTexture("readyattac14");
	//auto comBM16 = ResourceManager::Get().GetTexture("readyattac15");
	//auto comBM17 = ResourceManager::Get().GetTexture("readyattac16");
	//auto comBM18 = ResourceManager::Get().GetTexture("readyattac17");
	//auto comBM19 = ResourceManager::Get().GetTexture("readyattac18");
	//auto comBM20 = ResourceManager::Get().GetTexture("readyattac19");
	//auto comBM21 = ResourceManager::Get().GetTexture("readyattac20");
	//auto comBM22 = ResourceManager::Get().GetTexture("readyattac21");
	//auto comBM23 = ResourceManager::Get().GetTexture("readyattac22");
	//auto comBM24 = ResourceManager::Get().GetTexture("readyattac23");
	//auto comBM25 = ResourceManager::Get().GetTexture("readyattac24");

	item->ComponentClear();
	item->GetTransform().SetScale({ 0.8f, 0.8f });
	auto info = item->GetRenderInfo();

	auto ray1 = item->AddComponent<Rotate3D_Effect>(info, 0.f, 0.f, 0.f, 0.2f, rayBM.Get()); // 0
	auto ray2 = item->AddComponent<Rotate3D_Effect>(info, 0.f, 0.f, 0.f, -0.2f, rayBM.Get());	// 1
	auto rayComposite = item->AddComponent<Composite_Effect>(info, ray1->GetEffect(), ray2->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER); //2
	auto rayBlur = item->AddComponent<GaussianBlur_Effect>(info, 2.f, rayComposite->GetEffect()); // 3
	auto increase = item->AddComponent<Increasing_Effect>(info, D2D1_POINT_2F{ rayBM->GetSize().width / 2.f, rayBM->GetSize().height / 2.f }, 1.5f, 1.f, rayBlur->GetEffect()); // 4
	auto offsetRay = item->AddComponent<Offset_Effect>(info, info->GetRenderInfo().srcRect.left, 0.f, increase->GetEffect()); // 5

	auto glowColor = item->AddComponent<Color_Effect>(info, 1.f, 1.0f, 1.0f, 0.6f, glowBM.Get()); // 6
	auto glowMove = item->AddComponent<Sideway_Effect>(info, 2.f, 0.02f, 0.02f, glowColor->GetEffect()); // 7
	auto glowOffset = item->AddComponent<Offset_Effect>(info, info->GetRenderInfo().srcRect.left, 0.f, glowMove->GetEffect()); // 8

	auto runeRo = item->AddComponent<Rotate3D_Effect>(info, 0.f, 0.f, 0.f, 0.2f, runeBM.Get()); // 9
	auto runeOffset = item->AddComponent<Offset_Effect>(info, info->GetRenderInfo().srcRect.left, 0.f, runeRo->GetEffect()); // 10

	auto glowruneCompo = item->AddComponent<Composite_Effect>(info, runeOffset->GetEffect(), glowOffset->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER); // 11
	auto fade = item->AddComponent<Fade_Effect>(info, 0.f, 0.9f, 0.005f, glowruneCompo->GetEffect()); // 12
	fade->OnEvent("SHOW");

	auto rayGlowruneCompo = item->AddComponent<Composite_Effect>(info, fade->GetEffect(), offsetRay->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER); // 13

	//auto itemOpacity = item->AddComponent<Opacity_Effect>(info, 1.f, info->GetBitmap()); // 14
	auto behindcompo = item->AddComponent<Composite_Effect>(info, info->GetBitmap(), rayGlowruneCompo->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER); // 15

	//auto fail = item->AddComponent<Clip_Effect>(info, fail1BM.Get(), fail2BM.Get(), fail3BM.Get(), fail4BM.Get(), fail5BM.Get()); // 16
	//auto failRotate = item->AddComponent<Rotate3D_Effect>(info, 0.f, 0.f, 0.f, 3.f, fail->GetEffect()); // 17
	//auto failOffset = item->AddComponent<Offset_Effect>(info, info->GetRenderInfo().srcRect.left, 0.f, failRotate->GetEffect()); // 18
	//auto failBlur1 = item->AddComponent<GaussianBlur_Effect>(info, 1.f, failOffset->GetEffect()); // 19
	//auto failBlur2 = item->AddComponent<GaussianBlur_Effect>(info, 3.f, failOffset->GetEffect()); // 20
	//auto failBlurcompo = item->AddComponent<Composite_Effect>(info, failBlur1->GetEffect(), failBlur2->GetEffect(), D2D1_COMPOSITE_MODE_PLUS); // 21
	//auto frontcompo = item->AddComponent<Composite_Effect>(info, failBlurcompo->GetEffect(), behindcompo->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER); // 22
}

void SynthesisWin::Update()
{
	if (!m_isActive) return;
	//m_position = mousePos - m_dragOffset;
	UpdatePosition();
	for (auto& [type, item] : m_slot_Item)
	{
		if (item == nullptr) continue;
		item->Update();
	}
}

void SynthesisWin::FixedUpdate(float dt)
{
	if (!m_isActive) return;

	for (auto& [type, item] : m_slot_Item)
	{
		if (item == nullptr) continue;
		item->Update();
	}
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

		float rightMargin = 65;  // 65.0f���� 47.0f�� ����
		Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 35 };  // +36���� +7�� ����
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

				D2D_RECT_F src = ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id)->srcRect;
				src.left = src.left + 80.f;
				src.top = src.top + 80.f;
				src.right = src.right - 80.f;
				src.bottom = src.bottom - 80.f;

				D2DRenderer::Get().DrawBitmap(ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id)->atlas.Get(),
					DEST, src, 1);

				//auto info = item->GetRenderInfo();
				//item->GetTransform().SetPosition({ pos.x - 91.f * 0.8f + 8.f, pos.y - 91.f * 0.8f + 8.f });
				//D2DRenderer::Get().DrawBitmap(info->GetRenderInfo());
			}
			else

			{
				ID2D1Bitmap1* SlotBitmap = (type == SynSlot::Result) ? uiRenderer->GetBitmap("Syn_Result").Get() : uiRenderer->GetBitmap("Syn_Slot").Get();
				D2DRenderer::Get().DrawBitmap(SlotBitmap, DEST);
			}


		}

		for (const auto& [type, pos] : m_but) //�׳�
		{
			//ID2D1Bitmap1* ButtonBitmap = (type == SynButton::Cancle) ? uiRenderer->GetBitmap("Cancle_But").Get() : uiRenderer->GetBitmap("Syn_But").Get();
			auto info = m_buttons[type]->GetRenderInfo();
			D2D1_RECT_F DEST = { pos.x,pos.y,pos.x + ButX,pos.y + ButY };
			info->SetDestRect(DEST);
			//D2DRenderer::Get().DrawBitmap(ButtonBitmap, DEST);
			D2DRenderer::Get().DrawBitmap(info->GetRenderInfo());
		}


	}


}

UIWindowType SynthesisWin::GetType()
{
	return m_windowType;
}
