#pragma once
#include "UIWindow.h"
#include "ItemGlobal.h"

#define ToolTipSize 98.0f
#define EnchancerSizeX 18.0f
#define EnchancerSizeY 19.0f



enum class TextType
{
    ItemNm,
    Enchancable,
    Synthesisable,
    STR,
    DEX,
    INT,
    LUK,
    DES,
    DES_N,
    Nothing
};




constexpr float PtToPx(float pt) { return pt * (96.0f / 72.0f); }


class Item;
class Wearable;

class ToolTip : public  UIWindow
{
public:
    ToolTip();
    ~ToolTip() = default;

    void Update() override;
    void Render() override;

    bool HandleMouseHover(Vec2 mousePos) override;
    bool HandleMouseDown(Vec2 mousePos) override;
    bool HandleMouseUp(Vec2 mousePos) override;
    bool HandleDoubleClick(Vec2 mousePos) override;
    bool HandleMouseRight(Vec2 mousePos) override;

    bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) override;

    void UpdatePositions();


    virtual void SetActivate(bool active) { m_isActive = active; }
    UIWindowType GetType() override;
    void MemInit();

    void DrawMessageCenterHorizontal(
        const std::wstring& text,
        float topY,
        float fontSize,
        const std::wstring& fontName,
        const D2D1::ColorF& color
    );

    void LoadUIBitmaps(); //기본 양식들만 만져주자 



    void TextRender(Item*& item);
    void BitmapRender(Item*& item);


private:
    //
    std::vector<EnchancerType> m_enchancer_Type;

    std::vector<ComPtr<ID2D1Bitmap1> > m_enchacer_bitmap;


    std::vector<Vec2> m_enchancer_Pos; //그냥 벡터랑 연결 
    std::vector<Vec2> m_enchancer_PosBase;

    std::map< TextType, Vec2> m_text_pos;
    std::map<TextType, Vec2> m_text_posBase;

};

