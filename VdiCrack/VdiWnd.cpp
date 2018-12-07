#include "stdafx.h"
#include "VdiWnd.h"
#include "KbdHook.h"
#include "WindowUtils.h"

void VdiWnd::SyncNavbarState()
{
    RECT rcNavbar;
    GetWindowRect(this->hwndNavbar, &rcNavbar);

    // 如果为活动窗口显示导航栏, 否则隐藏导航栏
    if (IsWindowVisible(this->hwndVdi) && GetForegroundWindow() == this->hwndVdi)
    {
        // check
        if (this->rcNavbarLast.left == this->rcNavbarLast.right)
        {
            HMONITOR hMonitor = MonitorFromWindow(this->hwndVdi, MONITOR_DEFAULTTONEAREST);
            MONITORINFO monitorInfo;
            monitorInfo.cbSize = sizeof(MONITORINFO);
            GetMonitorInfo(hMonitor, &monitorInfo);
            this->rcNavbarLast.left = (monitorInfo.rcMonitor.left + monitorInfo.rcMonitor.right - NAVBAR_WIDTH) / 2;
            this->rcNavbarLast.top = monitorInfo.rcMonitor.top;
            this->rcNavbarLast.right = this->rcNavbarLast.left + NAVBAR_WIDTH;
            this->rcNavbarLast.bottom = this->rcNavbarLast.top + NAVBAR_HEIGHT;
        }
        // show
        if (rcNavbar.left == rcNavbar.right)
            SetWindowPos(this->hwndNavbar, HWND_TOPMOST, this->rcNavbarLast.left, this->rcNavbarLast.top, this->rcNavbarLast.right - this->rcNavbarLast.left, this->rcNavbarLast.bottom - this->rcNavbarLast.top, SWP_ASYNCWINDOWPOS);
        return;
    }

    // hide
    if (rcNavbar.right > rcNavbar.left)
    {
        SetWindowPos(this->hwndNavbar, HWND_BOTTOM, HIDE_OFFSET, HIDE_OFFSET, HIDE_SIZE, HIDE_SIZE, SWP_ASYNCWINDOWPOS);
        this->rcNavbarLast = rcNavbar;
    }
}

void VdiWnd::SetRunning(BOOL bRunning)
{
    if (this->bRunning == bRunning)
        return;
    this->bRunning = bRunning;
    if (!this->bRunning)
    {
        KbdHook::Uninstall();
        return;
    }
    KbdHook::Install();
    WindowUtils::HideWindow(this->hwndWaterMark);
}

VdiWnd::VdiWnd()
{
}

VdiWnd::~VdiWnd()
{
    KbdHook::Uninstall();
}

BOOL VdiWnd::IsRunning()
{
    return this->bRunning;
}

void VdiWnd::Refresh()
{
    this->hwndVdi = FindWindow(CLASS_VDI, NULL);
    this->hwndWaterMark = FindWindow(CLASS_WATER_MARK, NULL);
    this->hwndNavbar = FindWindow(CLASS_NAVBAR, NULL);
    this->SetRunning(this->hwndVdi != NULL && this->hwndWaterMark != NULL && this->hwndNavbar != NULL);
}

void VdiWnd::WakeUp()
{
    WindowUtils::SendKey(this->hwndVdi, WAKE_KEY);
}

void VdiWnd::PressBossKey()
{
    WindowUtils::SwitchVisible(this->hwndVdi);
}

void VdiWnd::SyncState()
{
    this->SyncNavbarState();
    WindowUtils::NoTopMost(this->hwndVdi);
}
