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

}



void Scene_Title::Render()
{
	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetRenderInfo()->GetRenderInfo());
	}

	if (isCredit)
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
	auto bgComp = Background->AddComponent<BackgroundComponent>(bgInfo);
	// 3.1.1 사이즈 다르면 
	bgComp->SetWidth(1920.f); bgComp->SetHeight(1080.f);
	bgComp->BitmapPush("Background", gameStartBackground);
	// 9. 게임 오브젝트들에 집어넣기
	m_gameObjects.emplace("Background", std::move(Background));


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
			gameSettingButton_buttonComp->Worked(msg);
				}
	);

	// 버튼 설정
	gameSettingButton_buttonComp->BitmapPush("setting", settingButtonTexture);
	gameSettingButton_buttonComp->SetCurrentBitmap("setting");

	gameSettingButton_buttonComp->SetOnClickCallback([this]() {
		std::cout << "The 세팅 button is pressed." << std::endl;
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
			creditButton_buttonComp->Worked(msg);
				}
	);

	// 버튼 설정
	creditButton_buttonComp->BitmapPush("credit", creditButtonTexture);
	creditButton_buttonComp->SetCurrentBitmap("credit");

	creditButton_buttonComp->SetOnClickCallback([this]() {
		std::cout << "The 크레딧 button is pressed." << std::endl;
		isCredit = !isCredit;
		SafeChangeScene("Title");
		});

	m_gameObjects.emplace("creditButton", std::move(creditButton));
	
	if(isCredit)
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
		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("크레딧_06", std::move(Credit06));


		/////////////////////
		/////////////////////
		/////////////////////
		// [9] 뒤로가기 버튼

		// 1. 이미지 갖고 오기
		auto 뒤로가기 = ResourceManager::Get().GetTexture("뒤로가기");
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
				backComp->Worked(msg);
			}
		);

		backComp->SetOnClickCallback([this]() {
			std::cout << "닫기 버튼 클릭됨 - 현재 씬: " << typeid(*this).name() << std::endl;
			isCredit = !isCredit;
			SafeChangeScene("Title");
			});

		/// 9
		m_gameObjects.emplace("크레딧_뒤로가기", std::move(뒤로));
	}

}