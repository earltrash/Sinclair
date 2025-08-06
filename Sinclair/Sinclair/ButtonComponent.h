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
	~ButtonComponent() { m_Bitmaps.clear(); }
	
	void BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap);

	//���� ��Ʈ�� ����
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

	// �ݹ� �Լ� ���
	void SetOnClickCallback(std::function<void()> callback) { m_onClick = callback; }
	void OnClick() { if (m_onClick) m_onClick(); }

	void Worked(const MSG& MSG);

	void SetWidth(int w)  { m_renderInfo->SetDestRight (w); }
	void SetHeight(int h) { m_renderInfo->SetDestBottom(h); }

	void SetOpacity(float o) { m_renderInfo->SetOpacity(o); }


	// [ ] ���� ��ư ����
	// ���� ��ư�� ������ ��� 
	// 1. ���� 0.f
	// 2. �����ʸ� �ִ� ����!
	bool m_isInvisible = false;

private:
	// ��ư�� ���Ƴ���� �뵵
	unordered_map<string, ComPtr<ID2D1Bitmap1>> m_Bitmaps;

	

	ButtonState m_currentState = ButtonState::Normal;

	std::function<void()> m_onClick;

	RenderInfo* m_renderInfo;
};