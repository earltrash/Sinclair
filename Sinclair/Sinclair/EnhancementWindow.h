#pragma once
#include "pch.h"
#include "Status.h"
#include "UIWindow.h"
#include "UIManager.h"
#include "SimpleMathHelper.h"
#include "Item.h"
#include "Inventory.h"
#include "UI_Renderer.h"
#include "ResourceManager.h"

class EnhancementWindow : public UIWindow
{
public:
		EnhancementWindow() : UIWindow(UIWindowType::EnhancementWindow, { 100, 100 }, { 234, 345 })
		{
				AddComponent<UI_Renderer>();
		}
		virtual ~EnhancementWindow() = default;

		void Update() override;
		void Render() override;

		// 마우스 인풋 관련 함수들.
		bool HandleMouseDown(Vec2 mousePos) override;

		bool HandleMouseUp(Vec2 mousePos) override;

		// 호버는 아이템 툴팁 떠야할듯. 정확히 모르겠음.
		bool HandleMouseHover(Vec2 mousePos) override;

		// 더블클릭 안받을거임.
		bool HandleDoubleClick(Vec2 mousePos) override { return true; }

		bool HandleMouseRight(Vec2 mousePos) override;

		// 타입 체크용
		UIWindowType GetType() override { return m_windowType; }

		bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) override;



		// 렌더용
		void RenderBackground();
		void RenderTitleBar();
		void RenderCloseButton();
		void RenderEnhancementSlot();
		// 좌우측 끝에 있어서 str int luk dex 이걸 text 바꿔줘야함.
		void RenderStatSelectionButtons();

		// 배경화면과 stat 및 +n 나오는 텍스트 출력.
		void RenderStatText();

		// 강화 횟수 그리기.
		void RenderEnhancementCount();

		// 강화 버튼 3개 그리기.
		void RenderScrollButtons();
private:

};

