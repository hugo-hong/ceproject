#pragma once
#include "resource.h"
#include "page.h"
#include "regedit.h"
#include "bzi_bzmw.h"
#include "BZI_Call.h"
#include "bzs_comm.h"
#include "BZI_Pairing.h"
#include "BZI_Search.h"
#include "BZI_Connect.h"


enum _E_TabMode
{
    TAB_PAIR,
    TAB_DIAL,
    TAB_MUSIC,
    TAB_PHONEBOOK,
    TAB_MAX
};

enum EBT_TIMER
{
    OPEN_TIMER = 1,
    CLOSE_TIMER,
    DISCOVER_TIMER,
    SEARCH_TIMER,
    MAX_TIMER

};

enum EBT_VIEWID
{
    ID_MAIN = 1,
    ID_PAIR,
    ID_DIAL,
    ID_MUSIC,
    ID_PHONEBOOK,
};

enum EBT_STATE
{
    STATE_UNKNOW,
    STATE_INIT = (1 << 0),
    STATE_OPEN = (1 << 1),
    STATE_CLOSE = (1 << 2),
    STATE_SEARCH = (1 << 3),
    STATE_DISCOVER = (1 << 4),
    STATE_PAIR = (1 << 5),
    STATE_CONNECT = (1 << 6),
    STATE_CALL = (1 << 7),
    STATE_MUSIC = (1 << 8),
    STATE_PHONEBOOK = (1 << 9),
};



class EBTManager
{
public:
    EBTManager();
    virtual ~EBTManager();

    void DoInitDialog(WPARAM wParam, LPARAM lParam);
    void DoResizeDialog(WPARAM wParam, LPARAM lParam);    
    void DoSwitchTab(WPARAM wParam, LPARAM lParam);

    void SetDebugLog(int idLog, DWORD dwLogMask);

    BOOL OpenBTDevice(void);
    BOOL CloseBTDevice(void);

public:
    static EBTManager * GetInstance() 
    {
        return &m_Instance; 
    }

    CPage * GetSubInstance(int iSubIndex)
    {
        return m_pPage[iSubIndex];
    }

    int GetBTState(void)
    {
        return m_eBTState;
    }

    void SetBTState(EBT_STATE eState)
    {
        m_eBTState |= eState;             
    }

    void ClearBTState(EBT_STATE eState)
    {
        m_eBTState &= (~eState);
    }

    void SetWindow(HWND hWnd)
    {
        m_hDlgWnd = hWnd;
    }

    void RegisterCallBack(void);
    void UnRegisterCallBack(void);

   static void CALLBACK OnBTSwitch(DWORD dwIndID, WPARAM wParam, LPARAM lParam);  
   static void CALLBACK OnBTConnect(DWORD dwIndID, WPARAM wParam, LPARAM lParam);

protected:   
    void RefreshUI(void);
    BOOL CreateTabs(void);
    

private:
    static EBTManager m_Instance;

    HWND m_hDlgWnd;
    HWND m_hTabWnd;
    int m_iTabIndex;
    CPage *m_pPage[TAB_MAX];

    int m_eBTState; 
};

