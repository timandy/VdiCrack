#pragma once

class VdiManager
{
#define CLASS_VDI                   _T("redc_wclass")
#define CLASS_WATER_MARK            _T("waterMark_wclass")
#define CLASS_NAVBAR                _T("Sangfor_VDI_Navbar_Window_Type")
#define NAVBAR_WIDTH                486
#define NAVBAR_HEIGHT               31
#define HIDE_OFFSET                 -5000
#define HIDE_SIZE                   0
#define CRACK_TIMER_INTERVAL        1000
#define WAKE_TIMER_INTERVAL         20*1000
#define BOSS_HOTKEY_MSG             WM_USER + 1
#define BOSS_HOTKEY_MOD             LLKHF_ALTDOWN
#define BOSS_HOTKEY                 'Q'

private:
    //记忆字段
    HWND        hwndVdi;            //Vdi 主窗口句柄
    HWND        hwndWaterMark;      //水印窗口句柄
    HWND        hwndNavbar;         //导航栏句柄
    BOOL        bRunning;           //Vdi 是否在运行
    BOOL        bVdiVisible;        //Vdi 是否可见
    BOOL        bVdiActive;         //Vdi 主窗口是否激活
    BOOL        bVdiTopMost;        //Vdi 主窗口是否置顶
    //辅助字段
    HWND        hwndVdiLast;        //上次 Vdi 窗口
    HWND        hwndWaterMarkLast;  //上次水印窗口
    RECT        rcNavbarLast;       //上次导航栏位置尺寸
    //资源字段
    UINT_PTR    nCrackEventID;      //破解定时器
    UINT_PTR    nWakeEventID;       //唤醒定时器
    HHOOK       hKeyboardHook;      //全局键盘钩子

    void Refresh();
    void HideWaterMark();
    void DisableClose();
    void SyncNavbarState();
    void SyncVdiState();
    void CrackOnce();
    void WakeUp();
    void SwithVisible();

    static HHOOK s_hKeyboardHook;
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
public:
    VdiManager();
    ~VdiManager();
    void ProcMessage(MSG *lpMsg);
};
