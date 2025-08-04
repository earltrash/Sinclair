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
		Initalize();
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
	// 씬 진입 시마다 실행할 것들
	// 오브젝트 상태 초기화, 사운드 재생 등
	// ResetObjectStates();
	// PlayBGM();
}

void Scene_Title::Exit()
{
	// 씬 종료 시마다 실행할 것들  
	// 사운드 정지, 상태 저장 등
	//StopBGM();
	//PauseAnimations();
}

void Scene_Title::Clean()
{
	m_gameObjects.clear();
	//SceneAssets.clear();
}

void Scene_Title::Update()
{

}

void Scene_Title::LogicUpdate(float delta)
{

}



void Scene_Title::Render()
{
		D2DRenderer::Get().RenderBegin();
		for (const auto& [Name, obj] : m_gameObjects)
		{

			ComPtr<ID2D1Bitmap1> bitmap = nullptr;
			D2D1_RECT_F dest;
			dest.left = 0;
			dest.top = 0;
			dest.right = 0;
			dest.bottom = 0;
			// 투명 구현한다면...
			float opacity = 1.0f;

			// ButtonComponent 우선 확인
			auto buttonComp = obj->GetComponent<ButtonComponent>();
			if (buttonComp != nullptr) {
				bitmap = buttonComp->GetBitmap();
				dest.right += buttonComp->GetWidth();
				dest.bottom += buttonComp->GetHeight();
				opacity = buttonComp->m_opacity;
			}
			else {
				// ButtonComponent가 없으면 BackgroundComponent 확인
				auto bgComp = obj->GetComponent<BackgroundComponent>();
				if (bgComp != nullptr) {
					bitmap = bgComp->GetBitmap();
					dest.right += bgComp->GetWidth();
					dest.bottom += bgComp->GetHeight();
				}
			}

			if (!bitmap) {
				continue;
			}

			dest.left += obj->GetTransform().m_position.x;
			dest.top += obj->GetTransform().m_position.y;
			dest.right += dest.left;
			dest.bottom += dest.top;

			// 투명도 적용 x
			//D2DRenderer::Get().DrawBitmap(bitmap.Get(), dest);
			// 투명도 적용 o
			D2D1_RECT_F srcRect = D2D1::RectF(0, 0, bitmap->GetSize().width, bitmap->GetSize().height); ;

			D2DRenderer::Get().DrawBitmap(bitmap.Get(), dest, srcRect, opacity);
		}

		//D2DRenderer::Get().CreateWriteResource(L"궁서체", DWRITE_FONT_WEIGHT_BOLD, 100.0f);
		D2DRenderer::Get().DrawMessage(L"싱클레어 가 히스토리", 147.f, 254.f, 1300.f, 1000.f, D2D1::ColorF::LightPink);
		//D2DRenderer::Get().CreateWriteResource();

		D2DRenderer::Get().RenderEnd();
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

	// 3. 배경 컴포넌트 만들기
	auto bgComp = Background->AddComponent<BackgroundComponent>();
	// 3.1.1 사이즈 다르면 
	bgComp->SetWidth(1920); bgComp->SetHeight(1080);
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

	// 3. 버튼 컴포넌트 만들기
	auto gameStartButton_buttonComp = gameStartButton->AddComponent<ButtonComponent>();
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

	auto gameSettingButton_buttonComp = gameSettingButton->AddComponent<ButtonComponent>();
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
		SceneManager::Get().ChangeScene("Title");
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

	auto creditButton_buttonComp = creditButton->AddComponent<ButtonComponent>();
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
