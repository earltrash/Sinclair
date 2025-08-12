#include "GameManager.h"
#include "StatWindow.h"
#include "Inventory.h"
#include "EquipmentWindow.h"
#include "ItemBank.h"
#include "TextBank.h"
#include "ResourceManager.h"
#include "SoundBank.h"
#include <random>
#include <algorithm>

#ifdef min
#undef min
#endif

void GameManager::Initalize()
{

    adv_wepon.assign(11, false); //  -> �̰� 4->2�� �� �� �ٽ� �ʱ�ȭ �ؾ� ��. 
    m_wearable_part.assign(static_cast<int>(Wearable_part::UnKnown), false); // ���-������ Ǯ ���� 

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
    static GameManager Gamemanager;
    return Gamemanager;
}


std::vector<std::string> GameManager::GetRandomItemsByFam
(
    int fam,
    const std::unordered_map<int, std::vector<PoolCount>>& poolMap,
     std::unordered_set<std::string>& alreadyPicked // �̹� ���� �����۵�
) 

{
    std::vector<std::string> result;

    auto famIt = poolMap.find(fam);
    if (famIt == poolMap.end()) return result;

    static std::random_device rd;
    static std::mt19937 gen(rd());

    for (auto& pc : famIt->second) {
        auto poolIt = poolItems.find(pc.pool);
        if (poolIt == poolItems.end() || poolIt->second.empty()) continue;

        std::vector<std::string> temp = poolIt->second;

        int count = pc.count;
        while (count > 0 && !temp.empty()) {
            std::shuffle(temp.begin(), temp.end(), gen);
            bool found = false;

            for (auto it = temp.begin(); it != temp.end();) {
                if (alreadyPicked.find(*it) == alreadyPicked.end()) {
                    int index_num = static_cast<int>(
                        ResourceManager::Get().Get_ItemBank().CreateItem(*it)->m_data.wearablePart
                        );
                    if (!m_wearable_part[index_num]) {
                        result.push_back(*it);
                        alreadyPicked.insert(*it);
                        m_wearable_part[index_num] = true;
                        count--;
                        it = temp.erase(it);
                        found = true;
                        if (count == 0) break;
                    }
                    else {
                        ++it; // ���� �ߺ��̸� ���� �ĺ�
                    }
                }
                else {
                    ++it; // ������ �ߺ��̸� ���� �ĺ�
                }
            }
            if (!found) break; // �̹� ���忡�� �ƹ��͵� �� �̾����� ����
        }
    }
    std::fill(m_wearable_part.begin(), m_wearable_part.end(), false);
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
    FindEnding();

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
    TempToNext(); 
   
}




//EndingScene�� Enter �κ��ΰ� 
//id�� ���ؼ� ��Ʈ�� �غ��� �Ű� 
// �׳� ���⼭ ���̶� �̷��͵� �� �����״ϱ�

std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> GameManager::AftAdv() //ending scene -> endingScene�� id�� �����Ű� ? 
{
   
    int index = GetCurrentGen() - 2;
    int id = arrEndingID[index];



  SaveEndingBgm(id);
  
  //-> �긦 ���� ���� �Ŵ������� ������� �ϴ� �ǰ� 

    
    

    std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> ending_bitmap =
        ResourceManager::Get().GetEndingBitmap(std::to_string(id));
    
    if (ending_bitmap.size() == 1)
    {
        auto it = ending_bitmap.begin();
        arrTotalEndingImg[index] = it->second;
    }
    else if (ending_bitmap.size() == 2)
    {
        auto it = ending_bitmap.begin();
        ++it;
        arrTotalEndingImg[index] = it->second;
    }

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

        auto it = invenDB.find(id); //�ᱹ�� ���â id�� �κ��� ���� �����ϱ� �װŴ� �κ����� �������� �Ű� 
        if (it != invenDB.end())
        {
            m_tempItem.push_back(std::move(it->second));
            AdvResult_Wep(id); // -> �� ��� ���� �ű� �ѵ�, Ư�� ������ ��� ���� ������ Ư���� ������ �ִ� �� �װ� �� ����. 
            invenDB.erase(it); //�ӽÿ� �־����ϱ� �����ٴ� ���ݾ� 
            equipmentWindow->ClearSlot((Wearable_part)i);

        }
    }

    // �κ��丮 ����
    inven->ClearAllSlots();

}

void GameManager::AdvResult_Wep(string itemkey) //
{
    Inventory* inven = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));

    int idx = 0;
    for (const auto& [key, val] : weaponMap) // val�� �� ������
    {
        if (key == itemkey && adv_wepon[idx] != true ) //true �ΰ�쿡�� ġ��Կ� 
        {
            inven->GetItemBase().AddItemData(std::move(ResourceManager::Get().Get_ItemBank().CreateItem(val)
));

            if (idx >= 0 && idx < adv_wepon.size()) 
                adv_wepon[idx] = true;
            break; 
        }
        ++idx;
    }
}

void GameManager::AdvResult()
{
    

    int this_Gen_Fam = GetCurrentFam();

    int Gen = GetCurrentGen();


    AdvResult_Potion(this_Gen_Fam);

    switch(Gen)
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

    Inventory* inven = dynamic_cast<Inventory*>(
        UIManager::Get().GetWindow(UIWindowType::InventoryWindow));

    for (auto& [id, count] : it->second)
    {
        for (int i = 0; i < count; ++i)
        {
            auto item = bank.CreateItem(id); // unique_ptr<Item> ��ȯ�Ѵٰ� ����
            string id = (item->m_data.id);
            inven->GetItemBase().AddItemData(std::move(item));
            inven->AddItem(id, 1);
        }
    }
}

