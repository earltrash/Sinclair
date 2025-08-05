#include "EffectComponent.h"

GaussianBlur_Effect::GaussianBlur_Effect(RenderInfo* renderInfo, float strength, ID2D1Bitmap1* bitmap) : m_renderInfo(renderInfo), m_strength(strength), m_bitmap(bitmap)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1GaussianBlur, &m_gaussianBlurEffect);
	DX::ThrowIfFailed(hr);
}

GaussianBlur_Effect::GaussianBlur_Effect(RenderInfo* renderInfo, float strength, const wchar_t* path) : m_renderInfo(renderInfo), m_strength(strength)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1GaussianBlur, &m_gaussianBlurEffect);
	DX::ThrowIfFailed(hr);

	D2DRenderer::Get().CreateBitmapFromFile(path, m_bitmap.GetAddressOf());
	if (m_bitmap == nullptr)	std::cout << "GaussianBlur_Effect : 비트맵 생성 실패\n";
}

GaussianBlur_Effect::GaussianBlur_Effect(RenderInfo* renderInfo, float strength, ID2D1Effect* pEffect) : m_renderInfo(renderInfo), m_strength(strength), m_effect(pEffect)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1GaussianBlur, &m_gaussianBlurEffect);
	DX::ThrowIfFailed(hr);
}

void GaussianBlur_Effect::Update()
{
	if (m_effect == nullptr)
		m_gaussianBlurEffect->SetInput(0, m_bitmap.Get());
	else
		m_gaussianBlurEffect->SetInputEffect(0, m_effect);

	m_gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, m_strength);

	m_renderInfo->SetEffect(m_gaussianBlurEffect.Get());
}

Shadow_Effect::Shadow_Effect(RenderInfo* renderInfo, float edgeBlur, float r, float g, float b, float a, float xOffset, float yOffset, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_edgeBlur(edgeBlur), m_shadowColor{ r, g, b, a }, m_xOffset(xOffset), m_yOffset(yOffset), m_bitmap(bitmap)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Shadow, &m_shadowEffect);
	DX::ThrowIfFailed(hr);

	hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D12DAffineTransform, &m_offsetEffect);
	DX::ThrowIfFailed(hr);
}

Shadow_Effect::Shadow_Effect(RenderInfo* renderInfo, float edgeBlur, float r, float g, float b, float a, float xOffset, float yOffset, const wchar_t* path)
	: m_renderInfo(renderInfo), m_edgeBlur(edgeBlur), m_shadowColor{ r, g, b, a }, m_xOffset(xOffset), m_yOffset(yOffset)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Shadow, &m_shadowEffect);
	DX::ThrowIfFailed(hr);

	hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D12DAffineTransform, &m_offsetEffect);
	DX::ThrowIfFailed(hr);

	D2DRenderer::Get().CreateBitmapFromFile(path, m_bitmap.GetAddressOf());
	if (m_bitmap == nullptr)	std::cout << "Shadow_Effect : 비트맵 생성 실패\n";
}

Shadow_Effect::Shadow_Effect(RenderInfo* renderInfo, float edgeBlur, float r, float g, float b, float a, float xOffset, float yOffset, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_edgeBlur(edgeBlur), m_shadowColor{ r, g, b, a }, m_xOffset(xOffset), m_yOffset(yOffset), m_effect(effect)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Shadow, &m_shadowEffect);
	DX::ThrowIfFailed(hr);

	hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D12DAffineTransform, &m_offsetEffect);
	DX::ThrowIfFailed(hr);
}

void Shadow_Effect::Update()
{
	if (m_effect == nullptr)
		m_shadowEffect->SetInput(0, m_bitmap.Get());
	else
		m_shadowEffect->SetInputEffect(0, m_effect);
	m_shadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, m_edgeBlur);
	m_shadowEffect->SetValue(D2D1_SHADOW_PROP_COLOR, m_shadowColor);

	D2D1_MATRIX_3X2_F offsetMT = D2D1::Matrix3x2F::Translation(m_xOffset, m_yOffset);
	m_offsetEffect->SetInputEffect(0, m_shadowEffect.Get());
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, offsetMT);
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_LINEAR);

	m_renderInfo->SetEffect(m_offsetEffect.Get());
}

