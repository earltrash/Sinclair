#include "Scene_Outgame.h"

Scene_Outgame::Scene_Outgame(string name)
{
		m_name = name;
}

Scene_Outgame::~Scene_Outgame()
{
}

void Scene_Outgame::Initalize()
{
		GetAsset(m_name);

}

void Scene_Outgame::Clean()
{
		m_gameObjects.clear();
		SceneAssets.clear();
}

void Scene_Outgame::Update()
{
}

void Scene_Outgame::LogicUpdate(float delta)
{
}

void Scene_Outgame::Enter()
{
		Initalize();
}

void Scene_Outgame::Exit()
{
		Clean();
}

void Scene_Outgame::Render()
{
}
