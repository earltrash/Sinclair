#pragma once
#include "pch.h"
#include "Renderer.h"
#include "Component.h"
#include "RenderInfo.h"

#define FPS60 0.0166666666666667f

using namespace Microsoft::WRL;

class GaussianBlur_Effect;					// �� ȿ��
class Shadow_Effect;						// �׸��� ȿ��
class Offset_Effect;						// �̵� ȿ��
//class Composite_Effect;						// �̹��� �ռ� ȿ��
class Crop_Effect;							// �̹��� ũ�� ȿ��
class Contrast_Effect;						// �̹��� ��� ���� ȿ��
class Vignette_Effect;						// �׵θ� ���� ���� ȿ��
class Scale_Effect;							// ������ ���� ȿ��
class Posterize_Effect;						// ������ȭ ȿ�� : ��ȭ���� �����̳� ����ǳ ������ �ִ� ����
class Gray_Effect;							// ��ä��ȭ ȿ��
class Rotate3D_Effect;						// 3D ȸ�� ȿ��
class Color_Effect;							// ���� �ٲٱ� ȿ��

class GaussianBlur_Effect : public Component				// �� ȿ��
{
public:
	GaussianBlur_Effect(RenderInfo* renderInfo, float strength, ID2D1Bitmap1* bitmap);	// ������Ʈ�� ��Ʈ�ʿ� ȿ�� ����
	GaussianBlur_Effect(RenderInfo* renderInfo, float strength, const wchar_t* path);	// �ٸ� ��Ʈ�� �����Ͽ� ȿ�� ����
	GaussianBlur_Effect(RenderInfo* renderInfo, float strength, ID2D1Effect* pEffect);	// �ٸ� ����Ʈ�� ����� ��Ʈ�ʿ� ȿ�� ����
	~GaussianBlur_Effect() { m_gaussianBlurEffect.Reset(); }

	void Update() override;

	void SetStrength(float strength) { m_strength = strength; }

	ID2D1Effect* GetEffect() { return m_gaussianBlurEffect.Get(); }
private:
	float m_strength;							// �� ���� ���� : 1 ~ 6���� ��õ

	ID2D1Effect* m_effect = nullptr;			// �� ȿ�� ������ ����Ʈ
	ComPtr<ID2D1Bitmap1> m_bitmap;				// �� ȿ�� ������ ��Ʈ��

