#include "stdafx.h"
#include "DialPage.h"
#include "ecommon.h"
#include "EBTMini.h"

extern INSTANCE_ENTRYDATA g_tEntryData;

BOOL CALLBACK DialDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    CDialPage *pThis = (CDialPage *)GetWindowLong(hDlg, GWL_USERDATA);
    
    switch (message)
    {
    case WM_INITDIALOG:
        ASSERT(lParam != NULL);
        SetWindowLong(hDlg, GWL_USERDATA, lParam);   
        pThis = (CDialPage *)lParam;
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

CDialPage::CDialPage()
{
    m_hWnd = NULL;    
}

CDialPage::~CDialPage()
{

}

BOOL CDialPage::Init(HWND hwndParent)
{
    TCITEM tie = {0};

    ASSERT(hwndParent != NULL);

    tie.mask = TCIF_TEXT | TCIF_IMAGE;
    tie.iImage = -1;
    tie.pszText = _T("Dial");
    if(-1 == TabCtrl_InsertItem(hwndParent, TAB_DIAL, &tie))
    {
        return FALSE;
    }
    else
    {
        m_hWnd = CreateDialogParam(g_tEntryData.m_hInstance, 
            MAKEINTRESOURCE(IDD_DLG_DIAL),
            hwndParent,
            DialDlgProc,
            (LPARAM)this);

        //ShowWindow(m_hWnd, SW_SHOWNORMAL);
    }    

    return TRUE;
}

void CDialPage:: DeInit()
{
    //add code here
}


void CDialPage::DoInitDialog(WPARAM wParam, LPARAM lParam)
{
    //add code here   
}

void CDialPage::DoResizeWindow(WPARAM wParam, LPARAM lParam)
{
    //add code here
    HDWP hdwp = NULL;
    RECT rcNew = {0};
    RECT rcWidget[16] = {0};   
    HWND hWidgetWnd[16] = {NULL};    
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

    hdwp = BeginDeferWindowPos(16);

    hWidgetWnd[0] = GetDlgItem(m_hWnd, IDC_EDIT_PHONE);
    GetWindowRect(hWidgetWnd[0], &rcWidget[0]);
    iEditWidth = rcWidget[0].right - rcWidget[0].left;
    iEditHeight = rcWidget[0].bottom - rcWidget[0].top;
    
    hWidgetWnd[1] = GetDlgItem(m_hWnd, IDC_BUTTON_CALL);
    GetWindowRect(hWidgetWnd[1], &rcWidget[1]);
    iButtonWidth = rcWidget[1].right - rcWidget[1].left;
    iButtonHeight = rcWidget[1].bottom - rcWidget[1].top;

   
    //calcultae the widget position      
    dx = (iEditWidth - iButtonWidth * 3)/4;//x space
    dy = DEFAULT_SPACE * 2;//y space
    rcNew.left = (iWidth - iEditWidth)/2;// DEFAULT_SPACE;
    rcNew.top = (iHeight-iEditHeight-iButtonHeight*5 - dy * 5)/2;
    if (rcNew.top < 0)
    {
        rcNew.top = 0;
        dy = DEFAULT_SPACE;
        while ((iHeight-iEditHeight-iButtonHeight*5 - dy * 5) < 0)
        {
            dy--;       
            if (dy == 0)
            {
                break;
            }
        }
    }

    //adjust phone number edit position
    i = 0; 
    rcNew.right = rcNew.left + iEditWidth;
    rcNew.bottom = rcNew.top + iEditHeight;    
    DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);  
         
    
    cx = rcNew.left + dx; //x coordinate

    //adjust function button position 
    i++; 
    rcNew.left = cx; 
    rcNew.top = rcNew.bottom + dy;
    rcNew.right = rcNew.left + iButtonWidth;
    rcNew.bottom = rcNew.top + iButtonHeight;   
    DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);
    

    //adjust audio button position
    i++;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_BUTTON_AUDIO);
    rcNew.left = rcNew.right + dx;
    rcNew.top = rcNew.top;
    rcNew.right = rcNew.left + iButtonWidth;
    rcNew.bottom = rcNew.top + iButtonHeight;   
    DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);

    //adjust end button position
    i++;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_BUTTON_END);
    rcNew.left = rcNew.right + dx;
    rcNew.top = rcNew.top;
    rcNew.right = rcNew.left + iButtonWidth;
    rcNew.bottom = rcNew.top + iButtonHeight;   
    DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);

    //adjust number button position    
    for (int iId = IDC_BUTTON_1,i = 4; i < 16; i++, iId++)
    {
        hWidgetWnd[i] = GetDlgItem(m_hWnd, iId);
        if ((i % 3) == 1)
        {
            rcNew.left = cx;
            rcNew.top = rcNew.bottom + dy;
        }
        else 
        {
            rcNew.left = rcNew.right + dx;
            rcNew.top = rcNew.top;
        }        
        rcNew.right = rcNew.left + iButtonWidth;
        rcNew.bottom = rcNew.top + iButtonHeight; 

        DeferWindowPos(hdwp,
            hWidgetWnd[i], 
            HWND_TOP,
            rcNew.left,
            rcNew.top,
            rcNew.right - rcNew.left,
            rcNew.bottom - rcNew.top,
            0);
    }  
 
    EndDeferWindowPos(hdwp);
}

void CDialPage::DoTimerEvent(WPARAM wParam, LPARAM lParam)
{
    //add code here
}

void CDialPage::RegisterCallBack(void)
{
    //register callback function
    BZ_RegBTCallBack(ID_DIAL,BZ_MODULE_CALL, CDialPage::OnBTCall);
}

void CDialPage::OnBTCall(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
    switch(dwIndID)
    {
    case BZ_IND_CALL_OUTGOING:
        {
            BZ_VerifySCOAudioChannel(TRUE);

            //notify to switch to dial view,and switch audio channel
        }
        break;

    case BZ_IND_CALL_INCOMING:
        {
            //notify to switch to dial view,and switch audio channel
        }
        break;

    case BZ_IND_CALL_SPEAKING:
        {
            BZ_VerifySCOAudioChannel(TRUE);

        }
        break;

    case BZ_IND_CALL_TERMINATED:
        {
            BZ_VerifySCOAudioChannel(FALSE);

            //notify to switch from dial view,and restore audio channel

        }
        break;

    }
}