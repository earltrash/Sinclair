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
	if (dirty) return; // �̹� �ʱ�ȭ�Ǿ����� ��ŵ

	CreateObj(); // ������Ʈ ���� (�� ����)
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
	// �� ���� �ø��� ������ �͵�  
	// ���� ����, ���� ���� ��
	//StopBGM();
	//PauseAnimations();

	Clean();
}

void Scene_Tutorial::Clean()
{


	if (!dirty) return;

	// UIManager���� �� ��ü���� ���� ����
	UIManager::Get().ClearSceneObjects();

	// ��� ��� �� ���� ��ü�� ����
	m_gameObjects.clear();

	dirty = false;
}

// �� ��ȯ ���� ó���� ���� ��_���Ĵٵ忡�� ������Ʈ �ϰ� ó��.
void Scene_Tutorial::Update()
{
	// �� ��ȯ ���� ó��
	if (m_isTransitioning && !m_nextScene.empty())
	{
		m_currentDelay += 0.016f; // �뷫���� ������ �ð� (60FPS ����)
		if (m_currentDelay >= m_transitionDelay)
		{
			// ���� �� ��ȯ ����
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

	D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 42.0f);
	D2DRenderer::Get().DrawMessage(L"�ǳʶٱ�", 1636 , 60, 1636 + 200, 60 + 200, D2D1::ColorF(0xeedcc4));
	D2DRenderer::Get().DrawMessage(L"����", 1726 , 975, 1726 + 200, 975 + 200, D2D1::ColorF(0xeedcc4));
	

}


void Scene_Tutorial::CreateObj()
{
	//////////////////////
	//////////////////////
	//////////////////////
	// [1] Background 

	// 1. 16���� �̹��� ���� ����
	vector<ComPtr<ID2D1Bitmap1>> tutorialBackground{};

	for (int i = 1; i <= 16 ; i++)
	{
		//Ʃ�丮��_01
		tutorialBackground.emplace_back(
			ResourceManager::Get().GetTexture("Ʃ�丮��", to_string(i))
		);
	}
	
	ResourceManager::Get().GetTexture("����ȭ��");

	// 2. ������Ʈ �����
	auto Background = std::make_unique<Object>();
	Background->SetPosition(Vec2(0, 0));

	auto bgInfo = Background->GetRenderInfo();
	bgInfo->SetBitmap(tutorialBackground[0].Get());

	// 3. ��� ������Ʈ �����
	auto bgComp = Background->AddComponent<BackgroundComponent>(bgInfo);
	// 3.1.1 ������ �ٸ��� 
	bgComp->SetWidth(1920.f); bgComp->SetHeight(1080.f);
	for (int i = 0; i < 16; i++)
	{
		bgComp->BitmapPush( to_string(i + 1), tutorialBackground[i]);
	}

	// 9
	m_gameObjects.emplace("Background", std::move(Background));

	// 1. �̹��� ���� ����
	auto TutorialImg = ResourceManager::Get().GetTexture("Ʃ�丮��", "00");

	//////////////////////
	//////////////////////
	//////////////////////
	// [2] TutorialButton1
	// [3] TutorialButton2

	// 2. ������Ʈ �����
	auto TutorialButton1 = std::make_unique<Object>();
	TutorialButton1 ->SetPosition(Vec2(1820, 32));

	auto TutorialButton2 = std::make_unique<Object>();
	TutorialButton2->SetPosition(Vec2(1820, 947));

	// 3.0. ����
	auto TutorialInfo1 = TutorialButton1->GetRenderInfo();
	TutorialInfo1->SetBitmap(TutorialImg.Get());
	auto TutorialInfo2 = TutorialButton2->GetRenderInfo();
	TutorialInfo2->SetBitmap(TutorialImg.Get());

	// 3. ��ư1 ������Ʈ �����
	auto tutorialButton1Comp = TutorialButton1->
		AddComponent<ButtonComponent>(TutorialInfo1);
	tutorialButton1Comp->SetWidth (54 );
	tutorialButton1Comp->SetHeight(105);

	// 3. ��ư2 ������Ʈ �����
	auto tutorialButton2Comp = TutorialButton2->
		AddComponent<ButtonComponent>(TutorialInfo2);
	tutorialButton2Comp->SetWidth (54 );
	tutorialButton2Comp->SetHeight(105);

	// 4. ��ư ��Ʈ�� ����
	tutorialButton1Comp->BitmapPush("1", TutorialImg);
	tutorialButton1Comp->SetCurrentBitmap("1");

	tutorialButton2Comp->BitmapPush("1", TutorialImg);
	tutorialButton2Comp->SetCurrentBitmap("1");


	// 5. ���콺 ������ ������Ʈ
	// �갡 �ʿ���� �� �ƴѰ�...
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
		cout << "�ǳʶٱ�" << endl;
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
			cout << "16�� �ǳʶٱ�" << endl;
			SafeChangeScene("InGame");
		}

		});

	// 9
	m_gameObjects.emplace("TutorialButton1", std::move(TutorialButton1));
	m_gameObjects.emplace("TutorialButton2", std::move(TutorialButton2));


}