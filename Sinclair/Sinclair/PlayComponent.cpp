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
	m_opacityEffect->SetValue(D2D1_COMPOSITE_MODE_SOURCE_OVER, 0.f);
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

void Blink_Effect::OnEvent(const std::string& ev)
{
	if (ev == "STOP") 
	{
		isStop = true;
		m_opacityEffect->SetValue(D2D1_COMPOSITE_MODE_SOURCE_OVER, 0.f);
		x = 0.f;
		time = 0.f;
	}
	else if (ev == "PLAY")
	{
		isStop = false;
	}
}

Explode_Effect::Explode_Effect(RenderInfo* renderInfo, float x_currentScale, float y_currentScale, float x_addScale, float y_addScale, float totalSecond, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_currentScale{ x_currentScale, y_currentScale }, mx_addScale(x_addScale), my_addScale(y_addScale), m_totalSecond(totalSecond), m_effect(effect)
{
	Initialize();
}

Explode_Effect::Explode_Effect(RenderInfo* renderInfo, D2D1_POINT_2F pivot, D2D1_VECTOR_2F currentScale, D2D1_VECTOR_2F addScale, float totalSecond, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_pivot(pivot), m_currentScale(currentScale), mx_addScale(addScale.x), my_addScale(addScale.y), m_totalSecond(totalSecond), m_effect(effect)
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

	if (m_pivot.x != 0.f && m_pivot.y != 0.f)
		offsetMT = D2D1::Matrix3x2F::Scale(m_currentScale.x, m_currentScale.y, m_pivot);
	else
		offsetMT = D2D1::Matrix3x2F::Scale(m_currentScale.x, m_currentScale.y, { m_renderInfo->GetSize().width / 2.f, m_renderInfo->GetSize().height / 2.f });
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, offsetMT);
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_LINEAR);
}

void Explode_Effect::FixedUpdate(float dt)
{
	if (isStop)	return;

	time += dt;
	//if (time >= FPS60)
	//{
	//	if (time >= 5.f)
	//		time = 0.f;		// 처음 dt가 5이상 큰 수가 들어옴
	//	else
	//		time -= FPS60;
	//	x += FPS60;
	//}
	if(time >= 1.f)
		time = 0.f;		// 처음 dt가 1이상 큰 수가 들어옴
	else
		time -= FPS60;
	x += FPS60;

	D2D1_VECTOR_2F scale;

	scale = { m_currentScale.x + Graph(x, mx_addScale), m_currentScale.y + Graph(x, my_addScale) };

	if (m_pivot.x != 0.f && m_pivot.y != 0.f)
		offsetMT = D2D1::Matrix3x2F::Scale(scale.x, scale.y, m_pivot);
	else
		offsetMT = D2D1::Matrix3x2F::Scale(scale.x, scale.y, { m_renderInfo->GetSize().width / 2.f, m_renderInfo->GetSize().height / 2.f });
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, offsetMT);

	if (scale.x < 0.f && scale.y < 0.f)
	{
		x = 0;
		if (m_pivot.x != 0.f && m_pivot.y != 0.f)
			offsetMT = D2D1::Matrix3x2F::Scale(0.f, 0.f, m_pivot);
		else
			offsetMT = D2D1::Matrix3x2F::Scale(0.f, 0.f, { m_renderInfo->GetSize().width / 2.f, m_renderInfo->GetSize().height / 2.f });
		m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, offsetMT);
		isStop = true;
		//m_renderInfo->SetisActive(false);
	}
	m_renderInfo->SetEffect(m_offsetEffect.Get());
}

