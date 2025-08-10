#pragma once
#include "pch.h"
#include "Renderer.h"
#include "Component.h"
//#include "Transform.h"

class RenderInfo : public Component
{
public:
	RenderInfo() = default;

	RenderInfo(ID2D1Bitmap1* bitmap) { SetBitmap(bitmap); }

	void Clear()
	{
		m_renderInfo.bitmap = nullptr;
		m_renderInfo.effect = nullptr;
		m_renderInfo.mt = D2D1::Matrix3x2F::Identity();
		m_renderInfo.destRect.left = m_renderInfo.destRect.top = m_renderInfo.destRect.right = m_renderInfo.destRect.bottom = 0.f;
		m_renderInfo.srcRect.left = m_renderInfo.srcRect.top = m_renderInfo.srcRect.right = m_renderInfo.srcRect.bottom = 0.f;
		m_renderInfo.opacity = 1.f;
		m_renderInfo.activated = true;
	}

	void SetBitmap(ID2D1Bitmap1* bitmap)		// 비트맵 변경 시, src에 비트맵 사이즈 들어감. dest는 따로 설정 필요.
	{
		m_renderInfo.bitmap = bitmap;
		SetSrcRect({ 0.f, 0.f, bitmap->GetSize().width, bitmap->GetSize().height });
	}

	void SetSrcRect(D2D1_RECT_F srcRect)		// src rect가 비트맵 사이즈와 다르다면, 이 함수를 호출해야 함
	{
		m_renderInfo.srcRect = srcRect;
	}

	void SetDestRect(D2D1_RECT_F destRect)
	{
		m_renderInfo.destRect = destRect;
	}

	void SetDestLeft(float x) { m_renderInfo.destRect.left = x; }
	void SetDestTop(float y) { m_renderInfo.destRect.top = y; }
	void SetDestRight(float width) { m_renderInfo.destRect.right = m_renderInfo.destRect.left + width; }
	void SetDestBottom(float height) { m_renderInfo.destRect.bottom = m_renderInfo.destRect.top + height; }

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

	void SetisActive(bool activated)
	{
		m_renderInfo.activated = activated;
	}

	const D2D1_SIZE_F GetSize()			// src rect의 너비와 높이
	{ 
		return { m_renderInfo.srcRect.right - m_renderInfo.srcRect.left, m_renderInfo.srcRect.bottom - m_renderInfo.srcRect.top }; 
	}	
														
	const renderInfo& GetRenderInfo()							// Scene - Render()에서 사용
	{ 
		return m_renderInfo; 
	}
private:
	renderInfo m_renderInfo;
};