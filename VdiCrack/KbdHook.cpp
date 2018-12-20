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
    if (nCode >= HC_ACTION)
    {
        LPKBDLLHOOKSTRUCT pKbs = (LPKBDLLHOOKSTRUCT)lParam;
        switch (wParam)
        {
        case WM_SYSKEYDOWN:
            if (pKbs->vkCode == BOSS_HOTKEY && pKbs->flags & BOSS_HOTKEY_MOD)
            {
                keybd_event(BOSS_HOTKEY_VK, MapVirtualKey(BOSS_HOTKEY_VK, 0), 0, 0);
                PostMessage(NULL, BOSS_HOTKEY_MSG, 0, 0);
                return FALSE;
            }
            break;
        case WM_SYSKEYUP:
            if (pKbs->vkCode == BOSS_HOTKEY && pKbs->flags & BOSS_HOTKEY_MOD)
                return TRUE;
            break;
        default:
            break;
        }
    }
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}
