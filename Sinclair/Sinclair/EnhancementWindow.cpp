#include "EnhancementWindow.h"

#include <windows.h>
#undef min
#undef max
#include <algorithm>

void EnhancementWindow::Update()
{
		// ���⿡ ������Ʈ ��� ���� component update ��Ű��.
		
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
	std::cout << "HandleMouseDown - ���콺 ��ġ: (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
		if (!m_isActive) return false;

		// �޼��� ���� ������.
		MSG msg{};
		msg.message = WM_LBUTTONDOWN;
		Vec2 relativePos = mousePos - m_position;
		msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
		if (m_statSelectionButton)
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
		}
		// ���� ��ư ����.
		if (IsMouseOverObject(mousePos, m_statSelectionButton.get()))
		{
				// ��� ��ǥ�� ��ȯ
				Vec2 relativePos = mousePos + m_position;
				msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
				m_statSelectionButton->GetComponent<ButtonComponent>()->Worked(msg);
		}

		// left ��ư ����.
		if (IsMouseOverObject(mousePos, m_leftArrowButton.get()))
		{
				Vec2 relativePos = mousePos + m_position;
				msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
				m_leftArrowButton->GetComponent<ButtonComponent>()->Worked(msg);
		}

		// right ��ư ����.
		if (IsMouseOverObject(mousePos, m_rightArrowButton.get()))
		{
				Vec2 relativePos = mousePos + m_position;
				msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
				m_rightArrowButton->GetComponent<ButtonComponent>()->Worked(msg);
		}

		// �ֹ��� ��ư ����.
		for (auto& btn : m_enhancementButtons)
		{
				if (IsMouseOverObject(mousePos, btn.get()))
				{
						Vec2 relativePos = mousePos + m_position;
						msg.lParam = MAKELPARAM((int)relativePos.x, (int)relativePos.y);
						btn->GetComponent<ButtonComponent>()->Worked(msg);
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
		MSG msg{};
		msg.message = WM_LBUTTONUP;
		msg.lParam = MAKELPARAM((int)mousePos.x, (int)mousePos.y);

		// ��ư Ŭ�� ó��
		if (IsMouseOverObject(mousePos, m_statSelectionButton.get()))
				m_statSelectionButton->GetComponent<ButtonComponent>()->Worked(msg);

		if (IsMouseOverObject(mousePos, m_leftArrowButton.get()))
				m_leftArrowButton->GetComponent<ButtonComponent>()->Worked(msg);

		if (IsMouseOverObject(mousePos, m_rightArrowButton.get()))
				m_rightArrowButton->GetComponent<ButtonComponent>()->Worked(msg);

		for (auto& btn : m_enhancementButtons)
				if (IsMouseOverObject(mousePos, btn.get()))
						btn->GetComponent<ButtonComponent>()->Worked(msg);

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
										// �̹� ���Կ� �������� ������ �κ��丮�� ��ȯ
										if (m_targetItem != nullptr)
										{
												// ���� �������� �κ��丮�� ��ȯ
												auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
												if (inventoryWindow)
												{
														inventoryWindow->AddItem(m_targetItem->m_data.id, 1);
														std::cout << "���� �������� �κ��丮�� ��ȯ: " << m_targetItem->m_data.name << std::endl;
												}
										}

										m_targetItem = dragged;
										CursorManager::Get().EndItemDrag();

										// ���� ������ ���� ��Ʈ ������ ���� ����
										UpdateSheetVisibility();
										UIManager::Get().OpenWindow(m_windowType);
										return true;
								}
								else
								{
										// Wearable�� �ƴϸ� ���� â���� ��ȯ
										DragSource source = CursorManager::Get().GetDragSource();
										return HandleDropFailure(mousePos, dragged, source);
								}
						}
				}
		}

		if (IsInBounds(mousePos))
		{
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
				m_statSelectionButton->GetComponent<ButtonComponent>()->Worked(msg);

		if (IsMouseOverObject(mousePos, m_leftArrowButton.get()))
				m_leftArrowButton->GetComponent<ButtonComponent>()->Worked(msg);

		if (IsMouseOverObject(mousePos, m_rightArrowButton.get()))
				m_rightArrowButton->GetComponent<ButtonComponent>()->Worked(msg);

		for (auto& btn : m_enhancementButtons)
				if (IsMouseOverObject(mousePos, btn.get()))
						btn->GetComponent<ButtonComponent>()->Worked(msg);

		for (auto& btn : m_enhancementButtons)
				if (IsMouseOverButton(mousePos, btn.get()))
						btn->GetComponent<ButtonComponent>()->Worked(msg);

		return true;
}

