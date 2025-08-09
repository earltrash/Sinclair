#pragma once
#include "UIWindow.h"
#include "Status.h"
//StatPotionUseWindow         // ���� ���� ���� ���â

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

    void LoadUIBitmaps(); //��� ��Ʈ�� ���������� ��
    void InitializeMem(); //��ư ������Ʈ�� ������ �ϰ�, ���Ե� ó���ؾ� ��
    UIWindowType GetType();
    bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) override;

    bool InButton(Vec2 Butpos ,Vec2 mousePos);
    void SetLevel(PotionLevel level);
    PotionLevel GetLevel();

    void UpdateSlotPositions();

private: // ������Ʈ�� ���� ������Ʈ �־���� ��ġ ������ ó��?

    //��ġ�� ��� �� ó���ع�����
    
    // ������ ������ �ް� UPdate�� �Ǿ����
    //Ȱ��ȭ ��Ų �˸��� ������ �������� �޾Ƽ� ������ ���������� 
    // 
    //�� �������� �̸�, ���� ��·� 


    //�ʱ�

    vector<Vec2> m_button;
    vector < ComPtr<ID2D1Bitmap1>> m_BG_BITMAP;

    vector<float> m_But_opacity;

    vector <ComPtr<ID2D1Bitmap1>> m_str_bitmap; //0 1 2 level�� ���� �����Ϸ���
    vector <ComPtr<ID2D1Bitmap1>> m_dex_bitmap;
    vector <ComPtr<ID2D1Bitmap1>> m_int_bitmap;
    vector <ComPtr<ID2D1Bitmap1>> m_luk_bitmap;

    PotionLevel m_PotionLevel;
    PotionStat m_Stat;
    //�׷� �޾ƾ� �ϴ°�, �����̶� ��ġ ����ġ�ݾ� 
};

