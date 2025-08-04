#pragma once
#include "pch.h"
#include "Object.h"
#include "Status.h"
#include "SimpleMathHelper.h"
#include "Inputmanager.h"

using namespace D2DTM;

class UIWindow	: public Object
{
public:
		UIWindow(UIWindowType type, Vec2 pos, Vec2 size)
		: Object(), m_windowType(type), m_position(pos), m_size(size) {} // Object ������ ȣ��
		virtual ~UIWindow() = default;

		virtual void Update() override {};
		virtual void Render() override = 0;
		
		// ���콺 ��ǲ ���� �Լ���.
		virtual bool HandleMouseHover(Vec2 mousePos) = 0;
		virtual bool HandleMouseDown(Vec2 mousePos) = 0;
		virtual bool HandleMouseUp(Vec2 mousePos) = 0;
		virtual bool HandleDoubleClick(Vec2 mousePos) = 0;

		// Ȱ��ȭ�뵵
		virtual void SetActivate(bool active) { m_isActive = active; }

		// Ÿ�� üũ��
		virtual UIWindowType GetType() = 0;

		// ��ġ üũ��
		Vec2 GetPosition() const { return m_position; }

		// �����ִ��� üũ
		bool IsActive() const { return m_isActive; }

		// ���� üũ �Լ���
		bool IsInBounds(Vec2 pos) const
		{
			return pos.x >= m_position.x && pos.x <= m_position.x + m_size.x &&
				pos.y >= m_position.y && pos.y <= m_position.y + m_size.y;
		}

		// Ÿ��Ʋ�� ���� 42�̿��� (��� â �� ����.)
		bool IsInTitleBar(Vec2 pos) const
		{
			return pos.x >= m_position.x && pos.x <= m_position.x + m_size.x &&
				pos.y >= m_position.y && pos.y <= m_position.y + 42; 
		}

		bool IsInCloseButton(Vec2 pos) const
		{
			// �����ʿ��� 47 ������ ������ �����̶�.
			float rightMargin = 47.0f;

			// �ݱ� ��ư x,y ��ġ �����. +7�� ��ȹ�� ���� �־��.
			Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 7 };

			// �ݱ� ��ư ũ��. ���������� ��ȹ���� �ִ� �״����.
			float closeButtonWidth = 27.0f;
			float closeButtonHeight = 27.0f;

			// �ݱ� ��ư �ȿ� �ִ��� üũ.
			return pos.x >= closeButtonPos.x && pos.x <= closeButtonPos.x + closeButtonWidth &&
				pos.y >= closeButtonPos.y && pos.y <= closeButtonPos.y + closeButtonHeight;
		}

		 bool HandleInput(const MSG& msg)
		{
			 Vec2 CORD = { GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) };

			// â�� ��Ȱ��ȭ�Ǿ� ������ ó�� ����
			if (!m_isActive)
				return false;

			// ���콺�� â ���� ���̸� ó�� ����
			if (!IsInBounds(CORD))
				return false;

			// ���� ���콺 �̺�Ʈ ó��
		/*	if (InputManager::Get().IsDoubleClicked())
			{
				return HandleDoubleClick(mousePos);
			}*/

			if (msg.message == WM_LBUTTONDOWN)
			{
				return HandleMouseDown(CORD);
			}

			if (msg.message == WM_LBUTTONUP)
			{
				return HandleMouseUp(CORD);
			}

			// ȣ���� �׻� ó�� (���� ���� ����) -> �� �� �𸣰ڵ�..
			HandleMouseHover(CORD);

			return true; // â ���� ���� �Է��� ��� "ó����"���� ����
		}

protected:
	UIWindowType m_windowType;
	Vec2 m_position;
	Vec2 m_size;
	bool m_isActive = false;
	bool m_isDragging = false;
	Vec2 m_dragOffset;

private:
};

