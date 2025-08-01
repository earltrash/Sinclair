#include "ButtonComponent.h"
#include "ResourceManager.h"

void ButtonComponent::BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap)
{
	m_Bitmap.emplace(NM, Bitmap);
}

ComPtr<ID2D1Bitmap1> ButtonComponent::GetBitmap()
{
	return m_curbm;
}

void ButtonComponent::SetCurrentBitmap(string Nm)
{
	m_curbm = m_Bitmap[Nm];
	//Ȥ�� Find�� �����ϴ� �� �ʿ��ϱ� ��. 
}

