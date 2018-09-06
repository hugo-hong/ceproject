// EGPSDemo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "winioctl.h"
#include "EGNSSProber.h"
#include "CommPage.h"
#include "TrackPage.h"

#define IDC_STATUSWND   100


extern INSTANCE_ENTRYDATA g_tEntryData;

DemoManager::DemoManager()
{
    m_hDlgWnd = NULL;
    m_hWndCommandBar = NULL;
    m_hStatusWnd = NULL;
    m_hTabWnd = NULL;
    m_iTabIndex = 0;  

    for (int i = 0; i < TAB_MAX; i++)
    {
        m_pPage[i] = NULL;
    }
}

DemoManager::~DemoManager()
{   
    
}

void DemoManager::DoInitDialog(WPARAM wParam, LPARAM lParam)
{
    ASSERT(g_tEntryData.m_hMainWnd != NULL);
    ASSERT(g_tEntryData.m_hInstance != NULL);

    //create command bar
    m_hWndCommandBar = ::CommandBar_Create(g_tEntryData.m_hInstance, g_tEntryData.m_hMainWnd, 1);
    if (m_hWndCommandBar)
    {        
        ::CommandBar_Show(m_hWndCommandBar, TRUE);
    }

    //Add menu bar
    ::CommandBar_InsertMenubar(m_hWndCommandBar, g_tEntryData.m_hInstance, IDR_MENU, 0);
    //CommandBar_AddAdornments(m_hWndCommandBar, 0, 0);
    

    //Add command button   
    TBBUTTON aButton[7] = {0};
    int aString[4] = {0};
    int idCommand = ID_COMMAND_CONNECT;
    int idString = IDS_COLD_BOOT;
    int i = 0;

    //add bitmap
    ::CommandBar_AddBitmap(m_hWndCommandBar, g_tEntryData.m_hInstance, IDB_BMP_CONNECT, 0, 16, 16);
    ::CommandBar_AddBitmap(m_hWndCommandBar, g_tEntryData.m_hInstance, IDB_BMP_DISCONNECT, 1, 16, 16);
    ::CommandBar_AddBitmap(m_hWndCommandBar, g_tEntryData.m_hInstance, IDB_BMP_SEETING, 2, 16, 16);

    //add string 
    for (i = 0; i < DIM(aString); i++, idString++)
    {
        aString[i] = ::SendMessage(m_hWndCommandBar, TB_ADDSTRING, (WPARAM)g_tEntryData.m_hInstance, (LPARAM)MAKELONG(idString,0));
    }

    for (i = 0; i < DIM(aButton); i++, idCommand++)
    {
        if (i < 3)
        {
            aButton[i].iBitmap = i;
            aButton[i].idCommand = idCommand;
            aButton[i].fsState = TBSTATE_ENABLED;
            aButton[i].fsStyle = TBSTYLE_BUTTON;
            aButton[i].iString = -1;
        }
        else
        {
            aButton[i].iBitmap = -1;
            aButton[i].idCommand = idCommand;
            aButton[i].fsState = TBSTATE_ENABLED;
            aButton[i].fsStyle = TBSTYLE_BUTTON;
            aButton[i].iString = aString[i - 3];
        }
    }   

    ::CommandBar_AddButtons(m_hWndCommandBar, DIM(aButton), aButton);
   
    ::SendMessage(m_hWndCommandBar, TB_AUTOSIZE, 0, 0);

    //Add status bar
    m_hStatusWnd = ::CreateStatusWindow(WS_CHILD| WS_VISIBLE | SBT_OWNERDRAW,
        NULL,
        g_tEntryData.m_hMainWnd,
        IDC_STATUSWND);    

    //adjust size
    //::PostMessage(g_tEntryData.m_hMainWnd, APP_RESIZE_MSG, 0, 0);
    ::PostMessage(m_hDlgWnd, WM_COMMAND, MAKEWPARAM(ID_FILE_FULLSRCEEN, 0), NULL);

    if (!CreateTabs())
    {
        ::MessageBox(m_hDlgWnd, _T("Application init failed!"), NULL, MB_ICONERROR);
        ::PostMessage(g_tEntryData.m_hMainWnd, WM_QUIT, 0, 0);        
    }
    else
    {
        RefreshUI();
    }
}

