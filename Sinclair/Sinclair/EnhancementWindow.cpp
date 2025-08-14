#include "EnhancementWindow.h"
#include "ButtonComponent.h"

#include <windows.h>
#undef min
#undef max
#include <algorithm>

#include "Wearable.h"

void EnhancementWindow::Update()
{
	// ���⿡ ������Ʈ ��� ���� component update ��Ű��.

	if (m_targetItem == nullptr)	return;

	auto clip = m_targetItem->GetComponent<Clip_Effect>();				// target item�� ���� ȿ�� ������Ʈ
	auto failRotate = m_targetItem->GetComponent<Rotate3D_Effect>(17);
	failRotate->SetInputEffect(clip->GetEffect());
	auto failOffset = m_targetItem->GetComponent<Offset_Effect>(18);
	failOffset->SetInputEffect(failRotate->GetEffect());
	auto failBlur1 = m_targetItem->GetComponent<GaussianBlur_Effect>(19);
	failBlur1->SetInputEffect(failOffset->GetEffect());
	auto failBlur2 = m_targetItem->GetComponent<GaussianBlur_Effect>(20);
	failBlur2->SetInputEffect(failOffset->GetEffect());
	auto failBlurCompo = m_targetItem->GetComponent<Composite_Effect>(21);
	failBlurCompo->SetInputEffect(1, failBlur1->GetEffect());
	failBlurCompo->SetInputEffect(0, failBlur2->GetEffect());
	auto frontcompo = m_targetItem->GetComponent<Composite_Effect>(22);
	frontcompo->SetInputEffect(1, failBlurCompo->GetEffect());

	m_targetItem->Update();

	for (auto& sheets : m_normalImages)
	{
		sheets->Update();
	}

	for (auto& sheets : m_successImages)
	{
		sheets->Update();
	}

	for (auto& sheets : m_failImages)
	{
		sheets->Update();
	}

	Check4ItemEffect();

	Wearable* wearableItem = dynamic_cast<Wearable*>(m_targetItem);
	auto enchResults = wearableItem->GetEnchancResult();
	for (int i = 0; i < 5; i++)
	{
		auto explode = m_normalImages[i]->GetComponent<Explode_Effect>();	// sheetȿ�� ������Ʈ
		auto increase = m_targetItem->GetComponent<Increasing_Effect>();	// target item�� ���� ȿ�� ������Ʈ
		
		auto opacity = m_targetItem->GetComponent<Opacity_Effect>();

		if (explode->GetScale().x <= 0.f && explode->GetScale().y <= 0.f && explode != nullptr)
		{
			explode->OnEvent("DORMANT");
			
			opacity->SetOpacity(1.f);
			// ���º� ��Ʈ�� ����
			if (m_targetItem->m_data.wearablePart == Wearable_part::Weapon)
			{
				switch (enchResults[i])
				{
				case EnchancerType::Sucess:
					m_normalImages[i]->GetRenderInfo()->SetisActive(false);
					m_successImages[i]->GetRenderInfo()->SetisActive(true);
					m_failImages[i]->GetRenderInfo()->SetisActive(false);
					increase->OnEvent("PLAY");
					break;
				case EnchancerType::Fail:
					m_normalImages[i]->GetRenderInfo()->SetisActive(false);
					m_successImages[i]->GetRenderInfo()->SetisActive(false);
					m_failImages[i]->GetRenderInfo()->SetisActive(true);
					if(increase->GetCurrentState() == "PLAY")
						increase->OnEvent("DORMANT");
					clip->OnEvent("PLAY");
					break;
				}
			}
			else if (i > 0 && i < 4)
			{
				switch (enchResults[i - 1])
				{
				case EnchancerType::Sucess:
					m_normalImages[i]->GetRenderInfo()->SetisActive(false);
					m_successImages[i]->GetRenderInfo()->SetisActive(true);
					m_failImages[i]->GetRenderInfo()->SetisActive(false);
					increase->OnEvent("PLAY");
					break;
				case EnchancerType::Fail:
					m_normalImages[i]->GetRenderInfo()->SetisActive(false);
					m_successImages[i]->GetRenderInfo()->SetisActive(false);
					m_failImages[i]->GetRenderInfo()->SetisActive(true);
					if (increase->GetCurrentState() == "PLAY")
						increase->OnEvent("DORMANT");
					clip->OnEvent("PLAY");
					break;
				}
			}
		}
		else if (m_targetItem->m_data.wearablePart == Wearable_part::Weapon)
		{
			switch (enchResults[i])
			{
			case EnchancerType::Default:
				m_normalImages[i]->GetRenderInfo()->SetisActive(true);
				m_successImages[i]->GetRenderInfo()->SetisActive(false);
				m_failImages[i]->GetRenderInfo()->SetisActive(false);
				break;
			}
		}
		else if (i > 0 && i < 4)
		{
			switch (enchResults[i - 1])
			{
			case EnchancerType::Default:
				m_normalImages[i]->GetRenderInfo()->SetisActive(true);
				m_successImages[i]->GetRenderInfo()->SetisActive(false);
				m_failImages[i]->GetRenderInfo()->SetisActive(false);
				break;
			}
		}
	}
}

float EnhancementWindow::Timer(float dt)
{
	time += dt;
	if (time >= 1.f)
		time = 0.f;		// ó�� dt�� 1�̻� ū ���� ����
	else
		time -= FPS60;
	return FPS60;
}

void EnhancementWindow::FixedUpdate(float dt)
{
	if (m_targetItem == nullptr)	return;

	m_targetItem->FixedUpdate(dt);

	auto increase = m_targetItem->GetComponent<Increasing_Effect>();
	if (increase->GetScale().x != 0 && increase->GetisStop())	// increase != nullptr && 
	{
		x += dt;
		if (x >= 1.f)
		{
			increase->OnEvent("DISAPPEAR");
			x = 0;
		}
	}

	for (auto& sheets : m_normalImages)
	{
		sheets->FixedUpdate(dt);
	}

	for (auto& sheets : m_successImages)
	{
		sheets->FixedUpdate(dt);
	}

	for (auto& sheets : m_failImages)
	{
		sheets->FixedUpdate(dt);
	}

}

void EnhancementWindow::Render()
{
		if (!m_isActive) return;

		RenderBackground();
		RenderEnhancementSlot();
		RenderSheetImages();
		RenderStatSelectionButtons();
		RenderStatText();
		RenderScrollButtons();
		RenderCloseButton();
}

