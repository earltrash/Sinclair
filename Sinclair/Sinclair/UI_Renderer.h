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


		// �μ� �Ѱ�¥�� �߰��� �������� �ϴ�. �ӽ÷�
		void SetBitmap(ComPtr<ID2D1Bitmap1> bitmap);
		const ComPtr<ID2D1Bitmap1>& GetBitmap() const;

		// Ư�� Ű�� �ش��ϴ� ��Ʈ���� ����.
		void SetBitmap(const std::string& key, ComPtr<ID2D1Bitmap1> bitmap);
		// Ư�� Ű�� �ش��ϴ� ��Ʈ�� ��������.
		ComPtr<ID2D1Bitmap1> GetBitmap(const std::string& key) const;

private:
		std::unordered_map<std::string, ComPtr<ID2D1Bitmap1>> m_bitmaps;

		ComPtr<ID2D1Bitmap1> m_bitmap;

};

