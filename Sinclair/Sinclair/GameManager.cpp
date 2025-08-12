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

    adv_wepon.assign(11, false); //  -> 이거 4->2로 갈 때 다시 초기화 해야 함. 
    m_wearable_part.assign(static_cast<int>(Wearable_part::UnKnown), false); // 명셩-아이템 풀 관련 

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
     std::unordered_set<std::string>& alreadyPicked // 이미 뽑힌 아이템들
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
                        ++it; // 파츠 중복이면 다음 후보
                    }
                }
                else {
                    ++it; // 아이템 중복이면 다음 후보
                }
            }
            if (!found) break; // 이번 라운드에서 아무것도 못 뽑았으면 종료
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
                return recipe.result; // 결과 아이템 id
        }
    }
    return "F";

}

//ending Scene 들어가기 전 -> endingScene에 필요한게 bitmap = id // stat 처리를 다 해서 id를 구해오는 과정 
void GameManager::PreAdv() 
{
    FindEnding();

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
    TempToNext(); 
   
}




//EndingScene의 Enter 부분인가 
//id를 통해서 비트맵 준비할 거고 
// 그냥 여기서 명성이랑 이런것들 다 나올테니깐

std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID2D1Bitmap1>> GameManager::AftAdv() //ending scene -> endingScene의 id를 받을거고 ? 
{
   
    int index = GetCurrentGen() - 2;
    int id = arrEndingID[index];



  SaveEndingBgm(id);
  
  //-> 얘를 이제 사운드 매니저한테 보내줘야 하는 건가 

    
    

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

        auto it = invenDB.find(id); //결국에 장비창 id도 인벤이 갖고 있으니깐 그거는 인벤에서 가져오는 거고 
        if (it != invenDB.end())
        {
            m_tempItem.push_back(std::move(it->second));
            AdvResult_Wep(id); // -> 그 겸사 넣은 거긴 한데, 특정 아이템 들고 모험 나가면 특수한 아이템 넣는 거 그거 한 거임. 
            invenDB.erase(it); //임시에 넣었으니깐 지웠다는 거잖아 
            equipmentWindow->ClearSlot((Wearable_part)i);

        }
    }

    // 인벤토리 비우기
    inven->ClearAllSlots();

}

void GameManager::AdvResult_Wep(string itemkey) //
{
    Inventory* inven = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));

    int idx = 0;
    for (const auto& [key, val] : weaponMap) // val이 곧 아이템
    {
        if (key == itemkey && adv_wepon[idx] != true ) //true 인경우에는 치울게요 
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

    Inventory* inven = dynamic_cast<Inventory*>(
        UIManager::Get().GetWindow(UIWindowType::InventoryWindow));

    for (auto& [id, count] : it->second)
    {
        for (int i = 0; i < count; ++i)
        {
            auto item = bank.CreateItem(id); // unique_ptr<Item> 반환한다고 가정
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

    auto items = GetRandomItemsByFam(Fam , famToPoolCounts_Gen2to3 , alreadyPicked); //id를 받은거고 
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

void GameManager::Default_Item_TO_Inven(int GEN) //폐기
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
    bool debug = true;

    if(debug)
    {
        cout << "\n=== FindEnding 디버깅 시작 ===" << endl;
        cout << "현재 스탯: 근력(" << arrTotalStatus[curGen - 2].Strength << "), ";
        cout << "마력(" << arrTotalStatus[curGen - 2].Magic_Power << "), ";
        cout << "건강(" << arrTotalStatus[curGen - 2].Health << "), ";
        cout << "지식(" << arrTotalStatus[curGen - 2].Knowledge << "), ";
        cout << "매력(" << arrTotalStatus[curGen - 2].Charm << ")" << endl;
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
        cout << "Positive 스탯 개수: " << positiveStats.size() << " (";
        for (auto stat : positiveStats) {
            cout << GetStatName(stat) << " ";
        }
        cout << ")" << endl;

        cout << "Negative 스탯 개수: " << negativeStats.size() << " (";
        for (auto stat : negativeStats) {
            cout << GetStatName(stat) << " ";
        }
        cout << ")" << endl;
    }


    // 전체 상위 엔딩 (모든 스탯 50% 이상)
    if (positiveStats.size() >= 5) {
        arrEndingID[curGen - 2] = 3011;
        arrTotalFam[curGen - 2] = 7;
        if (debug) cout << "결과: 전체 상위 엔딩 (3011 - 선택받은 자)" << endl;
        return;
    }

    // 전체 하위 엔딩 (모든 스탯 20% 이하)
    if (negativeStats.size() >= 5) {
        arrEndingID[curGen - 2] = 3031;
        arrTotalFam[curGen - 2] = 1;
        if (debug) cout << "결과: 전체 하위 엔딩 (3031 - 평범한 가정)" << endl;
        return;
    }

    // positive가 없는 경우 중간 엔딩
    if (positiveStats.size() == 0) {
        arrEndingID[curGen - 2] = 3021;
        arrTotalFam[curGen - 2] = 2;
        if (debug) cout << "결과: 중간 엔딩 (3021 - 모험가) - positive 스탯 없음" << endl;
        return;
    }

    // positive 스탯들을 값 기준으로 내림차순 정렬 (가장 높은 것부터)
    sort(positiveStats.begin(), positiveStats.end(),
        [&statMap](Status_Total a, Status_Total b) {
            return statMap[a] > statMap[b];
        });

    if (debug)
    {
        cout << "Positive 스탯 정렬 후 (값 기준): ";
        for (auto stat : positiveStats) {
            cout << GetStatName(stat) << "(" << statMap[stat] << ") ";
        }
        cout << endl;
    }

    // negative 스탯들을 값 기준으로 오름차순 정렬 (가장 낮은 것부터)
    sort(negativeStats.begin(), negativeStats.end(),
        [&statMap](Status_Total a, Status_Total b) {
            return statMap[a] < statMap[b];
        });

    if (debug)
    {
        cout << "Negative 스탯 정렬 후 (값 기준): ";
        for (auto stat : negativeStats) {
            cout << GetStatName(stat) << "(" << statMap[stat] << ") ";
        }
        cout << endl;
    }

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


    if (debug)
    {
        cout << "최종 매칭 조건 - Positive: ";
        for (auto stat : finalPositive) {
            cout << GetStatName(stat) << " ";
        }
        cout << ", Negative: " << GetStatName(finalNegative) << endl;
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
        arrTotalFam[curGen - 2] = ending.fame;

        if(debug) cout << "매칭된 엔딩: " << ending.ID << " - " << ending.job << endl;
        return;
    }

    // 매칭되는 엔딩이 없으면 기본 엔딩 (중)
    arrTotalFam[curGen - 2] = 2;
    arrEndingID[curGen - 2] = 3021;
    if (debug)
    {
        cout << "결과: 기본 중간 엔딩 (3021 - 모험가) - 매칭 실패" << endl;
    }
}

void GameManager::SaveEndingBgm(int id)
{

   auto st_id = std::to_string(id);
   //endingBgm = ResourceManager::Get().Get_SoundBank().GetEndingBGM(st_id); //그냥 멤버에 담아두었다가 해제 식

   if (GetCurrentGen() == 4)
   {
       //historyBgm = ResourceManager::Get().Get_SoundBank().GetEndingBGM(st_id); //일단 임시값 
   }

}
