#include "EnhancementWindow.h"

#include <windows.h>
#undef min
#undef max
#include <algorithm>

#include "Wearable.h"

void EnhancementWindow::Update()
{
		// 여기에 오브젝트 얘들 전부 component update 시키기.
		
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
		SoundManager::Instance().PlaySFX("EH");
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
						btn->GetComponent<ButtonComponent>()->Worked(msg);          // 그 다음 버튼 처리
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
						btn->GetComponent<ButtonComponent>()->Worked(msg);          // 그 다음 버튼 처리
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
					UIManager::Get().OpenWindow(m_windowType);
					return true;
				}
		}
		// 위에 조건 안들어왔으면 창으로 반환.
		DragSource source = CursorManager::Get().GetDragSource();
		return HandleDropFailure(mousePos, dragged, source);
	}
}

		if (IsInBounds(mousePos))
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
				Vec2 relativePos = mousePos - m_position;
				msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
				m_statSelectionButton->GetComponent<ButtonComponent>()->CheckCollision(msg);  // 충돌 검사 먼저
				m_statSelectionButton->GetComponent<ButtonComponent>()->Worked(msg);
		}
			

		if (IsMouseOverObject(mousePos, m_leftArrowButton.get()))
		{
				Vec2 relativePos = mousePos - m_position;
				msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
				m_leftArrowButton->GetComponent<ButtonComponent>()->CheckCollision(msg);  // 충돌 검사 먼저
				m_leftArrowButton->GetComponent<ButtonComponent>()->Worked(msg);
		}
				

		if (IsMouseOverObject(mousePos, m_rightArrowButton.get()))
		{
				Vec2 relativePos = mousePos - m_position;
				msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
				m_rightArrowButton->GetComponent<ButtonComponent>()->CheckCollision(msg);  // 충돌 검사 먼저
				m_rightArrowButton->GetComponent<ButtonComponent>()->Worked(msg);
		}
				

		for (auto& btn : m_enhancementButtons)
		{
				if (IsMouseOverObject(mousePos, btn.get()))
				{
						Vec2 relativePos = mousePos - m_position;
						msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
						btn->GetComponent<ButtonComponent>()->CheckCollision(msg);  // 충돌 검사 먼저
						btn->GetComponent<ButtonComponent>()->Worked(msg);
				}
		}
				

		for (auto& btn : m_enhancementButtons)
		{
				if (IsMouseOverButton(mousePos, btn.get()))
				{
						Vec2 relativePos = mousePos - m_position;
						msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
						btn->GetComponent<ButtonComponent>()->CheckCollision(msg);  // 충돌 검사 먼저
						btn->GetComponent<ButtonComponent>()->Worked(msg);
				}
		}
		
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
	if (!draggedItem) return false;

	// 다른 창들의 영역인지 확인
	bool isInOtherWindow = false;

	// 인벤창 영역 확인
	UIWindow* inventoryWindow = UIManager::Get().GetWindow(UIWindowType::InventoryWindow);
	if (inventoryWindow && inventoryWindow->IsActive() && inventoryWindow->IsInBounds(mousePos))
	{
		isInOtherWindow = true;
	}

	// 장비창 영역 확인
	UIWindow* equipmentWindow = UIManager::Get().GetWindow(UIWindowType::EquipmentWindow);
	if (equipmentWindow && equipmentWindow->IsActive() && equipmentWindow->IsInBounds(mousePos))
	{
		isInOtherWindow = true;
	}

	// 다른 창 영역이면 해당 창에서 처리하도록 넘김
	if (isInOtherWindow)
	{
		return false;
	}

	// 어떤 창 영역도 아니면 원래 위치로 복구
	if (source == DragSource::Equipment  || source == DragSource::Inventory  || source == DragSource::Enhancement || source == DragSource::Synthesis)
	{
		// sheetimage 다시 render 해야해서 그냥 inven으로 복구.
		auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
		if (inventoryWindow)
		{
			ItemDrop(draggedItem); //-> soundmanager 
			inventoryWindow->AddItem(draggedItem->m_data.id, 1);
			std::cout << "인벤토리 아이템을 인벤토리로 복구했습니다: " << draggedItem->m_data.name << std::endl;
		}
	}

	CursorManager::Get().EndItemDrag();
	return true;
}
bool EnhancementWindow::HandleMouseRight(Vec2 mousePos)
{
	if (!IsInBounds(mousePos)) return false;
	
	if (SlotInit(mousePos) != SynSlot::Result || m_targetItem == nullptr)
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
				if (auto* inventory = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow)))
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
			D2D_RECT_F src = ResourceManager::Get().Get_ItemBank().GetItemClip(m_targetItem->m_data.id)->srcRect;
			src.left = src.left + 80.f;
			src.top = src.top + 80.f;
			src.right = src.right - 80.f;
			src.bottom = src.bottom - 80.f;
				auto clip = ResourceManager::Get().Get_ItemBank().GetItemClip(m_targetItem->m_data.id);
				ID2D1Bitmap1* itemAtlas = clip->atlas.Get();
				if (itemAtlas)
				{
						// destRect 크기에 맞춰서 아이템 그리기
						D2DRenderer::Get().DrawBitmap(itemAtlas, destRect, src);
				}
		}
}

