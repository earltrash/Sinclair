#include "Scene_Outgame.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "Renderer.h"
#include "UIManager.h"
#include "SliderHandleComponent.h"

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
	if (dirty) return;  

	CreateObj();  
	for (const auto& [Name, obj] : m_gameObjects)
	{
		UIManager::Get().AddSceneObject(obj);
	}

	dirty = true;
}

void Scene_Outgame::Clean()
{
	if (!dirty) return;

	m_gameObjects.clear();
	UIManager::Get().ClearSceneObjects();
	
	dirty = false;
}

void Scene_Outgame::Enter()
{
	Initalize();
	SetupCharacterAndBackground();


	 if (GameManager::Get().isFirst) {
		 m_state = State::FIRST_ENTER;
		 GameManager::Get().isFirst = false;
	 } 
	 else {
		 m_state = State::CHOICE_MENU;
	 }
	 
	 characterName = L"싱클레어 " + std::to_wstring(GameManager::Get().curGen) + L"세";

	// 우선 초기 상태로 진입
	ChangeState(m_state);
}

void Scene_Outgame::Exit()
{

		Clean();
}

void Scene_Outgame::Update()
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

void Scene_Outgame::LogicUpdate(float delta)
{

}


void Scene_Outgame::Render()
{

	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetRenderInfo()->GetRenderInfo());
	}


	D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 36.0f);
	D2DRenderer::Get().DrawMessage(characterName.c_str(), 481.74f, 828.38f, 1300.f, 1000.f, D2D1::ColorF::White);
	D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Regular", DWRITE_FONT_WEIGHT_NORMAL, 24.0f);
	// 상태에 따른 메인 텍스트 렌더링
	if (!curText.empty())
	{
		if (m_state == CHOICE_MENU)
		{
			ButtonComponent::ButtonState changoState = m_gameObjects["Chango"]->GetComponent<ButtonComponent>()->GetState();
			ButtonComponent::ButtonState moheomState = m_gameObjects["Moheom"]->GetComponent<ButtonComponent>()->GetState();

			// 선택 메뉴일 때는 각 옵션을 따로 그리기
			std::wstring wOption1 = L"> 창고로 이동한다.";
			std::wstring wOption2 = L"> 모험을 떠난다.";

			// 첫 번째 옵션 색상 결정
			D2D1::ColorF color1 = D2D1::ColorF::White;
			switch (changoState)
			{
			case ButtonComponent::ButtonState::Normal:
				color1 = D2D1::ColorF::White;
				break;
			case ButtonComponent::ButtonState::Hover:
				color1 = D2D1::ColorF::Yellow;
				break;
			case ButtonComponent::ButtonState::Pressed:
				color1 = D2D1::ColorF::Red;
				break;
			}

			// 두 번째 옵션 색상 결정
			D2D1::ColorF color2 = D2D1::ColorF::White;
			switch (moheomState)
			{
			case ButtonComponent::ButtonState::Normal:
				color2 = D2D1::ColorF::White;
				break;
			case ButtonComponent::ButtonState::Hover:
				color2 = D2D1::ColorF::Yellow;
				break;
			case ButtonComponent::ButtonState::Pressed:
				color2 = D2D1::ColorF::Red;
				break;
			}
			// 각각 다른 위치에 다른 색으로 그리기
			D2DRenderer::Get().DrawMessage(wOption1.c_str(), 768.21f, 828.38f, 1300.f, 50.f, color1);
			D2DRenderer::Get().DrawMessage(wOption2.c_str(), 768.21f, 878.38f, 1300.f, 50.f, color2);
		}
		else
		{
			// 일반 텍스트는 기존과 동일하게
			std::wstring wCurText = StrToWstr(curText);
			D2DRenderer::Get().DrawMessage(wCurText.c_str(), 768.21f, 828.38f, 1300.f, 1000.f, D2D1::ColorF::White);
		}
	}


	if (isSETTING)
	{
		// [설정 텍스트] 48pt 텍스트
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
		D2DRenderer::Get().DrawMessage(L"설정", 528.77f, 343.0f, 528.77f + 200, 343.0f + 200, D2D1::ColorF(0xffe6d8));

		// [배경음 텍스트] 30pt 텍스트
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 30.0f);
		D2DRenderer::Get().DrawMessage(L"배경음", 550.18f, 498.77f, 550.18f + 200, 498.77f + 200, D2D1::ColorF(0xffe6d8));

		// [효과음 텍스트] 30pt 텍스트
		D2DRenderer::Get().DrawMessage(L"효과음", 550.18f, 641.77f, 550.18f + 200, 641.77f + 200, D2D1::ColorF(0xffe6d8));

	}
}

