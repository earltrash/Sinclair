#include "Scene_Title.h"
#include "Button.h"

#include "pch.h"

#include "MouseListenerComponent.h"
#include "InputManager.h"
#include "UI_Renderer.h"
#include "Renderer.h"
#include "SpriteRenderer.h"


using path = std::filesystem::path;

Scene_Title::Scene_Title(string name)
{
		m_name = name;
}

Scene_Title::~Scene_Title()
{

}

void Scene_Title::Initalize()
{
		GetAsset(m_name);
}

void Scene_Title::Clean()
{
		m_gameObjects.clear();
		SceneAssets.clear();
}

void Scene_Title::Update()
{

}

void Scene_Title::LogicUpdate(float delta)
{

}

void Scene_Title::Enter()
{
		Initalize();
}

void Scene_Title::Exit()
{
}