void EnhancementWindow::RenderSheetImages()
{
	if (!m_targetItem) return;

	Wearable* wearableItem = dynamic_cast<Wearable*>(m_targetItem);
	if (!wearableItem) return;

	auto& enchResults = wearableItem->GetEnchancResult();

	size_t count = m_sheetImages.size();
	size_t renderCount = std::min(static_cast<size_t>(m_renderSheetCount), count);

	size_t startIndex = (wearableItem->Getpart() == Wearable_part::Weapon) ? 0 : 1;


	// 정순으로 렌더링 (0->1->2->3->4)
	for (size_t i = 0; i < renderCount; ++i)
	{
		// 방어구면 시작 인덱스 1 추가.
		size_t sheetIndex = startIndex + i;
		if (sheetIndex >= m_sheetImages.size()) break;

		auto& sheet = m_sheetImages[sheetIndex];
		auto bgComp = sheet->GetComponent<BackgroundComponent>();
		if (!bgComp) continue;

		// 상태별 비트맵 선택
		switch (enchResults[i])
		{
		case EnchancerType::Default:
			bgComp->SetCurrentBitmap("Normal");
			break;
		case EnchancerType::Sucess:
			bgComp->SetCurrentBitmap("Success");
			break;
		case EnchancerType::Fail:
			bgComp->SetCurrentBitmap("Fail");
			break;
		}

		// 화면에 렌더링
		ID2D1Bitmap1* bmp = sheet->GetRenderInfo()->GetRenderInfo().bitmap;
		Vec2 pos = m_position + sheet->GetTransform().GetPosition();

		if (bmp)
		{
			D2D1_SIZE_F size = bmp->GetSize();
			D2D1_RECT_F dest = { pos.x, pos.y, pos.x + size.width, pos.y + size.height };
			D2DRenderer::Get().DrawBitmap(bmp, dest);
		}
	}
}