void DemoManager::DoResizeDialog(WPARAM wParam, LPARAM lParam)
{
    HDWP hdwp = NULL; 
    RECT rcTabs = {0};  
    RECT rcCommandBar = {0};
    RECT rcStatusBar = {0};
    int iWidth = LOWORD(lParam);
    int iHeight = HIWORD(lParam);    
    int iTabWidth = 0;
    int iTabHeight = 0;
    int iPageWidth = 0;
    int iPageHeight = 0;    
    int dx = 0, dy = 0;
    int cx = 0, cy = 0;

    if (NULL == m_hTabWnd)
    {
        return;
    }    

    dx = dy = DEFAULT_SPACE;

    //get menu bar size
    if (m_hWndCommandBar)
    {
        ::GetWindowRect(m_hWndCommandBar, &rcCommandBar);
        ::MapWindowPoints(HWND_DESKTOP, m_hDlgWnd, (LPPOINT) &rcCommandBar, 2);
    }    

    //get status bar size
    if (m_hStatusWnd)
    {
        ::GetWindowRect(m_hStatusWnd, &rcStatusBar);
        ::MapWindowPoints(HWND_DESKTOP, m_hDlgWnd, (LPPOINT) &rcStatusBar, 2);

        HDC hdc = ::GetDC(NULL);
        INT nInch = ::GetDeviceCaps(hdc, LOGPIXELSX);
        ::ReleaseDC(NULL, hdc);

        if (g_tEntryData.m_fgFullSreen)
        {
            nInch *= 2;
        }
        else
        {
            nInch = nInch * 3 / 2;
        }

        int ciParts[] = {nInch, 
            ciParts[0] + nInch,          
            -1};

        if (m_hStatusWnd) 
        {
            ::SendMessage(m_hStatusWnd, SB_SETPARTS, DIM(ciParts), (LPARAM)ciParts);
        }

        ::MoveWindow(m_hStatusWnd, 0, 0, 0, 0, TRUE); //redraw status bar
    }    
                

    // Size the tab control to fit the client area. 
    iTabWidth = iWidth - rcCommandBar.left * 2;
    iTabHeight = iHeight - (rcCommandBar.bottom-rcCommandBar.top) - (rcStatusBar.bottom - rcStatusBar.top) - dy;     

    cx = rcTabs.left = rcCommandBar.left;
    cy = rcTabs.top = rcCommandBar.bottom + dy;
    rcTabs.right = rcTabs.left + iTabWidth;
    rcTabs.bottom = rcTabs.top + iTabHeight;    

    hdwp = ::BeginDeferWindowPos(TAB_MAX + 1);    

    ::DeferWindowPos(hdwp, 
        m_hTabWnd, 
        NULL,//HWND_TOP, 
        cx, 
        cy, 
        iTabWidth, 
        iTabHeight, 
        0);       
   

    //get tab display area size
    TabCtrl_AdjustRect(m_hTabWnd, FALSE, &rcTabs);     
    cx = rcTabs.left - cx;
    cy = rcTabs.top - cy;
    iPageWidth = rcTabs.right - rcTabs.left;
    iPageHeight = rcTabs.bottom - rcTabs.top;  

    //adjust tabs windows position          
    for (int i = 0; i < TAB_MAX; i++)
    {
        if (m_pPage[i] != NULL &&
            m_pPage[i]->m_hWnd != NULL)
        {
            
            ::DeferWindowPos(hdwp, 
                m_pPage[i]->m_hWnd, 
                HWND_TOP, 
                cx, 
                cy, 
                iPageWidth, 
                iPageHeight, 
                0); 
        }
    } 


    ::EndDeferWindowPos(hdwp);
}

