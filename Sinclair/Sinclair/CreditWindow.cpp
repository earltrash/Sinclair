#include "CreditWindow.h"

void CreditWindow::Update()
{
}

void CreditWindow::Render()
{
		if (!m_uiRenderer) return;

		RenderBackground();
		RenderCloseButton();
		RenderCredtBG();
		RenderCreditText();
}

void CreditWindow::RenderBackground()
{
		if (m_uiRenderer)
		{
				ID2D1Bitmap1* bg = m_uiRenderer->GetBitmap("creditwindow").Get();
				D2D1_RECT_F dest = { m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y };
				D2DRenderer::Get().DrawBitmap(bg, dest);
		}
}

void CreditWindow::RenderCloseButton()
{
		float rightMargin = 85.0f;
		Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 35.0f };
		Vec2 closeButtonSize = { 35.0f, 35.0f };

		D2D1_RECT_F destRect = { closeButtonPos.x, closeButtonPos.y, closeButtonPos.x + closeButtonSize.x, closeButtonPos.y + closeButtonSize.y };

		if (m_uiRenderer)
		{
				ID2D1Bitmap1* cb = m_uiRenderer->GetBitmap("close_button").Get();
				if (cb) D2DRenderer::Get().DrawBitmap(cb, destRect);
		}
}

void CreditWindow::RenderCreditText()
{
		if (m_uiRenderer)
		{
				// ��ȹ �ؽ�Ʈ
				float planTitleX = m_position.x + 218;
				float planTitleY = m_position.y + 84;
				D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
				D2DRenderer::Get().DrawMessage(L"��ȹ", planTitleX, planTitleY, 100, 48.0f, D2D1::ColorF(0xffe6d8));

				// ��ȹ �� �̸�
				float planNamesX = m_position.x + 225;
				float planNamesY = m_position.y + 266;
				D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 30.0f);
				D2DRenderer::Get().DrawMessage(L"������\n������\n���ֿ�", planNamesX, planNamesY, 100, 30.0f, D2D1::ColorF(0xffe6d8));

				// ���α׷��� �ؽ�Ʈ
				float progTitleX = m_position.x + 612;
				float progTitleY = m_position.y + 84;
				D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 48.0f);
				D2DRenderer::Get().DrawMessage(L"���α׷���", progTitleX, progTitleY, 300, 48.0f, D2D1::ColorF(0xffe6d8));

				// ���α׷��� �� �̸�
				float progNamesX = m_position.x + 689;
				float progNamesY = m_position.y + 242;
				D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 30.0f);
				D2DRenderer::Get().DrawMessage(L"��ΰ�\n�����\n��ȿ��\n�����", progNamesX, progNamesY, 100, 30.0f, D2D1::ColorF(0xffe6d8));
		}
}

void CreditWindow::RenderCredtBG()
{
		if (m_uiRenderer)
		{
				ID2D1Bitmap1* bg = m_uiRenderer->GetBitmap("creditBG").Get();

				D2D1_RECT_F destRect1 = {
			m_position.x + 535.0f - 431.0f,
			m_position.y + 467.0f - 270.0f,
			m_position.x + 535.0f - 431.0f + 335.0f,
			m_position.y + 467.0f - 270.0f + 283.0f
				};
				D2DRenderer::Get().DrawBitmap(bg, destRect1);


				D2D1_RECT_F destRect2 = {
						m_position.x + 999.0f - 431.0f,
						m_position.y + 467.0f - 270.0f,
						m_position.x + 999.0f - 431.0f + 335.0f,
						m_position.y + 467.0f - 270.0f + 283.0f
				};
				D2DRenderer::Get().DrawBitmap(bg, destRect2);

		}
}
