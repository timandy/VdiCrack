#include "stdafx.h"
#include "VdiManager.h"

HHOOK VdiManager::s_hKeyboardHook = NULL;

// 构造
VdiManager::VdiManager()
{
    this->nCrackEventID = SetTimer(NULL, 0, CRACK_TIMER_INTERVAL, NULL);
    this->nWakeEventID = SetTimer(NULL, 0, WAKE_TIMER_INTERVAL, NULL);
    this->hKeyboardHook = s_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, &LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
}

// 析构
VdiManager::~VdiManager()
{
    if (this->nCrackEventID != NULL)
    {
        KillTimer(NULL, this->nCrackEventID);
        this->nCrackEventID = NULL;
    }
    if (this->nWakeEventID != NULL)
    {
        KillTimer(NULL, this->nWakeEventID);
        this->nWakeEventID = NULL;
    }
    if (this->hKeyboardHook != NULL)
    {
        UnhookWindowsHookEx(this->hKeyboardHook);
        this->hKeyboardHook = NULL;
    }
}

// 刷新
void VdiManager::Refresh()
{
    this->hwndVdi = FindWindow(CLASS_VDI, NULL);
    this->hwndWaterMark = FindWindow(CLASS_WATER_MARK, NULL);
    this->hwndNavbar = FindWindow(CLASS_NAVBAR, NULL);
    this->bRunning = this->hwndVdi != NULL && this->hwndWaterMark != NULL && this->hwndNavbar != NULL;
    if (this->bRunning)
    {
        this->bVdiVisible = IsWindowVisible(this->hwndVdi);
        this->bVdiActive = this->hwndVdi == GetForegroundWindow();
        this->bVdiTopMost = GetWindowLong(this->hwndVdi, GWL_EXSTYLE) & WS_EX_TOPMOST;
    }
}

// 去除水印
void VdiManager::HideWaterMark()
{
    if (this->hwndWaterMark == this->hwndWaterMarkLast)
        return;
    SetLayeredWindowAttributes(this->hwndWaterMark, 0, 0, LWA_COLORKEY);
    SetWindowPos(this->hwndWaterMark, HWND_BOTTOM, HIDE_OFFSET, HIDE_OFFSET, HIDE_SIZE, HIDE_SIZE, SWP_ASYNCWINDOWPOS);
    this->hwndWaterMarkLast = this->hwndWaterMark;
}

// 禁用关闭
void VdiManager::DisableClose()
{
    if (this->hwndVdi == this->hwndVdiLast)
        return;
    HMENU hMenu = GetSystemMenu(this->hwndVdi, FALSE);
    DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
    this->hwndVdiLast = this->hwndVdi;
}

// 同步导航栏状态
void VdiManager::SyncNavbarState()
{
    RECT rcNavbar;
    GetWindowRect(this->hwndNavbar, &rcNavbar);

    // 如果为活动窗口显示导航栏, 否则隐藏导航栏
    if (this->bVdiVisible && this->bVdiActive)
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

// 同步主窗口状态
void VdiManager::SyncVdiState()
{
    if (this->bVdiTopMost)
        SetWindowPos(this->hwndVdi, HWND_NOTOPMOST, HIDE_OFFSET, HIDE_OFFSET, HIDE_SIZE, HIDE_SIZE, SWP_ASYNCWINDOWPOS | SWP_NOMOVE | SWP_NOSIZE);
}

// 执行一次任务
void VdiManager::CrackOnce()
{
    this->HideWaterMark();
    this->DisableClose();
    this->SyncNavbarState();
    this->SyncVdiState();
}

// 唤醒
void VdiManager::WakeUp()
{
    if ((GetAsyncKeyState(VK_LBUTTON) & 0xFF00)
        || (GetAsyncKeyState(VK_RBUTTON) & 0xFF00)
        || (GetAsyncKeyState(VK_MBUTTON) & 0xFF00)
        || (GetAsyncKeyState(VK_XBUTTON1) & 0xFF00)
        || (GetAsyncKeyState(VK_XBUTTON2) & 0xFF00))
        return;
    LPARAM lParam = MAKELPARAM(HIDE_OFFSET, HIDE_OFFSET);
    SendMessage(this->hwndVdi, WM_MOUSEMOVE, NULL, lParam);
}

// 老板键切换
void VdiManager::SwithVisible()
{
    if (this->bVdiVisible)
    {
        ShowWindow(this->hwndVdi, SW_MINIMIZE);
        ShowWindow(this->hwndVdi, SW_HIDE);
    }
    else
    {
        ShowWindow(this->hwndVdi, SW_SHOW);
        SwitchToThisWindow(this->hwndVdi, TRUE);
    }
}

// 键盘钩子回调
LRESULT CALLBACK VdiManager::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= HC_ACTION && wParam == WM_SYSKEYDOWN)
    {
        LPKBDLLHOOKSTRUCT pKbs = (LPKBDLLHOOKSTRUCT)lParam;
        if (pKbs->vkCode == BOSS_HOTKEY && (pKbs->flags & BOSS_HOTKEY_MOD) != 0)
        {
            PostMessage(NULL, BOSS_HOTKEY_MSG, NULL, NULL);
            return FALSE;
        }
    }
    return CallNextHookEx(s_hKeyboardHook, nCode, wParam, lParam);
}

// 处理消息
void VdiManager::ProcMessage(MSG *lpMsg)
{
    this->Refresh();
    if (!this->bRunning)
        return;

    switch (lpMsg->message)
    {
    case WM_TIMER:
        if (lpMsg->wParam == this->nCrackEventID)
            this->CrackOnce();
        else if (lpMsg->wParam == this->nWakeEventID)
            this->WakeUp();
        return;

    case BOSS_HOTKEY_MSG:
        this->SwithVisible();
        return;

    default:
        return;
    }
}
