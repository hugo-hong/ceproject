#include "stdafx.h"
#include "MusicPage.h"
#include "ecommon.h"
#include "EBTMini.h"

extern INSTANCE_ENTRYDATA g_tEntryData;

BOOL CALLBACK MusicDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    CMusicPage *pThis = (CMusicPage *)GetWindowLong(hDlg, GWL_USERDATA);       

    switch (message)
    {
    case WM_INITDIALOG:
        ASSERT(lParam != NULL);
        SetWindowLong(hDlg, GWL_USERDATA, lParam);  
        pThis = (CMusicPage *)lParam;
        pThis->m_hWnd = hDlg;
        pThis->DoInitDialog(wParam, lParam);
        return TRUE;
        break;

    case WM_NOTIFY:
        break;

    case WM_TIMER:
        break;

    case WM_SIZE:
        ASSERT(pThis != NULL);
        pThis->DoResizeWindow(wParam, lParam);      
        break;

    case WM_COMMAND:
        {
            int wmId    = LOWORD(wParam); 
            int wmEvent = HIWORD(wParam); 

            switch (wmId)
            {

            }
        }
        break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        SendMessage(g_tEntryData.m_hMainWnd, message, wParam, lParam);
        return TRUE;
        break;
    }



    return FALSE;
}

CMusicPage::CMusicPage()
          :CPage()
{
       
}

CMusicPage::~CMusicPage()
{

}

BOOL CMusicPage::Init(HWND hwndParent)
{
    TCITEM tie = {0};   

    ASSERT(hwndParent != NULL);

    tie.mask = TCIF_TEXT | TCIF_IMAGE;
    tie.iImage = -1;
    tie.pszText = _T("Music");
    if(-1 == TabCtrl_InsertItem(hwndParent, TAB_MUSIC, &tie))
    {
        return FALSE;
    }
    else
    {
        m_hWnd = CreateDialogParam(g_tEntryData.m_hInstance, 
            MAKEINTRESOURCE(IDD_DLG_MUSIC),
            hwndParent,
            MusicDlgProc,
            (LPARAM)this);

        //ShowWindow(m_hWnd, SW_SHOWNORMAL);
    }
    
    return TRUE;
}

void CMusicPage:: DeInit()
{
    //add code here 
}


void CMusicPage::DoInitDialog(WPARAM wParam, LPARAM lParam)
{
    //add code here     
    LPVOID plock = ENTER_LOCK(_T("Music"));

    EXIT_LOCK(plock);

}

