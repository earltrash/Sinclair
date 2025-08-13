#include "Scene_End.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "UIManager.h"
#include "GameManager_2.h"


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
		UIManager::Get().AddSceneObject(obj); // 1011 1. �ϴ� id�� �޾ƾ� �� 2. �뷡�� ���� id ��ȯ
	}


	int id = GameManager::Get().arrEndingID[GameManager::Get().GetCurrentGen() - 2];
	PlayEDM(id);

	

	//id�� �ް� ������ �´� ���尡 ���;� ��. id -> index -> vector = channel ������ 


}

void Scene_End::Enter()
{
	ebm = GameManager::Get().AftAdv();


	m_fadeStarted = true;
	m_currentTime = 0.f;

	Initalize();

	int gen = GameManager::Get().curGen;
	int targetID = GameManager::Get().arrEndingID[gen - 2];
	auto& EndingVector = ResourceManager::Get().Get_TextBank().EndingVector;
	auto it = std::find_if(EndingVector.begin(), EndingVector.end(),
		[targetID](const TextBank::Ending& e) {
			return e.ID == targetID;
		});


	if (it != EndingVector.end())
	{
		job = StrToWstr(it->job);
	}
	else
	{
		job = L"���µ���";
	}
	if (it != EndingVector.end())
	{
		auto txt = ResourceManager::Get().Get_TextBank().replaceGeneration(it->endingText, gen);
		text = StrToWstr(txt);
	}
	else
	{
		text = L"���µ���";
	}
}

void Scene_End::Exit()
{
	Clean();

	m_image1Opacity = 0.f;
	m_image2Opacity = 0.f;
	m_image3Opacity = 0.f;
	after_fadeStarted = false;
	m_fadeStarted = false;
	m_isTransitioning = false;
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
			m_nextScene = "";
			m_currentDelay = 0.0f;


		}
	}

	if (m_fadeStarted)
	{
		m_currentTime += 0.016f;

		// ù ��° �̹���: 0��~1.5�� ���̵���, 3��~4.5�� ���̵�ƿ�
		if (m_currentTime <= m_fadeDuration)
		{
			m_image1Opacity = std::clamp(m_currentTime / m_fadeDuration, 0.f, 1.f);
		}

		// �� ��° �̹���: 0.5��~2�� ���̵���, 3.5��~5�� ���̵�ƿ�  
		if (m_currentTime >= m_intervalTime && m_currentTime <= m_intervalTime + m_fadeDuration)
		{
			m_image2Opacity = std::clamp((m_currentTime - m_intervalTime) / m_fadeDuration, 0.f, 1.f);
		}

		// �� ��° �̹���: 1��~2.5�� ���̵���, 4��~5.5�� ���̵�ƿ�
		if (m_currentTime >= m_intervalTime * 2 && m_currentTime <= m_intervalTime * 2 + m_fadeDuration)
		{
			m_image3Opacity = std::clamp((m_currentTime - m_intervalTime * 2) / m_fadeDuration, 0.f, 1.f);
		}

		// ��� ���̵� �Ϸ� üũ
		if (m_currentTime >= 5.5f)
		{
			m_fadeStarted = false;
			m_gameObjects["��ŵ��ư"]->GetComponent<ButtonComponent>()->SetCurrentBitmap("��ŵ");
			after_fadeStarted = true;
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
		D2D1_RECT_F rect{ 17, 22, 1024 + 17, 1024 +22 };
		D2D1_RECT_F srcRect{ 0, 0, 1024 , 1024 };
		D2DRenderer::Get().DrawBitmap(obj.Get(), rect, srcRect, m_image1Opacity);
	}
	else if (ebm.size() == 2)
	{
		// �� �� ���� �� �� good/bad ����
		for (const auto& [Name, obj] : ebm) // -> �ӽ÷� �ϰ� �ϴ� 
		{
			D2D1_RECT_F rect{ 17, 22, 1024 + 17, 1024 + 22 };
			D2D1_RECT_F srcRect{ 0, 0, 1024 , 1024 };

			if (Name.find("_good") != std::string::npos)
			{
				// good�� �� ó��
				D2DRenderer::Get().DrawBitmap(obj.Get(), rect, srcRect, m_image1Opacity);

			}
			else if (Name.find("_bad") != std::string::npos)
			{
				// bad�� �� ó��
				D2DRenderer::Get().DrawBitmap(obj.Get(), rect, srcRect, m_image1Opacity);
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



	D2DRenderer::Get().DrawMessageCenter(job.c_str(),
		1080.f, 120.f, 1920.f - 1080.f, 255.f - 120.f, D2D1::ColorF::White, m_image3Opacity);
	

	D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 30.0f);
	

	D2DRenderer::Get().DrawMessageCenter(text.c_str(),
		1223.f, 255.f, 564.f, 1080.f - 255.f, D2D1::ColorF::White, m_image2Opacity);
	
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
			if(after_fadeStarted)
			{
				��ŵ������Ʈ->CheckCollision(msg);
				��ŵ������Ʈ->Worked(msg);
			}
		}
	);

	��ŵ������Ʈ->SetOnClickCallback([this]() {
		
		if (m_isTransitioning)
		{
			return;
		}

		std::cout << "��ŵ ��ư Ŭ����" << std::endl;
		// ���� ��ŵ ���� �߰�
		
		int temp = GameManager::Get().curGen;

		if (temp == 4)
		{
			temp = 2;
			SafeChangeScene("History" , temp);

			SoundManager::Instance().PauseBGM(cur_EDM_ID, true);
		}
		else
		{
			temp++;
			SafeChangeScene("OutGame", temp);
			//SoundManager::Instance().PauseBGM(cur_EDM_ID, true);
			SoundManager::Instance().CrossfadeBGM(SoundManager::Instance().Get_Playing_Key(), "OutGame", 2.5f);

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

void Scene_End::PlayEDM(int id)
{
	//string sid = to_string(id);

	string idStr = to_string(id); // "1011"
	string first3 = idStr.substr(0, 3); // "101"

	string EDMID = first3 + "1";

	SoundManager::Instance().PlayBGM(EDMID, false);

	cur_EDM_ID = EDMID;

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
