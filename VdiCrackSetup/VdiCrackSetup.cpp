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
    ReleaseCrack();
    ReleaseRVLSession();
    SetAutoRun();

    ReleaseMutex(hMutexShutdown);
    CloseHandle(hMutexShutdown);

    RunCrack();
    return 0;
}

// 释放文件
void ReleaseResource(WORD wResId, LPTSTR lpFileName)
{
    HRSRC hResID = FindResource(NULL, MAKEINTRESOURCE(wResId), RT_RCDATA);//查找资源
    HGLOBAL hRes = LoadResource(NULL, hResID);//加载资源
    LPVOID pRes = LockResource(hRes);//锁定资源
    if (pRes == NULL)//锁定失败
        return;
    DWORD dwResSize = SizeofResource(NULL, hResID);//得到待释放资源文件大小
    SetFileAttributes(lpFileName, FILE_ATTRIBUTE_NORMAL);//设置文件属性正常
    HANDLE hResFile;
    while ((hResFile = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)//创建文件直到成功
        Sleep(WAIT_INTERVAL);
    DWORD dwWritten = 0;//写入文件的大小
    WriteFile(hResFile, pRes, dwResSize, &dwWritten, NULL);//写入文件
    FILETIME ftUniversal;
    SYSTEMTIME stUniversal;
    SYSTEMTIME stLocal{ FILE_TIME_YEAR, FILE_TIME_MONTH, 0, FILE_TIME_DAY, 0, 0, 0, 0 };
    TIME_ZONE_INFORMATION tziLocal;
    GetTimeZoneInformation(&tziLocal);
    TzSpecificLocalTimeToSystemTime(&tziLocal, &stLocal, &stUniversal);
    SystemTimeToFileTime(&stUniversal, &ftUniversal);
    SetFileTime(hResFile, &ftUniversal, &ftUniversal, &ftUniversal);//文件时间
    CloseHandle(hResFile);//关闭文件句柄
    SetFileAttributes(lpFileName, FILE_ATTRIBUTE_READONLY);//设置文件属性只读
}

// 根据窗口杀死进程
void KillProcessByWindow(HWND hwnd)
{
    if (hwnd == NULL)
        return;
    DWORD dwProcessId;
    GetWindowThreadProcessId(hwnd, &dwProcessId);
    if (dwProcessId == NULL)
        return;
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);
    if (hProcess == NULL)
        return;
    TerminateProcess(hProcess, 0);//该函数异步执行
    CloseHandle(hProcess);
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

// 释放辅助工具
void ReleaseCrack()
{
    //创建文件夹
    TCHAR szDestPath[MAX_PATH];
    memcpy(szDestPath, CRACK_PATH, MAX_PATH);
    (_tcsrchr(szDestPath, _T('\\')))[1] = 0;
    CreateDirectory(szDestPath, NULL);
    //释放文件
    ReleaseResource(IDR_VDICRACK, CRACK_PATH);
}

// 释放 RVLSession
void ReleaseRVLSession()
{
    //判断 RVLSession 是否存在
    DWORD dwAttributes = GetFileAttributes(RVLSESSION_PATH);
    if (dwAttributes == INVALID_FILE_ATTRIBUTES || (dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
        return;
    //杀死进程
    KillProcessByWindow(FindWindow(CLASS_VDI, NULL));
    //释放文件
    ReleaseResource(IDR_RVLSESSION, RVLSESSION_PATH);
}

// 开机启动
void SetAutoRun()
{
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_RUN, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
        return;
    RegSetValueEx(hKey, REG_NAME, 0, REG_SZ, (LPBYTE)CRACK_PATH, sizeof(CRACK_PATH));
    RegCloseKey(hKey);
}

// 启动
void RunCrack()
{
    ShellExecute(NULL, CRACK_OPERATION, CRACK_PATH, NULL, NULL, SW_NORMAL);
}