void DemoManager::DoSwitchTab(WPARAM wParam, LPARAM lParam)
{
    LPNMHDR lpnmhdr = (LPNMHDR)lParam;            
    ASSERT(lpnmhdr != NULL);

    if ((lpnmhdr->hwndFrom == m_hTabWnd)
        && (lpnmhdr->idFrom == IDC_TAB_GNSS)
        && (lpnmhdr->code == TCN_SELCHANGE))
    {
        int index = TabCtrl_GetCurSel(m_hTabWnd);
        m_pPage[m_iTabIndex]->DeActivate();
        m_pPage[index]->Activate();
        m_iTabIndex = index;
    }
}

void DemoManager::DoCommand(WPARAM wParam, LPARAM lParam)
{
    HMENU hMenu = NULL;
    MENUITEMINFO mii = {0};     
    int wmId    = LOWORD(wParam); 
    int wmEvent = HIWORD(wParam); 

    hMenu = ::CommandBar_GetMenu(m_hWndCommandBar, 0);
    ASSERT(hMenu != NULL);

    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STATE;
    ::GetMenuItemInfo(hMenu, wmId, FALSE, &mii);    

    BOOL fgPressed = 0;
    switch (wmId)
    { 
    case ID_FILE_FULLSRCEEN:
        if (g_tEntryData.m_fgFullSreen)
        {
            if (!(mii.fState & MF_CHECKED))
            {
                ::CheckMenuItem(hMenu, wmId, MF_CHECKED);
                ::CheckMenuItem(hMenu, ID_FILE_NORMAL, MF_UNCHECKED);
            }
        }
        else
        {
            ::CheckMenuItem(hMenu, wmId, MF_CHECKED);
            ::CheckMenuItem(hMenu, ID_FILE_NORMAL, MF_UNCHECKED);

            ::PostMessage(g_tEntryData.m_hMainWnd, APP_RESIZE_MSG, 0, 0);
        }
        break;

    case ID_FILE_NORMAL:
        if (g_tEntryData.m_fgFullSreen)
        {
            ::CheckMenuItem(hMenu, wmId, MF_CHECKED);
            ::CheckMenuItem(hMenu, ID_FILE_FULLSRCEEN, MF_UNCHECKED);

            ::PostMessage(g_tEntryData.m_hMainWnd, APP_RESIZE_MSG, 0, 0);
        }
        else
        {
            if (!(mii.fState & MF_CHECKED))
            {
                ::CheckMenuItem(hMenu, wmId, MF_CHECKED);
                ::CheckMenuItem(hMenu, ID_FILE_FULLSRCEEN, MF_UNCHECKED);
            }
        }
        break;

    case ID_FILE_SAVELOG:
        if ((mii.fState & MF_CHECKED))
        {
            ::CheckMenuItem(hMenu, wmId, MF_UNCHECKED);
        }
        else
        {
            ::CheckMenuItem(hMenu, wmId, MF_CHECKED);
        }
        m_pPage[TAB_COMM]->DoCommand(wParam, lParam);
        break;

    case ID_FILE_DEBUGLOG:
        {
            #define FILE_DEVICE_MTK_GPS 0x9000
            #define IOCTL_MTK_SET_DEBUG  \
                CTL_CODE(FILE_DEVICE_MTK_GPS, 0x902, METHOD_BUFFERED, FILE_ANY_ACCESS)

            DWORD dwEnable = 0;
            if ((mii.fState & MF_CHECKED))
            {
                ::CheckMenuItem(hMenu, wmId, MF_UNCHECKED);
                dwEnable = 0;
            }
            else
            {
                ::CheckMenuItem(hMenu, wmId, MF_CHECKED);
                dwEnable = 1;
            }

            HANDLE hGPS = ::CreateFile(
                _T("GPS1:"),
                GENERIC_READ | GENERIC_WRITE,	//允许读和写
                0,								//独占方式（共享模式）
                NULL,
                OPEN_EXISTING,					//打开而不是创建（创建方式）
                0,
                NULL 
                );
            if (INVALID_HANDLE_VALUE != hGPS)
            {               
                SYSTEMTIME sysTime = {0};
                TCHAR szPath[MAX_PATH] = {0};
                ::GetSystemTime(&sysTime);
                GetCurrentDirectory(szPath, MAX_PATH); 
                _stprintf(szPath, _T("%sGPS_debug_log_%04d%02d%02d%02d%02d%02d.txt"), 
                    szPath,
                    sysTime.wYear, 
                    sysTime.wMonth,
                    sysTime.wDay,
                    sysTime.wHour,
                    sysTime.wMinute,
                    sysTime.wSecond);

                ::DeviceIoControl(hGPS, 
                    IOCTL_MTK_SET_DEBUG,
                    &dwEnable, sizeof(DWORD),
                    szPath, sizeof(szPath), NULL, 
                    NULL);

                ::CloseHandle(hGPS);
            }

        }
        break;

    case ID_EDIT_CLEARRX:
    case ID_EDIT_CLEARTX:
    case ID_EDIT_CLEARALL:      
    case ID_COMMAND_CONNECT:   
    case ID_COMMAND_DISCONNECT:
        m_pPage[TAB_COMM]->DoCommand(wParam, lParam);
        RefreshUI(wmId);    
        break;

    case ID_COMMAND_SETTINGS:
        m_pPage[TAB_COMM]->DoCommand(wParam, lParam);
        break;

    case ID_COMMAND_COLD_BOOT:
    case ID_COMMAND_WARM_BOOT:       
    case ID_COMMAND_HOT_BOOT:       
    case ID_COMMAND_FULLCOLD_BOOT:
        m_pPage[TAB_COMM]->DoCommand(wParam, lParam);
        m_pPage[TAB_TRACK]->DoCommand(wParam, lParam);
        break;    
    }
}

