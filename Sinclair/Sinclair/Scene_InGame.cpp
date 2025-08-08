#include "Scene_InGame.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "UIManager.h"

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


	for (const auto& [Name, obj] : m_gameObjects)
	{
		UIManager::Get().AddSceneObject(obj);
	}
}

void Scene_InGame::Enter()
{
	Initalize();



}

void Scene_InGame::Exit()
{

	Clean();
}

void Scene_InGame::Clean()
{
	UIManager::Get().ClearSceneObjects();

	m_gameObjects.clear();

	dirty = false;
}

void Scene_InGame::Update()
{
	// 씬 전환 지연 처리
	if (m_isTransitioning && !m_nextScene.empty())
	{
		m_currentDelay += 0.016f;
		if (m_currentDelay >= m_transitionDelay)
		{
			SceneManager::Get().ChangeScene(m_nextScene);
			m_isTransitioning = false;
			m_nextScene = "";
			m_currentDelay = 0.0f;
		}
	}
}

void Scene_InGame::LogicUpdate(float delta)
{
}

void Scene_InGame::Render()
{

	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetRenderInfo()->GetRenderInfo());
}

}

void Scene_InGame::CreateObj()
{
	//////////////////////
	//////////////////////
	//////////////////////
	// [0] Background 

	// 1. 이미지 갖고 오기
	auto inGameBackground = ResourceManager::Get().GetTexture("인게임");
	// 2. 오브젝트 만들기
	auto Background = std::make_unique<Object>();
	Background->SetPosition(Vec2(0, 0));

	// 3.0. 랜더 인포 컴포넌트
	auto bgInfo = Background->GetRenderInfo();
	bgInfo->SetBitmap(inGameBackground.Get());
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
	/////////////////////
	// [1] 설정 버튼

	// 1. 이미지 갖고 오기
	auto 뒤로가기 = ResourceManager::Get().GetTexture("뒤로가기");
	// 2. 오브젝트 만들기
	auto 뒤로 = std::make_unique<Object>();
	뒤로->SetPosition(Vec2(41, 31));

	auto 뒤로info = 뒤로->GetRenderInfo();
	뒤로info->SetBitmap(뒤로가기.Get());
	// 3. 버튼 컴포넌트 만들기
	auto backComp = 뒤로->AddComponent<ButtonComponent>(뒤로info);
	backComp->SetWidth(뒤로가기->GetSize().width);
	backComp->SetHeight(뒤로가기->GetSize().height);

	//  4. 버튼 비트맵 설정
	backComp->BitmapPush("back", 뒤로가기);

	backComp->SetCurrentBitmap("back");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto Back_mouseListener = 뒤로->AddComponent<MouseListenerComponent>(
		[backComp](const MSG& msg) {
			backComp->Worked(msg);
		}
	);

	backComp->SetOnClickCallback([this]() {
		std::cout << "뒤로가기 버튼 클릭됨 - 현재 씬: " << typeid(*this).name() << std::endl;
		SafeChangeScene("OutGame");
		});

	/// 9
	m_gameObjects.emplace("뒤로가기", std::move(뒤로));




	/////////////////////
	/////////////////////
	/////////////////////
	// [2] 창고

	// 1. 이미지 갖고 오기
	auto 인게임2 = ResourceManager::Get().GetTexture("인게임", "02");
	// 2. 오브젝트 만들기
	auto 창고 = std::make_unique<Object>();
	창고->SetPosition(Vec2(64, 570));

	auto 창고info = 창고->GetRenderInfo();
	창고info->SetBitmap(인게임2.Get());

	// 3. 버튼 컴포넌트 만들기
	auto 창고버튼 = 창고->AddComponent<ButtonComponent>(창고info);

	창고버튼->SetWidth(인게임2->GetSize().width);
	창고버튼->SetHeight(인게임2->GetSize().height);

	//  4. 버튼 비트맵 설정
	// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
	창고버튼->BitmapPush("창고",  인게임2  );

	창고버튼->SetCurrentBitmap("창고");

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
	m_gameObjects.emplace("창고", std::move(창고));



	/////////////////////
	/////////////////////
	/////////////////////
	// [3] 장비

	// 1. 이미지 갖고 오기
	auto 인게임3 = ResourceManager::Get().GetTexture("인게임", "03");
	// 2. 오브젝트 만들기
	auto 장비 = std::make_unique<Object>();
	장비->SetPosition(Vec2(64, 703));

	auto 장비info = 장비->GetRenderInfo();
	장비info->SetBitmap(인게임3.Get());
	// 3. 버튼 컴포넌트 만들기
	auto 장비버튼 = 장비->AddComponent<ButtonComponent>(장비info);
	장비버튼->SetWidth(인게임3->GetSize().width);
	장비버튼->SetHeight(인게임3->GetSize().height);

	//  4. 버튼 비트맵 설정
	// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
	장비버튼->BitmapPush("장비버튼",  인게임3);
	장비버튼->SetCurrentBitmap("장비버튼");

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
	m_gameObjects.emplace("장비", std::move(장비));



	/////////////////////
	/////////////////////
	/////////////////////
	// [4] 스테이터스

	// 1. 이미지 갖고 오기
	auto 인게임4 = ResourceManager::Get().GetTexture("인게임", "04");
	// 2. 오브젝트 만들기
	auto 스테이터스 = std::make_unique<Object>();
	스테이터스->SetPosition(Vec2(64, 836));

	auto 스테이터스info = 스테이터스->GetRenderInfo();
	스테이터스info->SetBitmap(인게임4.Get());
	// 3. 버튼 컴포넌트 만들기
	auto 스테이터스버튼 = 스테이터스->AddComponent<ButtonComponent>(스테이터스info);
	스테이터스버튼->SetWidth(인게임4->GetSize().width);
	스테이터스버튼->SetHeight(인게임4->GetSize().height);

	//  4. 버튼 비트맵 설정
	스테이터스버튼->BitmapPush("스테이터스", 인게임4);

	스테이터스버튼->SetCurrentBitmap("스테이터스");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto 스테이터스리스너 = 스테이터스->AddComponent<MouseListenerComponent>(
		[스테이터스버튼](const MSG& msg) {
			스테이터스버튼->Worked(msg);
		}
	);

	스테이터스버튼->SetOnClickCallback([this]() {
		std::cout << "스테이터스" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("스테이터스", std::move(스테이터스));


	/////////////////////
	/////////////////////
	/////////////////////
	// [5] 강화

	// 1. 이미지 갖고 오기
	auto 인게임5 = ResourceManager::Get().GetTexture("인게임", "05");
	// 2. 오브젝트 만들기
	auto 강화 = std::make_unique<Object>();
	강화->SetPosition(Vec2(589, 537));

	auto 강화info = 강화->GetRenderInfo();
	강화info->SetBitmap(인게임5.Get());
	// 3. 버튼 컴포넌트 만들기
	auto 강화버튼 = 강화->AddComponent<ButtonComponent>(강화info);
	강화버튼->SetWidth(인게임5->GetSize().width);
	강화버튼->SetHeight(인게임5->GetSize().height);

	//  4. 버튼 비트맵 설정
	강화버튼->BitmapPush("강화", 인게임5);

	강화버튼->SetCurrentBitmap("강화");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto 강화리스너 = 강화->AddComponent<MouseListenerComponent>(
		[강화버튼](const MSG& msg) {
		 강화버튼->Worked(msg);
		}
	);

	강화버튼->SetOnClickCallback([this]() {
		std::cout << "강화" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("강화", std::move(강화));


	/////////////////////
	/////////////////////
	/////////////////////
	// [6] 합성

	// 1. 이미지 갖고 오기
	auto 인게임6 = ResourceManager::Get().GetTexture("인게임", "06");
	// 2. 오브젝트 만들기
	auto 합성 = std::make_unique<Object>();
	합성->SetPosition(Vec2(889, 608));

	auto 합성info = 합성->GetRenderInfo();
	합성info->SetBitmap(인게임6.Get());
	// 3. 버튼 컴포넌트 만들기
	auto 합성버튼 = 합성->AddComponent<ButtonComponent>(합성info);
	합성버튼->SetWidth(인게임6->GetSize().width);
	합성버튼->SetHeight(인게임6->GetSize().height);

	//  4. 버튼 비트맵 설정
	합성버튼->BitmapPush("합성", 인게임6);

	합성버튼->SetCurrentBitmap("합성");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto 합성리스너 = 합성->AddComponent<MouseListenerComponent>(
		[합성버튼](const MSG& msg) {
			합성버튼->Worked(msg);
}
	);

	합성버튼->SetOnClickCallback([this]() {
		std::cout << "합성" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("합성", std::move(합성));


	/////////////////////
	/////////////////////
	/////////////////////
	// [7] 설정 버튼

	// 1. 이미지 갖고 오기
	auto 설정 = ResourceManager::Get().GetTexture("설정");
	// 2. 오브젝트 만들기
	auto 설정로 = std::make_unique<Object>();
	설정로->SetPosition(Vec2(41, 106));

	auto 설정info = 설정로->GetRenderInfo();
	설정info->SetBitmap(설정.Get());
	// 3. 버튼 컴포넌트 만들기
	auto settingComp = 설정로->AddComponent<ButtonComponent>(뒤로info);
	settingComp->SetWidth(설정->GetSize().width);
	settingComp->SetHeight(설정->GetSize().height);

	//  4. 버튼 비트맵 설정
	settingComp->BitmapPush("setting", 설정);

	settingComp->SetCurrentBitmap("setting");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto set_mouseListener = 설정로->AddComponent<MouseListenerComponent>(
		[settingComp](const MSG& msg) {
			settingComp->Worked(msg);
		}
	);

	settingComp->SetOnClickCallback([this]() {
		std::cout << "설정 버튼 클릭됨 - 현재 씬: " << typeid(*this).name() << std::endl;
		});

	/// 9
	m_gameObjects.emplace("설정", std::move(설정로));

}
