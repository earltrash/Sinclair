#pragma once
#include "pch.h"
using namespace std;
#include <fstream>
#include <sstream>

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

