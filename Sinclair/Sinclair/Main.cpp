#include <iostream>
#include "M_Core.h"
#include <Windows.h>

#if defined(DEBUG) || defined(_DEBUG)
int main()
{
		M_Core core;
		core.Init();

		core.Run();
		core.End();

		_CrtDumpMemoryLeaks();
		return 0;


}

#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    M_Core core;

    core.Init();

    core.Run();
    core.End();

    return 0;
}


#endif