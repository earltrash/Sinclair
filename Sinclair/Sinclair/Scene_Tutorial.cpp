#include "Scene_Tutorial.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "UIManager.h"


Scene_Tutorial::Scene_Tutorial(string name)
{
	m_name = name;
}

Scene_Tutorial::~Scene_Tutorial()
{
}

void Scene_Tutorial::Initalize()
{
	if (dirty) return; // 이미 초기화되었으면 스킵

	CreateObj(); // 오브젝트 생성 (한 번만)
	dirty = true;

	for (const auto& [Name, obj] : m_gameObjects)
	{
		UIManager::Get().AddSceneObject(obj);
	}
}

void Scene_Tutorial::Enter()
{
	Initalize();
}

void Scene_Tutorial::Exit()
{
	// 씬 종료 시마다 실행할 것들  
	// 사운드 정지, 상태 저장 등
	//StopBGM();
	//PauseAnimations();

	Clean();
}

void Scene_Tutorial::Clean()
{


	if (!dirty) return;

	// UIManager에서 씬 객체들을 먼저 정리
	UIManager::Get().ClearSceneObjects();

	// 잠시 대기 후 게임 객체들 정리
	m_gameObjects.clear();

	dirty = false;
}

// 씬 전환 지연 처리를 위해 씬_스탠다드에서 업데이트 일괄 처리.
void Scene_Tutorial::Update()
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

void Scene_Tutorial::LogicUpdate(float delta)
{

}



void Scene_Tutorial::Render()
{
	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetRenderInfo()->GetRenderInfo());
	}

	D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 42.0f);
	D2DRenderer::Get().DrawMessage(L"건너뛰기", 1636 , 60, 1636 + 200, 60 + 200, D2D1::ColorF(0xeedcc4));
	D2DRenderer::Get().DrawMessage(L"다음", 1726 , 975, 1726 + 200, 975 + 200, D2D1::ColorF(0xeedcc4));
	

}


void Scene_Tutorial::CreateObj()
{
	//////////////////////
	//////////////////////
	//////////////////////
	// [1] Background 

	// 1. 16개의 이미지 갖고 오기
	vector<ComPtr<ID2D1Bitmap1>> tutorialBackground{};

	for (int i = 1; i <= 16 ; i++)
	{
		//튜토리얼_01
		tutorialBackground.emplace_back(
			ResourceManager::Get().GetTexture("튜토리얼", to_string(i))
		);
	}
	
	ResourceManager::Get().GetTexture("시작화면");

	// 2. 오브젝트 만들기
	auto Background = std::make_unique<Object>();
	Background->SetPosition(Vec2(0, 0));

	auto bgInfo = Background->GetRenderInfo();
	bgInfo->SetBitmap(tutorialBackground[0].Get());

	// 3. 배경 컴포넌트 만들기
	auto bgComp = Background->AddComponent<BackgroundComponent>(bgInfo);
	// 3.1.1 사이즈 다르면 
	bgComp->SetWidth(1920.f); bgComp->SetHeight(1080.f);
	for (int i = 0; i < 16; i++)
	{
		bgComp->BitmapPush( to_string(i + 1), tutorialBackground[i]);
	}

	// 9
	m_gameObjects.emplace("Background", std::move(Background));

	// 1. 이미지 갖고 오기
	auto TutorialImg = ResourceManager::Get().GetTexture("튜토리얼", "00");

	//////////////////////
	//////////////////////
	//////////////////////
	// [2] TutorialButton1
	// [3] TutorialButton2

	// 2. 오브젝트 만들기
	auto TutorialButton1 = std::make_unique<Object>();
	TutorialButton1 ->SetPosition(Vec2(1820, 32));

	auto TutorialButton2 = std::make_unique<Object>();
	TutorialButton2->SetPosition(Vec2(1820, 947));

	// 3.0. 인포
	auto TutorialInfo1 = TutorialButton1->GetRenderInfo();
	TutorialInfo1->SetBitmap(TutorialImg.Get());
	auto TutorialInfo2 = TutorialButton2->GetRenderInfo();
	TutorialInfo2->SetBitmap(TutorialImg.Get());

	// 3. 버튼1 컴포넌트 만들기
	auto tutorialButton1Comp = TutorialButton1->
		AddComponent<ButtonComponent>(TutorialInfo1);
	tutorialButton1Comp->SetWidth (54 );
	tutorialButton1Comp->SetHeight(105);

	// 3. 버튼2 컴포넌트 만들기
	auto tutorialButton2Comp = TutorialButton2->
		AddComponent<ButtonComponent>(TutorialInfo2);
	tutorialButton2Comp->SetWidth (54 );
	tutorialButton2Comp->SetHeight(105);

	// 4. 버튼 비트맵 설정
	tutorialButton1Comp->BitmapPush("1", TutorialImg);
	tutorialButton1Comp->SetCurrentBitmap("1");

	tutorialButton2Comp->BitmapPush("1", TutorialImg);
	tutorialButton2Comp->SetCurrentBitmap("1");


	// 5. 마우스 리스너 컴포넌트
	// 얘가 필요없는 거 아닌가...
	auto tutorialButton1_mouseListener = TutorialButton1->AddComponent<MouseListenerComponent>(
		[tutorialButton1Comp](const MSG& msg) {
			tutorialButton1Comp->CheckCollision(msg);
			tutorialButton1Comp->Worked(msg);
		}
	);
	auto tutorialButton2_mouseListener = TutorialButton2->AddComponent<MouseListenerComponent>(
		[tutorialButton2Comp](const MSG& msg) {
			tutorialButton2Comp->CheckCollision(msg);
			tutorialButton2Comp->Worked(msg);
		}
	);

	tutorialButton1Comp->SetOnClickCallback([this]() {
		cout << "건너뛰기" << endl;
		SafeChangeScene("InGame");
		});

	tutorialButton2Comp->SetOnClickCallback([this, bgComp]() {
		static int i = 1;

		if ( i  <= 16)
		{
			bgComp->SetCurrentBitmap(to_string(i));
			i++;
		}
		else
		{
			cout << "16번 건너뛰기" << endl;
			SafeChangeScene("InGame");
		}

		});

	// 9
	m_gameObjects.emplace("TutorialButton1", std::move(TutorialButton1));
	m_gameObjects.emplace("TutorialButton2", std::move(TutorialButton2));


}