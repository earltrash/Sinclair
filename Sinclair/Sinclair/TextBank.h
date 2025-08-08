#pragma once
#include "pch.h"
using namespace std;
#include <fstream>
#include <sstream>
#include <optional>

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
    
    struct Ending
    {
        //int ID;
        //pair<string, optional<string>> positive;
        //optional<string> negative;
        //int fame;
        //string job;
        //vector<string> endingTexts;

        int ID;
        string positive;
        string negative;
        int 명성치;
        string 직업명;
        string 엔딩텍스트;
    };

    void parseTSV_Ending(const string& filename);


//private: //일단 퍼블릭
public:
     vector<Ending>  EndingVector;


};

