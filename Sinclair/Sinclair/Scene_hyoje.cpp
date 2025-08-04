#include "Scene_hyoje.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"

Scene_hyoje::Scene_hyoje(string name)
{
	m_name = name;



	CreateObj();
	

}

Scene_hyoje::~Scene_hyoje()
{
}

void Scene_hyoje::Render() 
{
	D2DRenderer::Get().RenderBegin();
	
	for (const auto& [Name, obj] : m_gameObjects)
	{

		ComPtr<ID2D1Bitmap1> bitmap = nullptr;
		D2D1_RECT_F dest;
		dest.left =   0;
		dest.top =    0;
		dest.right = 0;
		dest.bottom = 0;

		// ButtonComponent 우선 확인
		auto buttonComp = obj->GetComponent<ButtonComponent>();
		if (buttonComp != nullptr) {
			bitmap = buttonComp->GetBitmap();
			dest.right += buttonComp-> GetWidth();
			dest.bottom += buttonComp->GetHeight();
		}
		else {
			// ButtonComponent가 없으면 BackgroundComponent 확인
			auto bgComp = obj->GetComponent<BackgroundComponent>();
			if (bgComp != nullptr) {
				bitmap = bgComp->GetBitmap(); 
				dest.right +=  bgComp->GetWidth();
				dest.bottom += bgComp->GetHeight();
			}
		}

		if (!bitmap) {
			continue;
		}

		auto size = bitmap->GetSize();

		dest.left   += obj->GetTransform().m_position.x;
		dest.top    += obj->GetTransform().m_position.y;
		dest.right  +=			dest.left;
		dest.bottom +=			dest.top ;

		D2DRenderer::Get().DrawBitmap(bitmap.Get(), dest);
	}

	D2DRenderer::Get().CreateWriteResource(L"궁서체", DWRITE_FONT_WEIGHT_BOLD, 100.0f);
	D2DRenderer::Get().DrawMessage(L"싱클레어 가 히스토리", 147.f, 254.f, 1300.f, 1000.f, D2D1::ColorF::LightPink);
	D2DRenderer::Get().CreateWriteResource();

	D2DRenderer::Get().RenderEnd();
}

void Scene_hyoje::CreateObj()
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
	auto gameStartButton_texture_normal = ResourceManager::Get().GetTexture("시작화면_2");
	auto gameStartButton_texture_hover = ResourceManager::Get().GetTexture("시작화면_3");
	auto gameStartButton_texture_pressed = ResourceManager::Get().GetTexture("시작화면_4");

	// 2. 오브젝트 만들기
	auto gameStartButton = std::make_unique<Object>();
	gameStartButton->SetPosition(Vec2(147, 564));

	// 3. 버튼 컴포넌트 만들기
	auto gameStartButton_buttonComp = gameStartButton->AddComponent<ButtonComponent>();
	// 3.1.2 사이즈 같으면
	gameStartButton_buttonComp->SetWidth(gameStartButton_texture_normal->GetSize().width);
	gameStartButton_buttonComp->SetHeight(gameStartButton_texture_normal->GetSize().height);
	
	// 4. 버튼 비트맵 설정
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
		//SceneManager::Get().ChangeScene("Title");
		std::cout << "The button is pressed." << std::endl;
		});

	m_gameObjects.emplace("gameStartButton", std::move(gameStartButton));


	//////////////////////
	// gameSettingButton
	
		// 리소스 매니저
	auto gameSettingButton_texture_normal = ResourceManager::Get().GetTexture("시작화면_3");
	auto gameSettingButton_texture_hover = ResourceManager::Get().GetTexture("시작화면_2");
	auto gameSettingButton_texture_pressed = ResourceManager::Get().GetTexture("시작화면_4");

	auto 
	gameSettingButton = std::make_unique<Object>();
	gameSettingButton->SetPosition(Vec2(147, 697));

	auto gameSettingButton_buttonComp =    gameSettingButton->AddComponent<ButtonComponent>();
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
	gameSettingButton_buttonComp->BitmapPush("normal",  gameSettingButton_texture_normal);
	gameSettingButton_buttonComp->BitmapPush("hover",   gameSettingButton_texture_hover);
	gameSettingButton_buttonComp->BitmapPush("pressed", gameSettingButton_texture_pressed);
	gameSettingButton_buttonComp->SetOnClickCallback([this]() {
		//SceneManager::Get().ChangeScene("Title");
		std::cout << "The button is pressed." << std::endl;
		});

	m_gameObjects.emplace("gameSettingButton", std::move(gameSettingButton));

	//////////////////////
	// creditButton

	// 리소스 매니저
	auto creditButton_texture_normal = ResourceManager::Get().GetTexture("시작화면_4");
	auto creditButton_texture_hover = ResourceManager::Get().GetTexture("시작화면_2");
	auto creditButton_texture_pressed = ResourceManager::Get().GetTexture("시작화면_3");

	auto
	creditButton = std::make_unique<Object>();
	creditButton->SetPosition(Vec2(147, 830));

	auto creditButton_buttonComp =    creditButton->AddComponent<ButtonComponent>();
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
	creditButton_buttonComp->BitmapPush("normal",  creditButton_texture_normal);
	creditButton_buttonComp->BitmapPush("hover",   creditButton_texture_hover);
	creditButton_buttonComp->BitmapPush("pressed", creditButton_texture_pressed);
	creditButton_buttonComp->SetOnClickCallback([this]() {
		//SceneManager::Get().ChangeScene("Title");
		std::cout << "The button is pressed." << std::endl;
		});

	m_gameObjects.emplace("gameSettingButton", std::move(creditButton));


}

