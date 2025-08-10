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
                return recipe.result; // 결과 아이템 id
        }
    }
    return "F";

}

//ending Scene 들어가기 전 -> endingScene에 필요한게 bitmap = id // stat 처리를 다 해서 id를 구해오는 과정 
void GameManager::PreAdv() 
{

    //1. 스탯을 받아옴
    //2. 기존 장비를 정리함.
    //3. 스탯을 통한 ending id & ending id를 통한 한 세대의 명성치를 받아와야 함. 
    //4. 아이템을 넘겨줌까지 함. 


    int index = GetCurrentGen() - 2; //int curGen 2 ; 

    StatWindow* statwin = dynamic_cast<StatWindow*>(UIManager::Get().GetWindow(UIWindowType::StatsWindow));
    if (statwin != nullptr)
    {
        arrTotalStatus[index] = statwin->GetTotalStatus(); //저장 하고 이거로 보내주면 됨 -> 보내줄거야. 
    }
    UsedEquipedClean(); // temp에다가 저장, 기존 인벤은 싹다 정리함.
    AdvResult(); //명성치와 세대에 따른 인벤 & 임시 vector 업데이트 함. ㅇㅇ 
    TempToNext(); //최종적으로 아이템을 인벤에 넣어둠. 


    // 명성치랑 엔딩에 던져줄 string id 값 준비하기 
    
    // -> ending id를 통해서 명성치까지 계산해야 함. 

   
}


//EndingScene의 Enter 부분인가 
//id를 통해서 비트맵 준비할 거고 
// 그냥 여기서 명성이랑 이런것들 다 나올테니깐

void GameManager::AftAdv() //ending scene -> endingScene의 id를 받을거고 ? 
{
    
    
    
   


    //세대 업데이트 , 
    UpdateGen(); 
}

//장비 스텟이랑 수치값 
void GameManager::PotionUsed(Status_fundamental stat, int Much)
{
    //스탯창 가져와서, 해당 부분에 해당하는 스텟 much 만큼 올리기 
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
        std::cout << "초과인뎁쇼";
}

void GameManager::UsedEquipedClean() //청소하는 김에 추가까지 해버릴래 . 
{
    auto* equipmentWindow = dynamic_cast<EquipmentWindow*>(
        UIManager::Get().GetWindow(UIWindowType::EquipmentWindow));
    if (!equipmentWindow) return;

    // 인벤 가져오기
    auto* inven = dynamic_cast<Inventory*>(
        UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
    if (!inven) return;

    // 아이템 데이터베이스 참조
    auto& invenDB = inven->GetItemBase().GetMap();

    // 장착된 아이템 순회
    for (int i = 0; i < static_cast<int>(Wearable_part::UnKnown); ++i)
    {
        Item* equipped = equipmentWindow->GetEquippedItem((Wearable_part)i);
        if (!equipped) continue;

        std::string id = equipped->m_data.id;

        auto it = invenDB.find(id);
        if (it != invenDB.end())
        {
            // 소유권 이전
            m_tempItem.push_back(std::move(it->second));
            AdvResult_Wep(id); // -> 그 겸사 넣은 거긴 한데, 특정 아이템 들고 모험 나가면 특수한 아이템 넣는 거 그거 한 거임. 
            invenDB.erase(it);
        }
    }

    // 인벤토리 비우기
    inven->ClearAllSlots();

}

void GameManager::AdvResult_Wep(string itemkey)
{
    for (const auto& [key, val] : weaponMap) //val이 곧 아이템임. 
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
    int gen = GetCurrentGen(); //현재 세대의 값 2 3 4 -> 
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
        //너는 마지막이잖아
        break;
    default:
        std::cout << "? 넌 뭐임" << endl;
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
            auto item = bank.CreateItem(id); // unique_ptr<Item> 반환한다고 가정
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
