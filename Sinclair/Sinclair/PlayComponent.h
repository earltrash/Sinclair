#pragma once
#include "pch.h"
#include "Renderer.h"
#include "object.h"
#include "Component.h"
#include "RenderInfo.h"

using namespace Microsoft::WRL;
#define FPS60 0.0166666666666667f

class Slide_Effect;		// 옆으로 이동 연출
class UpDown_Effect;	// 위아래 왔다갔다 연출
class Blink_Effect;
class Explode_Effect;
class DynamicContrast_Effect;

class Slide_Effect : public Component
{
public:
	Slide_Effect(RenderInfo* renderInfo, Transform& transform, float speed, float bitmapWidth, float screenWidth, bool isRight);

	void Update() override;
	void OnEvent(const std::string& ev) override;

	void SetSpeed(float sp) { m_speed = sp; }
private:
	float m_speed;

	float m_bitmapWidth;
	float m_screenWidth;

	bool m_isRight;
	bool isStop = false;

	Transform& m_transform;
	RenderInfo* m_renderInfo = nullptr;
};

class UpDown_Effect : public Component	// 위아래 왔다갔다하는 효과
{
public:
	UpDown_Effect(RenderInfo* renderInfo, Transform& transform, float moveAmount, float upSpeed, float downSpeed);
	void Update() override;
	void OnEvent(const std::string& ev) override;

	void SetUpSpeed(float sp) { m_upSpeed = sp; }
	void SetDownSpeed(float sp) { m_downSpeed = sp; }
	void SetOriginPos() { m_transform.SetPosition({ m_transform.GetPosition().x, m_position.y }); }	// 본래 높이로 돌아가기
private:
	float m_moveAmount;					// 세로로 어디까지 움직일 건지

	float m_upSpeed;					// 올라가는 속도 : 0.1부터 확인 추천
	float m_downSpeed;					// 내려가는 속도 : 0.1부터 확인 추천

	bool isUp = true;
	bool isStop = false;

	D2D1_VECTOR_2F m_position;			// 본래 y위치 저장할 용도
	Transform& m_transform;
	RenderInfo* m_renderInfo = nullptr;
};

class Blink_Effect : public Component
{
public:
	Blink_Effect(RenderInfo* renderInfo, float minOpacity, float totalSecond, ID2D1Effect* effect);
	Blink_Effect(RenderInfo* renderInfo, float minOpacity, float totalSecond, ID2D1Bitmap1* bitmap);
	~Blink_Effect() { m_opacityEffect.Reset(); }

	void Initialize();

	void FixedUpdate(float dt) override;

	float Graph(float x)
	{
		float a = 4.f * (1.f - m_minOpacity);
		float result = (a * x * x) / (m_totalSecond * m_totalSecond) - ((a * x) / m_totalSecond) + 1.f;

		return result;
	}

	ID2D1Effect* GetEffect() { return m_opacityEffect.Get(); }
private:
	float time = 0.f;
	float x = 0.f;

	float m_totalSecond = 0.f;	// 애니메이션 총 시간
	float m_minOpacity = 0.f;	// 최소밝기

	bool isStop = false;

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_opacityEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Explode_Effect : public Component
{
public:
	Explode_Effect(RenderInfo* renderInfo, float x_currentScale, float y_currentScale, float x_addScale, float y_addScale, float totalSecond, ID2D1Effect* effect);
	Explode_Effect(RenderInfo* renderInfo, float x_currentScale, float y_currentScale, float x_addScale, float y_addScale, float totalSecond, ID2D1Bitmap1* bitmap);
	Explode_Effect() { m_offsetEffect.Reset(); }

	void Initialize();

	void FixedUpdate(float dt) override;
	void OnEvent(const std::string& ev) override;

	float GraphA(float x, float addScale)
	{
		float a = -(4.f * addScale) / (m_totalSecond * m_totalSecond); // 포물선 계수
		float shiftX = m_totalSecond / 2.f; // x축 이동량 (원래 중심)
		float yOffset = m_currentScale.y - a * (-shiftX) * (m_totalSecond - shiftX); // y축 이동량

		float result = a * (x - shiftX) * (x - m_totalSecond + shiftX) + yOffset;
		return result;
	}

	float Graph(float x, float addScale)
	{
		float result = (-(4.f * addScale) / (m_totalSecond * m_totalSecond)) * x * (x - m_totalSecond);
		return result;
	}

	ID2D1Effect* GetEffect() { return m_offsetEffect.Get(); }
private:
	float time = 0.f;
	float x = 0.f;

	D2D1_VECTOR_2F m_currentScale;

	float m_totalSecond = 0.f;		// 스케일 애니메이션 총 시간
	float mx_addScale = 0.f;		// x 최대 스케일
	float my_addScale = 0.f;		// y 최대 스케일

	bool isFirst = true;
	bool isStop = false;			// 초기값이 true로 되어 있어야함.	false ev가 들어왔을 때 효과주고 비활성화

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_offsetEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class DynamicContrast_Effect : public Component
{
public:
	DynamicContrast_Effect(RenderInfo* renderInfo, float minContrast, float maxContrast, float addNum, ID2D1Effect* effect);
	DynamicContrast_Effect(RenderInfo* renderInfo, float minContrast, float maxContrast, float addNum, ID2D1Bitmap1* bitmap);
	~DynamicContrast_Effect() { m_contrastEffect.Reset(); }

	void Initialize();

	void Update() override;

	ID2D1Effect* GetEffect() { return m_contrastEffect.Get(); }

private:
	float m_minContrast;			// -1~1까지
	float m_maxContrast;			// -1~1까지
	float m_addNum;					// 증가량
	float m_currentStrength = 0.f;	// 대조 효과 기본 0.f

	bool isUp = true;

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_contrastEffect;

	RenderInfo* m_renderInfo = nullptr;
};