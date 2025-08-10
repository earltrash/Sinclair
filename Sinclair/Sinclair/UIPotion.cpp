#include "UIPotion.h"
#include "UI_Bank.h"
#include "UI_Renderer.h"
#include "ResourceManager.h"
#include "GameManager.h"

UIPotion::UIPotion() : UIWindow(UIWindowType::StatPotionUseWindow, Vec2{ 0,0 }, Vec2{ 524,766 })  // Vec2{ 1097,766 }) 
{
	//포지션은 불러오는 쪽에서 업데이트 시킴.

	//비트맵은 일단 컴포넌트에서 다 받아오게 시킴
	//
	InitializeMem(); //버튼 위치, 나가기 버튼, 타이틀 바도 필요하긴 함. //슬롯 위치도
	LoadUIBitmaps(); //버튼, bg, 나가기 버튼, etc 

	m_PotionLevel = PotionLevel::level3; //일단 암것도 아니야 
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



		//끔찍한 하드 코딩 의 현장 

			destRect = { m_position.x + m_button[0].x ,m_position.y + m_button[0].y,
				m_position.x + m_button[0].x + ButtonX, m_position.y + m_button[0].y + ButtonY };

			ID2D1Bitmap1* STRBUT = m_str_bitmap[static_cast<int>(GetLevel())].Get();

			D2D1_SIZE_F size = STRBUT->GetSize();

			D2D1_RECT_F srect;
			srect.left = 0;
			srect.top = 0;
			srect.right = static_cast<LONG>(size.width);  // D2D1_SIZE_F는 float 타입이므로 LONG으로 형변환
			srect.bottom = static_cast<LONG>(size.height);

			D2DRenderer::Get().DrawBitmap(STRBUT, destRect, srect, m_But_opacity[0]);

			destRect = { m_position.x + m_button[1].x ,m_position.y + m_button[1].y,
				m_position.x + m_button[1].x + ButtonX, m_position.y + m_button[1].y + ButtonY };

			ID2D1Bitmap1* DEXBUT = m_dex_bitmap[static_cast<int>(GetLevel())].Get();

			size = DEXBUT->GetSize();

			srect;
			srect.right = static_cast<LONG>(size.width);  // D2D1_SIZE_F는 float 타입이므로 LONG으로 형변환
			srect.bottom = static_cast<LONG>(size.height);


			D2DRenderer::Get().DrawBitmap(DEXBUT, destRect, srect,m_But_opacity[1]);

			destRect = { m_position.x + m_button[2].x ,m_position.y + m_button[2].y,
				m_position.x + m_button[2].x + ButtonX, m_position.y + m_button[2].y + ButtonY };

			ID2D1Bitmap1* INTBUT = m_int_bitmap[static_cast<int>(GetLevel())].Get();

			size = INTBUT->GetSize();

			srect;
			srect.right = static_cast<LONG>(size.width);  // D2D1_SIZE_F는 float 타입이므로 LONG으로 형변환
			srect.bottom = static_cast<LONG>(size.height);


			D2DRenderer::Get().DrawBitmap(INTBUT, destRect, srect, m_But_opacity[2]);



			destRect = { m_position.x + m_button[3].x ,m_position.y + m_button[3].y,
				m_position.x + m_button[3].x + ButtonX, m_position.y + m_button[3].y + ButtonY };

			ID2D1Bitmap1* LUKBUT = m_luk_bitmap[static_cast<int>(GetLevel())].Get();


			size = LUKBUT->GetSize();

			srect;
			srect.right = static_cast<LONG>(size.width);  // D2D1_SIZE_F는 float 타입이므로 LONG으로 형변환
			srect.bottom = static_cast<LONG>(size.height);


			D2DRenderer::Get().DrawBitmap(DEXBUT, destRect, srect, m_But_opacity[3]);

	}

}

bool UIPotion::HandleMouseHover(Vec2 mousePos) //버튼 위치에 있는 경우에는 opacity를 수정해야 함.
{
	if (!m_isActive) return false; //해당 버튼의 위치가 vector의 위치 영역 안에 들어 온 경우 

	UpdateSlotPositions();


	for (int i = 0; i < m_button.size(); i++)
	{
		if (InButton(m_button[i], mousePos)) //Opacity를 알아서 처리해야 함. 
		{
			m_But_opacity[i] = 0.5f;

			return true; //투명도를 조정해야 한다네요 -> 그럼 비트맵을 다르게 해야 하나 하하 

		}

		else {
			m_But_opacity[i] = 1.f;
		}

	}
	return false;
}

bool UIPotion::HandleMouseDown(Vec2 mousePos)
{
	if (!m_isActive) return false; //해당 버튼의 위치가 vector의 위치 영역 안에 들어 온 경우 

	for (int i=0; i<m_button.size(); i++)
	{
		if (InButton(m_button[i], mousePos))
		{
			//1번 인덱스에서 그랬다면,  str

			m_Stat = static_cast<Status_fundamental>(i); // 인덱스가 스텟임. 
			int Much = static_cast<int>(GetLevel()) +1 ; 

			//이대로 게임 매니져한테 보내주던가 //스탯이랑 수치 
			std::cout << " 스탯 인덱스 " << static_cast<int>(m_Stat) << "수치 " << Much << endl;


			//게임 매니져 한테, 타입으로 주고, 

			//더블클릭 된 아이템의 수명 주기를 여기서 해결한다. 
			//합성 및 강화로 인한 아이템 삭제를 처리 해야하긴 할 듯. 
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
	//자체적인 포지션도 여기서 해결해야 하고 버튼 위치인 경우에는 

	for (auto pos : m_button)
	{
		pos.x = m_position.x + pos.x;
		pos.y = m_position.y + pos.y;
	}
}
