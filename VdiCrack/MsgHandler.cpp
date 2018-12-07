#include "stdafx.h"
#include "MsgHandler.h"

MsgHandler::MsgHandler()
{
    this->nSyncEventID = SetTimer(NULL, 0, SYNC_TIMER_INTERVAL, NULL);
    this->nWakeEventID = SetTimer(NULL, 0, WAKE_TIMER_INTERVAL, NULL);
    this->pVdiWnd = new VdiWnd();
}

MsgHandler::~MsgHandler()
{
    if (this->nSyncEventID != NULL)
    {
        KillTimer(NULL, this->nSyncEventID);
        this->nSyncEventID = NULL;
    }
    if (this->nWakeEventID != NULL)
    {
        KillTimer(NULL, this->nWakeEventID);
        this->nWakeEventID = NULL;
    }
    if (this->pVdiWnd != NULL)
    {
        delete this->pVdiWnd;
        this->pVdiWnd = NULL;
    }
}

void MsgHandler::HandleMessage(MSG * lpMsg)
{
    this->pVdiWnd->Refresh();
    if (!this->pVdiWnd->IsRunning())
        return;

    switch (lpMsg->message)
    {
    case WM_TIMER:
        if (lpMsg->wParam == this->nSyncEventID)
            this->pVdiWnd->SyncState();
        else if (lpMsg->wParam == this->nWakeEventID)
            this->pVdiWnd->WakeUp();
        return;

    case BOSS_HOTKEY_MSG:
        this->pVdiWnd->PressBossKey();
        return;

    default:
        return;
    }
}
