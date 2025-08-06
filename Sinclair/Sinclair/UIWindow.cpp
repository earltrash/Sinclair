#include "UIWindow.h"
#include "UIManager.h"

bool UIWindow::HandleInput(const MSG& msg)
{
	{
		// 창이 비활성화되어 있으면 처리 안함
		if (!m_isActive)
			return false;

		Vec2 CORD = { static_cast<float>(F_GET_X_LPARAM(msg.lParam)), static_cast<float>(F_GET_Y_LPARAM(msg.lParam)) };

		if (IsInBounds(CORD) && msg.message == WM_LBUTTONDOWN)
		{
			std::cout << "입력받은 X" << CORD.x << "입력받은 Y" << CORD.y << endl;
			return HandleMouseDown(CORD);
		}

		/*if (IsInBounds(CORD) &&  msg.message == WM_LBUTTONUP)
		{
			return HandleMouseUp(CORD);
		}*/

		if (msg.message == WM_LBUTTONUP)
		{
			return HandleMouseUp(CORD);
		}
				//break;

		return HandleMouseHover(CORD);

		return true; // 창 영역 안의 입력은 모두 "처리됨"으로 간주
	}
		return false;
}
