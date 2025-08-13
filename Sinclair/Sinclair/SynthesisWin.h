#pragma once
#include "UIWindow.h"
#include "Item.h"

#define SlotSize 74.0f
#define ButX 186.0f
#define ButY 47.0f

enum class SynButton
{
    Syn,
    Cancle,
    Nothing
};

enum class SynSlot
{
    Result,
    Slot1,
    Slot2,
    Nothing
};


class SynthesisWin : public UIWindow
{
public:
    SynthesisWin();
    ~SynthesisWin();

    virtual bool HandleMouseHover(Vec2 mousePos) override;
    virtual bool HandleMouseDown(Vec2 mousePos)  override;
    virtual bool HandleMouseUp(Vec2 mousePos)  override;
    virtual bool HandleDoubleClick(Vec2 mousePos) override;
    virtual bool HandleMouseRight(Vec2 mousePos) override;
    virtual void SetActivate(bool active) { m_isActive = active; }


    void Update() override;
    void Render() override;

    // 타입 체크용
    virtual UIWindowType GetType() override;

    void UpdatePosition();
    void MemBitmapLoad();
    void InitializeObj();

    void MemInit();
    void MemRender();
    SynButton ButtonInit(Vec2 pos);
    SynSlot SlotInit(Vec2 pos);
    

    bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) override;

    // 합성용.
    void PerformSynthesis();

    // 아이템 인벤토리로 반환.
    void ReturnItemToInventory();

    // 합성창에서 드래그한거 막기 위한 함수 추가.

private:

    // size?, bound는 여기에 다 있긴 함. 


    //배열로 해도 되긴 하다만 가독성 
    std::map<SynSlot, Vec2>m_slot;
    std::map<SynSlot, Item*>m_slot_Item;

    std::map<SynButton, Vec2>m_but;
    std::map<SynButton, unique_ptr<Object>> m_buttons;

    //얘는 뭐라고 불러야 할 까 
};