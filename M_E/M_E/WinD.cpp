#include "pch.h"
#include "WinD.h"

LRESULT NzWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
		switch (msg) //기본적인 wnd event처리만 하게 함. msg->Input처리는 RUN에서 직접 이루어짐(확장성 땜에) 
		{
		case WM_SIZING:
		{
				RECT* rect = (RECT*)lparam;

				const int Width = 1920;
				const int Height = 1080;

				rect->right = rect->left + Width;
				rect->bottom = rect->top + Height;

				break;
		}


		case WM_DESTROY:
				std::cout << "WM_DESTROY received, exiting..." << std::endl;
				PostQuitMessage(0);
				break;


		default:
				return::DefWindowProc(hwnd, msg, wparam, lparam);
		}//switch

		return NULL;
}

bool WinD::Create(const wchar_t* className, const wchar_t* windowName, int width, int height)
{
		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpszClassName = className;
		wc.lpfnWndProc = NzWndProc; //프로시저 등록 
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; // ← 여기 스타일 추가


		ATOM classId = 0;
		if (!GetClassInfoEx(HINSTANCE(), className, &wc))
		{
				classId = RegisterClassEx(&wc);

				if (0 == classId) return false;
		}

		m_width = width;
		m_height = height;

		RECT rc = { 0, 0, width, height };
		//윈도우 창 만들 때, 설정값 넣는 거. 
		AdjustWindowRect(&rc,
				WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);

		m_hWnd = CreateWindowEx(NULL, MAKEINTATOM(classId), L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
				rc.right - rc.left, rc.bottom - rc.top, HWND(), HMENU(), HINSTANCE(), NULL);

		if (NULL == m_hWnd) return false;

		::SetWindowText((HWND)m_hWnd, windowName);
		SetWindowLongPtr((HWND)m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
		//WinD의 자식 객체인 Core를 가리키는 Pointer-> 이벤트 결과 함수는 해당 포인터 객체에서 실행

		ShowWindow((HWND)m_hWnd, SW_SHOW);
		UpdateWindow((HWND)m_hWnd);

		return true;
}

void WinD::Delete()
{
		if (NULL != m_hWnd)
		{
				DestroyWindow((HWND)m_hWnd);
				m_hWnd = NULL;
		}
}
