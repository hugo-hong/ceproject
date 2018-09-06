/************************************************************************
page.h: head file of the CPage class.
************************************************************************/
#ifndef __COMMPAGE_H__
#define __COMMPAGE_H__

#include "page.h"
#include "serial.h"
#include <utility>
#include <map>

using namespace std;

enum PORT_STATUS
{
    PORT_IDLE,
    PORT_CONNECTED,
    PORT_DISCONNECTED,
    PORT_COMMUNICATE,
};

enum
{
    DATA_MSG,
    CMD_MSG,
};


class CCommPage : public CPage
{
public:
    CCommPage(void);
    ~CCommPage(void);


    BOOL Init(HWND hwndParent);
    void DeInit(void);

    virtual void DoInitDialog(WPARAM wParam, LPARAM lParam);
    virtual void DoTimerEvent(WPARAM wParam, LPARAM lParam);
    virtual void DoResizeWindow(WPARAM wParam, LPARAM lParam);
    virtual void DoCommand(WPARAM wParam, LPARAM lParam);

    void RegisterMsg(DWORD dwId, DWORD dwMsg);
    void UnRegisterMsg(DWORD dwId, DWORD dwMsg);
    void DispatchMsg(DWORD dwPara1, DWORD dwPara2, DWORD eMsg = 0);

    int GetPortStatus(void)
    {
        return m_ePortStatus;
    }

    void GetPortProperty(COMM_DCB_T &commDcb)
    {
        commDcb = m_commDCB;
    }
   

protected:        

    void ResetComm(void);
    void GetCommDCB(void);

    void ReceiveData(LPCSTR pData, DWORD dwLen);
    void TransmitData(LPCTSTR pData, DWORD dwLen); 

    friend void OnRxDataInd(void *pCaller, PBYTE pData, DWORD dwLen);
    friend BOOL CommDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    friend INT_PTR SettingsBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


private:
    CSerial m_serial;
    COMM_DCB_T m_commDCB;
    map<DWORD, DWORD> m_msgMap;

    int  m_ePortStatus;
    DWORD m_dwRxBytes;
    DWORD m_dwTxBytes;

    HWND m_hSettingsBox;
   
    HANDLE m_hLog;
    BOOL m_fgSaveLog;
    tstring m_strLogPath;
};



#endif//__COMMPAGE_H__