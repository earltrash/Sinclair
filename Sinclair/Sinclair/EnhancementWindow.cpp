#include "EnhancementWindow.h"
#include "ButtonComponent.h"

#include <windows.h>
#undef min
#undef max
#include <algorithm>

#include "Wearable.h"

void EnhancementWindow::Update()
{
	// 여기에 오브젝트 얘들 전부 component update 시키기.

	if (m_targetItem == nullptr)	return;

	auto clip = m_targetItem->GetComponent<Clip_Effect>();				// target item의 실패 효과 컴포넌트
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
		auto explode = m_normalImages[i]->GetComponent<Explode_Effect>();	// sheet효과 컴포넌트
		auto increase = m_targetItem->GetComponent<Increasing_Effect>();	// target item의 성공 효과 컴포넌트
		
		auto opacity = m_targetItem->GetComponent<Opacity_Effect>();

		if (explode->GetScale().x <= 0.f && explode->GetScale().y <= 0.f && explode != nullptr)
		{
			explode->OnEvent("DORMANT");
			
			opacity->SetOpacity(1.f);
			// 상태별 비트맵 선택
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
		time = 0.f;		// 처음 dt가 1이상 큰 수가 들어옴
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

		// 메세지 만들어서 던지기.
		MSG msg{};
		msg.message = WM_LBUTTONDOWN;

		// 상대좌표로 이미 계산함 머지 겹쳐서 자동머지로 터진거임
		Vec2 relativePos = mousePos - m_position;
		msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);

		std::cout << "HandleMouseDown - 마우스 위치: (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
	/*	if (m_statSelectionButton)
		{
			Vec2 btnPos = m_statSelectionButton->GetTransform().GetPosition();
			Vec2 btnScreenPos = m_position + btnPos;
			std::cout << "스탯 버튼 화면 위치: (" << btnScreenPos.x << ", " << btnScreenPos.y << ")" << std::endl;
		}
		if (m_leftArrowButton)
		{
			Vec2 btnPos = m_leftArrowButton->GetTransform().GetPosition();
			Vec2 btnScreenPos = m_position + btnPos;
			std::cout << "left 버튼 화면 위치: (" << btnScreenPos.x << ", " << btnScreenPos.y << ")" << std::endl;
		}
		if (m_rightArrowButton)
		{
			Vec2 btnPos = m_rightArrowButton->GetTransform().GetPosition();
			Vec2 btnScreenPos = m_position + btnPos;
			std::cout << "right 버튼 화면 위치: (" << btnScreenPos.x << ", " << btnScreenPos.y << ")" << std::endl;
		}*/
		// 스탯 버튼 영역.
	if (IsMouseOverObject(mousePos, m_statSelectionButton.get()))
	{
		msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
		m_statSelectionButton->GetComponent<ButtonComponent>()->CheckCollision(msg);  // 충돌 검사 먼저
		m_statSelectionButton->GetComponent<ButtonComponent>()->Worked(msg);          // 그 다음 버튼 처리


		SettingWindow* SETWin = dynamic_cast<SettingWindow*>(UIManager::Get().GetWindow(UIWindowType::SettingsWindow));
		if (SETWin)
		{
			float val = SETWin->GetSFXValue();
			SoundManager::Instance().PlaySFX("EH", val);
		}

		//SoundManager::Instance().PlaySFX("EH");
	}

	// left 버튼 영역.
	if (IsMouseOverObject(mousePos, m_leftArrowButton.get()))
	{
		msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
		m_leftArrowButton->GetComponent<ButtonComponent>()->CheckCollision(msg);  // 충돌 검사 먼저
		m_leftArrowButton->GetComponent<ButtonComponent>()->Worked(msg);          // 그 다음 버튼 처리
	}

	// right 버튼 영역.
	if (IsMouseOverObject(mousePos, m_rightArrowButton.get()))
	{
		msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
		m_rightArrowButton->GetComponent<ButtonComponent>()->CheckCollision(msg);  // 충돌 검사 먼저
		m_rightArrowButton->GetComponent<ButtonComponent>()->Worked(msg);          // 그 다음 버튼 처리
	}

	// 주문서 버튼 영역.
	for (auto& btn : m_enhancementButtons)
	{
		if (IsMouseOverObject(mousePos, btn.get()))
		{
			msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
			btn->GetComponent<ButtonComponent>()->CheckCollision(msg);
			btn->GetComponent<ButtonComponent>()->Worked2(msg);          // 그 다음 버튼 처리
		}

	}

	// 슬롯 영역 체크해서 해야함.
	Vec2 slotScreen = m_position + m_enhancementSlot->GetTransform().GetPosition();
	ID2D1Bitmap1* slotBmp = m_enhancementSlot->GetRenderInfo()->GetRenderInfo().bitmap;
	D2D1_SIZE_F size = slotBmp->GetSize();

	if (mousePos.x >= slotScreen.x && mousePos.x <= slotScreen.x + size.width &&
		mousePos.y >= slotScreen.y && mousePos.y <= slotScreen.y + size.height)
	{
		if (m_targetItem != nullptr)
		{
			// 슬롯에 있는 아이템으로 드래그 시작
			CursorManager::Get().StartItemDrag(m_targetItem->m_data.id, DragSource::Enhancement);
			CursorManager::Get().SetDraggedItem(m_targetItem);

			// 슬롯에서 아이템 제거
			m_targetItem = nullptr;

			// 시트 이미지 초기화
			m_renderSheetCount = 0;

			// 주문서 버튼 normal state로 복귀
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

	// 영역 밖이면 체크안함.
	return false;
}

bool EnhancementWindow::HandleMouseUp(Vec2 mousePos)
{
	if (!m_isActive) return false;

	// 메세지 만들어서 던지기.
	MSG msg{};
	msg.message = WM_LBUTTONUP;
	Vec2 relativePos = mousePos - m_position;
	msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);

	// 버튼 클릭 처리
	if (IsMouseOverObject(mousePos, m_statSelectionButton.get()))
	{
		// 상대 좌표로 변환
		//Vec2 relativePos = mousePos + m_position;
		msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);

		m_statSelectionButton->GetComponent<ButtonComponent>()->CheckCollision(msg);  // 충돌 검사 먼저
		m_statSelectionButton->GetComponent<ButtonComponent>()->Worked(msg);          // 그 다음 버튼 처리
	}

	// left 버튼 영역.
	if (IsMouseOverObject(mousePos, m_leftArrowButton.get()))
	{
		//Vec2 relativePos = mousePos + m_position;
		msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);

		m_leftArrowButton->GetComponent<ButtonComponent>()->CheckCollision(msg);  // 충돌 검사 먼저
		m_leftArrowButton->GetComponent<ButtonComponent>()->Worked(msg);          // 그 다음 버튼 처리
	}

	// right 버튼 영역.
	if (IsMouseOverObject(mousePos, m_rightArrowButton.get()))
	{
		//Vec2 relativePos = mousePos + m_position;
		msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);

		m_rightArrowButton->GetComponent<ButtonComponent>()->CheckCollision(msg);  // 충돌 검사 먼저
		m_rightArrowButton->GetComponent<ButtonComponent>()->Worked(msg);          // 그 다음 버튼 처리
	}

	// 주문서 버튼 영역.
	for (auto& btn : m_enhancementButtons)
	{
		if (IsMouseOverObject(mousePos, btn.get()))
		{
			//Vec2 relativePos = mousePos + m_position;
			msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);

			btn->GetComponent<ButtonComponent>()->CheckCollision(msg);  // 충돌 검사 먼저
			btn->GetComponent<ButtonComponent>()->Worked2(msg);          // 그 다음 버튼 처리
		}

		}

		// 슬롯 화면 좌표 계산
		Vec2 slotScreen = m_position + m_enhancementSlot->GetTransform().GetPosition();
		ID2D1Bitmap1* slotBmp = m_enhancementSlot->GetRenderInfo()->GetRenderInfo().bitmap;
		D2D1_SIZE_F size = slotBmp->GetSize();

	// 드래그된 아이템을 슬롯에 드롭할 때 처리
	if (mousePos.x >= slotScreen.x && mousePos.x <= slotScreen.x + size.width &&
		mousePos.y >= slotScreen.y && mousePos.y <= slotScreen.y + size.height)
	{
		if (CursorManager::Get().IsDragging())
		{
			Item* dragged = CursorManager::Get().GetDraggedItem();
			if (dragged)
			{
				// Wearable인지 체크
				if (auto* wear = dynamic_cast<Wearable*>(dragged))
				{	
					// 강화 불가능한 아이템이 들어오면 인벤토리로 반환
					if (!dragged->m_data.enchantable)
					{
						auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
						if (inventoryWindow)
						{
							inventoryWindow->AddItem(dragged->m_data.id, 1);
							std::cout << "강화불가능 아이템을 인벤토리로 반환: " << dragged->m_data.name << std::endl;
							CursorManager::Get().EndItemDrag();
							UIManager::Get().OpenWindow(m_windowType);
							return true;
						}
					}
					// 이미 슬롯에 아이템이 있으면 인벤토리로 반환
					if (m_targetItem != nullptr)
					{
						// 기존 아이템을 인벤토리로 반환
						auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
						if (inventoryWindow)
						{
							ItemDrop(m_targetItem);
							inventoryWindow->AddItem(m_targetItem->m_data.id, 1);
							std::cout << "기존 아이템을 인벤토리로 반환: " << m_targetItem->m_data.name << std::endl;
						}
					}

					m_targetItem = dragged;

					CursorManager::Get().EndItemDrag();

										// 무기 종류에 따라 시트 렌더링 개수 결정
					ItemDrop(m_targetItem);
					UpdateSheetVisibility();

					InitializeItemObject();

					UIManager::Get().OpenWindow(m_windowType);
					return true;
				}
		}
		// 위에 조건 안들어왔으면 창으로 반환.
		DragSource source = CursorManager::Get().GetDragSource();
		return HandleDropFailure(mousePos, dragged, source);
	}
}

		if (IsInBounds(relativePos))
		{
				// 창 영역 내에서 드래그된 아이템이 있으면 인벤토리로 반환
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
			m_statSelectionButton->GetComponent<ButtonComponent>()->SetCollision(true);  // 충돌 여부 설정
			m_statSelectionButton->GetComponent<ButtonComponent>()->Worked(msg);
		}
		else
		{
			m_statSelectionButton->GetComponent<ButtonComponent>()->SetCollision(false);  // 충돌 여부 설정
			m_statSelectionButton->GetComponent<ButtonComponent>()->Worked(msg);
		}
				

		if (IsMouseOverObject(mousePos, m_leftArrowButton.get()))
		{
			m_leftArrowButton->GetComponent<ButtonComponent>()->SetCollision(true);   // 충돌 여부 설정
			m_leftArrowButton->GetComponent<ButtonComponent>()->Worked(msg);
		}
		else
		{
			m_leftArrowButton->GetComponent<ButtonComponent>()->SetCollision(false);   // 충돌 여부 설정
			m_leftArrowButton->GetComponent<ButtonComponent>()->Worked(msg);
		}
				

		if (IsMouseOverObject(mousePos, m_rightArrowButton.get()))
		{
			m_rightArrowButton->GetComponent<ButtonComponent>()->SetCollision(true);    // 충돌 여부 설정
			m_rightArrowButton->GetComponent<ButtonComponent>()->Worked(msg);
		}
		else
		{
			m_rightArrowButton->GetComponent<ButtonComponent>()->SetCollision(false);    // 충돌 여부 설정
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
				// 슬롯에 마우스 오버 시 툴팁 표시. 상대좌표로 변환
				Vec2 relativePos = mousePos - m_position;
				SynSlot whichSlot = SlotInit(relativePos);

				if (whichSlot == SynSlot::Result)
				{
						std::cout << "슬롯에 위치해있음." << std::endl;
						Item* Clicked_Item = m_targetItem;
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
				else
				{

						std::cout << "슬롯 아이템이 없거나 위치 아니여서 닫음." << std::endl;
						UIManager::Get().CloseWindow(UIWindowType::InventoryTooltip);
						CursorManager::Get().HoveredReleased(); //추적 금지 
						return false;
				}
		}
		return true;
}

