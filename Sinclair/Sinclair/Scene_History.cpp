#include "Scene_History.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "UIManager.h"
#include "GameManager.h"


Scene_History::Scene_History(string name)
{
	m_name = name;
}

Scene_History::~Scene_History()
{

}

void Scene_History::Initalize()
{
	if (dirty) return; // �̹� �ʱ�ȭ�Ǿ����� ��ŵ

	CreateObj(); // ������Ʈ ���� (�� ����)
	dirty = true;
	for (const auto& [Name, obj] : m_gameObjects)
	{
		UIManager::Get().AddSceneObject(obj);
	}
}

void Scene_History::Enter()
{
	//���⼭ �����丮 ���
	int resultFame = GameManager::Get().GetResultFam();
	if (resultFame >= 19)
	{
		m_History = History::H1;
	}
	else if (resultFame >= 15)
	{
		m_History = History::H2;
	}
	else if (resultFame >= 10)
	{
		m_History = History::H3;
	}
	else
	{
		m_History = History::H4;
	}

	Initalize();

}

void Scene_History::Exit()
{

	Clean();
}

void Scene_History::Clean()
{
	if (!dirty) return;

	UIManager::Get().ClearSceneObjects();
	m_gameObjects.clear();

	dirty = false;
}

void Scene_History::Update()
{
	// �� ��ȯ ���� ó��
	if (m_isTransitioning && !m_nextScene.empty())
	{
		m_currentDelay += 0.016f;
		if (m_currentDelay >= m_transitionDelay)
		{
			SceneManager::Get().ChangeScene(m_nextScene);
			m_isTransitioning = false;
			m_nextScene = "";
			m_currentDelay = 0.0f;


		}
	}
}

void Scene_History::LogicUpdate(float delta)
{


}


