#include "BackgroundComponent.h"



void BackgroundComponent::Update()
{
}

void BackgroundComponent::BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap)
{
	m_Bitmaps.emplace(NM, Bitmap);
}

void BackgroundComponent::EffectPush(string NM, ID2D1Effect* Effect)
{
    m_Effects.emplace(NM, Effect);
}

ComPtr<ID2D1Bitmap1> BackgroundComponent::GetBitmap()
{
    return GetBitmap("Background");
}

ComPtr<ID2D1Bitmap1> BackgroundComponent::GetBitmap(std::string key )
{
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

void BackgroundComponent::SetCurrentEffect(string Nm)
{
    m_currentName = Nm;
    m_currentEffect = m_Effects[Nm];

    m_renderInfo->SetEffect(m_currentEffect);
}


ComPtr<ID2D1Bitmap1> BackgroundComponent::GetCurrentBitmap()
{
    return m_currentBitmap;
}