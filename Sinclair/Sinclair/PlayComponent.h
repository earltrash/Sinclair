#pragma once
#include "pch.h"
#include "Renderer.h"
#include "object.h"
#include "Component.h"
#include "RenderInfo.h"

using namespace Microsoft::WRL;
#define FPS60 0.0166666666666667f

class Slide_Effect;		// ������ �̵� ����
class UpDown_Effect;	// ���Ʒ� �Դٰ��� ����
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

	float m_totalSecond = 0.f;	// �ִϸ��̼� �� �ð�
	float m_minOpacity = 0.f;	// �ּҹ��

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
		float a = -(4.f * addScale) / (m_totalSecond * m_totalSecond); // ������ ���
		float shiftX = m_totalSecond / 2.f; // x�� �̵��� (���� �߽�)
		float yOffset = m_currentScale.y - a * (-shiftX) * (m_totalSecond - shiftX); // y�� �̵���

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

	float m_totalSecond = 0.f;		// ������ �ִϸ��̼� �� �ð�
	float mx_addScale = 0.f;		// x �ִ� ������
	float my_addScale = 0.f;		// y �ִ� ������

	bool isFirst = true;
	bool isStop = false;			// �ʱⰪ�� true�� �Ǿ� �־����.	false ev�� ������ �� ȿ���ְ� ��Ȱ��ȭ

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
	float m_minContrast;			// -1~1����
	float m_maxContrast;			// -1~1����
	float m_addNum;					// ������
	float m_currentStrength = 0.f;	// ���� ȿ�� �⺻ 0.f

	bool isUp = true;

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_contrastEffect;

	RenderInfo* m_renderInfo = nullptr;
};