#include "pch.h"
#include "GameManager.h"
#include <fstream>

GameManager::GameManager()
	: gameTime(0)
{

}

GameManager& GameManager::Get() {
	static GameManager gamemanager;
	return gamemanager;
}

void GameManager::GameStart()
{
}

