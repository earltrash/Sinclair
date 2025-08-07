#pragma once
#include "Scene.h"

class Scene_Title : public SceneStandard
{
public:
		Scene_Title(string name);
		~Scene_Title();


public:

		virtual void Initalize() override; //SpriteAnimator �� ���⼭ ���� ���� ������. ��������� ������ 
		virtual void Clean() override;
		//�� ���� �������̽��� �ִ°Ŵ� ���� �����ε���. 

		virtual void Update() override;
		virtual void LogicUpdate(float delta) override;
		virtual void Enter() override;
		virtual void Exit() override; //���� Ŭ�������� ó�� 
		virtual void Render()override;
public:
	void CreateObj();

//private:
//	std::multimap<std::string, std::unique_ptr<Object>> m_gameObjects;

};

