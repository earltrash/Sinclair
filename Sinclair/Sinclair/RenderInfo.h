#pragma once
#include "pch.h"
#include "Renderer.h"
#include "Component.h"
#include "Transform.h"

class RenderInfo : public Component
{
public:
	RenderInfo(ID2D1Bitmap1* bitmap) { SetBitmap(bitmap); }

	void SetBitmap(ID2D1Bitmap1* bitmap)		// 비트맵 변경 시, src와 dest rect에 비트맵 사이즈 들어감
	{
		m_renderInfo.bitmap = bitmap;
		m_renderInfo.srcRect.right = bitmap->GetSize().width;
		m_renderInfo.srcRect.bottom = bitmap->GetSize().height;
		m_renderInfo.destRect = m_renderInfo.srcRect;
	}

	void SetSrcRect(D2D1_RECT_F srcRect)		// src rect가 비트맵 사이즈와 다르다면, 이 함수를 호출해야 함
	{
		m_renderInfo.srcRect = srcRect;
		m_renderInfo.destRect = srcRect;
	}

	void SetEffect(ID2D1Effect* effect)
	{
		m_renderInfo.effect = effect;
	}

	void SetWorldTM(D2D1::Matrix3x2F mt)
	{
		m_renderInfo.mt = mt;
	}

	void SetOpacity(float opacity)
	{
		m_renderInfo.opacity = opacity;
	}

	void SetzOrder(int zOrder)
	{
		m_renderInfo.zOrder = zOrder;
	}

	void SetisActive(bool activated)
	{
		m_renderInfo.activated = activated;
	}

	const D2D1_SIZE_F GetSize() { return { m_renderInfo.srcRect.right - m_renderInfo.srcRect.left, m_renderInfo.srcRect.bottom - m_renderInfo.srcRect.top }; }

	//renderInfo& GetReference()								// obj 내 다른 component에서 사용
	//{ 
	//	return m_renderInfo; 
	//}															
	const renderInfo& GetRenderInfo()							// Scene - Render()에서 사용
	{ 
		return m_renderInfo; 
	}
private:
	renderInfo m_renderInfo;
};