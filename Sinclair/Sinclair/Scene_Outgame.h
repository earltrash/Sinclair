#pragma once
#include "Scene.h"
#include <iostream>
#include <map>
#include <string>
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
public:
	void CreateObj();
	std::string getRandomText();

private:

	int m_id;

	std::multimap<int, std::string> outGameTextTable = {
	{1, "�� ���� �������� �ƹ���ó��\n������ ������ �ʹٰ� �����ؿԾ�."},
	{1, "������ ���� �� �� ����.\n�������� ���� ����� ?"},
	{1, "�и� �ƹ����� �������ֽǰž�.\n�ƹ����� â�� ������ ?"},
	{2, "> â��� �̵��Ѵ�.\n> ������ ������."},
	{3, "â�� ���ðڽ��ϱ�?"},
	{4, "������ �����ðڽ��ϱ�?\n(����.�ѹ� ������ ������ �ǵ��� �� �����ϴ�)"}
	};

};

