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
#include "Renderer.h"

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
	/// 뉴 체인지 씬
	void ChangeScene(const string& index) {
		if (m_nextSceneIndex.empty() && index != m_currentindex) {
			m_nextSceneIndex = index;
			m_isFadeOutEnd = false;
			m_isFadeInEnd = false;
		}
	};

	// 올드 체인지 씬
	void ChangeSceneImmediate(const string& index) {
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

	void UpdateFade(float deltaTime) {
		// 페이드 아웃 중
		if (!m_nextSceneIndex.empty() && !m_isFadeOutEnd) {
			FadeOut(deltaTime);
		}
		// 페이드 아웃 완료 후 씬 교체
		else if (!m_nextSceneIndex.empty() && m_isFadeOutEnd && !m_isSceneChanged) {
			// 실제 씬 변경
			m_Scene_map->at(m_currentindex)->Exit();
			m_currentindex = m_nextSceneIndex;
			m_Scene_map->at(m_currentindex)->Enter();
			m_isSceneChanged = true;
		}
		// 씬 교체 후 페이드 인
		else if (!m_nextSceneIndex.empty() && m_isSceneChanged && !m_isFadeInEnd) {
			FadeIn(deltaTime);
		}
		// 페이드 인 완료 후 정리
		else if (m_isFadeInEnd && m_isSceneChanged) {
			m_nextSceneIndex = "";
			m_isSceneChanged = false;
		}
	}

	// 페이드 아웃 처리
	void FadeOut(float deltaTime) {
		m_fadeTime += deltaTime * 30.0f; // 30fps 기준
		if (m_fadeTime >= 1.0f) {
			m_fadeTime = 0.0f;
			m_opacity += 0.05f;
			if (m_opacity >= 1.0f) {
				m_opacity = 1.0f;
				m_isFadeOutEnd = true;
			}
		}
	}

	// 페이드 인 처리
	void FadeIn(float deltaTime) {
		m_fadeTime += deltaTime * 30.0f;
		if (m_fadeTime >= 1.0f) {
			m_fadeTime = 0.0f;
			m_opacity -= 0.05f;
			if (m_opacity <= 0.0f) {
				m_opacity = 0.0f;
				m_isFadeInEnd = true;
			}
		}
	}

	// 페이드 효과 렌더링 (모든 씬 렌더링 후 마지막에 호출)
	void RenderFade() {
		if (m_opacity > 0.0f) {
			// 전체 화면 크기 가져오기
			auto& renderer = D2DRenderer::Get();
			D2D1_RECT_F fullScreenRect = {
				0.0f,
				0.0f,
				1920.f,
				1080.f
			};

			// 검은색으로 페이드 효과
			renderer.FillRectangle(fullScreenRect, D2D1::ColorF::Black, m_opacity);
		}
	}


	bool IsFading() const {
		return !m_nextSceneIndex.empty();
	}

	bool IsFadeOutComplete() const {
		return m_isFadeOutEnd;
	}

	bool IsFadeInComplete() const {
		return m_isFadeInEnd;
	}
public:
	string m_currentindex;
	shared_ptr<unordered_map<string, shared_ptr<SceneStandard>>> m_Scene_map = nullptr;

private:
	// 페이드 관련 변수들
	float m_opacity = 0.0f;
	bool m_isFadeInEnd = true;
	bool m_isFadeOutEnd = false;
	float m_fadeTime = 0.0f;

	// 씬 전환 관리 변수들
	string m_nextSceneIndex = "";
	bool m_isSceneChanged = false;

};