#include "GameManager.h"
#include "StatWindow.h"
#include "Inventory.h"
#include "EquipmentWindow.h"
#include "ItemBank.h"
#include "TextBank.h"
#include "ResourceManager.h"

#include <random>
#include <algorithm>

#ifdef min
#undef min
#endif

void GameManager::Initalize()
{
    SyntableInit();
    PoolInit();

}

void GameManager::SyntableInit()
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
}

void GameManager::PoolInit()
{
    auto& bank = ResourceManager::Get().Get_ItemBank();

    poolItems[Need_Moment::Fam3_a] = bank.GetItemIDsByMoment(Need_Moment::Fam3_a);
    poolItems[Need_Moment::Fam3_b] = bank.GetItemIDsByMoment(Need_Moment::Fam3_b);


    poolItems[Need_Moment::Fam4_a] = bank.GetItemIDsByMoment(Need_Moment::Fam4_a);
    poolItems[Need_Moment::Fam4_b] = bank.GetItemIDsByMoment(Need_Moment::Fam4_b);

}

GameManager& GameManager::Get()
{
    static GameManager resourcemanager;
    return resourcemanager;
}


std::vector<std::string> GameManager::GetRandomItemsByFam(
    int fam,
    const std::unordered_map<int, std::vector<PoolCount>>& poolMap)
{
    std::vector<std::string> result;

    auto famIt = poolMap.find(fam);
    if (famIt == poolMap.end()) return result;

    static std::random_device rd;
    static std::mt19937 gen(rd());

    for (auto& pc : famIt->second)
    {
        auto poolIt = poolItems.find(pc.pool);
        if (poolIt == poolItems.end() || poolIt->second.empty()) continue;

        std::vector<std::string> temp = poolIt->second;
        std::shuffle(temp.begin(), temp.end(), gen);

        int count = std::min(pc.count, (int)temp.size());
        result.insert(result.end(), temp.begin(), temp.begin() + count);
    }
    return result;
}

void GameManager::FamValue()
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

//ending Scene ���� �� -> endingScene�� �ʿ��Ѱ� bitmap = id // stat ó���� �� �ؼ� id�� ���ؿ��� ���� 
void GameManager::PreAdv() 
{

    //1. ������ �޾ƿ�
    //2. ���� ��� ������.
    //3. ������ ���� ending id & ending id�� ���� �� ������ ��ġ�� �޾ƿ;� ��. 
    //4. �������� �Ѱ��ܱ��� ��. 
    int index = GetCurrentGen() - 2; //int curGen 2 ; 

    StatWindow* statwin = dynamic_cast<StatWindow*>(UIManager::Get().GetWindow(UIWindowType::StatsWindow));
    if (statwin != nullptr)
    {
        arrTotalStatus[index] = statwin->GetTotalStatus(); //���� �ϰ� �̰ŷ� �����ָ� �� -> �����ٰž�. 
        statwin->ResetStat(); 
    }
    UsedEquipedClean(); // temp���ٰ� ����, ���� �κ��� �ϴ� ������.
    AdvResult(); //��ġ�� ���뿡 ���� �κ� & �ӽ� vector ������Ʈ ��. ���� 


    TempToNext(); //���������� �������� �κ��� �־��. 
    Default_Item_TO_Inven(GetCurrentGen());

    // ��ġ�� ������ ������ string id �� �غ��ϱ� 
    
    // -> ending id�� ���ؼ� ��ġ���� ����ؾ� ��. 

   
}


//EndingScene�� Enter �κ��ΰ� 
//id�� ���ؼ� ��Ʈ�� �غ��� �Ű� 
// �׳� ���⼭ ���̶� �̷��͵� �� �����״ϱ�

std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> GameManager::AftAdv() //ending scene -> endingScene�� id�� �����Ű� ? 
{
    FindEnding();
    int index = GetCurrentGen() - 2;
    int id = arrEndingID[index];
    
    //SceneManager�� �������� �޾ƿ�? -> ������ٰ� �����? 

    std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> ending_bitmap =
        ResourceManager::Get().GetEndingBitmap(std::to_string(id));
    
  //  UpdateGen(); 

    return ending_bitmap;
}

//��� �����̶� ��ġ�� 
void GameManager::PotionUsed(Status_fundamental stat, int Much)
{
    //����â �����ͼ�, �ش� �κп� �ش��ϴ� ���� much ��ŭ �ø��� 
    StatWindow* statwin = dynamic_cast<StatWindow*>(UIManager::Get().GetWindow(UIWindowType::StatsWindow));
    if (statwin != nullptr)
    {
        statwin->StatUpdate(stat, Much);
    }

}