void Scene_Outgame::CreateObj()
{
	//////////////////////
	// 1.투명 이미지 갖고 오기
	// 아웃게임에서 이미지 비활성화를 위해서 존재
	auto transparentImg = ResourceManager::Get().GetTexture("transparent");


	//////////////////////
	//////////////////////
	//////////////////////
	// [1] Background 

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
	//////////////////////
	// [2] 캐릭터 이미지 
	// 배경 컴포넌트라고 명명 했지만 -> 이미지 컴포넌트라고 바꿔야 해. 
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
	//////////////////////
	// [3] 텍스트 창
	
	// 1. 이미지 갖고 오기
	auto 아웃게임1 = ResourceManager::Get().GetTexture("아웃게임", "01");
	// 2. 오브젝트 만들기
	auto 텍스트박스 = std::make_unique<Object>();
	     텍스트박스->SetPosition(Vec2(421, 766));

	// 3.0. 랜더 인포 컴포넌트
		 auto outgame1Info = 텍스트박스->GetRenderInfo();
		 outgame1Info->SetBitmap(아웃게임1.Get());
	// 3. 배경 컴포넌트 만들기
	auto 텍스트박스Comp = 텍스트박스->AddComponent<BackgroundComponent>(outgame1Info);
	
	// 3.1.1 사이즈 
	텍스트박스Comp->SetWidth(아웃게임1->GetSize().width);
	텍스트박스Comp->SetHeight(아웃게임1->GetSize().height);


	텍스트박스Comp->BitmapPush("아웃게임1", 아웃게임1);
	텍스트박스Comp->SetCurrentBitmap("아웃게임1");
	// 9. 게임 오브젝트들에 집어넣기 
	m_gameObjects.emplace("아웃게임1", std::move(텍스트박스));


	/////////////////////
	/////////////////////
	/////////////////////
	// [4] 예 버튼

	// 1. 이미지 갖고 오기
	auto 아웃게임4 = ResourceManager::Get().GetTexture("아웃게임", "04");
	// 2. 오브젝트 만들기
	auto 예 = std::make_unique<Object>();
	예->SetPosition(Vec2(1134, 905));

	auto 예info = 예->GetRenderInfo();
	예info->SetBitmap(아웃게임4.Get());
	// 3. 버튼 컴포넌트 만들기
	auto YesComp = 예->AddComponent<ButtonComponent>(예info);
	YesComp->SetWidth( 아웃게임4->GetSize().width);
	YesComp->SetHeight(아웃게임4->GetSize().height);

	//  4. 버튼 비트맵 설정
	YesComp->BitmapPush("yes",  아웃게임4);
	YesComp->BitmapPush("transparent", transparentImg);
	
	if (m_state == FIRST_ENTER)
		YesComp->SetCurrentBitmap("transparent");
	else
		YesComp->SetCurrentBitmap("yes");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto Yes_mouseListener = 예->AddComponent<MouseListenerComponent>(
		[YesComp](const MSG& msg) {
			YesComp->CheckCollision(msg);
			YesComp->Worked(msg);
		}
	);

	YesComp->SetOnClickCallback([this]() {
		// 그렇게 하면 되는 구나
		//SceneManager::Get().ChangeScene("InGame");
		//std::cout << "SceneManager::Get().ChangeScene(\"InGame\");" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("아웃게임4", std::move(예));


	/////////////////////
	/////////////////////
	/////////////////////
	// [5] 아니오 버튼/다음 버튼

	// 1. 이미지 갖고 오기
	auto 아웃게임5다음 = ResourceManager::Get().GetTexture("아웃게임", "05다음");
	auto 아웃게임5아니오 = ResourceManager::Get().GetTexture("아웃게임", "05아니오");

	// 2. 오브젝트 만들기
	auto 아니오 = std::make_unique<Object>();
	아니오->SetPosition(Vec2(1296, 906));

	auto 아니오info = 아니오->GetRenderInfo();
	아니오info->SetBitmap(아웃게임5다음.Get());

	// 3. 버튼 컴포넌트 만들기
	auto NoComp = 아니오->AddComponent<ButtonComponent>(아니오info);
	NoComp->SetWidth(아웃게임5아니오->GetSize().width);
	NoComp->SetHeight(아웃게임5아니오->GetSize().height);

	//  4. 버튼 비트맵 설정
	// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
	NoComp->BitmapPush("next", 아웃게임5다음);
	NoComp->BitmapPush("no", 아웃게임5아니오);
	NoComp->BitmapPush("transparent", transparentImg);

	//changeState()
	//NoComp->SetCurrentBitmap("no");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto No_mouseListener = 아니오->AddComponent<MouseListenerComponent>(
		[NoComp](const MSG& msg) {
			NoComp->CheckCollision(msg);
			NoComp->Worked(msg);
		}
	);

	NoComp->SetOnClickCallback([this]() {
			//changeState()
		});

	/// 9 
	m_gameObjects.emplace("아웃게임5", std::move(아니오));

	

	/////////////////////
	/////////////////////
	/////////////////////
	// [6] 뒤로가기 버튼

	// 1. 이미지 갖고 오기
	auto 뒤로가기 = ResourceManager::Get().GetTexture("뒤로가기");
	// 2. 오브젝트 만들기
	auto 뒤로 = std::make_unique<Object>();
	뒤로->SetPosition(Vec2(41, 31));

	auto 뒤로info = 뒤로->GetRenderInfo();
	뒤로info->SetBitmap(뒤로가기.Get());
	// 3. 버튼 컴포넌트 만들기
	auto backComp = 뒤로->AddComponent<ButtonComponent>(뒤로info);
	backComp->SetWidth(65); 
	backComp->SetHeight(65);
	
	//  4. 버튼 비트맵 설정
	// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
	backComp->BitmapPush("back", 뒤로가기);

	backComp->SetCurrentBitmap("back");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto Back_mouseListener = 뒤로->AddComponent<MouseListenerComponent>(
		[backComp](const MSG& msg) {
			backComp->CheckCollision(msg);
			backComp->Worked(msg);
		}
	);

	backComp->SetOnClickCallback([this]() {

		if(m_state == State::FIRST_ENTER ) 
		{
			GameManager::Get().isFirst = true;
		}

		std::cout << "버튼 클릭됨 - 현재 씬: " << typeid(*this).name() << std::endl;
		isSETTING = false;
		SafeChangeScene("Title");

		});

	/// 9
	m_gameObjects.emplace("뒤로가기", std::move(뒤로));


	/////////////////////
	/////////////////////
	/////////////////////
	// [7] 투명 :  > 창고로 이동한다.
	// size = 250 * 38
	// pos = 750, 825


	// 2. 오브젝트 만들기
	auto Chango = std::make_unique<Object>();
	Chango->SetPosition(Vec2(750, 825));

	auto Changoinfo = Chango->GetRenderInfo();
	Changoinfo->SetBitmap(transparentImg.Get());

	// 3. 버튼 컴포넌트 만들기
	auto ChangoComp = Chango->AddComponent<ButtonComponent>(Changoinfo);
	ChangoComp->SetWidth (250);
	ChangoComp->SetHeight(38 );

	//  4. 버튼 비트맵 설정
	// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
	ChangoComp->BitmapPush("transparent", transparentImg);

	ChangoComp->SetCurrentBitmap("transparent");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto Chango_mouseListener = Chango->AddComponent<MouseListenerComponent>(
		[ChangoComp](const MSG& msg) {
			ChangoComp->CheckCollision(msg);
			ChangoComp->Worked(msg);
		}
	);

	ChangoComp->SetOnClickCallback([this]() {

		});

	/// 9
	m_gameObjects.emplace("Chango", std::move(Chango));


	/////////////////////
	/////////////////////
	/////////////////////
	// [8] 투명 :  > 모험을 떠난다.
	// pos = 750, 863
	
	// 2. 오브젝트 만들기
	auto Moheom = std::make_unique<Object>();
	Moheom->SetPosition(Vec2(750, 863));

	auto Moheominfo = Moheom->GetRenderInfo();
	Moheominfo->SetBitmap(transparentImg.Get());

	// 3. 버튼 컴포넌트 만들기
	auto MoheomComp = Moheom->AddComponent<ButtonComponent>(Moheominfo);
	MoheomComp->SetWidth(250);
	MoheomComp->SetHeight(38);

	//  4. 버튼 비트맵 설정
	// 투명도 기준이면 굳이 이렇게 할 필요 없긴 해. 
	MoheomComp->BitmapPush("transparent", transparentImg);

	MoheomComp->SetCurrentBitmap("transparent");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto Moheom_mouseListener = Moheom->AddComponent<MouseListenerComponent>(
		[MoheomComp](const MSG& msg) {
			MoheomComp->CheckCollision(msg);
			MoheomComp->Worked(msg);
		}
	);

	MoheomComp->SetOnClickCallback([this]() {
		});

	/// 9
	m_gameObjects.emplace("Moheom", std::move(Moheom));

	/////////////////////
	/////////////////////
	/////////////////////
	// [10] 설정 버튼

	// 1. 이미지 갖고 오기
	auto 설정 = ResourceManager::Get().GetTexture("설정");
	// 2. 오브젝트 만들기
	auto 설정로 = std::make_unique<Object>();
	설정로->SetPosition(Vec2(41, 106));

	auto 설정info = 설정로->GetRenderInfo();
	설정info->SetBitmap(설정.Get());
	// 3. 버튼 컴포넌트 만들기
	auto settingComp = 설정로->AddComponent<ButtonComponent>(설정info);
	settingComp->SetWidth (65);
	settingComp->SetHeight(65);

	//  4. 버튼 비트맵 설정
	settingComp->BitmapPush("setting", 설정);

	settingComp->SetCurrentBitmap("setting");

	// 5. 마우스 리스너 컴포넌트 (버튼 컴포넌트를 캡처로 전달)
	auto set_mouseListener = 설정로->AddComponent<MouseListenerComponent>(
		[settingComp](const MSG& msg) {
			settingComp->CheckCollision(msg);
			settingComp->Worked(msg);
		}
	);



	settingComp->SetOnClickCallback([this]() {
		std::cout << "설정 버튼 클릭됨 - 현재 씬: " << typeid(*this).name() << std::endl;
		isSETTING = !isSETTING;
		if (m_state == State::FIRST_ENTER)
		{
			GameManager::Get().isFirst = true;
		}

		SafeChangeScene("OutGame");
		});

	/// 9
	m_gameObjects.emplace("설정", std::move(설정로));

	if (isSETTING)
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
		// [7] 배경음슬라이더_바   
		// [8] 배경음슬라이더_핸들 
		// [9] 효과음슬라이더_바   
		// [10] 효과음슬라이더_핸들 

		// 1. 이미지 리소스 가져오기
		auto barImg = ResourceManager::Get().GetTexture("설정", "bar");
		auto handleImg = ResourceManager::Get().GetTexture("설정", "handle");

		// 2. 슬라이더 오브젝트 만들기
		auto bgmBar = std::make_unique<Object>();
		auto bgmHandle = std::make_unique<Object>();
		auto sfxBar = std::make_unique<Object>();
		auto sfxHandle = std::make_unique<Object>();


		bgmBar->SetPosition(Vec2(729, 498));
		auto bgmBarInfo = bgmBar->GetRenderInfo();
		bgmBarInfo->SetBitmap(barImg.Get());

		bgmHandle->SetPosition(Vec2(1347, 495));
		auto bgmHandleInfo = bgmHandle->GetRenderInfo();
		bgmHandleInfo->SetBitmap(handleImg.Get());

		sfxBar->SetPosition(Vec2(729, 641));
		auto sfxBarInfo = sfxBar->GetRenderInfo();
		sfxBarInfo->SetBitmap(barImg.Get());

		sfxHandle->SetPosition(Vec2(1347, 638));
		auto sfxHandleInfo = sfxHandle->GetRenderInfo();
		sfxHandleInfo->SetBitmap(handleImg.Get());


		// 3. 배경/ 슬라이더핸들 컴포넌트 만들기
		auto bgmBarComp = bgmBar->AddComponent<BackgroundComponent>(bgmBarInfo);
		auto bgmHandleComp = bgmHandle->AddComponent<SliderHandleComponent>(bgmHandleInfo, 729.0f, 1347.0f, GameManager::Get().bgmValue);
		auto sfxBarComp = sfxBar->AddComponent<BackgroundComponent>(sfxBarInfo);
		auto sfxHandleComp = sfxHandle->AddComponent<SliderHandleComponent>(sfxHandleInfo, 729.0f, 1347.0f, GameManager::Get().sfxValue);

		// 3.1.1 사이즈 설정 
		bgmBarComp->SetWidth(630.f);
		bgmBarComp->SetHeight(32.f);
		bgmBarComp->BitmapPush("barImg", barImg);
		bgmHandleComp->SetWidth(19.f);
		bgmHandleComp->SetHeight(38.f);

		sfxBarComp->SetWidth(630.f);
		sfxBarComp->SetHeight(32.f);
		sfxBarComp->BitmapPush("barImg", barImg);
		sfxHandleComp->SetWidth(19.f);
		sfxHandleComp->SetHeight(38.f);

		// 4. 값 변경 콜백 설정
		bgmHandleComp->SetOnValueChanged([](float value) {
			std::cout << "슬라이더 값: " << value << " (0.0~1.0)" << std::endl;
			// 예: 볼륨 설정
			// SoundManager::Get().SetMasterVolume(value);
			});

		sfxHandleComp->SetOnValueChanged([](float value) {
			std::cout << "슬라이더 값: " << value << " (0.0~1.0)" << std::endl;
			// 예: 볼륨 설정
			// SoundManager::Get().SetMasterVolume(value);
			});


		// 5. 마우스 리스너 컴포넌트
		auto bgmHandle_mouseListener = bgmHandle->AddComponent<MouseListenerComponent>(
			[bgmHandleComp](const MSG& msg) {
				bgmHandleComp->HandleMouse(msg);
			}
		);

		auto sfxHandle_mouseListener = sfxHandle->AddComponent<MouseListenerComponent>(
			[sfxHandleComp](const MSG& msg) {
				sfxHandleComp->HandleMouse(msg);
			}
		);

		// 6. 초기값 설정 -> 임시로 게임 매니저 쪽에서 받게끔, 하자. 
		// 사운드 매니저에서 받아오는 걸로 바꿔야 함.

		bgmHandleComp->SetValue(GameManager::Get().bgmValue);
		sfxHandleComp->SetValue(GameManager::Get().sfxValue);



		// 6. 게임 오브젝트에 등록
		m_gameObjects.emplace("크크_bgm바", std::move(bgmBar));
		m_gameObjects.emplace("크크_bgm핸들", std::move(bgmHandle));
		m_gameObjects.emplace("크크_sfx바", std::move(sfxBar));
		m_gameObjects.emplace("크크_sfx핸들", std::move(sfxHandle));




		/////////////////////
		/////////////////////
		/////////////////////
		// [11] 뒤로가기 버튼

		// 1. 이미지 갖고 오기
		auto 뒤로가기 = ResourceManager::Get().GetTexture("닫기");
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
				backComp->CheckCollision(msg);
				backComp->Worked(msg);
			}
		);

		backComp->SetOnClickCallback([this]() {
			std::cout << "닫기 버튼 클릭됨 - 현재 씬: " << typeid(*this).name() << std::endl;
			isSETTING = !isSETTING;
			SafeChangeScene("OutGame");
			});

		/// 9
		m_gameObjects.emplace("크레딧_뒤로가기", std::move(뒤로));
	}
}