void Scene_History::Render()
{
	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetRenderInfo()->GetRenderInfo());
	}

	if (m_History == History::H1)
	{
		// ���� (�ڽ� 3)
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
		std::wstring Title = StrToWstr(vvs[0][0]);
		D2DRenderer::Get().DrawMessageCenter(Title.c_str(),
			1100.f, 65.f, 1880.f - 1100.f, 155.f - 65.f, D2D1::ColorF::White);

		// ���� �ؽ�Ʈ�� (�ڽ� 5, 6, 7 ������)
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_NORMAL, 30.0f);

		// ù ��° �ؽ�Ʈ ��� (�ڽ� 5 ����)
		if (vvs[0].size() > 2) {
			std::wstring text1 = StrToWstr(vvs[0][2]);
			D2DRenderer::Get().DrawMessage(text1.c_str(),
				1130.f, 200.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[0].size() > 3) {
			std::wstring text2 = StrToWstr(vvs[0][3]);
			D2DRenderer::Get().DrawMessage(text2.c_str(),
				1130.f, 250.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}

		// �� ��° �ؽ�Ʈ ��� (�ڽ� 6 ����)
		if (vvs[0].size() > 5) {
			std::wstring text3 = StrToWstr(vvs[0][5]);
			D2DRenderer::Get().DrawMessage(text3.c_str(),
				1130.f, 350.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[0].size() > 6) {
			std::wstring text4 = StrToWstr(vvs[0][6]);
			D2DRenderer::Get().DrawMessage(text4.c_str(),
				1130.f, 400.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}

		// �� ��° �ؽ�Ʈ ��� (�ڽ� 7 ����)
		if (vvs[0].size() > 8) {
			std::wstring text5 = StrToWstr(vvs[0][8]);
			D2DRenderer::Get().DrawMessage(text5.c_str(),
				1130.f, 500.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[0].size() > 9) {
			std::wstring text6 = StrToWstr(vvs[0][9]);
			D2DRenderer::Get().DrawMessage(text6.c_str(),
				1130.f, 550.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
	}
	else if (m_History == History::H2)
	{
		// ����
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
		std::wstring Title = StrToWstr(vvs[1][0]);
		D2DRenderer::Get().DrawMessageCenter(Title.c_str(),
			1100.f, 65.f, 1880.f - 1100.f, 155.f - 65.f, D2D1::ColorF::White);

		// ���� �ؽ�Ʈ��
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_NORMAL, 30.0f);

		if (vvs[1].size() > 2) {
			std::wstring text1 = StrToWstr(vvs[1][2]);
			D2DRenderer::Get().DrawMessage(text1.c_str(),
				1130.f, 200.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[1].size() > 3) {
			std::wstring text2 = StrToWstr(vvs[1][3]);
			D2DRenderer::Get().DrawMessage(text2.c_str(),
				1130.f, 250.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[1].size() > 5) {
			std::wstring text3 = StrToWstr(vvs[1][5]);
			D2DRenderer::Get().DrawMessage(text3.c_str(),
				1130.f, 350.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[1].size() > 6) {
			std::wstring text4 = StrToWstr(vvs[1][6]);
			D2DRenderer::Get().DrawMessage(text4.c_str(),
				1130.f, 400.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[1].size() > 7) {
			std::wstring text5 = StrToWstr(vvs[1][7]);
			D2DRenderer::Get().DrawMessage(text5.c_str(),
				1130.f, 450.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[1].size() > 8) {
			std::wstring text6 = StrToWstr(vvs[1][8]);
			D2DRenderer::Get().DrawMessage(text6.c_str(),
				1130.f, 500.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[1].size() > 9) {
			std::wstring text7 = StrToWstr(vvs[1][9]);
			D2DRenderer::Get().DrawMessage(text7.c_str(),
				1130.f, 550.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
	}
	else if (m_History == History::H3)
	{
		// ����
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
		std::wstring Title = StrToWstr(vvs[2][0]);
		D2DRenderer::Get().DrawMessageCenter(Title.c_str(),
			1100.f, 65.f, 1880.f - 1100.f, 155.f - 65.f, D2D1::ColorF::White);

		// ���� �ؽ�Ʈ��
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_NORMAL, 30.0f);

		if (vvs[2].size() > 2) {
			std::wstring text1 = StrToWstr(vvs[2][2]);
			D2DRenderer::Get().DrawMessage(text1.c_str(),
				1130.f, 200.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[2].size() > 3) {
			std::wstring text2 = StrToWstr(vvs[2][3]);
			D2DRenderer::Get().DrawMessage(text2.c_str(),
				1130.f, 250.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[2].size() > 4) {
			std::wstring text3 = StrToWstr(vvs[2][4]);
			D2DRenderer::Get().DrawMessage(text3.c_str(),
				1130.f, 300.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[2].size() > 6) {
			std::wstring text4 = StrToWstr(vvs[2][6]);
			D2DRenderer::Get().DrawMessage(text4.c_str(),
				1130.f, 400.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[2].size() > 8) {
			std::wstring text5 = StrToWstr(vvs[2][8]);
			D2DRenderer::Get().DrawMessage(text5.c_str(),
				1130.f, 500.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[2].size() > 9) {
			std::wstring text6 = StrToWstr(vvs[2][9]);
			D2DRenderer::Get().DrawMessage(text6.c_str(),
				1130.f, 550.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
	}
	else // H4
	{
		// ����
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
		std::wstring Title = StrToWstr(vvs[3][0]);
		D2DRenderer::Get().DrawMessageCenter(Title.c_str(),
			1100.f, 65.f, 1880.f - 1100.f, 155.f - 65.f, D2D1::ColorF::White);

		// ���� �ؽ�Ʈ��
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_NORMAL, 30.0f);

		if (vvs[3].size() > 2) {
			std::wstring text1 = StrToWstr(vvs[3][2]);
			D2DRenderer::Get().DrawMessage(text1.c_str(),
				1130.f, 200.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[3].size() > 3) {
			std::wstring text2 = StrToWstr(vvs[3][3]);
			D2DRenderer::Get().DrawMessage(text2.c_str(),
				1130.f, 250.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[3].size() > 5) {
			std::wstring text3 = StrToWstr(vvs[3][5]);
			D2DRenderer::Get().DrawMessage(text3.c_str(),
				1130.f, 350.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[3].size() > 6) {
			std::wstring text4 = StrToWstr(vvs[3][6]);
			D2DRenderer::Get().DrawMessage(text4.c_str(),
				1130.f, 400.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[3].size() > 8) {
			std::wstring text5 = StrToWstr(vvs[3][8]);
			D2DRenderer::Get().DrawMessage(text5.c_str(),
				1130.f, 500.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[3].size() > 9) {
			std::wstring text6 = StrToWstr(vvs[3][9]);
			D2DRenderer::Get().DrawMessage(text6.c_str(),
				1130.f, 550.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
	}
}

void Scene_History::CreateObj()
{
	//////////////////////
	//////////////////////
	//////////////////////
	// [1] ���� â

	// 1. �̹��� ���� ����
	auto History1 = ResourceManager::Get().GetTexture("����", "01");
	// 2. ������Ʈ �����
	auto ����â = std::make_unique<Object>();
	//����â->SetPosition(Vec2(-20, -16));
	����â->SetPosition(Vec2(0, 0));

	// 3.0. ���� ���� ������Ʈ
	auto ����âinfo = ����â->GetRenderInfo();
	����âinfo->SetBitmap(History1.Get());
	// 3. ��� ������Ʈ �����
	auto ����âComp = ����â->AddComponent<BackgroundComponent>(����âinfo);
	����âComp->SetWidth(1098);
	����âComp->SetHeight(1080);
	����âComp->BitmapPush("����â", History1);
	����âComp->SetCurrentBitmap("����â");

	/// 9
	m_gameObjects.emplace("aa����â", std::move(����â));

	//////////////////////
	//////////////////////
	//////////////////////
	// [2] �̹���
	// ȿ�� : �̰� ��� �ؾ����� ����ϱ�

	// 1. �̹��� ���� ����
	
	
	ComPtr<ID2D1Bitmap1> History2;
	if(m_History == History::H1)
	{
		History2 = ResourceManager::Get().GetTexture("H1");
	}
	else if (m_History == History::H2)
	{
		History2 = ResourceManager::Get().GetTexture("H2");
	}
	else if (m_History == History::H3)
	{
		History2 = ResourceManager::Get().GetTexture("H3");
	}
	else 
	{
		History2 = ResourceManager::Get().GetTexture("H4");
	}
	// 2. ������Ʈ �����
	auto �̹��� = std::make_unique<Object>();
	�̹���->SetPosition(Vec2(17, 22));

	auto �̹���info = �̹���->GetRenderInfo();
	�̹���info->SetBitmap(History2.Get());
	// 3. ��� ������Ʈ �����
	auto �̹���Comp = �̹���->AddComponent<BackgroundComponent>(�̹���info);
	�̹���Comp->SetWidth(1036);
	�̹���Comp->SetHeight(1036);
	�̹���Comp->BitmapPush("�̹���", History2);
	�̹���Comp->SetCurrentBitmap("�̹���");


	/// 9
	m_gameObjects.emplace("�̹���", std::move(�̹���));

	//////////////////////
	//////////////////////
	//////////////////////
	// [3] ���� â

	// 1. �̹��� ���� ����
	auto History3 = ResourceManager::Get().GetTexture("����", "03");
	// 2. ������Ʈ �����
	auto ����â = std::make_unique<Object>();
	����â->SetPosition(Vec2(1080, 0));

	auto ����âinfo = ����â->GetRenderInfo();
	����âinfo->SetBitmap(History3.Get());
	// 3. ��� ������Ʈ �����
	auto ����âComp = ����â->AddComponent<BackgroundComponent>(����âinfo);
	����âComp->SetWidth(843);
	����âComp->SetHeight(1080);
	����âComp->BitmapPush("����â", History3);
	����âComp->SetCurrentBitmap("����â");

	/// 9
	m_gameObjects.emplace("a����â", std::move(����â));

	//////////////////////
	//////////////////////
	//////////////////////
	// [4] ��ŵ ��ư

	// 1. �̹��� ���� ����
	auto History4 = ResourceManager::Get().GetTexture("����", "04");
	// 2. ������Ʈ �����
	auto ��ŵ��ư = std::make_unique<Object>();
	��ŵ��ư->SetPosition(Vec2(1827, 960));

	auto ��ŵ��ưinfo = ��ŵ��ư->GetRenderInfo();
	��ŵ��ưinfo->SetBitmap(History4.Get());

	// 3. ��ư ������Ʈ �����
	auto ��ŵ������Ʈ = ��ŵ��ư->AddComponent<ButtonComponent>(��ŵ��ưinfo);
	��ŵ������Ʈ->SetWidth(History4->GetSize().width);
	��ŵ������Ʈ->SetHeight(History4->GetSize().height);

	// 4. ��ư ��Ʈ�� ����
	��ŵ������Ʈ->BitmapPush("��ŵ", History4);
	��ŵ������Ʈ->SetCurrentBitmap("��ŵ");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto ��ŵ������ = ��ŵ��ư->AddComponent<MouseListenerComponent>(
		[��ŵ������Ʈ](const MSG& msg) {
			��ŵ������Ʈ->CheckCollision(msg);
			��ŵ������Ʈ->Worked(msg);
		}
	);

	��ŵ������Ʈ->SetOnClickCallback([this]() {
		std::cout << "��ŵ ��ư Ŭ����" << std::endl;
		// History ��ŵ ���� �߰�

		SafeChangeScene("Title");
		});

	/// 9
	m_gameObjects.emplace("��ŵ��ư", std::move(��ŵ��ư));


	//////////////////////
	//////////////////////
	//////////////////////
	// [6] ��

	// 1. �̹��� ���� ����
	auto History6 = ResourceManager::Get().GetTexture("����", "06");
	// 2. ������Ʈ �����
	auto �� = std::make_unique<Object>();
	��->SetPosition(Vec2(1223, 255));

	auto ��info = ��->GetRenderInfo();
	��info->SetBitmap(History6.Get());
	// 3. ��� ������Ʈ �����
	auto ��Comp = ��->AddComponent<BackgroundComponent>(��info);
	��Comp->SetWidth(History6->GetSize().width);
	��Comp->SetHeight(History6->GetSize().height);
	��Comp->BitmapPush("��", History6);
	��Comp->SetCurrentBitmap("��");

	/// 9
	m_gameObjects.emplace("��", std::move(��));
}