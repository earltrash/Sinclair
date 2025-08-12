#pragma once
#include "Scene.h"

class Scene_End : public SceneStandard
{
public:
	Scene_End(string name);
	~Scene_End();

	virtual void Initalize() override; //SpriteAnimator 를 여기서 만들어도 되지 않을까. 만들어지는 시점은 
	virtual void Clean() override;
	//위 둘을 인터페이스로 넣는거는 취향 영역인듯함. 

	virtual void Update() override;
	virtual void LogicUpdate(float delta) override;
	virtual void Enter() override;
	virtual void Exit() override; //하위 클래스에서 처리 
	virtual void Render()override;
public:
	void CreateObj();

	std::wstring StrToWstr(const std::string& source)
	{
		if (source.empty()) return std::wstring();

		// CP_ACP: 시스템 기본 코드 페이지 (한국어 Windows = CP949)
		int size = MultiByteToWideChar(CP_ACP, 0, source.c_str(), -1, nullptr, 0);
		if (size <= 0) return std::wstring();

		std::wstring result(size - 1, 0);
		MultiByteToWideChar(CP_ACP, 0, source.c_str(), -1, &result[0], size);
		return result;
	}

	void SafeChangeScene(const std::string& sceneName, int gen);

private:
	// 엔딩의 텍스트 테이블은 아웃게임의 테이블하고 다르게 
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> ebm;


	bool  m_fadeStarted = false;
	bool  after_fadeStarted = false;
	float m_image1Opacity = 0.f;  // 첫 번째 이미지 opacity
	float m_image2Opacity = 0.f;  // 두 번째 이미지 opacity  
	float m_image3Opacity = 0.f;  // 세 번째 이미지 opacity
	float m_currentTime = 0.f;
	float m_fadeDuration = 1.5f;   // 페이드 지속시간
	float m_intervalTime = 1.5f;   // 이미지 간 간격



	std::wstring job;
	std::wstring text;

};

