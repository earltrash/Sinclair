#include "ButtonComponent.h"
#include "ResourceManager.h"
#include "MouseListenerComponent.h"

void ButtonComponent::BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap)
{
	m_Bitmap.emplace(NM, Bitmap);
}

ComPtr<ID2D1Bitmap1> ButtonComponent::GetBitmap()
{
    string key;
    switch (m_currentState)
    {
    case ButtonState::Normal:   key = "normal"; break;
    case ButtonState::Hover:    key = "hover"; break;
    case ButtonState::Pressed:  key = "pressed"; break;
    case ButtonState::Disabled: key = "disabled"; break;
    default: key = "normal"; break;
    }

    auto it = m_Bitmap.find(key);
    if (it != m_Bitmap.end())
        return it->second;

    return nullptr;
}

//Object 활성화 비활성화에 따른 조건이 사전에 있을 수도 있음 -> 경석이형이랑 얘기해 봄.
void ButtonComponent::Worked(const MSG& msg)
{
    // 비트맵이 설정되지 않았다면 리턴
    if (!GetBitmap()) return;

    POINT CORD = { GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) };
    Vec2 pos = m_Owner->GetTransform().GetPosition();

    // 현재 상태에 맞는 비트맵 크기 사용
    auto currentBitmap = GetBitmap();
    D2D1_SIZE_F size = currentBitmap->GetSize();
    Rect rect(pos.x, pos.y, size.width, size.height);

    bool wasInside = (m_currentState == ButtonState::Hover || m_currentState == ButtonState::Pressed);
    bool isInside = InRect(rect, CORD);

    if (isInside)
    {
        if (msg.message == WM_LBUTTONDOWN)
        {
            SetState(ButtonState::Pressed);
            // 클릭 이벤트는 마우스 버튼을 뗄 때 발생시키는 것이 일반적
        }
        else if (msg.message == WM_LBUTTONUP && m_currentState == ButtonState::Pressed)
        {
            // 버튼이 눌린 상태에서 마우스를 뗐을 때만 클릭으로 처리
            if (m_onClick) m_onClick();
            SetState(ButtonState::Hover);
        }
        else if (msg.message == WM_MOUSEMOVE && m_currentState != ButtonState::Pressed)
        {
            SetState(ButtonState::Hover);
        }
    }
    else
    {
        // 마우스가 버튼 범위를 벗어났을 때
        if (wasInside)
        {
            SetState(ButtonState::Normal);
        }
    }

   ///* if (!activated || !m_curbm) 
   // return; */
   //
   //
   // POINT CORD = { GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) }; //msg -> 마우스 클릭 좌표 
   // Vec2 pos = m_Owner->GetTransform().GetPosition(); //현 Obj 좌표
   // D2D1_SIZE_F size = m_curbm->GetSize(); //Bitmap 의 너비와 height -> float rect를 만들어서 범위 측정 
   // Rect rect(pos.x, pos.y, size.width, size.height); //Float type Rect임 



   // if (InRect(rect, CORD)) //마우스 좌표값이 Object 내부인 경우 
   // {
   //     if (msg.message == WM_LBUTTONDOWN) //그게 클릭이면
   //     {
   //         if (m_onClick)
   //             m_onClick();
   //         SetState(ButtonState::Pressed);
   //     }
   //     else if (msg.message == WM_MOUSEMOVE) //hovered 면 
   //     {
   //         SetState(ButtonState::Hover); //이에 대한 Bitmap 변경도 여기에 넣어도 될듯? 일단 냅둠 
   //     }
   // }

   // else
   // SetState(ButtonState::Normal); //범위 내에 없다면 Normal로 
}

void ButtonComponent::SetCurrentBitmap(string Nm)
{
	m_curbm = m_Bitmap[Nm];
	//혹은 Find로 설정하는 게 필요하긴 함. 
}

