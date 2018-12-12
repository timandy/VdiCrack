#include "stdafx.h"
#include "KbdHook.h"

HHOOK KbdHook::hKeyboardHook = NULL;

// 重新安装键盘钩子
void KbdHook::Install()
{
    Uninstall();
    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, &LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
}

// 卸载键盘钩子
void KbdHook::Uninstall()
{
    if (hKeyboardHook != NULL)
    {
        UnhookWindowsHookEx(hKeyboardHook);
        hKeyboardHook = NULL;
    }
}

// 钩子回调
LRESULT KbdHook::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= HC_ACTION && wParam == WM_SYSKEYDOWN)
    {
        LPKBDLLHOOKSTRUCT pKbs = (LPKBDLLHOOKSTRUCT)lParam;
        if (pKbs->vkCode == BOSS_HOTKEY && (pKbs->flags & BOSS_HOTKEY_MOD) != 0)
        {
            PostMessage(NULL, BOSS_HOTKEY_MSG, 0, 0);
            return FALSE;
        }
    }
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}
