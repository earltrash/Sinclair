#include "ButtonComponent.h"
#include "ResourceManager.h"

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

void ButtonComponent::SetCurrentBitmap(string Nm)
{
	m_curbm = m_Bitmap[Nm];
	//혹은 Find로 설정하는 게 필요하긴 함. 
}

