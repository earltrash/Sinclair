#pragma once
#include <Windows.h>

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp)) 



#define F_GET_X_LPARAM(lp) ((float)(short)LOWORD(lp))
#define F_GET_Y_LPARAM(lp) ((float)(short)HIWORD(lp))