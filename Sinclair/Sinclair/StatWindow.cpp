#include "StatWindow.h"
#include <cmath>

// 매크로 쓸려고
#include <DirectXMath.h>
using namespace DirectX;

// min max 매크로 없앨라면 따로 선언해줘야함
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif


void StatWindow::Update()
{

}

void StatWindow::Render()
{
    if (!m_isActive) return;

    RenderBackground();     // 배경 렌더
    RenderRadarChart();     // 레이더 차트 렌더
    //RenderStatsText();      // 스탯 텍스트 렌더
    //RenderTitleBar();       // 타이틀 바 렌더
    RenderCloseButton();    // 닫기 버튼 렌더
}

bool StatWindow::HandleMouseDown(Vec2 mousePos)
{
    if (!m_isActive) return false;

    // 창 내부 클릭 이벤트 처리 완료. 그래서 화면 최상단으로 올리기.
    if (IsInBounds(mousePos))
    {
        UIManager::Get().OpenWindow(m_windowType);
        return true;
    }
    // 영역 밖이면 체크 안하기
    return false;
}

bool StatWindow::HandleMouseUp(Vec2 mousePos)
{
    std::cout << "바운드 체크 " << std::endl;
    // 창 내부 클릭 이벤트 처리 완료. 그래서 화면 최상단으로 올리기.
    if (IsInBounds(mousePos))
    {
        // 창 영역 내에서 드래그된 아이템이 있으면 인벤토리로 반환
        if (CursorManager::Get().IsDragging())
        {
            Item* draggedItem = CursorManager::Get().GetDraggedItem();
            if (draggedItem)
            {
                DragSource source = CursorManager::Get().GetDragSource();
                HandleDropFailure(mousePos, draggedItem, source);
            }
        }

        UIManager::Get().OpenWindow(m_windowType);
        return true;
    }
    // 영역 밖이면 체크 안하기
    return false;
}

bool StatWindow::HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source)
{
    if (!draggedItem) return false;

    // 다른 창들의 영역인지 확인
    bool isInOtherWindow = false;

    // 인벤창 영역 확인
    UIWindow* inventoryWindow = UIManager::Get().GetWindow(UIWindowType::InventoryWindow);
    if (inventoryWindow)
    {
        DragSource source = CursorManager::Get().GetDragSource();
        Inventory* inven = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow));
        inven->AddItem(draggedItem->m_data.id, 1);
        CursorManager::Get().EndItemDrag();
        return true;
    }

    return false;
}
bool StatWindow::HandleMouseRight(Vec2 mousePos)
{
    return false;
}

//void StatWindow::CalculateStats()
//{
//    m_playerBaseFundamentalStats = { 0,0,0,0 };
//    //m_totalStats.Strength = 100;
//    //m_totalStats.Magic_Power = 70;
//    //m_totalStats.Health = 100;
//    //m_totalStats.Knowledge = 100;
//    //m_totalStats.Charm = 100;
//    // 1. UIManager에서 UIWindow 포인터를 가져오기.
//    UIWindow* window = UIManager::Get().GetWindow(UIWindowType::EquipmentWindow);
//    // 2. 포인터 타입 맞는지 체크
//    if (auto* equipmentWindow = dynamic_cast<EquipmentWindow*>(window))
//    {
//        // 모든 슬롯 타입을 반복하며 아이템을 가져옴
//        for (int i = 0; i < static_cast<int>(Wearable_part::UnKnown); ++i)
//        {
//            // 장비창에서 현재 슬롯에 맞는 아이템 가져오기.
//            Wearable_part slotType = static_cast<Wearable_part>(i);
//            Item* equippedItem = equipmentWindow->GetEquippedItem(slotType);
//
//            // 장비 있으면
//            if (equippedItem != nullptr)
//            {
//                // 아이템 데이터 가져오기
//                unique_ptr<Item> item = ResourceManager::Get().Get_ItemBank().Get_Item_Status(equippedItem->m_data.id);
//                if (item != nullptr) // unique_ptr 체크
//                {
//                    Wearable* wearableItem = dynamic_cast<Wearable*>(item.get());
//                    if (wearableItem != nullptr) // dynamic_cast 결과 체크
//                    {
//                        // 1차 스탯 누적
//                        m_playerBaseFundamentalStats.power += wearableItem->GetStat(Status_fundamental::power);
//                        m_playerBaseFundamentalStats.agile += wearableItem->GetStat(Status_fundamental::agile);
//                        m_playerBaseFundamentalStats.intelligence += wearableItem->GetStat(Status_fundamental::intelligence);
//                        m_playerBaseFundamentalStats.luck += wearableItem->GetStat(Status_fundamental::luck);
//                    }
//                }
//            }
//        }
//    }
//
//    m_totalStats.Strength = m_playerBaseFundamentalStats.power * 2 + m_playerBaseFundamentalStats.agile * 1;
//    m_totalStats.Magic_Power = m_playerBaseFundamentalStats.intelligence * 3;
//    m_totalStats.Health = static_cast<int>(m_playerBaseFundamentalStats.power * 1 + m_playerBaseFundamentalStats.intelligence * 0.5f);
//    m_totalStats.Knowledge = m_playerBaseFundamentalStats.intelligence * 2 + m_playerBaseFundamentalStats.luck * 1;
//    m_totalStats.Charm = m_playerBaseFundamentalStats.luck * 3 + m_playerBaseFundamentalStats.agile * 1;
//
//    // 디버그 출력
//    std::cout << "Final Stats - Strength: " << m_totalStats.Strength
//        << ", Magic Power: " << m_totalStats.Magic_Power
//        << ", Health: " << m_totalStats.Health
//        << ", Knowledge: " << m_totalStats.Knowledge
//        << ", Charm: " << m_totalStats.Charm << std::endl;
//
//}

