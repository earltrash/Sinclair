#include "GameManager.h"

GameManager& GameManager::Get()
{
    static GameManager resourcemanager;
    return resourcemanager;
}

void GameManager::SetGen()
{
}
