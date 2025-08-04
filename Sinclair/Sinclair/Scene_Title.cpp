#include "Scene_Title.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"

Scene_Title::Scene_Title(string name)
{
		m_name = name;
		Initalize();
}

Scene_Title::~Scene_Title()
{
}

void Scene_Title::Initalize()
{
	if (dirty) return; // �̹� �ʱ�ȭ�Ǿ����� ��ŵ

	CreateObj(); // ������Ʈ ���� (�� ����)
	dirty = true;
}

void Scene_Title::Enter()
{
	// �� ���� �ø��� ������ �͵�
	// ������Ʈ ���� �ʱ�ȭ, ���� ��� ��
	// ResetObjectStates();
	// PlayBGM();
}

void Scene_Title::Exit()
{
	// �� ���� �ø��� ������ �͵�  
	// ���� ����, ���� ���� ��
	//StopBGM();
	//PauseAnimations();
}

void Scene_Title::Clean()
{
	m_gameObjects.clear();
	//SceneAssets.clear();
}

void Scene_Title::Update()
{

}

void Scene_Title::LogicUpdate(float delta)
{

}



void Scene_Title::Render()
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
					bitmap = bgComp->GetBitmap();
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

		//D2DRenderer::Get().CreateWriteResource(L"�ü�ü", DWRITE_FONT_WEIGHT_BOLD, 100.0f);
		D2DRenderer::Get().DrawMessage(L"��Ŭ���� �� �����丮", 147.f, 254.f, 1300.f, 1000.f, D2D1::ColorF::LightPink);
		//D2DRenderer::Get().CreateWriteResource();

		D2DRenderer::Get().RenderEnd();
}


