#include "ButtonComponent.h"
#include "ResourceManager.h"
#include "MouseListenerComponent.h"

void ButtonComponent::BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap)
{
	m_Bitmap.emplace(NM, Bitmap);
}

ComPtr<ID2D1Bitmap1> ButtonComponent::GetBitmap()
{
    string key;
    switch (m_currentState)
    {
    case ButtonState::Normal:   key = "normal"; break;
    case ButtonState::Hover:    key = "hover"; break;
    case ButtonState::Pressed:  key = "pressed"; break;
    case ButtonState::Disabled: key = "disabled"; break;
    default: key = "normal"; break;
    }

    auto it = m_Bitmap.find(key);
    if (it != m_Bitmap.end())
        return it->second;

    return nullptr;
}

//Object Ȱ��ȭ ��Ȱ��ȭ�� ���� ������ ������ ���� ���� ���� -> �漮�����̶� ����� ��.
void ButtonComponent::Worked(const MSG& msg)
{
    // ��Ʈ���� �������� �ʾҴٸ� ����
    if (!GetBitmap()) return;

    POINT CORD = { GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) };
    Vec2 pos = m_Owner->GetTransform().GetPosition();

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

   ///* if (!activated || !m_curbm) 
   // return; */
   //
   //
   // POINT CORD = { GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) }; //msg -> ���콺 Ŭ�� ��ǥ 
   // Vec2 pos = m_Owner->GetTransform().GetPosition(); //�� Obj ��ǥ
   // D2D1_SIZE_F size = m_curbm->GetSize(); //Bitmap �� �ʺ�� height -> float rect�� ���� ���� ���� 
   // Rect rect(pos.x, pos.y, size.width, size.height); //Float type Rect�� 



   // if (InRect(rect, CORD)) //���콺 ��ǥ���� Object ������ ��� 
   // {
   //     if (msg.message == WM_LBUTTONDOWN) //�װ� Ŭ���̸�
   //     {
   //         if (m_onClick)
   //             m_onClick();
   //         SetState(ButtonState::Pressed);
   //     }
   //     else if (msg.message == WM_MOUSEMOVE) //hovered �� 
   //     {
   //         SetState(ButtonState::Hover); //�̿� ���� Bitmap ���浵 ���⿡ �־ �ɵ�? �ϴ� ���� 
   //     }
   // }

   // else
   // SetState(ButtonState::Normal); //���� ���� ���ٸ� Normal�� 
}

void ButtonComponent::SetCurrentBitmap(string Nm)
{
	m_curbm = m_Bitmap[Nm];
	//Ȥ�� Find�� �����ϴ� �� �ʿ��ϱ� ��. 
}

