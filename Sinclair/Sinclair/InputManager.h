#pragma once
#include "pch.h"
#include "Dispatcher.h"
#include "SimpleMathHelper.h"
using namespace std;

enum class InputType
{
    MouseDown  = 0,
    MouseUp,
    MouseMove,
    DoubleClick,
    Unknown
};

enum class MouseButton
{
    Left
};

struct InputEvent
{
    InputType type = InputType::Unknown;
    MouseButton button = MouseButton::Left;
    Vec2 mousePosition = { 0, 0 };
    int keyCode = 0;
    DWORD timestamp = 0;

    InputEvent() = default;
    InputEvent(InputType t, Vec2 pos) : type(t), mousePosition(pos), timestamp(GetTickCount()) {}
    InputEvent(InputType t, int key) : type(t), keyCode(key), timestamp(GetTickCount()) {}
    InputEvent(InputType t, MouseButton btn, Vec2 pos) : type(t), button(btn), mousePosition(pos), timestamp(GetTickCount()) {}
};



class InputManager 
{
public:
		static InputManager& Get();
		bool MsgCheck(MSG& msg); //InputManager에서 다루는 입력이 아닌 경우는 프로시저로 넘김. 
		void Clean();
    void Update();

		unique_ptr<EventDispatcher> m_broadcaster = nullptr;
private:
		InputManager();
		~InputManager();
};
