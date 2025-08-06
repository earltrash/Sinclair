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
	if (dirty) return; // 이미 초기화되었으면 스킵

	CreateObj(); // 오브젝트 생성 (한 번만)
	dirty = true;
}

void Scene_Title::Enter()
{
	Initalize();
	// 씬 진입 시마다 실행할 것들
	// 오브젝트 생성, 오브젝트 상태 초기화, 사운드 재생 등
	// ResetObjectStates();
	// PlayBGM();
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

		D2DRenderer::Get().DrawMessage(L"싱클레어 가 히스토리", 147.f, 254.f, 1300.f, 1000.f, D2D1::ColorF::LightPink);		

}


void Scene_Title::CreateObj()
{
	//////////////////////
	// Background 

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
	// 텍스트는 이렇게 만들면 안됨!
	// 생성할 때 말고 랜더 할 때 마다

	//////////////////////
	// gameStartButton
	// 리소스 매니저 해결하기

	// 1. 이미지 갖고 오기
	auto gameStartButton_texture_normal = ResourceManager::Get().GetTexture("시작화면", "2");
	auto gameStartButton_texture_hover = ResourceManager::Get().GetTexture("시작화면", "3");
	auto gameStartButton_texture_pressed = ResourceManager::Get().GetTexture("시작화면", "4");

	// 2. 오브젝트 만들기
	auto gameStartButton = std::make_unique<Object>();
	gameStartButton->SetPosition(Vec2(147, 564));

	auto gstartInfo = gameStartButton->GetRenderInfo();
	gstartInfo->SetBitmap(gameStartButton_texture_normal.Get());

	// 3. 버튼 컴포넌트 만들기
	auto gameStartButton_buttonComp = gameStartButton->AddComponent<ButtonComponent>(gstartInfo);
	// 3.1.2 사이즈 같으면
	gameStartButton_buttonComp->SetWidth(gameStartButton_texture_normal->GetSize().width);
	gameStartButton_buttonComp->SetHeight(gameStartButton_texture_normal->GetSize().height);

	// 4. 버튼 비트맵 설정
	// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
	gameStartButton_buttonComp->BitmapPush("normal", gameStartButton_texture_normal);
	gameStartButton_buttonComp->BitmapPush("hover", gameStartButton_texture_hover);
	gameStartButton_buttonComp->BitmapPush("pressed", gameStartButton_texture_pressed);

	// *** 중요: 초기 비트맵 설정 ***
	gameStartButton_buttonComp->SetCurrentBitmap("normal");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
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

		// 리소스 매니저
	auto gameSettingButton_texture_normal = ResourceManager::Get().GetTexture("시작화면", "3");
	auto gameSettingButton_texture_hover = ResourceManager::Get().GetTexture("시작화면", "4");
	auto gameSettingButton_texture_pressed = ResourceManager::Get().GetTexture("시작화면", "2");

	auto
		gameSettingButton = std::make_unique<Object>();
	gameSettingButton->SetPosition(Vec2(147, 697));

	auto gsettingInfo = gameSettingButton->GetRenderInfo();
	gsettingInfo->SetBitmap(gameSettingButton_texture_normal.Get());

	auto gameSettingButton_buttonComp = gameSettingButton->AddComponent<ButtonComponent>(gsettingInfo);
	// 3.1.2 사이즈 같으면
	gameSettingButton_buttonComp->SetWidth(gameStartButton_texture_normal->GetSize().width);
	gameSettingButton_buttonComp->SetHeight(gameStartButton_texture_normal->GetSize().height);
	auto gameSettingButton_mouseListener = gameSettingButton->AddComponent<MouseListenerComponent>(
		// 여기서 어떻게 입력 처리 하지?
		[gameSettingButton_buttonComp](const MSG& msg) {
			gameSettingButton_buttonComp->Worked(msg);
		}
	);

	// 버튼 설정
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

	// 리소스 매니저
	auto creditButton_texture_normal = ResourceManager::Get().GetTexture("시작화면", "4");
	auto creditButton_texture_hover = ResourceManager::Get().GetTexture("시작화면", "2");
	auto creditButton_texture_pressed = ResourceManager::Get().GetTexture("시작화면", "3");

	auto
		creditButton = std::make_unique<Object>();
	creditButton->SetPosition(Vec2(147, 830));

	auto creditInfo = creditButton->GetRenderInfo();
	creditInfo->SetBitmap(creditButton_texture_normal.Get());

	auto creditButton_buttonComp = creditButton->AddComponent<ButtonComponent>(creditInfo);
	// 3.1.2 사이즈 같으면
	creditButton_buttonComp->SetWidth(gameStartButton_texture_normal->GetSize().width);
	creditButton_buttonComp->SetHeight(gameStartButton_texture_normal->GetSize().height);
	auto creditButton_mouseListener = creditButton->AddComponent<MouseListenerComponent>(
		// 여기서 어떻게 입력 처리 하지?
		[creditButton_buttonComp](const MSG& msg) {
			creditButton_buttonComp->Worked(msg);
		}
	);

	// 버튼 설정
	creditButton_buttonComp->BitmapPush("normal", creditButton_texture_normal);
	creditButton_buttonComp->BitmapPush("hover", creditButton_texture_hover);
	creditButton_buttonComp->BitmapPush("pressed", creditButton_texture_pressed);
	creditButton_buttonComp->SetOnClickCallback([this]() {
		//SceneManager::Get().ChangeScene("Title");
		std::cout << "The button is pressed." << std::endl;
		});

	m_gameObjects.emplace("creditButton", std::move(creditButton));

}