Composite_Effect::Composite_Effect(RenderInfo* renderInfo, ID2D1Effect* top, ID2D1Effect* bottom, D2D1_COMPOSITE_MODE mode)
	: m_renderInfo(renderInfo), m_EffectT(top), m_EffectB(bottom), m_mode(mode)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Composite, &m_compositeEffect);
	DX::ThrowIfFailed(hr);
}

Composite_Effect::Composite_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* top, ID2D1Effect* bottom, D2D1_COMPOSITE_MODE mode)
	: m_renderInfo(renderInfo), m_bitmapT(top), m_EffectB(bottom), m_mode(mode)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Composite, &m_compositeEffect);
	DX::ThrowIfFailed(hr);
}

Composite_Effect::Composite_Effect(RenderInfo* renderInfo, ID2D1Effect* top, ID2D1Bitmap1* bottom, D2D1_COMPOSITE_MODE mode)
	: m_renderInfo(renderInfo), m_EffectT(top), m_bitmapB(bottom), m_mode(mode)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Composite, &m_compositeEffect);
	DX::ThrowIfFailed(hr);
}

Composite_Effect::Composite_Effect(RenderInfo* renderInfo, const wchar_t* top, ID2D1Bitmap1* bottom, D2D1_COMPOSITE_MODE mode)
	: m_renderInfo(renderInfo), m_bitmapB(bottom), m_mode(mode)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Composite, &m_compositeEffect);
	DX::ThrowIfFailed(hr);

	D2DRenderer::Get().CreateBitmapFromFile(top, m_bitmapT.GetAddressOf());
	if (m_bitmapT == nullptr)	std::cout << "Composite_Effect : 비트맵 생성 실패\n";
}

Composite_Effect::Composite_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* top, const wchar_t* bottom, D2D1_COMPOSITE_MODE mode)
	: m_renderInfo(renderInfo), m_bitmapT(top), m_mode(mode)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Composite, &m_compositeEffect);
	DX::ThrowIfFailed(hr);

	D2DRenderer::Get().CreateBitmapFromFile(bottom, m_bitmapB.GetAddressOf());
	if (m_bitmapB == nullptr)	std::cout << "Composite_Effect : 비트맵 생성 실패\n";
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
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Crop, &m_cropEffect);
	DX::ThrowIfFailed(hr);
}

Crop_Effect::Crop_Effect(RenderInfo* renderInfo, float x, float y, float width, float height, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_rect{ x, y, width, height }, m_bitmap(bitmap)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Crop, &m_cropEffect);
	DX::ThrowIfFailed(hr);
}

Crop_Effect::Crop_Effect(RenderInfo* renderInfo, float x, float y, float width, float height, const wchar_t* path)
	: m_renderInfo(renderInfo), m_rect{ x, y, width, height }
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Crop, &m_cropEffect);
	DX::ThrowIfFailed(hr);

	D2DRenderer::Get().CreateBitmapFromFile(path, m_bitmap.GetAddressOf());
	if (m_bitmap == nullptr)	std::cout << "Crop_Effect : 비트맵 생성 실패\n";
}

void Crop_Effect::Update()
{
	if (m_effect == nullptr)
		m_cropEffect->SetInput(0, m_bitmap.Get());
	else
		m_cropEffect->SetInputEffect(0, m_effect);
	m_cropEffect->SetValue(D2D1_CROP_PROP_RECT, m_rect);

	m_renderInfo->SetEffect(m_cropEffect.Get());
}

Contrast_Effect::Contrast_Effect(RenderInfo* renderInfo, float strength, ID2D1Effect* effect) : m_renderInfo(renderInfo), m_strength(strength), m_effect(effect)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Contrast, &m_contrastEffect);
	DX::ThrowIfFailed(hr);
}

Contrast_Effect::Contrast_Effect(RenderInfo* renderInfo, float strength, ID2D1Bitmap1* bitmap) : m_renderInfo(renderInfo), m_strength(strength), m_bitmap(bitmap)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Contrast, &m_contrastEffect);
	DX::ThrowIfFailed(hr);
}

