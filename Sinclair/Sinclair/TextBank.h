#pragma once
#include "pch.h"
using namespace std;
#include <fstream>
#include <sstream>
#include <optional>
#include "Status.h"

class TextBank
{
public:
    TextBank() = default;
    ~TextBank()= default;

    /// <summary>
    /// 유틸리티 함수
    /// string replaceGeneration(const string& text, int generation)
    /// </summary>

    string replaceGeneration(const string& text, int generation);

    /// <summary>
    /// Ending에 관한 것
    /// struct Ending
    /// vector<Ending> parseTSV(const string& filename)
    /// </summary>

    // 이걸로 바꿔야 함
    struct Ending
    {
        int ID;
        pair<Status_Total, Status_Total> positive;
        Status_Total negative; // ? 면 없는걸로
        int fame;
        string job;
        //vector<string> endingTexts;
        string endingText;
    };

    void parseTSV_Ending(const string& filename);
    
    // 문자열을 스탯 타입으로 변환하는 함수
    Status_Total stringToStatType(string statStr)
    {
        if (statStr == "근력") return Status_Total::Strength;
        else if (statStr == "마력") return Status_Total::Magic_Power;
        else if (statStr == "건강") return Status_Total::Health;
        else if (statStr == "지식") return Status_Total::Knowledge;
        else if (statStr == "매력") return Status_Total::Charm;
        //else if (statStr == "상") return Status_Total::Sang;
        //else if (statStr == "중") return Status_Total::Jung;
        //else if (statStr == "하") return Status_Total::Ha;
        else return Status_Total::Null; // 기본값 ? 그런애들
    }


//private: //일단 퍼블릭
public:
     vector<Ending>  EndingVector;
};

