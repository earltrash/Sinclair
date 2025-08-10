#pragma once
#include "pch.h"
#include "status.h"
#include <array>

using namespace std;

struct StatCondition {
    Status_Total stat;
    int value;
    bool isPositive; // true면 positive 조건, false면 negative 조건
};
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


    //엔딩 구하려고 함
    void FindEnding();

public:// get set 만들기 전
    bool isFirst = true;
    int curGen = 2;

    // soundmanager야 가져가
    float bgmValue = 1.0f;
    float sfxValue = 1.0f;
    
    std::vector<Recipe> m_SynTable;
    
    // 엔딩에 필요한 숫자
    std::array<TotalStatus, 3> arrTotalStatus{};
    std::array<int, 3> arrEndingID{};
    std::array<int, 3> arrFame{};

};