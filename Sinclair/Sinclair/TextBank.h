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
    /// ��ƿ��Ƽ �Լ�
    /// string replaceGeneration(const string& text, int generation)
    /// </summary>

    string replaceGeneration(const string& text, int generation);

    /// <summary>
    /// Ending�� ���� ��
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
        int ��ġ;
        string ������;
        string �����ؽ�Ʈ;
    };

    void parseTSV_Ending(const string& filename);


//private: //�ϴ� �ۺ�
public:
     vector<Ending>  EndingVector;


};

