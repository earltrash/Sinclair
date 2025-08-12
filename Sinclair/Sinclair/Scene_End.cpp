#include "Scene_End.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "UIManager.h"
#include "GameManager.h"


Scene_End::Scene_End(string name)
{
	m_name = name;
}

Scene_End::~Scene_End()
{

}

void Scene_End::Initalize()
{
	if (dirty) return; // �̹� �ʱ�ȭ�Ǿ����� ��ŵ
	CreateObj(); // ������Ʈ ���� (�� ����)
	dirty = true;
	for (const auto& [Name, obj] : m_gameObjects)
	{
		UIManager::Get().AddSceneObject(obj);
	}
}

void Scene_End::Enter()
{
	ebm = GameManager::Get().AftAdv();


	m_textShown = true;


	Initalize();

}

void Scene_End::Exit()
{

	m_textShown = false;
	m_titleShown = false;
	m_scriptShown = false;

	Clean();
}

void Scene_End::Clean()
{
	if (!dirty) return;

	UIManager::Get().ClearSceneObjects();
	m_gameObjects.clear();
	
	dirty = false;
}

void Scene_End::Update()
{
	// �� ��ȯ ���� ó��
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

	if (m_textShown)
	{
		m_currentShowingDelay += 0.016f;
		if (m_currentShowingDelay >= m_showingDelay)
		{
			if(m_scriptShown == false)
			{
				m_scriptShown = true;
				m_currentShowingDelay = 0.f;
			}
			else
			{
				m_titleShown = true;
				m_currentShowingDelay = 0.f;
				m_textShown = false;
				m_gameObjects["��ŵ��ư"]->GetComponent<ButtonComponent>()->SetCurrentBitmap("��ŵ");
			}
		}
	}



}

void Scene_End::LogicUpdate(float delta)
{


}


void Scene_End::Render()
{

	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetRenderInfo()->GetRenderInfo());
	}

	if (ebm.size() == 1)
	{
		// �ϳ��� ���� �� �� �׳� ����
		const auto& [Name, obj] = *ebm.begin();
		D2D1_RECT_F rect{ 17, 22, 1024 + 17, 1024 + 22 };
		D2DRenderer::Get().DrawBitmap(obj.Get(), rect);
	}
	else if (ebm.size() == 2)
	{
		// �� �� ���� �� �� good/bad ����
		for (const auto& [Name, obj] : ebm) // -> �ӽ÷� �ϰ� �ϴ� 
		{
			D2D1_RECT_F rect{ 17, 22, 1024 + 17, 1024 + 22 };

			if (Name.find("_good") != std::string::npos)
			{
				// good�� �� ó��
				D2DRenderer::Get().DrawBitmap(obj.Get(), rect);
			}
			else if (Name.find("_bad") != std::string::npos)
			{
				// bad�� �� ó��
				D2DRenderer::Get().DrawBitmap(obj.Get(), rect);
			}
			else
			{
				// ���� ���� �̸��̸� ���
				std::cerr << "[WARN] good/bad ���� �Ұ�: " << Name << std::endl;
			}
		}
	}

	D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 90.0f);
	// ���ͷ� ��ȸ�ؼ� ã��

	int gen = GameManager::Get().curGen;
	int targetID = GameManager::Get().arrEndingID[gen - 2];
	auto& EndingVector = ResourceManager::Get().Get_TextBank().EndingVector;
	auto it = std::find_if(EndingVector.begin(), EndingVector.end(),
		[targetID](const TextBank::Ending& e) {
			return e.ID == targetID;
		});
	
	static std::wstring job;
	if (it != EndingVector.end())
	{
		job = StrToWstr(it->job);
	}
	else
	{
	
	}
	if(m_titleShown)
	{
		D2DRenderer::Get().DrawMessageCenter(job.c_str(),
			1080.f, 120.f, 1920.f - 1080.f, 255.f - 120.f, D2D1::ColorF::White);
	}

	D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 30.0f);
	static std::wstring text;
	if (it != EndingVector.end())
	{
		auto txt = ResourceManager::Get().Get_TextBank().replaceGeneration(it->endingText, gen);
		text = StrToWstr(txt);
	}
	else
	{

	}
	if(m_scriptShown)
	{
		D2DRenderer::Get().DrawMessageCenter(text.c_str(),
			1223.f, 255.f, 564.f, 1080.f - 255.f, D2D1::ColorF::White);
	}
}