Contrast_Effect::Contrast_Effect(RenderInfo* renderInfo, float strength, const wchar_t* path) : m_renderInfo(renderInfo), m_strength(strength)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Contrast, &m_contrastEffect);
	DX::ThrowIfFailed(hr);

	D2DRenderer::Get().CreateBitmapFromFile(path, m_bitmap.GetAddressOf());
	if (m_bitmap == nullptr)	std::cout << "Contrast_Effect : 비트맵 생성 실패\n";
}

void Contrast_Effect::Update()
{
	if (m_effect == nullptr)
		m_contrastEffect->SetInput(0, m_bitmap.Get());
	else
		m_contrastEffect->SetInputEffect(0, m_effect);
	m_contrastEffect->SetValue(D2D1_CONTRAST_PROP_CONTRAST, m_strength);
	m_contrastEffect->SetValue(D2D1_CONTRAST_PROP_CLAMP_INPUT, true);

	m_renderInfo->SetEffect(m_contrastEffect.Get());
}

Vignette_Effect::Vignette_Effect(RenderInfo* renderInfo, float blurRadius, float strength, float r, float g, float b, float a, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_blurRadius(blurRadius), m_strength(strength), m_color{ r, g, b, a }, m_effect(effect)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Vignette, &m_vignetteEffect);
	DX::ThrowIfFailed(hr);
}

Vignette_Effect::Vignette_Effect(RenderInfo* renderInfo, float blurRadius, float strength, float r, float g, float b, float a, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_blurRadius(blurRadius), m_strength(strength), m_color{ r, g, b, a }, m_bitmap(bitmap)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Vignette, &m_vignetteEffect);
	DX::ThrowIfFailed(hr);
}

Vignette_Effect::Vignette_Effect(RenderInfo* renderInfo, float blurRadius, float strength, float r, float g, float b, float a, const wchar_t* path)
	: m_renderInfo(renderInfo), m_blurRadius(blurRadius), m_strength(strength), m_color{ r, g, b, a }
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Vignette, &m_vignetteEffect);
	DX::ThrowIfFailed(hr);

	D2DRenderer::Get().CreateBitmapFromFile(path, m_bitmap.GetAddressOf());
	if (m_bitmap == nullptr)	std::cout << "Vignette_Effect : 비트맵 생성 실패\n";
}

void Vignette_Effect::Update()
{
	if (m_effect == nullptr)
		m_vignetteEffect->SetInput(0, m_bitmap.Get());
	else
		m_vignetteEffect->SetInputEffect(0, m_effect);
	m_vignetteEffect->SetValue(D2D1_VIGNETTE_PROP_COLOR, m_color);
	m_vignetteEffect->SetValue(D2D1_VIGNETTE_PROP_TRANSITION_SIZE, m_blurRadius);
	m_vignetteEffect->SetValue(D2D1_VIGNETTE_PROP_STRENGTH, m_strength);

	m_renderInfo->SetEffect(m_vignetteEffect.Get());
}

Scale_Effect::Scale_Effect(RenderInfo* renderInfo, float pivotX, float pivotY, float scaleX, float scaleY, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_pivot{ pivotX, pivotY }, m_scale{ scaleX, scaleY }, m_effect(effect)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Scale, &m_scaleEffect);
	DX::ThrowIfFailed(hr);
}

Scale_Effect::Scale_Effect(RenderInfo* renderInfo, float pivotX, float pivotY, float scaleX, float scaleY, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_pivot{ pivotX, pivotY }, m_scale{ scaleX, scaleY }, m_bitmap(bitmap)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Scale, &m_scaleEffect);
	DX::ThrowIfFailed(hr);
}

Scale_Effect::Scale_Effect(RenderInfo* renderInfo, float pivotX, float pivotY, float scaleX, float scaleY, const wchar_t* path)
	: m_renderInfo(renderInfo), m_pivot{ pivotX, pivotY }, m_scale{ scaleX, scaleY }
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Scale, &m_scaleEffect);
	DX::ThrowIfFailed(hr);

	D2DRenderer::Get().CreateBitmapFromFile(path, m_bitmap.GetAddressOf());
	if (m_bitmap == nullptr)	std::cout << "Scale_Effect : 비트맵 생성 실패\n";
}

