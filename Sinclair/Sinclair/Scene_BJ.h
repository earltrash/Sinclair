#pragma once
#include "Scene.h"




class SceneBj : public SceneStandard
{
public:
	SceneBj(string name);
	~SceneBj() = default;


	virtual void Initalize() override;
	virtual void Clean() override;

	virtual void Update() override;
	virtual void LogicUpdate(float delta) override;
	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Render();

	void CreateObj();
};