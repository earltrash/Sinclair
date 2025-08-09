#pragma once
#include "UIWindow.h"
#include "Status.h"
//StatPotionUseWindow         // 스탯 증가 물약 사용창

#define ButtonX 425.0f
#define ButtonY 55.0f


enum class PotionStat
{
    STR,
    DEX,
    INT,
    LUK,
    nothing

};



class UIPotion : public UIWindow
{

public:
    UIPotion();
    ~UIPotion() = default;


    void Update() override;
    void Render() override;

    bool HandleMouseHover(Vec2 mousePos) override;
    bool HandleMouseDown(Vec2 mousePos) override;
    bool HandleMouseUp(Vec2 mousePos) override;
    bool HandleDoubleClick(Vec2 mousePos) override;

    void LoadUIBitmaps(); //얘는 비트맵 가져오기지 뭐
    void InitializeMem(); //버튼 오브젝트도 만들어야 하고, 슬롯도 처리해야 함
    UIWindowType GetType();
    bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) override;

    bool InButton(Vec2 Butpos ,Vec2 mousePos);
    void SetLevel(PotionLevel level);
    PotionLevel GetLevel();

    void UpdateSlotPositions();

private: // 오브젝트로 만들어서 컴포넌트 넣어가지고 위치 받으면 처리?

    //위치로 모두 다 처리해버리고
    
    // 아이템 정보를 받고 UPdate가 되어버림
    //활성화 시킨 촉매의 아이템 정보값을 받아서 설정을 때려버리고 
    // 
    //그 아이템의 이름, 스탯 상승률 


    //초기

    vector<Vec2> m_button;
    vector < ComPtr<ID2D1Bitmap1>> m_BG_BITMAP;

    vector<float> m_But_opacity;

    vector <ComPtr<ID2D1Bitmap1>> m_str_bitmap; //0 1 2 level에 따라서 쉽게하려고
    vector <ComPtr<ID2D1Bitmap1>> m_dex_bitmap;
    vector <ComPtr<ID2D1Bitmap1>> m_int_bitmap;
    vector <ComPtr<ID2D1Bitmap1>> m_luk_bitmap;

    PotionLevel m_PotionLevel;
    PotionStat m_Stat;
    //그럼 받아야 하는게, 스탯이랑 수치 정도치잖아 
};