void StatWindow::RenderBackground()
{
    UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();
    if (uiRenderer)
    {
        if (m_backgroundBitmap)
        {
            D2D1_RECT_F destRect = { m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y };
            D2DRenderer::Get().DrawBitmap(m_backgroundBitmap.Get(), destRect);
        }
    }
}

void StatWindow::RenderTitleBar()
{
    UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();
    if (!uiRenderer) return;

    Vec2 titleBarSize = { m_size.x, 42 };

    if (uiRenderer)
    {
        ID2D1Bitmap1* titleBarBitmap = uiRenderer->GetBitmap("창고_1").Get();
        D2D1_RECT_F destRect = { m_position.x, m_position.y, m_position.x + titleBarSize.x, m_position.y + titleBarSize.y };
        if (titleBarBitmap)
        {
            D2DRenderer::Get().DrawBitmap(titleBarBitmap, destRect);
        }
        else
        {
            D2DRenderer::Get().DrawRectangle(m_position.x, m_position.y, m_position.x + titleBarSize.x, m_position.y + titleBarSize.y, D2D1::ColorF(D2D1::ColorF::SteelBlue, 1.0f));
        }
    }

}

void StatWindow::RenderCloseButton()
{
    UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();
    float rightMargin = 65;
    Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 35 };
    Vec2 closeButtonSize = { 35, 35 };
    D2D1_RECT_F destRect = { closeButtonPos.x, closeButtonPos.y, closeButtonPos.x + closeButtonSize.x, closeButtonPos.y + closeButtonSize.y };

    if (uiRenderer)
    {
        if (m_closeButton)
        {
            D2DRenderer::Get().DrawBitmap(m_closeButton.Get(), destRect);
        }
    }
}

void StatWindow::RenderStatsText()
{
    UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();
    if (!uiRenderer) return;

    // 레이더 차트 중심점과 최대 반경 
    float titleBarHeight = 42.0f;
    Vec2 center = m_position + Vec2(m_size.x * 0.4f, titleBarHeight + (m_size.y - titleBarHeight) * 0.435f);
    float maxRadius = 180.0f; // 레이더 차트보다 약간 더 큰 반경으로 설정

    // 각도 초기화 (RenderRadarChart와 동일)
    float angle = -XM_PIDIV2;
    float angleIncrement = 2.0f * XM_PI / 5.0f;

    // 각 스탯에 대한 정보와 위치를 설정
    for (int i = 0; i < 5; ++i)
    {
        std::wstring statName;
        int statValue = 0;

        switch (i)
        {
        case 0: statName = L"Strength "; statValue = m_totalStats.Strength; break;
        case 1: statName = L"Magic Power "; statValue = m_totalStats.Magic_Power; break;
        case 2: statName = L"Health "; statValue = m_totalStats.Health; break;
        case 3: statName = L"Knowledge "; statValue = m_totalStats.Knowledge; break;
        case 4: statName = L"Charm "; statValue = m_totalStats.Charm; break;
        }

        const Vec2& basePoint = m_statPoints[i];

        float textX = basePoint.x;
        float textY = basePoint.y;

        // **여기서 패딩 값을 개별적으로 조정**
        switch (i)
        {
        case 0: // Strength (위)
            textX -= 35.0f; // 왼쪽으로 패딩
            textY -= 30.0f; // 위로 패딩
            break;
        case 1: // Magic Power (오른쪽 위)
            textX += 20.0f; // 오른쪽 패딩
            textY -= 15.0f; // 살짝 위로 패딩
            break;
        case 2: // Health (오른쪽 아래)
            textX += 15.0f; // 오른쪽 패딩
            textY += 10.0f; // 아래로 패딩
            break;
        case 3: // Knowledge (왼쪽 아래)
            textX -= 120.0f; // 왼쪽 패딩
            textY += 10.0f; // 아래로 패딩
            break;
        case 4: // Charm (왼쪽 위)
            textX -= 90.0f; // 왼쪽 패딩
            textY -= 15.0f; // 살짝 위로 패딩
            break;
        }

        // 최종 텍스트값
        std::wstring statsText = statName + L": " + std::to_wstring(statValue);

        // 텍스트 그리기.
        D2DRenderer::Get().DrawMessage(statsText.c_str(), textX, textY, 150.0f, 20.0f, D2D1::ColorF(D2D1::ColorF::Black));

        // 다음 각도
        angle += angleIncrement;
    }
}