void GameManager::UpdateGen()
{
    if (curGen < 4)
        curGen++;
    else
        std::cout << "�ʰ��ε���";
}

void GameManager::UsedEquipedClean() //û���ϴ� �迡 �߰����� �ع����� . 
{
    auto* equipmentWindow = dynamic_cast<EquipmentWindow*>(
        UIManager::Get().GetWindow(UIWindowType::EquipmentWindow));
    if (!equipmentWindow) return;

    // �κ� ��������
    auto* inven = dynamic_cast<Inventory*>(
        UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
    if (!inven) return;

    // ������ �����ͺ��̽� ����
    auto& invenDB = inven->GetItemBase().GetMap();

    // ������ ������ ��ȸ
    for (int i = 0; i < static_cast<int>(Wearable_part::UnKnown); ++i)
    {
        Item* equipped = equipmentWindow->GetEquippedItem((Wearable_part)i);
        if (!equipped) continue;

        std::string id = equipped->m_data.id;

        auto it = invenDB.find(id);
        if (it != invenDB.end())
        {
            // ������ ����
            m_tempItem.push_back(std::move(it->second));
            AdvResult_Wep(id); // -> �� ��� ���� �ű� �ѵ�, Ư�� ������ ��� ���� ������ Ư���� ������ �ִ� �� �װ� �� ����. 
            invenDB.erase(it);
            equipmentWindow->ClearSlot((Wearable_part)i);

        }
    }

    // �κ��丮 ����
    inven->ClearAllSlots();

}

void GameManager::AdvResult_Wep(string itemkey)
{
    for (const auto& [key, val] : weaponMap) //val�� �� ��������. 
    {
        if (key == itemkey)
        {
            m_tempItem.push_back(ResourceManager::Get().Get_ItemBank().CreateItem(val));
        }


    }
}

void GameManager::AdvResult()
{
    //int this_Gen_Fam = GetCurrentFam();

    //int gen = GetCurrentGen(); //���� ������ �� 2 3 4 -> 

    int this_Gen_Fam = 5;

    int gen = 2;
    //

    AdvResult_Potion(this_Gen_Fam);

    switch(gen)
    {
    case 2:
        AdvResult_Item_Gen2_Gen3(this_Gen_Fam);
        break;
    case 3:
        AdvResult_Item_Gen3_Gen4(this_Gen_Fam);
        break;
    case 4:
        //�ʴ� ���������ݾ�
        break;
    default:
        std::cout << "? �� ����" << endl;
        break;
    }


}

void GameManager::SpawnItemsByFame(int fame)
{
    auto it = famePool.find(fame);
    if (it == famePool.end()) return;

    auto& bank = ResourceManager::Get().Get_ItemBank();

    for (auto& [id, count] : it->second)
    {
        for (int i = 0; i < count; ++i)
        {
            auto item = bank.CreateItem(id); // unique_ptr<Item> ��ȯ�Ѵٰ� ����
            m_tempItem.push_back(std::move(item));
        }
    }
}

void GameManager::AdvResult_Item_Gen2_Gen3(int Fam)
{
    Inventory* inven = dynamic_cast<Inventory*>(
        UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
    if (!inven) return;

    auto items = GetRandomItemsByFam(Fam , famToPoolCounts_Gen2to3); //id�� �����Ű� 
    for (auto& id : items)
    {
        inven->GetItemBase().AddItemData(ResourceManager::Get().Get_ItemBank().CreateItem(id));
    }
    inven->PackItem();
}

void GameManager::AdvResult_Item_Gen3_Gen4(int Fam)
{
    Inventory* inven = dynamic_cast<Inventory*>(
        UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
    if (!inven) return;

    auto items = GetRandomItemsByFam(Fam, famToPoolCounts_Gen3to4);
    for (auto& id : items)
        //inven->AddItem(id);
    {
        inven->GetItemBase().AddItemData(ResourceManager::Get().Get_ItemBank().CreateItem(id));
    }
    inven->PackItem();

}

void GameManager::AdvResult_Potion(int Fam)
{
    SpawnItemsByFame(Fam);
}

void GameManager::TempToNext()
{
   
    Inventory* inven = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
    if (inven != nullptr)
    {
        for (auto& item : m_tempItem)
        {
            inven->GetItemBase().AddItemData(std::move(item));
        }
        inven->PackItem();
    }
    m_tempItem.clear();
}

void GameManager::Default_Item_TO_Inven(int GEN)
{
    if (GEN == 2) //�⺻ 3 
    {
        Inventory* inven = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
        if (inven != nullptr)
        {
            inven-> LoadItemDatabase(Need_Moment::Gen_3);
            inven->PackItem();

        }
    }

    if (GEN == 3) //�⺻ 4 
    {
        Inventory* inven = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
        if (inven != nullptr)
        {
            inven->LoadItemDatabase(Need_Moment::Gen_4);
            inven->PackItem();

        }
    }
}

int GameManager::GetResultFam()
{
    int i = 0;
    for (int i = 0; i < arrTotalFam.size(); i++)
    {
        i += arrTotalFam[i];
    }
    return i;
}

void GameManager::FindEnding()
{
    unordered_map<Status_Total, int> statMap = {
        {Status_Total::Strength   , arrTotalStatus[curGen - 2].Strength   },
        {Status_Total::Magic_Power, arrTotalStatus[curGen - 2].Magic_Power  },
        {Status_Total::Health     , arrTotalStatus[curGen - 2].Health       },
        {Status_Total::Knowledge  , arrTotalStatus[curGen - 2].Knowledge    },
        {Status_Total::Charm      , arrTotalStatus[curGen - 2].Charm        }
    };

    vector<Status_Total> positiveStats;
    vector<Status_Total> negativeStats;

    for (const auto& stat : statMap) {
        if (stat.second >= 50) {
            positiveStats.push_back(stat.first);
        }
        else if (stat.second <= 20) {
            negativeStats.push_back(stat.first);
        }
    }

    // ��ü ���� ���� (��� ���� 50% �̻�)
    if (positiveStats.size() >= 5) {
        arrEndingID[curGen - 2] = 3011;
        return;
    }

    // ��ü ���� ���� (��� ���� 20% ����)
    if (negativeStats.size() >= 5) {
        arrEndingID[curGen - 2] = 3031;
        return;
    }

    // positive�� ���� ��� �߰� ����
    if (positiveStats.size() == 0) {
        arrEndingID[curGen - 2] = 3021;
        return;
    }

    // positive ���ȵ��� �� �������� �������� ���� (���� ���� �ͺ���)
    sort(positiveStats.begin(), positiveStats.end(),
        [&statMap](Status_Total a, Status_Total b) {
            return statMap[a] > statMap[b];
        });

    // negative ���ȵ��� �� �������� �������� ���� (���� ���� �ͺ���)
    sort(negativeStats.begin(), negativeStats.end(),
        [&statMap](Status_Total a, Status_Total b) {
            return statMap[a] < statMap[b];
        });

    // ���� ��Ī�� ���� ���� ����
    vector<Status_Total> finalPositive;
    Status_Total finalNegative = Status_Total::Null;

    if (positiveStats.size() >= 2) {
        // 2�� �̻��̸� ���� ���� 2�� ����
        finalPositive.push_back(positiveStats[0]);
        finalPositive.push_back(positiveStats[1]);

        // positive ������ enum �� �������� ���� (���� �����Ϳ� ��Ī�� ����)
        sort(finalPositive.begin(), finalPositive.end(),
            [](Status_Total lhs, Status_Total rhs) {
                return static_cast<int>(lhs) < static_cast<int>(rhs);
            });
    }
    else if (positiveStats.size() == 1) {
        // 1���� �װ͸� ���
        finalPositive.push_back(positiveStats[0]);
    }

    // negative ���� ����
    if (!negativeStats.empty()) {
        finalNegative = negativeStats[0]; // ���� ���� ����
    }

    // ���� ��Ī
    for (const auto& ending : ResourceManager::Get().Get_TextBank().EndingVector) {
        // positive ���� ����
        vector<Status_Total> requiredPositive;
        if (ending.positive.first != Status_Total::Null) {
            requiredPositive.push_back(ending.positive.first);
        }
        if (ending.positive.second != Status_Total::Null) {
            requiredPositive.push_back(ending.positive.second);
        }

        // positive ���� ���� Ȯ��
        if (requiredPositive.size() != finalPositive.size()) {
            continue;
        }

        // positive ���� ��Ī Ȯ�� (���ĵ� ���¿��� ��)
        sort(requiredPositive.begin(), requiredPositive.end(),
            [](Status_Total lhs, Status_Total rhs) {
                return static_cast<int>(lhs) < static_cast<int>(rhs);
            });

        bool positiveMatches = true;
        for (size_t i = 0; i < requiredPositive.size(); i++) {
            if (requiredPositive[i] != finalPositive[i]) {
                positiveMatches = false;
                break;
            }
        }

        if (!positiveMatches) {
            continue;
        }

        // negative ���� Ȯ��
        if (ending.negative != finalNegative) {
            continue;
        }

        // ��� ������ ��Ī�Ǹ� �ش� ���� ����
        arrEndingID[curGen - 2] = ending.ID;
        return;
    }

    // ��Ī�Ǵ� ������ ������ �⺻ ���� (��)
    arrEndingID[curGen - 2] = 3021;
}
//void GameManager::FindEnding()
//{
//
//    unordered_map<Status_Total, int> statMap = {
//    {Status_Total::Strength   , arrTotalStatus[curGen - 2].Strength   },
//    {Status_Total::Magic_Power, arrTotalStatus[curGen - 2].Magic_Power  },
//    {Status_Total::Health     , arrTotalStatus[curGen - 2].Health       },
//    {Status_Total::Knowledge  , arrTotalStatus[curGen - 2].Knowledge    },
//    {Status_Total::Charm      , arrTotalStatus[curGen - 2].Charm        }
//    };
//
//    vector<Status_Total> positiveStats;
//    vector<Status_Total> negativeStats;
//    for (const auto& stat : statMap) {
//        if (stat.second >= 50) {
//            positiveStats.push_back(stat.first);
//        }
//        else if (stat.second <= 20)
//        {
//            negativeStats.push_back(stat.first);
//        }
//    }
//
//    if (positiveStats.size() >= 5) 
//    {
//        // ��
//        arrEndingID[curGen - 2] = 3011;
//        return;
//    }
//    else if (positiveStats.size() >= 3 && positiveStats.size() < 5)
//    {
//        sort(positiveStats.begin(), positiveStats.end(),
//            [&statMap](Status_Total a, Status_Total b) {
//                return statMap[a] > statMap[b];
//            });
//        positiveStats.resize(2);
//        std::sort(positiveStats.begin(), positiveStats.end(),
//            [](Status_Total lhs, Status_Total rhs) {
//                return static_cast<int>(lhs) < static_cast<int>(rhs);
//            });
//    }
//    else if (positiveStats.size() == 0)
//    {
//        if(negativeStats.size() == 5)
//        {
//            // ��
//            arrEndingID[curGen - 2] = 3031;
//            return;
//        }
//        else
//        {
//            // ��
//            arrEndingID[curGen - 2] = 3021;
//            return;
//        }
//    }
//
//    if (negativeStats.size() >= 2)
//    {
//        sort(negativeStats.begin(), negativeStats.end(),
//            [&statMap](Status_Total a, Status_Total b) {
//                return statMap[a] < statMap[b];
//            });
//        negativeStats.resize(1);
//    }
//
//    //TextBank::EndingVector
//    for (const auto& ending : ResourceManager::Get().Get_TextBank().EndingVector) {
//        bool matches = true;
//
//        // positive ���� üũ
//        vector<Status_Total> requiredPositive;
//
//        // positive.first �߰�
//        if (ending.positive.first != Status_Total::Null) {
//            requiredPositive.push_back(ending.positive.first);
//        }
//
//        // positive.second �߰� (Null�� �ƴ� ���)
//        if (ending.positive.second != Status_Total::Null) {
//            requiredPositive.push_back(ending.positive.second);
//        }
//
//        // positive ���� ������ ��ġ�ϴ��� Ȯ��
//        if (requiredPositive.size() != positiveStats.size()) {
//            continue;
//        }
//
//        // positive ������ ��� ��Ī�Ǵ��� Ȯ��
//        for (Status_Total requiredStat : requiredPositive) {
//            bool found = false;
//            for (Status_Total playerStat : positiveStats) {
//                if (requiredStat == playerStat) {
//                    found = true;
//                    break;
//                }
//            }
//            if (!found) {
//                matches = false;
//                break;
//            }
//        }
//
//        if (!matches) {
//            continue;
//        }
//
//        // negative ���� üũ
//        if (ending.negative == Status_Total::Null) {
//            // �������� negative ������ ���� ���
//            if (!negativeStats.empty()) {
//                continue; // �÷��̾�� negative�� ������ ��Ī �ȵ�
//            }
//        }
//        else {
//            // �������� negative ������ �ִ� ���
//            if (negativeStats.size() != 1 || negativeStats[0] != ending.negative) {
//                continue; // negative ������ ��Ȯ�� ��Ī���� ����
//            }
//        }
//
//        // ��� ������ ��Ī�Ǹ� �ش� ���� ����
//        arrEndingID[curGen - 2] = ending.ID;
//        return;
//    }
//
//    // ��Ī�Ǵ� ������ ������ �⺻ ���� (��)
//    arrEndingID[curGen - 2] = 3021;
//
//}

