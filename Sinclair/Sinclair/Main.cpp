#include <iostream>
#include "M_Core.h"

int main()
{
		M_Core core;
		core.Init();

		core.Run();
		core.End();

		_CrtDumpMemoryLeaks();
		return 0;


}