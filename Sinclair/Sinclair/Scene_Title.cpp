#include "Scene_Title.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"

Scene_Title::Scene_Title(string name)
{
		m_name = name;
}

Scene_Title::~Scene_Title()
{
}

void Scene_Title::Initalize()
{
	if (dirty) return; // �̹� �ʱ�ȭ�Ǿ����� ��ŵ
		
	CreateObj(); // ������Ʈ ���� (�� ����)
	dirty = true;
}

void Scene_Title::Enter()
{
	Initalize();
	// �� ���� �ø��� ������ �͵�
	// ������Ʈ ����, ������Ʈ ���� �ʱ�ȭ, ���� ��� ��
	// ResetObjectStates();
	// PlayBGM();
	Initalize();
}

void Scene_Title::Exit()
{
	// �� ���� �ø��� ������ �͵�  
	// ���� ����, ���� ���� ��
	//StopBGM();
	//PauseAnimations();
	Clean();
}

void Scene_Title::Clean()
{
		m_gameObjects.clear();
	//SceneAssets.clear();
	dirty = false;
}

void Scene_Title::Update()
{

}

void Scene_Title::LogicUpdate(float delta)
{

}



void Scene_Title::Render()
{
	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetRenderInfo()->GetRenderInfo());
}

}


void Scene_Title::CreateObj()
{
	//////////////////////
	//////////////////////
	//////////////////////
	// [1] Background 

	// 1. �̹��� ���� ����
	auto gameStartBackground = ResourceManager::Get().GetTexture("����ȭ��");
	// 2. ������Ʈ �����
	auto Background = std::make_unique<Object>();
	Background->SetPosition(Vec2(0, 0));

	auto bgInfo = Background->GetRenderInfo();
	bgInfo->SetBitmap(gameStartBackground.Get());

	// 3. ��� ������Ʈ �����
	auto bgComp = Background->AddComponent<BackgroundComponent>(bgInfo);
	// 3.1.1 ������ �ٸ��� 
	bgComp->SetWidth(1920.f); bgComp->SetHeight(1080.f);
	bgComp->BitmapPush("Background", gameStartBackground);
	// 9. ���� ������Ʈ�鿡 ����ֱ�
	m_gameObjects.emplace("Background", std::move(Background));


	//////////////////////
	//////////////////////
	//////////////////////
	// [2] ���� �̸�

	// 1. �̹��� ���� ����
	auto gameNameBG = ResourceManager::Get().GetTexture("����ȭ��", "01");
	// 2. ������Ʈ �����
	auto gameName = std::make_unique<Object>();
	gameName->SetPosition(Vec2(100.15f, 239.66f));

	auto nameInfo = gameName->GetRenderInfo();
	nameInfo->SetBitmap(gameNameBG.Get());

	// 3. ��� ������Ʈ �����
	auto nameComp = gameName->AddComponent<BackgroundComponent>(nameInfo);
	nameComp->SetWidth (gameNameBG->GetSize().width);
	nameComp->SetHeight(gameNameBG->GetSize().height);
	nameComp->BitmapPush("gameName", gameNameBG);
	// 9. ���� ������Ʈ�鿡 ����ֱ�
	m_gameObjects.emplace("gameName", std::move(gameName));



	//////////////////////
	//////////////////////
	//////////////////////
	// [3] startButton

	// 1. �̹��� ���� ����
	auto startButtonTexture = ResourceManager::Get().GetTexture("����ȭ��", "02");

	// 2. ������Ʈ �����
	auto startButton = std::make_unique<Object>();
	startButton->SetPosition(Vec2(147, 564));

	auto startInfo = startButton->GetRenderInfo();
	startInfo->SetBitmap(startButtonTexture.Get());

	// 3. ��ư ������Ʈ �����
	auto gameStartButton_buttonComp = startButton->AddComponent<ButtonComponent>(startInfo);
	// 3.1.2 ������ ������
	gameStartButton_buttonComp->SetWidth(startButtonTexture->GetSize().width);
	gameStartButton_buttonComp->SetHeight(startButtonTexture->GetSize().height);

	// 4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	gameStartButton_buttonComp->BitmapPush("start", startButtonTexture);

	// *** �߿�: �ʱ� ��Ʈ�� ���� ***
	gameStartButton_buttonComp->SetCurrentBitmap("start");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto gameStartButton_mouseListener = startButton->AddComponent<MouseListenerComponent>(
		[gameStartButton_buttonComp](const MSG& msg) {
			gameStartButton_buttonComp->Worked(msg);
		}
						);

	gameStartButton_buttonComp->SetOnClickCallback([this]() {
		SceneManager::Get().ChangeScene("OutGame");
		std::cout << "SceneManager::Get().ChangeScene(\"OutGame\");" << std::endl;
		});

	m_gameObjects.emplace("startButton", std::move(startButton));


	////////////////////// 
	////////////////////// 
	////////////////////// 
	// [4] settingButton

	// ���ҽ� �Ŵ���
	auto settingButtonTexture = ResourceManager::Get().GetTexture("����ȭ��", "03");

	auto
		settingButton = std::make_unique<Object>();
	settingButton->SetPosition(Vec2(147, 697));

	auto settingInfo = settingButton->GetRenderInfo();
	settingInfo->SetBitmap(settingButtonTexture.Get());

	auto gameSettingButton_buttonComp = settingButton->AddComponent<ButtonComponent>(settingInfo);
	// 3.1.2 ������ ������
	gameSettingButton_buttonComp->SetWidth(startButtonTexture->GetSize().width);
	gameSettingButton_buttonComp->SetHeight(startButtonTexture->GetSize().height);
	auto gameSettingButton_mouseListener = settingButton->AddComponent<MouseListenerComponent>(
		// ���⼭ ��� �Է� ó�� ����?
		[gameSettingButton_buttonComp](const MSG& msg) {
			gameSettingButton_buttonComp->Worked(msg);
				}
	);

	// ��ư ����
	gameSettingButton_buttonComp->BitmapPush("setting", settingButtonTexture);
	gameSettingButton_buttonComp->SetCurrentBitmap("setting");

	gameSettingButton_buttonComp->SetOnClickCallback([this]() {
		std::cout << "The ���� button is pressed." << std::endl;
		});

	m_gameObjects.emplace("settingButton", std::move(settingButton));

	//////////////////////
	//////////////////////
	//////////////////////
	// [5] creditButton

	// ���ҽ� �Ŵ���
	auto creditButtonTexture = ResourceManager::Get().GetTexture("����ȭ��", "04");

	auto
		creditButton = std::make_unique<Object>();
	creditButton->SetPosition(Vec2(147, 830));

	auto creditInfo = creditButton->GetRenderInfo();
	creditInfo->SetBitmap(creditButtonTexture.Get());

	auto creditButton_buttonComp = creditButton->AddComponent<ButtonComponent>(creditInfo);
	// 3.1
	creditButton_buttonComp->SetWidth(  310 /*startButtonTexture->GetSize().width */);
	creditButton_buttonComp->SetHeight(  75/*startButtonTexture->GetSize().height*/);


	auto creditButton_mouseListener = creditButton->AddComponent<MouseListenerComponent>(
		[creditButton_buttonComp](const MSG& msg) {
			creditButton_buttonComp->Worked(msg);
				}
	);

	// ��ư ����
	creditButton_buttonComp->BitmapPush("credit", creditButtonTexture);
	creditButton_buttonComp->SetCurrentBitmap("credit");

	creditButton_buttonComp->SetOnClickCallback([this]() {
		std::cout << "The ũ���� button is pressed." << std::endl;
		});

	m_gameObjects.emplace("creditButton", std::move(creditButton));

}