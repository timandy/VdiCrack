// VdiCrack.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "VdiCrack.h"
#include "MsgHandler.h"

// 入口
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    HANDLE hMutexRun = CreateMutex(NULL, TRUE, MUTEXT_RUN);
    if (hMutexRun == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
        return 0;

    MSG msg;
    MsgHandler msgHandler;
    while (GetMessage(&msg, NULL, 0, 0) && AllowRun())
    {
        TranslateMessage(&msg);
        msgHandler.HandleMessage(&msg);
    }

    ReleaseMutex(hMutexRun);
    CloseHandle(hMutexRun);
    return 0;
}

// 允许运行
BOOL AllowRun()
{
    HANDLE hMutexShutdown = OpenMutex(SYNCHRONIZE, FALSE, MUTEXT_SHUTDOWN);
    if (hMutexShutdown == NULL)
        return TRUE;
    CloseHandle(hMutexShutdown);
    return FALSE;
}
