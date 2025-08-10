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

public:// get set 만들기 전
    bool isFirst = true;
    int curGen = 2;
    std::array<TotalStatus, 3> arrTotalStatus{};
    
    // soundmanager야 가져가
    float bgmValue = 1.0f;
    float sfxValue = 1.0f;
};

