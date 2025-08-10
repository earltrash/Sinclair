#include "StatWindow.h"
#include <cmath>

// ��ũ�� ������
#include <DirectXMath.h>
using namespace DirectX;

// min max ��ũ�� ���ٶ�� ���� �����������
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

    RenderBackground();     // ��� ����
    RenderRadarChart();     // ���̴� ��Ʈ ����
    //RenderStatsText();      // ���� �ؽ�Ʈ ����
    //RenderTitleBar();       // Ÿ��Ʋ �� ����
    RenderCloseButton();    // �ݱ� ��ư ����
}

bool StatWindow::HandleMouseDown(Vec2 mousePos)
{
    if (!m_isActive) return false;

    // â ���� Ŭ�� �̺�Ʈ ó�� �Ϸ�. �׷��� ȭ�� �ֻ������ �ø���.
    if (IsInBounds(mousePos))
    {
        UIManager::Get().OpenWindow(m_windowType);
        return true;
    }
    // ���� ���̸� üũ ���ϱ�
    return false;
}

bool StatWindow::HandleMouseUp(Vec2 mousePos)
{
    // â ���� Ŭ�� �̺�Ʈ ó�� �Ϸ�. �׷��� ȭ�� �ֻ������ �ø���.
    if (IsInBounds(mousePos))
    {
        UIManager::Get().OpenWindow(m_windowType);
        return true;
    }
    // ���� ���̸� üũ ���ϱ�
    return false;
}

bool StatWindow::HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source)
{
    return false;
}

bool StatWindow::HandleMouseRight(Vec2 mousePos)
{
    return false;
}

