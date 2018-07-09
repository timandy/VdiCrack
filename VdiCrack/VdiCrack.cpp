// VdiCrack.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "VdiCrack.h"

#define MUTEXT_RUN          _T("VDI_CRACK_RUN")
#define MUTEXT_SHUTDOWN     _T("VDI_CRACK_SHUTDOWN")
#define NAVBAR_WIDTH        486
#define NAVBAR_HEIGHT       31
#define INTERVAL            1000
#define HIDE_OFFSET         -100000
#define HIDE_SIZE           0

// 入口
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    HANDLE hMutexRun = CreateMutex(NULL, TRUE, MUTEXT_RUN);
    if (hMutexRun == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
        return 0;

    VDIINFO vdiInfo = {};
    while (allowRun())
    {
        vdiInfo.refresh();
        if (vdiInfo.isRunning())
        {
            hideWaterMark(&vdiInfo);
            syncNavbarState(&vdiInfo);
            noTopMostVdi(&vdiInfo);
        }
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
void hideWaterMark(LPVDIINFO lpVdiInfo)
{
    if (lpVdiInfo->hwndWaterMark == lpVdiInfo->hwndWaterMarkLast)
        return;
    SetLayeredWindowAttributes(lpVdiInfo->hwndWaterMark, 0, 0, LWA_COLORKEY);
    SetWindowPos(lpVdiInfo->hwndWaterMark, HWND_BOTTOM, HIDE_OFFSET, HIDE_OFFSET, HIDE_SIZE, HIDE_SIZE, SWP_ASYNCWINDOWPOS);
    lpVdiInfo->hwndWaterMarkLast = lpVdiInfo->hwndWaterMark;
}

// 同步导航栏状态
void syncNavbarState(LPVDIINFO lpVdiInfo)
{
    RECT rcNavbar;
    GetWindowRect(lpVdiInfo->hwndNavbar, &rcNavbar);

    // 如果为活动窗口显示导航栏, 否则隐藏导航栏
    if (lpVdiInfo->bVdiActive)
    {
        // check
        if (lpVdiInfo->rcNavbarLast.left == lpVdiInfo->rcNavbarLast.right)
        {
            HMONITOR hMonitor = MonitorFromWindow(lpVdiInfo->hwndVdi, MONITOR_DEFAULTTONEAREST);
            MONITORINFO monitorInfo;
            monitorInfo.cbSize = sizeof(MONITORINFO);
            GetMonitorInfo(hMonitor, &monitorInfo);
            lpVdiInfo->rcNavbarLast.left = (monitorInfo.rcMonitor.left + monitorInfo.rcMonitor.right - NAVBAR_WIDTH) / 2;
            lpVdiInfo->rcNavbarLast.top = monitorInfo.rcMonitor.top;
            lpVdiInfo->rcNavbarLast.right = lpVdiInfo->rcNavbarLast.left + NAVBAR_WIDTH;
            lpVdiInfo->rcNavbarLast.bottom = lpVdiInfo->rcNavbarLast.top + NAVBAR_HEIGHT;
        }
        // show
        if (rcNavbar.left == rcNavbar.right)
            SetWindowPos(lpVdiInfo->hwndNavbar, HWND_TOPMOST, lpVdiInfo->rcNavbarLast.left, lpVdiInfo->rcNavbarLast.top, lpVdiInfo->rcNavbarLast.right - lpVdiInfo->rcNavbarLast.left, lpVdiInfo->rcNavbarLast.bottom - lpVdiInfo->rcNavbarLast.top, SWP_ASYNCWINDOWPOS);
        return;
    }

    // hide
    if (rcNavbar.right > rcNavbar.left)
    {
        SetWindowPos(lpVdiInfo->hwndNavbar, HWND_BOTTOM, HIDE_OFFSET, HIDE_OFFSET, HIDE_SIZE, HIDE_SIZE, SWP_ASYNCWINDOWPOS);
        lpVdiInfo->rcNavbarLast = rcNavbar;
    }
}

// 不置顶
void noTopMostVdi(LPVDIINFO lpVdiInfo)
{
    if (lpVdiInfo->bVdiTopMost)
        SetWindowPos(lpVdiInfo->hwndVdi, HWND_NOTOPMOST, HIDE_OFFSET, HIDE_OFFSET, HIDE_SIZE, HIDE_SIZE, SWP_ASYNCWINDOWPOS | SWP_NOMOVE | SWP_NOSIZE);
}
