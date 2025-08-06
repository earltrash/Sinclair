#include "Scene_End.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"

Scene_End::Scene_End(string name)
{
	m_name = name;
}

Scene_End::~Scene_End()
{

}

void Scene_End::Initalize()
{
	if (dirty) return; // 이미 초기화되었으면 스킵

	CreateObj(); // 오브젝트 생성 (한 번만)
	dirty = true;
}

void Scene_End::Enter()
{

	Initalize();
}

void Scene_End::Exit()
{

	Clean();
}

void Scene_End::Clean()
{
	m_gameObjects.clear();
}

void Scene_End::Update()
{

}

void Scene_End::LogicUpdate(float delta)
{


}


void Scene_End::Render()
{

}

void Scene_End::CreateObj()
{
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
	장비버튼->BitmapPush("normal", 인게임3);
	장비버튼->BitmapPush("hover", 인게임3);
	장비버튼->BitmapPush("pressed", 인게임3);

	장비버튼->SetCurrentBitmap("normal");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto 장비리스너 = 장비->AddComponent<MouseListenerComponent>(
		[장비버튼](const MSG& msg) {
			장비버튼->Worked(msg);
		}
	);

	장비버튼->SetOnClickCallback([this]() {
		std::cout << "장비버튼이지만 히스토리로" << std::endl;
		SceneManager::Get().ChangeScene("History");
		});

	/// 9
	m_gameObjects.emplace("인게임3", std::move(장비));
}