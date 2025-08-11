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
        statwin->ResetStat(); 
    }
    UsedEquipedClean(); // temp에다가 저장, 기존 인벤은 싹다 정리함.
    AdvResult(); //명성치와 세대에 따른 인벤 & 임시 vector 업데이트 함. ㅇㅇ 


    TempToNext(); //최종적으로 아이템을 인벤에 넣어둠. 
    Default_Item_TO_Inven(GetCurrentGen());

    // 명성치랑 엔딩에 던져줄 string id 값 준비하기 
    
    // -> ending id를 통해서 명성치까지 계산해야 함. 

   
}


//EndingScene의 Enter 부분인가 
//id를 통해서 비트맵 준비할 거고 
// 그냥 여기서 명성이랑 이런것들 다 나올테니깐

std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> GameManager::AftAdv() //ending scene -> endingScene의 id를 받을거고 ? 
{
    FindEnding();
    int index = GetCurrentGen() - 2;
    int id = arrEndingID[index];
    
    //SceneManager로 엔딩씬을 받아옴? -> 멤버에다가 담아줌? 

    std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> ending_bitmap =
        ResourceManager::Get().GetEndingBitmap(std::to_string(id));
    
  //  UpdateGen(); 

    return ending_bitmap;
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
            equipmentWindow->ClearSlot((Wearable_part)i);

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
    //int this_Gen_Fam = GetCurrentFam();

    //int gen = GetCurrentGen(); //현재 세대의 값 2 3 4 -> 

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

    auto items = GetRandomItemsByFam(Fam , famToPoolCounts_Gen2to3); //id를 받은거고 
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
    if (GEN == 2) //기본 3 
    {
        Inventory* inven = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
        if (inven != nullptr)
        {
            inven-> LoadItemDatabase(Need_Moment::Gen_3);
            inven->PackItem();

        }
    }

    if (GEN == 3) //기본 4 
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

    // 전체 상위 엔딩 (모든 스탯 50% 이상)
    if (positiveStats.size() >= 5) {
        arrEndingID[curGen - 2] = 3011;
        return;
    }

    // 전체 하위 엔딩 (모든 스탯 20% 이하)
    if (negativeStats.size() >= 5) {
        arrEndingID[curGen - 2] = 3031;
        return;
    }

    // positive가 없는 경우 중간 엔딩
    if (positiveStats.size() == 0) {
        arrEndingID[curGen - 2] = 3021;
        return;
    }

    // positive 스탯들을 값 기준으로 내림차순 정렬 (가장 높은 것부터)
    sort(positiveStats.begin(), positiveStats.end(),
        [&statMap](Status_Total a, Status_Total b) {
            return statMap[a] > statMap[b];
        });

    // negative 스탯들을 값 기준으로 오름차순 정렬 (가장 낮은 것부터)
    sort(negativeStats.begin(), negativeStats.end(),
        [&statMap](Status_Total a, Status_Total b) {
            return statMap[a] < statMap[b];
        });

    // 엔딩 매칭을 위한 조건 설정
    vector<Status_Total> finalPositive;
    Status_Total finalNegative = Status_Total::Null;

    if (positiveStats.size() >= 2) {
        // 2개 이상이면 가장 높은 2개 선택
        finalPositive.push_back(positiveStats[0]);
        finalPositive.push_back(positiveStats[1]);

        // positive 조건을 enum 값 기준으로 정렬 (엔딩 데이터와 매칭을 위해)
        sort(finalPositive.begin(), finalPositive.end(),
            [](Status_Total lhs, Status_Total rhs) {
                return static_cast<int>(lhs) < static_cast<int>(rhs);
            });
    }
    else if (positiveStats.size() == 1) {
        // 1개면 그것만 사용
        finalPositive.push_back(positiveStats[0]);
    }

    // negative 조건 설정
    if (!negativeStats.empty()) {
        finalNegative = negativeStats[0]; // 가장 낮은 스탯
    }

    // 엔딩 매칭
    for (const auto& ending : ResourceManager::Get().Get_TextBank().EndingVector) {
        // positive 조건 구성
        vector<Status_Total> requiredPositive;
        if (ending.positive.first != Status_Total::Null) {
            requiredPositive.push_back(ending.positive.first);
        }
        if (ending.positive.second != Status_Total::Null) {
            requiredPositive.push_back(ending.positive.second);
        }

        // positive 조건 개수 확인
        if (requiredPositive.size() != finalPositive.size()) {
            continue;
        }

        // positive 조건 매칭 확인 (정렬된 상태에서 비교)
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

        // negative 조건 확인
        if (ending.negative != finalNegative) {
            continue;
        }

        // 모든 조건이 매칭되면 해당 엔딩 선택
        arrEndingID[curGen - 2] = ending.ID;
        return;
    }

    // 매칭되는 엔딩이 없으면 기본 엔딩 (중)
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
//        // 상
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
//            // 하
//            arrEndingID[curGen - 2] = 3031;
//            return;
//        }
//        else
//        {
//            // 중
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
//        // positive 조건 체크
//        vector<Status_Total> requiredPositive;
//
//        // positive.first 추가
//        if (ending.positive.first != Status_Total::Null) {
//            requiredPositive.push_back(ending.positive.first);
//        }
//
//        // positive.second 추가 (Null이 아닌 경우)
//        if (ending.positive.second != Status_Total::Null) {
//            requiredPositive.push_back(ending.positive.second);
//        }
//
//        // positive 조건 개수가 일치하는지 확인
//        if (requiredPositive.size() != positiveStats.size()) {
//            continue;
//        }
//
//        // positive 조건이 모두 매칭되는지 확인
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
//        // negative 조건 체크
//        if (ending.negative == Status_Total::Null) {
//            // 엔딩에서 negative 조건이 없는 경우
//            if (!negativeStats.empty()) {
//                continue; // 플레이어에게 negative가 있으면 매칭 안됨
//            }
//        }
//        else {
//            // 엔딩에서 negative 조건이 있는 경우
//            if (negativeStats.size() != 1 || negativeStats[0] != ending.negative) {
//                continue; // negative 조건이 정확히 매칭되지 않음
//            }
//        }
//
//        // 모든 조건이 매칭되면 해당 엔딩 선택
//        arrEndingID[curGen - 2] = ending.ID;
//        return;
//    }
//
//    // 매칭되는 엔딩이 없으면 기본 엔딩 (중)
//    arrEndingID[curGen - 2] = 3021;
//
//}

