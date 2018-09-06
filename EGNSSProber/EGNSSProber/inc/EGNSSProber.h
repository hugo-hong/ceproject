#pragma once
#include "resource.h"
#include "ecommon.h"
#include "page.h"

#define DEFAULT_SPACE           5

enum
{
    ID_COMMAND_CONNECT = 0xF000,
    ID_COMMAND_DISCONNECT,
    ID_COMMAND_SETTINGS,
    ID_COMMAND_COLD_BOOT,
    ID_COMMAND_WARM_BOOT,
    ID_COMMAND_HOT_BOOT,
    ID_COMMAND_FULLCOLD_BOOT,
};



enum _E_TabMode
{
    TAB_COMM,
    TAB_TRACK,        
    TAB_MAX
};

class DemoManager
{
public:
    DemoManager(void);
    ~DemoManager(void);

    //UI Control
    void DoInitDialog(WPARAM wParam, LPARAM lParam);
    void DoResizeDialog(WPARAM wParam, LPARAM lParam); 
    void DoSwitchTab(WPARAM wParam, LPARAM lParam);
    void DoCommand(WPARAM wParam, LPARAM lParam);
    void DoDestroy(WPARAM wParam, LPARAM lParam);
   
    void RefreshUI(WPARAM wParam = 0, LPARAM lParam = 0);

public:
    static DemoManager * GetInstance()
    {
        return &m_Instance;
    }

    CPage * GetSubInstance(int iSubIndex)
    {
        return m_pPage[iSubIndex];
    }

    void SetWindow(HWND hWnd)
    {
        m_hDlgWnd = hWnd;
    }

protected:
    
    BOOL CreateTabs(void);

private:
    static DemoManager m_Instance; 

    HWND m_hDlgWnd; //main dialog handle
    HWND m_hWndCommandBar;	// command bar handle
    HWND m_hStatusWnd; //status bar handle    

    HWND m_hTabWnd;
    int m_iTabIndex;
    CPage *m_pPage[TAB_MAX];
};