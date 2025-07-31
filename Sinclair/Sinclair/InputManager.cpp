#include "pch.h"
#include "InputManager.h"

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp)) // 그냥 멋지게 쓰고 싶었다...

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


        m_broadcaster->Broadcast(msg);



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


