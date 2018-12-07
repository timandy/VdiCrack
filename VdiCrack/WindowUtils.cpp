#include "stdafx.h"
#include "WindowUtils.h"

void WindowUtils::HideWindow(HWND hwnd)
{
    SetLayeredWindowAttributes(hwnd, 0, 0, LWA_COLORKEY);
    SetWindowPos(hwnd, HWND_BOTTOM, HIDE_OFFSET, HIDE_OFFSET, HIDE_SIZE, HIDE_SIZE, SWP_ASYNCWINDOWPOS);
}

void WindowUtils::SwitchVisible(HWND hwnd)
{
    if (IsWindowVisible(hwnd))
    {
        ShowWindow(hwnd, SW_MINIMIZE);
        ShowWindow(hwnd, SW_HIDE);
    }
    else
    {
        ShowWindow(hwnd, SW_SHOW);
        SwitchToThisWindow(hwnd, TRUE);
    }
}

void WindowUtils::NoTopMost(HWND hwnd)
{
    BOOL bTopMost = GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOPMOST;
    if (bTopMost)
        SetWindowPos(hwnd, HWND_NOTOPMOST, HIDE_OFFSET, HIDE_OFFSET, HIDE_SIZE, HIDE_SIZE, SWP_ASYNCWINDOWPOS | SWP_NOMOVE | SWP_NOSIZE);
}

void WindowUtils::SendKey(HWND hwnd, UINT vKey)
{
    WORD dwScanCode = MapVirtualKey(vKey, 0);
    DWORD dwKeyData = 1;
    dwKeyData |= dwScanCode << 16;
    dwKeyData |= 0 << 24;
    dwKeyData |= 1 << 29;
    //°´ÏÂ
    PostMessage(hwnd, WM_KEYDOWN, vKey, dwKeyData);
    //µ¯Æð
    dwKeyData |= 3 << 30;
    PostMessage(hwnd, WM_KEYUP, vKey, dwKeyData);
}
