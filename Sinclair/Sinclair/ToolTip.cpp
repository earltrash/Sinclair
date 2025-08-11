#include "ToolTip.h"
#include "UI_Renderer.h"
//#include "ResourceManager.h"
#include "CursorManager.h"
#include "Wearable.h"
#include "ItemBank.h"
//�������ͽ� 128 122
//���� 30 222
//�̹��� 31 120
//��ȭ �ռ� ���� 81 61
// ������ �� -> ��� ���� ��Ű�� 

ToolTip::ToolTip() : UIWindow(UIWindowType::InventoryTooltip,{0,0}, {234,345})
{
	MemInit();
	//��ġ���� 0,0���� ��ٰ� ���콺 �� �޾Ƽ� Update��Ű�� �ŷ� ���� ���� 
	LoadUIBitmaps();
}

void ToolTip::Update()
{
	//���� ���� 
}

void ToolTip::Render()
{
	if (!m_isActive) return;


	UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();

	if (uiRenderer) //���
	{
		D2D1_RECT_F destRect = { m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y };

		ID2D1Bitmap1* backgroundBitmap = uiRenderer->GetBitmap("Tool_Tip_Bg").Get();
		if (backgroundBitmap)
		{
			D2DRenderer::Get().DrawBitmap(backgroundBitmap, destRect);
		}

	
		Item* item = CursorManager::Get().GetHoveredItem(); //�̰� ���� �ٸ� �� �����;� ��. 

		if (item != nullptr) //�ȵ� ���� ������!! 
		{
			TextRender(item);
			BitmapRender(item);
		}


	}
}

bool ToolTip::HandleMouseHover(Vec2 mousePos) //�ʱⰪ�� �������� hover�� ���� ���� ������Ʈ �ؾ� �ڳ� 
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


	//�������� ���� ��ǥ�� �и��߾��.. 
	m_enchancer_Pos = m_enchancer_PosBase;
	m_text_pos = m_text_posBase;

}

void ToolTip::DrawMessageCenterHorizontal(const std::wstring& text, float topY, float fontSize, const std::wstring& fontName, const D2D1::ColorF& color)
{
	auto textSize = D2DRenderer::Get().MeasureText(text, fontSize, fontName);
	float textWidth = textSize.width;

	// �߾� ���� X ��ǥ ���
	float centerX = m_position.x + (m_size.x - textWidth) / 2.0f;

	// ��Ʈ ���ҽ� ���� (��Ʈ �̸��� ũ�� �ݿ�)
	D2DRenderer::Get().CreateWriteResource(fontName.c_str(), DWRITE_FONT_WEIGHT_NORMAL, fontSize);

	// ���� �׸���
	D2DRenderer::Get().DrawMessage(
		text.c_str(),
		centerX,
		topY,
		textWidth,  // width�� "����"��
		fontSize,   // height�� ��Ʈ ũ�� ���
		color
	);
}



