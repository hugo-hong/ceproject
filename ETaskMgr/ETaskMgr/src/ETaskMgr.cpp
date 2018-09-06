// ETaskMgr.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ETaskMgr.h"
#include <windows.h>
#include <commctrl.h>
#include "processes.h"
#include "eutil.h"

#define MAX_LOADSTRING 100

#define TASK_TIMER_ID   1
#define TASK_INTERVAL   2000//ms



// Global Variables:
HINSTANCE			g_hInst;			// current instance
HWND				g_hWndCommandBar;	// command bar handle

TASK_PARAM g_taskParam;

// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL DoInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam);
void DoResizeDialog(HWND hDlg, WPARAM wParam, LPARAM lParam);
BOOL CreateTabs(void);


/********************************************************************************
 WinMain entry point
*********************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	MSG msg;

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	//HACCEL hAccelTable;
	//hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ETASKMGR));

	//// Main message loop:
	//while (GetMessage(&msg, NULL, 0, 0)) 
	//{
	//	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}

	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ETASKMGR));
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{    
    BOOL fgRet = 0;
    TCHAR szTitle[MAX_LOADSTRING];		// title bar text
    TCHAR szWindowClass[MAX_LOADSTRING];	// main window class name

    //Init globel variable
    memset(&g_taskParam, 0, sizeof(TASK_PARAM));
    g_taskParam.m_hInstance = g_hInst = hInstance; // Store instance handle in our global variable 

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
    LoadString(hInstance, IDC_ETASKMGR, szWindowClass, MAX_LOADSTRING);


   /* if (!MyRegisterClass(hInstance, szWindowClass))
    {
    	return FALSE;
    }*/

   /* HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }*/

    //------------ Prevent multiple instance  ------------------
    HANDLE hMutex = CreateMutex(NULL, FALSE, szTitle);
    if( GetLastError() == ERROR_ALREADY_EXISTS )
    {
        HWND hWnd = FindWindow( szWindowClass , szTitle);
        if( hWnd )
        {
            ShowWindow(hWnd, SW_SHOWNORMAL);
            SetForegroundWindow(hWnd);
        }

        return FALSE; 
    }

    // Ensure that the common control DLL is loaded. 
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC  = ICC_LISTVIEW_CLASSES | ICC_TAB_CLASSES | ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex); 

    g_taskParam.m_pPage[TAB_PROCESS] = new CProcesses;
    if (NULL == g_taskParam.m_pPage[TAB_PROCESS])
    {
        goto EXIT;
    }



    /*ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    if (g_hWndCommandBar)
    {
        CommandBar_Show(g_hWndCommandBar, TRUE);
    }*/

    DialogBox(hInstance, (LPCTSTR)IDD_MAINDLG, NULL, (DLGPROC)MainDlgProc);

    fgRet = TRUE;

EXIT:
    if (hMutex)
    {
        CloseHandle(hMutex);
        hMutex = NULL;
    }

    if (g_taskParam.m_pPage[TAB_APP] != NULL)
    {
        delete (CProcesses *)g_taskParam.m_pPage[TAB_APP];
    }    

    if (g_taskParam.m_pPage[TAB_PROCESS] != NULL)
    {
        delete (CProcesses *)g_taskParam.m_pPage[TAB_PROCESS];
    }

    if (g_taskParam.m_pPage[TAB_PERFORMANCE] != NULL)
    {
        delete (CProcesses *)g_taskParam.m_pPage[TAB_PERFORMANCE];
    }

    return fgRet;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

	
    switch (message) 
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam); 
            wmEvent = HIWORD(wParam); 
            // Parse the menu selections:
            switch (wmId)
            {
                case IDM_HELP_ABOUT:
                    DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, About);
                    break;
                case IDM_FILE_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_CREATE:
            g_hWndCommandBar = CommandBar_Create(g_hInst, hWnd, 1);
            CommandBar_InsertMenubar(g_hWndCommandBar, g_hInst, IDR_MENU, 0);
            CommandBar_AddAdornments(g_hWndCommandBar, 0, 0);
            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            
            // TODO: Add any drawing code here...
            
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            CommandBar_Destroy(g_hWndCommandBar);
            PostQuitMessage(0);
            break;


        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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



BOOL CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return DoInitDialog(hWnd, wParam, lParam);
        break;

    case WM_NOTIFY:
        {
            LPNMHDR lpnmhdr = (LPNMHDR)lParam;
            ASSERT(lpnmhdr != NULL);
            if( (lpnmhdr->hwndFrom == g_taskParam.m_hTabWnd)
                && (lpnmhdr->idFrom == IDC_TAB_TASK)
                && (lpnmhdr->code == TCN_SELCHANGE))
            {
                g_taskParam.m_uTabIndex = TabCtrl_GetCurSel(g_taskParam.m_hTabWnd);
                switch( g_taskParam.m_uTabIndex)
                {
                case TAB_APP:
                    
                    break;

                case TAB_PROCESS:
                    
                    break;

                case TAB_PERFORMANCE:
                  
                    break;
                }
            }
        }
        
        break;

    case WM_SIZE:
        DoResizeDialog(hWnd, wParam, lParam);
        break;

    case WM_TIMER:
        PostMessage(g_taskParam.m_pPage[g_taskParam.m_uTabIndex]->m_hDlgWnd,
            WM_TIMER,
            wParam,
            lParam);
        break;

    case WM_CLOSE:
        EndDialog(hWnd, 0);        
        break;

    case WM_DESTROY:
        KillTimer(hWnd, TASK_TIMER_ID);
        waveOutMessage((HWAVEOUT)0, MM_WAV_NOTIFY_PROCESS_ID, (DWORD)NULL, (DWORD)MM_WAV_AUDIO_PID);
        break;

    case MM_WAV_AUDIO_PID:
        RETAILMSG(1, (_T("[ETaskMgr]:wave out PID=0X%X\r\n"), wParam));
        break;
    }

    return FALSE;
}

