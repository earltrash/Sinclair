#pragma once
#include "pch.h"
#include "Renderer.h"
#include "object.h"
#include "Component.h"
#include "RenderInfo.h"
#include "EffectComponent.h"

using namespace Microsoft::WRL;
#define FPS60 0.0166666666666667f

class Slide_Effect;		// 옆으로 이동 연출
class UpDown_Effect;	// 위아래 왔다갔다 연출
class Blink_Effect;
class Explode_Effect;
class DynamicContrast_Effect;
class Increase_Effect;
class Sideway_Effect;
class Clip_Effect;

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
	// minOpacity 쓰임 사라짐;;
	Blink_Effect(RenderInfo* renderInfo, float minOpacity, float totalSecond, ID2D1Effect* effect);
	Blink_Effect(RenderInfo* renderInfo, float minOpacity, float totalSecond, ID2D1Bitmap1* bitmap);
	~Blink_Effect() { m_opacityEffect.Reset(); }

	void Initialize();

	void FixedUpdate(float dt) override;

	void OnEvent(const std::string& ev) override;

	float Graph(float x)
	{
		//float a = 4.f * (1.f - m_minOpacity);
		//float result = (a * x * x) / (m_totalSecond * m_totalSecond) - ((a * x) / m_totalSecond) + 1.f;

		float result = -(2.f / m_totalSecond * x - 1.f) * (2.f / m_totalSecond * x - 1.f) + 1.f;

		return result;
	}

	ID2D1Effect* GetEffect() { return m_opacityEffect.Get(); }
