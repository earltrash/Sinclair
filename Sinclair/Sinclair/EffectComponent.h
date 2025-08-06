#pragma once
#include "pch.h"
#include "Renderer.h"
#include "Component.h"
#include "RenderInfo.h"

#define FPS60 0.0166666666666667f

using namespace Microsoft::WRL;

class GaussianBlur_Effect;					// 블러 효과
class Shadow_Effect;						// 그림자 효과
class Offset_Effect;						// 이동 효과
//class Composite_Effect;						// 이미지 합성 효과
class Crop_Effect;							// 이미지 크롭 효과
class Contrast_Effect;						// 이미지 대비 강조 효과
class Vignette_Effect;						// 테두리 음영 적용 효과
class Scale_Effect;							// 스케일 조정 효과
class Posterize_Effect;						// 포스터화 효과 : 만화같은 느낌이나 복고풍 느낌을 주는 필터
class Gray_Effect;							// 무채색화 효과
class Rotate3D_Effect;						// 3D 회전 효과
class Color_Effect;							// 색상 바꾸기 효과

class GaussianBlur_Effect : public Component				// 블러 효과
{
public:
	GaussianBlur_Effect(RenderInfo* renderInfo, float strength, ID2D1Bitmap1* bitmap);	// 오브젝트의 비트맵에 효과 적용
	GaussianBlur_Effect(RenderInfo* renderInfo, float strength, const wchar_t* path);	// 다른 비트맵 생성하여 효과 적용
	GaussianBlur_Effect(RenderInfo* renderInfo, float strength, ID2D1Effect* pEffect);	// 다른 이펙트가 적용된 비트맵에 효과 적용
	~GaussianBlur_Effect() { m_gaussianBlurEffect.Reset(); }

	void Update() override;

	void SetStrength(float strength) { m_strength = strength; }

	ID2D1Effect* GetEffect() { return m_gaussianBlurEffect.Get(); }
private:
	float m_strength;							// 블러 적용 정도 : 1 ~ 6까지 추천

	ID2D1Effect* m_effect = nullptr;			// 블러 효과 적용할 이펙트
	ComPtr<ID2D1Bitmap1> m_bitmap;				// 블러 효과 적용할 비트맵

	ComPtr<ID2D1Effect> m_gaussianBlurEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Shadow_Effect : public Component			// 그림자 효과
{
public:
	Shadow_Effect(RenderInfo* renderInfo, float edgeBlur, float r, float g, float b, float a, ID2D1Bitmap1* bitmap);	// 오브젝트의 비트맵에 효과 적용
	Shadow_Effect(RenderInfo* renderInfo, float edgeBlur, float r, float g, float b, float a, const wchar_t* path);		// 다른 비트맵 생성하여 효과 적용
	Shadow_Effect(RenderInfo* renderInfo, float edgeBlur, float r, float g, float b, float a, ID2D1Effect* effect);		// 다른 이펙트가 적용된 비트맵에 효과 적용

	~Shadow_Effect() { m_shadowEffect.Reset(); }

	void Update() override;

	void SetEdgeBlur(float edgeBlur) { m_edgeBlur = edgeBlur; }
	void SetShadowColor(float r, float g, float b, float a) { m_shadowColor.x = r;  m_shadowColor.y = g; m_shadowColor.z = b; m_shadowColor.w = a; }

	ID2D1Effect* GetEffect() { return m_shadowEffect.Get(); }
private:
	float m_edgeBlur;

	D2D1_VECTOR_4F m_shadowColor;

	ID2D1Effect* m_effect = nullptr;			// 그림자 효과 적용할 이펙트b
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;	// 그림자 효과 적용할 비트맵

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

	ID2D1Effect* m_effect = nullptr;			// 이동 효과 적용할 이펙트
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;	// 이동 그림자 효과 적용할 비트맵

	ComPtr<ID2D1Effect> m_offsetEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Composite_Effect : public Component		// 이미지 합성 효과
{
public:
	Composite_Effect(RenderInfo* renderInfo, ID2D1Effect* top, ID2D1Effect* bottom, D2D1_COMPOSITE_MODE mode);		// 이펙트끼리 합성
	Composite_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* top, ID2D1Effect* bottom, D2D1_COMPOSITE_MODE mode);		// 비트맵(위)과 이펙트(아래) 합성
	Composite_Effect(RenderInfo* renderInfo, ID2D1Effect* top, ID2D1Bitmap1* bottom, D2D1_COMPOSITE_MODE mode);		// 비트맵(아래)과 이펙트(위) 합성
	Composite_Effect(RenderInfo* renderInfo, const wchar_t* top, ID2D1Bitmap1* bottom, D2D1_COMPOSITE_MODE mode);	// 새 이미지(위)와 비트맵(아래) 합성
	Composite_Effect(RenderInfo* renderInfo, ID2D1Bitmap1* top, const wchar_t* bottom, D2D1_COMPOSITE_MODE mode);	// 새 이미지(아래)와 비트맵(위) 합성
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

class Crop_Effect : public Component		// 이미지 크롭 효과
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

class Contrast_Effect : public Component		// 이미지 대비 강조 효과
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

class Vignette_Effect : public Component	// 테두리 음영 적용 효과
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
	float m_blurRadius;		// 0~1까지
	float m_strength;		// 0~1까지

	D2D1_VECTOR_4F m_color;

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_vignetteEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Scale_Effect : public Component		// 스케일 조정 효과
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

// 포스터화 효과 : 원래 부드러운 색상 그라데이션을 인위적으로 단계별 색상 구간으로 나눠서
// 만화같은 느낌이나 복고풍 느낌을 주는 필터
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
	// 2~255까지
	int m_redStep;
	int m_greenStep;
	int m_blueStep;

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_posterEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Gray_Effect : public Component	// 그레이화 효과
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
	float m_strength;		// 0~1까지

	ID2D1Effect* m_effect = nullptr;
	ComPtr<ID2D1Bitmap1> m_bitmap = nullptr;

	ComPtr<ID2D1Effect> m_grayEffect;

	RenderInfo* m_renderInfo = nullptr;
};

class Rotate3D_Effect : public Component	// 회전 효과
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
	float m_depth;											// 원근감. 값이 작아질 수록 원근감↑, 커질 수록 원근감↓ (보통 100~1000 사이)

	D2D1_VECTOR_3F m_pivot{ 0.f, 0.f, 0.f };				// 회전 피봇
	D2D1_VECTOR_3F m_rotate{ 0.f, 0.f, 0.f };				// 회전량
	D2D1_VECTOR_3F m_rotation{ 0.f, 0.f, 0.f };				// 현재 회전 상태

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
//	D2D1_RECT_U sourceRect;     // 시트 내 프레임
//	D2D1_COLOR_F color;         // Tint + alpha
//
//	float rotation;             // 도 단위 (degrees)
//	float age;                  // 생존 시간
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