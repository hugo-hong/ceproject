// EBTMini.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "EBTMini.h"
#include <windows.h>
#include <commctrl.h>
#include "ecommon.h"
#include "PairPage.h"
#include "DialPage.h"
#include "MusicPage.h"
#include "PhonebookPage.h"

extern HWND	g_hWndCommandBar;	// command bar handle
extern INSTANCE_ENTRYDATA g_tEntryData;

void CALLBACK OnNotifyBTMsg(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{    
    ASSERT(g_tEntryData.m_hMainWnd != NULL);
    PostMessage(g_tEntryData.m_hMainWnd, BT_MW_MSG, 0, 0);
}

void CALLBACK BT_TimerProc(HWND hwnd, 
                        UINT uMsg, 
                        UINT idEvent, 
                        DWORD dwTime
                        )
{
    //add code here
    switch (idEvent)
    {
    case OPEN_TIMER:
        {
            if (STATE_INIT == EBTManager::GetInstance()->GetBTState())
            {
                //register callback 
                EBTManager::GetInstance()->RegisterCallBack();
            }       

            //open bt
            E_BT_STATUS_T  eBTStatus;
            BZ_GetBlueToothStatus(eBTStatus);
            if (MW_BT_STATUS_OFF == eBTStatus)
            {
                BZ_OpenBlueTooth(TRUE);
            }
        }                
        break;

    case CLOSE_TIMER:
        {
            //close bt
            E_BT_STATUS_T  eBTStatus;
            BZ_GetBlueToothStatus(eBTStatus);
            if (MW_BT_STATUS_ON == eBTStatus &&
                (EBTManager::GetInstance()->GetBTState() & STATE_OPEN))
            {
                BZ_CloseBlueTooth(TRUE);
            }
        }        
        break;
    }
}


EBTManager::EBTManager()
{
    m_hDlgWnd = NULL;
    m_hTabWnd = NULL;    
    m_iTabIndex = 0;    

    m_eBTState = STATE_UNKNOW;
}

EBTManager::~EBTManager()
{

}

void EBTManager::DoInitDialog(WPARAM wParam, LPARAM lParam)
{
    
    ASSERT(m_hDlgWnd != NULL);

    ASSERT(g_tEntryData.m_hMainWnd != NULL);

    PostMessage(g_tEntryData.m_hMainWnd, APP_RESIZE_MSG, 0, 0);
    
    if (!CreateTabs())
    {
        MessageBox(m_hDlgWnd, _T("Application init failed."), NULL, MB_ICONERROR);
        PostMessage(g_tEntryData.m_hMainWnd, WM_QUIT, 0, 0);        
    }
    else
    {
        RefreshUI();
    }
}

void EBTManager::DoResizeDialog(WPARAM wParam, LPARAM lParam)
{
    HDWP hdwp = NULL; 
    RECT rcTabs = {0};  
    RECT rcMenuBar = {0};
    int iWidth = LOWORD(lParam);
    int iHeight = HIWORD(lParam);    
    int iTabWidth = 0;
    int iTabHeight = 0;
    int iPageWidth = 0;
    int iPageHeight = 0;    
  
    if (NULL == m_hTabWnd)
    {
        return;
    }        

    //get menu bar size
    if (g_hWndCommandBar)
    {
        GetWindowRect(g_hWndCommandBar, &rcMenuBar);
    }
    MapWindowPoints(HWND_DESKTOP, m_hDlgWnd, (LPPOINT) &rcMenuBar, 2);

    // Size the tab control to fit the client area. 
    iTabWidth = iWidth - rcMenuBar.left * 2;
    iTabHeight = iHeight - (rcMenuBar.bottom-rcMenuBar.top) - DEFAULT_SPACE;     

    SetRect(&rcTabs, 
        rcMenuBar.left,
        rcMenuBar.bottom + DEFAULT_SPACE, 
        rcTabs.left + iTabWidth,
        rcTabs.top + iTabHeight);     

    hdwp = BeginDeferWindowPos(TAB_MAX + 1);    

    DeferWindowPos(hdwp, 
        m_hTabWnd, 
        NULL,//HWND_TOP, 
        rcTabs.left, 
        rcTabs.top, 
        iTabWidth, 
        iTabHeight, 
        0);    

    //get tab display area size
    TabCtrl_AdjustRect(m_hTabWnd, FALSE, &rcTabs); 

    iPageWidth = rcTabs.right - rcTabs.left;
    iPageHeight = rcTabs.bottom - rcTabs.top;  

    //adjust tabs windows position          
    for (int i = 0; i < TAB_MAX; i++)
    {
        if (m_pPage[i] != NULL &&
            m_pPage[i]->m_hWnd != NULL)
        {
            DeferWindowPos(hdwp, 
                m_pPage[i]->m_hWnd, 
                HWND_TOP, 
                rcTabs.left, 
                rcTabs.top, 
                iPageWidth, 
                iPageHeight, 
                0); 
        }
    } 


    EndDeferWindowPos(hdwp);
}

void EBTManager::DoSwitchTab(WPARAM wParam, LPARAM lParam)
{
    LPNMHDR lpnmhdr = (LPNMHDR)lParam;            
    ASSERT(lpnmhdr != NULL);

    if ((lpnmhdr->hwndFrom == m_hTabWnd)
        && (lpnmhdr->idFrom == IDC_TAB_BT)
        && (lpnmhdr->code == TCN_SELCHANGE))
    {
        int index = TabCtrl_GetCurSel(m_hTabWnd);
        m_pPage[m_iTabIndex]->DeActivate();
        m_pPage[index]->Activate();
        m_iTabIndex = index;
    }
}

void EBTManager::RefreshUI(void)
{
    if (STATE_UNKNOW == m_eBTState)
    {
        ShowWindow(m_hTabWnd, SW_HIDE);
    }
    else if ((STATE_INIT | STATE_OPEN) == m_eBTState)
    {
        ShowWindow(m_hTabWnd, SW_SHOWNORMAL);

        //first open bt
        for (int i = 0; i < TAB_MAX; i++)
        {
            if (m_pPage[i] != NULL)
            {
                m_pPage[i]->RegisterCallBack();
            }
        }

        m_pPage[TAB_PAIR]->Activate();
    }
    else if ((STATE_INIT | STATE_OPEN | STATE_CONNECT) == m_eBTState)
    {
        if (m_pPage[TAB_PAIR])
        {
            m_pPage[TAB_PAIR]->RefreshUI(STATE_CONNECT);
        }        
    }
    else if (m_eBTState & STATE_CLOSE)
    {
        ShowWindow(m_hTabWnd, SW_HIDE);
    }
}

void EBTManager::SetDebugLog(int idLog, DWORD dwLogMask)
{      
    CEReg reg;
        
    LPCTSTR lpSubKey = _T("SOFTWARE\\Microsoft\\Bluetooth\\Sys\\BZLog");
    if (!reg.Open(HKEY_LOCAL_MACHINE, lpSubKey))
    {
        return;
    }       

    switch (idLog)
    {
    case ID_DEBUG_BCH:
        reg.WriteDWORD(_T("BCH"), dwLogMask);
        break;

    case ID_DEBUG_BM:
        reg.WriteDWORD(_T("BM"), dwLogMask);
        break;

    case ID_DEBUG_BCM:
        reg.WriteDWORD(_T("BCM"), dwLogMask);
        break;

    case ID_DEBUG_BCP:
        reg.WriteDWORD(_T("BCP"), dwLogMask);
        break;

    case ID_DEBUG_BCS:
        reg.WriteDWORD(_T("BCS"), dwLogMask);
        break;

    case ID_DEBUG_BCAT:
        reg.WriteDWORD(_T("BCATData"), dwLogMask);
        break;

    case ID_DEBUG_BCMUSIC:
        reg.WriteDWORD(_T("BCMusic"), dwLogMask);
        break;

    case ID_DEBUG_BCMW:
        reg.WriteDWORD(_T("BZMWAPP"), dwLogMask);
        break;
    } 

    reg.Close();
}


BOOL EBTManager::CreateTabs(void)
{
    HWND hwndTab = NULL;
    
    ASSERT(m_hDlgWnd != NULL);

    hwndTab = GetDlgItem(g_tEntryData.m_hMainWnd, IDC_TAB_BT);
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
        case TAB_PAIR:
            m_pPage[i] = new CPairPage;  
            if (NULL == m_pPage[i] ||
                !m_pPage[i]->Init(hwndTab))
            {
                return FALSE;
            }
            break;

        case TAB_DIAL:
            m_pPage[i] = new CDialPage;  
            if (NULL == m_pPage[i] ||
                !m_pPage[i]->Init(hwndTab))
            {
                return FALSE;
            }
            break;

        case TAB_MUSIC:
            m_pPage[i] = new CMusicPage;     
            if (NULL == m_pPage[i] ||
                !m_pPage[i]->Init(hwndTab))
            {
                return FALSE;
            }
            break;

        case TAB_PHONEBOOK:
            m_pPage[i] = new CPoneBookPage;    
            if (NULL == m_pPage[i] ||
                !m_pPage[i]->Init(hwndTab))
            {
                return FALSE;
            }
            break;
        }  

    }

    TabCtrl_SetCurSel(m_hTabWnd, TAB_PAIR);
    m_iTabIndex = TAB_PAIR;
    m_pPage[TAB_PAIR]->Activate();

    return TRUE;           

}


