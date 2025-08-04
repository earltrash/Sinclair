#include "Scene_MK.h"
#include "Button.h"

#include "pch.h"
#include "BackgroundComponent.h"
#include "ButtonComponent.h"
#include "MouseListenerComponent.h"
#include "InputManager.h"
#include "UI_Renderer.h"
#include "Renderer.h"
#include "SpriteRenderer.h"
#include "RenderInfo.h"

using path = std::filesystem::path;

Scene_MK::Scene_MK(string name)
{
	m_name = name;
}

Scene_MK::~Scene_MK()
{
}

void Scene_MK::Initalize()
{
	//////////////////////
	// Background 

	// 1. �̹��� ���� ����
	auto gameStartBackground = ResourceManager::Get().GetTexture("����ȭ��");
	// 2. ������Ʈ �����
	auto Background = std::make_unique<Object>();
	//Background->SetPosition(Vec2(0, 0));
	// 3. ������Ʈ �ֱ�
	auto renderinfo = Background->AddComponent<RenderInfo>();	// �������� ������Ʈ
	auto bgComp = Background->AddComponent<BackgroundComponent>(renderinfo);
	// 3.1.1 ������ �ٸ��� 
	//bgComp->SetWidth(1920); bgComp->SetHeight(1080);
	bgComp->BitmapPush("Background", gameStartBackground);
	
	// 9. ���� ������Ʈ�鿡 ����ֱ�
	m_gameObjects.emplace("Background", std::move(Background));

}

void Scene_MK::Clean()
{
	m_gameObjects.clear();
	SceneAssets.clear();
}

void Scene_MK::Update()
{
	for (auto& obj : m_gameObjects)
	{
		obj.second->Update();
	}
}

void Scene_MK::LogicUpdate(float delta)
{
	for (auto& obj : m_gameObjects)
	{
		obj.second->FixedUpdate(delta);
	}
}

void Scene_MK::Enter()
{
	Initalize();
}

void Scene_MK::Exit()
{
}
using Vec2F = MYHelper::Vector2F;

void Scene_MK::Render()
{
	D2DRenderer::Get().RenderBegin();

	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetComponent<RenderInfo>()->GetRenderInfo());
	}

	D2DRenderer::Get().RenderEnd();
}