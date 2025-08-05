#include "BackgroundComponent.h"



void BackgroundComponent::Update()
{
    if (m_currentName == "Background")   return;
    SetCurrentBitmap("Background");
}

void BackgroundComponent::BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap)
{
	m_Bitmaps.emplace(NM, Bitmap);
}

ComPtr<ID2D1Bitmap1> BackgroundComponent::GetBitmap()
{
    string key = "Background";
    //switch (m_currentState)
    //{
    //case ButtonState::Normal:   key = "normal"; break;
    //case ButtonState::Hover:    key = "hover"; break;
    //case ButtonState::Pressed:  key = "pressed"; break;
    //case ButtonState::Disabled: key = "disabled"; break;
    //default: key = "normal"; break;
    //}

    auto it = m_Bitmaps.find(key);
    if (it != m_Bitmaps.end())
        return it->second;

    return nullptr;
}

void BackgroundComponent::SetCurrentBitmap(string Nm)
{
    m_currentName = Nm;
    m_currentBitmap = m_Bitmaps[Nm];
    //혹은 Find로 설정하는 게 필요하긴 함. 

    m_renderInfo->SetBitmap(m_currentBitmap.Get());  // 렌더 인포 update. by MK
}