void Scale_Effect::Update()
{
	if (m_effect == nullptr)
		m_scaleEffect->SetInput(0, m_bitmap.Get());
	else
		m_scaleEffect->SetInputEffect(0, m_effect);
	m_scaleEffect->SetValue(D2D1_SCALE_PROP_CENTER_POINT, m_pivot);
	m_scaleEffect->SetValue(D2D1_SCALE_PROP_SCALE, m_scale);

	m_renderInfo->SetEffect(m_scaleEffect.Get());
}

Posterize_Effect::Posterize_Effect(RenderInfo* renderInfo, int redStep, int greenStep, int blueStep, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_redStep(redStep), m_greenStep(greenStep), m_blueStep(blueStep), m_effect(effect)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Posterize, &m_posterEffect);
	DX::ThrowIfFailed(hr);
}

Posterize_Effect::Posterize_Effect(RenderInfo* renderInfo, int redStep, int greenStep, int blueStep, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_redStep(redStep), m_greenStep(greenStep), m_blueStep(blueStep), m_bitmap(bitmap)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Posterize, &m_posterEffect);
	DX::ThrowIfFailed(hr);
}

Posterize_Effect::Posterize_Effect(RenderInfo* renderInfo, int redStep, int greenStep, int blueStep, const wchar_t* path)
	: m_renderInfo(renderInfo), m_redStep(redStep), m_greenStep(greenStep), m_blueStep(blueStep)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Posterize, &m_posterEffect);
	DX::ThrowIfFailed(hr);

	D2DRenderer::Get().CreateBitmapFromFile(path, m_bitmap.GetAddressOf());
	if (m_bitmap == nullptr)	std::cout << "Posterize_Effect : 비트맵 생성 실패\n";
}

void Posterize_Effect::Update()
{
	if (m_effect == nullptr)
		m_posterEffect->SetInput(0, m_bitmap.Get());
	else
		m_posterEffect->SetInputEffect(0, m_effect);
	m_posterEffect->SetValue(D2D1_POSTERIZE_PROP_RED_VALUE_COUNT, m_redStep);
	m_posterEffect->SetValue(D2D1_POSTERIZE_PROP_GREEN_VALUE_COUNT, m_greenStep);
	m_posterEffect->SetValue(D2D1_POSTERIZE_PROP_BLUE_VALUE_COUNT, m_blueStep);

	m_renderInfo->SetEffect(m_posterEffect.Get());
}

Gray_Effect::Gray_Effect(RenderInfo* renderInfo, float strength, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_strength(strength), m_effect(effect)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Saturation, &m_grayEffect);
	DX::ThrowIfFailed(hr);
}

Gray_Effect::Gray_Effect(RenderInfo* renderInfo, float strength, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_strength(strength), m_bitmap(bitmap)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Saturation, &m_grayEffect);
	DX::ThrowIfFailed(hr);
}

Gray_Effect::Gray_Effect(RenderInfo* renderInfo, float strength, const wchar_t* path)
	: m_renderInfo(renderInfo), m_strength(strength)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Saturation, &m_grayEffect);
	DX::ThrowIfFailed(hr);

	D2DRenderer::Get().CreateBitmapFromFile(path, m_bitmap.GetAddressOf());
	if (m_bitmap == nullptr)	std::cout << "Gray_Effect : 비트맵 생성 실패\n";
}

void Gray_Effect::Update()
{
	if (m_effect == nullptr)
		m_grayEffect->SetInput(0, m_bitmap.Get());
	else
		m_grayEffect->SetInputEffect(0, m_effect);
	m_grayEffect->SetValue(D2D1_SATURATION_PROP_SATURATION, 1.f - m_strength);

	m_renderInfo->SetEffect(m_grayEffect.Get());
}

PerspectiveTM_Effect::PerspectiveTM_Effect(RenderInfo* renderInfo, float depth, float pivotX, float pivotY, float rotateX, float rotateY, float rotateZ, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_depth(depth), m_pivot{ pivotX, pivotY }, m_rotate{ rotateX, rotateY, rotateZ }, m_effect(effect)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D13DPerspectiveTransform, &m_perspectiveEffect);
	DX::ThrowIfFailed(hr);
}

PerspectiveTM_Effect::PerspectiveTM_Effect(RenderInfo* renderInfo, float depth, float pivotX, float pivotY, float rotateX, float rotateY, float rotateZ, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_depth(depth), m_pivot{ pivotX, pivotY }, m_rotate{ rotateX, rotateY, rotateZ }, m_bitmap(bitmap)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D13DPerspectiveTransform, &m_perspectiveEffect);
	DX::ThrowIfFailed(hr);
}

