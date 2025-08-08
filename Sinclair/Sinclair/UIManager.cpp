#include "UIManager.h"

void UIManager::Initialize()
{


    AddComponent<MouseListenerComponent>
        (
            // ���⼭ ��� �Է� ó�� ����?
            [this](const MSG& msg) {
                OnInput(msg); });
    try
    {
        // SettingsWindow
        //m_allWindows.emplace(UIWindowType::SettingsWindow, std::make_unique<SettingsWindow>());
        //if (auto* window = GetWindow(UIWindowType::SettingsWindow))
        //{
        //    window->SetActivate(false);
        //}
        // InventoryWindow
        m_allWindows.emplace(UIWindowType::InventoryWindow, std::make_unique<Inventory>());
        if (auto* window = GetWindow(UIWindowType::InventoryWindow))
        {
            window->SetActivate(true);
        }
        m_activeWindowOrder.push_back(UIWindowType::InventoryWindow);
        // InventoryTooltip
        //m_allWindows.emplace(UIWindowType::InventoryTooltip, std::make_unique<InventoryTooltip>());
        //if (auto* window = GetWindow(UIWindowType::InventoryTooltip))
        //{
        //    window->SetActivate(false);
        //}

        // EquipmentWindow
        m_allWindows.emplace(UIWindowType::EquipmentWindow, std::make_unique<EquipmentWindow>());

        if (auto* window = GetWindow(UIWindowType::EquipmentWindow))
        {
            window->SetActivate(true);
        }
        m_activeWindowOrder.push_back(UIWindowType::EquipmentWindow);

        // StatsWindow
        m_allWindows.emplace(UIWindowType::StatsWindow, std::make_unique<StatWindow>());
        if (auto* window = GetWindow(UIWindowType::StatsWindow))
        {
            window->SetActivate(true);
        }
        m_activeWindowOrder.push_back(UIWindowType::StatsWindow);
        // EnhancementWindow
        //m_allWindows.emplace(UIWindowType::EnhancementWindow, std::make_unique<EnhancementWindow>());
        //if (auto* window = GetWindow(UIWindowType::EnhancementWindow))
        //{
        //    window->SetActivate(false);
        //}

        // SynthesisWindow
        m_allWindows.emplace(UIWindowType::SynthesisWindow, std::make_unique<SynthesisWin>());
        if (auto* window = GetWindow(UIWindowType::SynthesisWindow))
        {
            window->SetActivate(true);
        }
        m_activeWindowOrder.push_back(UIWindowType::SynthesisWindow);

        //// StatPotionUseWindow
        ////m_allWindows.emplace(UIWindowType::StatPotionUseWindow, std::make_unique<StatPotionUseWindow>());
        //if (auto* window = GetWindow(UIWindowType::StatPotionUseWindow))
        //{
        //    window->SetActivate(false);
        //}

        OutputDebugStringA("UIManager initialized successfully\n");
    }
    // ����׿�.
    catch (const std::exception& e)
    {
        std::string errorMsg = "UIManager Initialize failed: " + std::string(e.what()) + "\n";
        OutputDebugStringA(errorMsg.c_str());
    }

}
void UIManager::Update()
{

    for (UIWindowType type : m_activeWindowOrder)
    {
        if (auto* window = GetWindow(type))
        {
            window->Update();
        }
    }
}

void UIManager::Render()
{
    for (UIWindowType type : m_activeWindowOrder)
    {
        if (auto* window = GetWindow(type))
        {
            window->Render();
        }
    }

    //m_allWindows.find(UIWindowType::InventoryWindow)->second->Render();
    //m_allWindows.find(UIWindowType::EquipmentWindow)->second->Render();
    //m_allWindows.find(UIWindowType::StatsWindow)->second->Render();
        m_allWindows.find(UIWindowType::SynthesisWindow)->second->Render();

}

const char* UIManager::UIWindowTypeToString(UIWindowType type)
{
    switch (type)
    {
    case UIWindowType::SettingsWindow:
        return "SettingsWindow";
    case UIWindowType::InventoryWindow:
        return "InventoryWindow";
    case UIWindowType::EquipmentWindow:
        return "EquipmentWindow";
    case UIWindowType::StatsWindow:
        return "StatsWindow";
    case UIWindowType::EnhancementWindow:
        return "EnhancementWindow";
    case UIWindowType::SynthesisWindow:
        return "SynthesisWindow";
    case UIWindowType::StatPotionUseWindow:
        return "StatPotionUseWindow";
    default:
        return "Unknown";
    }
}

