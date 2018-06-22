// VdiCrack.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "VdiCrack.h"
#include <WinUser.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	HWND hwndWaterMark = FindWindow(_T("waterMark_wclass"), NULL);
	if (hwndWaterMark == 0)
		return 0;

	SetLayeredWindowAttributes(hwndWaterMark, 0, 0, 0);
	SetWindowPos(hwndWaterMark, HWND_BOTTOM, -100000, -100000, 0, 0, SWP_ASYNCWINDOWPOS);
	return 0;
}
