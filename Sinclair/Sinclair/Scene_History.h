#pragma once
#include "Scene.h"

class Scene_History : public SceneStandard
{
public:
	Scene_History(string name);
	~Scene_History();

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

	//void ActivateAllTexts();
	void DeactivateAllTexts();
	void ActivateSingleText(int idx);


	// 이거 3곳에서 쓰이는 데.. 유틸리티로..
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

	enum class History { H1, H2, H3, H4 };

	History m_History;

	bool  m_allTextsShown = false;
	float m_currentShowingDelay = 0.f;
	float m_showingDelay = 3.0f;			// 텍스트 간격
	int   index = 0;
	int   m_targetTextCount = 6;				// H2면 6, 아니면 5
};

