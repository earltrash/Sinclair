#pragma once
#include "pch.h"
#include "Component.h"
#include "Object.h"

using namespace Microsoft::WRL;
using namespace std;

class BackgroundComponent :public Component
{
public:
	BackgroundComponent()  = default;
	~BackgroundComponent() = default;

	void BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap);

	void SetCurrentBitmap(string Nm);
	ComPtr<ID2D1Bitmap1> GetBitmap();

	// Getter
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	// Setter
	void SetWidth(int w) { width = w; }
	void SetHeight(int h) { height = h; }

private:
	unordered_map<string, ComPtr<ID2D1Bitmap1>> m_Bitmaps;
	ComPtr<ID2D1Bitmap1> m_currentBitmap;
	int width, height;
};