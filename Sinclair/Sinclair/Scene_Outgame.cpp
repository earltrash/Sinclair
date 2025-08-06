#include "Scene_Outgame.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "Renderer.h"

// 랜덤
#include <random>
#include <iterator>

Scene_Outgame::Scene_Outgame(string name)
{
	m_name = name;
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
	Initalize();
	SetupCharacterAndBackground();

	// InGame 씬에 있다가 돌아온 경우, 바로 선택지(80002) 상태로 갑니다.
	// GameManager에 플래그를 하나 만들어 관리하면 좋습니다.
	// if (GameManager::Get().WasInGame()) {
	//     ChangeState(선택지);
	// } else {
	//     ChangeState(처음아웃게임);
	// }


	//// 객체 뭐있나, 디버기용
	//for (const auto& [Name, obj] : m_gameObjects)
	//{
	//	cout << Name << endl;
	//}

	// 우선 초기 상태로 진입
	ChangeState(m_id);
}

void Scene_Outgame::Exit()
{
	Clean();
}

void Scene_Outgame::Render()
{
	// 멀티맵은 키(Name)기준으로 정렬 된다. 
	// 즉 background 배경을 먼저 그리게 됨
	
	//D2DRenderer::Get().RenderBegin();
	//for (const auto& [Name, obj] : m_gameObjects)
	//{

	//	ComPtr<ID2D1Bitmap1> bitmap = nullptr;
	//	D2D1_RECT_F dest;
	//	dest.left = 0;
	//	dest.top = 0;
	//	dest.right = 0;
	//	dest.bottom = 0;
	//	// 투명 구현한다면...
	//	float opacity = 1.0f;

	//	// ButtonComponent 우선 확인
	//	auto buttonComp = obj->GetComponent<ButtonComponent>();
	//	if (buttonComp != nullptr) {
	//		bitmap = buttonComp->GetBitmap();
	//		dest.right += buttonComp->GetWidth();
	//		dest.bottom += buttonComp->GetHeight();
	//		opacity = buttonComp->m_opacity;
	//	}
	//	else {
	//		// ButtonComponent가 없으면 BackgroundComponent 확인
	//		auto bgComp = obj->GetComponent<BackgroundComponent>();
	//		if (bgComp != nullptr) {
	//			bitmap = bgComp->GetCurrentBitmap();
	//			dest.right += bgComp->GetWidth();
	//			dest.bottom += bgComp->GetHeight();
	//		}
	//	}

	//	if (!bitmap) {
	//		continue;
	//	}

	//	//dest.left += obj->GetTransform().m_position.x;
	//	//dest.top += obj->GetTransform().m_position.y;
	//	dest.left += obj->GetTransform().GetPosition().x;
	//	dest.top += obj->GetTransform().GetPosition().y;
	//	dest.right += dest.left;
	//	dest.bottom += dest.top;

	//	// 투명도 적용 x
	//	//D2DRenderer::Get().DrawBitmap(bitmap.Get(), dest);
	//	// 투명도 적용 o
	//	D2D1_RECT_F srcRect = D2D1::RectF(0, 0, bitmap->GetSize().width, bitmap->GetSize().height); ;

	//	D2DRenderer::Get().DrawBitmap(bitmap.Get(), dest, srcRect, opacity);
	//}
	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetRenderInfo()->GetRenderInfo());
	}
	D2DRenderer::Get().CreateWriteRegularResource();
	std::wstring characterName = L"싱클레어 " + std::to_wstring(GameManager::Get().curGen) + L"세";
	D2DRenderer::Get().DrawMessage(characterName.c_str(), 503.f, 805.f, 1300.f, 1000.f, D2D1::ColorF::BlueViolet);

	// 상태에 따른 메인 텍스트 렌더링
	if (!curText.empty())
	{
		std::wstring wCurText = StrToWstr(curText);
		D2DRenderer::Get().DrawMessage(wCurText.c_str(), 503.f, 869.f, 1300.f, 1000.f, D2D1::ColorF::Black);
	}

	D2DRenderer::Get().CreateWriteBoldResource();

	
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
	auto bgInfo = Background->GetRenderInfo();
	bgInfo->SetBitmap(outGameBackground.Get());

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
	auto chInfo = Character->GetRenderInfo();
	chInfo->SetBitmap(Sinclair2.Get());
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
		 auto outgame1Info = 텍스트박스->GetRenderInfo();
		 outgame1Info->SetBitmap(아웃게임1.Get());
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
	/////////////////////
	// 예 (수락 버튼)

	// 1. 이미지 갖고 오기
	auto 아웃게임4 = ResourceManager::Get().GetTexture("아웃게임", "4");
	// 2. 오브젝트 만들기
	auto 예 = std::make_unique<Object>();
	예->SetPosition(Vec2(1087, 921));

	auto 예info = 예->GetRenderInfo();
	예info->SetBitmap(아웃게임4.Get());
	// 3. 버튼 컴포넌트 만들기
	auto YesComp = 예->AddComponent<ButtonComponent>(예info);
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
	/////////////////////
	/////////////////////
	// 아니오 (수락 버튼)

	// 1. 이미지 갖고 오기
	auto 아웃게임5 = ResourceManager::Get().GetTexture("아웃게임", "5");
	// 2. 오브젝트 만들기
	auto 아니오 = std::make_unique<Object>();
	아니오->SetPosition(Vec2(1280, 921));

	auto 아니오info = 아니오->GetRenderInfo();
	아니오info->SetBitmap(아웃게임5.Get());
	// 3. 버튼 컴포넌트 만들기
	auto NoComp = 아니오->AddComponent<ButtonComponent>(아니오info);
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
	/////////////////////
	// 뒤로가기 (시작화면으로)

	// 1. 이미지 갖고 오기
	auto 뒤로가기 = ResourceManager::Get().GetTexture("뒤로가기");
	// 2. 오브젝트 만들기
	auto 뒤로 = std::make_unique<Object>();
	뒤로->SetPosition(Vec2(64, 57));

	auto 뒤로info = 뒤로->GetRenderInfo();
	뒤로info->SetBitmap(뒤로가기.Get());
	// 3. 버튼 컴포넌트 만들기
	auto backComp = 뒤로->AddComponent<ButtonComponent>(뒤로info);
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
		SceneManager::Get().ChangeScene("Title");
		//std::cout << "뒤로가기 (시작화면으로)" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("뒤로가기", std::move(뒤로));

}

