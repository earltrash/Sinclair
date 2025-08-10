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

	D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 90.0f);
	std::wstring job = StrToWstr(ResourceManager::Get().Get_TextBank().EndingVector[GameManager::Get().curGen * 10].������);
	D2DRenderer::Get().DrawMessageCenter(job.c_str(),
		1080.f, 120.f, 1920.f - 1080.f, 255.f - 120.f, D2D1::ColorF::White);

	D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 30.0f);
	std::wstring text = StrToWstr(ResourceManager::Get().Get_TextBank().EndingVector[GameManager::Get().curGen * 10].�����ؽ�Ʈ);
	D2DRenderer::Get().DrawMessageCenter(text.c_str(),
		1223.f, 255.f, 564.f, 1080.f - 255.f, D2D1::ColorF::White);
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
	auto History2 = ResourceManager::Get().GetTexture("H1");
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