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

		// ���콺 ��ǲ ���� �Լ���.
		bool HandleMouseDown(Vec2 mousePos) override;

		bool HandleMouseUp(Vec2 mousePos) override;

		// ȣ���� ������ ���� �����ҵ�. ��Ȯ�� �𸣰���.
		bool HandleMouseHover(Vec2 mousePos) override;

		// ����Ŭ�� �ȹ�������.
		bool HandleDoubleClick(Vec2 mousePos) override { return true; }

		bool HandleMouseRight(Vec2 mousePos) override;

		// Ÿ�� üũ��
		UIWindowType GetType() override { return m_windowType; }

		bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) override;



		// ������
		void RenderBackground();
		void RenderTitleBar();
		void RenderCloseButton();
		void RenderEnhancementSlot();
		// �¿��� ���� �־ str int luk dex �̰� text �ٲ������.
		void RenderStatSelectionButtons();

		// ���ȭ��� stat �� +n ������ �ؽ�Ʈ ���.
		void RenderStatText();

		// ��ȭ Ƚ�� �׸���.
		void RenderEnhancementCount();

		// ��ȭ ��ư 3�� �׸���.
		void RenderScrollButtons();
private:

};

