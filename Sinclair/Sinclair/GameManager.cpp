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

    // 임시로 만듬 - 얘로 계산할꺼임
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
                return recipe.result; // 결과 아이템 id
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
        // 상
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
            // 하
            arrEndingID[curGen - 2] = 3031;
            return;
        }
        else
        {
            // 중
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

        // positive 조건 체크
        vector<Status_Total> requiredPositive;

        // positive.first 추가
        if (ending.positive.first != Status_Total::Null) {
            requiredPositive.push_back(ending.positive.first);
        }

        // positive.second 추가 (Null이 아닌 경우)
        if (ending.positive.second != Status_Total::Null) {
            requiredPositive.push_back(ending.positive.second);
        }

        // positive 조건 개수가 일치하는지 확인
        if (requiredPositive.size() != positiveStats.size()) {
            continue;
        }

        // positive 조건이 모두 매칭되는지 확인
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

        // negative 조건 체크
        if (ending.negative == Status_Total::Null) {
            // 엔딩에서 negative 조건이 없는 경우
            if (!negativeStats.empty()) {
                continue; // 플레이어에게 negative가 있으면 매칭 안됨
            }
        }
        else {
            // 엔딩에서 negative 조건이 있는 경우
            if (negativeStats.size() != 1 || negativeStats[0] != ending.negative) {
                continue; // negative 조건이 정확히 매칭되지 않음
            }
        }

        // 모든 조건이 매칭되면 해당 엔딩 선택
        arrEndingID[curGen - 2] = ending.ID;
        return;
    }

    // 매칭되는 엔딩이 없으면 기본 엔딩 (중)
    arrEndingID[curGen - 2] = 3021;

}