bool EnhancementWindow::HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source)
{
	// 드래그된 아이템이 없으면 아무것도 하지 않음
	if (!draggedItem) return false;

	// 인벤토리 창의 포인터를 가져옴
	auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));

	// 인벤토리 창이 유효하면 아이템을 추가
	if (inventoryWindow)
	{
		// ItemDrop 함수가 필요한 경우 호출 (코드에 따라 다름)
		ItemDrop(draggedItem);
		inventoryWindow->AddItem(draggedItem->m_data.id, 1);
		std::cout << "강화창 드래그 실패: 아이템을 인벤토리로 복구합니다: " << draggedItem->m_data.name << std::endl;
	}

	// UIManager가 드래그 종료를 처리하겠지만, 안전하게 여기서 호출해줄 수 있음
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

	//		UIManager::Get().ShowTooltip(UIWindowType::InventoryTooltip, tooltipPos); //위치 변경시키고, 활성화까지 
	//		return true;
	//	}
	//	else
	//		return true;

	//}
	//else
	//{
	//	UIManager::Get().CloseWindow(UIWindowType::InventoryTooltip);
	//	CursorManager::Get().HoveredReleased(); //추적 금지 
	//	return false;
	//}
	return true;
}

bool EnhancementWindow::HandleMouseRight(Vec2 mousePos)
{
	if (!IsInBounds(mousePos)) return false;

	// 상대좌표 이슈
	Vec2 relativeMousePos = mousePos - m_position;

	if (SlotInit(relativeMousePos) != SynSlot::Result || m_targetItem == nullptr)
	{
		UIManager::Get().OpenWindow(m_windowType);
		CursorManager::Get().EndItemDrag();
		return true;
	}

	Item* item = m_targetItem;

	Wearable* wearableItem = dynamic_cast<Wearable*>(item);
	// 만약 현재 착용한 아이템 있으면.
	if (wearableItem->Getpart() != Wearable_part::UnKnown)
	{
		// 인벤토리로 복귀
		if (auto inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow)))
		{
			// 슬롯 아이템 지워주기.
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
	auto opacity = m_targetItem->GetComponent<Opacity_Effect>();	// item의 opacity
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
		float rightMargin = 65.0f;  // 47에서 459로 변경
		Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 35 };
		Vec2 closeButtonSize = { 35.0f, 35.0f };  // 27에서 35로 변경

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
		// 창 위치를 더한 최종 화면 좌표
		Vec2 screenPos = m_position + m_enhancementSlot->GetTransform().GetPosition();

		// 슬롯 크기랑 위치 더해주기.
		D2D1_RECT_F destRect = { screenPos.x, screenPos.y, screenPos.x + 74, screenPos.y + 74 };

		// 슬롯 오브젝트 그리기.
		D2DRenderer::Get().DrawBitmap(m_enhancementSlot->GetRenderInfo()->GetRenderInfo().bitmap, destRect);

		// 슬롯 외곽선임.
		D2DRenderer::Get().DrawRectangle(screenPos.x, screenPos.y, screenPos.x + 74, screenPos.y + 74, D2D1::ColorF(D2D1::ColorF::Black));

		// 슬롯에 등록된 아이템 아이콘 표시
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
				//		// destRect 크기에 맞춰서 아이템 그리기
				//		D2DRenderer::Get().DrawBitmap(itemAtlas, destRect, src);
				//}
			m_targetItem->GetTransform().SetPosition({ screenPos.x - 91.f * 0.8f + 8.f, screenPos.y - 91.f * 0.8f + 6.f });
			D2DRenderer::Get().DrawBitmap(m_targetItem->GetRenderInfo()->GetRenderInfo());
		}
}

