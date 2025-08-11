#include "ToolTip.h"
#include "UI_Renderer.h"
//#include "ResourceManager.h"
#include "CursorManager.h"
#include "Wearable.h"
#include "ItemBank.h"
//스테이터스 128 122
//설명 30 222
//이미지 31 120
//강화 합성 가능 81 61
// 아이템 명 -> 가운데 정렬 시키기 

ToolTip::ToolTip() : UIWindow(UIWindowType::InventoryTooltip,{0,0}, {234,345})
{
	MemInit();
	//위치값은 0,0으로 잡다가 마우스 값 받아서 Update시키는 거로 하자 ㅇㅇ 
	LoadUIBitmaps();
}

void ToolTip::Update()
{
	//폐지 예정 
}

void ToolTip::Render()
{
	if (!m_isActive) return;


	UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();

	if (uiRenderer) //배경
	{
		D2D1_RECT_F destRect = { m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y };

		ID2D1Bitmap1* backgroundBitmap = uiRenderer->GetBitmap("Tool_Tip_Bg").Get();
		if (backgroundBitmap)
		{
			D2DRenderer::Get().DrawBitmap(backgroundBitmap, destRect);
		}

	
		Item* item = CursorManager::Get().GetHoveredItem(); //이거 말고 다른 거 가져와야 함. 

		if (item != nullptr) //안될 리가 없음요!! 
		{
			TextRender(item);
			BitmapRender(item);
		}


	}
}

bool ToolTip::HandleMouseHover(Vec2 mousePos) //초기값은 해주지만 hover될 때는 값도 업데이트 해야 겠네 
{
	if (!m_isActive)
	{
		// showTooltip = false;
		return false;
	}

	UpdatePositions();
	//return true;
	return false;
}

bool ToolTip::HandleMouseDown(Vec2 mousePos)
{
	return false;
}

bool ToolTip::HandleMouseUp(Vec2 mousePos)
{
	return false;
}

bool ToolTip::HandleDoubleClick(Vec2 mousePos)
{
	return true;
}

bool ToolTip::HandleMouseRight(Vec2 mousePos)
{
	return false;
}

bool ToolTip::HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source)
{
	return false;
}

void ToolTip::UpdatePositions()
{
	for (int i = 0; i < m_enchancer_PosBase.size(); ++i)
	{
		m_enchancer_Pos[i].x = m_position.x + m_enchancer_PosBase[i].x;
		m_enchancer_Pos[i].y = m_position.y + m_enchancer_PosBase[i].y;
	}

	for (auto& [type, basePos] : m_text_posBase)
	{
		m_text_pos[type].x = m_position.x + basePos.x;
		m_text_pos[type].y = m_position.y + basePos.y;
	}


}

UIWindowType ToolTip::GetType()
{
	return UIWindowType::InventoryTooltip;
}

void ToolTip::MemInit()
{

	m_enchancer_PosBase = {
	   { 38.0f, 65 }, { 73.0f, 65 }, { 108.0f, 65 }, { 143.0f, 65 }, { 178.0f, 65 }
	};

	m_text_posBase[TextType::ItemNm] = { 0.f, 35.7f };
	m_text_posBase[TextType::Enchancable] = { 0.f, 94.88f };
	m_text_posBase[TextType::Synthesisable] = { 0.f, 111.77f };
	m_text_posBase[TextType::STR] = { 130.61f, 144.94f };
	m_text_posBase[TextType::DEX] = { 130.61f, 161.04f };
	m_text_posBase[TextType::INT] = { 130.61f, 177.14f };
	m_text_posBase[TextType::LUK] = { 130.61f, 193.24f };
	m_text_posBase[TextType::DES] = { 41.0f, 236.0f };
	m_text_posBase[TextType::DES_N] = { 41.0f, 236.0f };


	//기준점과 렌더 좌표를 분리했어요.. 
	m_enchancer_Pos = m_enchancer_PosBase;
	m_text_pos = m_text_posBase;

}

void ToolTip::DrawMessageCenterHorizontal(const std::wstring& text, float topY, float fontSize, const std::wstring& fontName, const D2D1::ColorF& color)
{
	auto textSize = D2DRenderer::Get().MeasureText(text, fontSize, fontName);
	float textWidth = textSize.width;

	// 중앙 정렬 X 좌표 계산
	float centerX = m_position.x + (m_size.x - textWidth) / 2.0f;

	// 폰트 리소스 생성 (폰트 이름과 크기 반영)
	D2DRenderer::Get().CreateWriteResource(fontName.c_str(), DWRITE_FONT_WEIGHT_NORMAL, fontSize);

	// 실제 그리기
	D2DRenderer::Get().DrawMessage(
		text.c_str(),
		centerX,
		topY,
		textWidth,  // width는 "길이"로
		fontSize,   // height는 폰트 크기 기반
		color
	);
}