	ComPtr<ID2D1Effect> m_gaussianBlurEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Shadow_Effect : public Component			// �׸��� ȿ��
{
public:
	Shadow_Effect(RenderInfo* renderInfo, float edgeBlur, float r, float g, float b, float a, ID2D1Bitmap1* bitmap);	// ������Ʈ�� ��Ʈ�ʿ� ȿ�� ����
	Shadow_Effect(RenderInfo* renderInfo, float edgeBlur, float r, float g, float b, float a, const wchar_t* path);		// �ٸ� ��Ʈ�� �����Ͽ� ȿ�� ����
	Shadow_Effect(RenderInfo* renderInfo, float edgeBlur, float r, float g, float b, float a, ID2D1Effect* effect);		// �ٸ� ����Ʈ�� ����� ��Ʈ�ʿ� ȿ�� ����

	~Shadow_Effect() { m_shadowEffect.Reset(); }

	void Update() override;

	void SetEdgeBlur(float edgeBlur) { m_edgeBlur = edgeBlur; }
	void SetShadowColor(float r, float g, float b, float a) { m_shadowColor.x = r;  m_shadowColor.y = g; m_shadowColor.z = b; m_shadowColor.w = a; }

	ID2D1Effect* GetEffect() { return m_shadowEffect.Get(); }
private:
	float m_edgeBlur;

	D2D1_VECTOR_4F m_shadowColor;

	ID2D1Effect* m_effect = nullptr;			// �׸��� ȿ�� ������ ����Ʈb
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;	// �׸��� ȿ�� ������ ��Ʈ��

	ComPtr<ID2D1Effect> m_shadowEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Offset_Effect : public Component
{
public:
	Offset_Effect(RenderInfo* renderInfo, float xOffset, float yOffset, ID2D1Bitmap1* bitmap);
	Offset_Effect(RenderInfo* renderInfo, float xOffset, float yOffset, const wchar_t* path);
	Offset_Effect(RenderInfo* renderInfo, float xOffset, float yOffset, ID2D1Effect* effect);
	~Offset_Effect() { m_offsetEffect.Reset(); }

	void Update() override;

	void SetOffset(float xOffset, float yOffset) { m_xOffset = xOffset; m_yOffset = yOffset; }

	ID2D1Effect* GetEffect() { return m_offsetEffect.Get(); }
private:
	float m_xOffset;
	float m_yOffset;

	ID2D1Effect* m_effect = nullptr;			// �̵� ȿ�� ������ ����Ʈ
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;	// �̵� �׸��� ȿ�� ������ ��Ʈ��

	ComPtr<ID2D1Effect> m_offsetEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Composite_Effect : public Component		// �̹��� �ռ� ȿ��
{
public:
	Composite_Effect(RenderInfo* renderInfo, ID2D1Effect* top, ID2D1Effect* bottom, D2D1_COMPOSITE_MODE mode);		// ����Ʈ���� �ռ�
	Composite_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* top, ID2D1Effect* bottom, D2D1_COMPOSITE_MODE mode);		// ��Ʈ��(��)�� ����Ʈ(�Ʒ�) �ռ�
	Composite_Effect(RenderInfo* renderInfo, ID2D1Effect* top, ID2D1Bitmap1* bottom, D2D1_COMPOSITE_MODE mode);		// ��Ʈ��(�Ʒ�)�� ����Ʈ(��) �ռ�
	Composite_Effect(RenderInfo* renderInfo, const wchar_t* top, ID2D1Bitmap1* bottom, D2D1_COMPOSITE_MODE mode);	// �� �̹���(��)�� ��Ʈ��(�Ʒ�) �ռ�
	Composite_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* top, const wchar_t* bottom, D2D1_COMPOSITE_MODE mode);	// �� �̹���(�Ʒ�)�� ��Ʈ��(��) �ռ�
	Composite_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* top, ID2D1Bitmap1* bottom, D2D1_COMPOSITE_MODE mode);
	~Composite_Effect() { m_compositeEffect.Reset(); }

	void Update() override;

	void SetCompositeMode(D2D1_COMPOSITE_MODE mode) { m_mode = mode; }

	ID2D1Effect* GetEffect() { return m_compositeEffect.Get(); }
private:
	D2D1_COMPOSITE_MODE m_mode;

	ID2D1Effect* m_EffectT = nullptr;
	ID2D1Effect* m_EffectB = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmapT = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmapB = nullptr;