void StatWindow::RenderRadarChart()
{
    UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();
    if (!uiRenderer) return;


    if (m_statBase)
    {
        float newWidth = m_size.x / 1.4f;
        float newHeight = m_size.y / 2.0f;

        D2D1_RECT_F destRect = {
            m_position.x + (m_size.x - newWidth) / 2.0f,
            m_position.y + (m_size.y - newHeight) / 2.0f,
            m_position.x + (m_size.x - newWidth) / 2.0f + newWidth,
            m_position.y + (m_size.y - newHeight) / 2.0f + newHeight };
        D2DRenderer::Get().DrawBitmap(m_statBase.Get(), destRect);
    }


    // 레이더 차트 중심점
    float yOffset = 20.0f; // 오프셋으로 아래로 그리게
    Vec2 center = { m_position.x + m_size.x / 2.0f, m_position.y + m_size.y / 2.0f + yOffset };
    float maxRadius = 205.0f;

    float stats[5] =
    {
        m_totalStats.Strength,
        m_totalStats.Magic_Power,
        m_totalStats.Health,
        m_totalStats.Knowledge,
        m_totalStats.Charm
    };

    // 5개의 스탯 포인트를 계산
    Vec2 points[5];
    Vec2 maxPoints[5];

    float angleStep = 2.0f * XM_PI / 5.0f;
    float startAngle = -XM_PIDIV2;

    // 각 포인트 계산
    for (int i = 0; i < 5; ++i)
    {
        float theta = startAngle + angleStep * i;

        // 최대값 기준 포인트
        float maxX = center.x + cos(theta) * maxRadius;
        float maxY = center.y + sin(theta) * maxRadius;
        maxPoints[i] = Vec2(maxX, maxY);

        // 실제 스탯값에 따른 포인트
        float normalized = std::clamp(stats[i] / 100.0f, 0.0f, 1.0f);
        float actualX = center.x + cos(theta) * maxRadius * normalized;
        float actualY = center.y + sin(theta) * maxRadius * normalized;
        points[i] = Vec2(actualX, actualY);
    }


    // 채우기 효과 - 중심에서 각 스탯 포인트로 방사형 선 그리기
    for (int i = 0; i < 5; ++i)
    {
        int next = (i + 1) % 5;

        // 현재 변을 여러 선으로 분할
        for (float t = 0.0f; t <= 1.0f; t += 0.003f) // 0.01f = 선 밀도
        {
            float x = points[i].x + (points[next].x - points[i].x) * t;
            float y = points[i].y + (points[next].y - points[i].y) * t;

            // 중심에서 변의 각 점으로 선 그리기
            D2DRenderer::Get().DrawLine(
                center.x, center.y,
                x, y,
                D2D1::ColorF(D2D1::ColorF::SaddleBrown, 1.0f) // 반투명 원하면 0.1~0.5정도로 줄이기
            );
        }
    }

    // 실제 스탯값에 따른 5각형 외곽선 그리기 | 필요없으면 나중에 지울거임.
    //for (int i = 0; i < 5; ++i)
    //{
    //    const Vec2& p1 = points[i];
    //    const Vec2& p2 = points[(i + 1) % 5];
    //    D2DRenderer::Get().DrawLine(p1.x, p1.y, p2.x, p2.y, D2D1::ColorF(D2D1::ColorF::SaddleBrown, 1.0f));
    //}
}

// 0 ~ 100 사이값으로 스탯 정규화해주기
float StatWindow::NormalizeStat(float statValue, float min, float max)
{
    if (max <= min) return 0.0f;
    float normalized = (statValue - min) / (max - min) * 100.0f;
    return std::max(0.0f, std::min(100.0f, normalized));
}

void StatWindow::StatUpdate(Status_fundamental stat, int Much)
{
    switch (stat)
    {
    case Status_fundamental::power:
        m_potionFundamentalStats.power += Much;
        UpdateTotalStats();
        break;
    case Status_fundamental::agile:
        m_potionFundamentalStats.agile += Much;
        UpdateTotalStats();
        break;
    case Status_fundamental::intelligence:
        m_potionFundamentalStats.intelligence += Much;
        UpdateTotalStats();
        break;
    case Status_fundamental::luck:
        m_potionFundamentalStats.luck += Much;
        UpdateTotalStats();
        break;
    default:
        break;
    }
}

