#include "GameManager.h"

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
