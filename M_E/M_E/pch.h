#pragma once
#include <iostream>
#include <Windows.h>
#include <assert.h>
#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>         // shared_ptr
#include <string>
#include <map>
#include <algorithm>
#include <unordered_set>
#include <list>
//D2D 라이브러리 및 헤더 
#pragma region D2D
#pragma comment(lib, "d2d1.lib")     // D2D1CreateFactory
#pragma comment(lib, "d3d11.lib")    // D3D11CreateDevice
#pragma comment(lib, "dwrite.lib")
#include <wrl/client.h> 
#include <d3d11.h>                // ID3D11Device, ID3D11DeviceContext
#include <dxgi1_2.h>              // IDXGISwapChain1
#include <d2d1_3.h>               // ID2D1Device7, ID2D1DeviceContext7
#include <d2d1.h>                 // ID2D1HwndRenderTarget
#include <d2d1_1.h> 
#include <dwrite_3.h>                // DirectWrite (최신 텍스트 엔진)
#include <wincodec.h>                // WIC (이미지 로딩)
#pragma endregion 

#pragma region DX
namespace DX
{
    // Helper class for COM exceptions
    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr) : result(hr) {}

        const char* what() const noexcept override
        {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X",
                static_cast<unsigned int>(result));
            return s_str;
        }

    private:
        HRESULT result;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw com_exception(hr);
        }
    }
}

#pragma endregion