std::string Scene_Outgame::getRandomText()
{
	auto range = outGameTextTable.equal_range(m_id);

	if (range.first == range.second) {
		return "텍스트 없음"; // ID가 없는 경우
	}

	// 해당 ID의 모든 텍스트를 벡터로 복사
	std::vector<std::string> texts;
	for (auto it = range.first; it != range.second; ++it) {
		texts.push_back(it->second);
	}

	// 랜덤 선택
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, texts.size() - 1);

	return texts[dist(gen)];
}

void Scene_Outgame::SetupCharacterAndBackground()
{	// 배경은 안바뀜
	auto bg = m_gameObjects["Background"s]->GetComponent<BackgroundComponent>();
	bg->SetCurrentBitmap("Background");

	// 키로 이미지 설정 하는 과정 "Sinclair + GameManager::Get().curGen" = Sinclair3? 2? 4?
	auto ch = m_gameObjects["Character"s]->GetComponent<BackgroundComponent>();
	std::string result = "Sinclair";
	result += std::to_string(GameManager::Get().curGen/*얘가 세대를 나타내는 숫자임*/);
	ch->SetCurrentBitmap(result);
}

void Scene_Outgame::ChangeState(State newState)
{
	m_id = newState;

	// 모든 버튼을 일단 숨깁니다.
	// (이 기능을 구현하려면 ButtonComponent에 Show/Hide 기능이 필요합니다)
	// 예: m_gameObjects["예"]->GetComponent<ButtonComponent>()->Hide();
	// 예: m_gameObjects["아니오"]->GetComponent<ButtonComponent>()->Hide();
	// 예: m_gameObjects["다음버튼"]->GetComponent<ButtonComponent>()->Hide(); // '다음' 버튼 추가 필요

	// 버튼 콜백을 가져오기 위한 준비
	auto yesButton = m_gameObjects["아웃게임4"]->GetComponent<ButtonComponent>();
	auto noButton = m_gameObjects["아웃게임5"]->GetComponent<ButtonComponent>();
	// '다음' 또는 선택지 버튼들도 마찬가지로 가져옵니다.

	switch (m_id)
	{
	case 처음아웃게임:
	{
		// 텍스트 설정 (80001 상태에는 텍스트가 여러 개이므로, 순차적으로 보여줄 장치가 필요)
		// 예를 들어, m_dialogueIndex 같은 멤버 변수를 추가하여 관리할 수 있습니다.
		curText = getRandomText();

		// '다음' 버튼을 보여주고 '예/아니오' 버튼은 숨깁니다.
		// '다음' 버튼의 콜백을 설정하여, 다음 텍스트를 보여주거나 상태를 80002로 변경하도록 합니다.
		// nextButton->SetOnClickCallback([this]() {
		//      // 다음 텍스트 로직 or ChangeState(선택지);
		// });
		break;
	}

	case 선택지:
	{
		curText = outGameTextTable.find(m_id)->second; // "> 창고로 이동한다.\n> 모험을 떠난다."

		// '창고', '모험' 버튼을 보여주고 콜백을 설정해야 합니다.
		// 현재는 '예/아니오' 버튼만 있으므로, 이들을 재활용하거나 새로 만들어야 합니다.
		// 예시: '예' 버튼을 '창고' 버튼으로 사용
		yesButton->SetOnClickCallback([this]() {
			// 3, 4세대는 모험을 떠날 수 없다는 등의 조건은 여기서 GameManager::Get().curGen으로 확인 가능
			ChangeState(창고에들어갈때);
			});

		// 예시: '아니오' 버튼을 '모험' 버튼으로 사용
		noButton->SetOnClickCallback([this]() {
			ChangeState(모험떠나기클릭시);
			});
		break;
	}

	case 창고에들어갈때:
	{
		curText = outGameTextTable.find(m_id)->second; // "창고에 들어가시겠습니까?..."

		// '예', '아니오' 버튼을 보여줍니다.
		// m_gameObjects["아웃게임4"]->GetComponent<ButtonComponent>()->Show();
		// m_gameObjects["아웃게임5"]->GetComponent<ButtonComponent>()->Show();

		yesButton->SetOnClickCallback([this]() {
			SceneManager::Get().ChangeScene("InGame");
			});

		noButton->SetOnClickCallback([this]() {
			ChangeState(선택지); // 선택지로 돌아가기
			});
		break;
	}

	case 모험떠나기클릭시:
	{
		curText = outGameTextTable.find(m_id)->second; // "여행을 떠나시겠습니까?..."

		// '예', '아니오' 버튼을 보여줍니다.
		 // m_gameObjects["아웃게임4"]->GetComponent<ButtonComponent>()->Show();
		 // m_gameObjects["아웃게임5"]->GetComponent<ButtonComponent>()->Show();

		yesButton->SetOnClickCallback([this]() {
			// 엔딩 씬으로 전환
			SceneManager::Get().ChangeScene("Ending"); // "Ending" 씬이 있다고 가정
			});

		noButton->SetOnClickCallback([this]() {
			ChangeState(선택지); // 선택지로 돌아가기
			});
		break;
	}
	}
}