PerspectiveTM_Effect::PerspectiveTM_Effect(RenderInfo* renderInfo, float depth, float pivotX, float pivotY, float rotateX, float rotateY, float rotateZ, const wchar_t* path)
	: m_renderInfo(renderInfo), m_depth(depth), m_pivot{ pivotX, pivotY }, m_rotate{ rotateX, rotateY, rotateZ }
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D13DPerspectiveTransform, &m_perspectiveEffect);
	DX::ThrowIfFailed(hr);

	D2DRenderer::Get().CreateBitmapFromFile(path, m_bitmap.GetAddressOf());
	if (m_bitmap == nullptr)	std::cout << "PerspectiveTM_Effect : 비트맵 생성 실패\n";
}

void PerspectiveTM_Effect::Update()
{
	m_rotate.x += m_rotate.x;

	if (m_effect == nullptr)
		m_perspectiveEffect->SetInput(0, m_bitmap.Get());
	else
		m_perspectiveEffect->SetInputEffect(0, m_effect);
	m_perspectiveEffect->SetValue(D2D1_3DPERSPECTIVETRANSFORM_PROP_PERSPECTIVE_ORIGIN, m_perspective);
	m_perspectiveEffect->SetValue(D2D1_3DPERSPECTIVETRANSFORM_PROP_ROTATION_ORIGIN, m_pivot);
	m_perspectiveEffect->SetValue(D2D1_3DPERSPECTIVETRANSFORM_PROP_ROTATION, m_rotate);
	m_perspectiveEffect->SetValue(D2D1_3DPERSPECTIVETRANSFORM_PROP_DEPTH, m_depth);

	m_renderInfo->SetEffect(m_perspectiveEffect.Get());
}

//Particle_Effect::Particle_Effect(int particle_count)
//{
//	m_Particles.reserve(1000); // 초기 파티클 개수 예약
//
//	for (int i = 0; i < count; i++)
//	{
//		Particle p;
//		p.size = D2D1::SizeF(100.f, 100.f);
//	
//		float x = safeMargin + static_cast<float>(rand() % static_cast<int>(screenWidth - p.size.width - safeMargin));
//		float y = safeMargin + static_cast<float>(rand() % static_cast<int>(screenHeight - p.size.height - safeMargin));
//
//		p.position = D2D1::Point2F(x + p.size.width / 2.f, y + p.size.height / 2.f);	// 중심좌표
//
//		p.rotation = static_cast<float>(rand() % 360);
//		p.age = 0.0f;
//		p.lifetime = 1.5f + static_cast<float>(rand() % 100) / 5.f;
//
//		//int frameIndex = rand() % 4;
//		//p.sourceRect = GetFrameRect(frameIndex);
//		p.sourceRect.left = 0.f;
//		p.sourceRect.top = 0.f;
//		p.sourceRect.right = 420.f;
//		p.sourceRect.bottom = 420.f;
//
//		p.color = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
//
//		m_Particles.push_back(p);
//	}
//}
//
//void Particle_Effect::Update()
//{
//}
//
//void Particle_Effect::FixedUpdate(float dt)
//{
//	if (isStop) return;
//
//	static float time = 0;
//	static float x = 0;
//
//	time += dt;
//	if (time >= FPS60)
//	{
//		if (time >= 5.f)	time = 0.f;		// 처음 dt가 5이상 큰 수가 들어옴
//		else
//		{
//			time -= FPS60;
//			x += FPS60;
//		}
//	}
//
//	for (auto& p : m_Particles)
//	{
//		p.age += x;
//
//		// 간단한 연출: 위로 천천히 이동 (불꽃 상승)
//		p.position.y -= x * 30.f;
//
//		// 점점 사라지기 (fade-out)
//		float t = p.age / p.lifetime;
//		t = std::clamp(t, 0.f, 1.f);
//		p.color.a = 1.f - t;
//	}
//
//	m_Particles.erase(
//		std::remove_if(m_Particles.begin(), m_Particles.end(),
//			[](const Particle& p) {return p.age >= p.lifetime; }),
//		m_Particles.end()
//	);
//}