void GameManager::AdvResult_Item_Gen2_Gen3(int Fam)
{
    Inventory* inven = dynamic_cast<Inventory*>(
        UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
    if (!inven) return;

    auto items = GetRandomItemsByFam(Fam , famToPoolCounts_Gen2to3 , alreadyPicked); //id�� �����Ű� 
    for (auto& id : items)
    {
        inven->GetItemBase().AddItemData(ResourceManager::Get().Get_ItemBank().CreateItem(id));
    }
    //inven->PackItem();
}

void GameManager::AdvResult_Item_Gen3_Gen4(int Fam)
{
    Inventory* inven = dynamic_cast<Inventory*>(
        UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
    if (!inven) return;

    auto items = GetRandomItemsByFam(Fam, famToPoolCounts_Gen3to4, alreadyPicked);
    for (auto& id : items)
        //inven->AddItem(id);
    {
        inven->GetItemBase().AddItemData(ResourceManager::Get().Get_ItemBank().CreateItem(id));
    }
   // inven->PackItem();

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

void GameManager::Default_Item_TO_Inven(int GEN) //���
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
    bool debug = true;

    if(debug)
    {
        cout << "\n=== FindEnding ����� ���� ===" << endl;
        cout << "���� ����: �ٷ�(" << arrTotalStatus[curGen - 2].Strength << "), ";
        cout << "����(" << arrTotalStatus[curGen - 2].Magic_Power << "), ";
        cout << "�ǰ�(" << arrTotalStatus[curGen - 2].Health << "), ";
        cout << "����(" << arrTotalStatus[curGen - 2].Knowledge << "), ";
        cout << "�ŷ�(" << arrTotalStatus[curGen - 2].Charm << ")" << endl;
    }

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

    if (debug)
    {
        cout << "Positive ���� ����: " << positiveStats.size() << " (";
        for (auto stat : positiveStats) {
            cout << GetStatName(stat) << " ";
        }
        cout << ")" << endl;

        cout << "Negative ���� ����: " << negativeStats.size() << " (";
        for (auto stat : negativeStats) {
            cout << GetStatName(stat) << " ";
        }
        cout << ")" << endl;
    }


    // ��ü ���� ���� (��� ���� 50% �̻�)
    if (positiveStats.size() >= 5) {
        arrEndingID[curGen - 2] = 3011;
        arrTotalFam[curGen - 2] = 7;
        if (debug) cout << "���: ��ü ���� ���� (3011 - ���ù��� ��)" << endl;
        return;
    }

    // ��ü ���� ���� (��� ���� 20% ����)
    if (negativeStats.size() >= 5) {
        arrEndingID[curGen - 2] = 3031;
        arrTotalFam[curGen - 2] = 1;
        if (debug) cout << "���: ��ü ���� ���� (3031 - ����� ����)" << endl;
        return;
    }

    // positive�� ���� ��� �߰� ����
    if (positiveStats.size() == 0) {
        arrEndingID[curGen - 2] = 3021;
        arrTotalFam[curGen - 2] = 2;
        if (debug) cout << "���: �߰� ���� (3021 - ���谡) - positive ���� ����" << endl;
        return;
    }

    // positive ���ȵ��� �� �������� �������� ���� (���� ���� �ͺ���)
    sort(positiveStats.begin(), positiveStats.end(),
        [&statMap](Status_Total a, Status_Total b) {
            return statMap[a] > statMap[b];
        });

    if (debug)
    {
        cout << "Positive ���� ���� �� (�� ����): ";
        for (auto stat : positiveStats) {
            cout << GetStatName(stat) << "(" << statMap[stat] << ") ";
        }
        cout << endl;
    }

    // negative ���ȵ��� �� �������� �������� ���� (���� ���� �ͺ���)
    sort(negativeStats.begin(), negativeStats.end(),
        [&statMap](Status_Total a, Status_Total b) {
            return statMap[a] < statMap[b];
        });

    if (debug)
    {
        cout << "Negative ���� ���� �� (�� ����): ";
        for (auto stat : negativeStats) {
            cout << GetStatName(stat) << "(" << statMap[stat] << ") ";
        }
        cout << endl;
    }

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


    if (debug)
    {
        cout << "���� ��Ī ���� - Positive: ";
        for (auto stat : finalPositive) {
            cout << GetStatName(stat) << " ";
        }
        cout << ", Negative: " << GetStatName(finalNegative) << endl;
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
        arrTotalFam[curGen - 2] = ending.fame;

        if(debug) cout << "��Ī�� ����: " << ending.ID << " - " << ending.job << endl;
        return;
    }

    // ��Ī�Ǵ� ������ ������ �⺻ ���� (��)
    arrTotalFam[curGen - 2] = 2;
    arrEndingID[curGen - 2] = 3021;
    if (debug)
    {
        cout << "���: �⺻ �߰� ���� (3021 - ���谡) - ��Ī ����" << endl;
    }
}

void GameManager::SaveEndingBgm(int id)
{

   auto st_id = std::to_string(id);
   //endingBgm = ResourceManager::Get().Get_SoundBank().GetEndingBGM(st_id); //�׳� ����� ��Ƶξ��ٰ� ���� ��

   if (GetCurrentGen() == 4)
   {
       //historyBgm = ResourceManager::Get().Get_SoundBank().GetEndingBGM(st_id); //�ϴ� �ӽð� 
   }

}
