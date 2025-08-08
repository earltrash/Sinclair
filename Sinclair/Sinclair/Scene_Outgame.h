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
		virtual void Initalize()override; //SpriteAnimator �� ���⼭ ���� ���� ������. ��������� ������ 
		virtual void Clean()override;
		//�� ���� �������̽��� �ִ°Ŵ� ���� �����ε���. 

		virtual void Update()override;
		virtual void LogicUpdate(float delta)override;
		virtual void Enter()override;

		virtual void Exit() override; //���� Ŭ�������� ó�� 
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

		// CP_ACP: �ý��� �⺻ �ڵ� ������ (�ѱ��� Windows = CP949)
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
	{FIRST_ENTER , "�� ���� �������� �ƹ���ó��\n������ ������ �ʹٰ� �����ؿԾ�."},
	{FIRST_ENTER , "������ ���� �� �� ����.\n�������� ���� ����� ?"},
	{FIRST_ENTER , "�и� �ƹ����� �������ֽǰž�.\n�ƹ����� â�� ������ ?"},
	{CHOICE_MENU , "> â��� �̵��Ѵ�.\n> ������ ������."},
	{ENTER_OUTGAME , "â�� ���ðڽ��ϱ�?"},
	{ENTER_END , "������ �����ðڽ��ϱ�?\n(����.�ѹ� ������ ������ �ǵ��� �� �����ϴ�)"}
	};

	std::string curText;

	bool isChango = false;
	bool isMoheom = false;

};

