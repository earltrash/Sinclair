// 1. 디버그 관련 정의와 헤더를 파일 최상단에 둡니다.
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include <iostream>
#include "M_Core.h"
#include <Windows.h>

#ifdef _DEBUG
    // new를 디버그 버전으로 재정의합니다.
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

// 2. 엔트리 포인트는 WinMain 하나만 사용합니다.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 3. WinMain 함수 시작 부분에 디버그 코드를 조건부로 삽입합니다.
#ifdef _DEBUG
    // 프로그램 종료 시 자동으로 누수 검사를 하도록 플래그 설정
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // 특정 번호의 메모리 누수 지점에서 중단하고 싶을 때 사용
    // 예: _CrtSetBreakAlloc(40986); 
#endif

    M_Core core;

    core.Init();
    core.Run();

   
    core.End();

    // _CrtSetDbgFlag를 사용하면 아래 코드는 필요 없습니다.
    // 프로그램이 정상 종료될 때 자동으로 덤프를 실행해 줍니다.
    // _CrtDumpMemoryLeaks(); 

    return 0;
}

//int main()
//{
//#ifdef _DEBUG
//    // 프로그램 종료 시 자동으로 누수 검사를 하도록 플래그 설정
//    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#endif
//
//    M_Core core;
//
//    core.Init();
//    core.Run();
//    core.End();
//
//    return 0;
//}