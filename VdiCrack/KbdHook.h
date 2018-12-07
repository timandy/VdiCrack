#pragma once

class KbdHook
{
private:
    static HHOOK hKeyboardHook;
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

public:
    static void Install();
    static void Uninstall();
};
