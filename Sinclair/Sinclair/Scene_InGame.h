#pragma once
#include "Scene.h"
class Scene_InGame : public SceneStandard
{
public:
		Scene_InGame(string name);
		~Scene_InGame();

public:
		virtual void Initalize() override; //SpriteAnimator 를 여기서 만들어도 되지 않을까. 만들어지는 시점은 
		virtual void Clean() override;
		//위 둘을 인터페이스로 넣는거는 취향 영역인듯함. 

		virtual void Update() override;
		virtual void LogicUpdate(float delta) override;
		virtual void Enter() override;
		virtual void Exit() override; //하위 클래스에서 처리 
		virtual void Render()override;
};