void Explode_Effect::OnEvent(const std::string& ev)
{
	m_currendState = ev;
	if (ev == "PLAY")
	{
		isStop = false;
		//m_renderInfo->SetisActive(true);
	}
	else if (ev == "DORMANT")
	{
		isStop = true;
		//m_renderInfo->SetisActive(true);
		if (m_pivot.x != 0.f && m_pivot.y != 0.f)
			offsetMT = D2D1::Matrix3x2F::Scale(m_currentScale.x, m_currentScale.y, m_pivot);
		else
			offsetMT = D2D1::Matrix3x2F::Scale(m_currentScale.x, m_currentScale.y, { m_renderInfo->GetSize().width / 2.f, m_renderInfo->GetSize().height / 2.f });
		m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, offsetMT);
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

Increasing_Effect::Increasing_Effect(RenderInfo* renderInfo, D2D1_POINT_2F pivot, float maxScale, float totalTime, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_pivot(pivot), m_maxScale(maxScale), m_totalTime(totalTime), m_effect(effect)
{
	Initialize();
}

Increasing_Effect::Increasing_Effect(RenderInfo* renderInfo, float maxScale, float totalTime, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_maxScale(maxScale), m_totalTime(totalTime), m_bitmap(bitmap)
{
	Initialize();
}

Increasing_Effect::Increasing_Effect(RenderInfo* renderInfo, float maxScale, float totalTime, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_maxScale(maxScale), m_totalTime(totalTime), m_effect(effect)
{
	Initialize();
}

void Increasing_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D12DAffineTransform, &m_offsetEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_offsetEffect->SetInput(0, m_bitmap.Get());
	else
		m_offsetEffect->SetInputEffect(0, m_effect);

	if (m_pivot.x != 0.f && m_pivot.y != 0.f)
		offsetMT = D2D1::Matrix3x2F::Scale(0.f, 0.f, m_pivot);
	else
		offsetMT = D2D1::Matrix3x2F::Scale(0.f, 0.f, { m_renderInfo->GetSize().width / 2.f, m_renderInfo->GetSize().height / 2.f });
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, offsetMT);
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_LINEAR);
}

void Increasing_Effect::FixedUpdate(float dt)
{
	if (isStop)	return;

	time += dt;
	if (time >= 1.f)
		time = 0.f;		// 처음 dt가 1이상 큰 수가 들어옴
	else
		time -= FPS60;
	x += FPS60;

	float scale;

	if (isDisappear)
	{
		scale = GraphDisappear(x);

		SetScale(scale);

		if (scale <= 0.f)
			OnEvent("DORMANT");
	}
	else if (!isDisappear)
	{
		scale = Graph(x);

		SetScale(scale);

		if (!isUp && scale <= m_maxScale)
		{
			isStop = true;
		}
		else if (isUp && scale >= m_maxScale + m_offset)
		{
			isUp = false;
		}
	}


	m_renderInfo->SetEffect(m_offsetEffect.Get());
}

void Increasing_Effect::OnEvent(const std::string& ev)
{
	m_beforeState = m_currendState;
	m_currendState = ev;
	if (ev == "PLAY")			// 다시 떠오르고
	{
		isStop = false;
		isUp = true;
		x = 0;
	}
	else if (ev == "DISAPPEAR")	// 사라지고
	{
		isStop = false;
		isDisappear = true;
		x = 0;
	}
	else if (ev == "DORMANT")	// 멈추고..
	{
		isStop = true;
		isDisappear = false;
		SetScale(0.f);
	}
}

void Increasing_Effect::SetScale(float scale)
{
	if (m_pivot.x != 0.f && m_pivot.y != 0.f)
		offsetMT = D2D1::Matrix3x2F::Scale(scale, scale, m_pivot);
	else
		offsetMT = D2D1::Matrix3x2F::Scale(scale, scale, { m_renderInfo->GetSize().width / 2.f, m_renderInfo->GetSize().height / 2.f });
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, offsetMT);
}

