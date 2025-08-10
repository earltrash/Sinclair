#include "ButtonComponent.h"
#include "ResourceManager.h"
#include "MouseListenerComponent.h"

void ButtonComponent::BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap)
{
	m_Bitmaps.emplace(NM, Bitmap);
}

ComPtr<ID2D1Bitmap1> ButtonComponent::GetBitmap()
{

    switch (m_currentState)
    {
    case ButtonState::Normal:   SetOpacity(1.0f); break;
    case ButtonState::Hover:    SetOpacity(0.55f); break;
    case ButtonState::Pressed:  SetOpacity(0.1f); break;
    case ButtonState::Disabled: SetOpacity(0.0f); break;
    default:  break;
    }

    return m_renderInfo->GetRenderInfo().bitmap;
}



void ButtonComponent::CheckCollision(const MSG& msg)
{

    POINT CORD = { GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) };
    Vec2 pos = m_Owner->GetTransform().GetPosition();


    // �����ư�� �ƴѵ� ��Ʈ���� �������� �ʾҴٸ� ����
    if (!GetBitmap()) return;

    // ���� ���¿� �´� ��Ʈ�� ũ�� ���
    auto currentBitmap = GetBitmap();
    D2D1_SIZE_F size = currentBitmap->GetSize();
    Rect rect(pos.x, pos.y, size.width, size.height);

    wasInside = (m_currentState == ButtonState::Hover || m_currentState == ButtonState::Pressed);
    isInside = InRect(rect, CORD);
}

void ButtonComponent::Worked(const MSG& msg)
{
    if (isInside)
    {
        if (msg.message == WM_LBUTTONDOWN)
        {
            SetState(ButtonState::Pressed);
            // Ŭ�� �̺�Ʈ�� ���콺 ��ư�� �� �� �߻���Ű�� ���� �Ϲ���
        }
        else if (msg.message == WM_LBUTTONUP && m_currentState == ButtonState::Pressed)
        {
            // ��ư�� ���� ���¿��� ���콺�� ���� ���� Ŭ������ ó��
            if (m_onClick) m_onClick();
            SetState(ButtonState::Hover);
        }
        else if (msg.message == WM_MOUSEMOVE && m_currentState != ButtonState::Pressed)
        {
            SetState(ButtonState::Hover);
        }
    }
    else
    {
        // ���콺�� ��ư ������ ����� ��
        if (wasInside)
        {
            SetState(ButtonState::Normal);
        }
    }
}

void ButtonComponent::SetCurrentBitmap(string Nm)
{
    m_renderInfo->SetBitmap(m_Bitmaps[Nm].Get());
}