void Scene_Outgame::SetupCharacterAndBackground()
{	
	// 배경은 안바뀜
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
	m_state = newState;

	// 버튼 콜백을 가져오기 위한 준비
	auto yesButton = m_gameObjects["아웃게임4"]->GetComponent<ButtonComponent>();
	auto noButton = m_gameObjects["아웃게임5"]->GetComponent<ButtonComponent>();
	auto changoButton = m_gameObjects["Chango"]->GetComponent<ButtonComponent>();
	auto moheomButton = m_gameObjects["Moheom"]->GetComponent<ButtonComponent>();
	// '다음' 또는 선택지 버튼들도 마찬가지로 가져옵니다.

	switch (m_state)
	{
	case FIRST_ENTER:
	{
		curText = stringFIRST_ENTER[index];

		// '다음' 버튼을 보여주고 '예' 버튼은 숨깁니다.
		yesButton->SetCurrentBitmap("transparent");
		noButton->SetCurrentBitmap("next");

		// '다음' 버튼의 콜백을 설정하여, 다음 텍스트를 보여주거나 상태를 80002로 변경하도록 합니다.
		noButton->SetOnClickCallback([this]() {
			if (index < 2)
			{
				index++;
				curText = stringFIRST_ENTER[index];
			}
			else
			{
				m_state = CHOICE_MENU;
				SafeChangeScene("Tutorial");
			}
		});

		changoButton->SetOnClickCallback([this]() {
			});
		moheomButton->SetOnClickCallback([this]() {
			});
		break;
}

	case CHOICE_MENU: // 분리할 수도 있음
	{
		//curText = stringCHOICE_MENU;

		yesButton->SetCurrentBitmap("transparent");
		noButton->SetCurrentBitmap("transparent");

		yesButton->SetOnClickCallback([this]() {
			});

		noButton->SetOnClickCallback([this]() {
			});

		changoButton->SetOnClickCallback([this]() {
			ChangeState(ENTER_OUTGAME);
			});
		moheomButton->SetOnClickCallback([this]() {
			ChangeState(ENTER_END);
			});


		break;
	}

	case ENTER_OUTGAME:
	{
		curText = stringENTER_OUTGAME; 

		yesButton->SetCurrentBitmap("yes");
		noButton->SetCurrentBitmap("no");

		yesButton->SetOnClickCallback([this]() {
			cout << "창고에 들어가시겠습니까?..." << endl;
			SafeChangeScene("InGame");
			});

		noButton->SetOnClickCallback([this]() {
			ChangeState(CHOICE_MENU);
			});

		changoButton->SetOnClickCallback([this]() {
			});
		moheomButton->SetOnClickCallback([this]() {
			});

		break;
	}

	case ENTER_END:
	{
		curText = stringENTER_END; // "여행을 떠나시겠습니까?..."

		yesButton->SetCurrentBitmap("yes");
		noButton->SetCurrentBitmap("no");

		yesButton->SetOnClickCallback([this]() {
			cout << "여행을 떠나시겠습니까?..." << endl;
			GameManager::Get().PreAdv();
			SafeChangeScene("End");
			});

		noButton->SetOnClickCallback([this]() {
			ChangeState(CHOICE_MENU); // 선택지로 돌아가기
			});

		changoButton->SetOnClickCallback([this]() {
			});
		moheomButton->SetOnClickCallback([this]() {
			});

		break;
	}
	}
}