void ToolTip::LoadUIBitmaps()
{


	UI_Renderer* uiRenderer = AddComponent<UI_Renderer>();
	if (uiRenderer)
	{

		m_enchacer_bitmap.resize(static_cast<int>(EnchancerType::Nothing)); //최대 인덱스 크기임..

		// 필요한 UI 비트맵들을 ResourceManager에서 로드하여 UI_Renderer에 설정
		// ResourceManager가 해당 키로 비트맵을 로드
		uiRenderer->SetBitmap("Tool_Tip_Bg", ResourceManager::Get().GetTexture("ToolTipBg"));
		uiRenderer->SetBitmap("Tool_Tip_Slot", ResourceManager::Get().GetTexture("ToolTipSlot"));

	
	
		m_enchacer_bitmap[static_cast<int>(EnchancerType::Default)] = ResourceManager::Get().GetTexture("ToolTipD");
		m_enchacer_bitmap[static_cast<int>(EnchancerType::Fail)] = ResourceManager::Get().GetTexture("ToolTipF");
		m_enchacer_bitmap[static_cast<int>(EnchancerType::Sucess)] = ResourceManager::Get().GetTexture("ToolTipS");


	}

}

void ToolTip::TextRender(Item*& item)
{
	wstring ItemNm = UTF8ToWstr(item->m_data.name);
	wstring Ehchan = (item->m_data.enchantable ? StrToWstr("강화 가능") : StrToWstr("강화 불가능"));
	wstring Syn = (item->m_data.synthesizable ? StrToWstr("합성 가능") : StrToWstr("합성 불가능"));
	wstring des = UTF8ToWstr(item->m_data.description);

	Wearable* wear = dynamic_cast<Wearable*>(item);

	if (wear != nullptr)
	{
		wstring TotalStr = StrToWstr("STR: " + std::to_string(wear->GetStat(Status_fundamental::power)));
		wstring TotalDex = StrToWstr("DEX: " + std::to_string(wear->GetStat(Status_fundamental::agile)));
		wstring TotalInt = StrToWstr("INT: " + std::to_string(wear->GetStat(Status_fundamental::intelligence)));
		wstring TotalLuk = StrToWstr("LUK: " + std::to_string(wear->GetStat(Status_fundamental::luck)));

		// 아이템명 (가로 중앙 정렬)


	    DrawMessageCenterHorizontal(ItemNm, m_text_pos[TextType::ItemNm].y, PtToPx(15), L"빛의 계승자 Bold", D2D1::ColorF::White);

		// 강화 / 합성
		DrawMessageCenterHorizontal(Ehchan, m_text_pos[TextType::Enchancable].y, PtToPx(12.36f), L"빛의 계승자 Bold", D2D1::ColorF::Orange);
		DrawMessageCenterHorizontal(Syn, m_text_pos[TextType::Synthesisable].y, PtToPx(12.36f), L"빛의 계승자 Bold", D2D1::ColorF::Orange);

		


		//얘랑 제목은 일단 하드코딩 때려놓음 

		D2DRenderer::Get().DrawMessage(
			des.c_str(),
			m_text_pos[TextType::DES].x,                        // 좌측 여백
			m_text_pos[TextType::DES].y,                 // 시작 Y
			m_size.x - 80.0f,                            // 텍스트 영역 너비 (좌우 여백 고려)
			m_size.y,                                    // 충분히 큰 높이
			D2D1::ColorF::White);


		// 스탯 (좌측 정렬 그대로)
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, PtToPx(12.07f));
		D2DRenderer::Get().DrawMessage(TotalStr.c_str(), m_text_pos[TextType::STR].x, m_text_pos[TextType::STR].y,
			m_text_pos[TextType::STR].x + m_size.x, m_text_pos[TextType::STR].y + PtToPx(12.07f), D2D1::ColorF::White);
		D2DRenderer::Get().DrawMessage(TotalDex.c_str(), m_text_pos[TextType::DEX].x, m_text_pos[TextType::DEX].y,
			m_text_pos[TextType::DEX].x + m_size.x, m_text_pos[TextType::DEX].y + PtToPx(12.07f), D2D1::ColorF::White);
		D2DRenderer::Get().DrawMessage(TotalInt.c_str(), m_text_pos[TextType::INT].x, m_text_pos[TextType::INT].y,
			m_text_pos[TextType::INT].x + m_size.x, m_text_pos[TextType::INT].y + PtToPx(12.07f), D2D1::ColorF::White);
		D2DRenderer::Get().DrawMessage(TotalLuk.c_str(), m_text_pos[TextType::LUK].x, m_text_pos[TextType::LUK].y,
			m_text_pos[TextType::LUK].x + m_size.x, m_text_pos[TextType::LUK].y + PtToPx(12.07f), D2D1::ColorF::White);
	}
	else
	{
		D2DRenderer::Get().CreateWriteResource(L"빛의 계승자 Bold", DWRITE_FONT_WEIGHT_BOLD, PtToPx(12.07f));

		D2DRenderer::Get().DrawMessage(
			des.c_str(),
			m_text_pos[TextType::DES_N].x,                        // 좌측 여백
			m_text_pos[TextType::DES_N].y,                 // 시작 Y
			m_size.x - 80.0f,                            // 텍스트 영역 너비 (좌우 여백 고려)
			m_size.y,                                    // 충분히 큰 높이
			D2D1::ColorF::White);

		//DrawMessageCenterHorizontal(des.c_str(), m_text_pos[TextType::DES_N].y, PtToPx(12.07f), L"빛의 계승자 Bold", D2D1::ColorF::White);

		// 비장비 아이템 처리
		DrawMessageCenterHorizontal(ItemNm, m_text_pos[TextType::ItemNm].y, PtToPx(15), L"빛의 계승자 Bold", D2D1::ColorF::White);
		//DrawMessageCenterHorizontal(des, m_text_pos[TextType::DES].y, PtToPx(12), L"빛의 계승자 Bold", D2D1::ColorF::White);
	}



}

