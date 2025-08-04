#include "UI_Renderer.h"


void UI_Renderer::SetBitmap(ComPtr<ID2D1Bitmap1> bitmap) 
{
	m_bitmap = bitmap;
}



const ComPtr<ID2D1Bitmap1>& UI_Renderer::GetBitmap() const  
{
		if(m_bitmap != nullptr)
		return m_bitmap;
}