bool EnhancementWindow::HandleMouseDown(Vec2 mousePos)
{

	if (!m_isActive) return false;

		// �޼��� ���� ������.
		MSG msg{};
		msg.message = WM_LBUTTONDOWN;

		// �����ǥ�� �̹� ����� ���� ���ļ� �ڵ������� ��������
		Vec2 relativePos = mousePos - m_position;
		msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);

		std::cout << "HandleMouseDown - ���콺 ��ġ: (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
	/*	if (m_statSelectionButton)
		{
			Vec2 btnPos = m_statSelectionButton->GetTransform().GetPosition();
			Vec2 btnScreenPos = m_position + btnPos;
			std::cout << "���� ��ư ȭ�� ��ġ: (" << btnScreenPos.x << ", " << btnScreenPos.y << ")" << std::endl;
		}
		if (m_leftArrowButton)
		{
			Vec2 btnPos = m_leftArrowButton->GetTransform().GetPosition();
			Vec2 btnScreenPos = m_position + btnPos;
			std::cout << "left ��ư ȭ�� ��ġ: (" << btnScreenPos.x << ", " << btnScreenPos.y << ")" << std::endl;
		}
		if (m_rightArrowButton)
		{
			Vec2 btnPos = m_rightArrowButton->GetTransform().GetPosition();
			Vec2 btnScreenPos = m_position + btnPos;
			std::cout << "right ��ư ȭ�� ��ġ: (" << btnScreenPos.x << ", " << btnScreenPos.y << ")" << std::endl;
		}*/
		// ���� ��ư ����.
	if (IsMouseOverObject(mousePos, m_statSelectionButton.get()))
	{
		msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
		m_statSelectionButton->GetComponent<ButtonComponent>()->CheckCollision(msg);  // �浹 �˻� ����
		m_statSelectionButton->GetComponent<ButtonComponent>()->Worked(msg);          // �� ���� ��ư ó��


		SettingWindow* SETWin = dynamic_cast<SettingWindow*>(UIManager::Get().GetWindow(UIWindowType::SettingsWindow));
		if (SETWin)
		{
			float val = SETWin->GetSFXValue();
			SoundManager::Instance().PlaySFX("EH", val);
		}

		//SoundManager::Instance().PlaySFX("EH");
	}

	// left ��ư ����.
	if (IsMouseOverObject(mousePos, m_leftArrowButton.get()))
	{
		msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
		m_leftArrowButton->GetComponent<ButtonComponent>()->CheckCollision(msg);  // �浹 �˻� ����
		m_leftArrowButton->GetComponent<ButtonComponent>()->Worked(msg);          // �� ���� ��ư ó��
	}

	// right ��ư ����.
	if (IsMouseOverObject(mousePos, m_rightArrowButton.get()))
	{
		msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
		m_rightArrowButton->GetComponent<ButtonComponent>()->CheckCollision(msg);  // �浹 �˻� ����
		m_rightArrowButton->GetComponent<ButtonComponent>()->Worked(msg);          // �� ���� ��ư ó��
	}

	// �ֹ��� ��ư ����.
	for (auto& btn : m_enhancementButtons)
	{
		if (IsMouseOverObject(mousePos, btn.get()))
		{
			msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
			btn->GetComponent<ButtonComponent>()->CheckCollision(msg);
			btn->GetComponent<ButtonComponent>()->Worked2(msg);          // �� ���� ��ư ó��
		}

	}

	// ���� ���� üũ�ؼ� �ؾ���.
	Vec2 slotScreen = m_position + m_enhancementSlot->GetTransform().GetPosition();
	ID2D1Bitmap1* slotBmp = m_enhancementSlot->GetRenderInfo()->GetRenderInfo().bitmap;
	D2D1_SIZE_F size = slotBmp->GetSize();

	if (mousePos.x >= slotScreen.x && mousePos.x <= slotScreen.x + size.width &&
		mousePos.y >= slotScreen.y && mousePos.y <= slotScreen.y + size.height)
	{
		if (m_targetItem != nullptr)
		{
			// ���Կ� �ִ� ���������� �巡�� ����
			CursorManager::Get().StartItemDrag(m_targetItem->m_data.id, DragSource::Enhancement);
			CursorManager::Get().SetDraggedItem(m_targetItem);

			// ���Կ��� ������ ����
			m_targetItem = nullptr;

			// ��Ʈ �̹��� �ʱ�ȭ
			m_renderSheetCount = 0;

			// �ֹ��� ��ư normal state�� ����
			//for (auto& scroll : m_enhancementButtons)
			//{
			//	scroll->GetComponent<ButtonComponent>()->SetState(ButtonComponent::ButtonState::Normal);

			//}
			return true;
		}
	}
	if (IsInBounds(mousePos))
	{
		UIManager::Get().OpenWindow(m_windowType);
		return true;
	}

	// ���� ���̸� üũ����.
	return false;
}

