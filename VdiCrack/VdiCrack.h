#pragma once

#include "resource.h"

typedef struct tagVDIINFO
{
    HWND    hwndVdi;
    HWND    hwndWaterMark;
    HWND    hwndNavbar;
    HWND    hwndWaterMarkLast;//上次水印窗口
    RECT    rcNavbarLast;//上次导航栏位置尺寸

    BOOL isValid()
    {
        return this->hwndVdi != NULL && this->hwndWaterMark != NULL && this->hwndNavbar != NULL;
    }
} VDIINFO, *LPVDIINFO;

BOOL allowRun();

void getVdiInfo(LPVDIINFO lpVdiInfo);

void hideWaterMark(LPVDIINFO lpVdiInfo);

void syncNavbarState(LPVDIINFO lpVdiInfo);

void noTopMostVdi(LPVDIINFO lpVdiInfo);
