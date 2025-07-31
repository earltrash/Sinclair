#include "UI_Renderer.h"



UI_Renderer::UI_Renderer(ComPtr<ID2D1Bitmap1> bitmap)
{
		m_sheet = std::move(bitmap);
}

UI_Renderer::~UI_Renderer()
{
	//bitmap은 알아서 소멸 ㅇㅇ 
}

void UI_Renderer::SetBitmap(ComPtr<ID2D1Bitmap1> bitmap) 
{
		m_sheet = bitmap;
}

const ComPtr<ID2D1Bitmap1>& UI_Renderer::GetBitmap() const  
{
		if(m_sheet != nullptr)
		return m_sheet;
}