BOOL DoInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
    g_taskParam.m_hMainDlg = hDlg;
    g_taskParam.m_dwInterval = TASK_INTERVAL;

    //get system information
    GetSystemInfo(&g_taskParam.m_sysInfo);    

    RECT rcWorkArea = {0};
    if ( SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, FALSE))
    {
        RECT rcDlg;
        int dlgWidth, dlgHeight;	// dialog width and height in pixel units
        int sysWidth, sysHeight; // system width and height in pixel units

        GetWindowRect(hDlg, &rcDlg);
        dlgWidth = rcDlg.right - rcDlg.left;
        dlgHeight = rcDlg.bottom - rcDlg.top;
        sysWidth = rcWorkArea.right - rcWorkArea.left;
        sysHeight = rcWorkArea.bottom - rcWorkArea.top;            

        MoveWindow(hDlg,
            sysWidth>dlgWidth?((sysWidth - dlgWidth)/2):0,
            sysHeight>dlgHeight?((sysHeight - dlgHeight)/2):0,
            sysWidth>dlgWidth?dlgWidth:sysWidth,
            sysHeight>dlgHeight?dlgHeight:sysHeight,
            FALSE );
    }

    if (!CreateTabs())
    {
        MessageBox(hDlg, _T("Application init failed."), NULL, MB_ICONERROR);
        EndDialog(hDlg, 0);
        return FALSE;
    }

    g_taskParam.m_uTabIndex = TAB_PROCESS;

    // Create the status window
    g_taskParam.m_hStatusWnd = CreateStatusWindow(WS_CHILD| WS_VISIBLE | SBT_OWNERDRAW,
        NULL,
        hDlg,
        IDC_STATUSWND);
    if (NULL == g_taskParam.m_hStatusWnd)
    {
        return FALSE;
    }

    HDC hdc = GetDC(NULL);
    INT nInch = GetDeviceCaps(hdc, LOGPIXELSX);
    ReleaseDC(NULL, hdc);

    int ciParts[] = {nInch, 
        ciParts[0] + (nInch * 5) / 4, 
        ciParts[1] + (nInch * 5) / 2, 
        -1};

    if (g_taskParam.m_hStatusWnd) 
    {
        SendMessage(g_taskParam.m_hStatusWnd, SB_SETPARTS, DIM(ciParts), (LPARAM)ciParts);
    }

    SetTimer(hDlg, TASK_TIMER_ID, g_taskParam.m_dwInterval, NULL);

    //notify waveform 
    waveOutMessage((HWAVEOUT)0, MM_WAV_NOTIFY_PROCESS_ID, (DWORD)hDlg, (DWORD)MM_WAV_AUDIO_PID);

    return TRUE;
}

void DoResizeDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
    HDWP hdwp = NULL; 
    RECT rcTabs = {0};     
    int nWidth = LOWORD(lParam);
    int nHeight = HIWORD(lParam); 
    int xPoint = 0;
    int yPoint = 0;

    if (NULL == g_taskParam.m_hTabWnd ||
        NULL == g_taskParam.m_hStatusWnd)
    {
        return;
    }        

    GetClientRect(g_taskParam.m_hTabWnd, &rcTabs);
    MapWindowPoints(g_taskParam.m_hTabWnd, hDlg, (LPPOINT) &rcTabs, 2);
    xPoint = rcTabs.left;
    yPoint = rcTabs.top;

    SetRect(&rcTabs, 0, 0, nWidth, nHeight);     
    TabCtrl_AdjustRect(g_taskParam.m_hTabWnd, FALSE, &rcTabs); 

    nWidth = rcTabs.right - rcTabs.left;
    nHeight = rcTabs.bottom - rcTabs.top;

    // Size the tab control to fit the client area. 
    hdwp = BeginDeferWindowPos(TAB_MAX + 1);    

    DeferWindowPos(hdwp, 
        g_taskParam.m_hTabWnd, 
        NULL, 
        xPoint, 
        yPoint, 
        nWidth, 
        nHeight, 
        SWP_NOMOVE | SWP_NOZORDER); 

    //adjust tabs windows position
    MapWindowPoints(g_taskParam.m_hTabWnd, hDlg, (LPPOINT) &rcTabs, 2); 
    for (int i = 0; i < TAB_MAX; i++)
    {
        if (g_taskParam.m_pPage[i] != NULL &&
            g_taskParam.m_pPage[i]->m_hDlgWnd != NULL)
        {
            DeferWindowPos(hdwp, 
                g_taskParam.m_pPage[i]->m_hDlgWnd, 
                HWND_TOP, 
                rcTabs.left, 
                rcTabs.top, 
                nWidth - DEFAULT_SPACE - DEFAULT_SPACE, 
                nHeight - (rcTabs.top - yPoint) - DEFAULT_SPACE, 
                0); 

        }
    } 


    EndDeferWindowPos(hdwp);
}

BOOL CreateTabs(void)
{
    HWND hwndTab = NULL;

    hwndTab = GetDlgItem(g_taskParam.m_hMainDlg, IDC_TAB_TASK);
    if (NULL == hwndTab)
    {
        return FALSE;
    }
    else
    {
        g_taskParam.m_hTabWnd = hwndTab;
    }
  

    for (int i = 0; i < TAB_MAX; i++)
    {
        if (g_taskParam.m_pPage[i] != NULL)
        {
            g_taskParam.m_pPage[i]->Init();
        }         
    }    

    return TRUE;           

}