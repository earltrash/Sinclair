#pragma once
#include "pch.h"
#include "Object.h"
#include "InputManager.h"
#include "CursorManager.h"
#include "Status.h" // UIWindowType �����÷����ؼ�.
#include "UIWindow.h"

//GamerManager
//SoundManager
// ������ inventory, settingwindow, equipment, enhancement, synthesis, statpotionwindow include �ϱ�. 

#include "Inventory.h"
#include "EquipmentWindow.h"

class InputManager;
class CursorManager;
class EquipmentWindow;
class Inventory;

class UIManager
{
public:
    // �̱���
    static UIManager& Get()
    {
        static UIManager instance;
        return instance;
    }

    // ��� â ������Ʈ�� �����ϰ� setactive false ��ų����.
    void Initialize();
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
    void OnInput()
    {
        // 1. ��� â�� �����ִ��� ���� üũ
        if (m_activeWindowOrder.empty())
        {
            // �ٷ� �� ������Ʈ�鿡�� �Է� ����
            HandleSceneObjectInput();
            return;
        }

        // 2. â�� ������ �������� ��ȸ (�ֻ�ܺ���)
        for (auto it = m_activeWindowOrder.rbegin(); it != m_activeWindowOrder.rend(); ++it)
        {
            if (auto* window = GetWindow(*it))
            {
                if (window->HandleInput())
                {
                    return; // �Է��� ó���Ǹ� ����
                }
            }
        }

        // 3. ��� â�� ó�� �������� �� ������Ʈ�� ����
        HandleSceneObjectInput();
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
    void HandleSceneObjectInput()
    {
        for (auto& obj : m_currentSceneObjects)
        {
            /*if (obj.get()->GetComponent<Button>())
            {
                return;
            }*/
        }
    }

private:
    UIManager() = default;
    ~UIManager();

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