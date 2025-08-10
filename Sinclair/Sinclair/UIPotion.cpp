#include "UIPotion.h"
#include "UI_Bank.h"
#include "UI_Renderer.h"
#include "ResourceManager.h"
#include "GameManager.h"

UIPotion::UIPotion() : UIWindow(UIWindowType::StatPotionUseWindow, Vec2{ 0,0 }, Vec2{ 524,766 })  // Vec2{ 1097,766 }) 
{
	//�������� �ҷ����� �ʿ��� ������Ʈ ��Ŵ.

	//��Ʈ���� �ϴ� ������Ʈ���� �� �޾ƿ��� ��Ŵ
	//
	InitializeMem(); //��ư ��ġ, ������ ��ư, Ÿ��Ʋ �ٵ� �ʿ��ϱ� ��. //���� ��ġ��
	LoadUIBitmaps(); //��ư, bg, ������ ��ư, etc 

	m_PotionLevel = PotionLevel::level3; //�ϴ� �ϰ͵� �ƴϾ� 
}

void UIPotion::Update()
{
}

void UIPotion::Render()
{
	UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();
	if (uiRenderer)
	{
		ComPtr<ID2D1Bitmap1> backgroundBitmap = m_BG_BITMAP[static_cast<int>(GetLevel()) ];
		if (backgroundBitmap)
		{
			D2D1_RECT_F destRect = { m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y };

			D2DRenderer::Get().DrawBitmap(backgroundBitmap.Get(), destRect);

		}

		float rightMargin = 66.0f; //524-458
		Vec2 closeButtonPos = { m_position.x + m_size.x - rightMargin, m_position.y + 36 };
		Vec2 closeButtonSize = { 35.0f, 35.0f };
		D2D1_RECT_F destRect = { closeButtonPos.x, closeButtonPos.y, closeButtonPos.x + closeButtonSize.x, closeButtonPos.y + closeButtonSize.y };

		ID2D1Bitmap1* closeButtonBitmap = uiRenderer->GetBitmap("Close_Button").Get();
		if (closeButtonBitmap)
		{
			D2DRenderer::Get().DrawBitmap(closeButtonBitmap, destRect);
		}



		//������ �ϵ� �ڵ� �� ���� 

			destRect = { m_position.x + m_button[0].x ,m_position.y + m_button[0].y,
				m_position.x + m_button[0].x + ButtonX, m_position.y + m_button[0].y + ButtonY };

			ID2D1Bitmap1* STRBUT = m_str_bitmap[static_cast<int>(GetLevel())].Get();

			D2D1_SIZE_F size = STRBUT->GetSize();

			D2D1_RECT_F srect;
			srect.left = 0;
			srect.top = 0;
			srect.right = static_cast<LONG>(size.width);  // D2D1_SIZE_F�� float Ÿ���̹Ƿ� LONG���� ����ȯ
			srect.bottom = static_cast<LONG>(size.height);

			D2DRenderer::Get().DrawBitmap(STRBUT, destRect, srect, m_But_opacity[0]);

			destRect = { m_position.x + m_button[1].x ,m_position.y + m_button[1].y,
				m_position.x + m_button[1].x + ButtonX, m_position.y + m_button[1].y + ButtonY };

			ID2D1Bitmap1* DEXBUT = m_dex_bitmap[static_cast<int>(GetLevel())].Get();

			size = DEXBUT->GetSize();

			srect;
			srect.right = static_cast<LONG>(size.width);  // D2D1_SIZE_F�� float Ÿ���̹Ƿ� LONG���� ����ȯ
			srect.bottom = static_cast<LONG>(size.height);


			D2DRenderer::Get().DrawBitmap(DEXBUT, destRect, srect,m_But_opacity[1]);

			destRect = { m_position.x + m_button[2].x ,m_position.y + m_button[2].y,
				m_position.x + m_button[2].x + ButtonX, m_position.y + m_button[2].y + ButtonY };

			ID2D1Bitmap1* INTBUT = m_int_bitmap[static_cast<int>(GetLevel())].Get();

			size = INTBUT->GetSize();

			srect;
			srect.right = static_cast<LONG>(size.width);  // D2D1_SIZE_F�� float Ÿ���̹Ƿ� LONG���� ����ȯ
			srect.bottom = static_cast<LONG>(size.height);


			D2DRenderer::Get().DrawBitmap(INTBUT, destRect, srect, m_But_opacity[2]);



			destRect = { m_position.x + m_button[3].x ,m_position.y + m_button[3].y,
				m_position.x + m_button[3].x + ButtonX, m_position.y + m_button[3].y + ButtonY };

			ID2D1Bitmap1* LUKBUT = m_luk_bitmap[static_cast<int>(GetLevel())].Get();


			size = LUKBUT->GetSize();

			srect;
			srect.right = static_cast<LONG>(size.width);  // D2D1_SIZE_F�� float Ÿ���̹Ƿ� LONG���� ����ȯ
			srect.bottom = static_cast<LONG>(size.height);


			D2DRenderer::Get().DrawBitmap(DEXBUT, destRect, srect, m_But_opacity[3]);

	}

}

bool UIPotion::HandleMouseHover(Vec2 mousePos) //��ư ��ġ�� �ִ� ��쿡�� opacity�� �����ؾ� ��.
{
	if (!m_isActive) return false; //�ش� ��ư�� ��ġ�� vector�� ��ġ ���� �ȿ� ��� �� ��� 

	UpdateSlotPositions();


	for (int i = 0; i < m_button.size(); i++)
	{
		if (InButton(m_button[i], mousePos)) //Opacity�� �˾Ƽ� ó���ؾ� ��. 
		{
			m_But_opacity[i] = 0.5f;

			return true; //������ �����ؾ� �Ѵٳ׿� -> �׷� ��Ʈ���� �ٸ��� �ؾ� �ϳ� ���� 

		}

		else {
			m_But_opacity[i] = 1.f;
		}

	}
	return false;
}

