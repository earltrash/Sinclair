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
		virtual void Initalize()override; //SpriteAnimator 를 여기서 만들어도 되지 않을까. 만들어지는 시점은 
		virtual void Clean()override;
		//위 둘을 인터페이스로 넣는거는 취향 영역인듯함. 

		virtual void Update()override;
		virtual void LogicUpdate(float delta)override;
		virtual void Enter()override;

		virtual void Exit() override; //하위 클래스에서 처리 
		virtual void Render()override;
public:
	void CreateObj();
	std::string getRandomText();

private:

	int m_id;

	std::multimap<int, std::string> outGameTextTable = {
	{1, "늘 나도 언젠가는 아버지처럼\n모험을 떠나고 싶다고 생각해왔어."},
	{1, "이제는 때가 된 것 같아.\n하지만… 장비는 어떡하지 ?"},
	{1, "분명 아버지도 이해해주실거야.\n아버지의 창고에 들어가볼까 ?"},
	{2, "> 창고로 이동한다.\n> 모험을 떠난다."},
	{3, "창고에 들어가시겠습니까?"},
	{4, "여행을 떠나시겠습니까?\n(주의.한번 모험을 떠나면 되돌릴 수 없습니다)"}
	};

};

