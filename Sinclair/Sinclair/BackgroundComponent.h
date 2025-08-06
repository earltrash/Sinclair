#pragma once
#include "pch.h"
#include "Component.h"
#include "Object.h"
#include "RenderInfo.h"

using namespace Microsoft::WRL;
using namespace std;

class BackgroundComponent :public Component
{
public:
	//BackgroundComponent()  = default;
	BackgroundComponent(RenderInfo* renderInfo) : m_renderInfo(renderInfo) {}
	~BackgroundComponent() = default;

	void Update() override;

	void BitmapPush(string NM, ComPtr<ID2D1Bitmap1> Bitmap);

	void SetCurrentBitmap(string Nm);
	ComPtr<ID2D1Bitmap1> GetCurrentBitmap();

	// 이거 두 함수는 내 의도에 맞진 않음
	ComPtr<ID2D1Bitmap1> GetBitmap();
	ComPtr<ID2D1Bitmap1> GetBitmap(std::string);

	// Getter
	float GetWidth() const { return width; }
	float GetHeight() const { return height; }

	// Setter
	void SetWidth(float w) { width = w; m_renderInfo->SetDestRight(width); }
	void SetHeight(float h) { height = h; m_renderInfo->SetDestBottom(height); }

private:
	RenderInfo* m_renderInfo;
	unordered_map<string, ComPtr<ID2D1Bitmap1>> m_Bitmaps;
	ComPtr<ID2D1Bitmap1> m_currentBitmap;
	string m_currentName;
	float width, height;
};