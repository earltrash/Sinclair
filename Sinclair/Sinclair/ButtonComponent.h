#pragma once
#include "pch.h"
#include "Component.h"

using namespace Microsoft::WRL;
using namespace std;



class ButtonComponent : public Component
{
public:
	ButtonComponent() = default;
	~ButtonComponent() = default;
	
	void BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap);

	//���� ��Ʈ�� ����
	void SetCurrentBitmap(string Nm);
	ComPtr<ID2D1Bitmap1> GetBitmap();

public:
	enum class ButtonState { Normal, Hover, Pressed, Disabled };

	void SetState(ButtonState state) { m_currentState = state; };
	ButtonState GetState() const { return m_currentState; }

	// �ݹ� �Լ� ���
	void SetOnClickCallback(std::function<void()> callback) { m_onClick = callback; }
	void OnClick() { if (m_onClick) m_onClick(); }

private:
	unordered_map<string, ComPtr<ID2D1Bitmap1>> m_Bitmap; 
	ComPtr<ID2D1Bitmap1> m_curbm;

	ButtonState m_currentState = ButtonState::Normal;
	std::function<void()> m_onClick;
};