#pragma once
#include "CursorManager.h"
#include "EquipmentWindow.h"
#include "StatWindow.h"
#include "pch.h"
#include "Object.h"
#include "InputManager.h"
#include "Status.h" // UIWindowType �����÷����ؼ�.
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
// ������ inventory, settingwindow, equipment, enhancement, synthesis, statpotionwindow include �ϱ�. 

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
    // �̱���
    static UIManager& Get()
    {
        static UIManager instance;
        return instance;
    }

    // ��� â ������Ʈ�� �����ϰ� setactive false ��ų����.
    void Initialize();
   
    // m_activeWindowOrder ������� Update ȣ��.
    void Update();

    void FixedUpdate(float deltatime);
    // m_activeWindowOrder ������� Render ȣ��.
    void Render();

    // InputManager �̺�Ʈ �޾Ƽ� �� ���� â���� ������� �̺�Ʈ �����ϰ� ���� ���� ���θ� ���ؼ� ��� �޼��� �����غ���.
    // ��������.
    const char* UIWindowTypeToString(UIWindowType type);

    void OnInput(const MSG& msg); //MSG -> CLICK DOUBCLICK UP DOWN 
   

    // ������Ÿ���� â�� Ű�ų� ������.
    void ToggleWindow(UIWindowType type);
   

    // ������Ÿ���� â�� �����ִ��� üũ��.
    inline bool IsWindowActive(UIWindowType type) const;


    // ���� ���� Ȱ��ȭ�ȳ��߿��� �� ������ �������ֱ�.
    UIWindowType GetTopmostActiveWindow() const;

    // �ֻ������ ����
    void OpenWindow(UIWindowType type);

    // ������ �ݱ�
    void CloseWindow(UIWindowType type);

    // ��� â �ݱ�
    void CloseAllWindows();


    // Ư�� Ÿ���� â ������ ��ȯ
    inline UIWindow* GetWindow(UIWindowType type) const;


    // ���� ǥ��. �׻� �ֻ��
    void ShowTooltip(UIWindowType tooltipType, const Vec2& position);


    // ���� �����
    void HideTooltip(UIWindowType tooltipType);


    // �� ������Ʈ ����
    void SetCurrentSceneObjects(const std::vector<std::shared_ptr<Object>>& objects);

    // �� ������Ʈ �߰��ϱ�
    void AddSceneObject(std::shared_ptr<Object> obj);

    // �� Ư�� ������Ʈ �����
    void RemoveSceneObject(std::shared_ptr<Object> obj);


    // �� ������Ʈ ��ü �����.
    void ClearSceneObjects();

    // �� ������Ʈ �Է� ó��
    // buttoncomponent�� �����Ŵϱ� �ű⿡ �ִ� �Լ� �����Ű��.
    void HandleSceneObjectInput(const MSG& msg);

    void ShowPotionWindow(int level);

    std::string GetWearablePartString(Wearable_part type);



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