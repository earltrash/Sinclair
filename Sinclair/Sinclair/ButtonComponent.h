#pragma once
#include "pch.h"
#include "Component.h"
#include "Object.h"
#include "RenderInfo.h"
#include "MouseInput.h"
using namespace Microsoft::WRL;
using namespace std;

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

class ButtonComponent : public Component
{
public:
	//ButtonComponent() = default;
	ButtonComponent(RenderInfo* renderInfo) : m_renderInfo(renderInfo) {}
	~ButtonComponent() { m_Bitmaps.clear(); }
	
	void BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap);

	//현재 비트맵 갖기
	void SetCurrentBitmap(string Nm);
	ComPtr<ID2D1Bitmap1> GetBitmap();

public:
	enum class ButtonState { Normal, Hover, Pressed, Disabled };

	void SetState(ButtonState state) 
	{
		m_currentState = state;
		if (m_Bitmaps.size() > 100) return;
		m_renderInfo->SetBitmap(GetBitmap().Get());
	};
	ButtonState GetState() const { return m_currentState; }

	// 콜백 함수 등록
	void SetOnClickCallback(std::function<void()> callback) { m_onClick = callback; }
	void OnClick() { if (m_onClick) m_onClick(); }

	void CheckCollision(const MSG& MSG);
	void Worked(const MSG& MSG);

	void SetWidth (float w)  { m_renderInfo->SetDestRight (w); }
	void SetHeight(float h) { m_renderInfo->SetDestBottom(h); }

	void SetOpacity(float o) { m_renderInfo->SetOpacity(o); }



private:
	// 버튼을 갈아끼우는 용도
	unordered_map<string, ComPtr<ID2D1Bitmap1>> m_Bitmaps;

	ComPtr<ID2D1Bitmap1> m_curbm;

	ButtonState m_currentState = ButtonState::Normal;

	std::function<void()> m_onClick;

	RenderInfo* m_renderInfo;

	bool wasInside = false;
	bool isInside = false;
};