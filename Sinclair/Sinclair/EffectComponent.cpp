#include "EffectComponent.h"

GaussianBlur_Effect::GaussianBlur_Effect(RenderInfo* renderInfo, float strength, ID2D1Bitmap1* bitmap) : m_renderInfo(renderInfo), m_strength(strength), m_bitmap(bitmap)
{
	Initialize();
}

GaussianBlur_Effect::GaussianBlur_Effect(RenderInfo* renderInfo, float strength, ID2D1Effect* pEffect) : m_renderInfo(renderInfo), m_strength(strength), m_effect(pEffect)
{
	Initialize();
}

void GaussianBlur_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1GaussianBlur, &m_gaussianBlurEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_gaussianBlurEffect->SetInput(0, m_bitmap.Get());
	else
		m_gaussianBlurEffect->SetInputEffect(0, m_effect);

	m_gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, m_strength);

	m_renderInfo->SetEffect(m_gaussianBlurEffect.Get());
}

void GaussianBlur_Effect::Update()
{

}

Shadow_Effect::Shadow_Effect(RenderInfo* renderInfo, float edgeBlur, float r, float g, float b, float a, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_edgeBlur(edgeBlur), m_shadowColor{ r, g, b, a }, m_bitmap(bitmap)
{
	Initialize();
}

Shadow_Effect::Shadow_Effect(RenderInfo* renderInfo, float edgeBlur, float r, float g, float b, float a, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_edgeBlur(edgeBlur), m_shadowColor{ r, g, b, a }, m_effect(effect)
{
	Initialize();
}

void Shadow_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Shadow, &m_shadowEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_shadowEffect->SetInput(0, m_bitmap.Get());
	else
		m_shadowEffect->SetInputEffect(0, m_effect);

	m_shadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, m_edgeBlur);
	m_shadowEffect->SetValue(D2D1_SHADOW_PROP_COLOR, m_shadowColor);

	m_renderInfo->SetEffect(m_shadowEffect.Get());
}

void Shadow_Effect::Update()
{
}

Composite_Effect::Composite_Effect(RenderInfo* renderInfo, ID2D1Effect* top, ID2D1Effect* bottom, D2D1_COMPOSITE_MODE mode)
	: m_renderInfo(renderInfo), m_EffectT(top), m_EffectB(bottom), m_mode(mode)
{
	Initialize();
}

Composite_Effect::Composite_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* top, ID2D1Effect* bottom, D2D1_COMPOSITE_MODE mode)
	: m_renderInfo(renderInfo), m_bitmapT(top), m_EffectB(bottom), m_mode(mode)
{
	Initialize();
}

Composite_Effect::Composite_Effect(RenderInfo* renderInfo, ID2D1Effect* top, ID2D1Bitmap1* bottom, D2D1_COMPOSITE_MODE mode)
	: m_renderInfo(renderInfo), m_EffectT(top), m_bitmapB(bottom), m_mode(mode)
{
	Initialize();
}

Composite_Effect::Composite_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* top, ID2D1Bitmap1* bottom, D2D1_COMPOSITE_MODE mode)
	: m_renderInfo(renderInfo), m_bitmapT(top), m_bitmapB(bottom), m_mode(mode)
{
	Initialize();
}

void Composite_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Composite, &m_compositeEffect);
	DX::ThrowIfFailed(hr);

	if (m_EffectT != nullptr && m_EffectB != nullptr)
	{
		m_compositeEffect->SetInputEffect(0, m_EffectB);
		m_compositeEffect->SetInputEffect(1, m_EffectT);
		m_compositeEffect->SetValue(D2D1_COMPOSITE_PROP_MODE, m_mode);
	}
	else if (m_bitmapT != nullptr && m_EffectB != nullptr)
	{
		m_compositeEffect->SetInputEffect(0, m_EffectB);
		m_compositeEffect->SetInput(1, m_bitmapT.Get());
		m_compositeEffect->SetValue(D2D1_COMPOSITE_PROP_MODE, m_mode);
	}
	else if (m_EffectT != nullptr && m_bitmapB != nullptr)
	{
		m_compositeEffect->SetInput(0, m_bitmapB.Get());
		m_compositeEffect->SetInputEffect(1, m_EffectT);
		m_compositeEffect->SetValue(D2D1_COMPOSITE_PROP_MODE, m_mode);
	}
	else if (m_bitmapT != nullptr && m_bitmapB != nullptr)
	{
		m_compositeEffect->SetInput(0, m_bitmapB.Get());
		m_compositeEffect->SetInput(1, m_bitmapT.Get());
		m_compositeEffect->SetValue(D2D1_COMPOSITE_PROP_MODE, m_mode);
	}

	m_renderInfo->SetEffect(m_compositeEffect.Get());
}

