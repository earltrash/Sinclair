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
	// â�� ��Ȱ��ȭ�Ǿ� ������ ó�� ����
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
			UIManager::Get().CloseWindow(m_windowType);
			return true;
		}
		// Ÿ��Ʋ�ٸ� �巡��.
		if (IsInTitleBar(CORD))
		{
			std::cout << "�巡�� �Ұ���." << std::endl;
			m_isDragging = true;

			m_dragOffset = CORD - m_position;
			UIManager::Get().OpenWindow(m_windowType);
			return true;
		}
		// �ٰ� �ƴϸ� ���� ������ ��ġ ���� ���� ó��.
		return HandleMouseDown(CORD);
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
	if (m_isDragging && msg.message == WM_MOUSEMOVE)
	{
		// â�� ��ġ�� ������Ʈ
	//	m_position = CORD - m_dragOffset;
		//return true;
	return	HandleMouseHover(CORD);

	}

	return false;
}