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

	vector<vector<string>> vvs =
	{
		{"한 시대를 말하는 이름."s, ""s, "역사서의 첫 장을 장식하는 이름—싱클레어."s, "그 시작은 작았지만, 그 끝은 누구보다도 찬란했다."s, ""s, "학자들은 그들을 연구했고, 전사들은 그들의 휘장을 따랐으며,"s, "아이들은 전설 속 영웅처럼 그 이름을 불렀다."s, ""s, "이제, 싱클레어는 하나의 가문이 아닌,"s, "하나의 시대가 되었다."s},
		{"조용한 물줄기가 길을 열다."s, ""s, "처음엔 아무도 주목하지 않았다."s, "하지만 시간이 흐를수록, 사람들은 그 이름을 다시 보게 되었다."s, ""s, "장인의 도구 위에, 학자의 문장 속에,"s, "병사의 노래 끝에—싱클레어는 어느새 많은 곳에 스며들어 있었다."s, "찬란하지는 않아도, 묵직하고 단단한 무언가."s, "그 이름은 이제, 한 도시의 이야기 속에서"s, "조용히 중심을 차지하고 있었다."s},
		{"작지만 흔들리지 않는 흐름."s, ""s, "작은 이름은 세월 속에서도 잊히지 않았다."s, "작은 책 한구석, 누군가의 기억 속에서"s, "싱클레어의 흔적이 살아남았다."s, ""s, "화려하진 않아도 단단한 뿌리를 내린 그 이름."s, ""s, "그렇게, 한 가계는 오래도록"s, "기억될 이야기가 되었다."s},
		{"그저 그런 삶. 하지만, 지켜낸 이름."s, ""s, "특별할 것 없는 나날이 흘렀다."s, "싱클레어라는 이름은 여전히 작고 조용했다."s, ""s, "누군가의 기억에 깊이 새겨지지는 않았지만,"s, "그들은 끝까지 서로의 곁을 지켰다."s, ""s, "이야기는 여기서 멈추지만,"s, "그 이름은 여전히 한켠에서 조용히 살아간다."s}
	};
};

