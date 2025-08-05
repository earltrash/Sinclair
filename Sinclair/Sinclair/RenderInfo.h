#pragma once
#include "pch.h"
#include "Renderer.h"
#include "Component.h"
#include "Transform.h"

class RenderInfo : public Component
{
public:
	RenderInfo(ID2D1Bitmap1* bitmap) { SetBitmap(bitmap); }

	void SetBitmap(ID2D1Bitmap1* bitmap)		// ��Ʈ�� ���� ��, src�� dest rect�� ��Ʈ�� ������ ��
	{
		m_renderInfo.bitmap = bitmap;
		m_renderInfo.srcRect.right = bitmap->GetSize().width;
		m_renderInfo.srcRect.bottom = bitmap->GetSize().height;
		m_renderInfo.destRect = m_renderInfo.srcRect;
	}

	void SetSrcRect(D2D1_RECT_F srcRect)		// src rect�� ��Ʈ�� ������� �ٸ��ٸ�, �� �Լ��� ȣ���ؾ� ��
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

	//renderInfo& GetReference()								// obj �� �ٸ� component���� ���
	//{ 
	//	return m_renderInfo; 
	//}															
	const renderInfo& GetRenderInfo()							// Scene - Render()���� ���
	{ 
		return m_renderInfo; 
	}
private:
	renderInfo m_renderInfo;
};