#include "Scene_InGame.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"


Scene_InGame::Scene_InGame(string name)
{
	m_name = name;
	
}

Scene_InGame::~Scene_InGame()
{

}

void Scene_InGame::Initalize()
{
	if (dirty) return; // 이미 초기화되었으면 스킵

	CreateObj(); // 오브젝트 생성 (한 번만)
	dirty = true;
}

void Scene_InGame::Clean()
{
	m_gameObjects.clear();

	dirty = false;
}

void Scene_InGame::Update()
{
}

void Scene_InGame::LogicUpdate(float delta)
{
}

void Scene_InGame::Enter()
{
	Initalize();
}

void Scene_InGame::Exit()
{
	Clean();
}

void Scene_InGame::Render()
{

	
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
				bitmap = bgComp->GetCurrentBitmap();
				dest.right += bgComp->GetWidth();
				dest.bottom += bgComp->GetHeight();
			}
		}

		if (!bitmap) {
			continue;
		}

		dest.left += obj->GetTransform().GetPosition().x;
		dest.top += obj->GetTransform().GetPosition().y;
		dest.right += dest.left;
		dest.bottom += dest.top;

		// 투명도 적용 x
		//D2DRenderer::Get().DrawBitmap(bitmap.Get(), dest);
		// 투명도 적용 o
		D2D1_RECT_F srcRect = D2D1::RectF(0, 0, bitmap->GetSize().width, bitmap->GetSize().height); ;

		D2DRenderer::Get().DrawBitmap(bitmap.Get(), dest, srcRect, opacity);
	}

	//D2DRenderer::Get().CreateWriteRegularResource();

	
}

void Scene_InGame::CreateObj()
{
	//////////////////////
	// Background 

	// 1. 이미지 갖고 오기
	auto inGameBackground = ResourceManager::Get().GetTexture("인게임");
	// 2. 오브젝트 만들기
	auto Background = std::make_unique<Object>();
	Background->SetPosition(Vec2(0, 0));

	// 3.0. 랜더 인포 컴포넌트
	auto bgInfo = Background->AddComponent<RenderInfo>(inGameBackground.Get());
	// 3. 배경 컴포넌트 만들기
	auto bgComp = Background->AddComponent<BackgroundComponent>(bgInfo);
	// 3.1.1 사이즈 다르면 
	bgComp->SetWidth(1920); bgComp->SetHeight(1080);
	bgComp->BitmapPush("Background", inGameBackground);

	bgComp->SetCurrentBitmap("Background");

	// 9. 게임 오브젝트들에 집어넣기
	m_gameObjects.emplace("Background", std::move(Background));




	/////////////////////
	/////////////////////
	// 창고

	// 1. 이미지 갖고 오기
	auto 인게임2 = ResourceManager::Get().GetTexture("인게임", "2");
	// 2. 오브젝트 만들기
	auto 창고 = std::make_unique<Object>();
	창고->SetPosition(Vec2(147, 570));
	// 3. 버튼 컴포넌트 만들기
	auto 창고버튼 = 창고->AddComponent<ButtonComponent>();
	창고버튼->SetWidth(310); 창고버튼->SetHeight(64);

	//  4. 버튼 비트맵 설정
	// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
	창고버튼->BitmapPush("normal",  인게임2  );
	창고버튼->BitmapPush("hover",   인게임2  );
	창고버튼->BitmapPush("pressed", 인게임2);

	창고버튼->SetCurrentBitmap("normal");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto 창고리스너 = 창고->AddComponent<MouseListenerComponent>(
		[창고버튼](const MSG& msg) {
			창고버튼->Worked(msg);
		}
	);

	창고버튼->SetOnClickCallback([this]() {
		std::cout << "창고" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("인게임2", std::move(창고));



	/////////////////////
	/////////////////////
	// 장비

	// 1. 이미지 갖고 오기
	auto 인게임3 = ResourceManager::Get().GetTexture("인게임", "3");
	// 2. 오브젝트 만들기
	auto 장비 = std::make_unique<Object>();
	장비->SetPosition(Vec2(147, 703));
	// 3. 버튼 컴포넌트 만들기
	auto 장비버튼 = 장비->AddComponent<ButtonComponent>();
	장비버튼->SetWidth(310); 장비버튼->SetHeight(64);

	//  4. 버튼 비트맵 설정
	// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
	장비버튼->BitmapPush("normal",  인게임3);
	장비버튼->BitmapPush("hover",   인게임3);
	장비버튼->BitmapPush("pressed", 인게임3);

	장비버튼->SetCurrentBitmap("normal");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto 장비리스너 = 장비->AddComponent<MouseListenerComponent>(
		[장비버튼](const MSG& msg) {
			장비버튼->Worked(msg);
		}
	);

	장비버튼->SetOnClickCallback([this]() {
		std::cout << "장비버튼" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("인게임3", std::move(장비));



	/////////////////////
	/////////////////////
	// 스테이터스

	// 1. 이미지 갖고 오기
	auto 인게임4 = ResourceManager::Get().GetTexture("인게임", "4");
	// 2. 오브젝트 만들기
	auto 스테이터스 = std::make_unique<Object>();
	스테이터스->SetPosition(Vec2(147, 836));
	// 3. 버튼 컴포넌트 만들기
	auto 스테이터스버튼 = 스테이터스->AddComponent<ButtonComponent>();
	스테이터스버튼->SetWidth(310); 스테이터스버튼->SetHeight(64);

	//  4. 버튼 비트맵 설정
	// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
	스테이터스버튼->BitmapPush("normal", 인게임4);
	스테이터스버튼->BitmapPush("hover",  인게임4);
	스테이터스버튼->BitmapPush("pressed",인게임4);

	스테이터스버튼->SetCurrentBitmap("normal");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto 스테이터스리스너 = 스테이터스->AddComponent<MouseListenerComponent>(
		[스테이터스버튼](const MSG& msg) {
			스테이터스버튼->Worked(msg);
		}
	);

	스테이터스버튼->SetOnClickCallback([this]() {
		std::cout << "스테이터스버튼이지만 엔딩으로" << std::endl;
		SceneManager::Get().ChangeScene("End");
		});

	/// 9
	m_gameObjects.emplace("인게임4", std::move(스테이터스));



	/////////////////////
	/////////////////////
	// 뒤로가기 (아웃게임으로)

	// 1. 이미지 갖고 오기
	auto 뒤로가기 = ResourceManager::Get().GetTexture("뒤로가기");
	// 2. 오브젝트 만들기
	auto 뒤로 = std::make_unique<Object>();
	뒤로->SetPosition(Vec2(64, 57));
	// 3. 버튼 컴포넌트 만들기
	auto backComp = 뒤로->AddComponent<ButtonComponent>();
	backComp->SetWidth(37); backComp->SetHeight(37);

	//  4. 버튼 비트맵 설정
	// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
	backComp->BitmapPush("normal", 뒤로가기);
	backComp->BitmapPush("hover", 뒤로가기);
	backComp->BitmapPush("pressed", 뒤로가기);

	backComp->SetCurrentBitmap("normal");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto Back_mouseListener = 뒤로->AddComponent<MouseListenerComponent>(
		[backComp](const MSG& msg) {
			backComp->Worked(msg);
		}
	);

	backComp->SetOnClickCallback([this]() {
		std::cout << "버튼 클릭됨 - 현재 씬: " << typeid(*this).name() << std::endl;
		SceneManager::Get().ChangeScene("OutGame");
		//std::cout << "뒤로가기 (아웃게임으로 )" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("뒤로가기", std::move(뒤로));
}