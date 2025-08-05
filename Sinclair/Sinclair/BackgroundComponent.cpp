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
    //Ȥ�� Find�� �����ϴ� �� �ʿ��ϱ� ��. 

    m_renderInfo->SetBitmap(m_currentBitmap.Get());  // ���� ���� update. by MK
    

}


ComPtr<ID2D1Bitmap1> BackgroundComponent::GetCurrentBitmap()
{
    return m_currentBitmap;
}
