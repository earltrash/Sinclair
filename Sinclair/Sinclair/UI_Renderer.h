#pragma once
#include "pch.h"
#include "Component.h"

using namespace Microsoft::WRL;
using namespace std;
class UI_Renderer : public Component
{
public:
		UI_Renderer() = default;
		~UI_Renderer() = default; //Raii -> 


		// 인수 한개짜리 추가로 만들어두자 일단. 임시로
		void SetBitmap(ComPtr<ID2D1Bitmap1> bitmap);
		const ComPtr<ID2D1Bitmap1>& GetBitmap() const;

		// 특정 키에 해당하는 비트맵을 설정.
		void SetBitmap(const std::string& key, ComPtr<ID2D1Bitmap1> bitmap);
		// 특정 키에 해당하는 비트맵 가져오기.
		ComPtr<ID2D1Bitmap1> GetBitmap(const std::string& key) const;

private:
		std::unordered_map<std::string, ComPtr<ID2D1Bitmap1>> m_bitmaps;

		ComPtr<ID2D1Bitmap1> m_bitmap;

};

