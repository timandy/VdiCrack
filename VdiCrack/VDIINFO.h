#pragma once

#include "stdafx.h"

#define CLASS_VDI           _T("redc_wclass")
#define CLASS_WATER_MARK    _T("waterMark_wclass")
#define CLASS_NAVBAR        _T("Sangfor_VDI_Navbar_Window_Type")

typedef struct tagVDIINFO
{
    HWND    hwndVdi;            //Vdi �����ھ��
    HWND    hwndWaterMark;      //ˮӡ���ھ�� 
    HWND    hwndNavbar;         //���������
    BOOL    bRunning;           //Vdi �Ƿ�������
    BOOL    bVdiActive;         //Vdi �������Ƿ񼤻�
    BOOL    bVdiTopMost;        //Vdi �������Ƿ��ö�

    HWND    hwndWaterMarkLast;  //�ϴ�ˮӡ����
    RECT    rcNavbarLast;       //�ϴε�����λ�óߴ�

    //ˢ��
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

    //�Ƿ�������
    BOOL isRunning()
    {
        return this->bRunning;
    }
} VDIINFO, *LPVDIINFO;