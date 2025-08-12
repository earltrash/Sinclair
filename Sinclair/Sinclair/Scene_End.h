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


	bool  m_fadeStarted = false;
	bool  after_fadeStarted = false;
	float m_image1Opacity = 0.f;  // ù ��° �̹��� opacity
	float m_image2Opacity = 0.f;  // �� ��° �̹��� opacity  
	float m_image3Opacity = 0.f;  // �� ��° �̹��� opacity
	float m_currentTime = 0.f;
	float m_fadeDuration = 1.5f;   // ���̵� ���ӽð�
	float m_intervalTime = 1.5f;   // �̹��� �� ����



	std::wstring job;
	std::wstring text;

};

