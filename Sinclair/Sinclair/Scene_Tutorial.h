#pragma once
#include "Scene.h"

class Scene_Tutorial : public SceneStandard
{
public:
	Scene_Tutorial(string name);
	~Scene_Tutorial();


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
private:
	//unique_ptr<Object>	Background;
	//unique_ptr<Object>	TutorialButton1;
	//unique_ptr<Object>	TutorialButton2;
};