void Composite_Effect::Update()
{
	if (m_EffectT != nullptr && m_EffectB != nullptr)
	{
		m_compositeEffect->SetInputEffect(0, m_EffectB);
		m_compositeEffect->SetInputEffect(1, m_EffectT);
		m_compositeEffect->SetValue(D2D1_COMPOSITE_PROP_MODE, m_mode);
	}
	else if (m_bitmapT != nullptr && m_EffectB != nullptr)
	{
		m_compositeEffect->SetInputEffect(0, m_EffectB);
		m_compositeEffect->SetInput(1, m_bitmapT.Get());
		m_compositeEffect->SetValue(D2D1_COMPOSITE_PROP_MODE, m_mode);
	}
	else if (m_EffectT != nullptr && m_bitmapB != nullptr)
	{
		m_compositeEffect->SetInput(0, m_bitmapB.Get());
		m_compositeEffect->SetInputEffect(1, m_EffectT);
		m_compositeEffect->SetValue(D2D1_COMPOSITE_PROP_MODE, m_mode);
	}
	else if (m_bitmapT != nullptr && m_bitmapB != nullptr)
	{
		m_compositeEffect->SetInput(0, m_bitmapB.Get());
		m_compositeEffect->SetInput(1, m_bitmapT.Get());
		m_compositeEffect->SetValue(D2D1_COMPOSITE_PROP_MODE, m_mode);
	}

	m_renderInfo->SetEffect(m_compositeEffect.Get());
}

