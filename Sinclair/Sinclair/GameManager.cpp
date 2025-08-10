#include "GameManager.h"
#include "StatWindow.h"
#include "Inventory.h"
#include "EquipmentWindow.h"
#include "ItemBank.h"

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
    }
    UsedEquipedClean(); // temp���ٰ� ����, ���� �κ��� �ϴ� ������.
    AdvResult(); //��ġ�� ���뿡 ���� �κ� & �ӽ� vector ������Ʈ ��. ���� 
    TempToNext(); //���������� �������� �κ��� �־��. 


    // ��ġ�� ������ ������ string id �� �غ��ϱ� 
    
    // -> ending id�� ���ؼ� ��ġ���� ����ؾ� ��. 

   
}


//EndingScene�� Enter �κ��ΰ� 
//id�� ���ؼ� ��Ʈ�� �غ��� �Ű� 
// �׳� ���⼭ ���̶� �̷��͵� �� �����״ϱ�

void GameManager::AftAdv() //ending scene -> endingScene�� id�� �����Ű� ? 
{
    
    
    
   


    //���� ������Ʈ , 
    UpdateGen(); 
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
    int this_Gen_Fam = GetCurrentFam();
    int gen = GetCurrentGen(); //���� ������ �� 2 3 4 -> 
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

    auto items = GetRandomItemsByFam(Fam , famToPoolCounts_Gen2to3);
    for (auto& id : items)
        inven->AddItem(id);

}

void GameManager::AdvResult_Item_Gen3_Gen4(int Fam)
{
    Inventory* inven = dynamic_cast<Inventory*>(
        UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
    if (!inven) return;

    auto items = GetRandomItemsByFam(Fam, famToPoolCounts_Gen3to4);
    for (auto& id : items)
        inven->AddItem(id);
}

void GameManager::AdvResult_Potion(int Fam)
{
    SpawnItemsByFame(Fam);
}

void GameManager::TempToNext()
{
    Inventory* inven = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::StatsWindow));
    if (inven != nullptr)
    {
        for (auto& item : m_tempItem)
        {
            inven->GetItemBase().AddItemData(std::move(item));
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