bool EnhancementWindow::HandleMouseUp(Vec2 mousePos)
{
	if (!m_isActive) return false;

	// �޼��� ���� ������.
	MSG msg{};
	msg.message = WM_LBUTTONUP;
	Vec2 relativePos = mousePos - m_position;
	msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);

	// ��ư Ŭ�� ó��
	if (IsMouseOverObject(mousePos, m_statSelectionButton.get()))
	{
		// ��� ��ǥ�� ��ȯ
		//Vec2 relativePos = mousePos + m_position;
		msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);

		m_statSelectionButton->GetComponent<ButtonComponent>()->CheckCollision(msg);  // �浹 �˻� ����
		m_statSelectionButton->GetComponent<ButtonComponent>()->Worked(msg);          // �� ���� ��ư ó��
	}

	// left ��ư ����.
	if (IsMouseOverObject(mousePos, m_leftArrowButton.get()))
	{
		//Vec2 relativePos = mousePos + m_position;
		msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);

		m_leftArrowButton->GetComponent<ButtonComponent>()->CheckCollision(msg);  // �浹 �˻� ����
		m_leftArrowButton->GetComponent<ButtonComponent>()->Worked(msg);          // �� ���� ��ư ó��
	}

	// right ��ư ����.
	if (IsMouseOverObject(mousePos, m_rightArrowButton.get()))
	{
		//Vec2 relativePos = mousePos + m_position;
		msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);

		m_rightArrowButton->GetComponent<ButtonComponent>()->CheckCollision(msg);  // �浹 �˻� ����
		m_rightArrowButton->GetComponent<ButtonComponent>()->Worked(msg);          // �� ���� ��ư ó��
	}

	// �ֹ��� ��ư ����.
	for (auto& btn : m_enhancementButtons)
	{
		if (IsMouseOverObject(mousePos, btn.get()))
		{
			//Vec2 relativePos = mousePos + m_position;
			msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);

			btn->GetComponent<ButtonComponent>()->CheckCollision(msg);  // �浹 �˻� ����
			btn->GetComponent<ButtonComponent>()->Worked2(msg);          // �� ���� ��ư ó��
		}

		}

		// ���� ȭ�� ��ǥ ���
		Vec2 slotScreen = m_position + m_enhancementSlot->GetTransform().GetPosition();
		ID2D1Bitmap1* slotBmp = m_enhancementSlot->GetRenderInfo()->GetRenderInfo().bitmap;
		D2D1_SIZE_F size = slotBmp->GetSize();

	// �巡�׵� �������� ���Կ� ����� �� ó��
	if (mousePos.x >= slotScreen.x && mousePos.x <= slotScreen.x + size.width &&
		mousePos.y >= slotScreen.y && mousePos.y <= slotScreen.y + size.height)
	{
		if (CursorManager::Get().IsDragging())
		{
			Item* dragged = CursorManager::Get().GetDraggedItem();
			if (dragged)
			{
				// Wearable���� üũ
				if (auto* wear = dynamic_cast<Wearable*>(dragged))
				{	
					// ��ȭ �Ұ����� �������� ������ �κ��丮�� ��ȯ
					if (!dragged->m_data.enchantable)
					{
						auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
						if (inventoryWindow)
						{
							inventoryWindow->AddItem(dragged->m_data.id, 1);
							std::cout << "��ȭ�Ұ��� �������� �κ��丮�� ��ȯ: " << dragged->m_data.name << std::endl;
							CursorManager::Get().EndItemDrag();
							UIManager::Get().OpenWindow(m_windowType);
							return true;
						}
					}
					// �̹� ���Կ� �������� ������ �κ��丮�� ��ȯ
					if (m_targetItem != nullptr)
					{
						// ���� �������� �κ��丮�� ��ȯ
						auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
						if (inventoryWindow)
						{
							ItemDrop(m_targetItem);
							inventoryWindow->AddItem(m_targetItem->m_data.id, 1);
							std::cout << "���� �������� �κ��丮�� ��ȯ: " << m_targetItem->m_data.name << std::endl;
						}
					}

					m_targetItem = dragged;

					CursorManager::Get().EndItemDrag();

										// ���� ������ ���� ��Ʈ ������ ���� ����
					ItemDrop(m_targetItem);
					UpdateSheetVisibility();

					InitializeItemObject();

					UIManager::Get().OpenWindow(m_windowType);
					return true;
				}
		}
		// ���� ���� �ȵ������� â���� ��ȯ.
		DragSource source = CursorManager::Get().GetDragSource();
		return HandleDropFailure(mousePos, dragged, source);
	}
}

		if (IsInBounds(relativePos))
		{
				// â ���� ������ �巡�׵� �������� ������ �κ��丮�� ��ȯ
				if (CursorManager::Get().IsDragging())
				{
						Item* draggedItem = CursorManager::Get().GetDraggedItem();
						if (draggedItem)
						{
								DragSource source = CursorManager::Get().GetDragSource();
								HandleDropFailure(mousePos, draggedItem, source);
								return true;
						}
				}

				UIManager::Get().OpenWindow(m_windowType);
				return true;
		}

		return false;
}
bool EnhancementWindow::HandleMouseHover(Vec2 mousePos)
{
		MSG msg{};
		msg.message = WM_MOUSEMOVE;
		msg.lParam = MAKELPARAM((int)mousePos.x, (int)mousePos.y);

		if (IsMouseOverObject(mousePos, m_statSelectionButton.get()))
		{
			m_statSelectionButton->GetComponent<ButtonComponent>()->SetCollision(true);  // �浹 ���� ����
			m_statSelectionButton->GetComponent<ButtonComponent>()->Worked(msg);
		}
		else
		{
			m_statSelectionButton->GetComponent<ButtonComponent>()->SetCollision(false);  // �浹 ���� ����
			m_statSelectionButton->GetComponent<ButtonComponent>()->Worked(msg);
		}
				

		if (IsMouseOverObject(mousePos, m_leftArrowButton.get()))
		{
			m_leftArrowButton->GetComponent<ButtonComponent>()->SetCollision(true);   // �浹 ���� ����
			m_leftArrowButton->GetComponent<ButtonComponent>()->Worked(msg);
		}
		else
		{
			m_leftArrowButton->GetComponent<ButtonComponent>()->SetCollision(false);   // �浹 ���� ����
			m_leftArrowButton->GetComponent<ButtonComponent>()->Worked(msg);
		}
				

		if (IsMouseOverObject(mousePos, m_rightArrowButton.get()))
		{
			m_rightArrowButton->GetComponent<ButtonComponent>()->SetCollision(true);    // �浹 ���� ����
			m_rightArrowButton->GetComponent<ButtonComponent>()->Worked(msg);
		}
		else
		{
			m_rightArrowButton->GetComponent<ButtonComponent>()->SetCollision(false);    // �浹 ���� ����
			m_rightArrowButton->GetComponent<ButtonComponent>()->Worked(msg);
		}
				

		for (auto& btn : m_enhancementButtons)
		{
			if (IsMouseOverObject(mousePos, btn.get()))
			{
				btn->GetComponent<ButtonComponent>()->SetCollision(true);
				btn->GetComponent<ButtonComponent>()->Worked2(msg);
			}
			else
			{
				btn->GetComponent<ButtonComponent>()->SetCollision(false);
				btn->GetComponent<ButtonComponent>()->Worked2(msg);
			}
		}

						

		//for (auto& btn : m_enhancementButtons)
		//		if (IsMouseOverButton(mousePos, btn.get()))
		//				btn->GetComponent<ButtonComponent>()->Worked(msg);


		if (!m_isDragging)
		{
				// ���Կ� ���콺 ���� �� ���� ǥ��. �����ǥ�� ��ȯ
				Vec2 relativePos = mousePos - m_position;
				SynSlot whichSlot = SlotInit(relativePos);

				if (whichSlot == SynSlot::Result)
				{
						std::cout << "���Կ� ��ġ������." << std::endl;
						Item* Clicked_Item = m_targetItem;
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
				else
				{

						std::cout << "���� �������� ���ų� ��ġ �ƴϿ��� ����." << std::endl;
						UIManager::Get().CloseWindow(UIWindowType::InventoryTooltip);
						CursorManager::Get().HoveredReleased(); //���� ���� 
						return false;
				}
		}
		return true;
}

bool EnhancementWindow::HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source)
{
	// �巡�׵� �������� ������ �ƹ��͵� ���� ����
	if (!draggedItem) return false;

	// �κ��丮 â�� �����͸� ������
	auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));

	// �κ��丮 â�� ��ȿ�ϸ� �������� �߰�
	if (inventoryWindow)
	{
		// ItemDrop �Լ��� �ʿ��� ��� ȣ�� (�ڵ忡 ���� �ٸ�)
		ItemDrop(draggedItem);
		inventoryWindow->AddItem(draggedItem->m_data.id, 1);
		std::cout << "��ȭâ �巡�� ����: �������� �κ��丮�� �����մϴ�: " << draggedItem->m_data.name << std::endl;
	}

	// UIManager�� �巡�� ���Ḧ ó���ϰ�����, �����ϰ� ���⼭ ȣ������ �� ����
	CursorManager::Get().EndItemDrag();
	return true;

	//Vec2 relativePos = mousePos - m_position;
	//SynSlot whichSlot = SlotInit(relativePos);

	//if (whichSlot == SynSlot::Result)
	//{
	//	Item* Clicked_Item = m_targetItem;
	//	if (Clicked_Item != nullptr)
	//	{
	//		CursorManager::Get().SetHoveredItem(Clicked_Item);
	//		Vec2 tooltipPos = mousePos + Vec2(10, 10);

	//		UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, tooltipPos); //��ġ �����Ű��, Ȱ��ȭ���� 
	//		return true;
	//	}
	//	else
	//		return true;

	//}
	//else
	//{
	//	UIManager::Get().CloseWindow(UIWindowType::InventoryTooltip);
	//	CursorManager::Get().HoveredReleased(); //���� ���� 
	//	return false;
	//}
	return true;
}

