#pragma once

#include "stdafx.h"

#define CLASS_VDI           _T("redc_wclass")
#define CLASS_WATER_MARK    _T("waterMark_wclass")
#define CLASS_NAVBAR        _T("Sangfor_VDI_Navbar_Window_Type")

typedef struct tagVDIINFO
{
    HWND    hwndVdi;            //Vdi 主窗口句柄
    HWND    hwndWaterMark;      //水印窗口句柄 
    HWND    hwndNavbar;         //导航栏句柄
    BOOL    bRunning;           //Vdi 是否在运行
    BOOL    bVdiActive;         //Vdi 主窗口是否激活
    BOOL    bVdiTopMost;        //Vdi 主窗口是否置顶

    HWND    hwndWaterMarkLast;  //上次水印窗口
    RECT    rcNavbarLast;       //上次导航栏位置尺寸

    //刷新
    void refresh()
    {
        this->hwndVdi = FindWindow(CLASS_VDI, NULL);
        this->hwndWaterMark = FindWindow(CLASS_WATER_MARK, NULL);
        this->hwndNavbar = FindWindow(CLASS_NAVBAR, NULL);
        this->bRunning = this->hwndVdi != NULL && this->hwndWaterMark != NULL && this->hwndNavbar != NULL;
        if (this->bRunning)
        {
            this->bVdiActive = this->hwndVdi == GetForegroundWindow();
            this->bVdiTopMost = GetWindowLong(this->hwndVdi, GWL_EXSTYLE) & WS_EX_TOPMOST;
        }
    }

    //是否在运行
    BOOL isRunning()
    {
        return this->bRunning;
    }
} VDIINFO, *LPVDIINFO;
