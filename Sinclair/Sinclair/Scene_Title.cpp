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
#include "GameManager_2.h"
#include "EffectComponent.h"
#include "PlayComponent.h"


#include "SoundBank_.h"

#include "SoundManager.h"

Scene_Title::Scene_Title(string name)
{
	m_name = name;
}

Scene_Title::~Scene_Title()
{
}

void Scene_Title::Initalize()
{
	if (dirty) return; // 이미 초기화되었으면 스킵

	CreateObj(); // 오브젝트 생성 (한 번만)
	dirty = true;

	for (const auto& [Name, obj] : m_gameObjects)
	{
		UIManager::Get().AddSceneObject(obj);
	}

	//auto* music = ResourceManager::Get().Get_SoundBank().GetBGM("SM");
	//SoundManager::Instance().SetBGMVolume(0, 1.0f);
	SoundManager::Instance().PlayBGM("Title", false);
}

void Scene_Title::Enter()
{
	Initalize();
}

void Scene_Title::Exit()
{
	// 씬 종료 시마다 실행할 것들  
	// 사운드 정지, 상태 저장 등
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

	// UIManager에서 씬 객체들을 먼저 정리
	UIManager::Get().ClearSceneObjects();

	// 잠시 대기 후 게임 객체들 정리
	m_gameObjects.clear();

	dirty = false;
}