BOOL EBTManager::OpenBTDevice(void)
{
    HRESULT hr = 0;

    ASSERT(g_tEntryData.m_hMainWnd != NULL);    

    if (STATE_UNKNOW == m_eBTState)
    {
        hr = BZ_Init(OnNotifyBTMsg, BZ_BLUZEON_ALL_MASK, 0);
        if (BZE_OK != hr)
        {
            MessageBox(g_tEntryData.m_hMainWnd, _T("BT open failed."), NULL, MB_ICONERROR);
            return FALSE;
        }
        else
        {
            m_eBTState = STATE_INIT;
            BZ_VerifySCOAudioChannel(FALSE); //Initially disable SCO.
        }
    }   

    ::SetTimer(m_hDlgWnd, 0, OPEN_TIMER, BT_TimerProc);

    return TRUE;
}

BOOL EBTManager::CloseBTDevice(void)
{
    ::SetTimer(m_hDlgWnd, 0, CLOSE_TIMER, BT_TimerProc);

    return TRUE;
}


void EBTManager::RegisterCallBack(void)
{
    BZ_RegBTCallBack(ID_MAIN, BZ_MODULE_OCBT, EBTManager::OnBTSwitch);
    BZ_RegBTCallBack(ID_MAIN, BZ_MODULE_CONNECTION, EBTManager::OnBTConnect);
}


