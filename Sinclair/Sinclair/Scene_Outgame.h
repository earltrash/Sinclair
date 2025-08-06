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
		ó���ƿ����� = 80001,
		������ = 80002,
		â������ = 80003,
		���趰����Ŭ���� = 80004

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

	State m_id = ó���ƿ�����;
	bool wasInGame = false;

	std::multimap<State, std::string> outGameTextTable = {
	{ó���ƿ�����, "�� ���� �������� �ƹ���ó��\n������ ������ �ʹٰ� �����ؿԾ�."},
	{ó���ƿ�����, "������ ���� �� �� ����.\n�������� ���� ����� ?"},
	{ó���ƿ�����, "�и� �ƹ����� �������ֽǰž�.\n�ƹ����� â�� ������ ?"},
	{������, "> â��� �̵��Ѵ�.\n> ������ ������."},
	{â������, "â�� ���ðڽ��ϱ�?"},
	{���趰����Ŭ����, "������ �����ðڽ��ϱ�?\n(����.�ѹ� ������ ������ �ǵ��� �� �����ϴ�)"}
	};

	std::string curText;

};