bool EnhancementWindow::HandleMouseRight(Vec2 mousePos)
{
	if (!IsInBounds(mousePos)) return false;

	// �����ǥ �̽�
	Vec2 relativeMousePos = mousePos - m_position;

	if (SlotInit(relativeMousePos) != SynSlot::Result || m_targetItem == nullptr)
	{
		UIManager::Get().OpenWindow(m_windowType);
		CursorManager::Get().EndItemDrag();
		return true;
	}

	Item* item = m_targetItem;

	Wearable* wearableItem = dynamic_cast<Wearable*>(item);
	// ���� ���� ������ ������ ������.
	if (wearableItem->Getpart() != Wearable_part::UnKnown)
	{
		// �κ��丮�� ����
		if (auto inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow)))
		{
			// ���� ������ �����ֱ�.
			ItemDrop(item);
			inventory->AddItem(item->m_data.id, 1);
			m_targetItem = nullptr;
			return true;
		}
	}

	return false;
}
void EnhancementWindow::Check4ItemEffect()
{
	//for (auto& scroll : m_enhancementButtons)
	//{
	//	auto bt = scroll->GetComponent<ButtonComponent>();
	//	auto state = bt->GetState();
	//	if (state == ButtonComponent::ButtonState::Pressed)	return false;
	//}
	auto runeFade = m_targetItem->GetComponent<Fade_Effect>();
	auto opacity = m_targetItem->GetComponent<Opacity_Effect>();	// item�� opacity
	auto increase = m_targetItem->GetComponent<Increasing_Effect>();
	auto failClip = m_targetItem->GetComponent<Clip_Effect>();
	string state1 = increase->GetCurrentState();
	string state2 = increase->GetBeforeState();
	string state3 = failClip->GetCurrentState();

	if (state3 == "DORMANT")
	{
		runeFade->OnEvent("SHOW");
	}
	else if (state1 == "DORMANT" && state2 == "DISAPPEAR")
	{
		runeFade->OnEvent("SHOW");
	}

	if (state3 == "PLAY")
	{
		runeFade->OnEvent("HIDE");
	}
	else if (state1 == "PLAY" && state2 == "DORMANT")
	{
		runeFade->OnEvent("HIDE");
	}
	else if (state1 == "PLAY" && state2.empty())
	{
		runeFade->OnEvent("HIDE");
	}
}
void EnhancementWindow::RenderBackground()
{
		UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();
		if (uiRenderer)
		{
				ID2D1Bitmap1* bg = uiRenderer->GetBitmap("enhancement_window_bg").Get();
				D2D1_RECT_F dest = { m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y };
				D2DRenderer::Get().DrawBitmap(bg, dest);

				return;
		}
}

void EnhancementWindow::RenderCloseButton()
{
		UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();
		float rightMargin = 65.0f;  // 47���� 459�� ����
		Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 35 };
		Vec2 closeButtonSize = { 35.0f, 35.0f };  // 27���� 35�� ����

		D2D1_RECT_F destRect = { closeButtonPos.x, closeButtonPos.y, closeButtonPos.x + closeButtonSize.x, closeButtonPos.y + closeButtonSize.y };

		if (uiRenderer)
		{
			ID2D1Bitmap1* cb = uiRenderer->GetBitmap("close_button").Get();
			if (cb) D2DRenderer::Get().DrawBitmap(cb, destRect);
		}
}

void EnhancementWindow::RenderEnhancementSlot()
{
		UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();

		if (!uiRenderer) return;
		// â ��ġ�� ���� ���� ȭ�� ��ǥ
		Vec2 screenPos = m_position + m_enhancementSlot->GetTransform().GetPosition();

		// ���� ũ��� ��ġ �����ֱ�.
		D2D1_RECT_F destRect = { screenPos.x, screenPos.y, screenPos.x + 74, screenPos.y + 74 };

		// ���� ������Ʈ �׸���.
		D2DRenderer::Get().DrawBitmap(m_enhancementSlot->GetRenderInfo()->GetRenderInfo().bitmap, destRect);

		// ���� �ܰ�����.
		D2DRenderer::Get().DrawRectangle(screenPos.x, screenPos.y, screenPos.x + 74, screenPos.y + 74, D2D1::ColorF(D2D1::ColorF::Black));

		// ���Կ� ��ϵ� ������ ������ ǥ��
		if (m_targetItem)
		{
			//D2D_RECT_F src = ResourceManager::Get().Get_ItemBank().GetItemClip(m_targetItem->m_data.id)->srcRect;
			//src.left = src.left + 80.f;
			//src.top = src.top + 80.f;
			//src.right = src.right - 80.f;
			//src.bottom = src.bottom - 80.f;
				//auto clip = ResourceManager::Get().Get_ItemBank().GetItemClip(m_targetItem->m_data.id);
				//ID2D1Bitmap1* itemAtlas = clip->atlas.Get();
				//if (itemAtlas)
				//{
				//		// destRect ũ�⿡ ���缭 ������ �׸���
				//		D2DRenderer::Get().DrawBitmap(itemAtlas, destRect, src);
				//}
			m_targetItem->GetTransform().SetPosition({ screenPos.x - 91.f * 0.8f + 8.f, screenPos.y - 91.f * 0.8f + 6.f });
			D2DRenderer::Get().DrawBitmap(m_targetItem->GetRenderInfo()->GetRenderInfo());
		}
}