void EnhancementWindow::RenderSheetImages()
{
	if (!m_targetItem) return;
	//std::cout << "아이템있음" << std::endl;

	Wearable* wearableItem = dynamic_cast<Wearable*>(m_targetItem);
	if (!wearableItem) return;

	auto& enchResults = wearableItem->GetEnchancResult();

	size_t count = m_normalImages.size();
	size_t renderCount = std::min(static_cast<size_t>(m_renderSheetCount), count);

	size_t startIndex = (wearableItem->Getpart() == Wearable_part::Weapon) ? 0 : 1;

	// 3이 들어가는데 안뜸.
	//std::cout << "시트 갯수 :" << renderCount << std::endl;
	
	// 정순으로 렌더링 (0->1->2->3->4)
	for (size_t i = 0; i < renderCount; ++i)
	{
		
		// 방어구면 시작 인덱스 1 추가.
		size_t sheetIndex = startIndex + i;
		//if (sheetIndex >= m_sheetImages.size()) break;

		//auto& sheet = m_sheetImages[sheetIndex];
		auto& sheet1 = m_normalImages[sheetIndex];
		auto& sheet2 = m_successImages[sheetIndex];
		auto& sheet3 = m_failImages[sheetIndex];
		//auto bgComp = sheet->GetComponent<BackgroundComponent>();
		//if (!bgComp) continue;

		//// 상태별 비트맵 선택
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

		// 화면에 렌더링
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
		// 중앙 버튼 
		Vec2 screenPos = m_position + m_statSelectionButton->GetTransform().GetPosition();
		auto statSelectionInfo = m_statSelectionButton->GetRenderInfo();
		statSelectionInfo->SetDestRect({ screenPos.x, screenPos.y, screenPos.x + statSelectionInfo->GetSize().width, screenPos.y + statSelectionInfo->GetSize().height});

		D2DRenderer::Get().DrawBitmap(statSelectionInfo->GetRenderInfo());
		// 렌더링
		//if (bmp)
		//{
				//D2D1_SIZE_F size = bmp->GetSize();
				//D2D1_RECT_F dest = { screenPos.x, screenPos.y, screenPos.x + size.width, screenPos.y + size.height };
				//D2DRenderer::Get().DrawBitmap(bmp, dest);
		//}

		// 좌측 화살표 버튼 렌더링
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

		// 우측 화살표 버튼 렌더링
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
		Vec2 textPos = { buttonPos.x + 152, buttonPos.y + 15 }; // 버튼 중앙에 텍스트 배치

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

		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, PtToPx(12));
		D2DRenderer::Get().DrawMessage(statText.c_str(), textPos.x, textPos.y, 150, 70, D2D1::ColorF(D2D1::ColorF::White));
}

