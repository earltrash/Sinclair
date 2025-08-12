#include "Scene_Outgame.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "Renderer.h"
#include "UIManager.h"
#include "SliderHandleComponent.h"

Scene_Outgame::Scene_Outgame(string name)
{
		m_name = name;
}

Scene_Outgame::~Scene_Outgame()
{
	Clean();
}

void Scene_Outgame::Initalize()
{
	if (dirty) return;  

	CreateObj();  
	for (const auto& [Name, obj] : m_gameObjects)
	{
		UIManager::Get().AddSceneObject(obj);
	}

	dirty = true;
}

void Scene_Outgame::Clean()
{
	if (!dirty) return;

	m_gameObjects.clear();
	UIManager::Get().ClearSceneObjects();
	
	dirty = false;
}

void Scene_Outgame::Enter()
{
	Initalize();
	SetupCharacterAndBackground();


	 if (GameManager::Get().isFirst) {
		 m_state = State::FIRST_ENTER;
		 GameManager::Get().isFirst = false;
	 } 
	 else {
		 m_state = State::CHOICE_MENU;
	 }
	 
	 characterName = L"��Ŭ���� " + std::to_wstring(GameManager::Get().curGen) + L"��";

	// �켱 �ʱ� ���·� ����
	ChangeState(m_state);
}

void Scene_Outgame::Exit()
{

		Clean();
}

void Scene_Outgame::Update()
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

void Scene_Outgame::LogicUpdate(float delta)
{

}


void Scene_Outgame::Render()
{

	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetRenderInfo()->GetRenderInfo());
	}


	D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 36.0f);
	D2DRenderer::Get().DrawMessage(characterName.c_str(), 481.74f, 828.38f, 1300.f, 1000.f, D2D1::ColorF::White);
	D2DRenderer::Get().CreateWriteResource(L"���� ����� Regular", DWRITE_FONT_WEIGHT_NORMAL, 24.0f);
	// ���¿� ���� ���� �ؽ�Ʈ ������
	if (!curText.empty())
	{
		if (m_state == CHOICE_MENU)
		{
			ButtonComponent::ButtonState changoState = m_gameObjects["Chango"]->GetComponent<ButtonComponent>()->GetState();
			ButtonComponent::ButtonState moheomState = m_gameObjects["Moheom"]->GetComponent<ButtonComponent>()->GetState();

			// ���� �޴��� ���� �� �ɼ��� ���� �׸���
			std::wstring wOption1 = L"> â��� �̵��Ѵ�.";
			std::wstring wOption2 = L"> ������ ������.";

			// ù ��° �ɼ� ���� ����
			D2D1::ColorF color1 = D2D1::ColorF::White;
			switch (changoState)
			{
			case ButtonComponent::ButtonState::Normal:
				color1 = D2D1::ColorF::White;
				break;
			case ButtonComponent::ButtonState::Hover:
				color1 = D2D1::ColorF::Yellow;
				break;
			case ButtonComponent::ButtonState::Pressed:
				color1 = D2D1::ColorF::Red;
				break;
			}

			// �� ��° �ɼ� ���� ����
			D2D1::ColorF color2 = D2D1::ColorF::White;
			switch (moheomState)
			{
			case ButtonComponent::ButtonState::Normal:
				color2 = D2D1::ColorF::White;
				break;
			case ButtonComponent::ButtonState::Hover:
				color2 = D2D1::ColorF::Yellow;
				break;
			case ButtonComponent::ButtonState::Pressed:
				color2 = D2D1::ColorF::Red;
				break;
			}
			// ���� �ٸ� ��ġ�� �ٸ� ������ �׸���
			D2DRenderer::Get().DrawMessage(wOption1.c_str(), 768.21f, 828.38f, 1300.f, 50.f, color1);
			D2DRenderer::Get().DrawMessage(wOption2.c_str(), 768.21f, 878.38f, 1300.f, 50.f, color2);
		}
		else
		{
			// �Ϲ� �ؽ�Ʈ�� ������ �����ϰ�
			std::wstring wCurText = StrToWstr(curText);
			D2DRenderer::Get().DrawMessage(wCurText.c_str(), 768.21f, 828.38f, 1300.f, 1000.f, D2D1::ColorF::White);
		}
	}


	if (isSETTING)
	{
		// [���� �ؽ�Ʈ] 48pt �ؽ�Ʈ
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
		D2DRenderer::Get().DrawMessage(L"����", 528.77f, 343.0f, 528.77f + 200, 343.0f + 200, D2D1::ColorF(0xffe6d8));

		// [����� �ؽ�Ʈ] 30pt �ؽ�Ʈ
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 30.0f);
		D2DRenderer::Get().DrawMessage(L"�����", 550.18f, 498.77f, 550.18f + 200, 498.77f + 200, D2D1::ColorF(0xffe6d8));

		// [ȿ���� �ؽ�Ʈ] 30pt �ؽ�Ʈ
		D2DRenderer::Get().DrawMessage(L"ȿ����", 550.18f, 641.77f, 550.18f + 200, 641.77f + 200, D2D1::ColorF(0xffe6d8));

	}
}

