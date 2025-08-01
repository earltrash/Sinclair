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



public:// set, get ����� ��

	// ����
	float gameTime; // ��� ��� ������Ʈ ���� 

	int cur_gen; // ���� �������
	std::string cur_scene; // ���� ��

	int stat[3][5]; //3������ 5���� ����
	int fame; // ��

	// �ݵ�� �����ϰ� ����. ���߿�~~
	// ������ġ
	std::string saveFilePath = "Resource\\record.txt";
};

