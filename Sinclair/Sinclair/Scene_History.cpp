#include "Scene_History.h"
#include "Button.h"

#include "pch.h"

#include "MouseListenerComponent.h"
#include "InputManager.h"
#include "UI_Renderer.h"
#include "Renderer.h"
#include "SpriteRenderer.h"


using path = std::filesystem::path;

Scene_History::Scene_History(string name)
{
	m_name = name;
}

Scene_History::~Scene_History()
{

}

void Scene_History::Initalize()
{
	//GetAsset(m_name); 
}

void Scene_History::Clean()
{
	m_gameObjects.clear();
	SceneAssets.clear();
}

void Scene_History::Update()
{

}

void Scene_History::LogicUpdate(float delta)
{

}

void Scene_History::Enter()
{
	Initalize();
}

void Scene_History::Exit()
{
}