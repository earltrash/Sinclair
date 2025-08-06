#include "ButtonComponent.h"
#include "ResourceManager.h"
#include "MouseListenerComponent.h"

void ButtonComponent::BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap)
{
	m_Bitmaps.emplace(NM, Bitmap);
}

ComPtr<ID2D1Bitmap1> ButtonComponent::GetBitmap()
{
    if (m_isInvisible)
    {
        return nullptr;
    }

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


void ButtonComponent::Worked(const MSG& msg)
{

    POINT CORD = { GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) };
    Vec2 pos = m_Owner->GetTransform().GetPosition();
    
    //// ���� ��ư ���� �̷���.
    //// ���� ��ư�� ��� ������ ũ�� ���
    //if (m_isInvisible)
    //{
    //    Rect rect(pos.x, pos.y, width, height);
    //    bool isInside = InRect(rect, CORD);

    //    if (isInside)
    //    {
    //        if (msg.message == WM_LBUTTONDOWN)
    //        {
    //            SetState(ButtonState::Pressed);
    //        }
    //        else if (msg.message == WM_LBUTTONUP && m_currentState == ButtonState::Pressed)
    //        {
    //            if (m_onClick) 
    //            {
    //                m_onClick();
    //                return;
    //            }
    //            SetState(ButtonState::Normal);
    //        }
    //        else if (msg.message == WM_MOUSEMOVE && m_currentState != ButtonState::Pressed)
    //        {
    //            SetState(ButtonState::Hover);
    //        }
    //    }
    //    else
    //    {
    //        SetState(ButtonState::Normal);
    //    }
    //    return;
    //}

    // �����ư�� �ƴѵ� ��Ʈ���� �������� �ʾҴٸ� ����
    if (!GetBitmap()) return;

    // ���� ���¿� �´� ��Ʈ�� ũ�� ���
    auto currentBitmap = GetBitmap();
    D2D1_SIZE_F size = currentBitmap->GetSize();
    Rect rect(pos.x, pos.y, size.width, size.height);

    bool wasInside = (m_currentState == ButtonState::Hover || m_currentState == ButtonState::Pressed);
    bool isInside = InRect(rect, CORD);

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

// ���� ��ư ���� �̷�
//void ButtonComponent::SetInvisibleButton(int width, int height)
//{
//    m_isInvisible = true;
//    SetWidth(width);
//    SetHeight(height);
//}

void ButtonComponent::SetCurrentBitmap(string Nm)
{
    m_renderInfo->SetBitmap(m_Bitmaps[Nm].Get());
}

