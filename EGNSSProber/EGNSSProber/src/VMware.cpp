#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>
#include "ecommon.h"
#include "EGNSSProber.h"

#define MAX_LOADSTRING 100

// Global Variables:

INSTANCE_ENTRYDATA g_tEntryData = {0};

DemoManager DemoManager::m_Instance;

//function declaration
BOOL CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

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
        ::KillTimer(hWnd, TOUCHREPEAT_EVT);
        break;

    case WM_MOVE:
        ::SetTimer(hWnd, TOUCHMOVE_EVT, 100, TouchEventProc);
        break;
    }
}

void OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{    
    g_tEntryData.m_hMainWnd = hWnd;
    DemoManager::GetInstance()->SetWindow(hWnd);
    DemoManager::GetInstance()->DoInitDialog(wParam, lParam);    
}

void OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{     
    DemoManager::GetInstance()->DoCommand(wParam, lParam);  
}

void OnSize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{    
    DemoManager::GetInstance()->DoResizeDialog(wParam, lParam);    
}

void OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
   DemoManager::GetInstance()->DoSwitchTab(wParam, lParam); 
}

void OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    DemoManager::GetInstance()->DoDestroy(wParam, lParam);
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
    TCHAR szTitle[MAX_LOADSTRING];		    // title bar text
    TCHAR szWindowClass[MAX_LOADSTRING];	// main window class name  


    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_EGNSSPROBER, szWindowClass, MAX_LOADSTRING);

#if 0
    //------------ Prevent multiple instance  ------------------
    hWnd = FindWindow(szWindowClass , szTitle);
    if( hWnd != NULL)
    {
        ShowWindow(hWnd, SW_SHOWNORMAL);
        SetForegroundWindow(hWnd);
        return FALSE; 
    }
#endif

    g_tEntryData.m_hInstance = hInstance;
    g_tEntryData.m_pParam = (LPVOID)(DemoManager::GetInstance());

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
            (LPCTSTR)IDD_DEMO_DLG,
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
        case IDM_HELP_ABOUT:
            DialogBox(g_tEntryData.m_hInstance, (LPCTSTR)IDD_ABOUTBOX, hWnd, About);
            break;

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
        OnClose(hWnd, wParam, lParam);           
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

    case WM_LBUTTONDBLCLK:
    case TOUCHDBCLICK_EVT:
        //OnResizeDialog(hWnd, wParam, lParam);
        if (g_tEntryData.m_fgFullSreen)
        {
            ::PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(ID_FILE_NORMAL, 0), NULL);
        }
        else
        {
            ::PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(ID_FILE_FULLSRCEEN, 0), NULL);
        }         
        return TRUE;
        break;

    case TOUCHLONG_EVT:
        return TRUE;
        break;        
  
    }

    return FALSE;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        RECT rectChild, rectParent;
        int DlgWidth, DlgHeight;	// dialog width and height in pixel units
        int NewPosX, NewPosY;

        // trying to center the About dialog
        if (GetWindowRect(hDlg, &rectChild)) 
        {
            GetClientRect(GetParent(hDlg), &rectParent);
            DlgWidth	= rectChild.right - rectChild.left;
            DlgHeight	= rectChild.bottom - rectChild.top ;
            NewPosX		= (rectParent.right - rectParent.left - DlgWidth) / 2;
            NewPosY		= (rectParent.bottom - rectParent.top - DlgHeight) / 2;

            // if the About box is larger than the physical screen 
            if (NewPosX < 0) NewPosX = 0;
            if (NewPosY < 0) NewPosY = 0;
            SetWindowPos(hDlg, 0, NewPosX, NewPosY,
                0, 0, SWP_NOZORDER | SWP_NOSIZE);
        }
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
        {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        break;

    case WM_CLOSE:
        EndDialog(hDlg, message);
        return TRUE;

    }
    return (INT_PTR)FALSE;
}