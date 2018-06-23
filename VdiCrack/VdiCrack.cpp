// VdiCrack.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "VdiCrack.h"

#define MUTEXT_NAME         "VDI_CRACK"
#define CLASS_VDI           "redc_wclass"
#define CLASS_NAVBAR        "Sangfor_VDI_Navbar_Window_Type"
#define CLASS_WATER_MARK    "waterMark_wclass"
#define INTERVAL            1000
#define HIDE_OFFSET         -100000
#define HIDE_SIZE           0

// 入口
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    HANDLE hMutex = CreateMutex(NULL, false, _T(MUTEXT_NAME));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
        return 0;

    HWND hwndWaterMarkLast = 0;
    RECT rcNavbarLast = {};
    do {
        hwndWaterMarkLast = hideWaterMark(hwndWaterMarkLast);
        rcNavbarLast = syncNavbarState(rcNavbarLast);
        Sleep(INTERVAL);
    } while (true);

    ReleaseMutex(hMutex);
}

// 去水印
HWND hideWaterMark(HWND hwndWaterMarkLast)
{
    HWND hwndWaterMark = FindWindow(_T(CLASS_WATER_MARK), NULL);
    if (hwndWaterMark == 0 || hwndWaterMark == hwndWaterMarkLast)
        return hwndWaterMark;
    SetLayeredWindowAttributes(hwndWaterMark, 0, 0, LWA_COLORKEY);
    SetWindowPos(hwndWaterMark, HWND_BOTTOM, HIDE_OFFSET, HIDE_OFFSET, HIDE_SIZE, HIDE_SIZE, SWP_ASYNCWINDOWPOS);
    return hwndWaterMark;
}

// 同步导航栏状态
RECT syncNavbarState(RECT rcNavbarLast)
{
    HWND hwndVdi = FindWindow(_T(CLASS_VDI), NULL);
    if (hwndVdi == 0)
        return rcNavbarLast;
    HWND hwndNavbar = FindWindow(_T(CLASS_NAVBAR), NULL);
    if (hwndNavbar == 0)
        return rcNavbarLast;
    RECT rcNavbar;
    GetWindowRect(hwndNavbar, &rcNavbar);
    if (hwndVdi == GetForegroundWindow())
    {
        // show
        if (rcNavbar.left == rcNavbar.right)
            SetWindowPos(hwndNavbar, HWND_TOPMOST, rcNavbarLast.left, rcNavbarLast.top, rcNavbarLast.right - rcNavbarLast.left, rcNavbarLast.bottom - rcNavbarLast.top, SWP_ASYNCWINDOWPOS);
    }
    else
    {
        // hide
        if (rcNavbar.right > rcNavbar.left) {
            SetWindowPos(hwndNavbar, HWND_BOTTOM, HIDE_OFFSET, HIDE_OFFSET, HIDE_SIZE, HIDE_SIZE, SWP_ASYNCWINDOWPOS);
            return rcNavbar;
        }
    }
    return rcNavbarLast;
}
