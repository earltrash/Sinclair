#include "Scene_History.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "UIManager.h"
#include "GameManager.h"


Scene_History::Scene_History(string name)
{
	m_name = name;
}

Scene_History::~Scene_History()
{

}

void Scene_History::Initalize()
{
	if (dirty) return; // 이미 초기화되었으면 스킵

	CreateObj(); // 오브젝트 생성 (한 번만)
	dirty = true;
	for (const auto& [Name, obj] : m_gameObjects)
	{
		UIManager::Get().AddSceneObject(obj);
	}
}

void Scene_History::Enter()
{
	//여기서 히스토리 계산
	int resultFame = GameManager::Get().GetResultFam();
	if (resultFame >= 19)
	{
		m_History = History::H1;
		m_targetTextCount = 5;
	}
	else if (resultFame >= 15)
	{
		m_History = History::H2;
		m_targetTextCount = 6;
	}
	else if (resultFame >= 10)
	{
		m_History = History::H3;
		m_targetTextCount = 5;
	}
	else
	{
		m_History = History::H4;
		m_targetTextCount = 5;
	}

	// 텍스트 표시 관련 초기화
	m_allTextsShown = true;  
	m_currentShowingDelay = 0.f;

	Initalize();

}

void Scene_History::Exit()
{
	DeactivateAllTexts();
	Clean();
}

void Scene_History::Clean()
{
	if (!dirty) return;

	UIManager::Get().ClearSceneObjects();
	m_gameObjects.clear();

	dirty = false;
}

void Scene_History::Update()
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


	// 순차적 텍스트 표시
	if (m_allTextsShown && index < m_targetTextCount)
	{
		m_currentShowingDelay += 0.016f;
		if (m_currentShowingDelay >= m_showingDelay)
		{
			ActivateSingleText(index + 1);
			index++;
			m_currentShowingDelay = 0.f; // 다음 텍스트 타이머 리셋
		}
	}
}

void Scene_History::LogicUpdate(float delta)
{


}


void Scene_History::Render()
{
	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetRenderInfo()->GetRenderInfo());
	}

}