	ComPtr<ID2D1Effect> m_compositeEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Crop_Effect : public Component		// �̹��� ũ�� ȿ��
{
public:
	Crop_Effect(RenderInfo* renderInfo, float x, float y, float width, float height, ID2D1Effect* effect);
	Crop_Effect(RenderInfo* renderInfo, float x, float y, float width, float height, ID2D1Bitmap1* bitmap);
	Crop_Effect(RenderInfo* renderInfo, float x, float y, float width, float height, const wchar_t* path);
	~Crop_Effect() { m_cropEffect.Reset(); }

	void Update() override;

	void SetPosX(float x) { m_rect.left = x; }
	void SetPosY(float y) { m_rect.top = y; }
	void SetWidth(float width) { m_rect.right = width; }
	void SetHeight(float height) { m_rect.bottom = height; }

	ID2D1Effect* GetEffect() { return m_cropEffect.Get(); }

private:

	D2D1_RECT_F m_rect;

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_cropEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Contrast_Effect : public Component		// �̹��� ��� ���� ȿ��
{
public:
	Contrast_Effect(RenderInfo* renderInfo, float strength, ID2D1Effect* effect);
	Contrast_Effect(RenderInfo* renderInfo, float strength, ID2D1Bitmap1* bitmap);
	Contrast_Effect(RenderInfo* renderInfo, float strength, const wchar_t* path);
	~Contrast_Effect() { m_contrastEffect.Reset(); }

	void Update() override;

	void SetStrength(float str) { m_strength = str; }

	ID2D1Effect* GetEffect() { return m_contrastEffect.Get(); }

private:
	float m_strength;

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_contrastEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Vignette_Effect : public Component	// �׵θ� ���� ���� ȿ��
{
public:
	Vignette_Effect(RenderInfo* renderInfo, float blurRadius, float strength, float r, float g, float b, float a, ID2D1Effect* effect);
	Vignette_Effect(RenderInfo* renderInfo, float blurRadius, float strength, float r, float g, float b, float a, ID2D1Bitmap1* bitmap);
	Vignette_Effect(RenderInfo* renderInfo, float blurRadius, float strength, float r, float g, float b, float a, const wchar_t* path);
	~Vignette_Effect() { m_vignetteEffect.Reset(); }

	void Update() override;

	void SetBlurRadius(float blur) { m_blurRadius = blur; }
	void SetStrength(float str) { m_strength = str; }
	void SetColor(float r, float g, float b, float a) { m_color.x = r; m_color.y = g; m_color.z = b; m_color.w = a; }

	ID2D1Effect* GetEffect() { return m_vignetteEffect.Get(); }
private:
	float m_blurRadius;		// 0~1����
	float m_strength;		// 0~1����

	D2D1_VECTOR_4F m_color;

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_vignetteEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Scale_Effect : public Component		// ������ ���� ȿ��
{
public:
	Scale_Effect(RenderInfo* renderInfo, float pivotX, float pivotY, float scaleX, float scaleY, ID2D1Effect* effect);
	Scale_Effect(RenderInfo* renderInfo, float pivotX, float pivotY, float scaleX, float scaleY, ID2D1Bitmap1* bitmap);
	Scale_Effect(RenderInfo* renderInfo, float pivotX, float pivotY, float scaleX, float scaleY, const wchar_t* path);
	~Scale_Effect() { m_scaleEffect.Reset(); }

	void Update() override;

	void SetPivot(float x, float y) { m_pivot.x = x; m_pivot.y = y; }
	void SetScale(float x, float y) { m_scale.x = x; m_scale.y = y; }

	ID2D1Effect* GetEffect() { return m_scaleEffect.Get(); }
private:
	D2D1_VECTOR_2F m_pivot;
	D2D1_VECTOR_2F m_scale;

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_scaleEffect;

	RenderInfo* m_renderInfo = nullptr;
};

// ������ȭ ȿ�� : ���� �ε巯�� ���� �׶��̼��� ���������� �ܰ躰 ���� �������� ������
// ��ȭ���� �����̳� ����ǳ ������ �ִ� ����
class Posterize_Effect : public Component
{
public:
	Posterize_Effect(RenderInfo* renderInfo, int redStep, int greenStep, int blueStep, ID2D1Effect* effect);
	Posterize_Effect(RenderInfo* renderInfo, int redStep, int greenStep, int blueStep, ID2D1Bitmap1* bitmap);
	Posterize_Effect(RenderInfo* renderInfo, int redStep, int greenStep, int blueStep, const wchar_t* path);
	~Posterize_Effect() { m_posterEffect.Reset(); }

	void Update() override;

	void SetRedStep(int step) { m_redStep = step; }
	void SetGreenStep(int step) { m_greenStep = step; }
	void SetBlueStep(int step) { m_blueStep = step; }

	ID2D1Effect* GetEffect() { return m_posterEffect.Get(); }

private:
	// 2~255����
	int m_redStep;
	int m_greenStep;
	int m_blueStep;

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_posterEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Gray_Effect : public Component	// �׷���ȭ ȿ��
{
public:
	Gray_Effect(RenderInfo* renderInfo, float strength, ID2D1Effect* effect);
	Gray_Effect(RenderInfo* renderInfo, float strength, ID2D1Bitmap1* bitmap);
	Gray_Effect(RenderInfo* renderInfo, float strength, const wchar_t* path);
	~Gray_Effect() { m_grayEffect.Reset(); }

	void Update() override;

	void SetStrength(float str) { m_strength = str; }

	ID2D1Effect* GetEffect() { return m_grayEffect.Get(); }
private:
	float m_strength;		// 0~1����

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_grayEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Rotate3D_Effect : public Component	// ȸ�� ȿ��
{
public:
	Rotate3D_Effect(RenderInfo* renderInfo, float depth, float pivotX, float pivotY,  float rotateX, float rotateY, float rotateZ, ID2D1Effect* effect);
	Rotate3D_Effect(RenderInfo* renderInfo, float depth, float pivotX, float pivotY,  float rotateX, float rotateY, float rotateZ, ID2D1Bitmap1* bitmap);
	Rotate3D_Effect(RenderInfo* renderInfo, float depth, float pivotX, float pivotY,  float rotateX, float rotateY, float rotateZ, const wchar_t* path);
	~Rotate3D_Effect() { m_perspectiveEffect.Reset(); }

	void Update() override;

	void SetPivot(float x, float y) { m_pivot.x = x; m_pivot.y = y; }
	void SetRotate(float x, float y, float z) { m_rotate.x = x; m_rotate.y = y; m_rotate.z = z; }

	ID2D1Effect* GetEffect() { return m_perspectiveEffect.Get(); }
private:
	float m_depth;											// ���ٰ�. ���� �۾��� ���� ���ٰ���, Ŀ�� ���� ���ٰ��� (���� 100~1000 ����)

	D2D1_VECTOR_3F m_pivot{ 0.f, 0.f, 0.f };				// ȸ�� �Ǻ�
	D2D1_VECTOR_3F m_rotate{ 0.f, 0.f, 0.f };				// ȸ����
	D2D1_VECTOR_3F m_rotation{ 0.f, 0.f, 0.f };				// ���� ȸ�� ����

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_perspectiveEffect;

	RenderInfo* m_renderInfo = nullptr;
};
class Color_Effect : public Component
{
public:
	Color_Effect(RenderInfo* renderInfo, float r, float g, float b, float a, ID2D1Effect* effect);
	Color_Effect(RenderInfo* renderInfo, float r, float g, float b, float a, ID2D1Bitmap1* bitmap);
	Color_Effect(RenderInfo* renderInfo, float r, float g, float b, float a, const wchar_t* path);

	void Update() override;

	ID2D1Effect* GetEffect() { return m_colorEffect.Get(); }
private:
	float m_r;
	float m_g;
	float m_b;
	float m_a;

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_colorEffect;

	RenderInfo* m_renderInfo = nullptr;
};

//struct Particle
//{
//	D2D1_POINT_2F position;
//	D2D1_SIZE_F size;
//
//	D2D1_RECT_U sourceRect;     // ��Ʈ �� ������
//	D2D1_COLOR_F color;         // Tint + alpha
//
//	float rotation;             // �� ���� (degrees)
//	float age;                  // ���� �ð�
//	float lifetime;
//};
//
//class Particle_Effect : public Component
//{
//public:
//	Particle_Effect(int particle_count);
//
//	void Update() override;
//	void FixedUpdate(float dt) override;
//
//private:
//	int count = 100;
//	float safeMargin = 10.f;
//	float screenWidth = 1920.f;
//	float screenHeight = 1080.f;
//
//	bool isStop = false;
//
//	std::vector<Particle> m_Particles;
//};