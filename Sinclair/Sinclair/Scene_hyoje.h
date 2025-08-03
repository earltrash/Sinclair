#pragma once
#include "Scene.h"
class Scene_hyoje : public SceneStandard
{
public:
	Scene_hyoje(string name);
	~Scene_hyoje();

public:
	//virtual void Initalize() override;
	//virtual void Clean() override;

	////virtual void Update() override;
	////virtual void LogicUpdate(float delta) override;
	//virtual void Enter() override;
	//virtual void Exit() override;
	virtual void Render();

	void CreateButtons();
	void CreateBackground();
};

