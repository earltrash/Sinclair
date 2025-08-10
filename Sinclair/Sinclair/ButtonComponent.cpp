#include "ButtonComponent.h"
#include "ResourceManager.h"
#include "MouseListenerComponent.h"

void ButtonComponent::BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap)
{
	m_Bitmaps.emplace(NM, Bitmap);
}

ComPtr<ID2D1Bitmap1> ButtonComponent::GetBitmap()
{

    switch (m_currentState)
    {
    case ButtonState::Normal:   SetOpacity(1.0f); break;
    case ButtonState::Hover:    SetOpacity(0.55f); break;
    case ButtonState::Pressed:  SetOpacity(0.1f); break;
    case ButtonState::Disabled: SetOpacity(0.0f); break;
    default:  break;
    }

    return m_renderInfo->GetRenderInfo().bitmap;
}



void ButtonComponent::CheckCollision(const MSG& msg)
{

    POINT CORD = { GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) };
    Vec2 pos = m_Owner->GetTransform().GetPosition();


    // 투명버튼이 아닌데 비트맵이 설정되지 않았다면 리턴
    if (!GetBitmap()) return;

    // 현재 상태에 맞는 비트맵 크기 사용
    auto currentBitmap = GetBitmap();
    D2D1_SIZE_F size = currentBitmap->GetSize();
    Rect rect(pos.x, pos.y, size.width, size.height);

    wasInside = (m_currentState == ButtonState::Hover || m_currentState == ButtonState::Pressed);
    isInside = InRect(rect, CORD);
}

void ButtonComponent::Worked(const MSG& msg)
{
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
}

void ButtonComponent::SetCurrentBitmap(string Nm)
{
    m_renderInfo->SetBitmap(m_Bitmaps[Nm].Get());
}