bool UIPotion::HandleMouseDown(Vec2 mousePos)
{
	if (!m_isActive) return false; //�ش� ��ư�� ��ġ�� vector�� ��ġ ���� �ȿ� ��� �� ��� 

	for (int i=0; i<m_button.size(); i++)
	{
		if (InButton(m_button[i], mousePos))
		{
			//1�� �ε������� �׷��ٸ�,  str

			m_Stat = static_cast<Status_fundamental>(i); // �ε����� ������. 
			int Much = static_cast<int>(GetLevel()) +1 ; 

			//�̴�� ���� �Ŵ������� �����ִ��� //�����̶� ��ġ 
			std::cout << " ���� �ε��� " << static_cast<int>(m_Stat) << "��ġ " << Much << endl;


			//���� �Ŵ��� ����, Ÿ������ �ְ�, 

			//����Ŭ�� �� �������� ���� �ֱ⸦ ���⼭ �ذ��Ѵ�. 
			//�ռ� �� ��ȭ�� ���� ������ ������ ó�� �ؾ��ϱ� �� ��. 
			return true;

		}
	}

	return false;
}

bool UIPotion::HandleMouseUp(Vec2 mousePos)
{
	return false;
}

bool UIPotion::HandleDoubleClick(Vec2 mousePos)
{
	return false;
}

bool UIPotion::HandleMouseRight(Vec2 mousePos)
{
	return false;
}

void UIPotion::LoadUIBitmaps()
{

	m_BG_BITMAP.resize(static_cast<int>(PotionLevel::nothing));

	m_str_bitmap.resize(static_cast<int>(PotionLevel::nothing));
	m_dex_bitmap.resize(static_cast<int>(PotionLevel::nothing));
	m_int_bitmap.resize(static_cast<int>(PotionLevel::nothing));
	m_luk_bitmap.resize(static_cast<int>(PotionLevel::nothing));


	auto* uiRenderer = AddComponent<UI_Renderer>();

	uiRenderer->SetBitmap("Close_Button", ResourceManager::Get().GetTexture("CloseButton"));

	m_BG_BITMAP[static_cast<int>(PotionLevel::level1)] = ResourceManager::Get().GetTexture("Potion0");
	m_BG_BITMAP[static_cast<int>(PotionLevel::level2)] = ResourceManager::Get().GetTexture("Potion1");
	m_BG_BITMAP[static_cast<int>(PotionLevel::level3)] = ResourceManager::Get().GetTexture("Potion2");


	m_str_bitmap[static_cast<int>(PotionLevel::level1)] = ResourceManager::Get().GetTexture("P0BS");
	m_dex_bitmap[static_cast<int>(PotionLevel::level1)] = ResourceManager::Get().GetTexture("P0BD");
	m_int_bitmap[static_cast<int>(PotionLevel::level1)] = ResourceManager::Get().GetTexture("P0BI");
	m_luk_bitmap[static_cast<int>(PotionLevel::level1)] = ResourceManager::Get().GetTexture("P0BL");

	m_str_bitmap[static_cast<int>(PotionLevel::level2)] = ResourceManager::Get().GetTexture("P1BS");
	m_dex_bitmap[static_cast<int>(PotionLevel::level2)] = ResourceManager::Get().GetTexture("P1BD");
	m_int_bitmap[static_cast<int>(PotionLevel::level2)] = ResourceManager::Get().GetTexture("P1BI");
	m_luk_bitmap[static_cast<int>(PotionLevel::level2)] = ResourceManager::Get().GetTexture("P1BL");

	m_str_bitmap[static_cast<int>(PotionLevel::level3)] = ResourceManager::Get().GetTexture("P2BS");
	m_dex_bitmap[static_cast<int>(PotionLevel::level3)] = ResourceManager::Get().GetTexture("P2BD");
	m_int_bitmap[static_cast<int>(PotionLevel::level3)] = ResourceManager::Get().GetTexture("P2BI");
	m_luk_bitmap[static_cast<int>(PotionLevel::level3)] = ResourceManager::Get().GetTexture("P2BL");

}

void UIPotion::InitializeMem()
{
	m_button.resize(static_cast<int>(PotionStat::nothing));

	m_But_opacity.assign(static_cast<int>(PotionStat::nothing), 1);

	m_button[static_cast<int>(PotionStat::STR)] = { 51, 350 };
	m_button[static_cast<int>(PotionStat::DEX)] = { 51, 433 };
	m_button[static_cast<int>(PotionStat::INT)] = { 51, 517 };
	m_button[static_cast<int>(PotionStat::LUK)] = { 51, 600 };
}

UIWindowType UIPotion::GetType()
{
	return m_windowType;
}

bool UIPotion::HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source)
{
	return false;
}

bool UIPotion::InButton(Vec2 Butpos, Vec2 mousePos)
{
	return mousePos.x >= Butpos.x && mousePos.x <= Butpos.x + m_size.x &&
		mousePos.y >= Butpos.y && mousePos.y <= Butpos.y + 42;
}

void UIPotion::SetLevel(PotionLevel level)
{
	m_PotionLevel = level;
}

PotionLevel UIPotion::GetLevel()
{
	return m_PotionLevel;
}

void UIPotion::UpdateSlotPositions()
{
	//��ü���� �����ǵ� ���⼭ �ذ��ؾ� �ϰ� ��ư ��ġ�� ��쿡�� 

	for (auto pos : m_button)
	{
		pos.x = m_position.x + pos.x;
		pos.y = m_position.y + pos.y;
	}
}
