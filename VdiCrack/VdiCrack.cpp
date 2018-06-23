// VdiCrack.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "VdiCrack.h"

#define INTERVAL            10000
#define MUTEXT_NAME         "VDI_CRACK"
#define CLASS_WATER_MARK    "waterMark_wclass"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    HANDLE hMutex = CreateMutex(NULL, false, _T(MUTEXT_NAME));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
        return 0;

    HWND hwndWaterMarkLast = HWND_DESKTOP;
    do {
        HWND hwndWaterMark = FindWindow(_T(CLASS_WATER_MARK), NULL);
        if (hwndWaterMark == 0 || hwndWaterMark == hwndWaterMarkLast) {
            hwndWaterMarkLast = hwndWaterMark;
            Sleep(INTERVAL);
            continue;
        }
        SetLayeredWindowAttributes(hwndWaterMark, 0, 0, 0);
        SetWindowPos(hwndWaterMark, HWND_BOTTOM, -100000, -100000, 0, 0, SWP_ASYNCWINDOWPOS);
        hwndWaterMarkLast = hwndWaterMark;
        Sleep(INTERVAL);
    } while (true);

    ReleaseMutex(hMutex);
}
