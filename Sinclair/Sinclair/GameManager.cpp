#include "GameManager.h"
#include "TextBank.h"
#include "ResourceManager.h"

void GameManager::Initalize()
{
    //mat 1 mat2 receipe 
    m_SynTable.push_back({ "W001", "I016", "W006" });
    m_SynTable.push_back({ "W002", "I017", "W007" });
    m_SynTable.push_back({ "W003", "I018", "W008" });
    m_SynTable.push_back({ "W004", "I019", "W009" });
    m_SynTable.push_back({ "W005", "I020", "W010" });
    m_SynTable.push_back({ "W006", "I021", "W011" });
    m_SynTable.push_back({ "W007", "I022", "W012" });
    m_SynTable.push_back({ "W008", "I023", "W013" });
    m_SynTable.push_back({ "W009", "I024", "W014" });
    m_SynTable.push_back({ "W010", "I025", "W015" });

    // �ӽ÷� ���� - ��� ����Ҳ���
    TotalStatus gen2;
    gen2.Charm         = 21;
    gen2.Health        = 43;
    gen2.Knowledge     = 54;
    gen2.Magic_Power   = 65;
    gen2.Strength      = 78;

    TotalStatus gen3;
    gen3.Charm         = 58;
    gen3.Health        = 53;
    gen3.Knowledge     = 74;
    gen3.Magic_Power   = 21;
    gen3.Strength      = 46;

    TotalStatus gen4;
    gen4.Charm         = 46;
    gen4.Health        = 74;
    gen4.Knowledge     = 85;
    gen4.Magic_Power   = 12;
    gen4.Strength      = 35;

    arrTotalStatus = { gen2,gen3,gen4 };
}

GameManager& GameManager::Get()
{
    static GameManager resourcemanager;
    return resourcemanager;
}

void GameManager::SetGen()
{
    
}

string GameManager::Synthesis(const string& id1, const string& id2)
{
    if (m_SynTable.size() != 0)
    {
        for (const auto& recipe : m_SynTable)
        {
            bool match1 = (id1 == recipe.mat1 && id2 == recipe.mat2);
            bool match2 = (id1 == recipe.mat2 && id2 == recipe.mat1);

            if (match1 || match2)
                return recipe.result; // ��� ������ id
        }
    }
    return "F";

}

void GameManager::FindEnding()
{
    TotalStatus gen2;
    gen2.Charm                = 21;
    gen2.Health               = 43;
    gen2.Knowledge            = 54;
    gen2.Magic_Power          = 65;
    gen2.Strength             = 78;

    unordered_map<Status_Total, int> statMap = {
    {Status_Total::Strength   , gen2.Charm          },
    {Status_Total::Magic_Power, gen2.Health         },
    {Status_Total::Health     , gen2.Knowledge      },
    {Status_Total::Knowledge  , gen2.Magic_Power    },
    {Status_Total::Charm      , gen2.Strength       }
    };

    vector<Status_Total> positiveStats;
    vector<Status_Total> negativeStats;
    for (const auto& stat : statMap) {
        if (stat.second >= 50) {
            positiveStats.push_back(stat.first);
        }
        else if (stat.second <= 20)
        {
            negativeStats.push_back(stat.first);
        }
    }

    if (positiveStats.size() >= 5) 
    {
        // ��
        arrEndingID[curGen - 2] = 3011;
        return;
    }
    else if (positiveStats.size() >= 3 && positiveStats.size() < 5)
    {
        sort(positiveStats.begin(), positiveStats.end(),
            [&statMap](Status_Total a, Status_Total b) {
                return statMap[a] > statMap[b];
            });
        positiveStats.resize(2);
        std::sort(positiveStats.begin(), positiveStats.end(),
            [](Status_Total lhs, Status_Total rhs) {
                return static_cast<int>(lhs) < static_cast<int>(rhs);
            });
    }
    else if (positiveStats.size() == 0)
    {
        if(negativeStats.size() == 5)
        {
            // ��
            arrEndingID[curGen - 2] = 3031;
            return;
        }
        else
        {
            // ��
            arrEndingID[curGen - 2] = 3021;
            return;
        }
    }

    if (negativeStats.size() >= 2)
    {
        sort(negativeStats.begin(), negativeStats.end(),
            [&statMap](Status_Total a, Status_Total b) {
                return statMap[a] < statMap[b];
            });
        negativeStats.resize(1);
    }

    //TextBank::EndingVector
    for (const auto& ending : ResourceManager::Get().Get_TextBank().EndingVector) {
        bool matches = true;

        // positive ���� üũ
        vector<Status_Total> requiredPositive;

        // positive.first �߰�
        if (ending.positive.first != Status_Total::Null) {
            requiredPositive.push_back(ending.positive.first);
        }

        // positive.second �߰� (Null�� �ƴ� ���)
        if (ending.positive.second != Status_Total::Null) {
            requiredPositive.push_back(ending.positive.second);
        }

        // positive ���� ������ ��ġ�ϴ��� Ȯ��
        if (requiredPositive.size() != positiveStats.size()) {
            continue;
        }

        // positive ������ ��� ��Ī�Ǵ��� Ȯ��
        for (Status_Total requiredStat : requiredPositive) {
            bool found = false;
            for (Status_Total playerStat : positiveStats) {
                if (requiredStat == playerStat) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                matches = false;
                break;
            }
        }

        if (!matches) {
            continue;
        }

        // negative ���� üũ
        if (ending.negative == Status_Total::Null) {
            // �������� negative ������ ���� ���
            if (!negativeStats.empty()) {
                continue; // �÷��̾�� negative�� ������ ��Ī �ȵ�
            }
        }
        else {
            // �������� negative ������ �ִ� ���
            if (negativeStats.size() != 1 || negativeStats[0] != ending.negative) {
                continue; // negative ������ ��Ȯ�� ��Ī���� ����
            }
        }

        // ��� ������ ��Ī�Ǹ� �ش� ���� ����
        arrEndingID[curGen - 2] = ending.ID;
        return;
    }

    // ��Ī�Ǵ� ������ ������ �⺻ ���� (��)
    arrEndingID[curGen - 2] = 3021;

}