void CMusicPage::DoResizeWindow(WPARAM wParam, LPARAM lParam)
{
    //add code here
    HDWP hdwp = NULL;
    RECT rcNew = {0};
    RECT rcWidget[8] = {0};   
    HWND hWidgetWnd[8] = {NULL};    
    int iWidth = LOWORD(lParam);
    int iHeight = HIWORD(lParam); 
    int iEditWidth = 0;
    int iEditHeight = 0;
    int iButtonWidth = 0;
    int iButtonHeight = 0;
    int cx = 0, cy = 0;      
    int dx = 0, dy = 0;
    int i = 0;

    ASSERT(m_hWnd != NULL);      

    hdwp = BeginDeferWindowPos(8);

    hWidgetWnd[0] = GetDlgItem(m_hWnd, IDC_PLAY_PREV);
    GetWindowRect(hWidgetWnd[0], &rcWidget[0]);
    iButtonWidth = rcWidget[0].right - rcWidget[0].left;
    iButtonHeight = rcWidget[0].bottom - rcWidget[0].top;

    if (g_tEntryData.m_fgFullSreen)
    {
        dx = dy = (DEFAULT_SPACE * 2);
    }
    else
    {
        dx = dy = DEFAULT_SPACE;
    }
    
    cx = (iWidth - 3 * iButtonWidth - 2 * dx) / 5;
    cy = DEFAULT_SPACE * 5;

    //adjust button position
    i = 0;
    rcNew.left = cx;
    rcNew.top = cy;
    rcNew.right = rcNew.left + iButtonWidth;
    rcNew.bottom = rcNew.top + iButtonHeight;
    rcWidget[i] = rcNew;
    DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);  

    i++;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_PLAY_PAUSE);
    rcNew.left = rcNew.right + dx;
    rcNew.top = rcNew.top;
    rcNew.right = rcNew.left + iButtonWidth;
    rcNew.bottom = rcNew.top + iButtonHeight;   
    rcWidget[i] = rcNew;
    DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);

    i++;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_PLAY_NEXT);
    rcNew.left = rcNew.right + dx;
    rcNew.top = rcNew.top;
    rcNew.right = rcNew.left + iButtonWidth;
    rcNew.bottom = rcNew.top + iButtonHeight;   
    rcWidget[i] = rcNew;
    DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);

    i++;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_PLAY_STOP);
    rcNew.left = rcWidget[1].left;
    rcNew.top = rcWidget[1].bottom + DEFAULT_SPACE;
    rcNew.right = rcNew.left + iButtonWidth;
    rcNew.bottom = rcNew.top + iButtonHeight;  
    rcWidget[i] = rcNew;
    DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);


    //adjust music name text position
    i++;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_TEXT_MUSIC_NAME);
    GetWindowRect(hWidgetWnd[i], &rcWidget[i]);
    rcNew.left = rcWidget[3].left;
    rcNew.top = rcWidget[3].bottom + DEFAULT_SPACE * 3;
    rcNew.right = rcNew.left + (rcWidget[i].right - rcWidget[i].left);
    rcNew.bottom = rcNew.top + (rcWidget[i].bottom - rcWidget[i].top);
    rcWidget[i] = rcNew;
    DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);

    //adjust play text position
    i++;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_TEXT_PLAY);
    GetWindowRect(hWidgetWnd[i], &rcWidget[i]);
    rcNew.left = rcWidget[0].left;
    rcNew.top = rcWidget[i - 1].bottom + DEFAULT_SPACE * 2;
    rcNew.right = rcNew.left + (rcWidget[i].right - rcWidget[i].left);
    rcNew.bottom = rcNew.top + (rcWidget[i].bottom - rcWidget[i].top);
    rcWidget[i] = rcNew;
    DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);

    //adjust music slider position
    i++;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_SLIDER_MUSIC);
    GetWindowRect(hWidgetWnd[i], &rcWidget[i]);
    rcNew.left = rcWidget[i - 1].right + DEFAULT_SPACE;
    rcNew.top = rcWidget[i - 1].top;
    rcNew.right = rcNew.left + (rcWidget[i].right - rcWidget[i].left);
    rcNew.bottom = rcNew.top + (rcWidget[i].bottom - rcWidget[i].top);
    rcWidget[i] = rcNew;
    DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);

    //adjust total text position
    i++;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_TEXT_TOTAL);
    GetWindowRect(hWidgetWnd[i], &rcWidget[i]);
    rcNew.left = rcWidget[i - 1].right + DEFAULT_SPACE;
    rcNew.top = rcWidget[i - 1].top;
    rcNew.right = rcNew.left + (rcWidget[i].right - rcWidget[i].left);
    rcNew.bottom = rcNew.top + (rcWidget[i].bottom - rcWidget[i].top);
    rcWidget[i] = rcNew;
    DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);

    EndDeferWindowPos(hdwp);
   
}

void CMusicPage::DoTimerEvent(WPARAM wParam, LPARAM lParam)
{
    //add code here
}

void CMusicPage::RegisterCallBack(void)
{
    //register callback function
    BZ_RegBTCallBack(ID_MUSIC,BZ_MODULE_MUSIC, CMusicPage::OnBTMusicConnect);

}

void CMusicPage::OnBTMusicConnect(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{

}