void ToolTip::BitmapRender(Item*& item)
{
	UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();
	Wearable* wear = dynamic_cast<Wearable*>(item);

	if (uiRenderer == nullptr) return;


	if (wear != nullptr )
	{
		//주문서 비트맵 갯수랑 위치를 가져와야 하고 
		// 
		D2D1_RECT_F destRect = { m_position.x + 25.0f, m_position.y + 128.0f,
			 m_position.x + 25.0f + ToolTipSize, m_position.y + 128.0f + ToolTipSize };

		D2DRenderer::Get().DrawBitmap(uiRenderer->GetBitmap("Tool_Tip_Slot").Get(), destRect);

		D2DRenderer::Get().DrawBitmap(ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id)->atlas.Get() //비트맵 정도야 가능하긴 하지 
			,destRect, ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id)->srcRect, 1.0f);


			for (int i = 0; i < wear->GetEnchancResult().size(); i++) //횟수가 고정이라 굳이? 싶었음 iterator는 
			{
				destRect = (wear->Getpart() != Wearable_part::Weapon) ? //무기면 
					
					D2D1::RectF(
						m_enchancer_Pos[1 + i].x, m_enchancer_Pos[1 + i].y,
						m_enchancer_Pos[1 + i].x + 18.0f, m_enchancer_Pos[1 + i].y + 19.0f
					)

					: 

					D2D1::RectF(
						m_enchancer_Pos[i].x, m_enchancer_Pos[i].y,
						m_enchancer_Pos[i].x + 18.0f, m_enchancer_Pos[i].y + 19.0f
					);
			//	wear->GetEnchancResult().at(i) = EnchancerType::Fail;
				D2DRenderer::Get().DrawBitmap(m_enchacer_bitmap[static_cast<int>(wear->GetEnchancResult().at(i))].Get(), destRect);
				//아이템 뱅크에서 가져오는 거긴한데, 아마 저장될 거고, 물약은 따로 처리하던가 해야할 듯 
			}
	
		

	}
	else //포션이랑 재료에 대한 툴팁
	{
		//234,345 //100 134 67

		D2D1_RECT_F destRect = { m_position.x + 67.0f, m_position.y + 93.0f,
			 m_position.x + 67.0f + ToolTipSize, m_position.y + 93.0f + ToolTipSize };


		D2DRenderer::Get().DrawBitmap(uiRenderer->GetBitmap("Tool_Tip_Slot").Get(), destRect);
		D2DRenderer::Get().DrawBitmap(ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id)->atlas.Get()
			, destRect, ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id)->srcRect, 1.0f);
	}



}