void DemoManager::DoDestroy(WPARAM wParam, LPARAM lParam)
{
    if (m_hWndCommandBar)
    {
        ::CommandBar_Destroy(m_hWndCommandBar);
    }

    if (m_hStatusWnd)
    {
        ::DestroyWindow(m_hStatusWnd);
    }

    for (int i = 0; i < TAB_MAX; i++)
    {
        if (m_pPage[i] != NULL)
        {
            m_pPage[i]->DeInit();
            delete m_pPage[i];
            m_pPage[i] = NULL;
        }
    }

    ::EndDialog(m_hDlgWnd, 0);     
}

BOOL DemoManager::CreateTabs(void)
{
    HWND hwndTab = NULL;

    ASSERT(m_hDlgWnd != NULL);

    hwndTab = ::GetDlgItem(g_tEntryData.m_hMainWnd, IDC_TAB_GNSS);
    if (NULL == hwndTab)
    {
        return FALSE;
    }
    else
    {
        m_hTabWnd = hwndTab;
    }

    for (int i = 0; i < TAB_MAX; i++)
    {
        switch (i)
        {
        case TAB_COMM:
            m_pPage[i] = new CCommPage;  
            if (NULL == m_pPage[i] ||
                !m_pPage[i]->Init(hwndTab))
            {
                return FALSE;
            }
            break;

        case TAB_TRACK:
            m_pPage[i] = new CTrackPage;  
            if (NULL == m_pPage[i] ||
                !m_pPage[i]->Init(hwndTab))
            {
                return FALSE;
            }
            break;        
        }  

    }

    TabCtrl_SetCurSel(m_hTabWnd, TAB_COMM);
    m_iTabIndex = TAB_COMM;
    m_pPage[TAB_COMM]->Activate();

    return TRUE;           

}

