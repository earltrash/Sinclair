// SliderHandleComponent.h (���� ��� ����)
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

    // ��� 1: ������ �ܺ� ������ ���� ����
    SliderHandleComponent(RenderInfo* renderInfo, float minX, float maxX, float& externalValue)
        : m_renderInfo(renderInfo), m_minX(minX), m_maxX(maxX),
        m_externalValue(externalValue), m_isDragging(false), m_currentState(ButtonState::Normal),
        m_width(19.0f), m_height(38.0f)
    {
    }



    ~SliderHandleComponent() = default;

    // ���߿� �ܺ� ������ �����ϴ� �Լ�
    void BindToExternalValue(float& externalValue)
    {
        // ���� ���� �ܺ� ������ ����
        externalValue = m_externalValue;

        // ������ ���ο� �ܺ� ������ ���� (C++���� ���� ����ε��� �Ұ����ϹǷ� ������ ���)
        m_externalValuePtr = &externalValue;
        std::cout << "[SliderHandle] �ܺ� �� ���ε�: " << externalValue << std::endl;
    }

    // �̹��� ��Ƴ��� ���� ����
    unordered_map<string, ComPtr<ID2D1Bitmap1>> m_Bitmaps;

    void BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap)
    {
        m_Bitmaps.emplace(NM, Bitmap);
    }

    void SetCurrentBitmap(string Nm)
    {
        m_renderInfo->SetBitmap(m_Bitmaps[Nm].Get());
    }

    void SetWidth(float w) {
        m_width = w;
        UpdateDestRect();
    }

    void SetHeight(float h) {
        m_height = h;
        UpdateDestRect();
    }

    void SetOpacity(float o) {
        m_renderInfo->SetOpacity(o);
    }

    void SetState(ButtonState state)
    {
        if (m_currentState != state) {
            m_currentState = state;
            m_renderInfo->SetBitmap(GetBitmap().Get());
        }
    }

    ComPtr<ID2D1Bitmap1> GetBitmap()
    {
        switch (m_currentState)
        {
        case ButtonState::Normal:   SetOpacity(1.0f); break;
        case ButtonState::Hover:    SetOpacity(0.8f); break;
        case ButtonState::Pressed:  SetOpacity(0.6f); break;
        case ButtonState::Disabled: SetOpacity(0.3f); break;
        default: break;
        }

        return m_renderInfo->GetRenderInfo().bitmap;
    }

    // �� ���� - �ܺ� ������ �Բ� ������Ʈ
    void SetValue(float value)
    {
        float newValue = std::clamp(value, 0.0f, 1.0f);

        // �ܺ� ���� ������Ʈ
        GetCurrentValueRef() = newValue;

        std::cout << "[SliderHandle] �� ����: " << newValue << std::endl;
        UpdatePosition();

        if (m_onValueChanged) {
            m_onValueChanged(newValue);
        }
    }

    // ���� �� ��ȯ - �׻� �ܺ� ���� ���� ��ȯ
    float GetValue() const
    {
        return GetCurrentValueRef();
    }

    void SetOnValueChanged(std::function<void(float)> callback) {
        m_onValueChanged = callback;
    }

    bool IsPointInHandle(const Vec2& point, const Vec2& handlePos)
    {
        auto destRect = m_renderInfo->GetRenderInfo().destRect;
        return (point.x >= destRect.left && point.x <= destRect.right &&
            point.y >= destRect.top && point.y <= destRect.bottom);
    }

    void HandleMouse(const MSG& msg)
    {
        Vec2 mousePos = InputManager::Get().GetMousePosition();
        Vec2 handlePos = m_Owner->GetTransform().GetPosition();
        bool mousePressed = InputManager::Get().IsMousePressed(MouseButton::Left);
        bool pointInHandle = IsPointInHandle(mousePos, handlePos);

        switch (msg.message)
        {
        case WM_LBUTTONDOWN:
            if (pointInHandle) {
                m_isDragging = true;
                m_dragOffset = mousePos - handlePos;
                SetState(ButtonState::Pressed);
                UpdateValueFromMousePos(mousePos);
            }
            break;

        case WM_MOUSEMOVE:
            if (m_isDragging && mousePressed) {
                UpdateValueFromMousePos(mousePos);
                SetState(ButtonState::Pressed);
            }
            else if (pointInHandle && !m_isDragging) {
                SetState(ButtonState::Hover);
            }
            else if (!m_isDragging) {
                SetState(ButtonState::Normal);
            }
            break;

        case WM_LBUTTONUP:
            if (m_isDragging) {
                m_isDragging = false;
                if (pointInHandle) {
                    SetState(ButtonState::Hover);
                }
                else {
                    SetState(ButtonState::Normal);
                }
            }
            break;
        }
    }

private:
    // ���� ���� ������ ��ȯ�ϴ� ���� �Լ�
    float& GetCurrentValueRef() const
    {
        if (m_externalValuePtr) {
            return *m_externalValuePtr;
        }
        return m_externalValue;
    }

    void UpdateDestRect()
    {
        if (!m_Owner || !m_renderInfo) return;

        Vec2 pos = m_Owner->GetTransform().GetPosition();
        m_renderInfo->SetDestLeft(pos.x);
        m_renderInfo->SetDestTop(pos.y);
        m_renderInfo->SetDestRight(m_width);
        m_renderInfo->SetDestBottom(m_height);
    }

    void UpdateValueFromMousePos(const Vec2& mousePos)
    {
        float newX = mousePos.x - m_dragOffset.x;
        newX = std::clamp(newX, m_minX, m_maxX);

        Vec2 newPos = m_Owner->GetTransform().GetPosition();
        newPos.x = newX;
        m_Owner->SetPosition(newPos);
        UpdateDestRect();

        float oldValue = GetCurrentValueRef();
        float newValue = (newX - m_minX) / (m_maxX - m_minX);

        // �ܺ� ���� ���� ������Ʈ
        GetCurrentValueRef() = newValue;

        if (std::abs(oldValue - newValue) > 0.001f && m_onValueChanged) {
            m_onValueChanged(newValue);
        }
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
    float& m_externalValue;     // �ܺ� ���� ����
    float* m_externalValuePtr;  // ���߿� ���ε��ϴ� ���� ������
    //float m_defaultValue;       // �⺻�� (���� �ʱ�ȭ��)
    bool m_isDragging;
    Vec2 m_dragOffset;
    ButtonState m_currentState;
    float m_width;
    float m_height;

    std::function<void(float)> m_onValueChanged;
};