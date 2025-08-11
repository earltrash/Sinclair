#include "UIWindow.h"
#include "UIManager.h"

//bool UIWindow::HandleInput(const MSG& msg)
//{
//	{
//		// 창이 비활성화되어 있으면 처리 안함
//		if (!m_isActive)
//			return false;
//
//		Vec2 CORD = { static_cast<float>(F_GET_X_LPARAM(msg.lParam)), static_cast<float>(F_GET_Y_LPARAM(msg.lParam)) };
//
//		if (IsInBounds(CORD) && msg.message == WM_LBUTTONDOWN)
//		{
//			std::cout << "입력받은 X" << CORD.x << "입력받은 Y" << CORD.y << endl;
//			return HandleMouseDown(CORD);
//		}
//
//		/*if (IsInBounds(CORD) &&  msg.message == WM_LBUTTONUP)
//		{
//			return HandleMouseUp(CORD);
//		}*/
//
//		if (msg.message == WM_LBUTTONUP)
//		{
//			return HandleMouseUp(CORD);
//		}
//				//break;
//
//		return HandleMouseHover(CORD);
//
//		return true; // 창 영역 안의 입력은 모두 "처리됨"으로 간주
//	}
//		return false;
//}


bool UIWindow::HandleInput(const MSG& msg)
{
		// 창이 비활성화되어 있으면 처리 안함 -> 음.. 그럼 다른 창 함수 내에서 이걸 할 필요는 없네 
		if (!m_isActive)
				return false;


		Vec2 CORD = { static_cast<float>(F_GET_X_LPARAM(msg.lParam)), static_cast<float>(F_GET_Y_LPARAM(msg.lParam)) };

		// 1. WM_LBUTTONUP은 드래그 상태를 해제해야 하므로 가장 먼저 처리
		if (msg.message == WM_LBUTTONDOWN)
		{
				// 닫기창이면 끔.
				if (IsInCloseButton(CORD))
				{
						std::cout << "닫기 할거임." << std::endl;

						// 강화창인 경우 슬롯의 아이템을 인벤토리로 반환
						if (m_windowType == UIWindowType::EnhancementWindow)
						{
								auto* enhancementWindow = dynamic_cast<EnhancementWindow*>(this);
								if (enhancementWindow)
								{
										enhancementWindow->ReturnItemToInventory();
								}
						}
						// 합성창인 경우 슬롯의 아이템들을 인벤토리로 반환
						else if (m_windowType == UIWindowType::SynthesisWindow)
						{
								auto* synthesisWindow = dynamic_cast<SynthesisWin*>(this);
								if (synthesisWindow)
								{
										synthesisWindow->ReturnItemToInventory();
								}
						}

						UIManager::Get().CloseWindow(m_windowType);
						return true;
				}
				// 타이틀바면 드래그.
				if (IsInTitleBar(CORD)) //얘 없는 애들은 다 막아야 함. 
				{
						std::cout << "드래그 할거임." << std::endl;
						if(GetType() != UIWindowType::InventoryTooltip)
						{
							m_isDragging = true;
						}
						
						m_dragOffset = CORD - m_position;
						UIManager::Get().OpenWindow(m_windowType);
						return true;
				}
				// 바가 아니면 각자 윈도우 위치 내부 로직 처리.
				return HandleMouseDown(CORD);
		}


		if (msg.message == WM_RBUTTONDOWN)
		{
			return HandleMouseRight(CORD);
		}


		// 2. 마우스 왼쪽 버튼을 뗀 경우 (드래그 종료 및 드롭 처리)
		if (msg.message == WM_LBUTTONUP)
		{
				// 창 드래그 상태 해제
				m_isDragging = false;
				// false로 하면 처리 안해버리고 다른곳에서 처리해서 바꿔버림.
				return HandleMouseUp(CORD);
		}

		// 3. 마우스 이동 중 (창 드래그 로직)
						//if (m_isDragging && msg.message == WM_MOUSEMOVE)

		//if (CursorManager::Get().GetCurrentCursor() == CursorType::Drag
		//	&& msg.message == WM_MOUSEMOVE) //툴팁만을 위한 특별한 수임. 
		//{
		//	return HandleMouseHover(CORD);
		//}

		else if (m_isDragging && msg.message == WM_MOUSEMOVE) // ->  
		{	
				m_position = CORD - m_dragOffset;

				if (GetType() == UIWindowType::InventoryWindow && 
					CursorManager::Get().GetDraggedItem() != nullptr) //나 인벤이고, 커서매니저에 아이템 있는 경우 
				{
					if (UIManager::Get().IsWindowActive(UIWindowType::EquipmentWindow))
					{
						EquipmentWindow* win = 
							dynamic_cast<EquipmentWindow*>(UIManager::Get().GetWindow(UIWindowType::EquipmentWindow));
						Item* item = CursorManager::Get().GetDraggedItem();

						if(win->CanEquipItem(item, item->m_data.wearablePart)); // -> 들고 있는것도 모자 장비창에 모자칸에 해당하는 슬롯이 비어있는 경우 
						win->SendEventToComponent("PLAY", item->m_data.wearablePart);
					}
				}
				return true;
		}


		// 드래그 중이 아닐 때는 마우스 호버만 처리합니다.
		else if (!m_isDragging && msg.message == WM_MOUSEMOVE)
		{
			return HandleMouseHover(CORD); //업데이트를 하긴 함. 
		}


		return false; 
}