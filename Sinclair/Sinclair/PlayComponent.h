#pragma once
#include "pch.h"
#include "object.h"
#include "Component.h"
#include "RenderInfo.h"

using namespace Microsoft::WRL;
#define FPS60 0.0166666666666667f

class UpDown_Effect;	// 위아래 왔다갔다 연출
//class Rotate_Effect;	// 회전 연출
class Explode_Effect;
class Blinking_Effect;

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

//class Rotate_Effect : public Component	// 회전 효과
//{
//public:
//	Rotate_Effect(RenderInfo* renderInfo, Transform& transform, float rotate);
//	void Update() override;
//	void OnEvent(const std::string& ev) override;
//
//	void SetRotate(float rt) { m_rotate = rt; }
//	void SetOriginRotate() { m_transform.SetRotation(m_rotate); }	// 본래 회전량으로 돌아가기
//private:
//	float m_rotate;					// 회전량
//
//	bool isStop = false;
//
//	Transform& m_transform;
//	RenderInfo* m_renderInfo = nullptr;
//};

// scale이 0에서 max까지 커졌다가 사라지는 효과
// x와 y의 최대 scale 별개 적용 가능
class Explode_Effect : public Component
{
public:
	Explode_Effect(RenderInfo* renderInfo, Transform& transform, float x_maxScale, float y_maxScale, float totalSecond);

private:
	void FixedUpdate(float dt) override;
	void OnEvent(const std::string& ev) override;

	float Graph(float x, float maxScale)
	{
		float result = (-(4.f * maxScale) / (m_totalSecond * m_totalSecond)) * x * (x - m_totalSecond);
		return result;
	}
private:
	D2D1_VECTOR_2F m_scale;

	float m_totalSecond = 0.f;		// 스케일 애니메이션 총 시간
	float mx_maxScale = 0.f;		// x 최대 스케일
	float my_maxScale = 0.f;		// y 최대 스케일

	bool isStop = false;			// 초기값이 true로 되어 있어야함.	false ev가 들어왔을 때 효과주고 비활성화

	Transform& m_transform;
	RenderInfo* m_renderInfo = nullptr;
};

class Blinking_Effect : public Component	// 깜빡이는 효과
{
public:
	Blinking_Effect(RenderInfo* renderInfo, float minOpacity, float totalSecond);

	void FixedUpdate(float dt) override;
	void OnEvent(const std::string& ev) override;

private:
	float Graph(float x)
	{
		float a = 4.f * (1.f - m_minOpacity);
		float result = (a * x * x) / (m_totalSecond * m_totalSecond) - ((a * x) / m_totalSecond) + 1.f;

		return result;
	}
private:
	float m_totalSecond = 0.f;	// 애니메이션 총 시간
	float m_minOpacity = 0.f;	// 최소밝기

	bool isStop = false;

	RenderInfo* m_renderInfo = nullptr;
};