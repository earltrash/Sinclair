#pragma once
#include "UIWindow.h"
#include "Status.h"
#include "UIManager.h"

class CreditWindow : public UIWindow
{
public:
		CreditWindow() : UIWindow(UIWindowType::CreditWindow, { 431, 270 }, { 1059, 540 })
		{
				m_uiRenderer = std::make_unique<UI_Renderer>();

				if (m_uiRenderer)
				{
						m_uiRenderer->SetBitmap("creditwindow", ResourceManager::Get().GetTexture("CreditWindow"));
						m_uiRenderer->SetBitmap("creditBG", ResourceManager::Get().GetTexture("CreditBG"));
						m_uiRenderer->SetBitmap("close_button", ResourceManager::Get().GetTexture("CloseButton"));
						m_uiRenderer->SetBitmap("close_button_hover", ResourceManager::Get().GetTexture("CloseButton"));
				}
		}
		virtual ~CreditWindow() = default;

		void Update() override;
		void Render() override;

		// ���콺 ��ǲ ���� �Լ���.
		bool HandleMouseDown(Vec2 mousePos) override { if (IsInBounds(mousePos)) return true;  return false; }
		bool HandleMouseUp(Vec2 mousePos) override { if (IsInBounds(mousePos)) return true;  return false; }
		bool HandleMouseHover(Vec2 mousePos) override { if (IsInBounds(mousePos)) return true;  return false; }
		bool HandleDoubleClick(Vec2 mousePos) override { if (IsInBounds(mousePos)) return true;  return false; }
		bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) override { return true; }

		bool HandleMouseRight(Vec2 mousePos) override { if (IsInBounds(mousePos)) return true;  return false; }

		// Ÿ�� üũ��
		UIWindowType GetType() override { return m_windowType; }

		// ������
		void RenderBackground();
		void RenderCloseButton();
		void RenderCreditText();
		void RenderCredtBG();

private:
		std::unique_ptr<UI_Renderer> m_uiRenderer;
};

