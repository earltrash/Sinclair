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
	// 창이 비활성화되어 있으면 처리 안함
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
			UIManager::Get().CloseWindow(m_windowType);
			return true;
		}
		// 타이틀바면 드래그.
		if (IsInTitleBar(CORD))
		{
			std::cout << "드래그 할거임." << std::endl;
			m_isDragging = true;

			m_dragOffset = CORD - m_position;
			UIManager::Get().OpenWindow(m_windowType);
			return true;
		}
		// 바가 아니면 각자 윈도우 위치 내부 로직 처리.
		return HandleMouseDown(CORD);
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
	if (m_isDragging && msg.message == WM_MOUSEMOVE)
	{
		// 창의 위치를 업데이트
	//	m_position = CORD - m_dragOffset;
		//return true;
	return	HandleMouseHover(CORD);

	}

	return false;
}