Sideway_Effect::Sideway_Effect(RenderInfo* renderInfo, float moveAmount, float rightSpeed, float leftSpeed, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_moveAmount(moveAmount), m_rightSpeed(rightSpeed), m_leftSpeed(leftSpeed), m_effect(effect)
{
	Initialize();
}

Sideway_Effect::Sideway_Effect(RenderInfo* renderInfo, float moveAmount, float rightSpeed, float leftSpeed, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_moveAmount(moveAmount), m_rightSpeed(rightSpeed), m_leftSpeed(leftSpeed), m_bitmap(bitmap)
{
	Initialize();
}

void Sideway_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D12DAffineTransform, &m_offsetEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_offsetEffect->SetInput(0, m_bitmap.Get());
	else
		m_offsetEffect->SetInputEffect(0, m_effect);

	offsetMT = D2D1::Matrix3x2F::Translation(0.f, 0.f);
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, offsetMT);
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_LINEAR);

	m_renderInfo->SetEffect(m_offsetEffect.Get());
}

void Sideway_Effect::Update()
{
	if (isStop)	return;

	if (isRight)
	{
		m_currentPos += m_rightSpeed;

		offsetMT = D2D1::Matrix3x2F::Translation(m_currentPos, 0.f);

		if ((m_moveAmount) <= m_currentPos)	isRight = false;
	}
	else if (!isRight)
	{
		m_currentPos -= m_leftSpeed;

		offsetMT = D2D1::Matrix3x2F::Translation(m_currentPos, 0.f);

		if ((-m_moveAmount) >= m_currentPos)	isRight = true;
	}
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, offsetMT);
	m_renderInfo->SetEffect(m_offsetEffect.Get());
}

Fade_Effect::Fade_Effect(RenderInfo* renderInfo, float startOpacity, float maxOpacity, float adjustNum, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_currentOpacity(startOpacity), m_maxOpcity(maxOpacity), m_adjustNum(adjustNum), m_effect(effect)
{
	Initialize();
}

Fade_Effect::Fade_Effect(RenderInfo* renderInfo, float startOpacity, float maxOpacity, float adjustNum, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_currentOpacity(startOpacity), m_maxOpcity(maxOpacity), m_adjustNum(adjustNum), m_bitmap(bitmap)
{
	Initialize();
}

void Fade_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Opacity, &m_opacityEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_opacityEffect->SetInput(0, m_bitmap.Get());
	else
		m_opacityEffect->SetInputEffect(0, m_effect);
	m_opacityEffect->SetValue(D2D1_COMPOSITE_MODE_SOURCE_OVER, m_currentOpacity);

	m_renderInfo->SetEffect(m_opacityEffect.Get());
}

void Fade_Effect::Update()
{
	if (isStop)	return;

	if (isFadeIn)
	{
		if (m_currentOpacity == m_maxOpcity)	return;
		m_currentOpacity += m_adjustNum;
		
		if (m_currentOpacity >= m_maxOpcity)
		{
			m_currentOpacity = m_maxOpcity;
			isStop = true;
		}
	}
	else if (!isFadeIn)
	{
		if (m_currentOpacity == 0.f)	return;
		m_currentOpacity -= m_adjustNum;

		if (m_currentOpacity <= 0.f)
		{
			m_currentOpacity = 0.f;
			isStop = true;
		}
	}
	m_opacityEffect->SetValue(D2D1_COMPOSITE_MODE_SOURCE_OVER, m_currentOpacity);
	m_renderInfo->SetEffect(m_opacityEffect.Get());
}

void Fade_Effect::OnEvent(const std::string& ev)
{
	if (ev == "SHOW")
	{
		isStop = false;
		isFadeIn = true;
	}
	else if (ev == "HIDE")
	{
		isStop = false;
		isFadeIn = false;
	}
}

BackForth_Effect::BackForth_Effect(RenderInfo* renderInfo, float moveAmount, float upSpeed, float downSpeed, ID2D1Effect* effect)
	: m_renderInfo(renderInfo), m_moveAmount(moveAmount), m_upSpeed(upSpeed), m_downSpeed(downSpeed), m_effect(effect)
{
	Initialize();
}

BackForth_Effect::BackForth_Effect(RenderInfo* renderInfo, float moveAmount, float upSpeed, float downSpeed, ID2D1Bitmap1* bitmap)
	: m_renderInfo(renderInfo), m_moveAmount(moveAmount), m_upSpeed(upSpeed), m_downSpeed(downSpeed), m_bitmap(bitmap)
{
	Initialize();
}

void BackForth_Effect::Initialize()
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D12DAffineTransform, &m_offsetEffect);
	DX::ThrowIfFailed(hr);

	if (m_effect == nullptr)
		m_offsetEffect->SetInput(0, m_bitmap.Get());
	else
		m_offsetEffect->SetInputEffect(0, m_effect);

	offsetMT = D2D1::Matrix3x2F::Translation(0.f, 0.f);
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, offsetMT);
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_LINEAR);

	m_renderInfo->SetEffect(m_offsetEffect.Get());
}

