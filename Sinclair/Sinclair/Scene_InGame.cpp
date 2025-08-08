#include "Scene_InGame.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "UIManager.h"

Scene_InGame::Scene_InGame(string name)
{
		m_name = name;
}

Scene_InGame::~Scene_InGame()
{

}

void Scene_InGame::Initalize()
{
	if (dirty) return; // �̹� �ʱ�ȭ�Ǿ����� ��ŵ

	CreateObj(); // ������Ʈ ���� (�� ����)
	dirty = true;


	for (const auto& [Name, obj] : m_gameObjects)
	{
		UIManager::Get().AddSceneObject(obj);
	}
}

void Scene_InGame::Enter()
{
	Initalize();



}

void Scene_InGame::Exit()
{

	Clean();
}

void Scene_InGame::Clean()
{
	UIManager::Get().ClearSceneObjects();

	m_gameObjects.clear();

	dirty = false;
}

void Scene_InGame::Update()
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

void Scene_InGame::LogicUpdate(float delta)
{
}

void Scene_InGame::Render()
{

	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetRenderInfo()->GetRenderInfo());
}

}

void Scene_InGame::CreateObj()
{
	//////////////////////
	//////////////////////
	//////////////////////
	// [0] Background 

	// 1. �̹��� ���� ����
	auto inGameBackground = ResourceManager::Get().GetTexture("�ΰ���");
	// 2. ������Ʈ �����
	auto Background = std::make_unique<Object>();
	Background->SetPosition(Vec2(0, 0));

	// 3.0. ���� ���� ������Ʈ
	auto bgInfo = Background->GetRenderInfo();
	bgInfo->SetBitmap(inGameBackground.Get());
	// 3. ��� ������Ʈ �����
	auto bgComp = Background->AddComponent<BackgroundComponent>(bgInfo);
	// 3.1.1 ������ �ٸ��� 
	bgComp->SetWidth(1920); bgComp->SetHeight(1080);
	bgComp->BitmapPush("Background", inGameBackground);

	bgComp->SetCurrentBitmap("Background");

	// 9. ���� ������Ʈ�鿡 ����ֱ�
	m_gameObjects.emplace("Background", std::move(Background));


	/////////////////////
	/////////////////////
	/////////////////////
	// [1] ���� ��ư

	// 1. �̹��� ���� ����
	auto �ڷΰ��� = ResourceManager::Get().GetTexture("�ڷΰ���");
	// 2. ������Ʈ �����
	auto �ڷ� = std::make_unique<Object>();
	�ڷ�->SetPosition(Vec2(41, 31));

	auto �ڷ�info = �ڷ�->GetRenderInfo();
	�ڷ�info->SetBitmap(�ڷΰ���.Get());
	// 3. ��ư ������Ʈ �����
	auto backComp = �ڷ�->AddComponent<ButtonComponent>(�ڷ�info);
	backComp->SetWidth(�ڷΰ���->GetSize().width);
	backComp->SetHeight(�ڷΰ���->GetSize().height);

	//  4. ��ư ��Ʈ�� ����
	backComp->BitmapPush("back", �ڷΰ���);

	backComp->SetCurrentBitmap("back");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto Back_mouseListener = �ڷ�->AddComponent<MouseListenerComponent>(
		[backComp](const MSG& msg) {
			backComp->Worked(msg);
		}
	);

	backComp->SetOnClickCallback([this]() {
		std::cout << "�ڷΰ��� ��ư Ŭ���� - ���� ��: " << typeid(*this).name() << std::endl;
		SafeChangeScene("OutGame");
		});

	/// 9
	m_gameObjects.emplace("�ڷΰ���", std::move(�ڷ�));




	/////////////////////
	/////////////////////
	/////////////////////
	// [2] â��

	// 1. �̹��� ���� ����
	auto �ΰ���2 = ResourceManager::Get().GetTexture("�ΰ���", "02");
	// 2. ������Ʈ �����
	auto â�� = std::make_unique<Object>();
	â��->SetPosition(Vec2(64, 570));

	auto â��info = â��->GetRenderInfo();
	â��info->SetBitmap(�ΰ���2.Get());

	// 3. ��ư ������Ʈ �����
	auto â���ư = â��->AddComponent<ButtonComponent>(â��info);

	â���ư->SetWidth(�ΰ���2->GetSize().width);
	â���ư->SetHeight(�ΰ���2->GetSize().height);

	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	â���ư->BitmapPush("â��",  �ΰ���2  );

	â���ư->SetCurrentBitmap("â��");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto â������ = â��->AddComponent<MouseListenerComponent>(
		[â���ư](const MSG& msg) {
			â���ư->Worked(msg);
		}
	);

	â���ư->SetOnClickCallback([this]() {
		std::cout << "â��" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("â��", std::move(â��));



	/////////////////////
	/////////////////////
	/////////////////////
	// [3] ���

	// 1. �̹��� ���� ����
	auto �ΰ���3 = ResourceManager::Get().GetTexture("�ΰ���", "03");
	// 2. ������Ʈ �����
	auto ��� = std::make_unique<Object>();
	���->SetPosition(Vec2(64, 703));

	auto ���info = ���->GetRenderInfo();
	���info->SetBitmap(�ΰ���3.Get());
	// 3. ��ư ������Ʈ �����
	auto ����ư = ���->AddComponent<ButtonComponent>(���info);
	����ư->SetWidth(�ΰ���3->GetSize().width);
	����ư->SetHeight(�ΰ���3->GetSize().height);

	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	����ư->BitmapPush("����ư",  �ΰ���3);
	����ư->SetCurrentBitmap("����ư");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto ��񸮽��� = ���->AddComponent<MouseListenerComponent>(
		[����ư](const MSG& msg) {
			����ư->Worked(msg);
		}
	);

	����ư->SetOnClickCallback([this]() {
		std::cout << "����ư" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("���", std::move(���));



	/////////////////////
	/////////////////////
	/////////////////////
	// [4] �������ͽ�

	// 1. �̹��� ���� ����
	auto �ΰ���4 = ResourceManager::Get().GetTexture("�ΰ���", "04");
	// 2. ������Ʈ �����
	auto �������ͽ� = std::make_unique<Object>();
	�������ͽ�->SetPosition(Vec2(64, 836));

	auto �������ͽ�info = �������ͽ�->GetRenderInfo();
	�������ͽ�info->SetBitmap(�ΰ���4.Get());
	// 3. ��ư ������Ʈ �����
	auto �������ͽ���ư = �������ͽ�->AddComponent<ButtonComponent>(�������ͽ�info);
	�������ͽ���ư->SetWidth(�ΰ���4->GetSize().width);
	�������ͽ���ư->SetHeight(�ΰ���4->GetSize().height);

	//  4. ��ư ��Ʈ�� ����
	�������ͽ���ư->BitmapPush("�������ͽ�", �ΰ���4);

	�������ͽ���ư->SetCurrentBitmap("�������ͽ�");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto �������ͽ������� = �������ͽ�->AddComponent<MouseListenerComponent>(
		[�������ͽ���ư](const MSG& msg) {
			�������ͽ���ư->Worked(msg);
		}
	);

	�������ͽ���ư->SetOnClickCallback([this]() {
		std::cout << "�������ͽ�" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("�������ͽ�", std::move(�������ͽ�));


	/////////////////////
	/////////////////////
	/////////////////////
	// [5] ��ȭ

	// 1. �̹��� ���� ����
	auto �ΰ���5 = ResourceManager::Get().GetTexture("�ΰ���", "05");
	// 2. ������Ʈ �����
	auto ��ȭ = std::make_unique<Object>();
	��ȭ->SetPosition(Vec2(589, 537));

	auto ��ȭinfo = ��ȭ->GetRenderInfo();
	��ȭinfo->SetBitmap(�ΰ���5.Get());
	// 3. ��ư ������Ʈ �����
	auto ��ȭ��ư = ��ȭ->AddComponent<ButtonComponent>(��ȭinfo);
	��ȭ��ư->SetWidth(�ΰ���5->GetSize().width);
	��ȭ��ư->SetHeight(�ΰ���5->GetSize().height);

	//  4. ��ư ��Ʈ�� ����
	��ȭ��ư->BitmapPush("��ȭ", �ΰ���5);

	��ȭ��ư->SetCurrentBitmap("��ȭ");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto ��ȭ������ = ��ȭ->AddComponent<MouseListenerComponent>(
		[��ȭ��ư](const MSG& msg) {
		 ��ȭ��ư->Worked(msg);
		}
	);

	��ȭ��ư->SetOnClickCallback([this]() {
		std::cout << "��ȭ" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("��ȭ", std::move(��ȭ));


	/////////////////////
	/////////////////////
	/////////////////////
	// [6] �ռ�

	// 1. �̹��� ���� ����
	auto �ΰ���6 = ResourceManager::Get().GetTexture("�ΰ���", "06");
	// 2. ������Ʈ �����
	auto �ռ� = std::make_unique<Object>();
	�ռ�->SetPosition(Vec2(889, 608));

	auto �ռ�info = �ռ�->GetRenderInfo();
	�ռ�info->SetBitmap(�ΰ���6.Get());
	// 3. ��ư ������Ʈ �����
	auto �ռ���ư = �ռ�->AddComponent<ButtonComponent>(�ռ�info);
	�ռ���ư->SetWidth(�ΰ���6->GetSize().width);
	�ռ���ư->SetHeight(�ΰ���6->GetSize().height);

	//  4. ��ư ��Ʈ�� ����
	�ռ���ư->BitmapPush("�ռ�", �ΰ���6);

	�ռ���ư->SetCurrentBitmap("�ռ�");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto �ռ������� = �ռ�->AddComponent<MouseListenerComponent>(
		[�ռ���ư](const MSG& msg) {
			�ռ���ư->Worked(msg);
}
	);

	�ռ���ư->SetOnClickCallback([this]() {
		std::cout << "�ռ�" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("�ռ�", std::move(�ռ�));


	/////////////////////
	/////////////////////
	/////////////////////
	// [7] ���� ��ư

	// 1. �̹��� ���� ����
	auto ���� = ResourceManager::Get().GetTexture("����");
	// 2. ������Ʈ �����
	auto ������ = std::make_unique<Object>();
	������->SetPosition(Vec2(41, 106));

	auto ����info = ������->GetRenderInfo();
	����info->SetBitmap(����.Get());
	// 3. ��ư ������Ʈ �����
	auto settingComp = ������->AddComponent<ButtonComponent>(�ڷ�info);
	settingComp->SetWidth(����->GetSize().width);
	settingComp->SetHeight(����->GetSize().height);

	//  4. ��ư ��Ʈ�� ����
	settingComp->BitmapPush("setting", ����);

	settingComp->SetCurrentBitmap("setting");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto set_mouseListener = ������->AddComponent<MouseListenerComponent>(
		[settingComp](const MSG& msg) {
			settingComp->Worked(msg);
		}
	);

	settingComp->SetOnClickCallback([this]() {
		std::cout << "���� ��ư Ŭ���� - ���� ��: " << typeid(*this).name() << std::endl;
		});

	/// 9
	m_gameObjects.emplace("����", std::move(������));

}