Crop_Effect::Crop_Effect(RenderInfo* renderInfo, float x, float y, float width, float height, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_rect{ x, y, width, height }, m_effect(effect)
{
	Initialize();
}

Crop_Effect::Crop_Effect(RenderInfo* renderInfo, float x, float y, float width, float height, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_rect{ x, y, width, height }, m_bitmap(bitmap)
{
	Initialize();
}

Crop_Effect::Crop_Effect(RenderInfo* renderInfo, D2D1_RECT_F srcRect, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_rect(srcRect), m_effect(effect)
{
	Initialize();
}

Crop_Effect::Crop_Effect(RenderInfo* renderInfo, D2D1_RECT_F srcRect, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_rect(srcRect), m_bitmap(bitmap)
{
	Initialize();
}

void Crop_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Crop, &m_cropEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_cropEffect->SetInput(0, m_bitmap.Get());
	else
		m_cropEffect->SetInputEffect(0, m_effect);

	m_cropEffect->SetValue(D2D1_CROP_PROP_RECT, m_rect);

	m_renderInfo->SetEffect(m_cropEffect.Get());
}

void Crop_Effect::Update()
{

}

Contrast_Effect::Contrast_Effect(RenderInfo* renderInfo, float strength, ID2D1Effect* effect) : m_renderInfo(renderInfo), m_strength(strength), m_effect(effect)
{
	Initialize();
}

Contrast_Effect::Contrast_Effect(RenderInfo* renderInfo, float strength, ID2D1Bitmap1* bitmap) : m_renderInfo(renderInfo), m_strength(strength), m_bitmap(bitmap)
{
	Initialize();
}

void Contrast_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Contrast, &m_contrastEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_contrastEffect->SetInput(0, m_bitmap.Get());
	else
		m_contrastEffect->SetInputEffect(0, m_effect);

	m_contrastEffect->SetValue(D2D1_CONTRAST_PROP_CONTRAST, m_strength);
	m_contrastEffect->SetValue(D2D1_CONTRAST_PROP_CLAMP_INPUT, true);

	m_renderInfo->SetEffect(m_contrastEffect.Get());
}

void Contrast_Effect::Update()
{
}

Vignette_Effect::Vignette_Effect(RenderInfo* renderInfo, float blurRadius, float strength, float r, float g, float b, float a, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_blurRadius(blurRadius), m_strength(strength), m_color{ r, g, b, a }, m_effect(effect)
{
	Initialize();
}

Vignette_Effect::Vignette_Effect(RenderInfo* renderInfo, float blurRadius, float strength, float r, float g, float b, float a, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_blurRadius(blurRadius), m_strength(strength), m_color{ r, g, b, a }, m_bitmap(bitmap)
{
	Initialize();
}

void Vignette_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Vignette, &m_vignetteEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_vignetteEffect->SetInput(0, m_bitmap.Get());
	else
		m_vignetteEffect->SetInputEffect(0, m_effect);

	m_vignetteEffect->SetValue(D2D1_VIGNETTE_PROP_COLOR, m_color);
	m_vignetteEffect->SetValue(D2D1_VIGNETTE_PROP_TRANSITION_SIZE, m_blurRadius);
	m_vignetteEffect->SetValue(D2D1_VIGNETTE_PROP_STRENGTH, m_strength);

	m_renderInfo->SetEffect(m_vignetteEffect.Get());
}

void Vignette_Effect::Update()
{
}

Scale_Effect::Scale_Effect(RenderInfo* renderInfo, float pivotX, float pivotY, float scaleX, float scaleY, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_pivot{ pivotX, pivotY }, m_scale{ scaleX, scaleY }, m_effect(effect)
{
	Initialize();
}

Scale_Effect::Scale_Effect(RenderInfo* renderInfo, float pivotX, float pivotY, float scaleX, float scaleY, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_pivot{ pivotX, pivotY }, m_scale{ scaleX, scaleY }, m_bitmap(bitmap)
{
	Initialize();
}

void Scale_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Scale, &m_scaleEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_scaleEffect->SetInput(0, m_bitmap.Get());
	else
		m_scaleEffect->SetInputEffect(0, m_effect);

	m_scaleEffect->SetValue(D2D1_SCALE_PROP_CENTER_POINT, m_pivot);
	m_scaleEffect->SetValue(D2D1_SCALE_PROP_SCALE, m_scale);

	m_renderInfo->SetEffect(m_scaleEffect.Get());
}

void Scale_Effect::Update()
{
}

Posterize_Effect::Posterize_Effect(RenderInfo* renderInfo, int redStep, int greenStep, int blueStep, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_redStep(redStep), m_greenStep(greenStep), m_blueStep(blueStep), m_effect(effect)
{
	Initialize();
}

Posterize_Effect::Posterize_Effect(RenderInfo* renderInfo, int redStep, int greenStep, int blueStep, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_redStep(redStep), m_greenStep(greenStep), m_blueStep(blueStep), m_bitmap(bitmap)
{
	Initialize();
}

void Posterize_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Posterize, &m_posterEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_posterEffect->SetInput(0, m_bitmap.Get());
	else
		m_posterEffect->SetInputEffect(0, m_effect);
	m_posterEffect->SetValue(D2D1_POSTERIZE_PROP_RED_VALUE_COUNT, m_redStep);
	m_posterEffect->SetValue(D2D1_POSTERIZE_PROP_GREEN_VALUE_COUNT, m_greenStep);
	m_posterEffect->SetValue(D2D1_POSTERIZE_PROP_BLUE_VALUE_COUNT, m_blueStep);

	m_renderInfo->SetEffect(m_posterEffect.Get());
}

void Posterize_Effect::Update()
{
}

Gray_Effect::Gray_Effect(RenderInfo* renderInfo, float strength, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_strength(strength), m_effect(effect)
{
	Initialize();
}

Gray_Effect::Gray_Effect(RenderInfo* renderInfo, float strength, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_strength(strength), m_bitmap(bitmap)
{
	Initialize();
}

void Gray_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Saturation, &m_grayEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_grayEffect->SetInput(0, m_bitmap.Get());
	else
		m_grayEffect->SetInputEffect(0, m_effect);
	m_grayEffect->SetValue(D2D1_SATURATION_PROP_SATURATION, 1.f - m_strength);

	m_renderInfo->SetEffect(m_grayEffect.Get());
}

void Gray_Effect::Update()
{

}

Rotate3D_Effect::Rotate3D_Effect(RenderInfo* renderInfo, float depth, float pivotX, float pivotY, float rotateX, float rotateY, float rotateZ, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_depth(depth), m_pivot{ pivotX, pivotY, 0.f }, m_rotate{ rotateX, rotateY, rotateZ }, m_effect(effect)
{
	Initialize();
}

Rotate3D_Effect::Rotate3D_Effect(RenderInfo* renderInfo, float depth, float pivotX, float pivotY, float rotateX, float rotateY, float rotateZ, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_depth(depth), m_pivot{ pivotX, pivotY, 0.f }, m_rotate{ rotateX, rotateY, rotateZ }, m_bitmap(bitmap)
{
	Initialize();
}

void Rotate3D_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D13DPerspectiveTransform, &m_perspectiveEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
	{
		m_perspectiveEffect->SetInput(0, m_bitmap.Get());
	}
	else
	{
		m_perspectiveEffect->SetInputEffect(0, m_effect);
	}

	m_perspectiveEffect->SetValue(D2D1_3DPERSPECTIVETRANSFORM_PROP_ROTATION_ORIGIN, m_pivot);

	m_rotation.x = fmod((m_rotation.x + m_rotate.x), 360.f);	m_rotation.y = fmod((m_rotation.y + m_rotate.y), 360.f);	m_rotation.z = fmod((m_rotation.z + m_rotate.z), 360.f);

	m_perspectiveEffect->SetValue(D2D1_3DPERSPECTIVETRANSFORM_PROP_ROTATION, m_rotation);

	m_perspectiveEffect->SetValue(D2D1_3DPERSPECTIVETRANSFORM_PROP_DEPTH, m_depth);

	m_renderInfo->SetEffect(m_perspectiveEffect.Get());
}

void Rotate3D_Effect::Update()
{
	m_rotation.x = fmod((m_rotation.x + m_rotate.x), 360.f);	m_rotation.y = fmod((m_rotation.y + m_rotate.y), 360.f);	m_rotation.z = fmod((m_rotation.z + m_rotate.z), 360.f);
	m_perspectiveEffect->SetValue(D2D1_3DPERSPECTIVETRANSFORM_PROP_ROTATION, m_rotation);
	m_renderInfo->SetEffect(m_perspectiveEffect.Get());
}

Offset_Effect::Offset_Effect(RenderInfo* renderInfo, float xOffset, float yOffset, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_xOffset(xOffset), m_yOffset(yOffset), m_bitmap(bitmap)
{
	Initialize();
}

Offset_Effect::Offset_Effect(RenderInfo* renderInfo, float xOffset, float yOffset, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_xOffset(xOffset), m_yOffset(yOffset), m_effect(effect)
{
	Initialize();
}

void Offset_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D12DAffineTransform, &m_offsetEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_offsetEffect->SetInput(0, m_bitmap.Get());
	else
		m_offsetEffect->SetInputEffect(0, m_effect);

	D2D1_MATRIX_3X2_F offsetMT = D2D1::Matrix3x2F::Translation(m_xOffset, m_yOffset);
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, offsetMT);
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_LINEAR);

	m_renderInfo->SetEffect(m_offsetEffect.Get());
}

void Offset_Effect::Update()
{

}

Color_Effect::Color_Effect(RenderInfo* renderInfo, float r, float g, float b, float a, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_r(r), m_g(g), m_b(b), m_a(a), m_effect(effect)
{
	Initialize();
}

Color_Effect::Color_Effect(RenderInfo* renderInfo, float r, float g, float b, float a, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_r(r), m_g(g), m_b(b), m_a(a), m_bitmap(bitmap)
{
	Initialize();
}

void Color_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1ColorMatrix, &m_colorEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_colorEffect->SetInput(0, m_bitmap.Get());
	else
		m_colorEffect->SetInputEffect(0, m_effect);

	D2D1_MATRIX_5X4_F matrix = D2D1::Matrix5x4F
	(m_r, 0, 0, 0,
		0, m_g, 0, 0,
		0, 0, m_b, 0,
		0, 0, 0, m_a,
		0, 0, 0, 0);
	m_colorEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, matrix);

	m_renderInfo->SetEffect(m_colorEffect.Get());
}

void Color_Effect::Update()
{

}

Border_Effect::Border_Effect(RenderInfo* renderInfo, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_effect(effect)
{
	Initialize();
}

Border_Effect::Border_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* bitmap)
	:m_renderInfo(renderInfo), m_bitmap(bitmap)
{
	Initialize();
}

void Border_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Border, &m_borderEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_borderEffect->SetInput(0, m_bitmap.Get());
	else
		m_borderEffect->SetInputEffect(0, m_effect);
	m_borderEffect->SetValue(D2D1_BORDER_PROP_EDGE_MODE_X, D2D1_BORDER_EDGE_MODE_CLAMP);
	m_borderEffect->SetValue(D2D1_BORDER_PROP_EDGE_MODE_Y, D2D1_BORDER_EDGE_MODE_CLAMP);
}

void Border_Effect::Update()
{
}
