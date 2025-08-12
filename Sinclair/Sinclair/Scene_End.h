#pragma once
#include "Scene.h"

class Scene_End : public SceneStandard
{
public:
	Scene_End(string name);
	~Scene_End();

	virtual void Initalize() override; //SpriteAnimator �� ���⼭ ���� ���� ������. ��������� ������ 
	virtual void Clean() override;
	//�� ���� �������̽��� �ִ°Ŵ� ���� �����ε���. 

	virtual void Update() override;
	virtual void LogicUpdate(float delta) override;
	virtual void Enter() override;
	virtual void Exit() override; //���� Ŭ�������� ó�� 
	virtual void Render()override;
public:
	void CreateObj();

	std::wstring StrToWstr(const std::string& source)
	{
		if (source.empty()) return std::wstring();

		// CP_ACP: �ý��� �⺻ �ڵ� ������ (�ѱ��� Windows = CP949)
		int size = MultiByteToWideChar(CP_ACP, 0, source.c_str(), -1, nullptr, 0);
		if (size <= 0) return std::wstring();

		std::wstring result(size - 1, 0);
		MultiByteToWideChar(CP_ACP, 0, source.c_str(), -1, &result[0], size);
		return result;
	}

	void SafeChangeScene(const std::string& sceneName, int gen);

private:
	// ������ �ؽ�Ʈ ���̺��� �ƿ������� ���̺��ϰ� �ٸ��� 
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> ebm;



	bool  m_textShown = false;
	bool  m_titleShown = false;
	bool  m_scriptShown = false;
	float m_currentShowingDelay = 0.f;
	float m_showingDelay = 1.5f;			// �ؽ�Ʈ ����
	int   index = 0;
};

