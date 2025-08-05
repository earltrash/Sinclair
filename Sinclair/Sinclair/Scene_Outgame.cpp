#include "Scene_Outgame.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"

// 랜덤
#include <random>

Scene_Outgame::Scene_Outgame(string name)
{
		m_name = name;
		Initalize();
}

Scene_Outgame::~Scene_Outgame()
{
	Clean();
	
}

void Scene_Outgame::Initalize()
{
	if (dirty) return; // 이미 초기화되었으면 스킵

	CreateObj(); // 오브젝트 생성 (한 번만)
	dirty = true;
}

void Scene_Outgame::Clean()
{
	m_gameObjects.clear();
	//SceneAssets.clear(); // 클립에셋을 쓰고 있지 않음
	dirty = false;
}

void Scene_Outgame::Update()
{
}

void Scene_Outgame::LogicUpdate(float delta)
{
}

void Scene_Outgame::Enter()
{
	auto bg = m_gameObjects["Background"s]->GetComponent<BackgroundComponent>();
	bg->SetCurrentBitmap("Background");

	// 게임매니저가 아직 없으므로 임시로 싱클레어 n세 랜덤
	auto ch = m_gameObjects["Character"s]->GetComponent<BackgroundComponent>();
	
	std::vector<int> numbers = { 2, 3, 4 };
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, numbers.size() - 1);

	std::string result = "Sinclair";
	result += std::to_string(numbers[dis(gen)]);
	//ch->SetCurrentBitmap("Sinclair3");
	ch->SetCurrentBitmap(result);
}

void Scene_Outgame::Exit()
{
		
}

void Scene_Outgame::Render()
{
	// 멀티맵은 키(Name)기준으로 정렬 된다. 
	// 즉 background 배경을 먼저 그리게 됨
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
				bitmap = bgComp->GetCurrentBitmap();
				dest.right += bgComp->GetWidth();
				dest.bottom += bgComp->GetHeight();
			}
		}

		if (!bitmap) {
			continue;
		}

		//dest.left += obj->GetTransform().m_position.x;
		//dest.top += obj->GetTransform().m_position.y;
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

	D2DRenderer::Get().CreateWriteRegularResource();
	D2DRenderer::Get().DrawMessage(L"싱클레어 N세 ", 503.f, 805.f, 1300.f, 1000.f, D2D1::ColorF::BlueViolet);
	D2DRenderer::Get().DrawMessage(L"창고로 이동할까?\n(수락 시 창고로 이동합니다.)", 503.f, 869.f, 1300.f, 1000.f, D2D1::ColorF::BlueViolet);

	D2DRenderer::Get().RenderEnd();
}

