#pragma once
#include "pch.h"

LRESULT CALLBACK NzWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

class WinD
{
public:
		WinD() = default;
		virtual ~WinD() = default;

		bool Create(const wchar_t* className, const wchar_t* windowName, int width, int height);
		void Delete();
		void* Gethandle() const { return m_hWnd; }

		const int& GetWidth() const { return m_width; }
		const int& GetHeight() const { return m_height; }

protected:
		friend LRESULT CALLBACK NzWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		HWND m_hWnd = HWND();
		int m_width = 0;
		int m_height = 0;

		//이동 복사 대입 복사 대입 금! 
		WinD(const WinD&) = delete;
		WinD& operator=(const WinD&) = delete;
		WinD(WinD&&) = delete;
		WinD& operator=(WinD&&) = delete;
};