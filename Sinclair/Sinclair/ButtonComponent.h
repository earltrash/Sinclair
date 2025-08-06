#pragma once
#include "pch.h"
#include "Component.h"
#include "Object.h"
#include "RenderInfo.h"

using namespace Microsoft::WRL;
using namespace std;

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

class ButtonComponent : public Component
{
public:
	//ButtonComponent() = default;
	ButtonComponent(RenderInfo* renderInfo) : m_renderInfo(renderInfo) {}
	~ButtonComponent() { m_Bitmap.clear(); }
	
	void BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap);

	//현재 비트맵 갖기
	void SetCurrentBitmap(string Nm);
	ComPtr<ID2D1Bitmap1> GetBitmap();

public:
	enum class ButtonState { Normal, Hover, Pressed, Disabled };

	void SetState(ButtonState state) 
	{
		m_currentState = state;
		if (m_Bitmap.size() > 100) return;
		m_renderInfo->SetBitmap(GetBitmap().Get());
	};
	ButtonState GetState() const { return m_currentState; }

	// 콜백 함수 등록
	void SetOnClickCallback(std::function<void()> callback) { m_onClick = callback; }
	void OnClick() { if (m_onClick) m_onClick(); }

	void Worked(const MSG& MSG);

	// Getter
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	// Setter
	void SetWidth(int w) { width = w; m_renderInfo->SetDestRight(width); }
	void SetHeight(int h) { height = h; m_renderInfo->SetDestBottom(height); }
	
	// get set 나중에 만듬
	float m_opacity = 1.0f;

	unordered_map<string, ComPtr<ID2D1Bitmap1>> m_Bitmap; 

	void SetInvisibleButton(int width, int height);
private:
	bool m_isInvisible = false;
	
	ComPtr<ID2D1Bitmap1> m_curbm;

	ButtonState m_currentState = ButtonState::Normal;

	std::function<void()> m_onClick;

	int width, height;

	RenderInfo* m_renderInfo;
};