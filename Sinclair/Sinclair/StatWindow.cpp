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
    if (!m_isActive) return;
    
    CalculateStats();
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

void StatWindow::CalculateStats()
{
    m_fundamentalStats = { 43, 51, 25, 41 };
    m_totalStats.Strength = 48;
    m_totalStats.Magic_Power = 22;
    m_totalStats.Health = 30;
    m_totalStats.Knowledge = 20;
    m_totalStats.Charm = 30;
    // 1. UIManager에서 UIWindow 포인터를 가져오기.
    UIWindow* window = UIManager::Get().GetWindow(UIWindowType::EquipmentWindow);
    // 2. 포인터 타입 맞는지 체크
    if (auto* equipmentWindow = dynamic_cast<EquipmentWindow*>(window))
    {
        // 모든 슬롯 타입을 반복하며 아이템을 가져옴
        for (int i = 0; i < static_cast<int>(Wearable_part::UnKnown); ++i)
        {
            // 장비창에서 현재 슬롯에 맞는 아이템 가져오기.
            Wearable_part slotType = static_cast<Wearable_part>(i);
            Item* equippedItem = equipmentWindow->GetEquippedItem(slotType);

            // 장비 있으면
            if (equippedItem != nullptr)
            {
                // item id로 찾아서 wearable로 dynamic_cast 후 스탯 가져와서 더해주기.
                unique_ptr<Item> item = ResourceManager::Get().Get_ItemBank().Get_Item_Status(equippedItem->m_data.id);
                Wearable* wearableItem = dynamic_cast<Wearable*>(item.get());
                m_fundamentalStats.power += wearableItem->GetStat(Status_fundamental::power);
                m_fundamentalStats.agile += wearableItem->GetStat(Status_fundamental::agile);
                m_fundamentalStats.intelligence += wearableItem->GetStat(Status_fundamental::intelligence);
                m_fundamentalStats.luck += wearableItem->GetStat(Status_fundamental::luck);

                // 1차 스탯 기반으로 2차스탯 계산
                m_totalStats.Strength = m_fundamentalStats.power * 2 + m_fundamentalStats.agile * 1;
                m_totalStats.Magic_Power = m_fundamentalStats.intelligence * 3;
                m_totalStats.Health = static_cast<int>(m_fundamentalStats.power * 1 + m_fundamentalStats.intelligence * 0.5f);
                m_totalStats.Knowledge = m_fundamentalStats.intelligence * 2 + m_fundamentalStats.luck * 1;
                m_totalStats.Charm = m_fundamentalStats.luck * 3 + m_fundamentalStats.agile * 1;

                // 2차 스탯을 0~100 사이로 정규화
                std::cout << m_totalStats.Strength << std::endl;
                std::cout << m_totalStats.Magic_Power << std::endl;
                std::cout << m_totalStats.Health << std::endl;
                std::cout << m_totalStats.Knowledge << std::endl;
                std::cout << m_totalStats.Charm << std::endl;
            }
        }
    }

}

void StatWindow::RenderBackground()
{
    UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();
    if (uiRenderer)
    {
        ID2D1Bitmap1* backgroundBitmap = ResourceManager::Get().Get_UIBank().Get_Image("StatBG").Get();

       // ID2D1Bitmap1* backgroundBitmap = uiRenderer->GetBitmap("StatBG").Get();
        if (backgroundBitmap)
        {
            D2D1_RECT_F destRect = { m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y };
            D2DRenderer::Get().DrawBitmap(backgroundBitmap, destRect);
        }
        else
        {
            D2DRenderer::Get().DrawRectangle(m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y, D2D1::ColorF(D2D1::ColorF::DarkGray, 0.8f));
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
    float rightMargin = 47.0f;
    Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 7 };
    Vec2 closeButtonSize = { 27.0f, 27.0f };
    D2D1_RECT_F destRect = { closeButtonPos.x, closeButtonPos.y, closeButtonPos.x + closeButtonSize.x, closeButtonPos.y + closeButtonSize.y };

    if (uiRenderer)
    {
        ID2D1Bitmap1* closeButtonBitmap = ResourceManager::Get().Get_UIBank().Get_Image("CloseButton").Get();


       // ID2D1Bitmap1* closeButtonBitmap = uiRenderer->GetBitmap("CloseButton").Get();
        if (closeButtonBitmap)
        {
            D2DRenderer::Get().DrawBitmap(closeButtonBitmap, destRect);
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

    // 레이더 차트 중심점
    Vec2 center = m_position + Vec2(m_size.x * 0.5f, m_size.y * 0.5f);
    float maxRadius = 100.0f;

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

    // 1. 외곽 기준선 그리기
    //for (int i = 0; i < 5; ++i)
    //{
    //    const Vec2& p1 = maxPoints[i];
    //    const Vec2& p2 = maxPoints[(i + 1) % 5];
    //    D2DRenderer::Get().DrawLine(p1.x, p1.y, p2.x, p2.y, D2D1::ColorF(D2D1::ColorF::Gray, 0.5f));

    //    m_statPoints[i] = maxPoints[i];
    //}

    //// 2. 중심점에서 각 최대 포인트로 선 그리기
    //for (int i = 0; i < 5; ++i)
    //{
    //    D2DRenderer::Get().DrawLine(center.x, center.y, maxPoints[i].x, maxPoints[i].y, D2D1::ColorF(D2D1::ColorF::LightGray, 0.3f));
    //}

    // 3. 채우기 효과 - 중심에서 각 스탯 포인트로 방사형 선 그리기
    for (int i = 0; i < 5; ++i)
    {
        int next = (i + 1) % 5;

        // 현재 변을 여러 선으로 분할
        for (float t = 0.0f; t <= 1.0f; t += 0.01f) // 0.01f = 선 밀도
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

    // 4. 실제 스탯값에 따른 5각형 외곽선 그리기
    for (int i = 0; i < 5; ++i)
    {
        const Vec2& p1 = points[i];
        const Vec2& p2 = points[(i + 1) % 5];
        D2DRenderer::Get().DrawLine(p1.x, p1.y, p2.x, p2.y, D2D1::ColorF(D2D1::ColorF::SaddleBrown, 1.0f));
    }
}

// 0 ~ 100 사이값으로 스탯 정규화해주기
float StatWindow::NormalizeStat(float statValue, float min, float max)
{
    if (max <= min) return 0.0f;
    float normalized = (statValue - min) / (max - min) * 100.0f;
    return std::max(0.0f, std::min(100.0f, normalized));
}