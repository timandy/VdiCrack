// VdiCrackSetup.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include "VdiCrackSetup.h"

// 入口
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    HANDLE hMutexShutdown = CreateMutex(NULL, TRUE, MUTEXT_SHUTDOWN);
    if (hMutexShutdown == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
        return 0;

    WaitCrackEnd();
    ReleaseFile();
    SetAutoRun();

    ReleaseMutex(hMutexShutdown);
    CloseHandle(hMutexShutdown);

    RunCrack();
    return 0;
}

// 等待进程结束
void WaitCrackEnd()
{
    while (TRUE)
    {
        HANDLE hMutexRun = OpenMutex(SYNCHRONIZE, FALSE, MUTEXT_RUN);
        if (hMutexRun == NULL)
            return;
        CloseHandle(hMutexRun);
        Sleep(WAIT_INTERVAL);
    }
}

// 释放文件
void ReleaseFile()
{
    //创建文件夹
    TCHAR szDestPath[MAX_PATH];
    memcpy(szDestPath, EXE_PATH, MAX_PATH);
    (_tcsrchr(szDestPath, _T('\\')))[1] = 0;
    CreateDirectory(szDestPath, NULL);

    //释放文件
    HRSRC hResID = FindResource(NULL, MAKEINTRESOURCE(IDR_VDICRACK), RT_RCDATA);//查找资源
    HGLOBAL hRes = LoadResource(NULL, hResID);//加载资源
    LPVOID pRes = LockResource(hRes);//锁定资源
    if (pRes == NULL)//锁定失败
        return;
    DWORD dwResSize = SizeofResource(NULL, hResID);//得到待释放资源文件大小
    SetFileAttributes(EXE_PATH, FILE_ATTRIBUTE_NORMAL);//设置文件属性正常
    HANDLE hResFile = CreateFile(EXE_PATH, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//创建文件
    if (hResFile == INVALID_HANDLE_VALUE)
        return;
    DWORD dwWritten = 0;//写入文件的大小
    WriteFile(hResFile, pRes, dwResSize, &dwWritten, NULL);//写入文件
    CloseHandle(hResFile);//关闭文件句柄
    SetFileAttributes(EXE_PATH, FILE_ATTRIBUTE_READONLY);//设置文件属性只读
}

// 开机启动
void SetAutoRun()
{
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_RUN, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
        return;
    RegSetValueEx(hKey, REG_NAME, 0, REG_SZ, (LPBYTE)EXE_PATH, sizeof(EXE_PATH));
    RegCloseKey(hKey);
}

// 启动
void RunCrack()
{
    ShellExecute(NULL, EXE_OPERATION, EXE_PATH, NULL, NULL, SW_NORMAL);
}
