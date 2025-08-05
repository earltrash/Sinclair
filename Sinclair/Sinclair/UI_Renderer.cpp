#include "UI_Renderer.h"

void UI_Renderer::SetBitmap(const std::string& key, ComPtr<ID2D1Bitmap1> bitmap)
{
    m_bitmaps[key] = bitmap;
}

ComPtr<ID2D1Bitmap1> UI_Renderer::GetBitmap(const std::string& key) const
{
    auto it = m_bitmaps.find(key);
    if (it != m_bitmaps.end())
    {
        return it->second;
    }
    return nullptr; // ��Ʈ���� ã�� ���ϸ� nullptr ��ȯ
}


void UI_Renderer::SetBitmap(ComPtr<ID2D1Bitmap1> bitmap)
{
    m_bitmap = bitmap;
}

const ComPtr<ID2D1Bitmap1>& UI_Renderer::GetBitmap() const
{
    if (m_bitmap != nullptr)
        return m_bitmap;
}