void EnhancementWindow::RenderScrollButtons()
{

// 강화 버튼 3개 렌더링 
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


// 강화 버튼 누르는 경우. 
void EnhancementWindow::TryEnhance(int successRate)
{
		// 현재 버튼이 누를테니 successRate로 rand를 돌림.
		// 성공, 실패 반환, 이펙트 그릴거임. bool로 render에 이펙트 그리기. 
		// 아이템 정보 업데이트. 
		if (!m_targetItem) return;

		Wearable* wearableItem = dynamic_cast<Wearable*>(m_targetItem);

		if (!wearableItem || wearableItem->GetEnchanCount() <= 0)
				return;

		// 랜덤 성공/실패 결정
		std::uniform_int_distribution<int> dist(1, 100);

		int roll = dist(m_rng);
		bool success = roll <= successRate;
		//bool success = true;
		auto& results = wearableItem->GetEnchancResult();
		int currentIndex = -1;

		for (int i = 0; i < results.size(); ++i)
		{
				if (results[i] == EnchancerType::Default) //  기본값 확인
				{
						currentIndex = i;
						break;
				}
		}

		if (currentIndex == -1)
		{
				// 빈 슬롯 없음. 모두 기록되어있음무시
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
				std::cout << "강화 성공!" << std::endl;
				// 성공 애니메이션 및 아이템 스탯 업데이트
				// 아이템 정보 가져와서 스탯 올려주기.
				wearableItem->GetEnchancResult()[currentIndex] = EnchancerType::Sucess;
				// 선택된 스탯에 따라 수치 추가
				int statValue = GetSelectedStatValue();
				fundamentalStatus addStat = {};
				switch (m_selectedStat)
				{
				case Stat::STR:
						addStat.power = statValue;
						std::cout << "STR + " << statValue << " 추가!" << std::endl;
						break;
				case Stat::DEX:
						addStat.agile = statValue;
						std::cout << "DEX + " << statValue << " 추가!" << std::endl;
						break;
				case Stat::INT:
						addStat.intelligence = statValue;
						std::cout << "INT + " << statValue << " 추가!" << std::endl;
						break;
				case Stat::LUK:
						addStat.luck = statValue;
						std::cout << "LUK + " << statValue << " 추가!" << std::endl;
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

				std::cout << "강화 실패..." << std::endl;
				// 실패 애니메이션
				// 실패로 바꿔주기.
				wearableItem->GetEnchancResult()[currentIndex] = EnchancerType::Fail;
		}

		// 카운트 빼주기.
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

	// 무기인 경우 5개, 그 외는 3개로 강화 슬롯 개수 설정
	int newSheetCount = (wearableItem->Getpart() == Wearable_part::Weapon) ? 5 : 3;

	// m_renderSheetCount 업데이트
	m_renderSheetCount = newSheetCount;

	// 만약 아이템의 강화 이력 벡터가 비어있다면 (새로운 아이템), 초기화
	if (wearableItem->GetEnchancResult().empty())
	{
		wearableItem->GetEnchancResult().assign(m_renderSheetCount, EnchancerType::Default);
	}

	auto enchResults = wearableItem->GetEnchancResult();
	for (int i = 0; i < 5; i++)
	{
		// 상태별 비트맵 선택
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
	m_targetItem->GetTransform().SetScale({0.8f, 0.8f});	// 렌더 쪽에서 position 하드코딩해서 조정해야함..ㅋ
	
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
		// 마우스 오버 체크 로직 구현
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

		// 현재 상태에 따른 스케일 계산
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
		// 스탯 선택 버튼 콜백
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

		// 좌측 화살표 버튼 콜백
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

		// 우측 화살표 버튼 콜백
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

		// 강화 버튼들 콜백
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
		std::cout << "주문서를 선택해주세요" << std::endl;
		return;
	}

	// 눌린 scroll 버튼 normal state로 변경
	//m_pressedButton->SetState(ButtonComponent::ButtonState::Normal);
	//m_pressedButton->SetOpacity(1.f);
	std::cout << "스탯 강화 버튼 클릭됨" << std::endl;
	if (m_targetItem == nullptr)	return;	// 강화창에 아이템 없으면 리턴

	//auto runeFade = m_targetItem->GetComponent<Fade_Effect>();
	//runeFade->OnEvent("HIDE");
	auto success = m_targetItem->GetComponent<Increasing_Effect>();
	auto state = success->GetCurrentState();
	if (state != "DORMANT")	return;

	if (!m_targetItem->m_data.enchantable)
	{
		std::cout << "강화 불가능 아이템." << std::endl;
		std::cout << "강화 불가능 아이템." << std::endl;
		return;
	}

	if (!m_targetItem->m_data.enchantable)
	{

			std::cout << "강화 불가능 아이템." << std::endl;
			return;
	}

	//if (m_selectedScrollIndex >= m_enhancementButtons.size())	// 얘는 작동 안 하는 듯
	//{
	//	std::cout << "주문서를 선택해주세요" << std::endl;
	//	return;
	//}

	// 버튼 인덱스에 따른 성공률
	int successRate = 0;
	switch (m_selectedScrollIndex)
	{
	case 0: successRate = 100; break;
	case 1: successRate = 40; break;
	case 2: successRate = 20; break;
	default: successRate = 0; break;
	}

	std::cout << "강화 시도: 성공률 " << successRate << "%" << std::endl;

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
		currentStat = (currentStat - 1 + 4) % 4; // 4개 스탯 순환
		m_selectedStat = static_cast<Stat>(currentStat);
}

void EnhancementWindow::OnRightArrowClick()
{
		// 다음 스탯으로 변경
		int currentStat = static_cast<int>(m_selectedStat);
		currentStat = (currentStat + 1) % 4; // 4개 스탯 순환
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
	//// 새로 선택된 버튼 opacity 설정
	m_selectedScrollIndex = buttonIndex;

	if (!m_targetItem)
	{
		std::cout << "강화할 아이템이 없습니다" << std::endl;
		return;
	}

	// 버튼 인덱스에 따른 성공률
	int successRate = 0;
	switch (buttonIndex)
	{
	case 0: successRate = 100; break;
	case 1: successRate = 40; break;
	case 2: successRate = 20; break;
	default: successRate = 0; break;
	}

	std::cout << "주문서 선택: 성공률 " << successRate << "%, 스탯 +" << GetSelectedStatValue() << std::endl;

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
		case 0: return 1;  // 100% 주문서 = +1
		case 1: return 2;  // 40% 주문서 = +2  
		case 2: return 5;  // 20% 주문서 = +5
		default: return 0;
		}
}

void EnhancementWindow::ReturnItemToInventory()
{
	if (m_targetItem != nullptr)
	{
		// 인벤토리로 아이템 반환
		auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
		if (inventoryWindow)
		{
			inventoryWindow->AddItem(m_targetItem->m_data.id, 1);
			std::cout << "강화창 닫기 전 아이템 인벤토리로 반환: " << m_targetItem->m_data.name << std::endl;

			// 슬롯 비우기
			m_targetItem = nullptr;

			// 시트 이미지 초기화						m_renderSheetCount = 0;
		}
	}
	// 주문서 버튼 normal state로 복귀
	for (auto& scroll : m_enhancementButtons)
	{
		scroll->GetComponent<ButtonComponent>()->SetState(ButtonComponent::ButtonState::Normal);

	}
}

SynSlot EnhancementWindow::SlotInit(Vec2 pos)
{
		// 슬롯 위치 return 있으면 result 없으면 nothing임.
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


