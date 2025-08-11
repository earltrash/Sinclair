#include "Scene_Title.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "UIManager.h"
#include "SliderHandleComponent.h"
#include "GameManager.h"
#include "EffectComponent.h"
#include "PlayComponent.h"

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
	for (auto& obj : m_gameObjects)
	{
		obj.second->Update();
	}
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
	for (auto& obj : m_gameObjects)
	{
		obj.second->FixedUpdate(delta);
	}
}



void Scene_Title::Render()
{
	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetRenderInfo()->GetRenderInfo());
		//D2DRenderer::Get().DrawRectangle(obj->GetRenderInfo()->GetRenderInfo().destRect, D2D1::ColorF::Red);
	}

	switch (m_window)
	{
	case CREDIT:
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
		break;
	}
	case SETTING:
	{
		// [���� �ؽ�Ʈ] 48pt �ؽ�Ʈ
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
		D2DRenderer::Get().DrawMessage(L"����", 528.77f, 343.0f, 528.77f + 200, 343.0f + 200, D2D1::ColorF(0xffe6d8));

		// [����� �ؽ�Ʈ] 30pt �ؽ�Ʈ
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 30.0f);
		D2DRenderer::Get().DrawMessage(L"�����", 550.18f, 498.77f, 550.18f + 200, 498.77f + 200, D2D1::ColorF(0xffe6d8));

		// [ȿ���� �ؽ�Ʈ] 30pt �ؽ�Ʈ
		D2DRenderer::Get().DrawMessage(L"ȿ����", 550.18f, 641.77f, 550.18f + 200, 641.77f + 200, D2D1::ColorF(0xffe6d8));


		break;
	}

	case NORMAL:
	default:
		break;
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
	//auto bgComp = Background->AddComponent<BackgroundComponent>(bgInfo);
	// 3.1.1 ������ �ٸ��� 
	//bgComp->SetWidth(1920.f); bgComp->SetHeight(1080.f);
	//bgComp->BitmapPush("Background", gameStartBackground);
	//bgComp->SetCurrentBitmap("Background");

	// ���� ȿ��
	auto bg_contrast = Background->AddComponent<DynamicContrast_Effect>(bgInfo, -0.2f, 0.6f, 0.0005f, gameStartBackground.Get());
	// 9. ���� ������Ʈ�鿡 ����ֱ�
	m_gameObjects.emplace("Background", std::move(Background));

	//////////////////////
	//////////////////////
	//////////////////////
	//	airParticle

	// 1. �̹��� �ε�
	auto dustBM = ResourceManager::Get().GetTexture("airParticle");
	// 2. ������Ʈ ����
	auto dust1 = std::make_unique<Object>();
	// 3. �⺻ ����
	auto d1_info = dust1->GetRenderInfo();
	d1_info->SetBitmap(dustBM.Get());
	// 4. ������Ʈ ����
	auto d1_blur = dust1->AddComponent<GaussianBlur_Effect>(d1_info, 1.f, dustBM.Get());
	auto d1_updown = dust1->AddComponent<UpDown_Effect>(d1_info, dust1->GetTransform(), 10.f, 0.03f, 0.03f);
	auto d1_slide = dust1->AddComponent<Slide_Effect>(d1_info, dust1->GetTransform(), 0.07f, 1920.f, 1920.f, true);
	// 5. ���� ������Ʈ ���� map�� �߰�
	m_gameObjects.emplace("b_airDust1", std::move(dust1));

	// 2. ������Ʈ ����
	auto dust2 = std::make_unique<Object>();
	// 3. �⺻ ����
	auto d2_info = dust2->GetRenderInfo();
	d2_info->SetBitmap(dustBM.Get());
	dust2->GetTransform().SetPosition({ 1920.f, 0.f });
	// 4. ������Ʈ ����
	auto d2_blur = dust2->AddComponent<GaussianBlur_Effect>(d2_info, 3.f, dustBM.Get());
	auto d2_updown = dust2->AddComponent<UpDown_Effect>(d2_info, dust2->GetTransform(), 10.f, 0.03f, 0.03f);
	auto d2_slide = dust2->AddComponent<Slide_Effect>(d2_info, dust2->GetTransform(), 0.08f, 1920.f, 1920.f, true);
	// 5. ���� ������Ʈ ���� map�� �߰�
	m_gameObjects.emplace("b_airDust2", std::move(dust2));

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
	nameComp->SetCurrentBitmap("gameName");
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
			gameStartButton_buttonComp->CheckCollision(msg);
			gameStartButton_buttonComp->Worked(msg);
		});

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
			gameSettingButton_buttonComp->CheckCollision(msg);
			gameSettingButton_buttonComp->Worked(msg);
		});

	// ��ư ����
	gameSettingButton_buttonComp->BitmapPush("setting", settingButtonTexture);
	gameSettingButton_buttonComp->SetCurrentBitmap("setting");

	gameSettingButton_buttonComp->SetOnClickCallback([this]() {
		if (m_window != SETTING)
		{
			m_window = SETTING;
		}
		else
		{
			m_window = NORMAL;
		}
		SafeChangeScene("Title");
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
			creditButton_buttonComp->CheckCollision(msg);
			creditButton_buttonComp->Worked(msg);
		});

	// ��ư ����
	creditButton_buttonComp->BitmapPush("credit", creditButtonTexture);
	creditButton_buttonComp->SetCurrentBitmap("credit");

	creditButton_buttonComp->SetOnClickCallback([this]() {
		std::cout << "The ũ���� button is pressed." << std::endl;
		if(m_window != CREDIT)
		{
			m_window = CREDIT;
		}
		else
		{
			m_window = NORMAL;
		}
		SafeChangeScene("Title");
		});

	m_gameObjects.emplace("creditButton", std::move(creditButton));
	
	switch (m_window)
	{
	case CREDIT:
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
		credit01Comp->SetCurrentBitmap("ũ����_01");
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
		credit03Comp->SetCurrentBitmap("ũ����_03");
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
		credit06Comp->SetCurrentBitmap("ũ����_06");
		// 9. ���� ������Ʈ�鿡 ����ֱ�
		m_gameObjects.emplace("ũ����_06", std::move(Credit06));


		/////////////////////
		/////////////////////
		/////////////////////
		// [9] �ڷΰ��� ��ư

		// 1. �̹��� ���� ����
		auto �ڷΰ��� = ResourceManager::Get().GetTexture("�ݱ�");
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
				backComp->CheckCollision(msg);
				backComp->Worked(msg);
			}
		);

		backComp->SetOnClickCallback([this]() {
			std::cout << "�ݱ� ��ư Ŭ���� - ���� ��: " << typeid(*this).name() << std::endl;
			m_window = NORMAL;
			SafeChangeScene("Title");
			});

		/// 9
		m_gameObjects.emplace("ũ����_�ڷΰ���", std::move(�ڷ�));
	}
	break;
	case SETTING:
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
		credit01Comp->SetCurrentBitmap("ũ����_01");
		// 9. ���� ������Ʈ�鿡 ����ֱ�
		m_gameObjects.emplace("ũ����_01", std::move(Credit01));

		//////////////////////
		//////////////////////
		//////////////////////
		// [7] ����������̴�_��   
		// [8] ����������̴�_�ڵ� 
		// [9] ȿ���������̴�_��   
		// [10] ȿ���������̴�_�ڵ� 

		// 1. �̹��� ���ҽ� ��������
		auto barImg = ResourceManager::Get().GetTexture("����", "bar");
		auto handleImg = ResourceManager::Get().GetTexture("����", "handle");
		
		// 2. �����̴� ������Ʈ �����
		auto bgmBar    = std::make_unique<Object>();
		auto bgmHandle  = std::make_unique<Object>();
		auto sfxBar    = std::make_unique<Object>();
		auto sfxHandle  = std::make_unique<Object>();


		bgmBar->SetPosition(Vec2(729, 498));
		auto bgmBarInfo = bgmBar->GetRenderInfo();
		bgmBarInfo->SetBitmap(barImg.Get());

		bgmHandle->SetPosition(Vec2(1347, 495));
		auto bgmHandleInfo = bgmHandle->GetRenderInfo();
		bgmHandleInfo->SetBitmap(handleImg.Get());

		sfxBar->SetPosition(Vec2(729, 641));
		auto sfxBarInfo = sfxBar->GetRenderInfo();
		sfxBarInfo->SetBitmap(barImg.Get());

		sfxHandle->SetPosition(Vec2(1347, 638));
		auto sfxHandleInfo = sfxHandle->GetRenderInfo();
		sfxHandleInfo->SetBitmap(handleImg.Get());


		// 3. ���/ �����̴��ڵ� ������Ʈ �����
		auto bgmBarComp = bgmBar->AddComponent<BackgroundComponent>(bgmBarInfo);
		auto bgmHandleComp = bgmHandle->AddComponent<SliderHandleComponent>(bgmHandleInfo, 729.0f, 1347.0f, GameManager::Get().bgmValue);
		auto sfxBarComp = sfxBar->AddComponent<BackgroundComponent>(sfxBarInfo);
		auto sfxHandleComp = sfxHandle->AddComponent<SliderHandleComponent>(sfxHandleInfo, 729.0f, 1347.0f, GameManager::Get().sfxValue);

		// 3.1.1 ������ ���� 
		bgmBarComp->SetWidth(630.f);
		bgmBarComp->SetHeight(32.f);
		bgmBarComp->BitmapPush("barImg", barImg);
		bgmHandleComp->SetWidth(19.f);
		bgmHandleComp->SetHeight(38.f);
		
		sfxBarComp->SetWidth(630.f);
		sfxBarComp->SetHeight(32.f);
		sfxBarComp->BitmapPush("barImg", barImg);
		sfxHandleComp->SetWidth(19.f);
		sfxHandleComp->SetHeight(38.f);

		// 4. �� ���� �ݹ� ����
		bgmHandleComp->SetOnValueChanged([](float value) {
			std::cout << "�����̴� ��: " << value << " (0.0~1.0)" << std::endl;
			// ��: ���� ����
			// SoundManager::Get().SetMasterVolume(value);
			});

		sfxHandleComp->SetOnValueChanged([](float value) {
			std::cout << "�����̴� ��: " << value << " (0.0~1.0)" << std::endl;
			// ��: ���� ����
			// SoundManager::Get().SetMasterVolume(value);
			});


		// 5. ���콺 ������ ������Ʈ
		auto bgmHandle_mouseListener = bgmHandle->AddComponent<MouseListenerComponent>(
			[bgmHandleComp](const MSG& msg) {
				bgmHandleComp->HandleMouse(msg);
			}
		);

		auto sfxHandle_mouseListener = sfxHandle->AddComponent<MouseListenerComponent>(
			[sfxHandleComp](const MSG& msg) {
				sfxHandleComp->HandleMouse(msg);
			}
		);

		// 6. �ʱⰪ ���� -> �ӽ÷� ���� �Ŵ��� �ʿ��� �ްԲ�, ����. 
		// ���� �Ŵ������� �޾ƿ��� �ɷ� �ٲ�� ��.
		
		bgmHandleComp->SetValue(GameManager::Get().bgmValue);
		sfxHandleComp->SetValue(GameManager::Get().sfxValue);



		// 6. ���� ������Ʈ�� ���
		m_gameObjects.emplace("ũũ_bgm��", std::move(bgmBar  ));
		m_gameObjects.emplace("ũũ_bgm�ڵ�", std::move(bgmHandle));
		m_gameObjects.emplace("ũũ_sfx��", std::move(sfxBar  ));
		m_gameObjects.emplace("ũũ_sfx�ڵ�", std::move(sfxHandle));




		/////////////////////
		/////////////////////
		/////////////////////
		// [11] �ڷΰ��� ��ư

		// 1. �̹��� ���� ����
		auto �ڷΰ��� = ResourceManager::Get().GetTexture("�ݱ�");
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
				backComp->CheckCollision(msg);
				backComp->Worked(msg);
			}
		);

		backComp->SetOnClickCallback([this]() {
			std::cout << "�ݱ� ��ư Ŭ���� - ���� ��: " << typeid(*this).name() << std::endl;
			m_window = NORMAL;
			SafeChangeScene("Title");
			});

		/// 9
		m_gameObjects.emplace("ũ����_�ڷΰ���", std::move(�ڷ�));

	}
	break;
	case NORMAL:
	default:
		break;
	}

}