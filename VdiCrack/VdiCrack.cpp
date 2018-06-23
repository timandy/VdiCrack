// VdiCrack.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "VdiCrack.h"

#define INTERVAL            1000
#define MUTEXT_NAME         "VDI_CRACK"
#define CLASS_VDI           "redc_wclass"
#define CLASS_NAVBAR        "Sangfor_VDI_Navbar_Window_Type"
#define CLASS_WATER_MARK    "waterMark_wclass"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    HANDLE hMutex = CreateMutex(NULL, false, _T(MUTEXT_NAME));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
        return 0;

    HWND hwndWaterMarkLast = 0;
    do {
        hwndWaterMarkLast = hideWaterMark(hwndWaterMarkLast);
        syncNavbarState();
        Sleep(INTERVAL);
    } while (true);

    ReleaseMutex(hMutex);
}

// 去水印
HWND hideWaterMark(HWND  hwndWaterMarkLast)
{
    HWND hwndWaterMark = FindWindow(_T(CLASS_WATER_MARK), NULL);
    if (hwndWaterMark == 0 || hwndWaterMark == hwndWaterMarkLast)
        return hwndWaterMark;
    SetLayeredWindowAttributes(hwndWaterMark, 0, 0, 0);
    SetWindowPos(hwndWaterMark, HWND_BOTTOM, -100000, -100000, 0, 0, SWP_ASYNCWINDOWPOS);
    return hwndWaterMark;
}

// 同步导航栏状态
void syncNavbarState()
{
    HWND hwndVdi = FindWindow(_T(CLASS_VDI), NULL);
    if (hwndVdi == 0)
        return;
    HWND hwndNavbar = FindWindow(_T(CLASS_NAVBAR), NULL);
    if (hwndNavbar == 0)
        return;
    HWND hwndForeground = GetForegroundWindow();
    BOOL bNavbarTopmost = GetWindowLong(hwndNavbar, GWL_EXSTYLE) & WS_EX_TOPMOST;
    if (hwndVdi == hwndForeground)
    {
        if (!bNavbarTopmost)
            SetWindowPos(hwndNavbar, HWND_TOPMOST, 0, 0, 0, 0, SWP_ASYNCWINDOWPOS | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }
    else
    {
        if (bNavbarTopmost)
            SetWindowPos(hwndNavbar, HWND_BOTTOM, 0, 0, 0, 0, SWP_ASYNCWINDOWPOS | SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
    }
}
