#include "PlayComponent.h"

Slide_Effect::Slide_Effect(RenderInfo* renderInfo, Transform& transform, float speed, float bitmapWidth, float screenWidth, bool isRight)
	: m_renderInfo(renderInfo), m_transform(transform), m_speed(speed), m_bitmapWidth(bitmapWidth), m_screenWidth(screenWidth), m_isRight(isRight)
{
}

void Slide_Effect::Update()
{
	if (isStop)	return;

	if (m_isRight)
	{
		m_transform.Translate(-m_speed, 0.f);
		if (-m_bitmapWidth >= m_transform.GetPosition().x)	m_transform.SetPosition({ m_screenWidth, m_transform.GetPosition().y });
	}
	else if (!m_isRight)
	{
		m_transform.Translate(m_speed, 0.f);
		if (m_screenWidth <= m_transform.GetPosition().x)	m_transform.SetPosition({ -m_bitmapWidth, m_transform.GetPosition().y });
	}
}

void Slide_Effect::OnEvent(const std::string& ev)
{
	if (ev == "STOP")
	{
		isStop = true;
	}
	else if (ev == "PLAY")
	{
		isStop = false;
	}
}

UpDown_Effect::UpDown_Effect(RenderInfo* renderInfo, Transform& transform, float moveAmount, float upSpeed, float downSpeed)
	: m_renderInfo(renderInfo), m_transform(transform), m_moveAmount(moveAmount), m_upSpeed(upSpeed), m_downSpeed(downSpeed)
{
	m_position = m_transform.GetPosition();
}

void UpDown_Effect::Update()
{
	if (isStop)	return;

	if (isUp)
	{
		m_transform.Translate(0.f, -m_upSpeed);
		if ((m_position.y - m_moveAmount) >= m_transform.GetPosition().y)	isUp = false;
	}
	else if (!isUp)
	{
		m_transform.Translate(0.f, m_downSpeed);
		if ((m_position.y + m_moveAmount) <= m_transform.GetPosition().y)	isUp = true;
	}
}

void UpDown_Effect::OnEvent(const std::string& ev)
{
	if (ev == "STOP")
	{
		isStop = true;
	}
	else if (ev == "PLAY")
	{
		isStop = false;
	}
}

Blink_Effect::Blink_Effect(RenderInfo* renderInfo, float minOpacity, float totalSecond, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_minOpacity(minOpacity), m_totalSecond(totalSecond), m_effect(effect)
{
	Initialize();
}

Blink_Effect::Blink_Effect(RenderInfo* renderInfo, float minOpacity, float totalSecond, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_minOpacity(minOpacity), m_totalSecond(totalSecond), m_bitmap(bitmap)
{
	Initialize();
}

void Blink_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Opacity, &m_opacityEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_opacityEffect->SetInput(0, m_bitmap.Get());
	else
		m_opacityEffect->SetInputEffect(0, m_effect);
}

void Blink_Effect::FixedUpdate(float dt)
{
	if (isStop) return;

	time += dt;
	if (time >= FPS60)
	{
		if (time >= 5.f)	time = 0.f;		// 처음 dt가 5이상 큰 수가 들어옴
		else
		{
			time -= FPS60;
			x += FPS60;
		}
	}

	if (x >= m_totalSecond)
	{
		x = 0.f;
	}

	m_opacityEffect->SetValue(D2D1_COMPOSITE_MODE_SOURCE_OVER, Graph(x));
}

Explode_Effect::Explode_Effect(RenderInfo* renderInfo, float x_currentScale, float y_currentScale, float x_addScale, float y_addScale, float totalSecond, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_currentScale{ x_currentScale, y_currentScale }, mx_addScale(x_addScale), my_addScale(y_addScale), m_totalSecond(totalSecond), m_effect(effect)
{
	Initialize();
}

Explode_Effect::Explode_Effect(RenderInfo* renderInfo, float x_currentScale, float y_currentScale, float x_addScale, float y_addScale, float totalSecond, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_currentScale{ x_currentScale, y_currentScale }, mx_addScale(x_addScale), my_addScale(y_addScale), m_totalSecond(totalSecond), m_bitmap(bitmap)
{
	Initialize();
}

void Explode_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D12DAffineTransform, &m_offsetEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_offsetEffect->SetInput(0, m_bitmap.Get());
	else
		m_offsetEffect->SetInputEffect(0, m_effect);

	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_LINEAR);
}

void Explode_Effect::FixedUpdate(float dt)
{
	if (isStop)	return;

	time += dt;
	if (time >= FPS60)
	{
		if (time >= 5.f)	time = 0.f;		// 처음 dt가 5이상 큰 수가 들어옴
		else
		{
			time -= FPS60;
			x += FPS60;
		}
	}

	D2D1_VECTOR_2F scale;

	scale = { Graph(x, mx_addScale), Graph(x, my_addScale) };

	D2D1_MATRIX_3X2_F offsetMT = D2D1::Matrix3x2F::Scale(scale.x, scale.y, { m_renderInfo->GetSize().width / 2.f, m_renderInfo->GetSize().height / 2.f });
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, offsetMT);

	if (scale.x < 0.f && scale.y < 0.f)
	{
		x = 0;
		m_renderInfo->SetisActive(false);
		//isFirst = false;
		isStop = true;
	}
	m_renderInfo->SetEffect(m_offsetEffect.Get());
}

void Explode_Effect::OnEvent(const std::string& ev)
{
	if (ev == "PLAY")
	{
		isStop = false;
		m_renderInfo->SetisActive(true);
		//D2D1_MATRIX_3X2_F offsetMT = D2D1::Matrix3x2F::Scale(m_currentScale.x, m_currentScale.y, { m_renderInfo->GetSize().width / 2.f, m_renderInfo->GetSize().height / 2.f });
		//m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, offsetMT);
	}
}

DynamicContrast_Effect::DynamicContrast_Effect(RenderInfo* renderInfo, float minContrast, float maxContrast, float addNum, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_minContrast(minContrast), m_maxContrast(maxContrast), m_addNum(addNum), m_effect(effect)
{
	Initialize();
}

DynamicContrast_Effect::DynamicContrast_Effect(RenderInfo* renderInfo, float minContrast, float maxContrast, float addNum, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_minContrast(minContrast), m_maxContrast(maxContrast), m_addNum(addNum), m_bitmap(bitmap)
{
	Initialize();
}

void DynamicContrast_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Contrast, &m_contrastEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_contrastEffect->SetInput(0, m_bitmap.Get());
	else
		m_contrastEffect->SetInputEffect(0, m_effect);

	m_contrastEffect->SetValue(D2D1_CONTRAST_PROP_CONTRAST, m_currentStrength);
	m_contrastEffect->SetValue(D2D1_CONTRAST_PROP_CLAMP_INPUT, true);

	m_renderInfo->SetEffect(m_contrastEffect.Get());
}

void DynamicContrast_Effect::Update()
{
	m_currentStrength += m_addNum;
	if (m_currentStrength >= m_maxContrast)	m_addNum = -m_addNum;
	else if (m_currentStrength <= m_minContrast) 	m_addNum = -m_addNum;
	m_contrastEffect->SetValue(D2D1_CONTRAST_PROP_CONTRAST, m_currentStrength);
}
