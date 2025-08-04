#include "Scene_hyoje.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"

Scene_hyoje::Scene_hyoje(string name)
{
	m_name = name;



	CreateObj();
	

}

Scene_hyoje::~Scene_hyoje()
{
}

void Scene_hyoje::Render() 
{
	D2DRenderer::Get().RenderBegin();
	
	for (const auto& [Name, obj] : m_gameObjects)
	{

		ComPtr<ID2D1Bitmap1> bitmap = nullptr;
		D2D1_RECT_F dest;
		dest.left =   0;
		dest.top =    0;
		dest.right = 0;
		dest.bottom = 0;

		// ButtonComponent �켱 Ȯ��
		auto buttonComp = obj->GetComponent<ButtonComponent>();
		if (buttonComp != nullptr) {
			bitmap = buttonComp->GetBitmap();
			dest.right += buttonComp-> GetWidth();
			dest.bottom += buttonComp->GetHeight();
		}
		else {
			// ButtonComponent�� ������ BackgroundComponent Ȯ��
			auto bgComp = obj->GetComponent<BackgroundComponent>();
			if (bgComp != nullptr) {
				bitmap = bgComp->GetBitmap(); 
				dest.right +=  bgComp->GetWidth();
				dest.bottom += bgComp->GetHeight();
			}
		}

		if (!bitmap) {
			continue;
		}

		auto size = bitmap->GetSize();

		dest.left   += obj->GetTransform().m_position.x;
		dest.top    += obj->GetTransform().m_position.y;
		dest.right  +=			dest.left;
		dest.bottom +=			dest.top ;

		D2DRenderer::Get().DrawBitmap(bitmap.Get(), dest);
	}

	D2DRenderer::Get().CreateWriteResource(L"�ü�ü", DWRITE_FONT_WEIGHT_BOLD, 100.0f);
	D2DRenderer::Get().DrawMessage(L"��Ŭ���� �� �����丮", 147.f, 254.f, 1300.f, 1000.f, D2D1::ColorF::LightPink);
	D2DRenderer::Get().CreateWriteResource();

	D2DRenderer::Get().RenderEnd();
}

void Scene_hyoje::CreateObj()
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
	auto gameStartButton_texture_normal = ResourceManager::Get().GetTexture("����ȭ��_2");
	auto gameStartButton_texture_hover = ResourceManager::Get().GetTexture("����ȭ��_3");
	auto gameStartButton_texture_pressed = ResourceManager::Get().GetTexture("����ȭ��_4");

	// 2. ������Ʈ �����
	auto gameStartButton = std::make_unique<Object>();
	gameStartButton->SetPosition(Vec2(147, 564));

	// 3. ��ư ������Ʈ �����
	auto gameStartButton_buttonComp = gameStartButton->AddComponent<ButtonComponent>();
	// 3.1.2 ������ ������
	gameStartButton_buttonComp->SetWidth(gameStartButton_texture_normal->GetSize().width);
	gameStartButton_buttonComp->SetHeight(gameStartButton_texture_normal->GetSize().height);
	
	// 4. ��ư ��Ʈ�� ����
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
		//SceneManager::Get().ChangeScene("Title");
		std::cout << "The button is pressed." << std::endl;
		});

	m_gameObjects.emplace("gameStartButton", std::move(gameStartButton));


	//////////////////////
	// gameSettingButton
	
		// ���ҽ� �Ŵ���
	auto gameSettingButton_texture_normal = ResourceManager::Get().GetTexture("����ȭ��_3");
	auto gameSettingButton_texture_hover = ResourceManager::Get().GetTexture("����ȭ��_2");
	auto gameSettingButton_texture_pressed = ResourceManager::Get().GetTexture("����ȭ��_4");

	auto 
	gameSettingButton = std::make_unique<Object>();
	gameSettingButton->SetPosition(Vec2(147, 697));

	auto gameSettingButton_buttonComp =    gameSettingButton->AddComponent<ButtonComponent>();
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
	gameSettingButton_buttonComp->BitmapPush("normal",  gameSettingButton_texture_normal);
	gameSettingButton_buttonComp->BitmapPush("hover",   gameSettingButton_texture_hover);
	gameSettingButton_buttonComp->BitmapPush("pressed", gameSettingButton_texture_pressed);
	gameSettingButton_buttonComp->SetOnClickCallback([this]() {
		//SceneManager::Get().ChangeScene("Title");
		std::cout << "The button is pressed." << std::endl;
		});

	m_gameObjects.emplace("gameSettingButton", std::move(gameSettingButton));

	//////////////////////
	// creditButton

	// ���ҽ� �Ŵ���
	auto creditButton_texture_normal = ResourceManager::Get().GetTexture("����ȭ��_4");
	auto creditButton_texture_hover = ResourceManager::Get().GetTexture("����ȭ��_2");
	auto creditButton_texture_pressed = ResourceManager::Get().GetTexture("����ȭ��_3");

	auto
	creditButton = std::make_unique<Object>();
	creditButton->SetPosition(Vec2(147, 830));

	auto creditButton_buttonComp =    creditButton->AddComponent<ButtonComponent>();
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
	creditButton_buttonComp->BitmapPush("normal",  creditButton_texture_normal);
	creditButton_buttonComp->BitmapPush("hover",   creditButton_texture_hover);
	creditButton_buttonComp->BitmapPush("pressed", creditButton_texture_pressed);
	creditButton_buttonComp->SetOnClickCallback([this]() {
		//SceneManager::Get().ChangeScene("Title");
		std::cout << "The button is pressed." << std::endl;
		});

	m_gameObjects.emplace("gameSettingButton", std::move(creditButton));


}