void ToolTip::LoadUIBitmaps()
{


	UI_Renderer* uiRenderer = AddComponent<UI_Renderer>();
	if (uiRenderer)
	{

		m_enchacer_bitmap.resize(static_cast<int>(EnchancerType::Nothing)); //�ִ� �ε��� ũ����..

		// �ʿ��� UI ��Ʈ�ʵ��� ResourceManager���� �ε��Ͽ� UI_Renderer�� ����
		// ResourceManager�� �ش� Ű�� ��Ʈ���� �ε�
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
	wstring Ehchan = (item->m_data.enchantable ? StrToWstr("��ȭ ����") : StrToWstr("��ȭ �Ұ���"));
	wstring Syn = (item->m_data.synthesizable ? StrToWstr("�ռ� ����") : StrToWstr("�ռ� �Ұ���"));
	wstring des = UTF8ToWstr(item->m_data.description);

	Wearable* wear = dynamic_cast<Wearable*>(item);

	if (wear != nullptr)
	{
		wstring TotalStr = StrToWstr("STR: " + std::to_string(wear->GetStat(Status_fundamental::power)));
		wstring TotalDex = StrToWstr("DEX: " + std::to_string(wear->GetStat(Status_fundamental::agile)));
		wstring TotalInt = StrToWstr("INT: " + std::to_string(wear->GetStat(Status_fundamental::intelligence)));
		wstring TotalLuk = StrToWstr("LUK: " + std::to_string(wear->GetStat(Status_fundamental::luck)));

		// �����۸� (���� �߾� ����)


	    DrawMessageCenterHorizontal(ItemNm, m_text_pos[TextType::ItemNm].y, PtToPx(15), L"���� ����� Bold", D2D1::ColorF::White);

		// ��ȭ / �ռ�
		DrawMessageCenterHorizontal(Ehchan, m_text_pos[TextType::Enchancable].y, PtToPx(12.36f), L"���� ����� Bold", D2D1::ColorF::Orange);
		DrawMessageCenterHorizontal(Syn, m_text_pos[TextType::Synthesisable].y, PtToPx(12.36f), L"���� ����� Bold", D2D1::ColorF::Orange);

		


		//��� ������ �ϴ� �ϵ��ڵ� �������� 

		D2DRenderer::Get().DrawMessage(
			des.c_str(),
			m_text_pos[TextType::DES].x,                        // ���� ����
			m_text_pos[TextType::DES].y,                 // ���� Y
			m_size.x - 80.0f,                            // �ؽ�Ʈ ���� �ʺ� (�¿� ���� ���)
			m_size.y,                                    // ����� ū ����
			D2D1::ColorF::White);


		// ���� (���� ���� �״��)
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, PtToPx(12.07f));
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
		D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, PtToPx(12.07f));

		D2DRenderer::Get().DrawMessage(
			des.c_str(),
			m_text_pos[TextType::DES_N].x,                        // ���� ����
			m_text_pos[TextType::DES_N].y,                 // ���� Y
			m_size.x - 80.0f,                            // �ؽ�Ʈ ���� �ʺ� (�¿� ���� ���)
			m_size.y,                                    // ����� ū ����
			D2D1::ColorF::White);

		//DrawMessageCenterHorizontal(des.c_str(), m_text_pos[TextType::DES_N].y, PtToPx(12.07f), L"���� ����� Bold", D2D1::ColorF::White);

		// ����� ������ ó��
		DrawMessageCenterHorizontal(ItemNm, m_text_pos[TextType::ItemNm].y, PtToPx(15), L"���� ����� Bold", D2D1::ColorF::White);
		//DrawMessageCenterHorizontal(des, m_text_pos[TextType::DES].y, PtToPx(12), L"���� ����� Bold", D2D1::ColorF::White);
	}



}

void ToolTip::BitmapRender(Item*& item)
{
	UI_Renderer* uiRenderer = GetComponent<UI_Renderer>();
	Wearable* wear = dynamic_cast<Wearable*>(item);

	if (uiRenderer == nullptr) return;


	if (wear != nullptr )
	{
		//�ֹ��� ��Ʈ�� ������ ��ġ�� �����;� �ϰ� 
		// 
		D2D1_RECT_F destRect = { m_position.x + 25.0f, m_position.y + 128.0f,
			 m_position.x + 25.0f + ToolTipSize, m_position.y + 128.0f + ToolTipSize };

		D2DRenderer::Get().DrawBitmap(uiRenderer->GetBitmap("Tool_Tip_Slot").Get(), destRect);

		D2DRenderer::Get().DrawBitmap(ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id)->atlas.Get() //��Ʈ�� ������ �����ϱ� ���� 
			,destRect, ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id)->srcRect, 1.0f);


			for (int i = 0; i < wear->GetEnchancResult().size(); i++) //Ƚ���� �����̶� ����? �;��� iterator�� 
			{
				destRect = (wear->Getpart() != Wearable_part::Weapon) ? //����� 
					
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
				//������ ��ũ���� �������� �ű��ѵ�, �Ƹ� ����� �Ű�, ������ ���� ó���ϴ��� �ؾ��� �� 
			}
	
		

	}
	else //�����̶� ��ῡ ���� ����
	{
		//234,345 //100 134 67

		D2D1_RECT_F destRect = { m_position.x + 67.0f, m_position.y + 93.0f,
			 m_position.x + 67.0f + ToolTipSize, m_position.y + 93.0f + ToolTipSize };


		D2DRenderer::Get().DrawBitmap(uiRenderer->GetBitmap("Tool_Tip_Slot").Get(), destRect);
		D2DRenderer::Get().DrawBitmap(ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id)->atlas.Get()
			, destRect, ResourceManager::Get().Get_ItemBank().GetItemClip(item->m_data.id)->srcRect, 1.0f);
	}



}


