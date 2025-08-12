#pragma once
#include "pch.h"
#include "Object.h"
#include "Status.h"
#include "SimpleMathHelper.h"
#include "Inputmanager.h"
#include "MouseInput.h"

#include "Item.h"

#define F_GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define F_GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

using namespace D2DTM;

class UIWindow : public Object
{
public:
	UIWindow(UIWindowType type, Vec2 pos, Vec2 size)
		: Object(), m_windowType(type), m_position(pos), m_size(size) {} // Object ������ ȣ��
	virtual ~UIWindow() = default;

		virtual void Update() override {};
		virtual void Render() override = 0;
		virtual void FixedUpdate(float dt) override {}
		
		// ���콺 ��ǲ ���� �Լ���.
		virtual bool HandleMouseHover(Vec2 mousePos) = 0;
		virtual bool HandleMouseDown(Vec2 mousePos) = 0;
		virtual bool HandleMouseUp(Vec2 mousePos) = 0;
		virtual bool HandleDoubleClick(Vec2 mousePos) = 0;

	virtual bool HandleMouseRight(Vec2 mousePos) = 0;

	// Ȱ��ȭ�뵵
	virtual void SetActivate(bool active) { m_isActive = active; }


	virtual bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) = 0;

	// Ÿ�� üũ��
	virtual UIWindowType GetType() = 0;

	// ��ġ üũ��
	Vec2 GetPosition() const { return m_position; }

	void SetPosition(Vec2 Pos) { m_position = Pos; } //�� Object�� ������ position�� �Ǿ��ִ°�...


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
			float rightMargin;
			float yOffset;

			// �κ��丮�� ����ó��
			if (m_windowType == UIWindowType::InventoryWindow)
			{
				rightMargin = 75.0f; // x��
				yOffset = 30.0f;     // y��
			}
			else
			{
				rightMargin = 65.0f; // �Ϲ����� ���
				yOffset = 35.0f;     // �Ϲ����� ���
			}

			// �ݱ� ��ư x,y ��ġ ���
			Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + yOffset };

			// �ݱ� ��ư ũ��. ���������� ��ȹ���� �ִ� �״����.
			float closeButtonWidth = 35;
			float closeButtonHeight = 35;

			// �ݱ� ��ư �ȿ� �ִ��� üũ.
			return pos.x >= closeButtonPos.x && pos.x <= closeButtonPos.x + closeButtonWidth &&
				pos.y >= closeButtonPos.y && pos.y <= closeButtonPos.y + closeButtonHeight;
		}



	bool HandleInput(const MSG& msg);




protected:
	UIWindowType m_windowType;
	Vec2 m_position;
	Vec2 m_size;
	Vec2 m_dragOffset;
	bool m_isActive = false;
	bool m_isDragging = false;

private:
};

