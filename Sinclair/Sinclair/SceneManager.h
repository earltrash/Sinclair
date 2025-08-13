#pragma once
#include "pch.h"
#include "Scene.h"
#include "AssetProvider.h"

#include "Scene_MK.h"
#include "Scene_BJ.h"
#include "Scene_Tutorial.h"
#include "Scene_Title.h"
#include "Scene_Outgame.h"
#include "Scene_InGame.h"
#include "Scene_End.h"
#include "Scene_History.h"


using namespace std;

class SceneManager {
private:
	SceneManager() = default;
	~SceneManager() = default;
public:

	void Initalize(shared_ptr<unordered_map<string, shared_ptr<SceneStandard>>>& Scenes)
	{

		m_Scene_map = make_shared<unordered_map<string, shared_ptr<SceneStandard>>>();


		m_Scene_map->emplace("Title", make_shared<Scene_Title>("Title"));
		m_Scene_map->emplace("Tutorial", make_shared<Scene_Tutorial>("Tutorial"));
		m_Scene_map->emplace("OutGame", make_shared<Scene_Outgame>("OutGame"));
		m_Scene_map->emplace("InGame", make_shared<Scene_InGame>("InGame"));
		m_Scene_map->emplace("End", make_shared<Scene_End>("End"));
		m_Scene_map->emplace("History", make_shared<Scene_History>("History"));

		m_Scene_map->emplace("SceneBj", make_shared<SceneBj>("SceneBj"));
		m_Scene_map->emplace("MK", make_shared<Scene_MK>("MK"));

		m_currentindex = "Title";
		Scenes = m_Scene_map;

		m_Scene_map->at(m_currentindex)->Enter(); 
	}


	//그럼 얘는 core랑 얘기 되어야 할듯? 

	void ChangeScene(const string& index) {
		m_Scene_map->at(m_currentindex)->Exit();
		m_currentindex = index;
		m_Scene_map->at(m_currentindex)->Enter();
	};

	static SceneManager& Get() {
		static SceneManager scenemanager;
		return scenemanager;
	};

	string GetCurrentIndex() //이게 최저 수준의 소통 수준 
	{
		return m_currentindex;
	}


	void Clean()
	{
		for (auto& [key, scene] : *m_Scene_map)
		{
			scene->Clean();
		}
	}


public:
	string m_currentindex;
	shared_ptr<unordered_map<string, shared_ptr<SceneStandard>>> m_Scene_map = nullptr; 
};