void Scene_End::CreateObj()
{
	//////////////////////
	// ���� �̹��� ���� ����
	auto transparentImg = ResourceManager::Get().GetTexture("transparent");

	//////////////////////
	//////////////////////
	//////////////////////
	// [1] ���� â

	// 1. �̹��� ���� ����
	auto ����1 = ResourceManager::Get().GetTexture("����", "01");
	// 2. ������Ʈ �����
	auto ����â = std::make_unique<Object>();
	//����â->SetPosition(Vec2(-20, -16));
	����â->SetPosition(Vec2(0, 0));

	// 3.0. ���� ���� ������Ʈ
	auto ����âinfo = ����â->GetRenderInfo();
	����âinfo->SetBitmap(����1.Get());
	// 3. ��� ������Ʈ �����
	auto ����âComp = ����â->AddComponent<BackgroundComponent>(����âinfo);
	����âComp->SetWidth(1098);
	����âComp->SetHeight(1080);
	����âComp->BitmapPush("����â", ����1);
	����âComp->SetCurrentBitmap("����â");

	/// 9
	m_gameObjects.emplace("aa����â", std::move(����â));

	//////////////////////
	//////////////////////
	//////////////////////
	// [2] �̹���
	// ������ ��
	

	//////////////////////
	//////////////////////
	//////////////////////
	// [3] ���� â

	// 1. �̹��� ���� ����
	auto ����3 = ResourceManager::Get().GetTexture("����", "03");
	// 2. ������Ʈ �����
	auto ����â = std::make_unique<Object>();
	����â->SetPosition(Vec2(1080, 0));

	auto ����âinfo = ����â->GetRenderInfo();
	����âinfo->SetBitmap(����3.Get());
	// 3. ��� ������Ʈ �����
	auto ����âComp = ����â->AddComponent<BackgroundComponent>(����âinfo);
	����âComp->SetWidth(843);
	����âComp->SetHeight(1080);
	����âComp->BitmapPush("����â", ����3);
	����âComp->SetCurrentBitmap("����â");

	/// 9
	m_gameObjects.emplace("a����â", std::move(����â));

	//////////////////////
	//////////////////////
	//////////////////////
	// [4] ��ŵ ��ư

	// 1. �̹��� ���� ����
	auto ����4 = ResourceManager::Get().GetTexture("����", "04");
	// 2. ������Ʈ �����
	auto ��ŵ��ư = std::make_unique<Object>();
	��ŵ��ư->SetPosition(Vec2(1827, 960));

	auto ��ŵ��ưinfo = ��ŵ��ư->GetRenderInfo();
	��ŵ��ưinfo->SetBitmap(����4.Get());

	// 3. ��ư ������Ʈ �����
	auto ��ŵ������Ʈ = ��ŵ��ư->AddComponent<ButtonComponent>(��ŵ��ưinfo);
	��ŵ������Ʈ->SetWidth(����4->GetSize().width);
	��ŵ������Ʈ->SetHeight(����4->GetSize().height);

	// 4. ��ư ��Ʈ�� ����
	��ŵ������Ʈ->BitmapPush("��ŵ", ����4);
	��ŵ������Ʈ->BitmapPush("transparent", transparentImg);
	��ŵ������Ʈ->SetCurrentBitmap("transparent");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto ��ŵ������ = ��ŵ��ư->AddComponent<MouseListenerComponent>(
		[��ŵ������Ʈ, this](const MSG& msg) {
			if(m_scriptShown)
			{
				��ŵ������Ʈ->CheckCollision(msg);
				��ŵ������Ʈ->Worked(msg);
			}
		}
	);

	��ŵ������Ʈ->SetOnClickCallback([this]() {
		std::cout << "��ŵ ��ư Ŭ����" << std::endl;
		// ���� ��ŵ ���� �߰�
		
		int temp = GameManager::Get().curGen;

		if (temp == 4)
		{
			temp = 2;
			SafeChangeScene("History" , temp);
		}
		else
		{
			temp++;
			SafeChangeScene("OutGame", temp);
			Inventory* statwin = dynamic_cast<Inventory*>(UIManager::
				Get().GetWindow(UIWindowType::InventoryWindow));

			if (statwin != nullptr)
			{
				statwin->LoadItemDatabase(static_cast<Need_Moment>(temp - 2)); //-> ���� �ʿ��ϳ�?	
			}

		}

		});

	/// 9
	m_gameObjects.emplace("��ŵ��ư", std::move(��ŵ��ư));


	//////////////////////
	//////////////////////
	//////////////////////
	// [6] ��

	// 1. �̹��� ���� ����
	auto ����6 = ResourceManager::Get().GetTexture("����", "06");
	// 2. ������Ʈ �����
	auto �� = std::make_unique<Object>();
	��->SetPosition(Vec2(1223, 255));

	auto ��info = ��->GetRenderInfo();
	��info->SetBitmap(����6.Get());
	// 3. ��� ������Ʈ �����
	auto ��Comp = ��->AddComponent<BackgroundComponent>(��info);
	��Comp->SetWidth(����6->GetSize().width);
	��Comp->SetHeight(����6->GetSize().height);
	��Comp->BitmapPush("��", ����6);
	��Comp->SetCurrentBitmap("��");

	/// 9
	m_gameObjects.emplace("��", std::move(��));

}

void Scene_End::SafeChangeScene(const std::string& sceneName, int gen)
{
	if (!m_isTransitioning)
	{
		m_isTransitioning = true;
		m_nextScene = sceneName;
		m_currentDelay = 0.0f;

		GameManager::Get().curGen = gen;

		// ����� �α�
		std::cout << "�� ��ȯ ����: " << sceneName << std::endl;
	}
}