void Scene_History::CreateObj()
{
	//////////////////////
	//////////////////////
	//////////////////////
	// [1] 좌측 창

	// 1. 이미지 갖고 오기
	auto History1 = ResourceManager::Get().GetTexture("엔딩", "01");
	// 2. 오브젝트 만들기
	auto 좌측창 = std::make_unique<Object>();
	//좌측창->SetPosition(Vec2(-20, -16));
	좌측창->SetPosition(Vec2(0, 0));

	// 3.0. 랜더 인포 컴포넌트
	auto 좌측창info = 좌측창->GetRenderInfo();
	좌측창info->SetBitmap(History1.Get());
	// 3. 배경 컴포넌트 만들기
	auto 좌측창Comp = 좌측창->AddComponent<BackgroundComponent>(좌측창info);
	좌측창Comp->SetWidth(1098);
	좌측창Comp->SetHeight(1080);
	좌측창Comp->BitmapPush("좌측창", History1);
	좌측창Comp->SetCurrentBitmap("좌측창");

	/// 9
	m_gameObjects.emplace("aa좌측창", std::move(좌측창));

	//////////////////////
	//////////////////////
	//////////////////////
	// [2] 이미지
	// 효제 : 이거 어떻게 해야할지 고민하기

	// 1. 이미지 갖고 오기
	
	
	ComPtr<ID2D1Bitmap1> History2;
	if(m_History == History::H1)
	{
		History2 = ResourceManager::Get().GetTexture("H1");
	}
	else if (m_History == History::H2)
	{
		History2 = ResourceManager::Get().GetTexture("H2");
	}
	else if (m_History == History::H3)
	{
		History2 = ResourceManager::Get().GetTexture("H3");
	}
	else 
	{
		History2 = ResourceManager::Get().GetTexture("H4");
	}
	// 2. 오브젝트 만들기
	auto 이미지 = std::make_unique<Object>();
	이미지->SetPosition(Vec2(17, 22));

	auto 이미지info = 이미지->GetRenderInfo();
	이미지info->SetBitmap(History2.Get());
	// 3. 배경 컴포넌트 만들기
	auto 이미지Comp = 이미지->AddComponent<BackgroundComponent>(이미지info);
	이미지Comp->SetWidth(1036);
	이미지Comp->SetHeight(1036);
	이미지Comp->BitmapPush("이미지", History2);
	이미지Comp->SetCurrentBitmap("이미지");


	/// 9
	m_gameObjects.emplace("이미지", std::move(이미지));

	//////////////////////
	//////////////////////
	//////////////////////
	// [3] 우측 창

	// 1. 이미지 갖고 오기
	auto History3 = ResourceManager::Get().GetTexture("엔딩", "03");
	// 2. 오브젝트 만들기
	auto 우측창 = std::make_unique<Object>();
	우측창->SetPosition(Vec2(1080, 0));

	auto 우측창info = 우측창->GetRenderInfo();
	우측창info->SetBitmap(History3.Get());
	// 3. 배경 컴포넌트 만들기
	auto 우측창Comp = 우측창->AddComponent<BackgroundComponent>(우측창info);
	우측창Comp->SetWidth(843);
	우측창Comp->SetHeight(1080);
	우측창Comp->BitmapPush("우측창", History3);
	우측창Comp->SetCurrentBitmap("우측창");

	/// 9
	m_gameObjects.emplace("0a우측창", std::move(우측창));

	//////////////////////
	//////////////////////
	//////////////////////
	// [4] 스킵 버튼

	// 1. 이미지 갖고 오기
	auto History4 = ResourceManager::Get().GetTexture("엔딩", "04");
	// 2. 오브젝트 만들기
	auto 스킵버튼 = std::make_unique<Object>();
	스킵버튼->SetPosition(Vec2(1827, 960));

	auto 스킵버튼info = 스킵버튼->GetRenderInfo();
	스킵버튼info->SetBitmap(History4.Get());

	// 3. 버튼 컴포넌트 만들기
	auto 스킵컴포넌트 = 스킵버튼->AddComponent<ButtonComponent>(스킵버튼info);
	스킵컴포넌트->SetWidth(History4->GetSize().width);
	스킵컴포넌트->SetHeight(History4->GetSize().height);

	// 4. 버튼 비트맵 설정
	스킵컴포넌트->BitmapPush("스킵", History4);
	스킵컴포넌트->SetCurrentBitmap("스킵");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto 스킵리스너 = 스킵버튼->AddComponent<MouseListenerComponent>(
		[스킵컴포넌트](const MSG& msg) {
			스킵컴포넌트->CheckCollision(msg);
			스킵컴포넌트->Worked(msg);
		}
	);

	스킵컴포넌트->SetOnClickCallback([this]() {
		std::cout << "스킵 버튼 클릭됨" << std::endl;
		// History 스킵 로직 추가

		SafeChangeScene("Title");
		});

	/// 9
	m_gameObjects.emplace("스킵버튼", std::move(스킵버튼));


	//////////////////////
	//////////////////////
	//////////////////////
	// [6] 바

	// 1. 이미지 갖고 오기
	auto History6 = ResourceManager::Get().GetTexture("엔딩", "06");
	// 2. 오브젝트 만들기
	auto 바 = std::make_unique<Object>();
	바->SetPosition(Vec2(1223, 255));

	auto 바info = 바->GetRenderInfo();
	바info->SetBitmap(History6.Get());
	// 3. 배경 컴포넌트 만들기
	auto 바Comp = 바->AddComponent<BackgroundComponent>(바info);
	바Comp->SetWidth(History6->GetSize().width);
	바Comp->SetHeight(History6->GetSize().height);
	바Comp->BitmapPush("바", History6);
	바Comp->SetCurrentBitmap("바");

	/// 9
	m_gameObjects.emplace("바", std::move(바));


	//////////////////////
	// 1.투명 이미지 갖고 오기
	// 순차적으로 그리기 위한
	auto transparentImg = ResourceManager::Get().GetTexture("transparent");


	switch (m_History)
	{
	case History::H1:
	{
		//////////////////////
		//////////////////////
		//////////////////////
		// h101 
		
		// 1. 이미지 갖고 오기
		auto h101Img = ResourceManager::Get().GetTexture("H1", "01");

		// 2. 오브젝트 만들기
		auto h101Obj = std::make_unique<Object>();
		h101Obj->SetPosition(Vec2(1264, 142));

		// 3.0. 랜더 인포 컴포넌트
		auto h101Info = h101Obj->GetRenderInfo();
		h101Info->SetBitmap(h101Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h101Comp = h101Obj->AddComponent<BackgroundComponent>(h101Info);

		// 3.1.1 사이즈 다르면 
		h101Comp->SetWidth(1264); h101Comp->SetHeight(142);
		h101Comp->BitmapPush("Background", h101Img);
		h101Comp->BitmapPush("transparent", transparentImg);
		h101Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("1", std::move(h101Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h102 

		// 1. 이미지 갖고 오기
		auto h102Img = ResourceManager::Get().GetTexture("H1", "02");

		// 2. 오브젝트 만들기
		auto h102Obj = std::make_unique<Object>();
		h102Obj->SetPosition(Vec2(1220,	379));

		// 3.0. 랜더 인포 컴포넌트
		auto h102Info = h102Obj->GetRenderInfo();
		h102Info->SetBitmap(h102Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h102Comp = h102Obj->AddComponent<BackgroundComponent>(h102Info);

		// 3.1.1 사이즈 다르면 
		h102Comp->SetWidth(560 ); h102Comp->SetHeight(31);
		h102Comp->BitmapPush("Background", h102Img);
		h102Comp->BitmapPush("transparent", transparentImg);
		h102Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("2", std::move(h102Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h103 

		// 1. 이미지 갖고 오기
		auto h103Img = ResourceManager::Get().GetTexture("H1", "03");

		// 2. 오브젝트 만들기
		auto h103Obj = std::make_unique<Object>();
		h103Obj->SetPosition(Vec2(1306,	475));

		// 3.0. 랜더 인포 컴포넌트
		auto h103Info = h103Obj->GetRenderInfo();
		h103Info->SetBitmap(h103Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h103Comp = h103Obj->AddComponent<BackgroundComponent>(h103Info);

		// 3.1.1 사이즈 다르면 
		h103Comp->SetWidth(388 ); h103Comp->SetHeight(80);
		h103Comp->BitmapPush("Background", h103Img);
		h103Comp->BitmapPush("transparent", transparentImg);
		h103Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("3", std::move(h103Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h104 

		// 1. 이미지 갖고 오기
		auto h104Img = ResourceManager::Get().GetTexture("H1", "04");

		// 2. 오브젝트 만들기
		auto h104Obj = std::make_unique<Object>();
		h104Obj->SetPosition(Vec2(1206, 619));

		// 3.0. 랜더 인포 컴포넌트
		auto h104Info = h104Obj->GetRenderInfo();
		h104Info->SetBitmap(h104Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h104Comp = h104Obj->AddComponent<BackgroundComponent>(h104Info);

		// 3.1.1 사이즈 다르면 
		h104Comp->SetWidth(588); h104Comp->SetHeight(127);
		h104Comp->BitmapPush("Background", h104Img);
		h104Comp->BitmapPush("transparent", transparentImg);
		h104Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("4", std::move(h104Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h105 

		// 1. 이미지 갖고 오기
		auto h105Img = ResourceManager::Get().GetTexture("H1", "05");

		// 2. 오브젝트 만들기
		auto h105Obj = std::make_unique<Object>();
		h105Obj->SetPosition(Vec2(1259, 811));

		// 3.0. 랜더 인포 컴포넌트
		auto h105Info = h105Obj->GetRenderInfo();
		h105Info->SetBitmap(h105Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h105Comp = h105Obj->AddComponent<BackgroundComponent>(h105Info);

		// 3.1.1 사이즈 다르면 
		h105Comp->SetWidth(481); h105Comp->SetHeight(79);
		h105Comp->BitmapPush("Background", h105Img);
		h105Comp->BitmapPush("transparent", transparentImg);
		h105Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("5", std::move(h105Obj));
	}
	break;
	case History::H2:
	{
		//////////////////////
		//////////////////////
		//////////////////////
		// h201 

		// 1. 이미지 갖고 오기
		auto h201Img = ResourceManager::Get().GetTexture("H2", "01");

		// 2. 오브젝트 만들기
		auto h201Obj = std::make_unique<Object>();
		h201Obj->SetPosition(Vec2(1217, 141));

		// 3.0. 랜더 인포 컴포넌트
		auto h201Info = h201Obj->GetRenderInfo();
		h201Info->SetBitmap(h201Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h201Comp = h201Obj->AddComponent<BackgroundComponent>(h201Info);

		// 3.1.1 사이즈 다르면 
		h201Comp->SetWidth(559); h201Comp->SetHeight(49);
		h201Comp->BitmapPush("Background", h201Img);
		h201Comp->BitmapPush("transparent", transparentImg);
		h201Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("1", std::move(h201Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h202 

		// 1. 이미지 갖고 오기
		auto h202Img = ResourceManager::Get().GetTexture("H2", "02");

		// 2. 오브젝트 만들기
		auto h202Obj = std::make_unique<Object>();
		h202Obj->SetPosition(Vec2(1290, 340));

		// 3.0. 랜더 인포 컴포넌트
		auto h202Info = h202Obj->GetRenderInfo();
		h202Info->SetBitmap(h202Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h202Comp = h202Obj->AddComponent<BackgroundComponent>(h202Info);

		// 3.1.1 사이즈 다르면 
		h202Comp->SetWidth(417); h202Comp->SetHeight(32);
		h202Comp->BitmapPush("Background", h202Img);
		h202Comp->BitmapPush("transparent", transparentImg);
		h202Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("2", std::move(h202Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h203 

		// 1. 이미지 갖고 오기
		auto h203Img = ResourceManager::Get().GetTexture("H2", "03");

		// 2. 오브젝트 만들기
		auto h203Obj = std::make_unique<Object>();
		h203Obj->SetPosition(Vec2(1252, 436));

		// 3.0. 랜더 인포 컴포넌트
		auto h203Info = h203Obj->GetRenderInfo();
		h203Info->SetBitmap(h203Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h203Comp = h203Obj->AddComponent<BackgroundComponent>(h203Info);

		// 3.1.1 사이즈 다르면 
		h203Comp->SetWidth(494); h203Comp->SetHeight(79);
		h203Comp->BitmapPush("Background", h203Img);
		h203Comp->BitmapPush("transparent", transparentImg);
		h203Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("3", std::move(h203Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h204 

		// 1. 이미지 갖고 오기
		auto h204Img = ResourceManager::Get().GetTexture("H2", "04");

		// 2. 오브젝트 만들기
		auto h204Obj = std::make_unique<Object>();
		h204Obj->SetPosition(Vec2(1194, 581));

		// 3.0. 랜더 인포 컴포넌트
		auto h204Info = h204Obj->GetRenderInfo();
		h204Info->SetBitmap(h204Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h204Comp = h204Obj->AddComponent<BackgroundComponent>(h204Info);

		// 3.1.1 사이즈 다르면 
		h204Comp->SetWidth(611); h204Comp->SetHeight(127);
		h204Comp->BitmapPush("Background", h204Img);
		h204Comp->BitmapPush("transparent", transparentImg);
		h204Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("4", std::move(h204Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h205 

		// 1. 이미지 갖고 오기
		auto h205Img = ResourceManager::Get().GetTexture("H2", "05");

		// 2. 오브젝트 만들기
		auto h205Obj = std::make_unique<Object>();
		h205Obj->SetPosition(Vec2(1208, 722));

		// 3.0. 랜더 인포 컴포넌트
		auto h205Info = h205Obj->GetRenderInfo();
		h205Info->SetBitmap(h205Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h205Comp = h205Obj->AddComponent<BackgroundComponent>(h205Info);

		// 3.1.1 사이즈 다르면 
		h205Comp->SetWidth(582); h205Comp->SetHeight(33);
		h205Comp->BitmapPush("Background", h205Img);
		h205Comp->BitmapPush("transparent", transparentImg);
		h205Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("5", std::move(h205Obj));

		//////////////////////
		//////////////////////
		//////////////////////
		// h206 

		// 1. 이미지 갖고 오기
		auto h206Img = ResourceManager::Get().GetTexture("H2", "06");

		// 2. 오브젝트 만들기
		auto h206Obj = std::make_unique<Object>();
		h206Obj->SetPosition(Vec2(1234, 868));

		// 3.0. 랜더 인포 컴포넌트
		auto h206Info = h206Obj->GetRenderInfo();
		h206Info->SetBitmap(h206Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h206Comp = h206Obj->AddComponent<BackgroundComponent>(h206Info);

		// 3.1.1 사이즈 다르면 
		h206Comp->SetWidth(529); h206Comp->SetHeight(79);
		h206Comp->BitmapPush("Background", h206Img);
		h206Comp->BitmapPush("transparent", transparentImg);
		h206Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("6", std::move(h206Obj));

	}
	break;
	case History::H3:
	{
		//////////////////////
		//////////////////////
		//////////////////////
		// h301 

		// 1. 이미지 갖고 오기
		auto h301Img = ResourceManager::Get().GetTexture("H3", "01");

		// 2. 오브젝트 만들기
		auto h301Obj = std::make_unique<Object>();
		h301Obj->SetPosition(Vec2(1216, 141));

		// 3.0. 랜더 인포 컴포넌트
		auto h301Info = h301Obj->GetRenderInfo();
		h301Info->SetBitmap(h301Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h301Comp = h301Obj->AddComponent<BackgroundComponent>(h301Info);

		// 3.1.1 사이즈 다르면 
		h301Comp->SetWidth(559); h301Comp->SetHeight(50);
		h301Comp->BitmapPush("Background", h301Img);
		h301Comp->BitmapPush("transparent", transparentImg);
		h301Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("1", std::move(h301Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h302 

		// 1. 이미지 갖고 오기
		auto h302Img = ResourceManager::Get().GetTexture("H3", "02");

		// 2. 오브젝트 만들기
		auto h302Obj = std::make_unique<Object>();
		h302Obj->SetPosition(Vec2(1228, 389));

		// 3.0. 랜더 인포 컴포넌트
		auto h302Info = h302Obj->GetRenderInfo();
		h302Info->SetBitmap(h302Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h302Comp = h302Obj->AddComponent<BackgroundComponent>(h302Info);

		// 3.1.1 사이즈 다르면 
		h302Comp->SetWidth(551); h302Comp->SetHeight(31);
		h302Comp->BitmapPush("Background", h302Img);
		h302Comp->BitmapPush("transparent", transparentImg);
		h302Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("2", std::move(h302Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h303 

		// 1. 이미지 갖고 오기
		auto h303Img = ResourceManager::Get().GetTexture("H3", "03");

		// 2. 오브젝트 만들기
		auto h303Obj = std::make_unique<Object>();
		h303Obj->SetPosition(Vec2(1254, 484));

		// 3.0. 랜더 인포 컴포넌트
		auto h303Info = h303Obj->GetRenderInfo();
		h303Info->SetBitmap(h303Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h303Comp = h303Obj->AddComponent<BackgroundComponent>(h303Info);

		// 3.1.1 사이즈 다르면 
		h303Comp->SetWidth(498); h303Comp->SetHeight(79);
		h303Comp->BitmapPush("Background", h303Img);
		h303Comp->BitmapPush("transparent", transparentImg);
		h303Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("3", std::move(h303Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h304 

		// 1. 이미지 갖고 오기
		auto h304Img = ResourceManager::Get().GetTexture("H3", "04");

		// 2. 오브젝트 만들기
		auto h304Obj = std::make_unique<Object>();
		h304Obj->SetPosition(Vec2(1321, 628));

		// 3.0. 랜더 인포 컴포넌트
		auto h304Info = h304Obj->GetRenderInfo();
		h304Info->SetBitmap(h304Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h304Comp = h304Obj->AddComponent<BackgroundComponent>(h304Info);

		// 3.1.1 사이즈 다르면 
		h304Comp->SetWidth(365); h304Comp->SetHeight(80);
		h304Comp->BitmapPush("Background", h304Img);
		h304Comp->BitmapPush("transparent", transparentImg);
		h304Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("4", std::move(h304Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h305 

		// 1. 이미지 갖고 오기
		auto h305Img = ResourceManager::Get().GetTexture("H3", "05");

		// 2. 오브젝트 만들기
		auto h305Obj = std::make_unique<Object>();
		h305Obj->SetPosition(Vec2(1272, 772));

		// 3.0. 랜더 인포 컴포넌트
		auto h305Info = h305Obj->GetRenderInfo();
		h305Info->SetBitmap(h305Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h305Comp = h305Obj->AddComponent<BackgroundComponent>(h305Info);

		// 3.1.1 사이즈 다르면 
		h305Comp->SetWidth(455); h305Comp->SetHeight(79);
		h305Comp->BitmapPush("Background", h305Img);
		h305Comp->BitmapPush("transparent", transparentImg);
		h305Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("5", std::move(h305Obj));
	}
	break;
	case History::H4:
	{
		//////////////////////
		//////////////////////
		//////////////////////
		// h401 

		// 1. 이미지 갖고 오기
		auto h401Img = ResourceManager::Get().GetTexture("H4", "01");

		// 2. 오브젝트 만들기
		auto h401Obj = std::make_unique<Object>();
		h401Obj->SetPosition(Vec2(1288, 103));

		// 3.0. 랜더 인포 컴포넌트
		auto h401Info = h401Obj->GetRenderInfo();
		h401Info->SetBitmap(h401Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h401Comp = h401Obj->AddComponent<BackgroundComponent>(h401Info);

		// 3.1.1 사이즈 다르면 
		h401Comp->SetWidth(419); h401Comp->SetHeight(124);
		h401Comp->BitmapPush("Background", h401Img);
		h401Comp->BitmapPush("transparent", transparentImg);
		h401Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("1", std::move(h401Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h402 

		// 1. 이미지 갖고 오기
		auto h402Img = ResourceManager::Get().GetTexture("H4", "02");

		// 2. 오브젝트 만들기
		auto h402Obj = std::make_unique<Object>();
		h402Obj->SetPosition(Vec2(1300, 396));

		// 3.0. 랜더 인포 컴포넌트
		auto h402Info = h402Obj->GetRenderInfo();
		h402Info->SetBitmap(h402Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h402Comp = h402Obj->AddComponent<BackgroundComponent>(h402Info);

		// 3.1.1 사이즈 다르면 
		h402Comp->SetWidth(397); h402Comp->SetHeight(32);
		h402Comp->BitmapPush("Background", h402Img);
		h402Comp->BitmapPush("transparent", transparentImg);
		h402Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("2", std::move(h402Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h403 

		// 1. 이미지 갖고 오기
		auto h403Img = ResourceManager::Get().GetTexture("H4", "03");

		// 2. 오브젝트 만들기
		auto h403Obj = std::make_unique<Object>();
		h403Obj->SetPosition(Vec2(1213, 493));

		// 3.0. 랜더 인포 컴포넌트
		auto h403Info = h403Obj->GetRenderInfo();
		h403Info->SetBitmap(h403Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h403Comp = h403Obj->AddComponent<BackgroundComponent>(h403Info);

		// 3.1.1 사이즈 다르면 
		h403Comp->SetWidth(573); h403Comp->SetHeight(31);
		h403Comp->BitmapPush("Background", h403Img);
		h403Comp->BitmapPush("transparent", transparentImg);
		h403Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("3", std::move(h403Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h404 

		// 1. 이미지 갖고 오기
		auto h404Img = ResourceManager::Get().GetTexture("H4", "04");

		// 2. 오브젝트 만들기
		auto h404Obj = std::make_unique<Object>();
		h404Obj->SetPosition(Vec2(1212, 589));

		// 3.0. 랜더 인포 컴포넌트
		auto h404Info = h404Obj->GetRenderInfo();
		h404Info->SetBitmap(h404Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h404Comp = h404Obj->AddComponent<BackgroundComponent>(h404Info);

		// 3.1.1 사이즈 다르면 
		h404Comp->SetWidth(574); h404Comp->SetHeight(79);
		h404Comp->BitmapPush("Background", h404Img);
		h404Comp->BitmapPush("transparent", transparentImg);
		h404Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("4", std::move(h404Obj));


		//////////////////////
		//////////////////////
		//////////////////////
		// h405 

		// 1. 이미지 갖고 오기
		auto h405Img = ResourceManager::Get().GetTexture("H4", "05");

		// 2. 오브젝트 만들기
		auto h405Obj = std::make_unique<Object>();
		h405Obj->SetPosition(Vec2(1201	,733));

		// 3.0. 랜더 인포 컴포넌트
		auto h405Info = h405Obj->GetRenderInfo();
		h405Info->SetBitmap(h405Img.Get());

		// 3. 배경 컴포넌트 만들기
		auto h405Comp = h405Obj->AddComponent<BackgroundComponent>(h405Info);

		// 3.1.1 사이즈 다르면 
		h405Comp->SetWidth(589); h405Comp->SetHeight(79);
		h405Comp->BitmapPush("Background", h405Img);
		h405Comp->BitmapPush("transparent", transparentImg);
		h405Comp->SetCurrentBitmap("transparent");

		// 9. 게임 오브젝트들에 집어넣기
		m_gameObjects.emplace("5", std::move(h405Obj));
	}
	break;
	}





}
//
//void Scene_History::ActivateAllTexts()
//{
//	try
//	{
//		if (m_History == History::H2)
//		{
//			for (int i = 1; i <= 6; i++)
//			{
//				string objName = to_string(i);
//				auto it = m_gameObjects.find(objName);
//				if (it != m_gameObjects.end())
//				{
//					auto bgComp = it->second->GetComponent<BackgroundComponent>();
//					if (bgComp)
//					{
//						bgComp->SetCurrentBitmap("Background");
//						cout << "H2 텍스트 " << i << " 활성화됨" << endl;
//					}
//				}
//				else
//				{
//					cout << "경고: 오브젝트 " << objName << "를 찾을 수 없습니다" << endl;
//				}
//			}
//		}
//		else
//		{
//			for (int i = 1; i <= 5; i++)
//			{
//				string objName = to_string(i);
//				auto it = m_gameObjects.find(objName);
//				if (it != m_gameObjects.end())
//				{
//					auto bgComp = it->second->GetComponent<BackgroundComponent>();
//					if (bgComp)
//					{
//						bgComp->SetCurrentBitmap("Background");
//						cout << "텍스트 " << i << " 활성화됨" << endl;
//					}
//				}
//				else
//				{
//					cout << "경고: 오브젝트 " << objName << "를 찾을 수 없습니다" << endl;
//				}
//			}
//		}
//
//		cout << "모든 텍스트 활성화 완료!" << endl;
//	}
//	catch (const exception& e)
//	{
//		cout << "텍스트 활성화 중 오류: " << e.what() << endl;
//	}
//}

void Scene_History::DeactivateAllTexts()
{
	try
	{
		if (m_History == History::H2)
		{
			for (int i = 1; i <= 6; i++)
			{
				string objName = to_string(i);
				auto it = m_gameObjects.find(objName);
				if (it != m_gameObjects.end())
				{
					auto bgComp = it->second->GetComponent<BackgroundComponent>();
					if (bgComp)
					{
						bgComp->SetCurrentBitmap("transparent");
						cout << "H2 텍스트 " << i << " 비활성화됨" << endl;
					}
				}
				else
				{
					cout << "경고: 오브젝트 " << objName << "를 찾을 수 없습니다" << endl;
				}
			}
		}
		else
		{
			for (int i = 1; i <= 5; i++)
			{
				string objName = to_string(i);
				auto it = m_gameObjects.find(objName);
				if (it != m_gameObjects.end())
				{
					auto bgComp = it->second->GetComponent<BackgroundComponent>();
					if (bgComp)
					{
						bgComp->SetCurrentBitmap("transparent");
						cout << "텍스트 " << i << " 비활성화됨" << endl;
					}
				}
				else
				{
					cout << "경고: 오브젝트 " << objName << "를 찾을 수 없습니다" << endl;
				}
			}
		}

		cout << "모든 텍스트 비활성화 완료!" << endl;
	}
	catch (const exception& e)
	{
		cout << "텍스트 비활성화 중 오류: " << e.what() << endl;
	}
}

void Scene_History::ActivateSingleText(int idx)
{
	string objName = to_string(idx);
	auto it = m_gameObjects.find(objName);
	if (it != m_gameObjects.end())
	{
		auto bgComp = it->second->GetComponent<BackgroundComponent>();
		if (bgComp)
		{
			bgComp->SetCurrentBitmap("Background");
			cout << idx << "번 텍스트 활성화됨" << endl;
		}
	}
	else
	{
		cout << "경고: 오브젝트 " << objName << "를 찾을 수 없습니다" << endl;
	}
}
