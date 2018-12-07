#pragma once

class WindowUtils
{
public:
    static void HideWindow(HWND hwnd);
    static void SwitchVisible(HWND hwnd);
    static void NoTopMost(HWND hwnd);
    static void SendKey(HWND hwnd, UINT vKey);
};
