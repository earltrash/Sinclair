#include "UIPotion.h"
#include "UI_Bank.h"
#include "UI_Renderer.h"
#include "ResourceManager.h"
#include "GameManager.h"


UIPotion::UIPotion()
    : UIWindow(UIWindowType::StatPotionUseWindow, Vec2{ 0,0 }, Vec2{ 524,766 })
{
    InitializeMem();   // ��ư offset �ʱ�ȭ
    LoadUIBitmaps();   // ��ư, ���, �ݱ� ��ư ��Ʈ�� �ε�
    m_PotionLevel = PotionLevel::level3; // �⺻ ���� ����
}

void UIPotion::Update()
{
    // Ư���� ������Ʈ ���� ����
}

void UIPotion::Render()
{
    UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();
    if (!uiRenderer) return;

    // ���
    ComPtr<ID2D1Bitmap1> backgroundBitmap = m_BG_BITMAP[static_cast<int>(GetLevel())];
    if (backgroundBitmap)
    {
        D2D1_RECT_F destRect = {
            m_position.x, m_position.y,
            m_position.x + m_size.x, m_position.y + m_size.y
        };
        D2DRenderer::Get().DrawBitmap(backgroundBitmap.Get(), destRect);
    }

    // �ݱ� ��ư
    float rightMargin = 66.0f;
    Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 36 };
    Vec2 closeButtonSize = { 35.0f, 35.0f };
    D2D1_RECT_F closeRect = {
        closeButtonPos.x, closeButtonPos.y,
        closeButtonPos.x + closeButtonSize.x, closeButtonPos.y + closeButtonSize.y
    };
    ID2D1Bitmap1* closeButtonBitmap = uiRenderer->GetBitmap("Close_Button").Get();
    if (closeButtonBitmap)
    {
        D2DRenderer::Get().DrawBitmap(closeButtonBitmap, closeRect);
    }

    // ��ư ������
    auto drawButton = [&](int index, ID2D1Bitmap1* bitmap, float opacity)
        {
            if (!bitmap) return;
            D2D1_SIZE_F size = bitmap->GetSize();
            D2D1_RECT_F srect = { 0, 0, size.width, size.height };

            // â ��ġ + offset �� ���� ��ǥ ���
            D2D1_RECT_F buttonRect = {
                m_position.x + m_button[index].x,
                m_position.y + m_button[index].y,
                m_position.x + m_button[index].x + ButtonX,
                m_position.y + m_button[index].y + ButtonY
            };
            D2DRenderer::Get().DrawBitmap(bitmap, buttonRect, srect, opacity);
        };

    drawButton(0, m_str_bitmap[static_cast<int>(GetLevel())].Get(), m_But_opacity[0]);
    drawButton(1, m_dex_bitmap[static_cast<int>(GetLevel())].Get(), m_But_opacity[1]);
    drawButton(2, m_int_bitmap[static_cast<int>(GetLevel())].Get(), m_But_opacity[2]);
    drawButton(3, m_luk_bitmap[static_cast<int>(GetLevel())].Get(), m_But_opacity[3]);
}

bool UIPotion::HandleMouseHover(Vec2 mousePos)
{
    if (!m_isActive) return false;

    bool hovered = false;
    for (int i = 0; i < m_button.size(); i++)
    {
        if (InButton(m_button[i], mousePos))
        {
            m_But_opacity[i] = 0.5f;
            hovered = true;
        }
        else
        {
            m_But_opacity[i] = 1.f;
        }
    }
    return hovered;
}

bool UIPotion::HandleMouseDown(Vec2 mousePos)
{
    if (!m_isActive) return false;

    for (int i = 0; i < m_button.size(); i++)
    {
        if (InButton(m_button[i], mousePos))
        {
            m_Stat = static_cast<Status_fundamental>(i);
            int Much = static_cast<int>(GetLevel()) + 1;

            //����â�� ���� ��ġ�� �ø��� ������ ���� ��. -> �̴� �Ŀ� ���������� gm����
            GameManager::Get().PotionUsed(m_Stat, Much); 

            if (auto* inv = dynamic_cast<Inventory*>(UIManager::Get().GetWindow(UIWindowType::InventoryWindow)))
            {
                if (inv->ConsumePendingPotion())
                    UIManager::Get().CloseWindow(UIWindowType::StatPotionUseWindow);              
            }
            
            return true;
        }
    }
    return false;
}

