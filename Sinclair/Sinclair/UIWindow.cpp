#include "UIWindow.h"
#include "UIManager.h"

bool UIWindow::HandleInput(const MSG& msg)
{
		// 창이 비활성화되어 있으면 처리 안함
		if (!m_isActive)
				return false;

		Vec2 CORD = { static_cast<float>(F_GET_X_LPARAM(msg.lParam)), static_cast<float>(F_GET_Y_LPARAM(msg.lParam)) };

		// 1. WM_LBUTTONUP은 드래그 상태를 해제해야 하므로 가장 먼저 처리
		if (msg.message == WM_LBUTTONUP)
		{
				std::cout << "드래그 끝낼거임" << std::endl;
				m_isDragging = false;
				return true;
		}

		// 2. 드래그 중일 때 WM_MOUSEMOVE를 처리
		if (m_isDragging && msg.message == WM_MOUSEMOVE)
		{
				// 마우스 커서 위치로 계속 이동할거임.
				m_position = CORD - m_dragOffset;
				return true;
		}

		// 3. 드래그 중이 아니거나 WM_LBUTTONUP이 아닌 경우, 창 영역 안에 있는지 확인
		if (!IsInBounds(CORD))
		{
				return false;
		}

		// 4. 창 영역 안에서 이벤트 처리
		switch (msg.message)
		{
		case WM_LBUTTONDOWN:

				if (IsInCloseButton(CORD))
				{ 
						std::cout << "닫기버튼영역임. 닫을거임" << std::endl;
						UIManager::Get().CloseWindow(this->m_windowType);
						return true;
				}


				// 상단 바 클릭 시 드래그 시작
				if (IsInTitleBar(CORD))
				{
						std::cout << "드래그할거임" << std::endl;
						m_isDragging = true;
						m_dragOffset = CORD - m_position;
						return true;
				}
				else
				{
						// 상단 바가 아닌 영역을 클릭했을 경우, 자식 클래스에게 처리 위임
						return HandleMouseDown(CORD);
				}
				break;

				// WM_LBUTTONUP은 이미 위에서 처리했으니 안함.

		default:
				// 호버는 항상 처리 (툴팁 등을 위해) -> 난 잘 모르겠디..
				// 기타 마우스 이벤트 처리 범진이 말이 맞음. 호버 여기에 해야할듯
				//return true;
				break;
		}
		return false;
}
