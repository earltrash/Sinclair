#pragma once
#include "pch.h"
#include "IAssetProvider.h"
#include "AnimationClip.h"
#include "Object.h"


class SceneStandard
{
public: //�� �����ִ� �� ���� �� ��� �ϴµ�, core�� manager���� ������ ��¿ �� ���µ�.

		SceneStandard() = default;
		~SceneStandard() = default;

		virtual void Initalize() {}; //SpriteAnimator �� ���⼭ ���� ���� ������. ��������� ������ 
		virtual void Clean() {};
		//�� ���� �������̽��� �ִ°Ŵ� ���� �����ε���. 

		virtual void Update();
		virtual void LogicUpdate(float delta);
		virtual void Enter() {};

		virtual void Exit() { }; //���� Ŭ�������� ó�� 
		virtual void Render();


protected:
		
		void GetAsset(string info) { ResourceManager::Get().GetClips(info); } //ResourceManager �̱������� �޾ƿͼ� �� ����
		void AssetMapping();

		virtual bool ObjCreate() { return true; };
protected:
		

		std::vector<Clip_Asset> SceneAssets;


		std::map<std::string, std::shared_ptr<Object>> m_gameObjects; //�̹�Scene�� ���Ǵ� ���� obj
		std::multimap<std::string, std::shared_ptr<Object>> m_uiObjects;   //�̹�Scene�� ���Ǵ� UI obj
		std::multimap<std::string, std::shared_ptr<Object>> m_itemObjects; //�̹�Scene�� ���Ǵ� Item obj

		bool dirty = false;
		string m_name;

};


