#pragma once
#include "EquipmentWindow.h"
#include "StatWindow.h"
#include "pch.h"
#include "Object.h"
#include "InputManager.h"
#include "CursorManager.h"
#include "Status.h" // UIWindowType �����÷����ؼ�.
#include "UIWindow.h"
#include "MouseListenerComponent.h"
#include "ButtonComponent.h"


#include "Inventory.h"


//GamerManager
//SoundManager
// ������ inventory, settingwindow, equipment, enhancement, synthesis, statpotionwindow include �ϱ�. 

class InputManager;
class CursorManager;
class EquipmentWindow;
class StatWindow;

//class EquipmentWindow;
//class Inventory;

class UIManager : public Object
{
public:
    // �̱���
    static UIManager& Get()
    {
        static UIManager instance;
        return instance;
    }

    // ��� â ������Ʈ�� �����ϰ� setactive false ��ų����.
    void Initialize()
    {

        
        AddComponent<MouseListenerComponent>
            (
            // ���⼭ ��� �Է� ó�� ����?
            [this](const MSG& msg) {
                OnInput(msg);
            }
        );
        try
        {
            // SettingsWindow
            //m_allWindows.emplace(UIWindowType::SettingsWindow, std::make_unique<SettingsWindow>());
            /*if (auto* window = GetWindow(UIWindowType::SettingsWindow))
            {
                window->SetActivate(false);
            }*/
            // InventoryWindow
            //m_allWindows.emplace(UIWindowType::InventoryWindow, std::make_unique<Inventory>());
            //if (auto* window = GetWindow(UIWindowType::InventoryWindow))
            //{
            //   // window->SetActivate(false);

            //    window->SetActivate(true);

            //}
            //// InventoryTooltip
            ////m_allWindows.emplace(UIWindowType::InventoryTooltip, std::make_unique<InventoryTooltip>());
            //if (auto* window = GetWindow(UIWindowType::InventoryTooltip))
            //{
            //    //window->SetActivate(false);


            //    window->SetActivate(true);

            //}

            // EquipmentWindow
            /*m_allWindows.emplace(UIWindowType::EquipmentWindow, std::make_unique<EquipmentWindow>());
            if (auto* window = GetWindow(UIWindowType::EquipmentWindow))
            {
                window->SetActivate(false);
            }*/

            // StatsWindow
            m_allWindows.emplace(UIWindowType::StatsWindow, std::make_unique<StatWindow>());
            if (auto* window = GetWindow(UIWindowType::StatsWindow))
            {
                window->SetActivate(true);
            }
            OpenWindow(UIWindowType::StatsWindow);
            // EnhancementWindow
            //m_allWindows.emplace(UIWindowType::EnhancementWindow, std::make_unique<EnhancementWindow>());
            //if (auto* window = GetWindow(UIWindowType::EnhancementWindow))
            //{
            //    window->SetActivate(false);
            //}

            //// SynthesisWindow
            ////m_allWindows.emplace(UIWindowType::SynthesisWindow, std::make_unique<SynthesisWindow>());
            //if (auto* window = GetWindow(UIWindowType::SynthesisWindow))
            //{
            //    window->SetActivate(false);
            //}

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
    // m_activeWindowOrder ������� Update ȣ��.
    void Update()
    {
        
        for (UIWindowType type : m_activeWindowOrder)
        {
            if (auto* window = GetWindow(type))
            {
                window->Update();
            }
        }
    }
    // m_activeWindowOrder ������� Render ȣ��.
    void Render()
    {
        for (UIWindowType type : m_activeWindowOrder)
        {
            if (auto* window = GetWindow(type))
            {
                window->Render();
            }
        }
    }
    // InputManager �̺�Ʈ �޾Ƽ� �� ���� â���� ������� �̺�Ʈ �����ϰ� ���� ���� ���θ� ���ؼ� ��� �޼��� �����غ���.
    // ��������.
    const char* UIWindowTypeToString(UIWindowType type)
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
    void OnInput(const MSG& msg) //MSG -> CLICK DOUBCLICK UP DOWN 
    {
        // 1. ��� â�� �����ִ��� ���� üũ
        if (m_activeWindowOrder.empty())
        {
            // �ٷ� �� ������Ʈ�鿡�� �Է� ����
            HandleSceneObjectInput(msg);
            return;
        }

        // 2. â�� ������ �������� ��ȸ (�ֻ�ܺ���)
        for (auto it = m_activeWindowOrder.rbegin(); it != m_activeWindowOrder.rend(); ++it)
        {
            if (auto* window = GetWindow(*it))
            {
                if (window->HandleInput(msg))
                {
                    //std::cout << "â �����ִ°� ����. â ������ : ";
                    //std::cout << UIWindowTypeToString(window->GetType()) << std::endl;
                    return; // �Է��� ó���Ǹ� ����
                }
            }
        }

        // 3. ��� â�� ó�� �������� �� ������Ʈ�� ����
        HandleSceneObjectInput(msg);
    }

    // ������Ÿ���� â�� Ű�ų� ������.
    void ToggleWindow(UIWindowType type)
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

    // ������Ÿ���� â�� �����ִ��� üũ��.
    inline bool IsWindowActive(UIWindowType type) const
    {
        return std::find(m_activeWindowOrder.begin(), m_activeWindowOrder.end(), type) != m_activeWindowOrder.end();
    }

    // ���� ���� Ȱ��ȭ�ȳ��߿��� �� ������ �������ֱ�.
    UIWindowType GetTopmostActiveWindow() const
    {
        // ������� ���� ��� ������ ��� ����.
        if (!m_activeWindowOrder.empty())
        {
            return m_activeWindowOrder.back();
        }
    }

    // �ֻ������ ����
    void OpenWindow(UIWindowType type)
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

    // ������ �ݱ�
    void CloseWindow(UIWindowType type)
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

    // ��� â �ݱ�
    void CloseAllWindows()
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

    // Ư�� Ÿ���� â ������ ��ȯ
    inline UIWindow* GetWindow(UIWindowType type) const
    {
        auto it = m_allWindows.find(type);
        return (it != m_allWindows.end()) ? it->second.get() : nullptr;
    }

    // ���� ǥ��. �׻� �ֻ��
    void ShowTooltip(UIWindowType tooltipType, const Vec2& position)
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

    // ���� �����
    void HideTooltip(UIWindowType tooltipType)
    {
        CloseWindow(tooltipType);
    }

    // �� ������Ʈ ����
    void SetCurrentSceneObjects(const std::vector<std::shared_ptr<Object>>& objects)
    {
        m_currentSceneObjects = objects;
    }
    // �� ������Ʈ �߰��ϱ�
    void AddSceneObject(std::shared_ptr<Object> obj)
    {
        if (obj)
        {
            m_currentSceneObjects.push_back(obj);
        }
    }
    // �� Ư�� ������Ʈ �����
    void RemoveSceneObject(std::shared_ptr<Object> obj)
    {
        m_currentSceneObjects.erase(
            std::remove(m_currentSceneObjects.begin(), m_currentSceneObjects.end(), obj),
            m_currentSceneObjects.end()
        );
    }

    // �� ������Ʈ ��ü �����.
    void ClearSceneObjects()
    {
        m_currentSceneObjects.clear();
    }

    // �� ������Ʈ �Է� ó��
    // buttoncomponent�� �����Ŵϱ� �ű⿡ �ִ� �Լ� �����Ű��.
    void HandleSceneObjectInput(const MSG& msg)
    {
        for (auto& obj : m_currentSceneObjects)
        {
            if (obj.get()->GetComponent<ButtonComponent>())
            {
                obj.get()->GetComponent<ButtonComponent>()->Worked(msg);
            }
        }
    }

private:
    UIManager() = default;
    virtual ~UIManager() = default;

    // ���� ������.
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;

    // ��� â���� Ÿ�Ժ��� ���� 
    std::unordered_map<UIWindowType, std::unique_ptr<UIWindow>> m_allWindows;

    // ���� Ȱ��ȭ�� â���� ���̾�� | ������
    std::vector<UIWindowType> m_activeWindowOrder;

    // �� ������ ��ư ������Ʈ input �ֱ� ���ؼ�.
    std::vector<std::shared_ptr<Object>> m_currentSceneObjects;
};