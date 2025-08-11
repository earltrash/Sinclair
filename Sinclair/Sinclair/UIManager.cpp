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
    // 디버그용.
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
    // 1. 모든 창이 꺼져있는지 먼저 체크
    if (m_activeWindowOrder.empty())
    {
        // 바로 씬 오브젝트들에게 입력 전달
        HandleSceneObjectInput(msg);
        return;
    }

    if (msg.message == WM_LBUTTONUP)
    {
        Vec2 mousePos = { (float)GET_X_LPARAM(msg.lParam), (float)GET_Y_LPARAM(msg.lParam) };

        // CursorManager가 현재 아이템을 드래그 중인지 확인
        if (CursorManager::Get().IsDragging())
        {
            // 가장 상단에 있는 창부터 탐색하여 드롭 위치를 찾음.
            for (auto it = m_activeWindowOrder.rbegin(); it != m_activeWindowOrder.rend(); ++it)
            {
                if (*it == UIWindowType::InventoryTooltip) continue;

                UIWindow* window = GetWindow(*it);
                // 마우스가 현재 창 영역 안에 있고, 그 창이 드롭 처리를 성공했다면
                if (window && window->IsInBounds(mousePos) && window->HandleMouseUp(mousePos))
                {
                    CursorManager::Get().EndItemDrag(); // 드래그 종료
                    return; // 입력 처리 완료
                }
            }

            // 모든 창에서 드롭 처리가 실패했을 경우 아이템 다시 복귀시킬거임.
            if (CursorManager::Get().IsDragging())
            {
                Item* draggedItem = CursorManager::Get().GetDraggedItem();
                if (draggedItem)
                {
                    DragSource source = CursorManager::Get().GetDragSource();
                    if (auto* inventory = dynamic_cast<Inventory*>(GetWindow(UIWindowType::InventoryWindow)))
                    {
                        inventory->AddItem(draggedItem->m_data.id, 1);
                        std::cout << "창 외부 드롭으로 인해 인벤토리로 반환: " << draggedItem->m_data.name << std::endl;
                    }
                }
            }
            CursorManager::Get().EndItemDrag();
            return;
        }

    }

    // 창이 있으면 역순으로 순회 (최상단부터)
    //for (auto it = m_activeWindowOrder.rbegin(); it != m_activeWindowOrder.rend(); ++it)
    //{
    //    if (auto* window = GetWindow(*it))
    //    {
    //        if (window->HandleInput(msg))
    //        {
    //            return; // 입력이 처리되면 종료
    //        }
    //    }
    //}



    //auto orderSnapshot = m_activeWindowOrder;

    //if (msg.message == WM_MOUSEMOVE) {
    //    Vec2 mousePos = { (float)GET_X_LPARAM(msg.lParam), (float)GET_Y_LPARAM(msg.lParam) };

    //    // 기존 창 Hover 처리
    //    for (auto it = orderSnapshot.rbegin(); it != orderSnapshot.rend(); ++it) {
    //        if (auto* w = GetWindow(*it)) {
    //            w->HandleMouseHover(mousePos);
    //        }
    //    }
    //    // 툴팁은 예외적으로 항상 마지막에 호출
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
    // 비어있지 않은 경우 마지막 요소 리턴.
    if (!m_activeWindowOrder.empty())
    {
        return m_activeWindowOrder.back();
    }
}

void UIManager::OpenWindow(UIWindowType type)
{
    // 이미 열려있으면 닫기 (재정렬을 위해)
    CloseWindow(type);

    // 최상단에 추가
    m_activeWindowOrder.push_back(type);

    // 창 활성화
    if (auto* window = GetWindow(type))
    {
        window->SetActivate(true);
    }
}

void UIManager::CloseWindow(UIWindowType type)
{
    // m_activeWindowOrder에서 타입 찾음.
    auto it = std::find(m_activeWindowOrder.begin(), m_activeWindowOrder.end(), type);

    if (it != m_activeWindowOrder.end())
    {
        // 창 비활성화
        if (auto* window = GetWindow(type))
        {
            window->SetActivate(false);
        }
        // 활성 목록에서 지우기.
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
    // 툴팁은 다른 창들 위에 표시되어야 하므로 특별 처리
    if (auto* tooltip = GetWindow(tooltipType))
    {
        tooltip->SetPosition(position);

        // 툴팁이 이미 활성화되어 있다면 위치만 업데이트,  
        /*if (!IsWindowActive(tooltipType))
        {
            OpenWindow(tooltipType);
        }*/
        OpenWindow(tooltipType); // OpenWindow는 먼저 CloseWindow로 제거 후 push_back 하므로 항상 topmost가 됨


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

    // 툴팁은 다른 창들 위에 표시되어야 하므로 특별 처리
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
