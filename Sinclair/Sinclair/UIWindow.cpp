#include "UIWindow.h"
#include "UIManager.h"

bool UIWindow::HandleInput(const MSG& msg)
{
	{
		// â�� ��Ȱ��ȭ�Ǿ� ������ ó�� ����
		if (!m_isActive)
			return false;

		Vec2 CORD = { static_cast<float>(F_GET_X_LPARAM(msg.lParam)), static_cast<float>(F_GET_Y_LPARAM(msg.lParam)) };

		if (IsInBounds(CORD) && msg.message == WM_LBUTTONDOWN)
		{
			std::cout << "�Է¹��� X" << CORD.x << "�Է¹��� Y" << CORD.y << endl;
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

		return true; // â ���� ���� �Է��� ��� "ó����"���� ����
	}
		return false;
}
