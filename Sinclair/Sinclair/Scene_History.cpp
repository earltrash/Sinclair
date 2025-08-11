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
	}
	else if (resultFame >= 15)
	{
		m_History = History::H2;
	}
	else if (resultFame >= 10)
	{
		m_History = History::H3;
	}
	else
	{
		m_History = History::H4;
	}

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

	if (m_History == History::H1)
	{
		// 제목 (박스 3)
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
		std::wstring Title = StrToWstr(vvs[0][0]);
		D2DRenderer::Get().DrawMessageCenter(Title.c_str(),
			1100.f, 65.f, 1880.f - 1100.f, 155.f - 65.f, D2D1::ColorF::White);

		// 본문 텍스트들 (박스 5, 6, 7 영역들)
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_NORMAL, 30.0f);

		// 첫 번째 텍스트 블록 (박스 5 영역)
		if (vvs[0].size() > 2) {
			std::wstring text1 = StrToWstr(vvs[0][2]);
			D2DRenderer::Get().DrawMessage(text1.c_str(),
				1130.f, 200.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[0].size() > 3) {
			std::wstring text2 = StrToWstr(vvs[0][3]);
			D2DRenderer::Get().DrawMessage(text2.c_str(),
				1130.f, 250.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}

		// 두 번째 텍스트 블록 (박스 6 영역)
		if (vvs[0].size() > 5) {
			std::wstring text3 = StrToWstr(vvs[0][5]);
			D2DRenderer::Get().DrawMessage(text3.c_str(),
				1130.f, 350.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[0].size() > 6) {
			std::wstring text4 = StrToWstr(vvs[0][6]);
			D2DRenderer::Get().DrawMessage(text4.c_str(),
				1130.f, 400.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}

		// 세 번째 텍스트 블록 (박스 7 영역)
		if (vvs[0].size() > 8) {
			std::wstring text5 = StrToWstr(vvs[0][8]);
			D2DRenderer::Get().DrawMessage(text5.c_str(),
				1130.f, 500.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[0].size() > 9) {
			std::wstring text6 = StrToWstr(vvs[0][9]);
			D2DRenderer::Get().DrawMessage(text6.c_str(),
				1130.f, 550.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
	}
	else if (m_History == History::H2)
	{
		// 제목
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
		std::wstring Title = StrToWstr(vvs[1][0]);
		D2DRenderer::Get().DrawMessageCenter(Title.c_str(),
			1100.f, 65.f, 1880.f - 1100.f, 155.f - 65.f, D2D1::ColorF::White);

		// 본문 텍스트들
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_NORMAL, 30.0f);

		if (vvs[1].size() > 2) {
			std::wstring text1 = StrToWstr(vvs[1][2]);
			D2DRenderer::Get().DrawMessage(text1.c_str(),
				1130.f, 200.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[1].size() > 3) {
			std::wstring text2 = StrToWstr(vvs[1][3]);
			D2DRenderer::Get().DrawMessage(text2.c_str(),
				1130.f, 250.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[1].size() > 5) {
			std::wstring text3 = StrToWstr(vvs[1][5]);
			D2DRenderer::Get().DrawMessage(text3.c_str(),
				1130.f, 350.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[1].size() > 6) {
			std::wstring text4 = StrToWstr(vvs[1][6]);
			D2DRenderer::Get().DrawMessage(text4.c_str(),
				1130.f, 400.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[1].size() > 7) {
			std::wstring text5 = StrToWstr(vvs[1][7]);
			D2DRenderer::Get().DrawMessage(text5.c_str(),
				1130.f, 450.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[1].size() > 8) {
			std::wstring text6 = StrToWstr(vvs[1][8]);
			D2DRenderer::Get().DrawMessage(text6.c_str(),
				1130.f, 500.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[1].size() > 9) {
			std::wstring text7 = StrToWstr(vvs[1][9]);
			D2DRenderer::Get().DrawMessage(text7.c_str(),
				1130.f, 550.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
	}
	else if (m_History == History::H3)
	{
		// 제목
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
		std::wstring Title = StrToWstr(vvs[2][0]);
		D2DRenderer::Get().DrawMessageCenter(Title.c_str(),
			1100.f, 65.f, 1880.f - 1100.f, 155.f - 65.f, D2D1::ColorF::White);

		// 본문 텍스트들
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_NORMAL, 30.0f);

		if (vvs[2].size() > 2) {
			std::wstring text1 = StrToWstr(vvs[2][2]);
			D2DRenderer::Get().DrawMessage(text1.c_str(),
				1130.f, 200.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[2].size() > 3) {
			std::wstring text2 = StrToWstr(vvs[2][3]);
			D2DRenderer::Get().DrawMessage(text2.c_str(),
				1130.f, 250.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[2].size() > 4) {
			std::wstring text3 = StrToWstr(vvs[2][4]);
			D2DRenderer::Get().DrawMessage(text3.c_str(),
				1130.f, 300.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[2].size() > 6) {
			std::wstring text4 = StrToWstr(vvs[2][6]);
			D2DRenderer::Get().DrawMessage(text4.c_str(),
				1130.f, 400.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[2].size() > 8) {
			std::wstring text5 = StrToWstr(vvs[2][8]);
			D2DRenderer::Get().DrawMessage(text5.c_str(),
				1130.f, 500.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[2].size() > 9) {
			std::wstring text6 = StrToWstr(vvs[2][9]);
			D2DRenderer::Get().DrawMessage(text6.c_str(),
				1130.f, 550.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
	}
	else // H4
	{
		// 제목
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
		std::wstring Title = StrToWstr(vvs[3][0]);
		D2DRenderer::Get().DrawMessageCenter(Title.c_str(),
			1100.f, 65.f, 1880.f - 1100.f, 155.f - 65.f, D2D1::ColorF::White);

		// 본문 텍스트들
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_NORMAL, 30.0f);

		if (vvs[3].size() > 2) {
			std::wstring text1 = StrToWstr(vvs[3][2]);
			D2DRenderer::Get().DrawMessage(text1.c_str(),
				1130.f, 200.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[3].size() > 3) {
			std::wstring text2 = StrToWstr(vvs[3][3]);
			D2DRenderer::Get().DrawMessage(text2.c_str(),
				1130.f, 250.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[3].size() > 5) {
			std::wstring text3 = StrToWstr(vvs[3][5]);
			D2DRenderer::Get().DrawMessage(text3.c_str(),
				1130.f, 350.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[3].size() > 6) {
			std::wstring text4 = StrToWstr(vvs[3][6]);
			D2DRenderer::Get().DrawMessage(text4.c_str(),
				1130.f, 400.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[3].size() > 8) {
			std::wstring text5 = StrToWstr(vvs[3][8]);
			D2DRenderer::Get().DrawMessage(text5.c_str(),
				1130.f, 500.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
		if (vvs[3].size() > 9) {
			std::wstring text6 = StrToWstr(vvs[3][9]);
			D2DRenderer::Get().DrawMessage(text6.c_str(),
				1130.f, 550.f, 1850.f - 1130.f, 40.f, D2D1::ColorF::White);
		}
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