#pragma once
#include "pch.h"
#include "status.h"
#include <array>

using namespace std;

class GameManager
{
private:
    GameManager() = default;
    ~GameManager() = default;
public:


    void Initalize();
    static GameManager& Get();


    void SetGen();
    string Synthesis(const string& id1, const string& id2);



public:// get set ����� ��
    bool isFirst = true;
    int curGen = 2;

    // soundmanager�� ������
    float bgmValue = 1.0f;
    float sfxValue = 1.0f;
    
    std::vector<Recipe> m_SynTable;
    std::array<TotalStatus, 3> arrTotalStatus{};


};