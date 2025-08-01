#include "Scene_End.h"
#include "Button.h"

#include "pch.h"

#include "MouseListenerComponent.h"
#include "InputManager.h"
#include "UI_Renderer.h"
#include "Renderer.h"
#include "SpriteRenderer.h"


using path = std::filesystem::path;

Scene_End::Scene_End(string name)
{
	m_name = name;
}

Scene_End::~Scene_End()
{

}

void Scene_End::Initalize()
{
	GetAsset(m_name);
}

void Scene_End::Clean()
{
	m_gameObjects.clear();
	SceneAssets.clear();
}

void Scene_End::Update()
{

}

void Scene_End::LogicUpdate(float delta)
{

}

void Scene_End::Enter()
{
	Initalize();
}

void Scene_End::Exit()
{
}