void Scene_Title::CreateObj()
{
	//////////////////////
	// Background 

	// 1. �̹��� ���� ����
	auto gameStartBackground = ResourceManager::Get().GetTexture("����ȭ��");
	// 2. ������Ʈ �����
	auto Background = std::make_unique<Object>();
	Background->SetPosition(Vec2(0, 0));

	// 3. ��� ������Ʈ �����
	auto bgComp = Background->AddComponent<BackgroundComponent>();
	// 3.1.1 ������ �ٸ��� 
	bgComp->SetWidth(1920); bgComp->SetHeight(1080);
	bgComp->BitmapPush("Background", gameStartBackground);
	// 9. ���� ������Ʈ�鿡 ����ֱ�
	m_gameObjects.emplace("Background", std::move(Background));


	//////////////////////
	// �ؽ�Ʈ�� �̷��� ����� �ȵ�!
	// ������ �� ���� ���� �� �� ����

	//////////////////////
	// gameStartButton
	// ���ҽ� �Ŵ��� �ذ��ϱ�

	// 1. �̹��� ���� ����
	auto gameStartButton_texture_normal = ResourceManager::Get().GetTexture("����ȭ��", "2");
	auto gameStartButton_texture_hover = ResourceManager::Get().GetTexture("����ȭ��", "3");
	auto gameStartButton_texture_pressed = ResourceManager::Get().GetTexture("����ȭ��", "4");

	// 2. ������Ʈ �����
	auto gameStartButton = std::make_unique<Object>();
	gameStartButton->SetPosition(Vec2(147, 564));

	// 3. ��ư ������Ʈ �����
	auto gameStartButton_buttonComp = gameStartButton->AddComponent<ButtonComponent>();
	// 3.1.2 ������ ������
	gameStartButton_buttonComp->SetWidth(gameStartButton_texture_normal->GetSize().width);
	gameStartButton_buttonComp->SetHeight(gameStartButton_texture_normal->GetSize().height);

	// 4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	gameStartButton_buttonComp->BitmapPush("normal", gameStartButton_texture_normal);
	gameStartButton_buttonComp->BitmapPush("hover", gameStartButton_texture_hover);
	gameStartButton_buttonComp->BitmapPush("pressed", gameStartButton_texture_pressed);

	// *** �߿�: �ʱ� ��Ʈ�� ���� ***
	gameStartButton_buttonComp->SetCurrentBitmap("normal");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto gameStartButton_mouseListener = gameStartButton->AddComponent<MouseListenerComponent>(
		[gameStartButton_buttonComp](const MSG& msg) {
			gameStartButton_buttonComp->Worked(msg);
		}
	);

	gameStartButton_buttonComp->SetOnClickCallback([this]() {
		SceneManager::Get().ChangeScene("OutGame");
		std::cout << "SceneManager::Get().ChangeScene(\"OutGame\");" << std::endl;
		});

	m_gameObjects.emplace("gameStartButton", std::move(gameStartButton));


	//////////////////////
	// gameSettingButton

		// ���ҽ� �Ŵ���
	auto gameSettingButton_texture_normal = ResourceManager::Get().GetTexture("����ȭ��", "3");
	auto gameSettingButton_texture_hover = ResourceManager::Get().GetTexture("����ȭ��", "4");
	auto gameSettingButton_texture_pressed = ResourceManager::Get().GetTexture("����ȭ��", "2");

	auto
		gameSettingButton = std::make_unique<Object>();
	gameSettingButton->SetPosition(Vec2(147, 697));

	auto gameSettingButton_buttonComp = gameSettingButton->AddComponent<ButtonComponent>();
	// 3.1.2 ������ ������
	gameSettingButton_buttonComp->SetWidth(gameStartButton_texture_normal->GetSize().width);
	gameSettingButton_buttonComp->SetHeight(gameStartButton_texture_normal->GetSize().height);
	auto gameSettingButton_mouseListener = gameSettingButton->AddComponent<MouseListenerComponent>(
		// ���⼭ ��� �Է� ó�� ����?
		[gameSettingButton_buttonComp](const MSG& msg) {
			gameSettingButton_buttonComp->Worked(msg);
		}
	);

	// ��ư ����
	gameSettingButton_buttonComp->BitmapPush("normal", gameSettingButton_texture_normal);
	gameSettingButton_buttonComp->BitmapPush("hover", gameSettingButton_texture_hover);
	gameSettingButton_buttonComp->BitmapPush("pressed", gameSettingButton_texture_pressed);
	gameSettingButton_buttonComp->SetOnClickCallback([this]() {
		SceneManager::Get().ChangeScene("Title");
		std::cout << "The button is pressed." << std::endl;
		});

	m_gameObjects.emplace("gameSettingButton", std::move(gameSettingButton));

	//////////////////////
	// creditButton

	// ���ҽ� �Ŵ���
	auto creditButton_texture_normal = ResourceManager::Get().GetTexture("����ȭ��", "4");
	auto creditButton_texture_hover = ResourceManager::Get().GetTexture("����ȭ��", "2");
	auto creditButton_texture_pressed = ResourceManager::Get().GetTexture("����ȭ��", "3");

	auto
		creditButton = std::make_unique<Object>();
	creditButton->SetPosition(Vec2(147, 830));

	auto creditButton_buttonComp = creditButton->AddComponent<ButtonComponent>();
	// 3.1.2 ������ ������
	creditButton_buttonComp->SetWidth(gameStartButton_texture_normal->GetSize().width);
	creditButton_buttonComp->SetHeight(gameStartButton_texture_normal->GetSize().height);
	auto creditButton_mouseListener = creditButton->AddComponent<MouseListenerComponent>(
		// ���⼭ ��� �Է� ó�� ����?
		[creditButton_buttonComp](const MSG& msg) {
			creditButton_buttonComp->Worked(msg);
		}
	);

	// ��ư ����
	creditButton_buttonComp->BitmapPush("normal", creditButton_texture_normal);
	creditButton_buttonComp->BitmapPush("hover", creditButton_texture_hover);
	creditButton_buttonComp->BitmapPush("pressed", creditButton_texture_pressed);
	creditButton_buttonComp->SetOnClickCallback([this]() {
		//SceneManager::Get().ChangeScene("Title");
		std::cout << "The button is pressed." << std::endl;
		});

	m_gameObjects.emplace("creditButton", std::move(creditButton));

}
