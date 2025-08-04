#pragma once
#include "pch.h"



using namespace Microsoft::WRL;

class D2DRenderer
{
private:
    D2DRenderer() = default;

    ~D2DRenderer() = default;
    // 복사 방지용.
    D2DRenderer(const D2DRenderer&) = delete;
    D2DRenderer& operator=(const D2DRenderer&) = delete;
public:
    static D2DRenderer& Get();

    void Initialize(HWND hwnd);

    void Uninitialize();

    void Resize(UINT width, UINT height);

    void DrawLine(float x1, float y1, float x2, float y2, const D2D1::ColorF& color);

    void DrawCircle(float x, float y, float radius, const D2D1::ColorF& color);

    void DrawRectangle(float left, float top, float right, float bottom, const D2D1::ColorF& color);

    void DrawBitmap(ID2D1Bitmap1* bitmap, D2D1_RECT_F dest);

    void DrawBitmap(ID2D1Bitmap1* bitmap, D2D1_RECT_F destRect, D2D1_RECT_F srcRect, float opacity = 1.0f);

    void DrawMessage(const wchar_t* text, float left, float top, float width, float height, const D2D1::ColorF& color);

    void SetTransform(const D2D1_MATRIX_3X2_F tm);

    void RenderBegin();

    void RenderEnd(bool bPresent = true);

    void Present();

    void CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap);

    ID3D11Device* GetD3DDevice() const { return m_d3dDevice.Get(); }

    ID3D11DeviceContext* GetD3DContext() const { return m_d3dContext.Get(); }

    ID3D11RenderTargetView* GetD3DRenderTargetView() const { return m_d3dRenderTV.Get(); }

    ID2D1Bitmap1** GetBitmap() { return m_targetBitmap.GetAddressOf(); }

    void CreateWriteResource();

    void CreateWriteResource(const wchar_t* fontName, DWRITE_FONT_WEIGHT fontWeight, float fontSize);

private:

    void CreateDeviceAndSwapChain(HWND hwnd);

    void CreateRenderTargets();

    void ReleaseRenderTargets();

private:
    HWND m_hwnd = nullptr;

    ComPtr<ID3D11Device>           m_d3dDevice;
    ComPtr<IDXGISwapChain1>        m_swapChain;

    ComPtr<ID3D11DeviceContext>    m_d3dContext;
    ComPtr<ID3D11RenderTargetView> m_d3dRenderTV; // D3D render target view

    ComPtr<ID2D1Bitmap1>           m_targetBitmap; // D2D render target bitmap
    ComPtr<ID2D1SolidColorBrush>   m_brush;
    ComPtr<ID2D1SolidColorBrush>   m_textBrush;
    ComPtr <IDWriteTextFormat>     m_textFormat;

    ComPtr<ID2D1Device7>           m_d2dDevice;
    ComPtr<ID2D1DeviceContext7>    m_d2dContext;

    ComPtr<IWICImagingFactory>     m_wicFactory;
};
#pragma once
