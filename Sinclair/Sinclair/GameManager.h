#pragma once
#include "pch.h"
#include "status.h"
#include <array>


class GameManager
{
private:
    GameManager() = default;
    ~GameManager() = default;
public:
    

    static GameManager& Get();

public:// get set ����� ��
    bool isFirst = true;
    int curGen = 2;
    std::array<TotalStatus, 3> arrTotalStatus{};
    
    // soundmanager�� ������
    float bgmValue = 1.0f;
    float sfxValue = 1.0f;
};