private:
	float time = 0.f;
	float x = 0.f;

	float m_totalSecond = 0.f;	// 애니메이션 총 시간
	float m_minOpacity = 0.f;	// 최소밝기

	bool isStop = true;

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_opacityEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Explode_Effect : public Component
{
public:
	Explode_Effect(RenderInfo* renderInfo, float x_currentScale, float y_currentScale, float x_addScale, float y_addScale, float totalSecond, ID2D1Effect* effect);
	Explode_Effect(RenderInfo* renderInfo, D2D1_POINT_2F pivot, D2D1_VECTOR_2F currentScale, D2D1_VECTOR_2F addScale, float totalSecond, ID2D1Effect* effect);
	Explode_Effect(RenderInfo* renderInfo, float x_currentScale, float y_currentScale, float x_addScale, float y_addScale, float totalSecond, ID2D1Bitmap1* bitmap);
	Explode_Effect() { m_offsetEffect.Reset(); }

	void Initialize();

	void FixedUpdate(float dt) override;
	void OnEvent(const std::string& ev) override;

	const std::string GetState() { return m_currendState; }

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

	void SetInputEffect(ID2D1Effect* effect)
	{
		m_offsetEffect->SetInputEffect(0, effect);
	}
	void SetInput(ID2D1Bitmap1* bitmap)
	{
		m_offsetEffect->SetInput(0, bitmap);
	}

	const D2D1_VECTOR_2F& GetScale() { return { offsetMT.m11, offsetMT.m22 }; }

	ID2D1Effect* GetEffect() { return m_offsetEffect.Get(); }
private:
	std::string m_currendState;
	float time = 0.f;
	float x = 0.f;

	D2D1_MATRIX_3X2_F offsetMT;
	D2D1_VECTOR_2F m_currentScale;
	D2D1_POINT_2F m_pivot{ 0.f, 0.f };

	float m_totalSecond = 0.f;		// 스케일 애니메이션 총 시간
	float mx_addScale = 0.f;		// x 최대 스케일
	float my_addScale = 0.f;		// y 최대 스케일

	bool isFirst = false;
	bool isStop = true;				// 초기값이 true로 되어 있어야함.	false ev가 들어왔을 때 효과주고 비활성화

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

class Increasing_Effect : public Component
{
public:
	Increasing_Effect(RenderInfo* renderInfo, D2D1_POINT_2F pivot, float maxScale, float totalTime, ID2D1Effect* effect);
	Increasing_Effect(RenderInfo* renderInfo, float maxScale, float totalTime, ID2D1Bitmap1* bitmap);
	Increasing_Effect(RenderInfo* renderInfo, float maxScale, float totalTime, ID2D1Effect* effect);
	~Increasing_Effect() {}

	void Initialize();

	void FixedUpdate(float dt) override;
	void OnEvent(const std::string& ev) override;

	void SetScale(float scale);
	float Graph(float x)
	{
		//float result = x / m_totalTime;
		//return result;

		float result = (-(4.f * (m_maxScale + m_offset)) / (m_totalTime * m_totalTime)) * x * (x - m_totalTime);
		return result;
	}
	float GraphDisappear(float x)
	{
		float result = -m_maxScale / m_totalTime * x + m_maxScale;
		return result;
	}
	float Wave(float t, float period)
	{
		float amplitude = 0.2f;               // 물결 높이
		float frequency = 2 * 3.141592f / period; // 주기 = period
		float baseScale = m_maxScale;         // 기본 스케일

		return baseScale + amplitude * sin(frequency * t);
	}

	const std::string GetBeforeState() { return m_beforeState; }
	const std::string GetCurrentState() { return m_currendState; }
	const bool GetisStop() { return isStop; }
	const D2D1_VECTOR_2F& GetScale() { return { offsetMT.m11, offsetMT.m22 }; }

	ID2D1Effect* GetEffect() { return m_offsetEffect.Get(); }
private:
	std::string m_beforeState;
	std::string m_currendState;
	float time = 0.f;
	float x = 0.f;

	float m_maxScale;
	float m_totalTime;
	float m_offset = 0.2f;

	int bounceNum = 0;
	bool isDisappear = false;
	bool isUp = true;
	bool isStop = true;

	D2D1_MATRIX_3X2_F offsetMT;
	D2D1_POINT_2F m_pivot{0.f, 0.f};

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_offsetEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Sideway_Effect : public Component
{
public:
	Sideway_Effect(RenderInfo* renderInfo, float moveAmount, float rightSpeed, float leftSpeed, ID2D1Effect* effect);
	Sideway_Effect(RenderInfo* renderInfo, float moveAmount, float rightSpeed, float leftSpeed, ID2D1Bitmap1* bitmap);
	~Sideway_Effect() { m_offsetEffect.Reset(); }

	void Initialize();

	void Update();

	ID2D1Effect* GetEffect() { return m_offsetEffect.Get(); }
private:
	float m_moveAmount;
	float m_currentPos = 0.f;;
	float m_rightSpeed; 
	float m_leftSpeed;

	bool isRight = true;
	bool isStop = false;

	D2D1_MATRIX_3X2_F offsetMT;

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_offsetEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Fade_Effect : public Component
{
public:
	Fade_Effect(RenderInfo* renderInfo, float startOpacity, float maxOpacity, float adjustNum, ID2D1Effect* effect);
	Fade_Effect(RenderInfo* renderInfo, float startOpacity, float maxOpacity, float adjustNum, ID2D1Bitmap1* bitmap);
	~Fade_Effect() { m_opacityEffect.Reset(); }

	void Initialize();

	void Update();

	void OnEvent(const std::string& ev) override;

	ID2D1Effect* GetEffect() { return m_opacityEffect.Get(); }
private:
	float m_adjustNum;
	float m_maxOpcity;
	float m_currentOpacity;
	bool isFadeIn;
	bool isStop = true;

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_opacityEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class BackForth_Effect : public Component
{
public:
	BackForth_Effect(RenderInfo* renderInfo, float moveAmount, float upSpeed, float downSpeed, ID2D1Effect* effect);
	BackForth_Effect(RenderInfo* renderInfo, float moveAmount, float upSpeed, float downSpeed, ID2D1Bitmap1* bitmap);
	~BackForth_Effect() { m_offsetEffect.Reset(); }

	void Initialize();

	void Update();

	//void OnEvent(const std::string& ev) override;

	ID2D1Effect* GetEffect() { return m_offsetEffect.Get(); }
private:
	float m_moveAmount;
	float m_currentPos = 0.f;
	float m_upSpeed;
	float m_downSpeed;

	bool isUp = true;
	bool isStop = false;

	D2D1_MATRIX_3X2_F offsetMT;

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_offsetEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Clip_Effect : public Component
{
public:
	//Clip_Effect(RenderInfo* renderInfo, ID2D1Effect* effect1, ID2D1Effect* effect2);
	//Clip_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* bitmap1, ID2D1Bitmap1* bitmap2);
	//Clip_Effect(RenderInfo* renderInfo, ID2D1Effect* effect1, ID2D1Bitmap1* bitmap2);
	//Clip_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* bitmap1, ID2D1Effect* effect2);
	Clip_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* bitmap1, ID2D1Bitmap1* bitmap2, ID2D1Bitmap1* bitmap3, ID2D1Bitmap1* bitmap4, ID2D1Bitmap1* bitmap5);
	~Clip_Effect();

	//void Initialize();

	void FixedUpdate(float dt) override;

	void OnEvent(const std::string& ev) override;

	//bool GetIsDone() { return isDone; }

	const std::string GetCurrentState() { return m_currendState; }
	ID2D1Effect* GetEffect() { return m_output; }
private:

	std::string m_currendState;

	float time = 0.f;
	float x = 0.f;
	float weight = 0.f;

	int clipNum = 0;

	//bool isDone = false;
	bool isStop = true;

	ID2D1Effect* m_Effect1 = nullptr;
	ID2D1Effect* m_Effect2 = nullptr;

	ID2D1Effect* m_output = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap1 = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap2 = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap3 = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap4 = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap5 = nullptr;

	ComPtr<ID2D1Effect> m_crossFadeEffect1;
	ComPtr<ID2D1Effect> m_crossFadeEffect2;
	ComPtr<ID2D1Effect> m_crossFadeEffect3;
	ComPtr<ID2D1Effect> m_crossFadeEffect4;
	ComPtr<ID2D1Effect> m_opacityEffect;

	RenderInfo* m_renderInfo = nullptr;
};