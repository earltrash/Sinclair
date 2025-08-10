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
    /// ��ƿ��Ƽ �Լ�
    /// string replaceGeneration(const string& text, int generation)
    /// </summary>

    string replaceGeneration(const string& text, int generation);

    /// <summary>
    /// Ending�� ���� ��
    /// struct Ending
    /// vector<Ending> parseTSV(const string& filename)
    /// </summary>

    // �̰ɷ� �ٲ�� ��
    struct Ending
    {
        int ID;
        pair<Status_Total, Status_Total> positive;
        Status_Total negative; // ? �� ���°ɷ�
        int fame;
        string job;
        //vector<string> endingTexts;
        string endingText;
    };

    void parseTSV_Ending(const string& filename);
    
    // ���ڿ��� ���� Ÿ������ ��ȯ�ϴ� �Լ�
    Status_Total stringToStatType(string statStr)
    {
        if (statStr == "�ٷ�") return Status_Total::Strength;
        else if (statStr == "����") return Status_Total::Magic_Power;
        else if (statStr == "�ǰ�") return Status_Total::Health;
        else if (statStr == "����") return Status_Total::Knowledge;
        else if (statStr == "�ŷ�") return Status_Total::Charm;
        //else if (statStr == "��") return Status_Total::Sang;
        //else if (statStr == "��") return Status_Total::Jung;
        //else if (statStr == "��") return Status_Total::Ha;
        else return Status_Total::Null; // �⺻�� ? �׷��ֵ�
    }


//private: //�ϴ� �ۺ�
public:
     vector<Ending>  EndingVector;
};