void EnhancementWindow::RenderStatSelectionButtons()
{
		// 중앙 버튼 
		Vec2 screenPos = m_position + m_statSelectionButton->GetTransform().GetPosition();
		ID2D1Bitmap1* bmp = m_statSelectionButton->GetRenderInfo()->GetRenderInfo().bitmap;
		// 렌더링
		if (bmp)
		{
				D2D1_SIZE_F size = bmp->GetSize();
				D2D1_RECT_F dest = { screenPos.x, screenPos.y, screenPos.x + size.width, screenPos.y + size.height };
				D2DRenderer::Get().DrawBitmap(bmp, dest);
		}

		// 좌측 화살표 버튼 렌더링
		screenPos = m_position + m_leftArrowButton->GetTransform().GetPosition();
		bmp = m_leftArrowButton->GetRenderInfo()->GetRenderInfo().bitmap;

		if (bmp)
		{
				D2D1_SIZE_F size = bmp->GetSize();
				D2D1_RECT_F dest = { screenPos.x, screenPos.y, screenPos.x + size.width, screenPos.y + size.height };
				D2DRenderer::Get().DrawBitmap(bmp, dest);
		}

		// 우측 화살표 버튼 렌더링
		screenPos = m_position + m_rightArrowButton->GetTransform().GetPosition();
		bmp = m_rightArrowButton->GetRenderInfo()->GetRenderInfo().bitmap;

		if (bmp)
		{
				D2D1_SIZE_F size = bmp->GetSize();
				D2D1_RECT_F dest = { screenPos.x, screenPos.y, screenPos.x + size.width, screenPos.y + size.height };
				D2DRenderer::Get().DrawBitmap(bmp, dest);
		}
}


void EnhancementWindow::RenderStatText()
{
		Vec2 buttonPos = m_position + m_statSelectionButton->GetTransform().GetPosition();
		Vec2 textPos = { buttonPos.x + 135, buttonPos.y + 15 }; // 버튼 중앙에 텍스트 배치

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

		D2DRenderer::Get().DrawMessage(statText.c_str(), textPos.x, textPos.y, 150, 70, D2D1::ColorF(D2D1::ColorF::White));
}

void EnhancementWindow::RenderScrollButtons()
{

// 강화 버튼 3개 렌더링 
	for (size_t i = 0; i < m_enhancementButtons.size(); ++i)
	{
		auto& btn = m_enhancementButtons[i];
		Vec2 screenPos = m_position + btn->GetTransform().GetPosition();
		ID2D1Bitmap1* bmp = btn->GetRenderInfo()->GetRenderInfo().bitmap;

		if (bmp)
		{
			D2D1_SIZE_F size = bmp->GetSize();
			D2D1_RECT_F dest = { screenPos.x, screenPos.y, screenPos.x + size.width, screenPos.y + size.height };
			D2DRenderer::Get().DrawBitmap(bmp, dest);
		}
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
			SoundManager::Instance().PlaySFX("SC");
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
			SoundManager::Instance().PlaySFX("FA");

				std::cout << "강화 실패..." << std::endl;
				// 실패 애니메이션
				// 실패로 바꿔주기.
				wearableItem->GetEnchancResult()[currentIndex] = EnchancerType::Fail;
		}

		// 카운트 빼주기.
		wearableItem->UseEnhanceChance();

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
	std::cout << "스탯 강화 버튼 클릭됨" << std::endl;


	if (!m_targetItem)
	{
		std::cout << "강화할 아이템이 없습니다" << std::endl;
		return;
	}

	if (!m_targetItem->m_data.enchantable)
	{

			std::cout << "강화 불가능 아이템." << std::endl;
			return;
	}

	if (m_selectedScrollIndex >= m_enhancementButtons.size())
	{
		std::cout << "주문서를 선택해주세요" << std::endl;
		return;
	}

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
		if (!m_targetItem)
		{
				std::cout << "강화할 아이템이 없습니다" << std::endl;
				return;
		}

		// 이전 선택 버튼 opacity 복원
		if (m_selectedScrollIndex < m_enhancementButtons.size())
		{
				auto prevBtnComponent = m_enhancementButtons[m_selectedScrollIndex]->GetComponent<ButtonComponent>();
				if (prevBtnComponent) prevBtnComponent->SetOpacity(0.7f);
		}

		// 새로 선택된 버튼 opacity 설정
		m_selectedScrollIndex = buttonIndex;
		auto btnComponent = m_enhancementButtons[buttonIndex]->GetComponent<ButtonComponent>();
		if (btnComponent) btnComponent->SetOpacity(1.0f);

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

						// 시트 이미지 초기화
						m_renderSheetCount = 0;
				}
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


