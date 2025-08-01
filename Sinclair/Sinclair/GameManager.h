#pragma once
#include "pch.h"

class GameManager
{
private:
	GameManager();
	~GameManager() = default;
public:
	static GameManager& Get();
	void GameStart();
	void Finalize();

	float& getGameTime() { return gameTime; }
	void setGameTime(float t) { gameTime += t; }



public:// set, get 만들기 전

	// 게임
	float gameTime; // 얘는 계속 업데이트 받자 

	int cur_gen; // 현재 몇세대인지
	std::string cur_scene; // 현재 씬

	int stat[3][5]; //3세대의 5개의 스탯
	int fame; // 명성

	// 반드시 저장하고 만다. 나중에~~
	// 저장위치
	std::string saveFilePath = "Resource\\record.txt";
};