void Scene_Title::Update()
{
	for (auto& obj : m_gameObjects)
	{
		obj.second->Update();
	}
	// 씬 전환 지연 처리
	if (m_isTransitioning && !m_nextScene.empty())
	{
		m_currentDelay += 0.016f; // 대략적인 프레임 시간 (60FPS 기준)
		if (m_currentDelay >= m_transitionDelay)
		{
			// 실제 씬 전환 실행
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

}


void Scene_Title::CreateObj()
{
	////////////////////////
	//// 투명 이미지 갖고 오기
	//auto transparentImg = ResourceManager::Get().GetTexture("transparent");

	//////////////////////
	//////////////////////
	//////////////////////
	// [1] Background 

	// 1. 이미지 갖고 오기
	auto gameStartBackground = ResourceManager::Get().GetTexture("시작화면");
	// 2. 오브젝트 만들기
	auto Background = std::make_unique<Object>();
	Background->SetPosition(Vec2(0, 0));

	auto bgInfo = Background->GetRenderInfo();
	bgInfo->SetBitmap(gameStartBackground.Get());

	// 3. 배경 컴포넌트 만들기
	//auto bgComp = Background->AddComponent<BackgroundComponent>(bgInfo);
	// 3.1.1 사이즈 다르면 
	//bgComp->SetWidth(1920.f); bgComp->SetHeight(1080.f);
	//bgComp->BitmapPush("Background", gameStartBackground);
	//bgComp->SetCurrentBitmap("Background");

	// 대조 효과
	auto bg_contrast = Background->AddComponent<DynamicContrast_Effect>(bgInfo, -0.2f, 0.6f, 0.0005f, gameStartBackground.Get());
	// 9. 게임 오브젝트들에 집어넣기
	m_gameObjects.emplace("Background", std::move(Background));

	//////////////////////
	//////////////////////
	//////////////////////
	//	airParticle

	// 1. 이미지 로드
	auto dustBM = ResourceManager::Get().GetTexture("airParticle");
	// 2. 오브젝트 생성
	auto dust1 = std::make_unique<Object>();
	// 3. 기본 설정
	auto d1_info = dust1->GetRenderInfo();
	d1_info->SetBitmap(dustBM.Get());
	// 4. 컴포넌트 설정
	auto d1_blur = dust1->AddComponent<GaussianBlur_Effect>(d1_info, 1.f, dustBM.Get());
	auto d1_updown = dust1->AddComponent<UpDown_Effect>(d1_info, dust1->GetTransform(), 10.f, 0.03f, 0.03f);
	auto d1_slide = dust1->AddComponent<Slide_Effect>(d1_info, dust1->GetTransform(), 0.07f, 1920.f, 1920.f, true);
	// 5. 게임 오브젝트 관리 map에 추가
	m_gameObjects.emplace("b_airDust1", std::move(dust1));

	// 2. 오브젝트 생성
	auto dust2 = std::make_unique<Object>();
	// 3. 기본 설정
	auto d2_info = dust2->GetRenderInfo();
	d2_info->SetBitmap(dustBM.Get());
	dust2->GetTransform().SetPosition({ 1920.f, 0.f });
	// 4. 컴포넌트 설정
	auto d2_blur = dust2->AddComponent<GaussianBlur_Effect>(d2_info, 3.f, dustBM.Get());
	auto d2_updown = dust2->AddComponent<UpDown_Effect>(d2_info, dust2->GetTransform(), 10.f, 0.03f, 0.03f);
	auto d2_slide = dust2->AddComponent<Slide_Effect>(d2_info, dust2->GetTransform(), 0.08f, 1920.f, 1920.f, true);
	// 5. 게임 오브젝트 관리 map에 추가
	m_gameObjects.emplace("b_airDust2", std::move(dust2));

	//////////////////////
	//////////////////////
	//////////////////////
	// [2] 게임 이름

	// 1. 이미지 갖고 오기
	auto gameNameBG = ResourceManager::Get().GetTexture("시작화면", "01");
	// 2. 오브젝트 만들기
	auto gameName = std::make_unique<Object>();
	gameName->SetPosition(Vec2(100.15f, 239.66f));

	auto nameInfo = gameName->GetRenderInfo();
	nameInfo->SetBitmap(gameNameBG.Get());

	// 3. 배경 컴포넌트 만들기
	auto nameComp = gameName->AddComponent<BackgroundComponent>(nameInfo);
	nameComp->SetWidth(gameNameBG->GetSize().width);
	nameComp->SetHeight(gameNameBG->GetSize().height);
	nameComp->BitmapPush("gameName", gameNameBG);
	nameComp->SetCurrentBitmap("gameName");
	// 9. 게임 오브젝트들에 집어넣기
	m_gameObjects.emplace("gameName", std::move(gameName));



	//////////////////////
	//////////////////////
	//////////////////////
	// [3] startButton

	// 1. 이미지 갖고 오기
	auto startButtonTexture = ResourceManager::Get().GetTexture("시작화면", "02");

	// 2. 오브젝트 만들기
	auto startButton = std::make_unique<Object>();
	startButton->SetPosition(Vec2(147, 564));

	auto startInfo = startButton->GetRenderInfo();
	startInfo->SetBitmap(startButtonTexture.Get());

	// 3. 버튼 컴포넌트 만들기
	auto gameStartButton_buttonComp = startButton->AddComponent<ButtonComponent>(startInfo);
	// 3.1.2 사이즈 같으면
	gameStartButton_buttonComp->SetWidth(startButtonTexture->GetSize().width);
	gameStartButton_buttonComp->SetHeight(startButtonTexture->GetSize().height);

	// 4. 버튼 비트맵 설정
	// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
	gameStartButton_buttonComp->BitmapPush("start", startButtonTexture);

	// *** 중요: 초기 비트맵 설정 ***
	gameStartButton_buttonComp->SetCurrentBitmap("start");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto gameStartButton_mouseListener = startButton->AddComponent<MouseListenerComponent>(
		[gameStartButton_buttonComp](const MSG& msg) {
			gameStartButton_buttonComp->CheckCollision(msg);
			gameStartButton_buttonComp->Worked(msg);
		});

	gameStartButton_buttonComp->SetOnClickCallback([this]() {


		SoundManager::Instance().CrossfadeBGM(SoundManager::Instance().Get_Playing_Key(), "OutGame", 2.5f);
		SafeChangeScene("OutGame");

	//	SoundManager::Instance().PauseBGM("Title" , true);
		//SoundManager::Instance().PauseBGM(SoundManager::Instance().Get_Playing_Key(), true);



		std::cout << "SceneManager::Get().ChangeScene(\"OutGame\");" << std::endl;
		});

	m_gameObjects.emplace("startButton", std::move(startButton));


	////////////////////// 
	////////////////////// 
	////////////////////// 
	// [4] settingButton

	// 리소스 매니저
	auto settingButtonTexture = ResourceManager::Get().GetTexture("시작화면", "03");

	auto
		settingButton = std::make_unique<Object>();
	settingButton->SetPosition(Vec2(147, 697));

	auto settingInfo = settingButton->GetRenderInfo();
	settingInfo->SetBitmap(settingButtonTexture.Get());

	auto gameSettingButton_buttonComp = settingButton->AddComponent<ButtonComponent>(settingInfo);
	// 3.1.2 사이즈 같으면
	gameSettingButton_buttonComp->SetWidth(startButtonTexture->GetSize().width);
	gameSettingButton_buttonComp->SetHeight(startButtonTexture->GetSize().height);
	auto gameSettingButton_mouseListener = settingButton->AddComponent<MouseListenerComponent>(
		// 여기서 어떻게 입력 처리 하지?
		[gameSettingButton_buttonComp](const MSG& msg) {
			gameSettingButton_buttonComp->CheckCollision(msg);
			gameSettingButton_buttonComp->Worked(msg);
		});

	// 버튼 설정
	gameSettingButton_buttonComp->BitmapPush("setting", settingButtonTexture);
	gameSettingButton_buttonComp->SetCurrentBitmap("setting");

	gameSettingButton_buttonComp->SetOnClickCallback([this]() {
		UIManager::Get().OpenWindow(UIWindowType::SettingsWindow);
		});

	m_gameObjects.emplace("settingButton", std::move(settingButton));

	//////////////////////
	//////////////////////
	//////////////////////
	// [5] creditButton

	// 리소스 매니저
	auto creditButtonTexture = ResourceManager::Get().GetTexture("시작화면", "04");

	auto
		creditButton = std::make_unique<Object>();
	creditButton->SetPosition(Vec2(147, 830));

	auto creditInfo = creditButton->GetRenderInfo();
	creditInfo->SetBitmap(creditButtonTexture.Get());

	auto creditButton_buttonComp = creditButton->AddComponent<ButtonComponent>(creditInfo);
	// 3.1
	creditButton_buttonComp->SetWidth(310 /*startButtonTexture->GetSize().width */);
	creditButton_buttonComp->SetHeight(75/*startButtonTexture->GetSize().height*/);


	auto creditButton_mouseListener = creditButton->AddComponent<MouseListenerComponent>(
		[creditButton_buttonComp](const MSG& msg) {
			creditButton_buttonComp->CheckCollision(msg);
			creditButton_buttonComp->Worked(msg);
		});

	// 버튼 설정
	creditButton_buttonComp->BitmapPush("credit", creditButtonTexture);
	creditButton_buttonComp->SetCurrentBitmap("credit");

	creditButton_buttonComp->SetOnClickCallback([this]() {
		UIManager::Get().OpenWindow(UIWindowType::CreditWindow);
		});

	m_gameObjects.emplace("creditButton", std::move(creditButton));

}