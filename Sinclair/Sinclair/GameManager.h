#pragma once
#include "pch.h"
#include "status.h"
#include <array>
#include "Item.h"
//#include "fmod.h"


using namespace std;

class FMOD_SOUND;

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
        const std::unordered_map<int, std::vector<PoolCount>>& poolMap
    ,  std::unordered_set<std::string>& alreadyPicked);

    void FamValue();

    string Synthesis(const string& id1, const string& id2);

    //ending Scene ������ exit
    void PreAdv(); // -> ���� ingame string �޾Ƽ� ���� �����ϰ�, ���� ��� �����ϴ� ������. 

    //ending Scene -> �� InGame 
    std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> AftAdv(); // ->string�� �ش��ϴ� ��� �Ѱ��ֱ�
    //���� ���Ϸ��� ��
    void FindEnding();

    void SaveEndingBgm(int id);

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
    void Default_Item_TO_Inven(int GEN);

    int GetCurrentFam() { return arrTotalFam[GetCurrentGen() - 2]; }; //�� ���� ���� 
    int GetResultFam();

public:// get set ����� ��
    bool isFirst = true;

    int curGen = 2;

    // soundmanager�� ������
    float bgmValue = 1.0f;
    float sfxValue = 1.0f;
    
    // ������ �ʿ��� ����
    std::array<int, 3> arrEndingID{};
    std::array<TotalStatus, 3> arrTotalStatus{};
    std::array<int, 3> arrTotalFam{};
    vector<bool> adv_wepon;

    std::vector <unique_ptr<Item>> m_tempItem;
    std::vector<Recipe> m_SynTable;
    std::vector<bool> m_wearable_part;

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
        { "W010", "I025" },
        { "W016", "I016" }
    };

    std::unordered_map<Need_Moment, std::vector<std::string>> poolItems;
    std::unordered_set<std::string> alreadyPicked;

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

    //�ڷᱸ�� ���ذ� ������ ��¥ ���ڴ� ���� 
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
    


    string GetStatName(Status_Total stat) {
        switch (stat) {
        case Status_Total::Strength: return "�ٷ�";
        case Status_Total::Magic_Power: return "����";
        case Status_Total::Health: return "�ǰ�";
        case Status_Total::Knowledge: return "����";
        case Status_Total::Charm: return "�ŷ�";
        default: return "����";
        }
    }


    FMOD_SOUND* endingBgm;
    FMOD_SOUND* historyBgm;

};