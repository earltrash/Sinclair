#pragma once
#include "pch.h"
#include "Scene.h"
#include "AssetProvider.h"
//#include "ObjFactory.h"
//#include "SceneFactory.h"

#include "Scene_MK.h"
#include "Scene_Title.h"
#include "Scene_Outgame.h"
#include "Scene_InGame.h"
#include "Scene_End.h"
//#include "Scene_History.h"


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
				m_Scene_map->emplace("OutGame", make_shared<Scene_Outgame>("OutGame"));
				m_Scene_map->emplace("InGame", make_shared<Scene_InGame>("InGame"));
				m_Scene_map->emplace("End", make_shared<Scene_End>("End"));
				//m_Scene_map->emplace("History", make_shared<Scene_History>("History"));

				m_Scene_map->emplace("MK", make_shared<Scene_MK>("MK"));

				m_currentindex = "End";
				//m_currentindex = "MK";

				Scenes = m_Scene_map;

				m_Scene_map->at(m_currentindex)->Enter(); //다음은 바로 update니깐 

		}


		//그럼 얘는 core랑 얘기 되어야 할듯? 

		void ChangeScene(const string& index) {

				//MSG msg;
				//while (PeekMessage(&msg, nullptr, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE)) {} //기존에 있는 msg 정리하기 


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

		void SetAssetProvider(std::shared_ptr<IAssetProvider> provider) {
				m_assetProvider = provider;
		}
		void SetRenderer(const std::shared_ptr<D2DRenderer>& renderer)
		{
				if (!renderer)
						std::cout << "SetRenderer 전달받은 renderer가 null임!" << std::endl;
				m_Renderer = renderer;
		}

		void Clean()
		{
				m_Renderer.reset();
				m_assetProvider.reset();

				for (auto& [key, scene] : *m_Scene_map)
				{
						scene->Clean();
				}
		}


public:
		string m_currentindex;
		shared_ptr<unordered_map<string, shared_ptr<SceneStandard>>> m_Scene_map = nullptr; //core의 Scene이랑 공유함.

		std::shared_ptr<IAssetProvider> m_assetProvider;
		std::shared_ptr<D2DRenderer> m_Renderer;
};
