#pragma once
#include "Scene.h"
#include <iostream>
#include <map>
#include <string>
#include <Windows.h>

class Scene_Outgame : public SceneStandard
{
		
public:
		Scene_Outgame(string name);
		~Scene_Outgame();

public:
		virtual void Initalize()override; //SpriteAnimator 를 여기서 만들어도 되지 않을까. 만들어지는 시점은 
		virtual void Clean()override;
		//위 둘을 인터페이스로 넣는거는 취향 영역인듯함. 

		virtual void Update()override;
		virtual void LogicUpdate(float delta)override;
		virtual void Enter()override;

		virtual void Exit() override; //하위 클래스에서 처리 
		virtual void Render()override;
private:
	void CreateObj();
	std::string getRandomText();


	void SetupCharacterAndBackground();
	

	enum State {
		FIRST_ENTER = 80001,
		CHOICE_MENU = 80002,
		ENTER_OUTGAME = 80003,
		ENTER_END = 80004
	};

	void ChangeState(State newState);


	std::wstring StrToWstr(const std::string& source)
	{
		if (source.empty()) return std::wstring();

		// CP_ACP: 시스템 기본 코드 페이지 (한국어 Windows = CP949)
		int size = MultiByteToWideChar(CP_ACP, 0, source.c_str(), -1, nullptr, 0);
		if (size <= 0) return std::wstring();

		std::wstring result(size - 1, 0);
		MultiByteToWideChar(CP_ACP, 0, source.c_str(), -1, &result[0], size);
		return result;
	}
	
private:

	State m_state = FIRST_ENTER;
	bool wasInGame = false;

	std::multimap<State, std::string> outGameTextTable = {
	{FIRST_ENTER , "늘 나도 언젠가는 아버지처럼\n모험을 떠나고 싶다고 생각해왔어."},
	{FIRST_ENTER , "이제는 때가 된 것 같아.\n하지만… 장비는 어떡하지 ?"},
	{FIRST_ENTER , "분명 아버지도 이해해주실거야.\n아버지의 창고에 들어가볼까 ?"},
	{CHOICE_MENU , "> 창고로 이동한다.\n> 모험을 떠난다."},
	{ENTER_OUTGAME , "창고에 들어가시겠습니까?"},
	{ENTER_END , "여행을 떠나시겠습니까?\n(주의.한번 모험을 떠나면 되돌릴 수 없습니다)"}
	};

	std::string curText;

	bool isChango = false;
	bool isMoheom = false;

};

