#include "UIWindow.h"
#include "UIManager.h"

//bool UIWindow::HandleInput(const MSG& msg)
//{
//	{
//		// â�� ��Ȱ��ȭ�Ǿ� ������ ó�� ����
//		if (!m_isActive)
//			return false;
//
//		Vec2 CORD = { static_cast<float>(F_GET_X_LPARAM(msg.lParam)), static_cast<float>(F_GET_Y_LPARAM(msg.lParam)) };
//
//		if (IsInBounds(CORD) && msg.message == WM_LBUTTONDOWN)
//		{
//			std::cout << "�Է¹��� X" << CORD.x << "�Է¹��� Y" << CORD.y << endl;
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
//		return true; // â ���� ���� �Է��� ��� "ó����"���� ����
//	}
//		return false;
//}


bool UIWindow::HandleInput(const MSG& msg)
{
		// â�� ��Ȱ��ȭ�Ǿ� ������ ó�� ���� -> ��.. �׷� �ٸ� â �Լ� ������ �̰� �� �ʿ�� ���� 
		if (!m_isActive)
				return false;


		Vec2 CORD = { static_cast<float>(F_GET_X_LPARAM(msg.lParam)), static_cast<float>(F_GET_Y_LPARAM(msg.lParam)) };

		// 1. WM_LBUTTONUP�� �巡�� ���¸� �����ؾ� �ϹǷ� ���� ���� ó��
		if (msg.message == WM_LBUTTONDOWN)
		{
				// �ݱ�â�̸� ��.
				if (IsInCloseButton(CORD))
				{
						std::cout << "�ݱ� �Ұ���." << std::endl;

						// ��ȭâ�� ��� ������ �������� �κ��丮�� ��ȯ
						if (m_windowType == UIWindowType::EnhancementWindow)
						{
								auto* enhancementWindow = dynamic_cast<EnhancementWindow*>(this);
								if (enhancementWindow)
								{
										enhancementWindow->ReturnItemToInventory();
								}
						}
						// �ռ�â�� ��� ������ �����۵��� �κ��丮�� ��ȯ
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
				// Ÿ��Ʋ�ٸ� �巡��.
				if (IsInTitleBar(CORD)) //�� ���� �ֵ��� �� ���ƾ� ��. 
				{
						std::cout << "�巡�� �Ұ���." << std::endl;
						if(GetType() != UIWindowType::InventoryTooltip)
						{
							m_isDragging = true;
						}
						
						m_dragOffset = CORD - m_position;
						UIManager::Get().OpenWindow(m_windowType);
						return true;
				}
				// �ٰ� �ƴϸ� ���� ������ ��ġ ���� ���� ó��.
				return HandleMouseDown(CORD);
		}


		if (msg.message == WM_RBUTTONDOWN)
		{
			return HandleMouseRight(CORD);
		}


		// 2. ���콺 ���� ��ư�� �� ��� (�巡�� ���� �� ��� ó��)
		if (msg.message == WM_LBUTTONUP)
		{
				// â �巡�� ���� ����
				m_isDragging = false;
				// false�� �ϸ� ó�� ���ع����� �ٸ������� ó���ؼ� �ٲ����.
				return HandleMouseUp(CORD);
		}

		// 3. ���콺 �̵� �� (â �巡�� ����)
						//if (m_isDragging && msg.message == WM_MOUSEMOVE)

		//if (CursorManager::Get().GetCurrentCursor() == CursorType::Drag
		//	&& msg.message == WM_MOUSEMOVE) //�������� ���� Ư���� ����. 
		//{
		//	return HandleMouseHover(CORD);
		//}

		else if (m_isDragging && msg.message == WM_MOUSEMOVE) // ->  
		{	
				m_position = CORD - m_dragOffset;

				if (GetType() == UIWindowType::InventoryWindow && 
					CursorManager::Get().GetDraggedItem() != nullptr) //�� �κ��̰�, Ŀ���Ŵ����� ������ �ִ� ��� 
				{
					if (UIManager::Get().IsWindowActive(UIWindowType::EquipmentWindow))
					{
						EquipmentWindow* win = 
							dynamic_cast<EquipmentWindow*>(UIManager::Get().GetWindow(UIWindowType::EquipmentWindow));
						Item* item = CursorManager::Get().GetDraggedItem();

						if(win->CanEquipItem(item, item->m_data.wearablePart)); // -> ��� �ִ°͵� ���� ���â�� ����ĭ�� �ش��ϴ� ������ ����ִ� ��� 
						win->SendEventToComponent("PLAY", item->m_data.wearablePart);
					}
				}
				return true;
		}


		// �巡�� ���� �ƴ� ���� ���콺 ȣ���� ó���մϴ�.
		else if (!m_isDragging && msg.message == WM_MOUSEMOVE)
		{
			return HandleMouseHover(CORD); //������Ʈ�� �ϱ� ��. 
		}


		return false; 
}