bool UIPotion::HandleMouseUp(Vec2 mousePos)
{
    if (!m_isActive) return false;

    // ����â ���� �� Ŭ���̸� â�� �ֻ������
    if (IsInBounds(mousePos))
    {
        // �巡�� ���� �������� �־ ����â������ ���� ����
        if (CursorManager::Get().IsDragging())
        {
            return false; // UIManager�� �ٸ� â���� ó���ϵ���
        }

        UIManager::Get().OpenWindow(m_windowType);
        return true;
    }

    return false;
}
bool UIPotion::HandleDoubleClick(Vec2) { return false; }
bool UIPotion::HandleMouseRight(Vec2) { return false; }

void UIPotion::LoadUIBitmaps()
{
    m_BG_BITMAP.resize(static_cast<int>(PotionLevel::nothing));
    m_str_bitmap.resize(static_cast<int>(PotionLevel::nothing));
    m_dex_bitmap.resize(static_cast<int>(PotionLevel::nothing));
    m_int_bitmap.resize(static_cast<int>(PotionLevel::nothing));
    m_luk_bitmap.resize(static_cast<int>(PotionLevel::nothing));

    auto* uiRenderer = AddComponent<UI_Renderer>();
    uiRenderer->SetBitmap("Close_Button", ResourceManager::Get().GetTexture("CloseButton"));

    m_BG_BITMAP[static_cast<int>(PotionLevel::level1)] = ResourceManager::Get().GetTexture("Potion0");
    m_BG_BITMAP[static_cast<int>(PotionLevel::level2)] = ResourceManager::Get().GetTexture("Potion1");
    m_BG_BITMAP[static_cast<int>(PotionLevel::level3)] = ResourceManager::Get().GetTexture("Potion2");

    m_str_bitmap[static_cast<int>(PotionLevel::level1)] = ResourceManager::Get().GetTexture("P0BS");
    m_dex_bitmap[static_cast<int>(PotionLevel::level1)] = ResourceManager::Get().GetTexture("P0BD");
    m_int_bitmap[static_cast<int>(PotionLevel::level1)] = ResourceManager::Get().GetTexture("P0BI");
    m_luk_bitmap[static_cast<int>(PotionLevel::level1)] = ResourceManager::Get().GetTexture("P0BL");

    m_str_bitmap[static_cast<int>(PotionLevel::level2)] = ResourceManager::Get().GetTexture("P1BS");
    m_dex_bitmap[static_cast<int>(PotionLevel::level2)] = ResourceManager::Get().GetTexture("P1BD");
    m_int_bitmap[static_cast<int>(PotionLevel::level2)] = ResourceManager::Get().GetTexture("P1BI");
    m_luk_bitmap[static_cast<int>(PotionLevel::level2)] = ResourceManager::Get().GetTexture("P1BL");

    m_str_bitmap[static_cast<int>(PotionLevel::level3)] = ResourceManager::Get().GetTexture("P2BS");
    m_dex_bitmap[static_cast<int>(PotionLevel::level3)] = ResourceManager::Get().GetTexture("P2BD");
    m_int_bitmap[static_cast<int>(PotionLevel::level3)] = ResourceManager::Get().GetTexture("P2BI");
    m_luk_bitmap[static_cast<int>(PotionLevel::level3)] = ResourceManager::Get().GetTexture("P2BL");
}

void UIPotion::InitializeMem()
{
    m_button.resize(static_cast<int>(PotionStat::nothing));
    m_But_opacity.assign(static_cast<int>(PotionStat::nothing), 1);

    // â (0,0) ���� offset
    m_button[static_cast<int>(PotionStat::STR)] = { 51, 350 };
    m_button[static_cast<int>(PotionStat::DEX)] = { 51, 433 };
    m_button[static_cast<int>(PotionStat::INT)] = { 51, 517 };
    m_button[static_cast<int>(PotionStat::LUK)] = { 51, 600 };
}

UIWindowType UIPotion::GetType()
{
    return m_windowType;
}

bool UIPotion::HandleDropFailure(Vec2, Item*, DragSource) { return false; }

bool UIPotion::InButton(Vec2 offset, Vec2 mousePos)
{
    // â ��ġ + offset �� ���� ��ǥ
    Vec2 absPos = { m_position.x + offset.x, m_position.y + offset.y };
    return mousePos.x >= absPos.x && mousePos.x <= absPos.x + ButtonX &&
        mousePos.y >= absPos.y && mousePos.y <= absPos.y + ButtonY;
}

void UIPotion::SetLevel(PotionLevel level)
{
    m_PotionLevel = level;
}

PotionLevel UIPotion::GetLevel()
{
    return m_PotionLevel;
}
