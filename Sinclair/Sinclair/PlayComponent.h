#pragma once
#include "pch.h"
#include "object.h"
#include "Component.h"
#include "RenderInfo.h"

using namespace Microsoft::WRL;
#define FPS60 0.0166666666666667f

class UpDown_Effect;	// ���Ʒ� �Դٰ��� ����
//class Rotate_Effect;	// ȸ�� ����
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

class UpDown_Effect : public Component	// ���Ʒ� �Դٰ����ϴ� ȿ��
{
public:
	UpDown_Effect(RenderInfo* renderInfo, Transform& transform, float moveAmount, float upSpeed, float downSpeed);
	void Update() override;
	void OnEvent(const std::string& ev) override;

	void SetUpSpeed(float sp) { m_upSpeed = sp; }
	void SetDownSpeed(float sp) { m_downSpeed = sp; }
	void SetOriginPos() { m_transform.SetPosition({ m_transform.GetPosition().x, m_position.y }); }	// ���� ���̷� ���ư���
private:
	float m_moveAmount;					// ���η� ������ ������ ����

	float m_upSpeed;					// �ö󰡴� �ӵ� : 0.1���� Ȯ�� ��õ
	float m_downSpeed;					// �������� �ӵ� : 0.1���� Ȯ�� ��õ

	bool isUp = true;
	bool isStop = false;

	D2D1_VECTOR_2F m_position;			// ���� y��ġ ������ �뵵
	Transform& m_transform;
	RenderInfo* m_renderInfo = nullptr;
};

//class Rotate_Effect : public Component	// ȸ�� ȿ��
//{
//public:
//	Rotate_Effect(RenderInfo* renderInfo, Transform& transform, float rotate);
//	void Update() override;
//	void OnEvent(const std::string& ev) override;
//
//	void SetRotate(float rt) { m_rotate = rt; }
//	void SetOriginRotate() { m_transform.SetRotation(m_rotate); }	// ���� ȸ�������� ���ư���
//private:
//	float m_rotate;					// ȸ����
//
//	bool isStop = false;
//
//	Transform& m_transform;
//	RenderInfo* m_renderInfo = nullptr;
//};

// scale�� 0���� max���� Ŀ���ٰ� ������� ȿ��
// x�� y�� �ִ� scale ���� ���� ����
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

	float m_totalSecond = 0.f;		// ������ �ִϸ��̼� �� �ð�
	float mx_maxScale = 0.f;		// x �ִ� ������
	float my_maxScale = 0.f;		// y �ִ� ������

	bool isStop = false;			// �ʱⰪ�� true�� �Ǿ� �־����.	false ev�� ������ �� ȿ���ְ� ��Ȱ��ȭ

	Transform& m_transform;
	RenderInfo* m_renderInfo = nullptr;
};

class Blinking_Effect : public Component	// �����̴� ȿ��
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
	float m_totalSecond = 0.f;	// �ִϸ��̼� �� �ð�
	float m_minOpacity = 0.f;	// �ּҹ��

	bool isStop = false;

	RenderInfo* m_renderInfo = nullptr;
};