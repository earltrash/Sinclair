#pragma once
#include "pch.h"
#include "IAssetProvider.h"
#include "AnimationClip.h"
#include "Object.h"


class SceneStandard
{
public: //다 열어주는 거 맘에 안 들긴 하는데, core랑 manager에서 쓸려면 어쩔 수 없는듯.

		SceneStandard() = default;
		~SceneStandard() = default;

		virtual void Initalize() {}; //SpriteAnimator 를 여기서 만들어도 되지 않을까. 만들어지는 시점은 
		virtual void Clean() {};
		//위 둘을 인터페이스로 넣는거는 취향 영역인듯함. 

		virtual void Update();
		virtual void LogicUpdate(float delta);
		virtual void Enter() {};

		virtual void Exit() { }; //하위 클래스에서 처리 
		virtual void Render();


protected:
		
		void GetAsset(string info) { ResourceManager::Get().GetClips(info); } //ResourceManager 싱글톤으로 받아와서 함 ㅇㅇ
		void AssetMapping();

		virtual bool ObjCreate() { return true; };
protected:
		

		std::vector<Clip_Asset> SceneAssets;


		std::map<std::string, std::shared_ptr<Object>> m_gameObjects; //이번Scene에 사용되는 게임 obj
		std::multimap<std::string, std::shared_ptr<Object>> m_uiObjects;   //이번Scene에 사용되는 UI obj
		std::multimap<std::string, std::shared_ptr<Object>> m_itemObjects; //이번Scene에 사용되는 Item obj

		bool dirty = false;
		string m_name;

};


