#pragma once
#include "pch.h"
#include "Component.h"

using namespace Microsoft::WRL;

class UI_Renderer : public Component
{
public:
		UI_Renderer(ComPtr<ID2D1Bitmap1> bitmap);
		~UI_Renderer(); //Raii

		void SetBitmap(ComPtr<ID2D1Bitmap1> bitmap ); //¿©±â¿¡ Bitmap Á¤º¸ ³Ö¾î¹ö¸².
		const ComPtr<ID2D1Bitmap1>& GetBitmap() const ; //¾ê´Â ¹¹ ¹ñ´Â °Å°í 

private:

		ComPtr<ID2D1Bitmap1> m_sheet;
};