void EnhancementWindow::RenderSheetImages()
{
	if (!m_targetItem) return;
	//std::cout << "����������" << std::endl;

	Wearable* wearableItem = dynamic_cast<Wearable*>(m_targetItem);
	if (!wearableItem) return;

	auto& enchResults = wearableItem->GetEnchancResult();

	size_t count = m_normalImages.size();
	size_t renderCount = std::min(static_cast<size_t>(m_renderSheetCount), count);

	size_t startIndex = (wearableItem->Getpart() == Wearable_part::Weapon) ? 0 : 1;

	// 3�� ���µ� �ȶ�.
	//std::cout << "��Ʈ ���� :" << renderCount << std::endl;
	
	// �������� ������ (0->1->2->3->4)
	for (size_t i = 0; i < renderCount; ++i)
	{
		
		// ���� ���� �ε��� 1 �߰�.
		size_t sheetIndex = startIndex + i;
		//if (sheetIndex >= m_sheetImages.size()) break;

		//auto& sheet = m_sheetImages[sheetIndex];
		auto& sheet1 = m_normalImages[sheetIndex];
		auto& sheet2 = m_successImages[sheetIndex];
		auto& sheet3 = m_failImages[sheetIndex];
		//auto bgComp = sheet->GetComponent<BackgroundComponent>();
		//if (!bgComp) continue;

		//// ���º� ��Ʈ�� ����
		//switch (enchResults[i])
		//{
		//case EnchancerType::Default:
		//	//bgComp->SetCurrentBitmap("Normal");
		//	bgComp->SetCurrentEffect("Normal");
		//	break;
		//case EnchancerType::Sucess:
		//	//bgComp->SetCurrentBitmap("Success");
		//	bgComp->SetCurrentEffect("Success");
		//	break;
		//case EnchancerType::Fail:
		//	//bgComp->SetCurrentBitmap("Fail");
		//	bgComp->SetCurrentEffect("Fail");
		//	break;
		//}

		// ȭ�鿡 ������
		//auto info = sheet->GetRenderInfo();
		//Vec2 pos = m_position + m_sheetPosition[sheetIndex];
		//sheet->GetTransform().SetPosition({ pos.x, pos.y });
		////D2D1_RECT_F dest = { pos.x, pos.y, pos.x + info->GetSize().width, pos.y + info->GetSize().height};

		////D2DRenderer::Get().DrawBitmap(bmp, dest);
		//D2DRenderer::Get().DrawBitmap(info->GetRenderInfo());
		auto info1 = sheet1->GetRenderInfo();
		Vec2 pos1 = m_position + m_sheetPosition[sheetIndex];
		sheet1->GetTransform().SetPosition({ pos1.x, pos1.y });
		D2DRenderer::Get().DrawBitmap(info1->GetRenderInfo());

		auto info2 = sheet2->GetRenderInfo();
		Vec2 pos2 = m_position + m_sheetPosition[sheetIndex];
		sheet2->GetTransform().SetPosition({ pos2.x, pos2.y });
		D2DRenderer::Get().DrawBitmap(info2->GetRenderInfo());

		auto info3 = sheet3->GetRenderInfo();
		Vec2 pos3 = m_position + m_sheetPosition[sheetIndex];
		sheet3->GetTransform().SetPosition({ pos3.x, pos3.y });
		D2DRenderer::Get().DrawBitmap(info3->GetRenderInfo());
	}
}

void EnhancementWindow::RenderStatSelectionButtons()
{
		// �߾� ��ư 
		Vec2 screenPos = m_position + m_statSelectionButton->GetTransform().GetPosition();
		auto statSelectionInfo = m_statSelectionButton->GetRenderInfo();
		statSelectionInfo->SetDestRect({ screenPos.x, screenPos.y, screenPos.x + statSelectionInfo->GetSize().width, screenPos.y + statSelectionInfo->GetSize().height});

		D2DRenderer::Get().DrawBitmap(statSelectionInfo->GetRenderInfo());
		// ������
		//if (bmp)
		//{
				//D2D1_SIZE_F size = bmp->GetSize();
				//D2D1_RECT_F dest = { screenPos.x, screenPos.y, screenPos.x + size.width, screenPos.y + size.height };
				//D2DRenderer::Get().DrawBitmap(bmp, dest);
		//}

		// ���� ȭ��ǥ ��ư ������
		screenPos = m_position + m_leftArrowButton->GetTransform().GetPosition();
		auto leftArrowInfo = m_leftArrowButton->GetRenderInfo();

		leftArrowInfo->SetDestRect({ screenPos.x, screenPos.y, screenPos.x + leftArrowInfo->GetSize().width, screenPos.y + leftArrowInfo->GetSize().height});
		
		D2DRenderer::Get().DrawBitmap(leftArrowInfo->GetRenderInfo());
		
		//if (bmp)
		//{
		//		D2D1_SIZE_F size = bmp->GetSize();
		//		D2D1_RECT_F dest = { screenPos.x, screenPos.y, screenPos.x + size.width, screenPos.y + size.height };
		//		D2DRenderer::Get().DrawBitmap(bmp, dest);
		//}

		// ���� ȭ��ǥ ��ư ������
		screenPos = m_position + m_rightArrowButton->GetTransform().GetPosition();
		auto rightArrowInfo = m_rightArrowButton->GetRenderInfo();

		rightArrowInfo->SetDestRect({ screenPos.x, screenPos.y, screenPos.x + rightArrowInfo->GetSize().width, screenPos.y + rightArrowInfo->GetSize().height });

		D2DRenderer::Get().DrawBitmap(rightArrowInfo->GetRenderInfo());
		
		//if (bmp)
		//{
		//		D2D1_SIZE_F size = bmp->GetSize();
		//		D2D1_RECT_F dest = { screenPos.x, screenPos.y, screenPos.x + size.width, screenPos.y + size.height };
		//		D2DRenderer::Get().DrawBitmap(bmp, dest);
		//}
}


void EnhancementWindow::RenderStatText()
{
		Vec2 buttonPos = m_position + m_statSelectionButton->GetTransform().GetPosition();
		Vec2 textPos = { buttonPos.x + 152, buttonPos.y + 15 }; // ��ư �߾ӿ� �ؽ�Ʈ ��ġ

		std::wstring statText;
		int statValue = GetSelectedStatValue();

		switch (m_selectedStat)
		{
		case Stat::STR:
				statText = L"STR + " + std::to_wstring(statValue);
				break;
		case Stat::DEX:
				statText = L"DEX + " + std::to_wstring(statValue);
				break;
		case Stat::INT:
				statText = L"INT + " + std::to_wstring(statValue);
				break;
		case Stat::LUK:
				statText = L"LUK + " + std::to_wstring(statValue);
				break;
		
		}

		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, PtToPx(12));
		D2DRenderer::Get().DrawMessage(statText.c_str(), textPos.x, textPos.y, 150, 70, D2D1::ColorF(D2D1::ColorF::White));
}

void EnhancementWindow::RenderScrollButtons()
{

// ��ȭ ��ư 3�� ������ 
	for (size_t i = 0; i < m_enhancementButtons.size(); ++i)
	{
		auto& btn = m_enhancementButtons[i];
		auto info = btn->GetRenderInfo();
		Vec2 screenPos = m_position + btn->GetTransform().GetPosition();
		//ID2D1Bitmap1* bmp = btn->GetRenderInfo()->GetRenderInfo().bitmap;
		
		info->SetDestRect({ screenPos.x, screenPos.y, screenPos.x + info->GetSize().width, screenPos.y + info->GetSize().height });
		D2DRenderer::Get().DrawBitmap(info->GetRenderInfo());	
	}
}