void StatWindow::UpdateTotalStats()
{
    // 1. 장비 스탯 초기화
    m_equipmentFundamentalStats = {};

    // 2. 장비 스탯 합산
    UIWindow* window = UIManager::Get().GetWindow(UIWindowType::EquipmentWindow);
    if (auto* equipmentWindow = dynamic_cast<EquipmentWindow*>(window))
    {
        for (int i = 0; i < static_cast<int>(Wearable_part::UnKnown); ++i)
        {
            if (auto* wearableItem = dynamic_cast<Wearable*>(equipmentWindow->GetEquippedItem((Wearable_part)i)))
            {
                m_equipmentFundamentalStats.power += wearableItem->GetStat(Status_fundamental::power);
                m_equipmentFundamentalStats.agile += wearableItem->GetStat(Status_fundamental::agile);
                m_equipmentFundamentalStats.intelligence += wearableItem->GetStat(Status_fundamental::intelligence);
                m_equipmentFundamentalStats.luck += wearableItem->GetStat(Status_fundamental::luck);
            }
        }
    }

    // 3. 장비 + 포션 효과 합산
    fundamentalStatus totalFundamentalStats;
    totalFundamentalStats.power = m_equipmentFundamentalStats.power + m_potionFundamentalStats.power;
    totalFundamentalStats.agile = m_equipmentFundamentalStats.agile + m_potionFundamentalStats.agile;
    totalFundamentalStats.intelligence = m_equipmentFundamentalStats.intelligence + m_potionFundamentalStats.intelligence;
    totalFundamentalStats.luck = m_equipmentFundamentalStats.luck + m_potionFundamentalStats.luck;

    // 4. 2차 스탯 계산
    m_totalStats.Strength = totalFundamentalStats.power * 2 + totalFundamentalStats.agile * 1;
    m_totalStats.Magic_Power = totalFundamentalStats.intelligence * 3;
    m_totalStats.Health = static_cast<int>(totalFundamentalStats.power * 1 + totalFundamentalStats.intelligence * 0.5f);
    m_totalStats.Knowledge = totalFundamentalStats.intelligence * 2 + totalFundamentalStats.luck * 1;
    m_totalStats.Charm = totalFundamentalStats.luck * 3 + totalFundamentalStats.agile * 1;
}

// 무기 고유 id로 체크할거임.
void StatWindow::CalculateBonusStats(Item* item)
{
    if (item->m_data.id == "W006")
    {
        m_totalStats.Strength *= 1.2f;
    }
    else if (item->m_data.id == "W007")
    {
        m_totalStats.Health *= 1.2f;
    }
    else if (item->m_data.id == "W008")
    {
        m_totalStats.Magic_Power *= 1.2f;
    }
    else if (item->m_data.id == "W009")
    {
        m_totalStats.Knowledge *= 1.2f;
    }
    else if (item->m_data.id == "W010")
    {
        m_totalStats.Charm *= 1.2f;
    }
    else if (item->m_data.id == "W011")
    {
        m_totalStats.Strength *= 2;
        m_totalStats.Health *= 2;
        m_totalStats.Magic_Power *= 2;
        m_totalStats.Knowledge *= 2;
        m_totalStats.Charm *= 2;
    }
    else if (item->m_data.id == "W012")
    {
        m_totalStats.Strength *= 2;
        m_totalStats.Health *= 2;
        m_totalStats.Magic_Power *= 2;
        m_totalStats.Knowledge *= 2;
        m_totalStats.Charm *= 2;
    }
    else if (item->m_data.id == "W013")
    {
        m_totalStats.Strength *= 2;
        m_totalStats.Health *= 2;
        m_totalStats.Magic_Power *= 2;
        m_totalStats.Knowledge *= 2;
        m_totalStats.Charm *= 2;
    }
    else if (item->m_data.id == "W014")
    {
        m_totalStats.Strength *= 2;
        m_totalStats.Health *= 2;
        m_totalStats.Magic_Power *= 2;
        m_totalStats.Knowledge *= 2;
        m_totalStats.Charm *= 2;
    }
    else if (item->m_data.id == "W015")
    {
        m_totalStats.Strength *= 2;
        m_totalStats.Health *= 2;
        m_totalStats.Magic_Power *= 2;
        m_totalStats.Knowledge *= 2;
        m_totalStats.Charm *= 2;
    }
}

void StatWindow::ResetStat()
{
    m_equipmentFundamentalStats; {}
    m_potionFundamentalStats; {}
    m_totalStats = {};

}

