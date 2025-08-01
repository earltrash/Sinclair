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
#include "EquipmentWindow.h"


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
    void Initialize()
    {
        // 각각 창들 전부 생성시키고 vector로 push 
        
    }
    // m_windowRenderOrder 순서대로 Update 호출.
    void Update()
    {

    }
    // m_windowRenderOrder 순서대로 Render 호출.
    void Render()
    {

    }
    // InputManager 이벤트 받아서 젤 위의 창부터 순서대로 이벤트 전달하고 성공 실패 여부를 통해서 계속 메세지 전달해보기.
    void OnInput()
    {
        // 1. 모든 창이 꺼져있는지 먼저 체크
        if (m_windowsPtrs.empty())
        {
            // 바로 씬 오브젝트들에게 입력 전달
            for (auto& obj : m_currentSceneObjects)
            {
                //if (obj.HandleInput()) return; // 처리됐으면 종료
            }
            return;
        }

        // 2. 창이 있으면 역순으로 순회 (최상단부터)
        for (auto it = m_windowsPtrs.rbegin(); it != m_windowsPtrs.rend(); ++it)
        {
            //if (m_windows[*it]->HandleInput()) return; // 처리됐으면 종료
        }

        // 3. 모든 창이 처리 안했으면 씬 오브젝트에 전달
        for (auto& obj : m_currentSceneObjects)
        {
            //if (obj.HandleInput()) return;
        }
    }

    // 윈도우타입의 창을 키거나 끌거임.
    void ToggleWindow(UIWindowType type)
    {

    }

    // 윈도우타입의 창이 켜져있는지 체크용.
    bool IsWindowActive(UIWindowType type)
    {
        for (const auto& window : m_windowsPtrs)
        {
            if (window->GetType() == type)
                return true;
        }
        return false;
    }
    // 현재 제일 활성화된놈중에서 젤 위에놈 리턴해주기.
   std::shared_ptr<UIWindow> GetTopmostActiveWindow()
    {
        // 비어있지 않은 경우 마지막 요소 리턴.
        if (!m_windowsPtrs.empty())
        {
            return m_windowsPtrs.back();
        }
    }


private:
    UIManager() = default;
    ~UIManager() = default;

    // 각 씬마다 버튼 오브젝트 input 주기 위해서.
    std::vector<std::shared_ptr<Object>> m_currentSceneObjects; // 이놈 없어도 괜찮네 

    // 이게 렌더링 순서이자 각 창들 전부 가지고 있기.
    std::vector<std::shared_ptr<UIWindow>> m_windowsPtrs;
};