void DemoManager::RefreshUI(WPARAM wParam, LPARAM lParam)
{
    ASSERT(m_hWndCommandBar != NULL && m_hStatusWnd != NULL);
    
    CCommPage *pCommPage = (CCommPage *)m_pPage[TAB_COMM];
    int ePortStatus = pCommPage->GetPortStatus();
    int wmId = (int)wParam;

    switch (wmId)
    {
    case ID_EDIT_CLEARRX:
        ::SendMessage(m_hStatusWnd, SB_SETTEXT, 0, (LPARAM)_T(""));
        break;

    case ID_EDIT_CLEARTX:
        ::SendMessage(m_hStatusWnd, SB_SETTEXT, 1, (LPARAM)_T(""));
        break;

    case ID_EDIT_CLEARALL:
        ::SendMessage(m_hStatusWnd, SB_SETTEXT, 0, (LPARAM)_T(""));
        ::SendMessage(m_hStatusWnd, SB_SETTEXT, 1, (LPARAM)_T(""));
        break;

    case ID_COMMAND_CONNECT:
        if (PORT_CONNECTED == ePortStatus)
        {
            //update connect/disconnect status
            ::SendMessage(m_hWndCommandBar, TB_PRESSBUTTON, ID_COMMAND_CONNECT, TRUE);       
            ::SendMessage(m_hWndCommandBar, TB_PRESSBUTTON, ID_COMMAND_DISCONNECT, FALSE);

            //enable boot command button
            ::SendMessage(m_hWndCommandBar, TB_ENABLEBUTTON, ID_COMMAND_COLD_BOOT, TRUE);
            ::SendMessage(m_hWndCommandBar, TB_ENABLEBUTTON, ID_COMMAND_WARM_BOOT, TRUE);
            ::SendMessage(m_hWndCommandBar, TB_ENABLEBUTTON, ID_COMMAND_HOT_BOOT, TRUE);
            ::SendMessage(m_hWndCommandBar, TB_ENABLEBUTTON, ID_COMMAND_FULLCOLD_BOOT, TRUE);

            //update status bar
            TCHAR szFormat[64] = {0};
            COMM_DCB_T commDcb = {0};
            pCommPage->GetPortProperty(commDcb);
            _stprintf(szFormat, _T("COM%d Baudrate:%u"), commDcb.portNo, commDcb.baud);
            ::SendMessage(m_hStatusWnd, SB_SETTEXT, 2, (LPARAM)szFormat);
        }
        break;

    case ID_COMMAND_DISCONNECT:
        if (PORT_DISCONNECTED == ePortStatus)
        {
            //disable boot command button
            ::SendMessage(m_hWndCommandBar, TB_ENABLEBUTTON, ID_COMMAND_COLD_BOOT, FALSE);
            ::SendMessage(m_hWndCommandBar, TB_ENABLEBUTTON, ID_COMMAND_WARM_BOOT, FALSE);
            ::SendMessage(m_hWndCommandBar, TB_ENABLEBUTTON, ID_COMMAND_HOT_BOOT, FALSE);
            ::SendMessage(m_hWndCommandBar, TB_ENABLEBUTTON, ID_COMMAND_FULLCOLD_BOOT, FALSE);

            //update connect/disconnect status
            ::SendMessage(m_hWndCommandBar, TB_PRESSBUTTON, ID_COMMAND_CONNECT, FALSE);       
            ::SendMessage(m_hWndCommandBar, TB_PRESSBUTTON, ID_COMMAND_DISCONNECT, TRUE);

            //update status bar
            ::SendMessage(m_hStatusWnd, SB_SETTEXT, 2, (LPARAM)_T(""));
        }
        break;

    case UART_RX_DATA_MSG:
        if (PORT_COMMUNICATE == ePortStatus)
        {           
            ::SendMessage(m_hStatusWnd, SB_SETTEXT, 0, lParam);
        }
        break;

    case UART_TX_DATA_MSG:
        if (PORT_COMMUNICATE == ePortStatus)
        {           
            ::SendMessage(m_hStatusWnd, SB_SETTEXT, 1, lParam);
        }
        break;

    default:
        if (PORT_IDLE == ePortStatus)
        {
            ::SendMessage(m_hWndCommandBar, TB_PRESSBUTTON, ID_COMMAND_CONNECT, FALSE);
            ::SendMessage(m_hWndCommandBar, TB_PRESSBUTTON, ID_COMMAND_DISCONNECT, TRUE);
            ::SendMessage(m_hWndCommandBar, TB_ENABLEBUTTON, ID_COMMAND_COLD_BOOT, FALSE);
            ::SendMessage(m_hWndCommandBar, TB_ENABLEBUTTON, ID_COMMAND_WARM_BOOT, FALSE);
            ::SendMessage(m_hWndCommandBar, TB_ENABLEBUTTON, ID_COMMAND_HOT_BOOT, FALSE);
            ::SendMessage(m_hWndCommandBar, TB_ENABLEBUTTON, ID_COMMAND_FULLCOLD_BOOT, FALSE);            
        }
        break;
    }    
}