bool EnhancementWindow::HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source)
{
		if (!draggedItem) return false;

		// �ٸ� â���� �������� Ȯ��
		bool isInOtherWindow = false;

		// �κ�â ���� Ȯ��
		UIWindow* inventoryWindow = UIManager::Get().GetWindow(UIWindowType::InventoryWindow);
		if (inventoryWindow && inventoryWindow->IsActive() && inventoryWindow->IsInBounds(mousePos))
		{
				isInOtherWindow = true;
		}

		// ���â ���� Ȯ��
		UIWindow* equipmentWindow = UIManager::Get().GetWindow(UIWindowType::EquipmentWindow);
		if (equipmentWindow && equipmentWindow->IsActive() && equipmentWindow->IsInBounds(mousePos))
		{
				isInOtherWindow = true;
		}

		// �ٸ� â �����̸� �ش� â���� ó���ϵ��� �ѱ�
		if (isInOtherWindow)
		{
				return false;
		}

		// � â ������ �ƴϸ� ���� ��ġ�� ����
		if (source == DragSource::Equipment)
		{
				// ���â���� ���� �������̹Ƿ� �ٽ� ����
				Wearable* wearableItem = dynamic_cast<Wearable*>(draggedItem);
				if (wearableItem)
				{
						auto* equipWindow = dynamic_cast<EquipmentWindow*>(UIManager::Get().GetWindow(UIWindowType::EquipmentWindow));
						if (equipWindow)
						{
								equipWindow->EquipItem(draggedItem);
								std::cout << "��� �������� ���� �������� �����߽��ϴ�: " << draggedItem->m_data.name << std::endl;
						}
				}
		}
		else if (source == DragSource::Inventory)
		{
				// �κ��丮���� �� �������̹Ƿ� �κ��丮�� ����
				auto* inventoryWindow = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
				if (inventoryWindow)
				{
						inventoryWindow->AddItem(draggedItem->m_data.id, 1);
						std::cout << "�κ��丮 �������� �κ��丮�� �����߽��ϴ�: " << draggedItem->m_data.name << std::endl;
				}
		}
		else if (source == DragSource::Enhancement)
		{
				// ��ȭâ ���Կ��� �� �������̹Ƿ� �ٽ� ���Կ� ��ġ
				m_targetItem = draggedItem;
				std::cout << "��ȭâ �������� �������� �����߽��ϴ�: " << draggedItem->m_data.name << std::endl;
		}

		CursorManager::Get().EndItemDrag();
		return true;
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
		float rightMargin = 47.0f;
		Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 7 };
		Vec2 closeButtonSize = { 27.0f, 27.0f };
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
				auto& clip = ResourceManager::Get().Get_ItemBank().GetItemClip(m_targetItem->m_data.id);
				ID2D1Bitmap1* itemAtlas = clip.atlas.Get();
				if (itemAtlas)
				{
						// destRect ũ�⿡ ���缭 ������ �׸���
						D2DRenderer::Get().DrawBitmap(itemAtlas, destRect, clip.srcRect);
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

		
		//// �������� ������ (5->4->3->2->1)
		//for (int i = renderCount - 1; i >= 0; --i)
		//{
		//		auto& sheet = m_sheetImages[i];
		//		auto bgComp = sheet->GetComponent<BackgroundComponent>();
		//		if (!bgComp) continue;
		//		// ���º� ��Ʈ�� ����
		//		switch (enchResults[i])
		//		{
		//		case EnchancerType::Default:
		//				bgComp->SetCurrentBitmap("Normal");
		//				break;
		//		case EnchancerType::Sucess:
		//				bgComp->SetCurrentBitmap("Success");
		//				break;
		//		case EnchancerType::Fail:
		//				bgComp->SetCurrentBitmap("Fail");
		//				break;
		//		default:
		//				continue;
		//		}

		//		// ȭ�鿡 ������
		//		ID2D1Bitmap1* bmp = sheet->GetRenderInfo()->GetRenderInfo().bitmap;
		//		Vec2 pos = m_position + sheet->GetTransform().GetPosition();

		//		if (bmp)
		//		{
		//				D2D1_SIZE_F size = bmp->GetSize();
		//				D2D1_RECT_F dest = { pos.x, pos.y, pos.x + size.width, pos.y + size.height };
		//				D2DRenderer::Get().DrawBitmap(bmp, dest);
		//		}
		//}

		// �������� ������ (0->1->2->3->4)
		for (size_t i = 0; i < renderCount; ++i)
		{
				auto& sheet = m_sheetImages[i];
				auto bgComp = sheet->GetComponent<BackgroundComponent>();
				if (!bgComp) continue;

				// ���º� ��Ʈ�� ����
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

				// ȭ�鿡 ������
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
		// �߾� ��ư 
		Vec2 screenPos = m_position + m_statSelectionButton->GetTransform().GetPosition();
		ID2D1Bitmap1* bmp = m_statSelectionButton->GetRenderInfo()->GetRenderInfo().bitmap;
		// ������
		if (bmp)
		{
				D2D1_SIZE_F size = bmp->GetSize();
				D2D1_RECT_F dest = { screenPos.x, screenPos.y, screenPos.x + size.width, screenPos.y + size.height };
				D2DRenderer::Get().DrawBitmap(bmp, dest);
		}

		// ���� ȭ��ǥ ��ư ������
		screenPos = m_position + m_leftArrowButton->GetTransform().GetPosition();
		bmp = m_leftArrowButton->GetRenderInfo()->GetRenderInfo().bitmap;

		if (bmp)
		{
				D2D1_SIZE_F size = bmp->GetSize();
				D2D1_RECT_F dest = { screenPos.x, screenPos.y, screenPos.x + size.width, screenPos.y + size.height };
				D2DRenderer::Get().DrawBitmap(bmp, dest);
		}

		// ���� ȭ��ǥ ��ư ������
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
		Vec2 textPos = { buttonPos.x + 150, buttonPos.y + 15 }; // ��ư �߾ӿ� �ؽ�Ʈ ��ġ

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

		D2DRenderer::Get().DrawMessage(statText.c_str(), textPos.x, textPos.y, 100, 30, D2D1::ColorF(D2D1::ColorF::White));
}

void EnhancementWindow::RenderScrollButtons()
{

// ��ȭ ��ư 3�� ������ 
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
						wearableItem->AddStat(addStat, statValue);
						std::cout << "STR + " << statValue << " �߰�!" << std::endl;
						break;
				case Stat::DEX:
						addStat.agile = statValue;
						wearableItem->AddStat(addStat, statValue);
						std::cout << "DEX + " << statValue << " �߰�!" << std::endl;
						break;
				case Stat::INT:
						addStat.intelligence = statValue;
						wearableItem->AddStat(addStat, statValue);
						std::cout << "INT + " << statValue << " �߰�!" << std::endl;
						break;
				case Stat::LUK:
						addStat.luck = statValue;
						wearableItem->AddStat(addStat, statValue);
						std::cout << "LUK + " << statValue << " �߰�!" << std::endl;
						break;
				}
		}
		else
		{
				std::cout << "��ȭ ����..." << std::endl;
				// ���� �ִϸ��̼�
				// ���з� �ٲ��ֱ�.
				wearableItem->GetEnchancResult()[currentIndex] = EnchancerType::Fail;
		}

		// ī��Ʈ ���ֱ�.
		wearableItem->UseEnhanceChance();

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
		std::cout << "���� ��ȭ ��ư Ŭ����" << std::endl;

		if (!m_targetItem)
		{
				std::cout << "��ȭ�� �������� �����ϴ�" << std::endl;
				return;
		}

		if (m_selectedScrollIndex >= m_enhancementButtons.size())
		{
				std::cout << "�ֹ����� �������ּ���" << std::endl;
				return;
		}

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
		if (!m_targetItem)
		{
				std::cout << "��ȭ�� �������� �����ϴ�" << std::endl;
				return;
		}

		// ���� ���� ��ư opacity ����
		if (m_selectedScrollIndex < m_enhancementButtons.size())
		{
				auto prevBtnComponent = m_enhancementButtons[m_selectedScrollIndex]->GetComponent<ButtonComponent>();
				if (prevBtnComponent) prevBtnComponent->SetOpacity(0.7f);
		}

		// ���� ���õ� ��ư opacity ����
		m_selectedScrollIndex = buttonIndex;
		auto btnComponent = m_enhancementButtons[buttonIndex]->GetComponent<ButtonComponent>();
		if (btnComponent) btnComponent->SetOpacity(1.0f);

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


