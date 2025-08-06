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

		D2DRenderer::Get().DrawMessage(L"��Ŭ���� �� �����丮", 147.f, 254.f, 1300.f, 1000.f, D2D1::ColorF::LightPink);		

}


void Scene_Title::CreateObj()
{
	//////////////////////
	// Background 

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
	// �ؽ�Ʈ�� �̷��� ����� �ȵ�!
	// ������ �� ���� ���� �� �� ����

	//////////////////////
	// gameStartButton
	// ���ҽ� �Ŵ��� �ذ��ϱ�

	// 1. �̹��� ���� ����
	auto gameStartButton_texture_normal = ResourceManager::Get().GetTexture("����ȭ��", "2");
	auto gameStartButton_texture_hover = ResourceManager::Get().GetTexture("����ȭ��", "3");
	auto gameStartButton_texture_pressed = ResourceManager::Get().GetTexture("����ȭ��", "4");

	// 2. ������Ʈ �����
	auto gameStartButton = std::make_unique<Object>();
	gameStartButton->SetPosition(Vec2(147, 564));

	auto gstartInfo = gameStartButton->GetRenderInfo();
	gstartInfo->SetBitmap(gameStartButton_texture_normal.Get());

	// 3. ��ư ������Ʈ �����
	auto gameStartButton_buttonComp = gameStartButton->AddComponent<ButtonComponent>(gstartInfo);
	// 3.1.2 ������ ������
	gameStartButton_buttonComp->SetWidth(gameStartButton_texture_normal->GetSize().width);
	gameStartButton_buttonComp->SetHeight(gameStartButton_texture_normal->GetSize().height);

	// 4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	gameStartButton_buttonComp->BitmapPush("normal", gameStartButton_texture_normal);
	gameStartButton_buttonComp->BitmapPush("hover", gameStartButton_texture_hover);
	gameStartButton_buttonComp->BitmapPush("pressed", gameStartButton_texture_pressed);

	// *** �߿�: �ʱ� ��Ʈ�� ���� ***
	gameStartButton_buttonComp->SetCurrentBitmap("normal");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto gameStartButton_mouseListener = gameStartButton->AddComponent<MouseListenerComponent>(
		[gameStartButton_buttonComp](const MSG& msg) {
			gameStartButton_buttonComp->Worked(msg);
		}
	);

	gameStartButton_buttonComp->SetOnClickCallback([this]() {
		SceneManager::Get().ChangeScene("OutGame");
		std::cout << "SceneManager::Get().ChangeScene(\"OutGame\");" << std::endl;
		});

	m_gameObjects.emplace("gameStartButton", std::move(gameStartButton));

	//////////////////////
	// gameSettingButton

		// ���ҽ� �Ŵ���
	auto gameSettingButton_texture_normal = ResourceManager::Get().GetTexture("����ȭ��", "3");
	auto gameSettingButton_texture_hover = ResourceManager::Get().GetTexture("����ȭ��", "4");
	auto gameSettingButton_texture_pressed = ResourceManager::Get().GetTexture("����ȭ��", "2");

	auto
		gameSettingButton = std::make_unique<Object>();
	gameSettingButton->SetPosition(Vec2(147, 697));

	auto gsettingInfo = gameSettingButton->GetRenderInfo();
	gsettingInfo->SetBitmap(gameSettingButton_texture_normal.Get());

	auto gameSettingButton_buttonComp = gameSettingButton->AddComponent<ButtonComponent>(gsettingInfo);
	// 3.1.2 ������ ������
	gameSettingButton_buttonComp->SetWidth(gameStartButton_texture_normal->GetSize().width);
	gameSettingButton_buttonComp->SetHeight(gameStartButton_texture_normal->GetSize().height);
	auto gameSettingButton_mouseListener = gameSettingButton->AddComponent<MouseListenerComponent>(
		// ���⼭ ��� �Է� ó�� ����?
		[gameSettingButton_buttonComp](const MSG& msg) {
			gameSettingButton_buttonComp->Worked(msg);
		}
	);

	// ��ư ����
	gameSettingButton_buttonComp->BitmapPush("normal", gameSettingButton_texture_normal);
	gameSettingButton_buttonComp->BitmapPush("hover", gameSettingButton_texture_hover);
	gameSettingButton_buttonComp->BitmapPush("pressed", gameSettingButton_texture_pressed);
	gameSettingButton_buttonComp->SetOnClickCallback([this]() {
		//SceneManager::Get().ChangeScene("OutGame");
		std::cout << "The button is pressed." << std::endl;
		});

	m_gameObjects.emplace("gameSettingButton", std::move(gameSettingButton));

	//////////////////////
	// creditButton

	// ���ҽ� �Ŵ���
	auto creditButton_texture_normal = ResourceManager::Get().GetTexture("����ȭ��", "4");
	auto creditButton_texture_hover = ResourceManager::Get().GetTexture("����ȭ��", "2");
	auto creditButton_texture_pressed = ResourceManager::Get().GetTexture("����ȭ��", "3");

	auto
		creditButton = std::make_unique<Object>();
	creditButton->SetPosition(Vec2(147, 830));

	auto creditInfo = creditButton->GetRenderInfo();
	creditInfo->SetBitmap(creditButton_texture_normal.Get());

	auto creditButton_buttonComp = creditButton->AddComponent<ButtonComponent>(creditInfo);
	// 3.1.2 ������ ������
	creditButton_buttonComp->SetWidth(gameStartButton_texture_normal->GetSize().width);
	creditButton_buttonComp->SetHeight(gameStartButton_texture_normal->GetSize().height);
	auto creditButton_mouseListener = creditButton->AddComponent<MouseListenerComponent>(
		// ���⼭ ��� �Է� ó�� ����?
		[creditButton_buttonComp](const MSG& msg) {
			creditButton_buttonComp->Worked(msg);
		}
	);

	// ��ư ����
	creditButton_buttonComp->BitmapPush("normal", creditButton_texture_normal);
	creditButton_buttonComp->BitmapPush("hover", creditButton_texture_hover);
	creditButton_buttonComp->BitmapPush("pressed", creditButton_texture_pressed);
	creditButton_buttonComp->SetOnClickCallback([this]() {
		//SceneManager::Get().ChangeScene("Title");
		std::cout << "The button is pressed." << std::endl;
		});

	m_gameObjects.emplace("creditButton", std::move(creditButton));

}
