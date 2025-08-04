#pragma once
#include "pch.h"
#include "Dispatcher.h"
#include "SimpleMathHelper.h"
using namespace std;

enum class MouseButton
{
    Left = 0,
    Right = 1,
    Middle = 2
};

enum class KeyCode
{
    // �ʿ��� Ű�� �߰�
    ESC = VK_ESCAPE,
    I = 'I',
    E = 'E',
    P = 'P'
};

class InputManager 
{
public:
		static InputManager& Get();
		bool MsgCheck(MSG& msg); //InputManager���� �ٷ�� �Է��� �ƴ� ���� ���ν����� �ѱ�. 
		void Clean();

        void Update();

        // ���콺 ����
        Vec2 GetMousePosition() const { return m_mousePosition; }
        bool IsMouseClicked(MouseButton button) const;
        bool IsMousePressed(MouseButton button) const;
        bool IsMouseReleased(MouseButton button) const;
        bool IsDoubleClicked() const { return m_isDoubleClicked; }

        // Ű���� ����
        bool IsKeyPressed(KeyCode key) const;
        bool IsKeyDown(KeyCode key) const;
        bool IsKeyUp(KeyCode key) const;



        unique_ptr<EventDispatcher> m_broadcaster = nullptr;
private:
	InputManager();
	~InputManager();

    // ���� ������.
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

   

    // ���콺 ����
    Vec2 m_mousePosition = { 0, 0 };
    bool m_mouseButtons[3] = { false }; // Left, Right, Middle
    bool m_previousMouseButtons[3] = { false };
    bool m_isDoubleClicked = false;

    // Ű���� ����  
    bool m_keys[256] = { false };
    bool m_previousKeys[256] = { false };

    // ����Ŭ�� Ÿ�̸�
    float m_doubleClickTimer = 0.0f;
    const float DOUBLE_CLICK_TIME = 0.3f;

};
