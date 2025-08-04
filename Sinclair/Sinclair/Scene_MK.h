#pragma once
#include "Scene.h"

class Scene_MK : public SceneStandard
{
public:
	Scene_MK(string name);
	~Scene_MK();

public:

	virtual void Initalize() override; //SpriteAnimator �� ���⼭ ���� ���� ������. ��������� ������ 
	virtual void Clean() override;
	//�� ���� �������̽��� �ִ°Ŵ� ���� �����ε���. 

	virtual void Update() override;
	virtual void LogicUpdate(float delta) override;
	virtual void Enter() override;
	virtual void Exit() override; //���� Ŭ�������� ó�� 
	virtual void Render()override;

private:
};
