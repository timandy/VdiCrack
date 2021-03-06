#pragma once

class VdiWnd
{
private:
    HWND        hwndVdi;            //Vdi �����ھ��
    HWND        hwndWaterMark;      //ˮӡ���ھ��
    HWND        hwndNavbar;         //���������
    BOOL        bRunning;           //Vdi �Ƿ�������
    RECT        rcNavbarLast;       //�ϴε�����λ�óߴ�
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
