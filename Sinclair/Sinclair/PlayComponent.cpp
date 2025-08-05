#include "PlayComponent.h"

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

	m_renderInfo->SetWorldTM(m_transform.GetWorldMatrix());
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

//Rotate_Effect::Rotate_Effect(RenderInfo* renderInfo, Transform& transform, float rotate)
//	: m_renderInfo(renderInfo), m_transform(transform), m_rotate(rotate)
//{
//	/*transform.SetPivotPreset(PivotPreset::Center, m_renderInfo->GetSize());*/
//}
//
//void Rotate_Effect::Update()
//{
//	if (isStop)	return;
//
//	m_transform.Rotate(m_rotate);
//
//	m_renderInfo->SetWorldTM(m_transform.GetWorldMatrix());
//	std::cout << m_transform.GetPivotPoint().x << std::endl;
//}
//
//void Rotate_Effect::OnEvent(const std::string& ev)
//{
//	if (ev == "STOP")
//	{
//		isStop = true;
//	}
//	else if (ev == "PLAY")
//	{
//		isStop = false;
//	}
//}

Bounce_Effect::Bounce_Effect(RenderInfo* renderInfo, Transform& transform, float x_maxScale, float y_maxScale, float totalSecond)
	: m_renderInfo(renderInfo), m_transform(transform), mx_maxScale(x_maxScale), my_maxScale(y_maxScale), m_totalSecond(totalSecond)
{
	m_scale = m_transform.GetScale();
}

void Bounce_Effect::FixedUpdate(float dt)
{
	static float time = 0;
	static float x = 0;

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

	D2D1_VECTOR_2F scale{ Graph(x, mx_maxScale), Graph(x, my_maxScale) };
	m_transform.SetScale(scale);

	if (m_transform.GetScale().x < 0.f && m_transform.GetScale().y < 0.f)
	{
		x = 0.f;
		// m_renderInfo->SetisActive(false);
	}
}

Blinking_Effect::Blinking_Effect(RenderInfo* renderInfo, float minOpacity, float totalSecond)
	: m_renderInfo(renderInfo), m_minOpacity(minOpacity), m_totalSecond(totalSecond)
{
}

void Blinking_Effect::FixedUpdate(float dt)
{
	if (isStop) return;

	static float time = 0;
	static float x = 0;

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

	m_renderInfo->SetOpacity(Graph(x));
	if (x >= m_totalSecond)
	{
		x = 0.f;
	}
}

void Blinking_Effect::OnEvent(const std::string& ev)
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

	m_renderInfo->SetWorldTM(m_transform.GetWorldMatrix());
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
