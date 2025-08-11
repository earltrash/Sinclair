#pragma once
#include "pch.h"
using namespace std;
#include <fstream>
#include <sstream>
#include <optional>
#include "Status.h"

class TextBank
{
public:
    TextBank() = default;
    ~TextBank()= default;

    /// <summary>
    /// 유틸리티 함수
    /// string replaceGeneration(const string& text, int generation)
    /// </summary>

    string replaceGeneration(const string& text, int generation);

    /// <summary>
    /// Ending에 관한 것
    /// struct Ending
    /// vector<Ending> parseTSV(const string& filename)
    /// </summary>

    // 이걸로 바꿔야 함
    struct Ending
    {
        int ID;
        pair<Status_Total, Status_Total> positive;
        Status_Total negative; // ? 면 없는걸로
        int fame;
        string job;
        string endingText;
    };

    // 문자열을 스탯 타입으로 변환하는 함수
    Status_Total stringToStatType(string statStr)
    {
        if (statStr == "근력") return Status_Total::Strength;
        else if (statStr == "마력") return Status_Total::Magic_Power;
        else if (statStr == "건강") return Status_Total::Health;
        else if (statStr == "지식") return Status_Total::Knowledge;
        else if (statStr == "매력") return Status_Total::Charm;
        else return Status_Total::Null; 
    }


//private: //일단 퍼블릭
public:
     vector<Ending>  EndingVector{
            {1011, {Status_Total::Strength, Status_Total::Magic_Power}, Status_Total::Health, 5, "마검사",     "싱클레어 n세는 마법과 검술\n모두를 완벽히 익혔다.\n결투에서만큼은\n누구도 그를 쓰러뜨릴 수 없었다.\n하지만 수많은 결투의\n후유증은 남았고,\n싱클레어 n세는 향년 38세의 나이에\n검을 내려놓게 되었다."},
            {1012, {Status_Total::Strength, Status_Total::Magic_Power}, Status_Total::Knowledge, 5, "마검사",  "싱클레어 n세는 마법과 검술\n모두를 완벽히 익혔다.\n결투에서만큼은\n누구도 그를 쓰러뜨릴 수 없었다.\n명성을 바탕으로\n무기 사업을 시작했지만,\n정작 계산과 운영은 그와 맞지 않았다."},
            {1013, {Status_Total::Strength, Status_Total::Magic_Power}, Status_Total::Charm, 5, "마검사",      "싱클레어 n세는 마법과 검술\n모두를 완벽히 익혔다.\n결투에서만큼은\n누구도 그를 쓰러뜨릴 수 없었다.\n하지만 검을 내려놓은 그 곁에는\n아무도 남아 있지 않았다."},
            {1014, {Status_Total::Strength, Status_Total::Magic_Power}, Status_Total::Null, 6, "마검사",       "싱클레어 n세는 마법과 검술\n모두를 완벽히 익혔다.\n결투에서만큼은\n누구도 그를 쓰러뜨릴 수 없었다.\n사람들은 그를\n'완성된 전사'로 불렀다."},
            {1021, {Status_Total::Strength, Status_Total::Health}, Status_Total::Magic_Power, 5, "성기사",     "싱클레어 n세는\n방패로 아군을 지키며,\n검으로 신념을 실현한 전사였다.\n단신의 몸으로\n도시를 지켰다는 이야기는\n지금도 전설처럼 전해진다.\n신의 가호는 깃들었지만,\n마력은 끝내 깨어나지 않았다."},
            {1022, {Status_Total::Strength, Status_Total::Health}, Status_Total::Knowledge, 5, "성기사",       "싱클레어 n세는\n방패로 아군을 지키며,\n검으로 신념을 실현한 전사였다.\n단신의 몸으로\n도시를 지켰다는 이야기는\n지금도 전설처럼 전해진다.\n언제나 싱클레어 n세는\n이익보다는 신념을 따랐고,\n그래서 그는 늘 손해를 감수해야 했다."},
            {1023, {Status_Total::Strength, Status_Total::Health}, Status_Total::Charm, 5, "성기사",           "싱클레어 n세는\n방패로 아군을 지키며,\n검으로 신념을 실현한 전사였다.\n단신의 몸으로\n도시를 지켰다는 이야기는\n지금도 전설처럼 전해진다.\n싱클레어 n세의 방패는\n모두를 지켜냈지만,\n정작 마지막까지 곁에 남은 이는 없었다.	"},
            {1024, {Status_Total::Strength, Status_Total::Health}, Status_Total::Null, 6, "성기사",            "싱클레어 n세는\n방패로 아군을 지키며,\n검으로 신념을 실현한 전사였다.\n단신의 몸으로\n도시를 지켰다는 이야기는\n지금도 전설처럼 전해진다.\n그렇게 싱클레어 n세는\n'정의의 상징'이 되었다."},
            {1031, {Status_Total::Strength, Status_Total::Knowledge}, Status_Total::Magic_Power, 5, "전술가",  "한 발짝 먼저 움직이고,\n한 수 먼저 읽었다.\n그것이 싱클레어 n세가\n전장을 지배한 방식이었다.\n그러나 마법 전략을 짤 줄은 알았지만,\n그에게 마력은 끝내 깨어나지 않았다."},
            {1032, {Status_Total::Strength, Status_Total::Knowledge}, Status_Total::Health, 5, "전술가",       "한 발짝 먼저 움직이고,\n한 수 먼저 읽었다.\n그것이 싱클레어 n세가\n전장을 지배한 방식이었다.\n하지만 싱클레어 n세는\n병약한 몸을 버티지 못하고\n48세의 나이에 생을 마감했다."},
            {1033, {Status_Total::Strength, Status_Total::Knowledge}, Status_Total::Charm, 5, "전술가",        "한 발짝 먼저 움직이고,\n한 수 먼저 읽었다.\n그것이 싱클레어 n세가\n전장을 지배한 방식이었다.\n하지만 사람의 마음은 읽지 못했고,\n싱클레어 n세의 곁에 머무는 이는\n드물었다."},
            {1034, {Status_Total::Strength, Status_Total::Knowledge}, Status_Total::Null, 6, "전술가",         "한 발짝 먼저 움직이고,\n한 수 먼저 읽었다.\n그것이 싱클레어 n세가\n전장을 지배한 방식이었다.\n싱클레어 n세의 전술서는\n지금도 '장교들의 필독서'로 남아 있다."},
            {1041, {Status_Total::Strength, Status_Total::Charm}, Status_Total::Magic_Power, 5, "기사단장",    "검을 든 자들을\n하나로 이끌어낸 싱클레어 n세.\n그가 이끌던 기사단은\n해체된 후에도 서로를 형제로 불렀고,\n그 유대는 끊이지 않았다.\n싱클레어 n세는 늘 아쉬워했다.\n마법까지 다룰 수 있었다면\n더 많은 이들이 함께 했을지도 모른다.	"},
            {1042, {Status_Total::Strength, Status_Total::Charm}, Status_Total::Health, 5, "기사단장",         "검을 든 자들을\n하나로 이끌어낸 싱클레어 n세.\n그가 이끌던 기사단은\n해체된 후에도 서로를 형제로 불렀고,\n그 유대는 끊이지 않았다.\n하지만 지속된 전투의 대가는 컸다.\n결국 싱클레어 n세는\n34세의 나이에\n전우들보다 먼저 떠나게 되었다."},
            {1043, {Status_Total::Strength, Status_Total::Charm}, Status_Total::Knowledge, 5, "기사단장",      "검을 든 자들을\n하나로 이끌어낸 싱클레어 n세.\n그가 이끌던 기사단은\n해체된 후에도 서로를 형제로 불렀고,\n그 유대는 끊이지 않았다.\n명성을 바탕으로 훈련소를 차렸지만,\n운영보다는 전장에 더 잘 어울리는\n싱클레어 n세였다."},
            {1044, {Status_Total::Strength, Status_Total::Charm}, Status_Total::Null, 6, "기사단장",           "검을 든 자들을\n하나로 이끌어낸 싱클레어 n세.\n그가 이끌던 기사단은\n해체된 후에도 서로를 형제로 불렀고,\n그 유대는 끊이지 않았다.\n그가 떠난 후에도,\n그 아래 맺어진 유대는\n아직 남아 있다."},
            {1051, {Status_Total::Magic_Power, Status_Total::Health}, Status_Total::Strength, 5, "고위 신관",  "싱클레어 n세는\n도움이 필요한 사람들을 위해\n험난한 길 위를 걸었다.\n싱클레어 n세가 지나간 곳에\n성소가 세워지며,\n그들의 영원한 안식처가 되어주었다.\n그러나 체력이 부족한 그는\n성스러운 의식을 집행한 날이면\n성소 한편에서 오래 쉬어야 했다."},
            {1052, {Status_Total::Magic_Power, Status_Total::Health}, Status_Total::Knowledge, 5, "고위 신관", "싱클레어 n세는\n도움이 필요한 사람들을 위해\n험난한 길 위를 걸었다.\n싱클레어 n세가 지나간 곳에\n성소가 세워지며,\n그들의 영원한 안식처가 되어주었다.\n경전을 외우지는 못했지만,\n그의 기도에는\n신의 숨결이 분명 들어있었다."},
            {1053, {Status_Total::Magic_Power, Status_Total::Health}, Status_Total::Charm, 5, "고위 신관",     "싱클레어 n세는\n도움이 필요한 사람들을 위해\n험난한 길 위를 걸었다.\n싱클레어 n세가 지나간 곳에\n성소가 세워지며,\n그들의 영원한 안식처가 되어주었다.\n수 많은 이를 위로했지만,\n정작 그를 위로한 사람은 드물었다."},
            {1054, {Status_Total::Magic_Power, Status_Total::Health}, Status_Total::Null, 6, "고위 신관",      "싱클레어 n세는\n도움이 필요한 사람들을 위해\n험난한 길 위를 걸었다.\n싱클레어 n세가 지나간 곳에\n성소가 세워지며,\n그들의 영원한 안식처가 되어주었다.\n지금도 사람들은\n그가 남긴 길 위에서\n함께 기도를 올린다."},
            {1061, {Status_Total::Magic_Power, Status_Total::Knowledge}, Status_Total::Strength, 5, "대현자",  "모두가 포기한 고서의 비밀을\n끝까지 파고든 싱클레어 n세는\n세상에서 잊혀진 고대 마법을\n다시금 세상에 전했다.\n싱클레어 n세가 남긴 연구는\n마법사들의 교과서가 되었다.\n반복된 밤샘 연구로\n점점 몸이 약해진 싱클레어 n세는\n결국 조용한 숲 속에서 요양하게 되었다."},
            {1062, {Status_Total::Magic_Power, Status_Total::Knowledge}, Status_Total::Health, 5, "대현자",    "모두가 포기한 고서의 비밀을\n끝까지 파고든 싱클레어 n세는\n세상에서 잊혀진 고대 마법을\n다시금 세상에 전했다.\n싱클레어 n세가 남긴 연구는\n마법사들의 교과서가 되었다.\n하지만 무리한 연구는\n그의 건강을 악화시켰고,\n43세의 나이에 책을 덮게 되었다."},
            {1063, {Status_Total::Magic_Power, Status_Total::Knowledge}, Status_Total::Charm, 5, "대현자",     "모두가 포기한 고서의 비밀을\n끝까지 파고든 싱클레어 n세는\n세상에서 잊혀진 고대 마법을\n다시금 세상에 전했다.\n싱클레어 n세가 남긴 연구는\n마법사들의 교과서가 되었다.\n모두가 그의 지혜를 빌렸지만,\n그의 마음을 이해한 이는 끝내 없었다."},
            {1064, {Status_Total::Magic_Power, Status_Total::Knowledge}, Status_Total::Null, 6, "대현자",      "모두가 포기한 고서의 비밀을\n끝까지 파고든 싱클레어 n세는\n세상에서 잊혀진 고대 마법을\n다시금 세상에 전했다.\n싱클레어 n세가 남긴 연구는\n마법사들의 교과서가 되었다.\n그가 적은 문장들은\n지금도 수 많은 마법사들이 사용하고 있다."},
            {1071, {Status_Total::Magic_Power, Status_Total::Charm}, Status_Total::Strength, 5, "소환술사",    "싱클레어 n세는\n다른 이들이 볼 수 없는 존재들과도\n자연스럽게 대화를 나눴다.\n싱클레어 n세의 곁에는\n언제나 그들이 머물러 있었다.\n하지만 지속된 소환술은\n싱클레어 n세의 몸을 조금씩 무너뜨렸고,\n그는 가끔씩 며칠 동안 말을 잃곤 했다."},
            {1072, {Status_Total::Magic_Power, Status_Total::Charm}, Status_Total::Health, 5, "소환술사",      "싱클레어 n세는\n다른 이들이 볼 수 없는 존재들과도\n자연스럽게 대화를 나눴다.\n싱클레어 n세의 곁에는\n언제나 그들이 머물러 있었다.\n반복된 소환에\n마력은 점점 그를 잠식했고,\n결국 그 또한\n보이지 않는 존재가 되어버렸다."},
            {1073, {Status_Total::Magic_Power, Status_Total::Charm}, Status_Total::Knowledge, 5, "소환술사",   "싱클레어 n세는\n다른 이들이 볼 수 없는 존재들과도\n자연스럽게 대화를 나눴다.\n싱클레어 n세의 곁에는\n언제나 그들이 머물러 있었다.\n하지만, 어리숙한 싱클레어 n세는\n복잡한 소환의식에서\n종종 실수하기도 했다."},
            {1074, {Status_Total::Magic_Power, Status_Total::Charm}, Status_Total::Null, 6, "소환술사",        "싱클레어 n세는\n다른 이들이 볼 수 없는 존재들과도\n자연스럽게 대화를 나눴다.\n싱클레어 n세의 곁에는\n언제나 그들이 머물러 있었다."},
            {1081, {Status_Total::Health, Status_Total::Knowledge}, Status_Total::Strength, 5, "약술사",       "역병이 퍼진 마을에\n 누군가는 들어가야 했다.\n 싱클레어 n세는\n 자신이 여정을 통해 얻은\n 약초와 지식을 꺼내 들었다.\n 싱클레어 n세가 다녀간 마을엔\n 다시 생기가 돌아왔다.\n 금세 지치는 몸이었지만,\n 그의 손은 사람들을 위해\n 언제나 약을 제조하곤 했다."},
            {1082, {Status_Total::Health, Status_Total::Knowledge}, Status_Total::Magic_Power, 5, "약술사",    "역병이 퍼진 마을에\n 누군가는 들어가야 했다.\n 싱클레어 n세는\n 자신이 여정을 통해 얻은\n 약초와 지식을 꺼내 들었다.\n 싱클레어 n세가 다녀간 마을엔\n 다시 생기가 돌아왔다.\n 마법 없이 만든 약이었지만,\n 사람들은 싱클레어의 약 안에\n 기적이 들어있다 말하곤 했다."},
            {1083, {Status_Total::Health, Status_Total::Knowledge}, Status_Total::Charm, 5, "약술사",          "역병이 퍼진 마을에\n 누군가는 들어가야 했다.\n 싱클레어 n세는\n 자신이 여정을 통해 얻은\n 약초와 지식을 꺼내 들었다.\n 싱클레어 n세가 다녀간 마을엔\n 다시 생기가 돌아왔다.\n 그를 찾아오는 사람은 많았지만,\n 마지막엔 결국 가족만 곁에 남았다."},
            {1084, {Status_Total::Health, Status_Total::Knowledge}, Status_Total::Null, 6, "약술사",           "역병이 퍼진 마을에\n 누군가는 들어가야 했다.\n 싱클레어 n세는\n 자신이 여정을 통해 얻은\n 약초와 지식을 꺼내 들었다.\n 싱클레어 n세가 다녀간 마을엔\n 다시 생기가 돌아왔다."},
            {1091, {Status_Total::Health, Status_Total::Charm}, Status_Total::Strength, 5, "신탁자",           "사람들은 앞날을 묻기 위해\n 싱클레어 n세를 찾았고,\n 그는 길지 않은 말 한마디로\n 방향을 제시해주었다.\n 그 말들이 많은 마을들의\n 운명을 바꾸곤 했다.\n 예언을 전한 뒤엔 늘 기력이 빠졌고,\n 종종 며칠씩 눈을 감은 채 보내곤 했다."},
            {1092, {Status_Total::Health, Status_Total::Charm}, Status_Total::Magic_Power, 5, "신탁자",        "사람들은 앞날을 묻기 위해\n 싱클레어 n세를 찾았고,\n 그는 길지 않은 말 한마디로\n 방향을 제시해주었다.\n 그 말들이 많은 마을들의\n 운명을 바꾸곤 했다.\n 마력은 깨어나지 않았지만,\n 신은 싱클레어 n세를 통해\n 세상에 말을 전달했다."},
            {1093, {Status_Total::Health, Status_Total::Charm}, Status_Total::Knowledge, 5, "신탁자",          "사람들은 앞날을 묻기 위해\n 싱클레어 n세를 찾았고,\n 그는 길지 않은 말 한마디로\n 방향을 제시해주었다.\n 그 말들이 많은 마을들의\n 운명을 바꾸곤 했다.\n 입소문을 바탕으로\n 작은 사업을 시작했지만,\n 경영에는 그의 예언이 통하지 않았다."},
            {1094, {Status_Total::Health, Status_Total::Charm}, Status_Total::Null, 6, "신탁자",               "사람들은 앞날을 묻기 위해\n 싱클레어 n세를 찾았고,\n 그는 길지 않은 말 한마디로\n 방향을 제시해주었다.\n 그 말들이 많은 마을들의\n 운명을 바꾸곤 했다.\n 그가 영원히 잠들기 전까지\n 신은 늘 그와 함께였다."},
            {1101, {Status_Total::Knowledge, Status_Total::Charm}, Status_Total::Strength, 5, "재상",          "검 한 번 들지 않고도\n 사람을 움직이던 싱클레어 n세.\n 그의 존재는 왕국에 긴 번영을\n 가져다 주었다.\n 하지만 체력은 따라주지 않았고,\n 희의 중 잠시 눈을 붙이는 일도\n 종종 있었다."},
            {1102, {Status_Total::Knowledge, Status_Total::Charm}, Status_Total::Magic_Power, 5, "재상",       "검 한 번 들지 않고도\n 사람을 움직이던 싱클레어 n세.\n 그의 존재는 왕국에 긴 번영을\n 가져다 주었다.\n 싱클레어 n세는\n 마법을 다룰 수 없었지만,\n 그의 한 마디에 마법사들조차\n 고개를 끄덕였다."},
            {1103, {Status_Total::Knowledge, Status_Total::Charm}, Status_Total::Health, 5, "재상",            "검 한 번 들지 않고도\n 사람을 움직이던 싱클레어 n세.\n 그의 존재는 왕국에 긴 번영을\n 가져다 주었다.\n 무거운 책임감은\n 싱클레어 n세의 건강을\n 조금씩 악화시켰고,\n 결국 41세의 나이에\n 조용히 자리를 내려놓았다."},
            {1104, {Status_Total::Knowledge, Status_Total::Charm}, Status_Total::Null, 6, "재상",              "검 한 번 들지 않고도\n 사람을 움직이던 싱클레어 n세.\n 그의 존재는 왕국에 긴 번영을\n 가져다 주었다.\n 그가 떠난 후에도,\n 왕국은 그의 말 한마디를 원했다."},
            {2011, {Status_Total::Strength, Status_Total::Null}, Status_Total::Magic_Power, 3, "검사",         "모험을 통해 다양한 검술을\n 익히게 된 싱클레어 n세.\n 고향으로 돌아와 검술 학교를 세워\n 마을을 지킬 이들을 양성했다.\n 늦게 마법을 배워보려 했지만,\n 끝까지 손에 익은 건\n 검 한 자루 뿐이었다."},
            {2012, {Status_Total::Strength, Status_Total::Null}, Status_Total::Health, 3, "검사",              "모험을 통해 다양한 검술을\n 익히게 된 싱클레어 n세.\n 고향으로 돌아와 검술 학교를 세워\n 마을을 지킬 이들을 양성했다.\n 하지만 육신은 오래 버텨내지 못했고,\n 그는 50세의 나이에\n 마지막 수업을 마쳤다."},
            {2013, {Status_Total::Strength, Status_Total::Null}, Status_Total::Knowledge, 3, "검사",           "모험을 통해 다양한 검술을\n 익히게 된 싱클레어 n세.\n 고향으로 돌아와 검술 학교를 세워\n 마을을 지킬 이들을 양성했다.\n 하지만 체계적인 수업 계획은\n 그에겐 늘 어려운 숙제였다."},
            {2014, {Status_Total::Strength, Status_Total::Null}, Status_Total::Charm, 3, "검사",               "모험을 통해 다양한 검술을\n 익히게 된 싱클레어 n세.\n 고향으로 돌아와 검술 학교를 세워\n 마을을 지킬 이들을 양성했다.\n 검을 배우려는 제자들은\n 끊이지 않았지만,\n 끝내 그의 곁에\n 친구라 부를 이는 없었다."},
            {2015, {Status_Total::Strength, Status_Total::Null}, Status_Total::Null, 4, "검사",                "모험을 통해 다양한 검술을\n 익히게 된 싱클레어 n세.\n 고향으로 돌아와 검술 학교를 세워\n 마을을 지킬 이들을 양성했다.\n 그가 전수한 검술은\n 지금도 마을을 수호하고 있다."},
            {2021, {Status_Total::Magic_Power, Status_Total::Null}, Status_Total::Strength, 3, "마법사",       "처음에는 기초 마법도 실패했지만,\n 포기하지 않았다.\n 결국 싱클레어 n세는\n 자신만의 마법을 완성했고,\n 그가 세운 작은 연구실은\n 지금도 그의 제자들로 붐빈다.\n 하지만 마법을 한 번 쓸 때마다\n 긴 휴식이 필요했고,\n 제자들은 그런 그를\n '느린 마법사'로 부르기도 했다."},
            {2022, {Status_Total::Magic_Power, Status_Total::Null}, Status_Total::Health, 3, "마법사",         "처음에는 기초 마법도 실패했지만,\n 포기하지 않았다.\n 결국 싱클레어 n세는\n 자신만의 마법을 완성했고,\n 그가 세운 작은 연구실은\n 지금도 그의 제자들로 붐빈다.\n 하지만 잔병치레가 잦았던 그는\n 46세에 조용히 눈을 감았다."},
            {2023, {Status_Total::Magic_Power, Status_Total::Null}, Status_Total::Knowledge, 3, "마법사",      "처음에는 기초 마법도 실패했지만,\n 포기하지 않았다.\n 결국 싱클레어 n세는\n 자신만의 마법을 완성했고,\n 그가 세운 작은 연구실은\n 지금도 그의 제자들로 붐빈다.\n 하지만, 그는 마법의 원리를\n 말로 표현하는 데 서툴었고\n 제자들은 감각으로\n 그의 마법을 익혀야 했다."},
            {2024, {Status_Total::Magic_Power, Status_Total::Null}, Status_Total::Charm, 3, "마법사",          "처음에는 기초 마법도 실패했지만,\n 포기하지 않았다.\n 결국 싱클레어 n세는\n 자신만의 마법을 완성했고,\n 그가 세운 작은 연구실은\n 지금도 그의 제자들로 붐빈다.\n 수많은 제자들이\n 그에게 마법을 배웠지만,\n 그와 진정으로\n 대화를 나누는 이는 없었다."},
            {2025, {Status_Total::Magic_Power, Status_Total::Null}, Status_Total::Null, 4, "마법사",           "처음에는 기초 마법도 실패했지만,\n 포기하지 않았다.\n 결국 싱클레어 n세는\n 자신만의 마법을 완성했고,\n 그가 세운 작은 연구실은\n 지금도 그의 제자들로 붐빈다.\n 지금도 연구실 한 켠에\n 그의 이름이 새겨져 있다."},
            {2031, {Status_Total::Health, Status_Total::Null}, Status_Total::Strength, 3, "치유사",            "모험에서 다친 싱클레어 n세.\n 의문의 치유사가 그에게 다가와\n 치료해주었다.\n 이후 치유계에 관심을 가진\n 싱클레어 n세는 고향에 돌아와\n 치유법을 연구하며,\n 사람들의 아픔을 돌보았다.\n 체력이 부족한 싱클레어 n세는\n 자주 지쳤지만,\n 늘 마지막 환자까지 돌본 뒤에야\n 몸을 눕혔다."},
            {2032, {Status_Total::Health, Status_Total::Null}, Status_Total::Magic_Power, 3, "치유사",         "모험에서 다친 싱클레어 n세.\n 의문의 치유사가 그에게 다가와\n 치료해주었다.\n 이후 치유계에 관심을 가진\n 싱클레어 n세는 고향에 돌아와\n 치유법을 연구하며,\n 사람들의 아픔을 돌보았다.\n 싱클레어 n세는 마법은 쓰지 못했지만,\n 사람들은 그의 손 끝에서\n 기적이 일어난다고 말했다."},
            {2033, {Status_Total::Health, Status_Total::Null}, Status_Total::Knowledge, 3, "치유사",           "모험에서 다친 싱클레어 n세.\n 의문의 치유사가 그에게 다가와\n 치료해주었다.\n 이후 치유계에 관심을 가진\n 싱클레어 n세는 고향에 돌아와\n 치유법을 연구하며,\n 사람들의 아픔을 돌보았다.\n 전문적인 지식은 부족했지만,\n 그는 누구보다 먼저 손을 내미는\n 치유사였다."},
            {2034, {Status_Total::Health, Status_Total::Null}, Status_Total::Charm, 3, "치유사",               "모험에서 다친 싱클레어 n세.\n 의문의 치유사가 그에게 다가와\n 치료해주었다.\n 이후 치유계에 관심을 가진\n 싱클레어 n세는 고향에 돌아와\n 치유법을 연구하며,\n 사람들의 아픔을 돌보았다.\n 많은 이들의 상처를 어루만졌지만,\n 그의 마음을 살펴 주는 이들은 드물었다."},
            {2035, {Status_Total::Health, Status_Total::Null}, Status_Total::Null, 4, "치유사",                "모험에서 다친 싱클레어 n세.\n 의문의 치유사가 그에게 다가와\n 치료해주었다.\n 이후 치유계에 관심을 가진\n 싱클레어 n세는 고향에 돌아와\n 치유법을 연구하며,\n 사람들의 아픔을 돌보았다.\n 싱클레어 n세의 고향에서는\n 누군가 아프면\n 여전히 그의 이름을 부른다."},
            {2041, {Status_Total::Knowledge, Status_Total::Null}, Status_Total::Strength, 3, "학자",          "학문에 대한 열정으로\n 모험을 시작한 싱클레어 n세.\n 그의 연구가 국왕에 눈에 띄었고,\n 그는 왕실 연구원으로 발탁되었다.\n 체력이 약했던 싱클레어 n세는,\n 종종 책상 앞에서 쓰러지곤 했다."},
            {2042, {Status_Total::Knowledge, Status_Total::Null}, Status_Total::Magic_Power, 3, "학자",       "학문에 대한 열정으로\n 모험을 시작한 싱클레어 n세.\n 그의 연구가 국왕에 눈에 띄었고,\n 그는 왕실 연구원으로 발탁되었다.\n 그러나 마법에 대해선 원리만 알 뿐,\n 끝내 직접 마법을 부릴 수는 없었다."},
            {2043, {Status_Total::Knowledge, Status_Total::Null}, Status_Total::Health, 3, "학자",            "학문에 대한 열정으로\n 모험을 시작한 싱클레어 n세.\n 그의 연구가 국왕에 눈에 띄었고,\n 그는 왕실 연구원으로 발탁되었다.\n 몸이 허약했던 싱클레어 n세는\n 52세에 마지막 기록을 남기고\n 조용히 눈을 감았다."},
            {2044, {Status_Total::Knowledge, Status_Total::Null}, Status_Total::Charm, 3, "학자",             "학문에 대한 열정으로\n 모험을 시작한 싱클레어 n세.\n 그의 연구가 국왕에 눈에 띄었고,\n 그는 왕실 연구원으로 발탁되었다.\n 지식은 쌓여갔고 이름도 남았지만,\n 그의 곁엔 마음을 나눌 이가 없었다."},
            {2045, {Status_Total::Knowledge, Status_Total::Null}, Status_Total::Null, 4, "학자",              "학문에 대한 열정으로\n 모험을 시작한 싱클레어 n세.\n 그의 연구가 국왕에 눈에 띄었고,\n 그는 왕실 연구원으로 발탁되었다.\n 그의 이름은 매해 왕립 아카데미에서\n 언급이 되곤 한다."},
            {2051, {Status_Total::Charm, Status_Total::Null}, Status_Total::Strength, 3, "외교관",             "모험 중 만난 수많은 이들과의\n 소통을 통해 화술을 갈고 닦은\n 싱클레어 n세.\n 그는 결국 왕국의 외교관으로\n 임명되었다.\n 하지만 허약했던 싱클레어 n세에게\n 장시간의 외교 일정은 무리였다."},
            {2052, {Status_Total::Charm, Status_Total::Null}, Status_Total::Magic_Power, 3, "외교관",          "모험 중 만난 수많은 이들과의\n 소통을 통해 화술을 갈고 닦은\n 싱클레어 n세.\n 그는 결국 왕국의 외교관으로\n 임명되었다.\n 그의 말은 마법같은 힘이 있었지만,\n 정작 마법은 쓸 수 없었다."},
            {2053, {Status_Total::Charm, Status_Total::Null}, Status_Total::Health, 3, "외교관",               "모험 중 만난 수많은 이들과의\n 소통을 통해 화술을 갈고 닦은\n 싱클레어 n세.\n 그는 결국 왕국의 외교관으로\n 임명되었다.\n 그러나 건강은 끝내 따라주지 않았고,\n 싱클레어 n세는 49세에\n 마지막 회담을 앞두고\n 생을 마감하였다."},
            {2054, {Status_Total::Charm, Status_Total::Null}, Status_Total::Knowledge, 3, "외교관",            "모험 중 만난 수많은 이들과의\n 소통을 통해 화술을 갈고 닦은\n 싱클레어 n세.\n 그는 결국 왕국의 외교관으로\n 임명되었다.\n 외교 중으로 알게 된 물품으로\n 사업을 시작했지만,\n 수익보다는 손해가 더 많았다."},
            {2055, {Status_Total::Charm, Status_Total::Null}, Status_Total::Null, 4, "외교관",                 "모험 중 만난 수많은 이들과의\n 소통을 통해 화술을 갈고 닦은\n 싱클레어 n세.\n 그는 결국 왕국의 외교관으로\n 임명되었다.\n 그가 남긴 화술서가\n 아직도 전해지고 있다."},
            {3011, {Status_Total::Null, Status_Total::Null}, Status_Total::Null, 7, "선택받은 자",             "싱클레어 n세는 모든 무기를 다뤘으며\n 모든 것에 통달한 존재가 되었다.\n 그는 모두의 귀감이 되었으며\n 가장 뛰어난 영웅 중\n 한명으로 칭송받고 있다."},
            {3021, {Status_Total::Null, Status_Total::Null}, Status_Total::Null, 2, "모험가",                  "마을 밖은 낯설고 위험했지만,\n 호기심이 컸던 싱클레어 n세는\n 모험을 멈추지 않았다.\n 이제는 소소한 취미가 된 모험.\n 덕분에 다양한 아이템들을\n 집으로 가져올 수 있게 되었다."},
            {3031, {Status_Total::Null, Status_Total::Null}, Status_Total::Null, 1, "평범한 가정",             "마을 밖은 생각보다 더 위험한\n 몬스터들이 존재했다.\n 싱클레어 n세는 모험가의 꿈을 접고\n 고향으로 돌아갔다.\n 남은 생 동안 가족들과 조용하고\n 평온한 삶을 살았다."}
     };
};