// ��ȭ ��ư ������ ���. 
void EnhancementWindow::TryEnhance(int successRate)
{
		// ���� ��ư�� �����״� successRate�� rand�� ����.
		// ����, ���� ��ȯ, ����Ʈ �׸�����. bool�� render�� ����Ʈ �׸���. 
		// ������ ���� ������Ʈ. 
		if (!m_targetItem) return;

		Wearable* wearableItem = dynamic_cast<Wearable*>(m_targetItem);

		if (!wearableItem || wearableItem->GetEnchanCount() <= 0)
				return;

		// ���� ����/���� ����
		std::uniform_int_distribution<int> dist(1, 100);

		int roll = dist(m_rng);
		bool success = roll <= successRate;
		//bool success = true;
		auto& results = wearableItem->GetEnchancResult();
		int currentIndex = -1;

		for (int i = 0; i < results.size(); ++i)
		{
				if (results[i] == EnchancerType::Default) //  �⺻�� Ȯ��
				{
						currentIndex = i;
						break;
				}
		}

		if (currentIndex == -1)
		{
				// �� ���� ����. ��� ��ϵǾ���������
				return;
		}

		if (success)
		{

			SettingWindow* SETWin = dynamic_cast<SettingWindow*>(UIManager::Get().GetWindow(UIWindowType::SettingsWindow));
			if (SETWin)
			{
				float val = SETWin->GetSFXValue();
				SoundManager::Instance().PlaySFX("SC", val);
			}
				std::cout << "��ȭ ����!" << std::endl;
				// ���� �ִϸ��̼� �� ������ ���� ������Ʈ
				// ������ ���� �����ͼ� ���� �÷��ֱ�.
				wearableItem->GetEnchancResult()[currentIndex] = EnchancerType::Sucess;
				// ���õ� ���ȿ� ���� ��ġ �߰�
				int statValue = GetSelectedStatValue();
				fundamentalStatus addStat = {};
				switch (m_selectedStat)
				{
				case Stat::STR:
						addStat.power = statValue;
						std::cout << "STR + " << statValue << " �߰�!" << std::endl;
						break;
				case Stat::DEX:
						addStat.agile = statValue;
						std::cout << "DEX + " << statValue << " �߰�!" << std::endl;
						break;
				case Stat::INT:
						addStat.intelligence = statValue;
						std::cout << "INT + " << statValue << " �߰�!" << std::endl;
						break;
				case Stat::LUK:
						addStat.luck = statValue;
						std::cout << "LUK + " << statValue << " �߰�!" << std::endl;
						break;
				}
				wearableItem->AddStat(addStat);
		}
		else
		{
			SettingWindow* SETWin = dynamic_cast<SettingWindow*>(UIManager::Get().GetWindow(UIWindowType::SettingsWindow));
			if (SETWin)
			{
				float val = SETWin->GetSFXValue();
				SoundManager::Instance().PlaySFX("FA",val);
			}

				std::cout << "��ȭ ����..." << std::endl;
				// ���� �ִϸ��̼�
				// ���з� �ٲ��ֱ�.
				wearableItem->GetEnchancResult()[currentIndex] = EnchancerType::Fail;
		}

		// ī��Ʈ ���ֱ�.
		wearableItem->UseEnhanceChance();

}

void EnhancementWindow::SetTargetItem(Item* item)
{
	m_targetItem = item;
}

void EnhancementWindow::UpdateSheetVisibility()
{
	if (!m_targetItem) return;

	Wearable* wearableItem = dynamic_cast<Wearable*>(m_targetItem);
	if (!wearableItem) return;

	// ������ ��� 5��, �� �ܴ� 3���� ��ȭ ���� ���� ����
	int newSheetCount = (wearableItem->Getpart() == Wearable_part::Weapon) ? 5 : 3;

	// m_renderSheetCount ������Ʈ
	m_renderSheetCount = newSheetCount;

	// ���� �������� ��ȭ �̷� ���Ͱ� ����ִٸ� (���ο� ������), �ʱ�ȭ
	if (wearableItem->GetEnchancResult().empty())
	{
		wearableItem->GetEnchancResult().assign(m_renderSheetCount, EnchancerType::Default);
	}

	auto enchResults = wearableItem->GetEnchancResult();
	for (int i = 0; i < 5; i++)
	{
		// ���º� ��Ʈ�� ����
		if (m_targetItem->m_data.wearablePart == Wearable_part::Weapon)
		{
			switch (enchResults[i])
			{
			case EnchancerType::Sucess:
				m_normalImages[i]->GetRenderInfo()->SetisActive(false);
				m_successImages[i]->GetRenderInfo()->SetisActive(true);
				m_failImages[i]->GetRenderInfo()->SetisActive(false);

				break;
			case EnchancerType::Fail:
				m_normalImages[i]->GetRenderInfo()->SetisActive(false);
				m_successImages[i]->GetRenderInfo()->SetisActive(false);
				m_failImages[i]->GetRenderInfo()->SetisActive(true);
				break;
			case EnchancerType::Default:
				m_normalImages[i]->GetRenderInfo()->SetisActive(true);
				m_successImages[i]->GetRenderInfo()->SetisActive(false);
				m_failImages[i]->GetRenderInfo()->SetisActive(false);
				break;
			}
		}
		else if (i > 0 && i < 4)
		{
			switch (enchResults[i - 1])
			{
			case EnchancerType::Sucess:
				m_normalImages[i]->GetRenderInfo()->SetisActive(false);
				m_successImages[i]->GetRenderInfo()->SetisActive(true);
				m_failImages[i]->GetRenderInfo()->SetisActive(false);
				break;
			case EnchancerType::Fail:
				m_normalImages[i]->GetRenderInfo()->SetisActive(false);
				m_successImages[i]->GetRenderInfo()->SetisActive(false);
				m_failImages[i]->GetRenderInfo()->SetisActive(true);
				break;
			case EnchancerType::Default:
				m_normalImages[i]->GetRenderInfo()->SetisActive(true);
				m_successImages[i]->GetRenderInfo()->SetisActive(false);
				m_failImages[i]->GetRenderInfo()->SetisActive(false);
				break;
			}
		}
	}
}

