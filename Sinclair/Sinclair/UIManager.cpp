#include "UIManager.h"

void UIManager::Initialize()
{


    AddComponent<MouseListenerComponent>(
            [this](const MSG& msg) { OnInput(msg); }
    );

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
            window->SetActivate(false);
        }
      //  m_activeWindowOrder.push_back(UIWindowType::InventoryWindow);

         
        m_allWindows.emplace(UIWindowType::InventoryTooltip, std::make_unique<ToolTip>());
        if (auto* window = GetWindow(UIWindowType::InventoryTooltip))
        {
            window->SetActivate(false);
        }
        //m_activeWindowOrder.push_back(UIWindowType::InventoryTooltip);

        // EquipmentWindow
        m_allWindows.emplace(UIWindowType::EquipmentWindow, std::make_unique<EquipmentWindow>());

        if (auto* window = GetWindow(UIWindowType::EquipmentWindow))
        {
            window->SetActivate(false);
        }
        //m_activeWindowOrder.push_back(UIWindowType::EquipmentWindow);

        // StatsWindow
        m_allWindows.emplace(UIWindowType::StatsWindow, std::make_unique<StatWindow>());
        if (auto* window = GetWindow(UIWindowType::StatsWindow))
        {
            window->SetActivate(false);
        }
        //m_activeWindowOrder.push_back(UIWindowType::StatsWindow);

        //EnhancementWindow
        m_allWindows.emplace(UIWindowType::EnhancementWindow, std::make_unique<EnhancementWindow>());
        if (auto* window = GetWindow(UIWindowType::EnhancementWindow))
        {
            window->SetActivate(false);
        }
        //m_activeWindowOrder.push_back(UIWindowType::EnhancementWindow);

        //// SynthesisWindow
        m_allWindows.emplace(UIWindowType::SynthesisWindow, std::make_unique<SynthesisWin>());
        if (auto* window = GetWindow(UIWindowType::SynthesisWindow))
        {
            window->SetActivate(false);
        }
       // m_activeWindowOrder.push_back(UIWindowType::SynthesisWindow);

        //// StatPotionUseWindow

        m_allWindows.emplace(UIWindowType::StatPotionUseWindow, std::make_unique<UIPotion>());
        if (auto* window = GetWindow(UIWindowType::StatPotionUseWindow))
        {
            window->SetActivate(false);
        }
       // m_activeWindowOrder.push_back(UIWindowType::StatPotionUseWindow);



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
            if (window->IsActive())

            {
                window->Render();

            }
        }
    }

    //m_allWindows.find(UIWindowType::InventoryWindow)->second->Render();
    //m_allWindows.find(UIWindowType::EquipmentWindow)->second->Render();
    //m_allWindows.find(UIWindowType::StatsWindow)->second->Render();

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
                if (*it == UIWindowType::InventoryTooltip) continue;

                UIWindow* window = GetWindow(*it);
                // ���콺�� ���� â ���� �ȿ� �ְ�, �� â�� ��� ó���� �����ߴٸ�
                if (window && window->IsInBounds(mousePos) && window->HandleMouseUp(mousePos))
                {
                    CursorManager::Get().EndItemDrag(); // �巡�� ����
                    return; // �Է� ó�� �Ϸ�
                }
            }

            // ��� â���� ��� ó���� �������� ��� ������ �ٽ� ���ͽ�ų����.
            if (CursorManager::Get().IsDragging())
            {
                Item* draggedItem = CursorManager::Get().GetDraggedItem();
                if (draggedItem)
                {
                    DragSource source = CursorManager::Get().GetDragSource();
                    if (auto* inventory = dynamic_cast<Inventory*>(GetWindow(UIWindowType::InventoryWindow)))
                    {
                        inventory->AddItem(draggedItem->m_data.id, 1);
                        std::cout << "â �ܺ� ������� ���� �κ��丮�� ��ȯ: " << draggedItem->m_data.name << std::endl;
                    }
                }
            }
            CursorManager::Get().EndItemDrag();
            return;
        }

    }

    // â�� ������ �������� ��ȸ (�ֻ�ܺ���)
    //for (auto it = m_activeWindowOrder.rbegin(); it != m_activeWindowOrder.rend(); ++it)
    //{
    //    if (auto* window = GetWindow(*it))
    //    {
    //        if (window->HandleInput(msg))
    //        {
    //            return; // �Է��� ó���Ǹ� ����
    //        }
    //    }
    //}



    //auto orderSnapshot = m_activeWindowOrder;

    //if (msg.message == WM_MOUSEMOVE) {
    //    Vec2 mousePos = { (float)GET_X_LPARAM(msg.lParam), (float)GET_Y_LPARAM(msg.lParam) };

    //    // ���� â Hover ó��
    //    for (auto it = orderSnapshot.rbegin(); it != orderSnapshot.rend(); ++it) {
    //        if (auto* w = GetWindow(*it)) {
    //            w->HandleMouseHover(mousePos);
    //        }
    //    }
    //    // ������ ���������� �׻� �������� ȣ��
    //    if (auto* tooltip = GetWindow(UIWindowType::InventoryTooltip)) {
    //        tooltip->HandleMouseHover(mousePos);
    //    }
    //    return;
    //}

        auto orderSnapshot = m_activeWindowOrder;

    for (auto it = orderSnapshot.rbegin(); it != orderSnapshot.rend(); ++it)
    {
        if (auto* window = GetWindow(*it))
        {
            if (window->HandleInput(msg))
                return;
        }
    }



    HandleSceneObjectInput(msg);

    return;
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

        // ������ �̹� Ȱ��ȭ�Ǿ� �ִٸ� ��ġ�� ������Ʈ,  
        /*if (!IsWindowActive(tooltipType))
        {
            OpenWindow(tooltipType);
        }*/
        OpenWindow(tooltipType); // OpenWindow�� ���� CloseWindow�� ���� �� push_back �ϹǷ� �׻� topmost�� ��


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
            obj.get()->GetComponent<ButtonComponent>()->CheckCollision(msg);
            obj.get()->GetComponent<ButtonComponent>()->Worked(msg);
        }
    }
}

void UIManager::ShowPotionWindow(int level)
{
    Vec2 position(698, 157);

    // ������ �ٸ� â�� ���� ǥ�õǾ�� �ϹǷ� Ư�� ó��
    if (auto* tooltip = GetWindow(UIWindowType::StatPotionUseWindow))
    {
        tooltip->SetPosition(position);
        UIPotion* PotionUI = dynamic_cast<UIPotion*>(tooltip);

        if (PotionUI != nullptr)
        {
            PotionUI->SetLevel(static_cast<PotionLevel>(level));
            OpenWindow(UIWindowType::StatPotionUseWindow);
        }
    }
}

std::string UIManager::GetWearablePartString(Wearable_part type)
{
    switch (type)
    {
    case Wearable_part::Weapon:
        return "Weapon";
    case Wearable_part::Shoes:
        return "Shoes";
    case Wearable_part::Ring:
        return "Ring";
    case Wearable_part::Neckless:
        return "Neckless";
    case Wearable_part::Glove:
        return "Glove";
    case Wearable_part::Under:
        return "Under";
    case Wearable_part::Upper:
        return "Upper";
    case Wearable_part::Helmet:
        return "Helmet";
    case Wearable_part::Cape:
        return "Cape";
    case Wearable_part::UnKnown:
    default:
        return "UnKnown";
    }
}
