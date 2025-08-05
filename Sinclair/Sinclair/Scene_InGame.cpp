#include "Scene_InGame.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"


Scene_InGame::Scene_InGame(string name)
{
	m_name = name;
	Initalize();
}

Scene_InGame::~Scene_InGame()
{
	Clean();
}

void Scene_InGame::Initalize()
{
	if (dirty) return; // �̹� �ʱ�ȭ�Ǿ����� ��ŵ

	CreateObj(); // ������Ʈ ���� (�� ����)
	dirty = true;
}

void Scene_InGame::Clean()
{
	m_gameObjects.clear();

	dirty = false;
}

void Scene_InGame::Update()
{
}

void Scene_InGame::LogicUpdate(float delta)
{
}

void Scene_InGame::Enter()
{
}

void Scene_InGame::Exit()
{
}

void Scene_InGame::Render()
{
	D2DRenderer::Get().RenderBegin();
	
	for (const auto& [Name, obj] : m_gameObjects)
	{

		ComPtr<ID2D1Bitmap1> bitmap = nullptr;
		D2D1_RECT_F dest;
		dest.left = 0;
		dest.top = 0;
		dest.right = 0;
		dest.bottom = 0;
		// ���� �����Ѵٸ�...
		float opacity = 1.0f;

		// ButtonComponent �켱 Ȯ��
		auto buttonComp = obj->GetComponent<ButtonComponent>();
		if (buttonComp != nullptr) {
			bitmap = buttonComp->GetBitmap();
			dest.right += buttonComp->GetWidth();
			dest.bottom += buttonComp->GetHeight();
			opacity = buttonComp->m_opacity;
		}
		else {
			// ButtonComponent�� ������ BackgroundComponent Ȯ��
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

		dest.left += obj->GetTransform().m_position.x;
		dest.top += obj->GetTransform().m_position.y;
		dest.right += dest.left;
		dest.bottom += dest.top;

		// ���� ���� x
		//D2DRenderer::Get().DrawBitmap(bitmap.Get(), dest);
		// ���� ���� o
		D2D1_RECT_F srcRect = D2D1::RectF(0, 0, bitmap->GetSize().width, bitmap->GetSize().height); ;

		D2DRenderer::Get().DrawBitmap(bitmap.Get(), dest, srcRect, opacity);
	}

	D2DRenderer::Get().CreateWriteRegularResource();
	//D2DRenderer::Get().DrawMessage(L"�ΰ���", 503.f, 805.f, 1300.f, 1000.f, D2D1::ColorF::BlueViolet);
	//D2DRenderer::Get().DrawMessage(L"�ΰ���", 503.f, 869.f, 1300.f, 1000.f, D2D1::ColorF::BlueViolet);

	D2DRenderer::Get().RenderEnd();
}

void Scene_InGame::CreateObj()
{
	//////////////////////
	// Background 

	// 1. �̹��� ���� ����
	auto inGameBackground = ResourceManager::Get().GetTexture("�ΰ���");
	// 2. ������Ʈ �����
	auto Background = std::make_unique<Object>();
	Background->SetPosition(Vec2(0, 0));

	// 3. ��� ������Ʈ �����
	auto bgComp = Background->AddComponent<BackgroundComponent>();
	// 3.1.1 ������ �ٸ��� 
	bgComp->SetWidth(1920); bgComp->SetHeight(1080);
	bgComp->BitmapPush("Background", inGameBackground);

	bgComp->SetCurrentBitmap("Background");

	// 9. ���� ������Ʈ�鿡 ����ֱ�
	m_gameObjects.emplace("Background", std::move(Background));




	/////////////////////
	/////////////////////
	// â��

	// 1. �̹��� ���� ����
	auto �ΰ���2 = ResourceManager::Get().GetTexture("�ΰ���", "2");
	// 2. ������Ʈ �����
	auto â�� = std::make_unique<Object>();
	â��->SetPosition(Vec2(147, 570));
	// 3. ��ư ������Ʈ �����
	auto â���ư = â��->AddComponent<ButtonComponent>();
	â���ư->SetWidth(310); â���ư->SetHeight(64);

	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	â���ư->BitmapPush("normal",  �ΰ���2  );
	â���ư->BitmapPush("hover",   �ΰ���2  );
	â���ư->BitmapPush("pressed", �ΰ���2);

	â���ư->SetCurrentBitmap("normal");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto â������ = â��->AddComponent<MouseListenerComponent>(
		[â���ư](const MSG& msg) {
			â���ư->Worked(msg);
		}
	);

	â���ư->SetOnClickCallback([this]() {
		std::cout << "â��" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("�ΰ���2", std::move(â��));



	/////////////////////
	/////////////////////
	// ���

	// 1. �̹��� ���� ����
	auto �ΰ���3 = ResourceManager::Get().GetTexture("�ΰ���", "3");
	// 2. ������Ʈ �����
	auto ��� = std::make_unique<Object>();
	���->SetPosition(Vec2(147, 703));
	// 3. ��ư ������Ʈ �����
	auto ����ư = ���->AddComponent<ButtonComponent>();
	����ư->SetWidth(310); ����ư->SetHeight(64);

	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	����ư->BitmapPush("normal",  �ΰ���3);
	����ư->BitmapPush("hover",   �ΰ���3);
	����ư->BitmapPush("pressed", �ΰ���3);

	����ư->SetCurrentBitmap("normal");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto ��񸮽��� = ���->AddComponent<MouseListenerComponent>(
		[����ư](const MSG& msg) {
			����ư->Worked(msg);
		}
	);

	����ư->SetOnClickCallback([this]() {
		std::cout << "����ư" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("�ΰ���3", std::move(���));



	/////////////////////
	/////////////////////
	// �������ͽ�

	// 1. �̹��� ���� ����
	auto �ΰ���4 = ResourceManager::Get().GetTexture("�ΰ���", "4");
	// 2. ������Ʈ �����
	auto �������ͽ� = std::make_unique<Object>();
	�������ͽ�->SetPosition(Vec2(147, 836));
	// 3. ��ư ������Ʈ �����
	auto �������ͽ���ư = �������ͽ�->AddComponent<ButtonComponent>();
	�������ͽ���ư->SetWidth(310); �������ͽ���ư->SetHeight(64);

	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	�������ͽ���ư->BitmapPush("normal", �ΰ���4);
	�������ͽ���ư->BitmapPush("hover",  �ΰ���4);
	�������ͽ���ư->BitmapPush("pressed",�ΰ���4);

	�������ͽ���ư->SetCurrentBitmap("normal");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto �������ͽ������� = �������ͽ�->AddComponent<MouseListenerComponent>(
		[�������ͽ���ư](const MSG& msg) {
			�������ͽ���ư->Worked(msg);
		}
	);

	�������ͽ���ư->SetOnClickCallback([this]() {
		std::cout << "�������ͽ���ư" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("�ΰ���4", std::move(�������ͽ�));



	/////////////////////
	/////////////////////
	// �ڷΰ��� (�ƿ���������)

	// 1. �̹��� ���� ����
	auto �ڷΰ��� = ResourceManager::Get().GetTexture("�ڷΰ���");
	// 2. ������Ʈ �����
	auto �ڷ� = std::make_unique<Object>();
	�ڷ�->SetPosition(Vec2(64, 57));
	// 3. ��ư ������Ʈ �����
	auto backComp = �ڷ�->AddComponent<ButtonComponent>();
	backComp->SetWidth(37); backComp->SetHeight(37);

	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	backComp->BitmapPush("normal", �ڷΰ���);
	backComp->BitmapPush("hover", �ڷΰ���);
	backComp->BitmapPush("pressed", �ڷΰ���);

	backComp->SetCurrentBitmap("normal");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto Back_mouseListener = �ڷ�->AddComponent<MouseListenerComponent>(
		[backComp](const MSG& msg) {
			backComp->Worked(msg);
		}
	);

	backComp->SetOnClickCallback([this]() {
		SceneManager::Get().ChangeScene("OutGame");
		std::cout << "�ڷΰ��� (�ƿ��������� )" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("�ڷΰ���", std::move(�ڷ�));
}