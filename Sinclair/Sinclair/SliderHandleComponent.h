#pragma once
#include <pch.h>
#include "Component.h"
#include "RenderInfo.h"
#include <functional>
#include "SimpleMathHelper.h"
#include "InputManager.h"
#include "Object.h"

using Vec2 = MYHelper::Vector2F;

class SliderHandleComponent : public Component
{
public:
    enum class ButtonState { Normal, Hover, Pressed, Disabled };

    SliderHandleComponent(RenderInfo* renderInfo, float minX, float maxX, float& externalValue)
        : m_renderInfo(renderInfo), m_minX(minX), m_maxX(maxX),
        m_externalValue(externalValue), m_currentState(ButtonState::Normal),
        m_width(19.0f), m_height(38.0f)
    {
    }

    ~SliderHandleComponent() = default;

    void SetWidth(float w) {
        m_width = w;
        UpdateDestRect();
    }

    void SetHeight(float h) {
        m_height = h;
        UpdateDestRect();
    }

    void SetOpacity(float o) {
        if (m_renderInfo) {
            m_renderInfo->SetOpacity(o);
        }
    }

    void SetState(ButtonState state)
    {
        if (m_currentState != state) {
            m_currentState = state;
            if (m_renderInfo) {
                m_renderInfo->SetBitmap(GetBitmap().Get());
            }
        }
    }

    ComPtr<ID2D1Bitmap1> GetBitmap()
    {
        switch (m_currentState)
        {
        case ButtonState::Normal: SetOpacity(1.0f); break;
        case ButtonState::Hover: SetOpacity(0.8f); break;
        case ButtonState::Pressed: SetOpacity(0.6f); break;
        case ButtonState::Disabled: SetOpacity(0.3f); break;
        default: break;
        }

        return m_renderInfo ? m_renderInfo->GetRenderInfo().bitmap : nullptr;
    }

    void SetValue(float normalizedValue)
    {
        float newValue = std::clamp(normalizedValue, 0.0f, 1.0f);
        GetCurrentValueRef() = newValue;
        UpdatePosition();

        if (m_onValueChanged) {
            m_onValueChanged(newValue);
        }
    }

    float GetValue() const
    {
        return GetCurrentValueRef();
    }

    void SetOnValueChanged(std::function<void(float)> callback) {
        m_onValueChanged = callback;
    }

    bool IsPointInHandle(const Vec2& point)
    {
        if (!m_Owner) return false;

        Vec2 handlePos = m_Owner->GetTransform().GetPosition();
        return (point.x >= handlePos.x && point.x <= handlePos.x + m_width &&
            point.y >= handlePos.y && point.y <= handlePos.y + m_height);
    }

    // 이 함수는 SettingWindow에서 호출됩니다. 마우스 드래그에 따른 값 업데이트를 처리합니다.
    void UpdateValueFromMousePos(const Vec2& mousePos)
    {
        float newX = mousePos.x;
        newX = std::clamp(newX, m_minX, m_maxX);

        float oldValue = GetCurrentValueRef();
        float newValue = (newX - m_minX) / (m_maxX - m_minX);

        if (std::abs(oldValue - newValue) > 0.001f) {
            SetValue(newValue);
        }
    }

    void Update() override {
        UpdateDestRect();
    }

    // UpdateRange는 minX, maxX 2개의 인자만 사용
    void UpdateRange(float minX, float maxX)
    {
        m_minX = minX;
        m_maxX = maxX;        
    }

    void UpdateDestRect()
    {
        if (!m_Owner || !m_renderInfo) return;

        Vec2 pos = m_Owner->GetTransform().GetPosition();
        D2D1_RECT_F destRect = {
            pos.x,
            pos.y,
            pos.x + m_width,
            pos.y + m_height
        };
        m_renderInfo->SetDestRect(destRect);
    }

    float& GetCurrentValueRef() const
    {
        return m_externalValue;
    }

    void UpdatePosition()
    {
        float currentValue = GetCurrentValueRef();
        float x = m_minX + (m_maxX - m_minX) * currentValue;
        Vec2 pos = m_Owner->GetTransform().GetPosition();
        pos.x = x;
        m_Owner->SetPosition(pos);
        UpdateDestRect();
    }

private:
    RenderInfo* m_renderInfo;
    float m_minX;
    float m_maxX;
    float& m_externalValue;
    ButtonState m_currentState;
    float m_width;
    float m_height;

    std::function<void(float)> m_onValueChanged;
};