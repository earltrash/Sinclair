#include "Scene_Title.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "UIManager.h"

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

	for (const auto& [Name, obj] : m_gameObjects)
	{
		UIManager::Get().AddSceneObject(obj);
	}
}

void Scene_Title::Enter()
{
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
	//m_gameObjects.clear();
	////SceneAssets.clear();
	//dirty = false;
	//UIManager::Get().ClearSceneObjects();


	if (!dirty) return;

	// UIManager���� �� ��ü���� ���� ����
	UIManager::Get().ClearSceneObjects();

	// ��� ��� �� ���� ��ü�� ����
	m_gameObjects.clear();

	dirty = false;
}

// �� ��ȯ ���� ó���� ���� ��_���Ĵٵ忡�� ������Ʈ �ϰ� ó��.
void Scene_Title::Update()
{
	// �� ��ȯ ���� ó��
	if (m_isTransitioning && !m_nextScene.empty())
	{
		m_currentDelay += 0.016f; // �뷫���� ������ �ð� (60FPS ����)
		if (m_currentDelay >= m_transitionDelay)
		{
			// ���� �� ��ȯ ����
			SceneManager::Get().ChangeScene(m_nextScene);
			m_isTransitioning = false;
			m_nextScene = "";
			m_currentDelay = 0.0f;
		}
	}
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

	if (isCredit)
	{
		// [��ȹ �ؽ�Ʈ] 48pt �ؽ�Ʈ
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
		D2DRenderer::Get().DrawMessage(L"��ȹ", 649.23f, 354.14f, 649.23f + 200, 354.14f + 200, D2D1::ColorF(0xffe6d8));

		// [���α׷��� �ؽ�Ʈ] 48pt �ؽ�Ʈ
		D2DRenderer::Get().DrawMessage(L"���α׷���", 1043.14f, 354.14f, 1043.14f + 200, 354.14f + 600, D2D1::ColorF(0xffe6d8));
		
		// [��ȹ�� �̸�] 30pt �ؽ�Ʈ
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 30.0f);
		D2DRenderer::Get().DrawMessage(L"������\n������\n���ֿ�", 656.65f, 536.94f, 656.65f + 200, 536.94f + 700, D2D1::ColorF(0xffe6d8));

		// [���α׷��� �̸�] 30pt �ؽ�Ʈ
		D2DRenderer::Get().DrawMessage(L"��ΰ�\n�����\n��ȿ��\n���漮", 1120.65f, 512.49f, 1120.65f + 200, 512.49f + 600, D2D1::ColorF(0xffe6d8));
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
		SafeChangeScene("OutGame");
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
		isCredit = !isCredit;
		SafeChangeScene("Title");
		});

	m_gameObjects.emplace("creditButton", std::move(creditButton));
	
	if(isCredit)
	{
		//////////////////////
		//////////////////////
		//////////////////////
		// [6] [ũ����_01] â ������Ʈ
		// 1. �̹��� ���� ����
		auto credit01Texture = ResourceManager::Get().GetTexture("ũ����", "01");
		// 2. ������Ʈ �����
		auto Credit01 = std::make_unique<Object>();
		Credit01->SetPosition(Vec2(431, 270));
		auto credit01Info = Credit01->GetRenderInfo();
		credit01Info->SetBitmap(credit01Texture.Get());
		// 3. ��� ������Ʈ �����
		auto credit01Comp = Credit01->AddComponent<BackgroundComponent>(credit01Info);
		// 3.1.1 ������ ����
		credit01Comp->SetWidth(1059.f);
		credit01Comp->SetHeight(540.f);
		credit01Comp->BitmapPush("ũ����_01", credit01Texture);
		// 9. ���� ������Ʈ�鿡 ����ֱ�
		m_gameObjects.emplace("ũ����_01", std::move(Credit01));

		//////////////////////
		//////////////////////
		//////////////////////
		// [7] [ũ����_03] �ؽ�Ʈ ��� ������Ʈ
		// 1. �̹��� ���� ����
		auto credit03Texture = ResourceManager::Get().GetTexture("ũ����", "03");
		// 2. ������Ʈ �����
		auto Credit03 = std::make_unique<Object>();
		Credit03->SetPosition(Vec2(535, 467));
		auto credit03Info = Credit03->GetRenderInfo();
		credit03Info->SetBitmap(credit03Texture.Get());
		// 3. ��� ������Ʈ �����
		auto credit03Comp = Credit03->AddComponent<BackgroundComponent>(credit03Info);
		// 3.1.1 ������ ����
		credit03Comp->SetWidth(335.f);
		credit03Comp->SetHeight(283.f);
		credit03Comp->BitmapPush("ũ����_03", credit03Texture);
		// 9. ���� ������Ʈ�鿡 ����ֱ�
		m_gameObjects.emplace("ũ����_03", std::move(Credit03));


		//////////////////////
		//////////////////////
		//////////////////////
		// [8] [ũ����_06] �ؽ�Ʈ ��� ������Ʈ
		// 1. �̹��� ���� ����
		auto credit06Texture = ResourceManager::Get().GetTexture("ũ����", "06");
		// 2. ������Ʈ �����
		auto Credit06 = std::make_unique<Object>();
		Credit06->SetPosition(Vec2(999, 467));
		auto credit06Info = Credit06->GetRenderInfo();
		credit06Info->SetBitmap(credit06Texture.Get());
		// 3. ��� ������Ʈ �����
		auto credit06Comp = Credit06->AddComponent<BackgroundComponent>(credit06Info);
		// 3.1.1 ������ ����
		credit06Comp->SetWidth(335.f);
		credit06Comp->SetHeight(283.f);
		credit06Comp->BitmapPush("ũ����_06", credit06Texture);
		// 9. ���� ������Ʈ�鿡 ����ֱ�
		m_gameObjects.emplace("ũ����_06", std::move(Credit06));


		/////////////////////
		/////////////////////
		/////////////////////
		// [9] �ڷΰ��� ��ư

		// 1. �̹��� ���� ����
		auto �ڷΰ��� = ResourceManager::Get().GetTexture("�ڷΰ���");
		// 2. ������Ʈ �����
		auto �ڷ� = std::make_unique<Object>();
		�ڷ�->SetPosition(Vec2(1393, 307));

		auto �ڷ�info = �ڷ�->GetRenderInfo();
		�ڷ�info->SetBitmap(�ڷΰ���.Get());
		// 3. ��ư ������Ʈ �����
		auto backComp = �ڷ�->AddComponent<ButtonComponent>(�ڷ�info);
		backComp->SetWidth(35);
		backComp->SetHeight(35);

		//  4. ��ư ��Ʈ�� ����
		// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
		backComp->BitmapPush("back", �ڷΰ���);

		backComp->SetCurrentBitmap("back");

		// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
		auto Back_mouseListener = �ڷ�->AddComponent<MouseListenerComponent>(
			[backComp](const MSG& msg) {
				backComp->Worked(msg);
			}
		);

		backComp->SetOnClickCallback([this]() {
			std::cout << "�ݱ� ��ư Ŭ���� - ���� ��: " << typeid(*this).name() << std::endl;
			isCredit = !isCredit;
			SafeChangeScene("Title");
			});

		/// 9
		m_gameObjects.emplace("ũ����_�ڷΰ���", std::move(�ڷ�));
	}

}