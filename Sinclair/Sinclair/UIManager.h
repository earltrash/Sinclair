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
#include "EquipmentWindow.h"


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
    void Initialize()
    {
        // ���� â�� ���� ������Ű�� vector�� push 
        
    }
    // m_windowRenderOrder ������� Update ȣ��.
    void Update()
    {

    }
    // m_windowRenderOrder ������� Render ȣ��.
    void Render()
    {

    }
    // InputManager �̺�Ʈ �޾Ƽ� �� ���� â���� ������� �̺�Ʈ �����ϰ� ���� ���� ���θ� ���ؼ� ��� �޼��� �����غ���.
    void OnInput()
    {
        // 1. ��� â�� �����ִ��� ���� üũ
        if (m_windowsPtrs.empty())
        {
            // �ٷ� �� ������Ʈ�鿡�� �Է� ����
            for (auto& obj : m_currentSceneObjects)
            {
                //if (obj.HandleInput()) return; // ó�������� ����
            }
            return;
        }

        // 2. â�� ������ �������� ��ȸ (�ֻ�ܺ���)
        for (auto it = m_windowsPtrs.rbegin(); it != m_windowsPtrs.rend(); ++it)
        {
            //if (m_windows[*it]->HandleInput()) return; // ó�������� ����
        }

        // 3. ��� â�� ó�� �������� �� ������Ʈ�� ����
        for (auto& obj : m_currentSceneObjects)
        {
            //if (obj.HandleInput()) return;
        }
    }

    // ������Ÿ���� â�� Ű�ų� ������.
    void ToggleWindow(UIWindowType type)
    {

    }

    // ������Ÿ���� â�� �����ִ��� üũ��.
    bool IsWindowActive(UIWindowType type)
    {
        for (const auto& window : m_windowsPtrs)
        {
            if (window->GetType() == type)
                return true;
        }
        return false;
    }
    // ���� ���� Ȱ��ȭ�ȳ��߿��� �� ������ �������ֱ�.
   std::shared_ptr<UIWindow> GetTopmostActiveWindow()
    {
        // ������� ���� ��� ������ ��� ����.
        if (!m_windowsPtrs.empty())
        {
            return m_windowsPtrs.back();
        }
    }


private:
    UIManager() = default;
    ~UIManager() = default;

    // �� ������ ��ư ������Ʈ input �ֱ� ���ؼ�.
    std::vector<std::shared_ptr<Object>> m_currentSceneObjects; // �̳� ��� ������ 

    // �̰� ������ �������� �� â�� ���� ������ �ֱ�.
    std::vector<std::shared_ptr<UIWindow>> m_windowsPtrs;
};