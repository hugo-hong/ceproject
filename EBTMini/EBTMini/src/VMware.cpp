#include "stdafx.h"
#include "EBTMini.h"
#include <windows.h>
#include <commctrl.h>
#include "ecommon.h"

#define MAX_LOADSTRING 100

// Global Variables:
HWND				g_hWndCommandBar;	// command bar handle

INSTANCE_ENTRYDATA g_tEntryData = {0};

EBTManager EBTManager::m_Instance;

//function declaration
BOOL CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);

//////////////////////////////////////////////////////////////////////////////////////////////

void CALLBACK TouchEventProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    switch (idEvent)
    {
    case TOUCHMOVE_EVT:
        //add code here
        break;
    }

    ::KillTimer(hWnd, idEvent);
}
void HandleClickEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static POINT s_tPt = {0};
    static DWORD s_dwTouchTime = GetTickCount();
    DWORD dwTouchTime = GetTickCount();
    POINT touchPt = {LOWORD(lParam), HIWORD(lParam)};      

    switch (message)
    {
    case WM_LBUTTONDOWN:        
        if (abs((s_tPt.x - touchPt.x)) < 5 &&
            abs((s_tPt.y - touchPt.y)) < 5 &&
            (dwTouchTime - s_dwTouchTime) < 1000)
        {
            //touch double click event
            ::PostMessage(hWnd, TOUCHDBCLICK_EVT, wParam, lParam);
        }
        s_tPt.x = touchPt.x;
        s_tPt.y = touchPt.y;
        s_dwTouchTime = dwTouchTime;
        break;

    case WM_LBUTTONUP:
        //::KillTimer(hWnd, TOUCHREPEAT_EVT);
        break;

    case WM_MOVE:
        //::SetTimer(hWnd, TOUCHMOVE_EVT, 100, TouchEventProc);
        break;
    }
}

void OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    //Add menu bar
    ASSERT(g_tEntryData.m_hInstance != NULL);
    g_hWndCommandBar = CommandBar_Create(g_tEntryData.m_hInstance, hWnd, 1);
    CommandBar_InsertMenubar(g_hWndCommandBar, g_tEntryData.m_hInstance, IDR_MENU, 0);
    //CommandBar_AddAdornments(g_hWndCommandBar, 0, 0);
    if (g_hWndCommandBar)
    {
        CommandBar_Show(g_hWndCommandBar, TRUE);
    }

    g_tEntryData.m_hMainWnd = hWnd;

    EBTManager::GetInstance()->SetWindow(hWnd);
    EBTManager::GetInstance()->DoInitDialog(wParam, lParam);    
}

void OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{    
    HMENU hMenu = NULL;
    MENUITEMINFO mii = {0};     
    int wmId    = LOWORD(wParam); 
    int wmEvent = HIWORD(wParam); 

    hMenu = CommandBar_GetMenu(g_hWndCommandBar, 0);
    ASSERT(hMenu != NULL);

    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STATE;
    GetMenuItemInfo(hMenu, wmId, FALSE, &mii);    

    switch (wmId)
    { 
    case ID_FILE_OPEN:  
        if (mii.fState & MF_CHECKED)
        {
            //do nothing
        }  
        else
        {
            CheckMenuItem(hMenu, wmId, MF_CHECKED); 
            CheckMenuItem(hMenu, ID_FILE_CLOSE, MFS_UNCHECKED);
            EBTManager::GetInstance()->OpenBTDevice();            
        }       
        break;

    case ID_FILE_CLOSE:
        if (mii.fState & MF_CHECKED)
        {
            CheckMenuItem(hMenu, ID_FILE_OPEN, MFS_UNCHECKED);   
        }
        else
        {
            CheckMenuItem(hMenu, wmId, MF_CHECKED); 
            CheckMenuItem(hMenu, ID_FILE_OPEN, MFS_UNCHECKED);
            EBTManager::GetInstance()->CloseBTDevice();     
        }
        break;

    case ID_DEBUG_BCH:
    case ID_DEBUG_BM:
    case ID_DEBUG_BCM:
    case ID_DEBUG_BCP:
    case ID_DEBUG_BCS:
    case ID_DEBUG_BCAT:
    case ID_DEBUG_BCMUSIC:
    case ID_DEBUG_BCMW:
        DWORD dwlogMask;                
        if (mii.fState & MF_CHECKED)
        {
            CheckMenuItem(hMenu, wmId, MFS_UNCHECKED);
            dwlogMask = 0;
        }
        else
        {
            CheckMenuItem(hMenu, wmId, MF_CHECKED);   
            dwlogMask = 0xFF;
        }   

        EBTManager::GetInstance()->SetDebugLog(wmId, dwlogMask);

        break;
    }

}

void OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{    
    EBTManager::GetInstance()->DoResizeDialog(wParam, lParam);    
}

void OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    EBTManager::GetInstance()->DoSwitchTab(wParam, lParam); 
}

void OnResizeDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    int dlgWidth, dlgHeight;  // dialog width and height in pixel units
    int sysWidth, sysHeight; // system width and height in pixel units
    RECT rcDlg = {0};    

    ASSERT(hWnd != NULL);

    sysWidth = g_tEntryData.m_dwWidthPixels;
    sysHeight = g_tEntryData.m_dwHeightPixels; 

    if (g_tEntryData.m_fgFullSreen)
    {
        dlgWidth = g_tEntryData.m_dwOrgWidth;
        dlgHeight = g_tEntryData.m_dwOrgHeight;

        //update status
        g_tEntryData.m_fgFullSreen = FALSE;

        MoveWindow(hWnd,
            sysWidth>dlgWidth?((sysWidth - dlgWidth)/2):0,
            sysHeight>dlgHeight?((sysHeight - dlgHeight)/2):0,
            sysWidth>dlgWidth?dlgWidth:sysWidth,
            sysHeight>dlgHeight?dlgHeight:sysHeight,
            FALSE );       
    }
    else
    {
        dlgWidth = sysWidth;
        dlgHeight = sysHeight;

        //update status
        g_tEntryData.m_fgFullSreen = TRUE;

        MoveWindow(hWnd,
            0,
            0,
            dlgWidth,
            dlgHeight,
            FALSE );        
    }
}


 
/*******************************************************************************
FUNCTION: InitInstance(HINSTANCE, int)

   PURPOSE: Saves instance handle and creates main window

   COMMENTS:

        In this function, we save the instance handle in a global variable and
        create and display the main program window.

********************************************************************************/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    TCHAR szTitle[MAX_LOADSTRING];		// title bar text
    TCHAR szWindowClass[MAX_LOADSTRING];	// main window class name  


    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_EBTMINI, szWindowClass, MAX_LOADSTRING);

    //------------ Prevent multiple instance  ------------------
    hWnd = FindWindow(szWindowClass , szTitle);
    if( hWnd != NULL)
    {
        ShowWindow(hWnd, SW_SHOWNORMAL);
        SetForegroundWindow(hWnd);
        return FALSE; 
    }

    g_tEntryData.m_hInstance = hInstance;

    // Ensure that the common control DLL is loaded. 
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC  = ICC_LISTVIEW_CLASSES | ICC_TAB_CLASSES | ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex); 

    //Get system information
    DWORD dwResolution = 0;
    DEVMODE  devMode = {0};
    devMode.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &devMode);
    g_tEntryData.m_dwWidthPixels = devMode.dmPelsWidth;
    g_tEntryData.m_dwHeightPixels = devMode.dmPelsHeight;
    g_tEntryData.m_dwPixelsBpp = devMode.dmBitsPerPel;

    dwResolution = devMode.dmPelsWidth * devMode.dmPelsHeight;
    switch (dwResolution)
    {
    case HD_SCREEN:
        g_tEntryData.m_dwOrgWidth = HD_ORG_WIDTH;
        g_tEntryData.m_dwOrgHeight = HD_ORG_HEIGHT;
        break;

    case SD_SCREEN:
        g_tEntryData.m_dwOrgWidth = SD_ORG_WIDTH;
        g_tEntryData.m_dwOrgHeight = SD_ORG_HEIGHT;
        break;
    }   

    return TRUE;
}

/*******************************************************************************
FUNCTION: WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)

PURPOSE: Instance entry

COMMENTS:

In this function, we entry application...

********************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{


    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow)) 
    {
        return FALSE;
    }

    return DialogBox(hInstance, 
            (LPCTSTR)IDD_DIALOGMAIN,
            NULL, 
            (DLGPROC)MainDlgProc);
}


/*******************************************************************************

  FUNCTION: MainDlgProc(HWND, UINT, WPARAM, LPARAM)

  PURPOSE:  Processes messages for the main window.

  WM_COMMAND	- process the application menu
  WM_PAINT	- Paint the main window
  WM_DESTROY	- post a quit message and return

********************************************************************************/
BOOL CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId = 0;
    int wmEvent = 0; 

    switch (message)
    {
    case WM_INITDIALOG:
        OnInitDialog(hWnd, wParam, lParam);
        return TRUE;
        break;

    case WM_COMMAND:
        wmId    = LOWORD(wParam); 
        wmEvent = HIWORD(wParam); 
        // Parse the menu selections:
        switch (wmId)
        {        
        case IDM_FILE_EXIT:
            DestroyWindow(hWnd);
            break;  

        default:
            OnCommand(hWnd, wParam, lParam);
            break;
        }
        break;

    case WM_NOTIFY:
        OnNotify(hWnd, wParam, lParam);
        return TRUE;
        break;

    case WM_SIZE:
        OnSize(hWnd, wParam, lParam);
        return TRUE;
        break;

    case WM_TIMER:       
        break;

    case WM_CLOSE:
        CommandBar_Destroy(g_hWndCommandBar);
        EndDialog(hWnd, 0);    
        return TRUE;
        break;

    case WM_DESTROY:        
        break;

    case WM_MOVE:

        break;

    case WM_MOUSEMOVE:
        // if (GetAsyncKeyState(VK_LBUTTON) < 0) 
        {
            HandleClickEvent(hWnd, message, wParam, lParam);
        }
        break;

    case WM_LBUTTONDOWN: 
        SetFocus(hWnd);
        SetCapture(hWnd);
        HandleClickEvent(hWnd, message, wParam, lParam);
        return TRUE;
        break;

    case WM_LBUTTONUP:
        SetFocus(hWnd);
        ReleaseCapture();        
        HandleClickEvent(hWnd, message, wParam, lParam);
        return TRUE;
        break;

    case APP_RESIZE_MSG:
        OnResizeDialog(hWnd, wParam, lParam);
        return TRUE;
        break;

    case TOUCHDBCLICK_EVT:
        OnResizeDialog(hWnd, wParam, lParam);
        return TRUE;
        break;

    case TOUCHLONG_EVT:
        return TRUE;
        break;    

    case BT_MW_MSG:
        BZ_HandleBTEventEntry();
        return TRUE;
        break;
    }

    return FALSE;
}

