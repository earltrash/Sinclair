#include "Scene_hyoje.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"

Scene_hyoje::Scene_hyoje(string name)
{
	m_name = name;



	CreateBackground();
	CreateButtons();
	

}

Scene_hyoje::~Scene_hyoje()
{
}

void Scene_hyoje::Render() 
{
	D2DRenderer::Get().RenderBegin();
	
	for (const auto& [Name, obj] : m_gameObjects)
	{
		auto buttonComp = obj->GetComponent<ButtonComponent>();
		//if (buttonComp == nullptr) {
		//	std::cout << "[LOG] " << Name << " : ButtonComponent is nullptr\n";
		//	continue;
		//}

		auto bitmap = buttonComp->GetBitmap();
		//if (!bitmap) {
		//	std::cout << "[LOG] " << Name << " : bitmap is nullptr\n";
		//	continue;
		//}

		auto size = bitmap->GetSize();
		D2D1_RECT_F dest;
		dest.left = obj->GetTransform().m_position.x;
		dest.top = obj->GetTransform().m_position.y;
		dest.right = dest.left + size.width;
		dest.bottom = dest.top + size.height;

		D2DRenderer::Get().DrawBitmap(bitmap.Get(), dest);
	}

	D2DRenderer::Get().RenderEnd();
}

void Scene_hyoje::CreateButtons()
{
	//////////////////////
	// gameStartButton
	
	// ���ҽ� �Ŵ���
	auto gameStartButton_texture_normal = ResourceManager::Get().GetTexture("����ȭ��_2");
	auto gameStartButton_texture_hover = ResourceManager::Get().GetTexture("����ȭ��_3");
	auto gameStartButton_texture_pressed = ResourceManager::Get().GetTexture("����ȭ��_4");

	auto gameStartButton = std::make_unique<Object>();
	gameStartButton->SetPosition(Vec2(147, 564));

	auto gameStartButton_buttonComp = gameStartButton->AddComponent<ButtonComponent>();
	auto gameStartButton_mouseListener = gameStartButton->AddComponent<MouseListenerComponent>(
		[gameStartButton_buttonComp](const MSG& msg) {
			gameStartButton_buttonComp->Worked(msg);
		}
	);

	// ��ư ����
	gameStartButton_buttonComp->BitmapPush("normal", gameStartButton_texture_normal);
	gameStartButton_buttonComp->BitmapPush("hover", gameStartButton_texture_hover);
	gameStartButton_buttonComp->BitmapPush("pressed", gameStartButton_texture_pressed);
	gameStartButton_buttonComp->SetOnClickCallback([this]() {
		//SceneManager::Get().ChangeScene("Title");
		std::cout << "The button is pressed." << std::endl;
		});

	m_gameObjects.emplace("gameStartButton", std::move(gameStartButton));


	//////////////////////
	// gameSettingButton
		// ���ҽ� �Ŵ���
	auto gameSettingButton_texture_normal = ResourceManager::Get().GetTexture("����ȭ��_2");
	auto gameSettingButton_texture_hover = ResourceManager::Get().GetTexture("����ȭ��_3");
	auto gameSettingButton_texture_pressed = ResourceManager::Get().GetTexture("����ȭ��_4");

	auto 
	gameSettingButton = std::make_unique<Object>();
	gameSettingButton->SetPosition(Vec2(147, 564));

	auto gameSettingButton_buttonComp =    gameSettingButton->AddComponent<ButtonComponent>();
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

}

void Scene_hyoje::CreateBackground()
{
}
