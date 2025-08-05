#pragma once
#include "pch.h"
#include "Object.h"
#include "InputManager.h"
#include "CursorManager.h"
#include "Status.h" // UIWindowType 가져올려고해서.
#include "UIWindow.h"

//GamerManager
//SoundManager
// 각각의 inventory, settingwindow, equipment, enhancement, synthesis, statpotionwindow include 하기. 

#include "Inventory.h"
#include "EquipmentWindow.h"

class InputManager;
class CursorManager;
class EquipmentWindow;
class Inventory;

class UIManager
{
public:
    // 싱글톤
    static UIManager& Get()
    {
        static UIManager instance;
        return instance;
    }

    // 모든 창 오브젝트들 생성하고 setactive false 시킬거임.
    void Initialize();
    // m_activeWindowOrder 순서대로 Update 호출.
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
    // m_activeWindowOrder 순서대로 Render 호출.
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
    // InputManager 이벤트 받아서 젤 위의 창부터 순서대로 이벤트 전달하고 성공 실패 여부를 통해서 계속 메세지 전달해보기.
    void OnInput()
    {
        // 1. 모든 창이 꺼져있는지 먼저 체크
        if (m_activeWindowOrder.empty())
        {
            // 바로 씬 오브젝트들에게 입력 전달
            HandleSceneObjectInput();
            return;
        }

        // 2. 창이 있으면 역순으로 순회 (최상단부터)
        for (auto it = m_activeWindowOrder.rbegin(); it != m_activeWindowOrder.rend(); ++it)
        {
            if (auto* window = GetWindow(*it))
            {
                if (window->HandleInput())
                {
                    return; // 입력이 처리되면 종료
                }
            }
        }

        // 3. 모든 창이 처리 안했으면 씬 오브젝트에 전달
        HandleSceneObjectInput();
    }

    // 윈도우타입의 창을 키거나 끌거임.
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

    // 윈도우타입의 창이 켜져있는지 체크용.
    inline bool IsWindowActive(UIWindowType type) const
    {
        return std::find(m_activeWindowOrder.begin(), m_activeWindowOrder.end(), type) != m_activeWindowOrder.end();
    }

    // 현재 제일 활성화된놈중에서 젤 위에놈 리턴해주기.
    UIWindowType GetTopmostActiveWindow() const
    {
        // 비어있지 않은 경우 마지막 요소 리턴.
        if (!m_activeWindowOrder.empty())
        {
            return m_activeWindowOrder.back();
        }
    }

    // 최상단으로 열기
    void OpenWindow(UIWindowType type)
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

    // 윈도우 닫기
    void CloseWindow(UIWindowType type)
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

    // 모든 창 닫기
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

    // 특정 타입의 창 포인터 반환
    inline UIWindow* GetWindow(UIWindowType type) const
    {
        auto it = m_allWindows.find(type);
        return (it != m_allWindows.end()) ? it->second.get() : nullptr;
    }

    // 툴팁 표시. 항상 최상단
    void ShowTooltip(UIWindowType tooltipType, const Vec2& position)
    {
        // 툴팁은 다른 창들 위에 표시되어야 하므로 특별 처리
        if (auto* tooltip = GetWindow(tooltipType))
        {
            tooltip->SetPosition(position);
            // 툴팁이 이미 활성화되어 있다면 위치만 업데이트
            if (!IsWindowActive(tooltipType))
            {
                OpenWindow(tooltipType);
            }
        }
    }

    // 툴팁 숨기기
    void HideTooltip(UIWindowType tooltipType)
    {
        CloseWindow(tooltipType);
    }

    // 씬 오브젝트 관리
    void SetCurrentSceneObjects(const std::vector<std::shared_ptr<Object>>& objects)
    {
        m_currentSceneObjects = objects;
    }
    // 씬 오브젝트 추가하기
    void AddSceneObject(std::shared_ptr<Object> obj)
    {
        if (obj)
        {
            m_currentSceneObjects.push_back(obj);
        }
    }
    // 씬 특정 오브젝트 지우기
    void RemoveSceneObject(std::shared_ptr<Object> obj)
    {
        m_currentSceneObjects.erase(
            std::remove(m_currentSceneObjects.begin(), m_currentSceneObjects.end(), obj),
            m_currentSceneObjects.end()
        );
    }

    // 씬 오브젝트 전체 지우기.
    void ClearSceneObjects()
    {
        m_currentSceneObjects.clear();
    }

    // 씬 오브젝트 입력 처리
    // buttoncomponent가 있을거니까 거기에 있는 함수 실행시키기.
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

    // 복사 방지용.
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;

    // 모든 창들을 타입별로 저장 
    std::unordered_map<UIWindowType, std::unique_ptr<UIWindow>> m_allWindows;

    // 현재 활성화된 창들의 레이어용 | 역순임
    std::vector<UIWindowType> m_activeWindowOrder;

    // 각 씬마다 버튼 오브젝트 input 주기 위해서.
    std::vector<std::shared_ptr<Object>> m_currentSceneObjects;
};