void StatWindow::CalculateStats()
{
    m_fundamentalStats = { 43, 51, 25, 41 };
    m_totalStats.Strength = 100;
    m_totalStats.Magic_Power = 70;
    m_totalStats.Health = 100;
    m_totalStats.Knowledge = 100;
    m_totalStats.Charm = 100;
    // 1. UIManager���� UIWindow �����͸� ��������.
    UIWindow* window = UIManager::Get().GetWindow(UIWindowType::EquipmentWindow);
    // 2. ������ Ÿ�� �´��� üũ
    if (auto* equipmentWindow = dynamic_cast<EquipmentWindow*>(window))
    {
        // ��� ���� Ÿ���� �ݺ��ϸ� �������� ������
        for (int i = 0; i < static_cast<int>(Wearable_part::UnKnown); ++i)
        {
            // ���â���� ���� ���Կ� �´� ������ ��������.
            Wearable_part slotType = static_cast<Wearable_part>(i);
            Item* equippedItem = equipmentWindow->GetEquippedItem(slotType);

            // ��� ������
            if (equippedItem != nullptr)
            {
                // item id�� ã�Ƽ� wearable�� dynamic_cast �� ���� �����ͼ� �����ֱ�.
                unique_ptr<Item> item = ResourceManager::Get().Get_ItemBank().Get_Item_Status(equippedItem->m_data.id);
                Wearable* wearableItem = dynamic_cast<Wearable*>(item.get());
                m_fundamentalStats.power += wearableItem->GetStat(Status_fundamental::power);
                m_fundamentalStats.agile += wearableItem->GetStat(Status_fundamental::agile);
                m_fundamentalStats.intelligence += wearableItem->GetStat(Status_fundamental::intelligence);
                m_fundamentalStats.luck += wearableItem->GetStat(Status_fundamental::luck);

                // 1�� ���� ������� 2������ ���
                m_totalStats.Strength = m_fundamentalStats.power * 2 + m_fundamentalStats.agile * 1;
                m_totalStats.Magic_Power = m_fundamentalStats.intelligence * 3;
                m_totalStats.Health = static_cast<int>(m_fundamentalStats.power * 1 + m_fundamentalStats.intelligence * 0.5f);
                m_totalStats.Knowledge = m_fundamentalStats.intelligence * 2 + m_fundamentalStats.luck * 1;
                m_totalStats.Charm = m_fundamentalStats.luck * 3 + m_fundamentalStats.agile * 1;

                // 2�� ������ 0~100 ���̷� ����ȭ
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
        ID2D1Bitmap1* titleBarBitmap = uiRenderer->GetBitmap("â��_1").Get();
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

    // ���̴� ��Ʈ �߽����� �ִ� �ݰ� 
    float titleBarHeight = 42.0f;
    Vec2 center = m_position + Vec2(m_size.x * 0.4f, titleBarHeight + (m_size.y - titleBarHeight) * 0.435f);   
    float maxRadius = 180.0f; // ���̴� ��Ʈ���� �ణ �� ū �ݰ����� ����

    // ���� �ʱ�ȭ (RenderRadarChart�� ����)
    float angle = -XM_PIDIV2;
    float angleIncrement = 2.0f * XM_PI / 5.0f;

    // �� ���ȿ� ���� ������ ��ġ�� ����
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

        // **���⼭ �е� ���� ���������� ����**
        switch (i)
        {
        case 0: // Strength (��)
            textX -= 35.0f; // �������� �е�
            textY -= 30.0f; // ���� �е�
            break;
        case 1: // Magic Power (������ ��)
            textX += 20.0f; // ������ �е�
            textY -= 15.0f; // ��¦ ���� �е�
            break;
        case 2: // Health (������ �Ʒ�)
            textX += 15.0f; // ������ �е�
            textY += 10.0f; // �Ʒ��� �е�
            break;
        case 3: // Knowledge (���� �Ʒ�)
            textX -= 120.0f; // ���� �е�
            textY += 10.0f; // �Ʒ��� �е�
            break;
        case 4: // Charm (���� ��)
            textX -= 90.0f; // ���� �е�
            textY -= 15.0f; // ��¦ ���� �е�
            break;
        }

        // ���� �ؽ�Ʈ��
        std::wstring statsText = statName + L": " + std::to_wstring(statValue);

        // �ؽ�Ʈ �׸���.
        D2DRenderer::Get().DrawMessage(statsText.c_str(), textX, textY, 150.0f, 20.0f, D2D1::ColorF(D2D1::ColorF::Black));

        // ���� ����
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


    // ���̴� ��Ʈ �߽���
    float yOffset = 20.0f; // ���������� �Ʒ��� �׸���
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

    // 5���� ���� ����Ʈ�� ���
    Vec2 points[5];
    Vec2 maxPoints[5];

    float angleStep = 2.0f * XM_PI / 5.0f;
    float startAngle = -XM_PIDIV2;

    // �� ����Ʈ ���
    for (int i = 0; i < 5; ++i)
    {
        float theta = startAngle + angleStep * i;

        // �ִ밪 ���� ����Ʈ
        float maxX = center.x + cos(theta) * maxRadius;
        float maxY = center.y + sin(theta) * maxRadius;
        maxPoints[i] = Vec2(maxX, maxY);

        // ���� ���Ȱ��� ���� ����Ʈ
        float normalized = std::clamp(stats[i] / 100.0f, 0.0f, 1.0f);
        float actualX = center.x + cos(theta) * maxRadius * normalized;
        float actualY = center.y + sin(theta) * maxRadius * normalized;
        points[i] = Vec2(actualX, actualY);
    }


    // ä��� ȿ�� - �߽ɿ��� �� ���� ����Ʈ�� ����� �� �׸���
    for (int i = 0; i < 5; ++i)
    {
        int next = (i + 1) % 5;

        // ���� ���� ���� ������ ����
        for (float t = 0.0f; t <= 1.0f; t += 0.003f) // 0.01f = �� �е�
        {
            float x = points[i].x + (points[next].x - points[i].x) * t;
            float y = points[i].y + (points[next].y - points[i].y) * t;

            // �߽ɿ��� ���� �� ������ �� �׸���
            D2DRenderer::Get().DrawLine(
                center.x, center.y,
                x, y,
                D2D1::ColorF(D2D1::ColorF::SaddleBrown, 1.0f) // ������ ���ϸ� 0.1~0.5������ ���̱�
            );
        }
    }

    // ���� ���Ȱ��� ���� 5���� �ܰ��� �׸��� | �ʿ������ ���߿� �������.
    //for (int i = 0; i < 5; ++i)
    //{
    //    const Vec2& p1 = points[i];
    //    const Vec2& p2 = points[(i + 1) % 5];
    //    D2DRenderer::Get().DrawLine(p1.x, p1.y, p2.x, p2.y, D2D1::ColorF(D2D1::ColorF::SaddleBrown, 1.0f));
    //}
}

// 0 ~ 100 ���̰����� ���� ����ȭ���ֱ�
float StatWindow::NormalizeStat(float statValue, float min, float max)
{
    if (max <= min) return 0.0f;
    float normalized = (statValue - min) / (max - min) * 100.0f;
    return std::max(0.0f, std::min(100.0f, normalized));
}