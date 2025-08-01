#pragma once
#include "pch.h"
#include "Object.h"

using namespace Microsoft::WRL;

class Background : public Object
{
public:
	Background();
	~Background();


private:
	std::vector<ComPtr<ID2D1Bitmap1>> m_sheets;
};

