#include "pch.h"
#include "InputManager.h"
#include "UIManager.h"
#include "MouseInput.h"

//#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
//#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp)) // 그냥 멋지게 쓰고 싶었다...

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}





bool InputManager::MsgCheck(MSG& msg)
{



    switch (msg.message)
    {
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:


        //m_broadcaster->Broadcast(msg);


        UIManager::Get().OnInput(msg);
        return true;
        break;

    default:
        return false;
        break;
    }
}

void InputManager::Clean()
{
    m_broadcaster->clean();

}



InputManager& InputManager::Get()
{
    static InputManager Instance;
    return Instance;
}


void InputManager::Update()
{
    // 이전 프레임 상태 저장
    memcpy(m_previousMouseButtons, m_mouseButtons, sizeof(m_mouseButtons));
    memcpy(m_previousKeys, m_keys, sizeof(m_keys));

    // 마우스 위치 업데이트
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(m_hWnd, &cursorPos); 
    m_mousePosition = { static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y) };

    //// 마우스 버튼 상태
    m_mouseButtons[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
    m_mouseButtons[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
    m_mouseButtons[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000;

    bool isLeftPressed = IsMousePressed(MouseButton::Left);
    CursorManager::Get().UpdateMouseState(isLeftPressed);

    //// 더블클릭 타이머
    //if (m_doubleClickTimer > 0)
    //    m_doubleClickTimer -= 0.016f; // 대략 60fps 기준

    //// 더블클릭 체크 (왼쪽 버튼만)
    //if (IsMouseClicked(MouseButton::Left))
    //{
    //    if (m_doubleClickTimer > 0)
    //    {
    //        m_isDoubleClicked = true;
    //        m_doubleClickTimer = 0;
    //    }
    //    else
    //    {
    //        m_isDoubleClicked = false;
    //        m_doubleClickTimer = DOUBLE_CLICK_TIME;
    //    }
    //}
    //else
    //{
    //    m_isDoubleClicked = false;
    //}

    // UIManager에 입력 전달
    
}

// 마우스 클릭
bool InputManager::IsMouseClicked(MouseButton button) const
{
    int index = static_cast<int>(button);
    return m_mouseButtons[index] && !m_previousMouseButtons[index];
}

// 눌렸는지
bool InputManager::IsMousePressed(MouseButton button) const
{
    return m_mouseButtons[static_cast<int>(button)];
}

// 마우스 클릭 끝났는지 
bool InputManager::IsMouseReleased(MouseButton button) const
{
    int index = static_cast<int>(button);
    return !m_mouseButtons[index] && m_previousMouseButtons[index];
}