void Scene_Outgame::CreateObj()
{
	//////////////////////
	// 1.���� �̹��� ���� ����
	// �ƿ����ӿ��� �̹��� ��Ȱ��ȭ�� ���ؼ� ����
	auto transparentImg = ResourceManager::Get().GetTexture("transparent");


	//////////////////////
	//////////////////////
	//////////////////////
	// [1] Background 

	// 1. �̹��� ���� ����
	auto outGameBackground = ResourceManager::Get().GetTexture("�ƿ�����");
	// 2. ������Ʈ �����
	auto Background = std::make_unique<Object>();
	Background->SetPosition(Vec2(0, 0));

	// 3.0. ���� ���� ������Ʈ
	auto bgInfo = Background->GetRenderInfo();
	bgInfo->SetBitmap(outGameBackground.Get());

	// 3. ��� ������Ʈ �����
	auto bgComp = Background->AddComponent<BackgroundComponent>(bgInfo);
	// 3.1.1 ������ �ٸ��� 
	bgComp->SetWidth(1920); bgComp->SetHeight(1080);
	bgComp->BitmapPush("Background", outGameBackground);
	// 9. ���� ������Ʈ�鿡 ����ֱ�
	m_gameObjects.emplace("Background", std::move(Background));



	//////////////////////
	//////////////////////
	//////////////////////
	// [2] ĳ���� �̹��� 
	// ��� ������Ʈ��� ��� ������ -> �̹��� ������Ʈ��� �ٲ�� ��. 
	// ���� ����ٰ� ���̴�? �־�� �ȴٰ� ������. 
	// ĳ���Ϳ� �÷� ������ �ٲ�����. -> �׷� ��ư ������Ʈ�� �ٲ�� �ϴ� �� �ƴѰ�?

	// 1. �̹��� ���� ����
	auto Sinclair2 = ResourceManager::Get().GetTexture("��Ŭ����", "2��");
	auto Sinclair3 = ResourceManager::Get().GetTexture("��Ŭ����", "3��");
	auto Sinclair4 = ResourceManager::Get().GetTexture("��Ŭ����", "4��");
	// 2. ������Ʈ �����
	auto Character = std::make_unique<Object>();
	Character->SetPosition(Vec2(0, 0));

	// 3.0. ���� ���� ������Ʈ
	auto chInfo = Character->GetRenderInfo();
	chInfo->SetBitmap(Sinclair2.Get());
	// 3. ��� ������Ʈ �����
	auto chComp = Character->AddComponent<BackgroundComponent>(chInfo);

	// 3.1.1 ������ �ٸ��� 
	chComp->SetWidth(1920); chComp->SetHeight(1080);

	chComp->BitmapPush("Sinclair2", Sinclair2);
	chComp->BitmapPush("Sinclair3", Sinclair3);
	chComp->BitmapPush("Sinclair4", Sinclair4);
	// 9. ���� ������Ʈ�鿡 ����ֱ� 
	m_gameObjects.emplace("Character", std::move(Character));
	

	//////////////////////
	//////////////////////
	//////////////////////
	// [3] �ؽ�Ʈ â
	
	// 1. �̹��� ���� ����
	auto �ƿ�����1 = ResourceManager::Get().GetTexture("�ƿ�����", "01");
	// 2. ������Ʈ �����
	auto �ؽ�Ʈ�ڽ� = std::make_unique<Object>();
	     �ؽ�Ʈ�ڽ�->SetPosition(Vec2(421, 766));

	// 3.0. ���� ���� ������Ʈ
		 auto outgame1Info = �ؽ�Ʈ�ڽ�->GetRenderInfo();
		 outgame1Info->SetBitmap(�ƿ�����1.Get());
	// 3. ��� ������Ʈ �����
	auto �ؽ�Ʈ�ڽ�Comp = �ؽ�Ʈ�ڽ�->AddComponent<BackgroundComponent>(outgame1Info);
	
	// 3.1.1 ������ 
	�ؽ�Ʈ�ڽ�Comp->SetWidth(�ƿ�����1->GetSize().width);
	�ؽ�Ʈ�ڽ�Comp->SetHeight(�ƿ�����1->GetSize().height);


	�ؽ�Ʈ�ڽ�Comp->BitmapPush("�ƿ�����1", �ƿ�����1);
	�ؽ�Ʈ�ڽ�Comp->SetCurrentBitmap("�ƿ�����1");
	// 9. ���� ������Ʈ�鿡 ����ֱ� 
	m_gameObjects.emplace("�ƿ�����1", std::move(�ؽ�Ʈ�ڽ�));


	/////////////////////
	/////////////////////
	/////////////////////
	// [4] �� ��ư

	// 1. �̹��� ���� ����
	auto �ƿ�����4 = ResourceManager::Get().GetTexture("�ƿ�����", "04");
	// 2. ������Ʈ �����
	auto �� = std::make_unique<Object>();
	��->SetPosition(Vec2(1134, 905));

	auto ��info = ��->GetRenderInfo();
	��info->SetBitmap(�ƿ�����4.Get());
	// 3. ��ư ������Ʈ �����
	auto YesComp = ��->AddComponent<ButtonComponent>(��info);
	YesComp->SetWidth( �ƿ�����4->GetSize().width);
	YesComp->SetHeight(�ƿ�����4->GetSize().height);

	//  4. ��ư ��Ʈ�� ����
	YesComp->BitmapPush("yes",  �ƿ�����4);
	YesComp->BitmapPush("transparent", transparentImg);
	
	if (m_state == FIRST_ENTER)
		YesComp->SetCurrentBitmap("transparent");
	else
		YesComp->SetCurrentBitmap("yes");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto Yes_mouseListener = ��->AddComponent<MouseListenerComponent>(
		[YesComp](const MSG& msg) {
			YesComp->CheckCollision(msg);
			YesComp->Worked(msg);
		}
	);

	YesComp->SetOnClickCallback([this]() {
		// �׷��� �ϸ� �Ǵ� ����
		//SceneManager::Get().ChangeScene("InGame");
		//std::cout << "SceneManager::Get().ChangeScene(\"InGame\");" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("�ƿ�����4", std::move(��));


	/////////////////////
	/////////////////////
	/////////////////////
	// [5] �ƴϿ� ��ư/���� ��ư

	// 1. �̹��� ���� ����
	auto �ƿ�����5���� = ResourceManager::Get().GetTexture("�ƿ�����", "05����");
	auto �ƿ�����5�ƴϿ� = ResourceManager::Get().GetTexture("�ƿ�����", "05�ƴϿ�");

	// 2. ������Ʈ �����
	auto �ƴϿ� = std::make_unique<Object>();
	�ƴϿ�->SetPosition(Vec2(1296, 906));

	auto �ƴϿ�info = �ƴϿ�->GetRenderInfo();
	�ƴϿ�info->SetBitmap(�ƿ�����5����.Get());

	// 3. ��ư ������Ʈ �����
	auto NoComp = �ƴϿ�->AddComponent<ButtonComponent>(�ƴϿ�info);
	NoComp->SetWidth(�ƿ�����5�ƴϿ�->GetSize().width);
	NoComp->SetHeight(�ƿ�����5�ƴϿ�->GetSize().height);

	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	NoComp->BitmapPush("next", �ƿ�����5����);
	NoComp->BitmapPush("no", �ƿ�����5�ƴϿ�);
	NoComp->BitmapPush("transparent", transparentImg);

	//changeState()
	//NoComp->SetCurrentBitmap("no");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto No_mouseListener = �ƴϿ�->AddComponent<MouseListenerComponent>(
		[NoComp](const MSG& msg) {
			NoComp->CheckCollision(msg);
			NoComp->Worked(msg);
		}
	);

	NoComp->SetOnClickCallback([this]() {
			//changeState()
		});

	/// 9 
	m_gameObjects.emplace("�ƿ�����5", std::move(�ƴϿ�));

	

	/////////////////////
	/////////////////////
	/////////////////////
	// [6] �ڷΰ��� ��ư

	// 1. �̹��� ���� ����
	auto �ڷΰ��� = ResourceManager::Get().GetTexture("�ڷΰ���");
	// 2. ������Ʈ �����
	auto �ڷ� = std::make_unique<Object>();
	�ڷ�->SetPosition(Vec2(41, 31));

	auto �ڷ�info = �ڷ�->GetRenderInfo();
	�ڷ�info->SetBitmap(�ڷΰ���.Get());
	// 3. ��ư ������Ʈ �����
	auto backComp = �ڷ�->AddComponent<ButtonComponent>(�ڷ�info);
	backComp->SetWidth(65); 
	backComp->SetHeight(65);
	
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

		if(m_state == State::FIRST_ENTER ) 
		{
			GameManager::Get().isFirst = true;
		}

		std::cout << "��ư Ŭ���� - ���� ��: " << typeid(*this).name() << std::endl;
		isSETTING = false;
		SafeChangeScene("Title");

		});

	/// 9
	m_gameObjects.emplace("�ڷΰ���", std::move(�ڷ�));


	/////////////////////
	/////////////////////
	/////////////////////
	// [7] ���� :  > â��� �̵��Ѵ�.
	// size = 250 * 38
	// pos = 750, 825


	// 2. ������Ʈ �����
	auto Chango = std::make_unique<Object>();
	Chango->SetPosition(Vec2(750, 825));

	auto Changoinfo = Chango->GetRenderInfo();
	Changoinfo->SetBitmap(transparentImg.Get());

	// 3. ��ư ������Ʈ �����
	auto ChangoComp = Chango->AddComponent<ButtonComponent>(Changoinfo);
	ChangoComp->SetWidth (250);
	ChangoComp->SetHeight(38 );

	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	ChangoComp->BitmapPush("transparent", transparentImg);

	ChangoComp->SetCurrentBitmap("transparent");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto Chango_mouseListener = Chango->AddComponent<MouseListenerComponent>(
		[ChangoComp](const MSG& msg) {
			ChangoComp->CheckCollision(msg);
			ChangoComp->Worked(msg);
		}
	);

	ChangoComp->SetOnClickCallback([this]() {

		});

	/// 9
	m_gameObjects.emplace("Chango", std::move(Chango));


	/////////////////////
	/////////////////////
	/////////////////////
	// [8] ���� :  > ������ ������.
	// pos = 750, 863
	
	// 2. ������Ʈ �����
	auto Moheom = std::make_unique<Object>();
	Moheom->SetPosition(Vec2(750, 863));

	auto Moheominfo = Moheom->GetRenderInfo();
	Moheominfo->SetBitmap(transparentImg.Get());

	// 3. ��ư ������Ʈ �����
	auto MoheomComp = Moheom->AddComponent<ButtonComponent>(Moheominfo);
	MoheomComp->SetWidth(250);
	MoheomComp->SetHeight(38);

	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	MoheomComp->BitmapPush("transparent", transparentImg);

	MoheomComp->SetCurrentBitmap("transparent");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto Moheom_mouseListener = Moheom->AddComponent<MouseListenerComponent>(
		[MoheomComp](const MSG& msg) {
			MoheomComp->CheckCollision(msg);
			MoheomComp->Worked(msg);
		}
	);

	MoheomComp->SetOnClickCallback([this]() {
		});

	/// 9
	m_gameObjects.emplace("Moheom", std::move(Moheom));

	/////////////////////
	/////////////////////
	/////////////////////
	// [10] ���� ��ư

	// 1. �̹��� ���� ����
	auto ���� = ResourceManager::Get().GetTexture("����");
	// 2. ������Ʈ �����
	auto ������ = std::make_unique<Object>();
	������->SetPosition(Vec2(41, 106));

	auto ����info = ������->GetRenderInfo();
	����info->SetBitmap(����.Get());
	// 3. ��ư ������Ʈ �����
	auto settingComp = ������->AddComponent<ButtonComponent>(����info);
	settingComp->SetWidth (65);
	settingComp->SetHeight(65);

	//  4. ��ư ��Ʈ�� ����
	settingComp->BitmapPush("setting", ����);

	settingComp->SetCurrentBitmap("setting");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto set_mouseListener = ������->AddComponent<MouseListenerComponent>(
		[settingComp](const MSG& msg) {
			settingComp->CheckCollision(msg);
			settingComp->Worked(msg);
		}
	);



	settingComp->SetOnClickCallback([this]() {
		std::cout << "���� ��ư Ŭ���� - ���� ��: " << typeid(*this).name() << std::endl;
		isSETTING = !isSETTING;
		if (m_state == State::FIRST_ENTER)
		{
			GameManager::Get().isFirst = true;
		}

		SafeChangeScene("OutGame");
		});

	/// 9
	m_gameObjects.emplace("����", std::move(������));

	if (isSETTING)
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
		// [7] ����������̴�_��   
		// [8] ����������̴�_�ڵ� 
		// [9] ȿ���������̴�_��   
		// [10] ȿ���������̴�_�ڵ� 

		// 1. �̹��� ���ҽ� ��������
		auto barImg = ResourceManager::Get().GetTexture("����", "bar");
		auto handleImg = ResourceManager::Get().GetTexture("����", "handle");

		// 2. �����̴� ������Ʈ �����
		auto bgmBar = std::make_unique<Object>();
		auto bgmHandle = std::make_unique<Object>();
		auto sfxBar = std::make_unique<Object>();
		auto sfxHandle = std::make_unique<Object>();


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
		m_gameObjects.emplace("ũũ_bgm��", std::move(bgmBar));
		m_gameObjects.emplace("ũũ_bgm�ڵ�", std::move(bgmHandle));
		m_gameObjects.emplace("ũũ_sfx��", std::move(sfxBar));
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
			isSETTING = !isSETTING;
			SafeChangeScene("OutGame");
			});

		/// 9
		m_gameObjects.emplace("ũ����_�ڷΰ���", std::move(�ڷ�));
	}
}


