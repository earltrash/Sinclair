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
	Initalize();

}

void Scene_History::Exit()
{

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

	D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 90.0f);
	std::wstring job = StrToWstr(ResourceManager::Get().Get_TextBank().EndingVector[GameManager::Get().curGen * 10].직업명);
	D2DRenderer::Get().DrawMessageCenter(job.c_str(),
		1080.f, 120.f, 1920.f - 1080.f, 255.f - 120.f, D2D1::ColorF::White);

	D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 30.0f);
	std::wstring text = StrToWstr(ResourceManager::Get().Get_TextBank().EndingVector[GameManager::Get().curGen * 10].엔딩텍스트);
	D2DRenderer::Get().DrawMessageCenter(text.c_str(),
		1223.f, 255.f, 564.f, 1080.f - 255.f, D2D1::ColorF::White);
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
	auto History2 = ResourceManager::Get().GetTexture("H1");
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
	m_gameObjects.emplace("a우측창", std::move(우측창));

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
}