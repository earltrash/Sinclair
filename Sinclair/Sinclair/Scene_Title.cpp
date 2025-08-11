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
	if (dirty) return; // 이미 초기화되었으면 스킵
		
	CreateObj(); // 오브젝트 생성 (한 번만)
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

// 씬 전환 지연 처리를 위해 씬_스탠다드에서 업데이트 일괄 처리.
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

	switch (m_window)
	{
	case CREDIT:
	{
		// [기획 텍스트] 48pt 텍스트
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
		D2DRenderer::Get().DrawMessage(L"기획", 649.23f, 354.14f, 649.23f + 200, 354.14f + 200, D2D1::ColorF(0xffe6d8));

		// [프로그래밍 텍스트] 48pt 텍스트
		D2DRenderer::Get().DrawMessage(L"프로그래밍", 1043.14f, 354.14f, 1043.14f + 200, 354.14f + 600, D2D1::ColorF(0xffe6d8));

		// [기획팀 이름] 30pt 텍스트
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 30.0f);
		D2DRenderer::Get().DrawMessage(L"김유진\n문상진\n이주연", 656.65f, 536.94f, 656.65f + 200, 536.94f + 700, D2D1::ColorF(0xffe6d8));

		// [프로그래밍 이름] 30pt 텍스트
		D2DRenderer::Get().DrawMessage(L"김민경\n김범진\n장효제\n전경석", 1120.65f, 512.49f, 1120.65f + 200, 512.49f + 600, D2D1::ColorF(0xffe6d8));
		break;
	}
	case SETTING:
	{
		// [설정 텍스트] 48pt 텍스트
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
		D2DRenderer::Get().DrawMessage(L"설정", 528.77f, 343.0f, 528.77f + 200, 343.0f + 200, D2D1::ColorF(0xffe6d8));

		// [배경음 텍스트] 30pt 텍스트
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 30.0f);
		D2DRenderer::Get().DrawMessage(L"배경음", 550.18f, 498.77f, 550.18f + 200, 498.77f + 200, D2D1::ColorF(0xffe6d8));

		// [효과음 텍스트] 30pt 텍스트
		D2DRenderer::Get().DrawMessage(L"효과음", 550.18f, 641.77f, 550.18f + 200, 641.77f + 200, D2D1::ColorF(0xffe6d8));


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
	nameComp->SetWidth (gameNameBG->GetSize().width);
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
		SafeChangeScene("OutGame");
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

	// 리소스 매니저
	auto creditButtonTexture = ResourceManager::Get().GetTexture("시작화면", "04");

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

	// 버튼 설정
	creditButton_buttonComp->BitmapPush("credit", creditButtonTexture);
	creditButton_buttonComp->SetCurrentBitmap("credit");

	creditButton_buttonComp->SetOnClickCallback([this]() {
		std::cout << "The 크레딧 button is pressed." << std::endl;
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
		// [6] [크레딧_01] 창 오브젝트
		// 1. 이미지 갖고 오기
		auto credit01Texture = ResourceManager::Get().GetTexture("크레딧", "01");
		// 2. 오브젝트 만들기
		auto Credit01 = std::make_unique<Object>();
		Credit01->SetPosition(Vec2(431, 270));
		auto credit01Info = Credit01->GetRenderInfo();
		credit01Info->SetBitmap(credit01Texture.Get());
		// 3. 배경 컴포넌트 만들기
		auto credit01Comp = Credit01->AddComponent<BackgroundComponent>(credit01Info);
		// 3.1.1 사이즈 설정
		credit01Comp->SetWidth(1059.f);
		credit01Comp->SetHeight(540.f);
		credit01Comp->BitmapPush("크레딧_01", credit01Texture);
		credit01Comp->SetCurrentBitmap("크레딧_01");
		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("크레딧_01", std::move(Credit01));

		//////////////////////
		//////////////////////
		//////////////////////
		// [7] [크레딧_03] 텍스트 배경 오브젝트
		// 1. 이미지 갖고 오기
		auto credit03Texture = ResourceManager::Get().GetTexture("크레딧", "03");
		// 2. 오브젝트 만들기
		auto Credit03 = std::make_unique<Object>();
		Credit03->SetPosition(Vec2(535, 467));
		auto credit03Info = Credit03->GetRenderInfo();
		credit03Info->SetBitmap(credit03Texture.Get());
		// 3. 배경 컴포넌트 만들기
		auto credit03Comp = Credit03->AddComponent<BackgroundComponent>(credit03Info);
		// 3.1.1 사이즈 설정
		credit03Comp->SetWidth(335.f);
		credit03Comp->SetHeight(283.f);
		credit03Comp->BitmapPush("크레딧_03", credit03Texture);
		credit03Comp->SetCurrentBitmap("크레딧_03");
		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("크레딧_03", std::move(Credit03));


		//////////////////////
		//////////////////////
		//////////////////////
		// [8] [크레딧_06] 텍스트 배경 오브젝트
		// 1. 이미지 갖고 오기
		auto credit06Texture = ResourceManager::Get().GetTexture("크레딧", "06");
		// 2. 오브젝트 만들기
		auto Credit06 = std::make_unique<Object>();
		Credit06->SetPosition(Vec2(999, 467));
		auto credit06Info = Credit06->GetRenderInfo();
		credit06Info->SetBitmap(credit06Texture.Get());
		// 3. 배경 컴포넌트 만들기
		auto credit06Comp = Credit06->AddComponent<BackgroundComponent>(credit06Info);
		// 3.1.1 사이즈 설정
		credit06Comp->SetWidth(335.f);
		credit06Comp->SetHeight(283.f);
		credit06Comp->BitmapPush("크레딧_06", credit06Texture);
		credit06Comp->SetCurrentBitmap("크레딧_06");
		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("크레딧_06", std::move(Credit06));


		/////////////////////
		/////////////////////
		/////////////////////
		// [9] 뒤로가기 버튼

		// 1. 이미지 갖고 오기
		auto 뒤로가기 = ResourceManager::Get().GetTexture("닫기");
		// 2. 오브젝트 만들기
		auto 뒤로 = std::make_unique<Object>();
		뒤로->SetPosition(Vec2(1393, 307));

		auto 뒤로info = 뒤로->GetRenderInfo();
		뒤로info->SetBitmap(뒤로가기.Get());
		// 3. 버튼 컴포넌트 만들기
		auto backComp = 뒤로->AddComponent<ButtonComponent>(뒤로info);
		backComp->SetWidth(35);
		backComp->SetHeight(35);

		//  4. 버튼 비트맵 설정
		// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
		backComp->BitmapPush("back", 뒤로가기);

		backComp->SetCurrentBitmap("back");

		// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
		auto Back_mouseListener = 뒤로->AddComponent<MouseListenerComponent>(
			[backComp](const MSG& msg) {
				backComp->CheckCollision(msg);
				backComp->Worked(msg);
			}
		);

		backComp->SetOnClickCallback([this]() {
			std::cout << "닫기 버튼 클릭됨 - 현재 씬: " << typeid(*this).name() << std::endl;
			m_window = NORMAL;
			SafeChangeScene("Title");
			});

		/// 9
		m_gameObjects.emplace("크레딧_뒤로가기", std::move(뒤로));
	}
	break;
	case SETTING:
	{
		//////////////////////
		//////////////////////
		//////////////////////
		// [6] [크레딧_01] 창 오브젝트
		// 1. 이미지 갖고 오기
		auto credit01Texture = ResourceManager::Get().GetTexture("크레딧", "01");
		// 2. 오브젝트 만들기
		auto Credit01 = std::make_unique<Object>();
		Credit01->SetPosition(Vec2(431, 270));
		auto credit01Info = Credit01->GetRenderInfo();
		credit01Info->SetBitmap(credit01Texture.Get());
		// 3. 배경 컴포넌트 만들기
		auto credit01Comp = Credit01->AddComponent<BackgroundComponent>(credit01Info);
		// 3.1.1 사이즈 설정
		credit01Comp->SetWidth(1059.f);
		credit01Comp->SetHeight(540.f);
		credit01Comp->BitmapPush("크레딧_01", credit01Texture);
		credit01Comp->SetCurrentBitmap("크레딧_01");
		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("크레딧_01", std::move(Credit01));

		//////////////////////
		//////////////////////
		//////////////////////
		// [7] 배경음슬라이더_바   
		// [8] 배경음슬라이더_핸들 
		// [9] 효과음슬라이더_바   
		// [10] 효과음슬라이더_핸들 

		// 1. 이미지 리소스 가져오기
		auto barImg = ResourceManager::Get().GetTexture("설정", "bar");
		auto handleImg = ResourceManager::Get().GetTexture("설정", "handle");
		
		// 2. 슬라이더 오브젝트 만들기
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


		// 3. 배경/ 슬라이더핸들 컴포넌트 만들기
		auto bgmBarComp = bgmBar->AddComponent<BackgroundComponent>(bgmBarInfo);
		auto bgmHandleComp = bgmHandle->AddComponent<SliderHandleComponent>(bgmHandleInfo, 729.0f, 1347.0f, GameManager::Get().bgmValue);
		auto sfxBarComp = sfxBar->AddComponent<BackgroundComponent>(sfxBarInfo);
		auto sfxHandleComp = sfxHandle->AddComponent<SliderHandleComponent>(sfxHandleInfo, 729.0f, 1347.0f, GameManager::Get().sfxValue);

		// 3.1.1 사이즈 설정 
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

		// 4. 값 변경 콜백 설정
		bgmHandleComp->SetOnValueChanged([](float value) {
			std::cout << "슬라이더 값: " << value << " (0.0~1.0)" << std::endl;
			// 예: 볼륨 설정
			// SoundManager::Get().SetMasterVolume(value);
			});

		sfxHandleComp->SetOnValueChanged([](float value) {
			std::cout << "슬라이더 값: " << value << " (0.0~1.0)" << std::endl;
			// 예: 볼륨 설정
			// SoundManager::Get().SetMasterVolume(value);
			});


		// 5. 마우스 리스너 컴포넌트
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

		// 6. 초기값 설정 -> 임시로 게임 매니저 쪽에서 받게끔, 하자. 
		// 사운드 매니저에서 받아오는 걸로 바꿔야 함.
		
		bgmHandleComp->SetValue(GameManager::Get().bgmValue);
		sfxHandleComp->SetValue(GameManager::Get().sfxValue);



		// 6. 게임 오브젝트에 등록
		m_gameObjects.emplace("크크_bgm바", std::move(bgmBar  ));
		m_gameObjects.emplace("크크_bgm핸들", std::move(bgmHandle));
		m_gameObjects.emplace("크크_sfx바", std::move(sfxBar  ));
		m_gameObjects.emplace("크크_sfx핸들", std::move(sfxHandle));




		/////////////////////
		/////////////////////
		/////////////////////
		// [11] 뒤로가기 버튼

		// 1. 이미지 갖고 오기
		auto 뒤로가기 = ResourceManager::Get().GetTexture("닫기");
		// 2. 오브젝트 만들기
		auto 뒤로 = std::make_unique<Object>();
		뒤로->SetPosition(Vec2(1393, 307));

		auto 뒤로info = 뒤로->GetRenderInfo();
		뒤로info->SetBitmap(뒤로가기.Get());
		// 3. 버튼 컴포넌트 만들기
		auto backComp = 뒤로->AddComponent<ButtonComponent>(뒤로info);
		backComp->SetWidth(35);
		backComp->SetHeight(35);

		//  4. 버튼 비트맵 설정
		// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
		backComp->BitmapPush("back", 뒤로가기);

		backComp->SetCurrentBitmap("back");

		// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
		auto Back_mouseListener = 뒤로->AddComponent<MouseListenerComponent>(
			[backComp](const MSG& msg) {
				backComp->CheckCollision(msg);
				backComp->Worked(msg);
			}
		);

		backComp->SetOnClickCallback([this]() {
			std::cout << "닫기 버튼 클릭됨 - 현재 씬: " << typeid(*this).name() << std::endl;
			m_window = NORMAL;
			SafeChangeScene("Title");
			});

		/// 9
		m_gameObjects.emplace("크레딧_뒤로가기", std::move(뒤로));

	}
	break;
	case NORMAL:
	default:
		break;
	}

}