void BackForth_Effect::Update()
{
	if (isStop)	return;

	if (isUp)
	{
		m_currentPos -= m_upSpeed;

		offsetMT = D2D1::Matrix3x2F::Translation(0.f, m_currentPos);

		if (-m_moveAmount >= m_currentPos)	isUp = false;
	}
	else if (!isUp)
	{
		m_currentPos += m_downSpeed;

		offsetMT = D2D1::Matrix3x2F::Translation(0.f, m_currentPos);

		if (m_moveAmount <= m_currentPos)	isUp = true;
	}
	m_offsetEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, offsetMT);
	m_renderInfo->SetEffect(m_offsetEffect.Get());
}

//Clip_Effect::Clip_Effect(RenderInfo* renderInfo, ID2D1Effect* effect1, ID2D1Effect* effect2)
//	: m_renderInfo(renderInfo), m_Effect1(effect1), m_Effect2(effect2)
//{
//}
//
//Clip_Effect::Clip_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* bitmap1, ID2D1Bitmap1* bitmap2)
//	: m_renderInfo(renderInfo), m_bitmap1(bitmap1), m_bitmap2(bitmap2)
//{
//}
//
//Clip_Effect::Clip_Effect(RenderInfo* renderInfo, ID2D1Effect* effect1, ID2D1Bitmap1* bitmap2)
//	: m_renderInfo(renderInfo), m_Effect1(effect1), m_bitmap2(bitmap2)
//{
//}
//
//Clip_Effect::Clip_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* bitmap1, ID2D1Effect* effect2)
//	: m_renderInfo(renderInfo), m_bitmap1(bitmap1), m_Effect2(effect2)
//{
//}

Clip_Effect::Clip_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* bitmap1, ID2D1Bitmap1* bitmap2, ID2D1Bitmap1* bitmap3, ID2D1Bitmap1* bitmap4, ID2D1Bitmap1* bitmap5)
	: m_renderInfo(renderInfo), m_bitmap1(bitmap1), m_bitmap2(bitmap2), m_bitmap3(bitmap3), m_bitmap4(bitmap4), m_bitmap5(bitmap5)
{
	HRESULT hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1CrossFade, &m_crossFadeEffect1);
	DX::ThrowIfFailed(hr);
	hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1CrossFade, &m_crossFadeEffect2);
	DX::ThrowIfFailed(hr);
	hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1CrossFade, &m_crossFadeEffect3);
	DX::ThrowIfFailed(hr);
	hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1CrossFade, &m_crossFadeEffect4);
	DX::ThrowIfFailed(hr);

	m_crossFadeEffect1->SetInput(1, m_bitmap1.Get());
	m_crossFadeEffect1->SetInput(0, m_bitmap2.Get());
	m_crossFadeEffect1->SetValue(D2D1_CROSSFADE_PROP_WEIGHT, 0.0f);
	m_output = m_crossFadeEffect1.Get();

	hr = D2DRenderer::Get().GetD2DContext()->CreateEffect(CLSID_D2D1Opacity, &m_opacityEffect);
	DX::ThrowIfFailed(hr);

	m_opacityEffect->SetInputEffect(0, m_output);
	m_opacityEffect->SetValue(D2D1_COMPOSITE_MODE_SOURCE_OVER, 0.f);
	m_output = m_opacityEffect.Get();
}

