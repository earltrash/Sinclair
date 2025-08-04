#pragma once
#include "pch.h"
#include "Dispatcher.h"
#include "MouseInput.h"
using namespace std;

class InputManager { //얘 결국 싱글톤 안 쓸거면 얘 멤버를 싱글톤 하는 게 구조상 더 나아보이긴 함. 얘는 뭐랄까 관제탑 역할만 하는 애라. 

private:
		InputManager();
		~InputManager();
public:
		static InputManager& Get();
		bool MsgCheck(MSG& msg); //InputManager에서 다루는 입력이 아닌 경우는 프로시저로 넘김. 
		void Clean();

public:
		unique_ptr<EventDispatcher> m_broadcaster = nullptr;


};
