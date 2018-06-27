// VdiCrack.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "VdiCrack.h"

#define MUTEXT_RUN          _T("VDI_CRACK_RUN")
#define MUTEXT_SHUTDOWN     _T("VDI_CRACK_SHUTDOWN")
#define CLASS_VDI           _T("redc_wclass")
#define CLASS_NAVBAR        _T("Sangfor_VDI_Navbar_Window_Type")
#define CLASS_WATER_MARK    _T("waterMark_wclass")
#define INTERVAL            1000
#define HIDE_OFFSET         -100000
#define HIDE_SIZE           0

// 入口
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    HANDLE hMutexRun = CreateMutex(NULL, TRUE, MUTEXT_RUN);
    if (hMutexRun == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
        return 0;

    HWND hwndWaterMarkLast = NULL;
    RECT rcNavbarLast = {};
    while (allowRun())
    {
        hwndWaterMarkLast = hideWaterMark(hwndWaterMarkLast);
        rcNavbarLast = syncNavbarState(rcNavbarLast);
        Sleep(INTERVAL);
    }

    ReleaseMutex(hMutexRun);
    CloseHandle(hMutexRun);
    return 0;
}

// 允许运行
BOOL allowRun()
{
    HANDLE hMutexShutdown = OpenMutex(SYNCHRONIZE, FALSE, MUTEXT_SHUTDOWN);
    if (hMutexShutdown == NULL)
        return TRUE;
    CloseHandle(hMutexShutdown);
    return FALSE;
}

// 去除水印
HWND hideWaterMark(HWND hwndWaterMarkLast)
{
    HWND hwndWaterMark = FindWindow(CLASS_WATER_MARK, NULL);
    if (hwndWaterMark == NULL || hwndWaterMark == hwndWaterMarkLast)
        return hwndWaterMark;
    SetLayeredWindowAttributes(hwndWaterMark, 0, 0, LWA_COLORKEY);
    SetWindowPos(hwndWaterMark, HWND_BOTTOM, HIDE_OFFSET, HIDE_OFFSET, HIDE_SIZE, HIDE_SIZE, SWP_ASYNCWINDOWPOS);
    return hwndWaterMark;
}

// 同步导航栏状态
RECT syncNavbarState(RECT rcNavbarLast)
{
    HWND hwndVdi = FindWindow(CLASS_VDI, NULL);
    if (hwndVdi == NULL)
        return rcNavbarLast;
    HWND hwndNavbar = FindWindow(CLASS_NAVBAR, NULL);
    if (hwndNavbar == NULL)
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