void EBTManager::OnBTSwitch(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
    EBTManager *pThis = EBTManager::GetInstance();
    ASSERT(pThis != NULL);

    switch(dwIndID)
    {
    case BZ_IND_OPEN_BLUETOOTH_SUCCESS:
        pThis->SetBTState(STATE_OPEN);
        pThis->RefreshUI(); 
        break;

    case BZ_IND_ClOSE_BLUETOOTH_SUCCESS:
        pThis->ClearBTState(STATE_OPEN);
        pThis->SetBTState(STATE_CLOSE);
        pThis->RefreshUI(); 
        break;

    case BZ_IND_AUTO_CONNECT:
        {
            
            //BZ_Connect();
        }
        break;

    default:
        break;

    }
}

void EBTManager::OnBTConnect(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
    EBTManager *pThis = EBTManager::GetInstance();
    ASSERT(pThis != NULL);

    switch (dwIndID)
    {
    case BZ_IND_CREATE_CONNECT_SUCCESS:
        {
            CPairPage *pPairPage = NULL;
            pThis->SetBTState(STATE_CONNECT);
            pThis->RefreshUI();
            BZ_ClrDevNameFrmSrch();	
            pPairPage = (CPairPage *)pThis->GetSubInstance(TAB_PAIR);
        }
        
        break;

    case BZ_IND_CREATE_CONNECT_FAIL:
        break;

    case BZ_IND_DISCONNECT_SUCCESS:
        break;

    case BZ_IND_CONNECT_STATUS_CHANGED:
        break;
    }
}





