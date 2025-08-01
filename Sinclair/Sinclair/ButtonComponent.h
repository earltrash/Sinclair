#pragma once
#include "pch.h"
#include "Component.h"

using namespace Microsoft::WRL;
using namespace std;



class ButtonComponent : public Component
{
public:
	ButtonComponent() = default;
	~ButtonComponent() = default;
	
	void BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap);

	//현재 비트맵 갖기
	void SetCurrentBitmap(string Nm);
	ComPtr<ID2D1Bitmap1> GetBitmap();

private:
	unordered_map<string, ComPtr<ID2D1Bitmap1>> m_Bitmap; 
	ComPtr<ID2D1Bitmap1> m_curbm;
};