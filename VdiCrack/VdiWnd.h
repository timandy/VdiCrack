#pragma once

class VdiWnd
{
private:
    HWND        hwndVdi;            //Vdi 主窗口句柄
    HWND        hwndWaterMark;      //水印窗口句柄
    HWND        hwndNavbar;         //导航栏句柄
    BOOL        bRunning;           //Vdi 是否在运行
    RECT        rcNavbarLast;       //上次导航栏位置尺寸
    void SyncNavbarState();
    void SetRunning(BOOL bRunning);
public:
    VdiWnd();
    ~VdiWnd();
    BOOL IsRunning();
    void Refresh();
    void SyncState();
    void WakeUp();
    void PressBossKey();
};