void Scene_Outgame::SetupCharacterAndBackground()
{	
	// ����� �ȹٲ�
	auto bg = m_gameObjects["Background"s]->GetComponent<BackgroundComponent>();
	bg->SetCurrentBitmap("Background");

	// Ű�� �̹��� ���� �ϴ� ���� "Sinclair + GameManager::Get().curGen" = Sinclair3? 2? 4?
	auto ch = m_gameObjects["Character"s]->GetComponent<BackgroundComponent>();
	std::string result = "Sinclair";
	result += std::to_string(GameManager::Get().curGen/*�갡 ���븦 ��Ÿ���� ������*/);
	ch->SetCurrentBitmap(result);
}

void Scene_Outgame::ChangeState(State newState)
{
	m_state = newState;

	// ��ư �ݹ��� �������� ���� �غ�
	auto yesButton = m_gameObjects["�ƿ�����4"]->GetComponent<ButtonComponent>();
	auto noButton = m_gameObjects["�ƿ�����5"]->GetComponent<ButtonComponent>();
	auto changoButton = m_gameObjects["Chango"]->GetComponent<ButtonComponent>();
	auto moheomButton = m_gameObjects["Moheom"]->GetComponent<ButtonComponent>();
	// '����' �Ǵ� ������ ��ư�鵵 ���������� �����ɴϴ�.

	switch (m_state)
	{
	case FIRST_ENTER:
	{
		curText = stringFIRST_ENTER[index];

		// '����' ��ư�� �����ְ� '��' ��ư�� ����ϴ�.
		yesButton->SetCurrentBitmap("transparent");
		noButton->SetCurrentBitmap("next");

		// '����' ��ư�� �ݹ��� �����Ͽ�, ���� �ؽ�Ʈ�� �����ְų� ���¸� 80002�� �����ϵ��� �մϴ�.
		noButton->SetOnClickCallback([this]() {
			if (index < 2)
			{
				index++;
				curText = stringFIRST_ENTER[index];
			}
			else
			{
				m_state = CHOICE_MENU;
				SafeChangeScene("Tutorial");
			}
		});

		changoButton->SetOnClickCallback([this]() {
			});
		moheomButton->SetOnClickCallback([this]() {
			});
		break;
}

	case CHOICE_MENU: // �и��� ���� ����
	{
		//curText = stringCHOICE_MENU;

		yesButton->SetCurrentBitmap("transparent");
		noButton->SetCurrentBitmap("transparent");

		yesButton->SetOnClickCallback([this]() {
			});

		noButton->SetOnClickCallback([this]() {
			});

		changoButton->SetOnClickCallback([this]() {
			ChangeState(ENTER_OUTGAME);
			});
		moheomButton->SetOnClickCallback([this]() {
			ChangeState(ENTER_END);
			});


		break;
	}

	case ENTER_OUTGAME:
	{
		curText = stringENTER_OUTGAME; 

		yesButton->SetCurrentBitmap("yes");
		noButton->SetCurrentBitmap("no");

		yesButton->SetOnClickCallback([this]() {
			cout << "â�� ���ðڽ��ϱ�?..." << endl;
			SafeChangeScene("InGame");
			});

		noButton->SetOnClickCallback([this]() {
			ChangeState(CHOICE_MENU);
			});

		changoButton->SetOnClickCallback([this]() {
			});
		moheomButton->SetOnClickCallback([this]() {
			});

		break;
	}

	case ENTER_END:
	{
		curText = stringENTER_END; // "������ �����ðڽ��ϱ�?..."

		yesButton->SetCurrentBitmap("yes");
		noButton->SetCurrentBitmap("no");

		yesButton->SetOnClickCallback([this]() {
			cout << "������ �����ðڽ��ϱ�?..." << endl;
			GameManager::Get().PreAdv();
			SafeChangeScene("End");
			});

		noButton->SetOnClickCallback([this]() {
			ChangeState(CHOICE_MENU); // �������� ���ư���
			});

		changoButton->SetOnClickCallback([this]() {
			});
		moheomButton->SetOnClickCallback([this]() {
			});

		break;
	}
	}
}

