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
		: Object(), m_windowType(type), m_position(pos), m_size(size) {} // Object 생성자 호출
		virtual ~UIWindow() = default;

		virtual void Update() override {};
		virtual void Render() override = 0;
		
		// 마우스 인풋 관련 함수들.
		virtual bool HandleMouseHover(Vec2 mousePos) = 0;
		virtual bool HandleMouseDown(Vec2 mousePos) = 0;
		virtual bool HandleMouseUp(Vec2 mousePos) = 0;
		virtual bool HandleDoubleClick(Vec2 mousePos) = 0;

		// 활성화용도
		virtual void SetActivate(bool active) { m_isActive = active; }

		// 타입 체크용
		virtual UIWindowType GetType() = 0;

		// 위치 체크용
		Vec2 GetPosition() const { return m_position; }

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
			// 오른쪽에서 47 떨어진 지점이 기준이라.
			float rightMargin = 47.0f;

			// 닫기 버튼 x,y 위치 계산임. +7는 기획서 보고 넣어둠.
			Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 7 };

			// 닫기 버튼 크기. 마찬가지로 기획서에 있는 그대로임.
			float closeButtonWidth = 27.0f;
			float closeButtonHeight = 27.0f;

			// 닫기 버튼 안에 있는지 체크.
			return pos.x >= closeButtonPos.x && pos.x <= closeButtonPos.x + closeButtonWidth &&
				pos.y >= closeButtonPos.y && pos.y <= closeButtonPos.y + closeButtonHeight;
		}

		 bool HandleInput(const MSG& msg)
		{
			 Vec2 CORD = { GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) };

			// 창이 비활성화되어 있으면 처리 안함
			if (!m_isActive)
				return false;

			// 마우스가 창 영역 밖이면 처리 안함
			if (!IsInBounds(CORD))
				return false;

			// 각종 마우스 이벤트 처리
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

			// 호버는 항상 처리 (툴팁 등을 위해) -> 난 잘 모르겠디..
			HandleMouseHover(CORD);

			return true; // 창 영역 안의 입력은 모두 "처리됨"으로 간주
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

