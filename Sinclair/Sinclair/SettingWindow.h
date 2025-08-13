#pragma once
#include "UIWindow.h"
#include "SliderHandleComponent.h"
#include "Object.h"
#include <memory>

class UI_Renderer;
class D2DRenderer;
class Item;

class SettingWindow : public UIWindow
{
public:
    SettingWindow();
    virtual ~SettingWindow() = default;

    void Update() override;
    void Render() override;

    bool HandleMouseHover(Vec2 mousePos) override;
    bool HandleMouseDown(Vec2 mousePos) override;
    bool HandleMouseUp(Vec2 mousePos) override;
    bool HandleDoubleClick(Vec2 mousePos) override;
    bool HandleMouseRight(Vec2 mousePos) override;
    bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) override;

    UIWindowType GetType() override;

    void SetBGMValue(float value);
    float GetBGMValue() const;
    void SetSFXValue(float value);
    float GetSFXValue() const;

    void SetOnBGMValueChanged(std::function<void(float)> callback);
    void SetOnSFXValueChanged(std::function<void(float)> callback);

    void CreateSliderHandle();
    void RenderSliderBackground();
    void RenderSliderTrack();
    void RenderValueText();
    void RenderCloseButton();

    // 창과 핸들을 함께 이동시키는 함수
    void MoveHandles(Vec2 delta);

    float getBgmCurrentValue() const {
        return m_bgmCurrentValue;
    }

    float getSfxCurrentValue() const {
        return m_sfxCurrentValue;
    }

private:
    std::unique_ptr<UI_Renderer> m_uiRenderer;

    std::unique_ptr<Object> m_bgmHandleObject;
    SliderHandleComponent* m_bgmHandleComponent = nullptr;
    float m_bgmCurrentValue = 0.5f;
    float m_bgmMinX = 0.0f;
    float m_bgmMaxX = 0.0f;
    bool m_draggingBGM = false;

    std::unique_ptr<Object> m_sfxHandleObject;
    SliderHandleComponent* m_sfxHandleComponent = nullptr;
    float m_sfxCurrentValue = 0.5f;
    float m_sfxMinX = 0.0f;
    float m_sfxMaxX = 0.0f;
    bool m_draggingSFX = false;

    // 창 이동 관련 변수
    bool m_draggingWindow = false;
    Vec2 m_dragOffsetWindow = { 0, 0 };

    float m_minValue = 0.0f;
    float m_maxValue = 100.0f;
    std::function<void(float)> m_onBGMValueChanged;
    std::function<void(float)> m_onSFXValueChanged;
};