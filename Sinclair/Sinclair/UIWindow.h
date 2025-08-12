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
		: Object(), m_windowType(type), m_position(pos), m_size(size) {} // Object 생성자 호출
	virtual ~UIWindow() = default;

		virtual void Update() override {};
		virtual void Render() override = 0;
		virtual void FixedUpdate(float dt) override {}
		
		// 마우스 인풋 관련 함수들.
		virtual bool HandleMouseHover(Vec2 mousePos) = 0;
		virtual bool HandleMouseDown(Vec2 mousePos) = 0;
		virtual bool HandleMouseUp(Vec2 mousePos) = 0;
		virtual bool HandleDoubleClick(Vec2 mousePos) = 0;

	virtual bool HandleMouseRight(Vec2 mousePos) = 0;

	// 활성화용도
	virtual void SetActivate(bool active) { m_isActive = active; }


	virtual bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) = 0;

	// 타입 체크용
	virtual UIWindowType GetType() = 0;

	// 위치 체크용
	Vec2 GetPosition() const { return m_position; }

	void SetPosition(Vec2 Pos) { m_position = Pos; } //왜 Object랑 별개의 position이 되어있는가...


	// 켜져있는지 체크
	bool IsActive() const { return m_isActive; }

	// 영역 체크 함수들
	bool IsInBounds(Vec2 pos) const
	{
		return pos.x >= m_position.x && pos.x <= m_position.x + m_size.x &&
			pos.y >= m_position.y && pos.y <= m_position.y + m_size.y;
	}

	// 타이틀바 높이 42이여서 (모든 창 다 같음.)
	bool IsInTitleBar(Vec2 pos) const
	{
		return pos.x >= m_position.x && pos.x <= m_position.x + m_size.x &&
			pos.y >= m_position.y && pos.y <= m_position.y + 42;
	}

		bool IsInCloseButton(Vec2 pos) const
		{
			float rightMargin;
			float yOffset;

			// 인벤토리만 예외처리
			if (m_windowType == UIWindowType::InventoryWindow)
			{
				rightMargin = 75.0f; // x값
				yOffset = 30.0f;     // y값
			}
			else
			{
				rightMargin = 65.0f; // 일반적인 경우
				yOffset = 35.0f;     // 일반적인 경우
			}

			// 닫기 버튼 x,y 위치 계산
			Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + yOffset };

			// 닫기 버튼 크기. 마찬가지로 기획서에 있는 그대로임.
			float closeButtonWidth = 35;
			float closeButtonHeight = 35;

			// 닫기 버튼 안에 있는지 체크.
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

