#pragma once
#include "pch.h"
#include "status.h"
#include <array>
#include "Item.h"

using namespace std;

class GameManager
{
private:
    GameManager() = default;
    ~GameManager() = default;
public:


    void Initalize();
    void SyntableInit();
    void PoolInit();
    static GameManager& Get();

    std::vector<std::string> GetRandomItemsByFam( int fam,
        const std::unordered_map<int, std::vector<PoolCount>>& poolMap);

    void FamValue();

    string Synthesis(const string& id1, const string& id2);

    //ending Scene 기존씬 exit
    void PreAdv(); // -> 현재 ingame string 받아서 스탯 저장하고, 기존 장비 정리하는 과정임. 

    //ending Scene -> 새 InGame 
    void AftAdv(); // ->string에 해당하는 장비 넘겨주기

    void PotionUsed(Status_fundamental stat, int Much);

    int GetCurrentGen() { return curGen; }
    void UpdateGen();
    void UsedEquipedClean();
    void AdvResult_Wep(string key);


    void AdvResult();
    void SpawnItemsByFame(int fame);
    void AdvResult_Item_Gen2_Gen3(int Fam);
    void AdvResult_Item_Gen3_Gen4(int Fam);
    void AdvResult_Potion(int Fam);

    void TempToNext();


    int GetCurrentFam() { return arrTotalFam[GetCurrentGen() - 2]; }; //한 세대 기준 
    int GetResultFam();

public:// get set 만들기 전
    bool isFirst = true;

    int curGen = 2;



    // soundmanager야 가져가
    float bgmValue = 1.0f;
    float sfxValue = 1.0f;
    

    std::array<TotalStatus, 3> arrTotalStatus{};
    std::array<int, 3> arrTotalFam{};
    

    std::vector <unique_ptr<Item>> m_tempItem;
    std::vector<Recipe> m_SynTable;

        std::unordered_map<std::string, std::string> weaponMap = {
        { "W001", "I016" },
        { "W002", "I017" },
        { "W003", "I018" },
        { "W004", "I019" },
        { "W005", "I020" },
        { "W006", "I021" },
        { "W007", "I022" },
        { "W008", "I023" },
        { "W009", "I024" },
        { "W010", "I025" }
    };

    std::unordered_map<Need_Moment, std::vector<std::string>> poolItems;

    std::unordered_map<int, std::vector<PoolCount>> famToPoolCounts_Gen2to3{
    { 1, { { Need_Moment::Fam3_a, 1 }, { Need_Moment::Fam3_b, 0 } } },
    { 2, { { Need_Moment::Fam3_a, 2 }, { Need_Moment::Fam3_b, 0 } } },
    { 3, { { Need_Moment::Fam3_a, 3 }, { Need_Moment::Fam3_b, 1 } } },
    { 4, { { Need_Moment::Fam3_a, 3 }, { Need_Moment::Fam3_b, 2 } } },
    { 5, { { Need_Moment::Fam3_a, 4 }, { Need_Moment::Fam3_b, 2 } } },
    { 6, { { Need_Moment::Fam3_a, 4 }, { Need_Moment::Fam3_b, 3 } } },
    { 7, { { Need_Moment::Fam3_a, 4 }, { Need_Moment::Fam3_b, 4 } } },
    };

    std::unordered_map<int, std::vector<PoolCount>>famToPoolCounts_Gen3to4  {
    { 1, { { Need_Moment::Fam4_a, 1 }, { Need_Moment::Fam4_b, 0 } } },
    { 2, { { Need_Moment::Fam4_a, 2 }, { Need_Moment::Fam4_b, 0 } } },
    { 3, { { Need_Moment::Fam4_a, 3 }, { Need_Moment::Fam4_b, 1 } } },
    { 4, { { Need_Moment::Fam4_a, 3 }, { Need_Moment::Fam4_b, 2 } } },
    { 5, { { Need_Moment::Fam4_a, 4 }, { Need_Moment::Fam4_b, 2 } } },
    { 6, { { Need_Moment::Fam4_a, 4 }, { Need_Moment::Fam4_b, 3 } } },
    { 7, { { Need_Moment::Fam4_a, 4 }, { Need_Moment::Fam4_b, 4 } } },
    };

    //자료구조 이해가 높으면 진짜 쉽겠다 싶음 
    using FamePool = std::unordered_map<int, std::vector<std::pair<std::string, int>>>;

    FamePool famePool = {
     { 1, { {"I013", 1}, {"I014", 1}, {"I015", 0} } },
     { 2, { {"I013", 2}, {"I014", 1}, {"I015", 0} } },
     { 3, { {"I013", 3}, {"I014", 2}, {"I015", 0} } },
     { 4, { {"I013", 3}, {"I014", 2}, {"I015", 1} } },
     { 5, { {"I013", 3}, {"I014", 3}, {"I015", 1} } },
     { 6, { {"I013", 3}, {"I014", 3}, {"I015", 2} } },
     { 7, { {"I013", 3}, {"I014", 3}, {"I015", 3} } }
    };

};