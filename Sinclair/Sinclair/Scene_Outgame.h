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

	vector<string> stringFIRST_ENTER{ "�� ���� �������� �ƹ���ó��\n������ ������ �ʹٰ� �����ؿԾ�." ,
		"������ ���� �� �� ����.\n�������� ���� ����� ?",
		"�и� �ƹ����� �������ֽǰž�.\n�ƹ����� â�� ������ ?"
	};
	int index = 0;
	string stringENTER_OUTGAME{ "â�� ���ðڽ��ϱ�?" };
	string stringENTER_END{ "������ �����ðڽ��ϱ�?\n(����: ������ ������ �ǵ��� �� �����ϴ�.) " };

	std::wstring characterName;
	std::string curText;

};