void EnhancementWindow::InitializeItemObject()
{
	auto rayBM = ResourceManager::Get().GetTexture("treasureRay");
	auto glowBM = ResourceManager::Get().GetTexture("basicglow");
	auto runeBM = ResourceManager::Get().GetTexture("runeword05");
	auto fail1BM = ResourceManager::Get().GetTexture("splash00");
	auto fail2BM = ResourceManager::Get().GetTexture("splash01");
	auto fail3BM = ResourceManager::Get().GetTexture("splash02");
	auto fail4BM = ResourceManager::Get().GetTexture("splash03");
	auto fail5BM = ResourceManager::Get().GetTexture("splash04");

	m_targetItem->ComponentClear();
	m_targetItem->GetTransform().SetScale({0.8f, 0.8f});	// ���� �ʿ��� position �ϵ��ڵ��ؼ� �����ؾ���..��
	
	auto info = m_targetItem->GetRenderInfo();
	
	auto ray1 = m_targetItem->AddComponent<Rotate3D_Effect>(info, 0.f, 0.f, 0.f, 0.2f, rayBM.Get()); // 0
	auto ray2 = m_targetItem->AddComponent<Rotate3D_Effect>(info, 0.f, 0.f, 0.f, -0.2f, rayBM.Get());	// 1
	auto rayComposite = m_targetItem->AddComponent<Composite_Effect>(info, ray1->GetEffect(), ray2->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER); //2
	auto rayBlur = m_targetItem->AddComponent<GaussianBlur_Effect>(info, 2.f, rayComposite->GetEffect()); // 3
	auto increase = m_targetItem->AddComponent<Increasing_Effect>(info, D2D1_POINT_2F{ rayBM->GetSize().width / 2.f, rayBM->GetSize().height / 2.f }, 1.5f, 1.f, rayBlur->GetEffect()); // 4
	auto offsetRay = m_targetItem->AddComponent<Offset_Effect>(info, info->GetRenderInfo().srcRect.left, 0.f, increase->GetEffect()); // 5
	
	auto glowColor = m_targetItem->AddComponent<Color_Effect>(info, 0.f, 0.f, 0.f, 0.0f, glowBM.Get()); // 6
	auto glowMove = m_targetItem->AddComponent<Sideway_Effect>(info, 2.f, 0.02f, 0.02f, glowColor->GetEffect()); // 7
	auto glowOffset = m_targetItem->AddComponent<Offset_Effect>(info, info->GetRenderInfo().srcRect.left, 0.f, glowMove->GetEffect()); // 8

	auto runeMove = m_targetItem->AddComponent<BackForth_Effect>(info, 0.f, 0.f, 0.f, runeBM.Get()); // 9
	auto runeOffset = m_targetItem->AddComponent<Offset_Effect>(info, info->GetRenderInfo().srcRect.left, 0.f, runeMove->GetEffect()); // 10

	auto glowruneCompo = m_targetItem->AddComponent<Composite_Effect>(info, runeOffset->GetEffect(), glowOffset->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER); // 11
	auto fade = m_targetItem->AddComponent<Fade_Effect>(info, 0.f, 0.4f, 0.005f, glowruneCompo->GetEffect()); // 12
	fade->OnEvent("SHOW");

	auto rayGlowruneCompo = m_targetItem->AddComponent<Composite_Effect>(info, fade->GetEffect(), offsetRay->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER); // 13
	
	auto itemOpacity = m_targetItem->AddComponent<Opacity_Effect>(info, 1.f, info->GetBitmap()); // 14
	auto behindcompo = m_targetItem->AddComponent<Composite_Effect>(info, itemOpacity->GetEffect(), rayGlowruneCompo->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER); // 15

	auto fail = m_targetItem->AddComponent<Clip_Effect>(info, fail1BM.Get(), fail2BM.Get(), fail3BM.Get(), fail4BM.Get(), fail5BM.Get()); // 16
	auto failRotate = m_targetItem->AddComponent<Rotate3D_Effect>(info, 0.f, 0.f, 0.f, 3.f, fail->GetEffect()); // 17
	auto failOffset = m_targetItem->AddComponent<Offset_Effect>(info, info->GetRenderInfo().srcRect.left, 0.f, failRotate->GetEffect()); // 18
	auto failBlur1 = m_targetItem->AddComponent<GaussianBlur_Effect>(info, 1.f, failOffset->GetEffect()); // 19
	auto failBlur2 = m_targetItem->AddComponent<GaussianBlur_Effect>(info, 3.f, failOffset->GetEffect()); // 20
	auto failBlurcompo = m_targetItem->AddComponent<Composite_Effect>(info, failBlur1->GetEffect(), failBlur2->GetEffect(), D2D1_COMPOSITE_MODE_PLUS); // 21
	auto frontcompo = m_targetItem->AddComponent<Composite_Effect>(info, failBlurcompo->GetEffect(), behindcompo->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER); // 22
}

bool EnhancementWindow::IsMouseOverObject(const Vec2& mousePos, Object* obj) const
{
		// ���콺 ���� üũ ���� ����
		if (!obj) return false;

		Vec2 objPos = m_position + obj->GetTransform().GetPosition();
		ID2D1Bitmap1* bitmap = obj->GetRenderInfo()->GetRenderInfo().bitmap;
		if (!bitmap) return false;

		D2D1_SIZE_F size = bitmap->GetSize();

		return (mousePos.x >= objPos.x && mousePos.x <= objPos.x + size.width &&
				mousePos.y >= objPos.y && mousePos.y <= objPos.y + size.height);
}

Vec2 EnhancementWindow::GetObjectScreenPos(Object* obj) const
{
		if (!obj) return Vec2();
		return m_position + obj->GetTransform().GetPosition();
}

bool EnhancementWindow::IsMouseOverButton(const Vec2& mousePos, Object* obj) const
{
		if (!obj) return false;

		auto btnComponent = obj->GetComponent<ButtonComponent>();
		Vec2 objPos = m_position + obj->GetTransform().GetPosition();
		ID2D1Bitmap1* bitmap = obj->GetRenderInfo()->GetRenderInfo().bitmap;
		if (!bitmap) return false;

		D2D1_SIZE_F originalSize = bitmap->GetSize();

		// ���� ���¿� ���� ������ ���
		float scale = 1.0f;
		if (btnComponent)
		{
				switch (btnComponent->GetState())
				{
				case ButtonComponent::ButtonState::Hover:
						scale = 1.1f;
						break;
				case ButtonComponent::ButtonState::Pressed:
						scale = 0.95f;
						break;
				default:
						scale = 1.0f;
						break;
				}
		}

		float scaledWidth = originalSize.width * scale;
		float scaledHeight = originalSize.height * scale;
		float offsetX = (scaledWidth - originalSize.width) * 0.5f;
		float offsetY = (scaledHeight - originalSize.height) * 0.5f;

		std::cout << objPos.x - offsetX << " " << objPos.y - offsetY << std::endl;
		return (mousePos.x >= objPos.x - offsetX &&
				mousePos.x <= objPos.x - offsetX + scaledWidth &&
				mousePos.y >= objPos.y - offsetY &&
				mousePos.y <= objPos.y - offsetY + scaledHeight);
}

void EnhancementWindow::SetupButtonCallbacks()
{
		// ���� ���� ��ư �ݹ�
		if (m_statSelectionButton)
		{
				auto btnComponent = m_statSelectionButton->GetComponent<ButtonComponent>();
				if (btnComponent)
				{
						btnComponent->SetOnClickCallback([this]() {
								OnStatSelectionButtonClick();
								});
				}
		}

		// ���� ȭ��ǥ ��ư �ݹ�
		if (m_leftArrowButton)
		{
				auto btnComponent = m_leftArrowButton->GetComponent<ButtonComponent>();
				if (btnComponent)
				{
						btnComponent->SetOnClickCallback([this]() {
								OnLeftArrowClick();
								});
				}
		}

		// ���� ȭ��ǥ ��ư �ݹ�
		if (m_rightArrowButton)
		{
				auto btnComponent = m_rightArrowButton->GetComponent<ButtonComponent>();
				if (btnComponent)
				{
						btnComponent->SetOnClickCallback([this]() {
								OnRightArrowClick();
								});
				}
		}

		// ��ȭ ��ư�� �ݹ�
		for (size_t i = 0; i < m_enhancementButtons.size(); ++i)
		{
				auto btnComponent = m_enhancementButtons[i]->GetComponent<ButtonComponent>();
				if (btnComponent)
				{
						btnComponent->SetOnClickCallback([this, i]() {
								OnEnhancementButtonClick(i);
								});
				}
		}
}