void Scene_Outgame::CreateObj()
{
	//////////////////////
	// Background 

	// 1. 이미지 갖고 오기
	auto outGameBackground = ResourceManager::Get().GetTexture("아웃게임");
	// 2. 오브젝트 만들기
	auto Background = std::make_unique<Object>();
	Background->SetPosition(Vec2(0, 0));

	// 3.0. 랜더 인포 컴포넌트
	auto bgInfo = Background->AddComponent<RenderInfo>(outGameBackground.Get());

	// 3. 배경 컴포넌트 만들기
	auto bgComp = Background->AddComponent<BackgroundComponent>(bgInfo);
	// 3.1.1 사이즈 다르면 
	bgComp->SetWidth(1920); bgComp->SetHeight(1080);
	bgComp->BitmapPush("Background", outGameBackground);
	// 9. 게임 오브젝트들에 집어넣기
	m_gameObjects.emplace("Background", std::move(Background));



	//////////////////////
	//////////////////////
	// 캐릭터 이미지 : 배경 컴포넌트라고 명명 했지만 -> 이미지 컴포넌트라고 바꿔야 해. 
	// 나는 여기다가 셰이더? 넣어야 된다고 생각해. 
	// 캐릭터에 올려 놓으면 바뀌어야지. -> 그럼 버튼 컴포넌트로 바꿔야 하는 거 아닌가?

	// 1. 이미지 갖고 오기
	auto Sinclair2 = ResourceManager::Get().GetTexture("싱클레어", "2세");
	auto Sinclair3 = ResourceManager::Get().GetTexture("싱클레어", "3세");
	auto Sinclair4 = ResourceManager::Get().GetTexture("싱클레어", "4세");
	// 2. 오브젝트 만들기
	auto Character = std::make_unique<Object>();
	Character->SetPosition(Vec2(0, 0));

	// 3.0. 랜더 인포 컴포넌트
	auto chInfo = Character->AddComponent<RenderInfo>(Sinclair2.Get());
	// 3. 배경 컴포넌트 만들기
	auto chComp = Character->AddComponent<BackgroundComponent>(chInfo);

	// 3.1.1 사이즈 다르면 
	chComp->SetWidth(1920); chComp->SetHeight(1080);
	chComp->BitmapPush("Sinclair2", Sinclair2);
	chComp->BitmapPush("Sinclair3", Sinclair3);
	chComp->BitmapPush("Sinclair4", Sinclair4);
	// 9. 게임 오브젝트들에 집어넣기 
	m_gameObjects.emplace("Character", std::move(Character));
	

	//////////////////////
	//////////////////////
	// 텍스트 박스 
	
	// 1. 이미지 갖고 오기
	auto 아웃게임1 = ResourceManager::Get().GetTexture("아웃게임", "1");
	// 2. 오브젝트 만들기
	auto 텍스트박스 = std::make_unique<Object>();
	     텍스트박스->SetPosition(Vec2(455, 772));

	// 3.0. 랜더 인포 컴포넌트
	auto outgame1Info = 텍스트박스->AddComponent<RenderInfo>(아웃게임1.Get());
	// 3. 배경 컴포넌트 만들기
	auto 텍스트박스Comp = 텍스트박스->AddComponent<BackgroundComponent>(outgame1Info);
	// 3.1.1 사이즈 다르면 
	텍스트박스Comp->SetWidth(1010); 텍스트박스Comp->SetHeight(207);
	텍스트박스Comp->BitmapPush("아웃게임1", 아웃게임1);
	텍스트박스Comp->SetCurrentBitmap("아웃게임1");
	// 9. 게임 오브젝트들에 집어넣기 
	m_gameObjects.emplace("아웃게임1", std::move(텍스트박스));


	/////////////////////
	/////////////////////
	// 예 (수락 버튼)

	// 1. 이미지 갖고 오기
	auto 아웃게임4 = ResourceManager::Get().GetTexture("아웃게임", "4");
	// 2. 오브젝트 만들기
	auto 예 = std::make_unique<Object>();
	예->SetPosition(Vec2(1087, 921));
	// 3. 버튼 컴포넌트 만들기
	auto YesComp = 예->AddComponent<ButtonComponent>();
	YesComp->SetWidth(140); YesComp->SetHeight(40);

	//  4. 버튼 비트맵 설정
	// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
	YesComp->BitmapPush("normal",  아웃게임4);
	YesComp->BitmapPush("hover",   아웃게임4);
	YesComp->BitmapPush("pressed", 아웃게임4);

	YesComp->SetCurrentBitmap("normal");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto Yes_mouseListener = 예->AddComponent<MouseListenerComponent>(
		[YesComp](const MSG& msg) {
			YesComp->Worked(msg);
		}
	);

	YesComp->SetOnClickCallback([this]() {
		SceneManager::Get().ChangeScene("InGame");
		std::cout << "SceneManager::Get().ChangeScene(\"InGame\");" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("아웃게임4", std::move(예));


	/////////////////////
	// 아니오 (수락 버튼)

	// 1. 이미지 갖고 오기
	auto 아웃게임5 = ResourceManager::Get().GetTexture("아웃게임", "5");
	// 2. 오브젝트 만들기
	auto 아니오 = std::make_unique<Object>();
	아니오->SetPosition(Vec2(1280, 921));
	// 3. 버튼 컴포넌트 만들기
	auto NoComp = 아니오->AddComponent<ButtonComponent>();
	NoComp->SetWidth(140); NoComp->SetHeight(40);

	//  4. 버튼 비트맵 설정
	// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
	NoComp->BitmapPush("normal",  아웃게임5);
	NoComp->BitmapPush("hover",   아웃게임5);
	NoComp->BitmapPush("pressed", 아웃게임5);

	NoComp->SetCurrentBitmap("normal");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto No_mouseListener = 아니오->AddComponent<MouseListenerComponent>(
		[NoComp](const MSG& msg) {
			NoComp->Worked(msg);
		}
	);

	NoComp->SetOnClickCallback([this]() {
		//SceneManager::Get().ChangeScene("InGame");
		std::cout << "아니오 (수락 버튼)" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("아웃게임5", std::move(아니오));


	/////////////////////
	// 뒤로가기 (시작화면으로)

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
		SceneManager::Get().ChangeScene("Title");
		std::cout << "뒤로가기 (시작화면으로)" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("뒤로가기", std::move(뒤로));

}