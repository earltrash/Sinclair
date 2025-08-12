#pragma once
#include "CursorManager.h"
#include "EquipmentWindow.h"
#include "StatWindow.h"
#include "pch.h"
#include "Object.h"
#include "InputManager.h"
#include "Status.h" // UIWindowType 가져올려고해서.
#include "UIWindow.h"
#include "MouseListenerComponent.h"
#include "ButtonComponent.h"
#include "SynthesisWin.h"
#include "ToolTip.h"
#include "Inventory.h"
#include "UIPotion.h"
#include "EnhancementWindow.h"


//GamerManager
//SoundManager
// 각각의 inventory, settingwindow, equipment, enhancement, synthesis, statpotionwindow include 하기. 

class CursorManager;
class InputManager;
class CursorManager;
class EquipmentWindow;
class StatWindow;

//class EquipmentWindow;
//class Inventory;

class UIManager : public Object
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
    void Update();

    void FixedUpdate(float deltatime);
    // m_activeWindowOrder 순서대로 Render 호출.
    void Render();

    // InputManager 이벤트 받아서 젤 위의 창부터 순서대로 이벤트 전달하고 성공 실패 여부를 통해서 계속 메세지 전달해보기.
    // 디버깅용임.
    const char* UIWindowTypeToString(UIWindowType type);

    void OnInput(const MSG& msg); //MSG -> CLICK DOUBCLICK UP DOWN 
   

    // 윈도우타입의 창을 키거나 끌거임.
    void ToggleWindow(UIWindowType type);
   

    // 윈도우타입의 창이 켜져있는지 체크용.
    inline bool IsWindowActive(UIWindowType type) const;


    // 현재 제일 활성화된놈중에서 젤 위에놈 리턴해주기.
    UIWindowType GetTopmostActiveWindow() const;

    // 최상단으로 열기
    void OpenWindow(UIWindowType type);

    // 윈도우 닫기
    void CloseWindow(UIWindowType type);

    // 모든 창 닫기
    void CloseAllWindows();


    // 특정 타입의 창 포인터 반환
    inline UIWindow* GetWindow(UIWindowType type) const;


    // 툴팁 표시. 항상 최상단
    void ShowTooltip(UIWindowType tooltipType, const Vec2& position);


    // 툴팁 숨기기
    void HideTooltip(UIWindowType tooltipType);


    // 씬 오브젝트 관리
    void SetCurrentSceneObjects(const std::vector<std::shared_ptr<Object>>& objects);

    // 씬 오브젝트 추가하기
    void AddSceneObject(std::shared_ptr<Object> obj);

    // 씬 특정 오브젝트 지우기
    void RemoveSceneObject(std::shared_ptr<Object> obj);


    // 씬 오브젝트 전체 지우기.
    void ClearSceneObjects();

    // 씬 오브젝트 입력 처리
    // buttoncomponent가 있을거니까 거기에 있는 함수 실행시키기.
    void HandleSceneObjectInput(const MSG& msg);

    void ShowPotionWindow(int level);

    std::string GetWearablePartString(Wearable_part type);



private:
    UIManager() = default;
    virtual ~UIManager() = default;

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