void EnhancementWindow::OnStatSelectionButtonClick()
{
	auto m_pressedButton = m_enhancementButtons[m_selectedScrollIndex]->GetComponent<ButtonComponent>();
	auto buttonState = m_pressedButton->GetState();
	if (buttonState != ButtonComponent::ButtonState::Pressed)
	{
		std::cout << "�ֹ����� �������ּ���" << std::endl;
		return;
	}

	// ���� scroll ��ư normal state�� ����
	//m_pressedButton->SetState(ButtonComponent::ButtonState::Normal);
	//m_pressedButton->SetOpacity(1.f);
	std::cout << "���� ��ȭ ��ư Ŭ����" << std::endl;
	if (m_targetItem == nullptr)	return;	// ��ȭâ�� ������ ������ ����

	//auto runeFade = m_targetItem->GetComponent<Fade_Effect>();
	//runeFade->OnEvent("HIDE");
	auto success = m_targetItem->GetComponent<Increasing_Effect>();
	auto state = success->GetCurrentState();
	if (state != "DORMANT")	return;

	if (!m_targetItem->m_data.enchantable)
	{
		std::cout << "��ȭ �Ұ��� ������." << std::endl;
		std::cout << "��ȭ �Ұ��� ������." << std::endl;
		return;
	}

	if (!m_targetItem->m_data.enchantable)
	{

			std::cout << "��ȭ �Ұ��� ������." << std::endl;
			return;
	}

	//if (m_selectedScrollIndex >= m_enhancementButtons.size())	// ��� �۵� �� �ϴ� ��
	//{
	//	std::cout << "�ֹ����� �������ּ���" << std::endl;
	//	return;
	//}

	// ��ư �ε����� ���� ������
	int successRate = 0;
	switch (m_selectedScrollIndex)
	{
	case 0: successRate = 100; break;
	case 1: successRate = 40; break;
	case 2: successRate = 20; break;
	default: successRate = 0; break;
	}

	std::cout << "��ȭ �õ�: ������ " << successRate << "%" << std::endl;

	if (m_targetItem->m_data.wearablePart == Wearable_part::Weapon)
	{
		for (int i = 0; i < 5; i++)
		{
			bool normal = m_normalImages[i]->GetRenderInfo()->GetRenderInfo().activated;
			if (normal)
			{
				auto explode = m_normalImages[i]->GetComponent<Explode_Effect>();
				explode->OnEvent("PLAY");
				break;
			}
		}
	}
	else
	{
		for (int i = 1; i < 4; i++)
		{
			bool normal = m_normalImages[i]->GetRenderInfo()->GetRenderInfo().activated;
			if (normal)
			{
				auto explode = m_normalImages[i]->GetComponent<Explode_Effect>();
				explode->OnEvent("PLAY");
				break;
			}
		}
	}

	TryEnhance(successRate);
}



void EnhancementWindow::OnLeftArrowClick()
{
		int currentStat = static_cast<int>(m_selectedStat);
		currentStat = (currentStat - 1 + 4) % 4; // 4�� ���� ��ȯ
		m_selectedStat = static_cast<Stat>(currentStat);
}

void EnhancementWindow::OnRightArrowClick()
{
		// ���� �������� ����
		int currentStat = static_cast<int>(m_selectedStat);
		currentStat = (currentStat + 1) % 4; // 4�� ���� ��ȯ
		m_selectedStat = static_cast<Stat>(currentStat);
}

void EnhancementWindow::OnEnhancementButtonClick(size_t buttonIndex)
{
	for (auto& bt : m_enhancementButtons)
	{
		if (bt == m_enhancementButtons[buttonIndex])	continue;
		auto btComp = bt->GetComponent<ButtonComponent>();
		btComp->SetState(ButtonComponent::ButtonState::Normal);
		btComp->SetOpacity(1.f);
	}
	//// ���� ���õ� ��ư opacity ����
	m_selectedScrollIndex = buttonIndex;

	if (!m_targetItem)
	{
		std::cout << "��ȭ�� �������� �����ϴ�" << std::endl;
		return;
	}

	// ��ư �ε����� ���� ������
	int successRate = 0;
	switch (buttonIndex)
	{
	case 0: successRate = 100; break;
	case 1: successRate = 40; break;
	case 2: successRate = 20; break;
	default: successRate = 0; break;
	}

	std::cout << "�ֹ��� ����: ������ " << successRate << "%, ���� +" << GetSelectedStatValue() << std::endl;

	//auto increase = m_targetItem->GetComponent<Increasing_Effect>();
	//if (increase->GetScale().x != 0.f && increase->GetisStop())	// increase != nullptr && 
	//{
	//	increase->OnEvent("DISAPPEAR");
	//}

	//auto runeFade = m_targetItem->GetComponent<Fade_Effect>();
	//runeFade->OnEvent("HIDE");
}

int EnhancementWindow::GetSelectedStatValue() const
{
		switch (m_selectedScrollIndex)
		{
		case 0: return 1;  // 100% �ֹ��� = +1
		case 1: return 2;  // 40% �ֹ��� = +2  
		case 2: return 5;  // 20% �ֹ��� = +5
		default: return 0;
		}
}

void EnhancementWindow::ReturnItemToInventory()
{
	if (m_targetItem != nullptr)
	{
		// �κ��丮�� ������ ��ȯ
		auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
		if (inventoryWindow)
		{
			inventoryWindow->AddItem(m_targetItem->m_data.id, 1);
			std::cout << "��ȭâ �ݱ� �� ������ �κ��丮�� ��ȯ: " << m_targetItem->m_data.name << std::endl;

			// ���� ����
			m_targetItem = nullptr;

			// ��Ʈ �̹��� �ʱ�ȭ						m_renderSheetCount = 0;
		}
	}
	// �ֹ��� ��ư normal state�� ����
	for (auto& scroll : m_enhancementButtons)
	{
		scroll->GetComponent<ButtonComponent>()->SetState(ButtonComponent::ButtonState::Normal);

	}
}

SynSlot EnhancementWindow::SlotInit(Vec2 pos)
{
		// ���� ��ġ return ������ result ������ nothing��.
		// slot size 74
		if (pos.x >= m_enhancementSlot.get()->GetTransform().GetPosition().x &&
			  pos.x <= m_enhancementSlot.get()->GetTransform().GetPosition().x + SlotSize &&
				pos.y >= m_enhancementSlot.get()->GetTransform().GetPosition().y &&
				pos.y <= m_enhancementSlot.get()->GetTransform().GetPosition().y + SlotSize)
		{
				return SynSlot::Result;
		}
		return SynSlot::Nothing;
}