void UIManager::OnInput(const MSG& msg)
{
    // 1. ��� â�� �����ִ��� ���� üũ
    if (m_activeWindowOrder.empty())
    {
        // �ٷ� �� ������Ʈ�鿡�� �Է� ����
        HandleSceneObjectInput(msg);
        return;
    }

    if (msg.message == WM_LBUTTONUP)
    {
        Vec2 mousePos = { (float)GET_X_LPARAM(msg.lParam), (float)GET_Y_LPARAM(msg.lParam) };

        // CursorManager�� ���� �������� �巡�� ������ Ȯ��
        if (CursorManager::Get().IsDragging())
        {
            // ���� ��ܿ� �ִ� â���� Ž���Ͽ� ��� ��ġ�� ã��.
            for (auto it = m_activeWindowOrder.rbegin(); it != m_activeWindowOrder.rend(); ++it)
            {
                UIWindow* window = GetWindow(*it);
                // ���콺�� ���� â ���� �ȿ� �ְ�, �� â�� ��� ó���� �����ߴٸ�
                if (window && window->IsInBounds(mousePos) && window->HandleMouseUp(mousePos))
                {
                    CursorManager::Get().EndItemDrag(); // �巡�� ����
                    return; // �Է� ó�� �Ϸ�
                }
            }

            // ��� â���� ��� ó���� �������� ��� ������ �ٽ� ���ͽ�ų����.
            CursorManager::Get().EndItemDrag();
            return;
        }
    }

    // â�� ������ �������� ��ȸ (�ֻ�ܺ���)
    for (auto it = m_activeWindowOrder.rbegin(); it != m_activeWindowOrder.rend(); ++it)
    {
        if (auto* window = GetWindow(*it))
        {
            if (window->HandleInput(msg))
            {
                return; // �Է��� ó���Ǹ� ����
            }
        }
    }


    /* Vec2 CORD = { F_GET_X_LPARAM(msg.lParam), F_GET_X_LPARAM(msg.lParam) };
     std::cout << "������ X" << CORD.x << "������ Y" << CORD.y << endl;*/

      // 3. ��� â�� ó�� �������� �� ������Ʈ�� ����
     // HandleSceneObjectInput(msg);
}

void UIManager::ToggleWindow(UIWindowType type)
{
    if (IsWindowActive(type))
    {
        CloseWindow(type);
    }
    else
    {
        OpenWindow(type);
    }
}

inline bool UIManager::IsWindowActive(UIWindowType type) const
{
    return std::find(m_activeWindowOrder.begin(), m_activeWindowOrder.end(), type) != m_activeWindowOrder.end();
}

UIWindowType UIManager::GetTopmostActiveWindow() const
{
    // ������� ���� ��� ������ ��� ����.
    if (!m_activeWindowOrder.empty())
    {
        return m_activeWindowOrder.back();
    }
}

void UIManager::OpenWindow(UIWindowType type)
{
    // �̹� ���������� �ݱ� (�������� ����)
    CloseWindow(type);

    // �ֻ�ܿ� �߰�
    m_activeWindowOrder.push_back(type);

    // â Ȱ��ȭ
    if (auto* window = GetWindow(type))
    {
        window->SetActivate(true);
    }
}

void UIManager::CloseWindow(UIWindowType type)
{
    // m_activeWindowOrder���� Ÿ�� ã��.
    auto it = std::find(m_activeWindowOrder.begin(), m_activeWindowOrder.end(), type);

    if (it != m_activeWindowOrder.end())
    {
        // â ��Ȱ��ȭ
        if (auto* window = GetWindow(type))
        {
            window->SetActivate(false);
        }
        // Ȱ�� ��Ͽ��� �����.
        m_activeWindowOrder.erase(it);
    }
}

void UIManager::CloseAllWindows()
{
    for (UIWindowType type : m_activeWindowOrder)
    {
        if (auto* window = GetWindow(type))
        {
            window->SetActivate(false);
        }
    }
    m_activeWindowOrder.clear();
}

inline UIWindow* UIManager::GetWindow(UIWindowType type) const
{
    auto it = m_allWindows.find(type);
    return (it != m_allWindows.end()) ? it->second.get() : nullptr;
}

void UIManager::ShowTooltip(UIWindowType tooltipType, const Vec2& position)
{
    // ������ �ٸ� â�� ���� ǥ�õǾ�� �ϹǷ� Ư�� ó��
    if (auto* tooltip = GetWindow(tooltipType))
    {
        tooltip->SetPosition(position);
        // ������ �̹� Ȱ��ȭ�Ǿ� �ִٸ� ��ġ�� ������Ʈ
        if (!IsWindowActive(tooltipType))
        {
            OpenWindow(tooltipType);
        }
    }
}

void UIManager::HideTooltip(UIWindowType tooltipType)
{
    CloseWindow(tooltipType);
}

void UIManager::SetCurrentSceneObjects(const std::vector<std::shared_ptr<Object>>& objects)
{
    m_currentSceneObjects = objects;
}

void UIManager::AddSceneObject(std::shared_ptr<Object> obj)
{
    if (obj)
    {
        m_currentSceneObjects.push_back(obj);
    }
}

void UIManager::RemoveSceneObject(std::shared_ptr<Object> obj)
{
    m_currentSceneObjects.erase(
        std::remove(m_currentSceneObjects.begin(), m_currentSceneObjects.end(), obj),
        m_currentSceneObjects.end()
    );
}

void UIManager::ClearSceneObjects()
{
    m_currentSceneObjects.clear();
}

void UIManager::HandleSceneObjectInput(const MSG& msg)
{
    for (auto& obj : m_currentSceneObjects)
    {
        if (obj.get()->GetComponent<ButtonComponent>())
        {
            obj.get()->GetComponent<ButtonComponent>()->Worked(msg);
        }
    }
}