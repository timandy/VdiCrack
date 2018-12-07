#pragma once
#include "VdiWnd.h"

class MsgHandler
{
private:
    UINT_PTR    nSyncEventID;       //同步定时器
    UINT_PTR    nWakeEventID;       //唤醒定时器
    VdiWnd      *pVdiWnd;           //Vdi 窗口管理器
public:
    MsgHandler();
    ~MsgHandler();
    void HandleMessage(MSG *lpMsg);
};