Clip_Effect::~Clip_Effect()
{
	m_crossFadeEffect1.Reset();
	m_crossFadeEffect2.Reset();
	m_crossFadeEffect3.Reset();
	m_crossFadeEffect4.Reset();
	m_opacityEffect.Reset();
}

void Clip_Effect::FixedUpdate(float dt)
{
	if (isStop)	return;
	if (clipNum == 4)
	{
		return;
	}

	time += dt;
	if (time >= 1.f)
		time = 0.f;		// 처음 dt가 1이상 큰 수가 들어옴
	else
		time -= FPS60;
	x += FPS60;

	weight += x;
	if (clipNum == 0)
	{
		m_crossFadeEffect1->SetInput(1, m_bitmap1.Get());
		m_crossFadeEffect1->SetInput(0, m_bitmap2.Get());
		m_crossFadeEffect1->SetValue(D2D1_CROSSFADE_PROP_WEIGHT, weight);
		m_renderInfo->SetEffect(m_crossFadeEffect1.Get());
		if (weight >= 1.f)
		{
			weight = 0.f;
			clipNum++;
			x = 0;
		}
		m_output = m_crossFadeEffect1.Get();
	}
	else if (clipNum == 1)
	{
		m_crossFadeEffect2->SetInputEffect(1, m_crossFadeEffect1.Get());
		m_crossFadeEffect2->SetInput(0, m_bitmap3.Get());
		m_crossFadeEffect2->SetValue(D2D1_CROSSFADE_PROP_WEIGHT, weight);
		m_renderInfo->SetEffect(m_crossFadeEffect2.Get());
		if (weight >= 1.f)
		{
			weight = 0.f;
			clipNum++;
			x = 0;
		}
		m_output = m_crossFadeEffect2.Get();
	}
	else if (clipNum == 2)
	{
		m_crossFadeEffect3->SetInputEffect(1, m_crossFadeEffect2.Get());
		m_crossFadeEffect3->SetInput(0, m_bitmap4.Get());
		m_crossFadeEffect3->SetValue(D2D1_CROSSFADE_PROP_WEIGHT, weight);
		m_renderInfo->SetEffect(m_crossFadeEffect3.Get());
		if (weight >= 1.f)
		{
			weight = 0.f;
			clipNum++;
			x = 0;
		}
		m_output = m_crossFadeEffect3.Get();
	}
	else if (clipNum == 3)
	{
		m_crossFadeEffect4->SetInputEffect(1, m_crossFadeEffect3.Get());
		m_crossFadeEffect4->SetInput(0, m_bitmap5.Get());
		m_crossFadeEffect4->SetValue(D2D1_CROSSFADE_PROP_WEIGHT, weight);
		m_renderInfo->SetEffect(m_crossFadeEffect4.Get());
		if (weight >= 1.f)
		{
			weight = 0.f;
			clipNum++;
			x = 0;
		}
		m_output = m_crossFadeEffect4.Get();
		m_opacityEffect->SetInputEffect(0, m_output);
		m_opacityEffect->SetValue(D2D1_COMPOSITE_MODE_SOURCE_OVER, 0.f);
		m_output = m_opacityEffect.Get();
		OnEvent("DORMANT");
		return;
	}

	m_opacityEffect->SetInputEffect(0, m_output);
	m_opacityEffect->SetValue(D2D1_COMPOSITE_MODE_SOURCE_OVER, 1.f);
	m_output = m_opacityEffect.Get();
}

void Clip_Effect::OnEvent(const std::string& ev)
{
	m_currendState = ev;
	if (ev == "PLAY")
	{
		isStop = false;
		weight = 0.f;
		clipNum = 0;
		x = 0;
	}
	else if (ev == "DORMANT")
	{
		isStop = true;
	}
}
