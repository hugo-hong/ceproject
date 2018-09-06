#include "stdafx.h"
#include "PhonebookPage.h"
#include "ecommon.h"
#include "EBTMini.h"

extern INSTANCE_ENTRYDATA g_tEntryData;

BOOL CALLBACK PhoneBookDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    CPoneBookPage *pThis = (CPoneBookPage *)GetWindowLong(hDlg, GWL_USERDATA);

    switch (message)
    {
    case WM_INITDIALOG:
        ASSERT(lParam != NULL);
        SetWindowLong(hDlg, GWL_USERDATA, lParam);
        pThis = (CPoneBookPage *)lParam;
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

CPoneBookPage::CPoneBookPage()
{
    m_hWnd = NULL;    
}

CPoneBookPage::~CPoneBookPage()
{

}

BOOL CPoneBookPage::Init(HWND hwndParent)
{
    TCITEM tie = {0};
    
    ASSERT(hwndParent != NULL);

    tie.mask = TCIF_TEXT | TCIF_IMAGE;
    tie.iImage = -1;
    tie.pszText = _T("PhoneBook");
    if(-1 == TabCtrl_InsertItem(hwndParent, TAB_PHONEBOOK, &tie))
    {
        return FALSE;
    }
    else
    {
        m_hWnd = CreateDialogParam(g_tEntryData.m_hInstance, 
            MAKEINTRESOURCE(IDD_DLG_PHONEBOOK),
            hwndParent,
            PhoneBookDlgProc,
            (LPARAM)this);

        //ShowWindow(m_hWnd, SW_SHOWNORMAL);
    }

    return TRUE;
}

void CPoneBookPage:: DeInit()
{
    //add code here
}


void CPoneBookPage::DoInitDialog(WPARAM wParam, LPARAM lParam)
{
    HWND hWidgetWnd = NULL;

    ASSERT(m_hWnd != NULL);

    //Init list view
    hWidgetWnd = GetDlgItem(m_hWnd, IDC_LIST_PHONEBOOK);
    ASSERT(hWidgetWnd != NULL);

    InitListViewColumns();

    //ShowWindow(hWidgetWnd, SW_HIDE);
   
}

void CPoneBookPage::DoResizeWindow(WPARAM wParam, LPARAM lParam)
{
    HDWP hdwp = NULL;
    RECT rcNew = {0};
    RECT rcWidget[5] = {0};   
    HWND hWidgetWnd[5] = {NULL};    
    int iWidth = LOWORD(lParam);
    int iHeight = HIWORD(lParam);    
    int iButtonWidth = 0;
    int iButtonHeight = 0;
    int iListWidth = 0;
    int iListHeight = 0;
    int i = 0;

    ASSERT(m_hWnd != NULL);  

    hdwp = BeginDeferWindowPos(4);

    //adjust state position
    i = 0;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_PHONEBOOK_STATE);
    GetWindowRect(hWidgetWnd[i], &rcWidget[i]);
    rcNew.left = DEFAULT_SPACE * 5;
    rcNew.top = DEFAULT_SPACE;
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

    //adjust button position
    i++;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_SYNC_PHONEBOOK);
    GetWindowRect(hWidgetWnd[i], &rcWidget[i]);
    iButtonWidth = rcWidget[i].right - rcWidget[i].left;
    iButtonHeight = rcWidget[i].bottom - rcWidget[i].top;

    rcNew.left = rcWidget[i - 1].left;
    rcNew.top = iHeight - DEFAULT_SPACE * 2 - iButtonHeight;
    rcNew.right = rcNew.left + iButtonWidth;
    rcNew.bottom = iHeight - DEFAULT_SPACE * 2;
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
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_PHONEBOOK_CALL);
    
    rcNew.left = rcWidget[i - 1].right + DEFAULT_SPACE * 5;
    rcNew.top = rcWidget[i - 1].top;
    rcNew.right = rcNew.left + iButtonWidth;
    rcNew.bottom = rcWidget[i - 1].bottom;
    rcWidget[i] = rcNew;
    DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);

    //adjust phone book list position
    i++;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_LIST_PHONEBOOK);

    //Get list view width    
    iListWidth = ListView_GetColumnWidth(hWidgetWnd[i], 0);
    iListWidth += ListView_GetColumnWidth(hWidgetWnd[i], 1);
     
    iListHeight = (rcWidget[1].top - DEFAULT_SPACE) - (rcWidget[0].bottom + DEFAULT_SPACE);
    
    rcNew.left = rcWidget[0].left;
    rcNew.top = rcWidget[0].bottom + DEFAULT_SPACE;
    rcNew.right = rcNew.left + iListWidth;
    rcNew.bottom = rcNew.top + iListHeight;
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

void CPoneBookPage::DoTimerEvent(WPARAM wParam, LPARAM lParam)
{
    //add code here
}

BOOL CPoneBookPage::InitListViewColumns()
{
    LVCOLUMN lvColumn = {0};
    int columnIndex = 0;   
    HWND hListViewWnd = GetDlgItem(m_hWnd, IDC_LIST_PHONEBOOK);

    lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

    // BT name 
    lvColumn.iSubItem = columnIndex;
    lvColumn.pszText = _T("Name");
    lvColumn.fmt = LVCFMT_LEFT;
    if (ListView_InsertColumn(hListViewWnd, columnIndex++, &lvColumn) == -1) 
    {
        return FALSE;
    }

    // BT ID
    lvColumn.iSubItem = columnIndex;
    lvColumn.pszText = _T("Phone number");
    lvColumn.fmt = LVCFMT_CENTER;
    if (ListView_InsertColumn(hListViewWnd, columnIndex++, &lvColumn) == -1)
    {
        return FALSE;
    }  

    //compute the size of column
    HDC hDC = GetDC(hListViewWnd);
    SIZE sz = {0};
    GetTextExtentPoint(hDC, _T("+86-13800138000"), 15, &sz);
    ReleaseDC(hListViewWnd, hDC);

    ListView_SetColumnWidth(hListViewWnd, 0, LVSCW_AUTOSIZE_USEHEADER);
    ListView_SetColumnWidth(hListViewWnd, 1, sz.cx);     

    return TRUE;
}

void CPoneBookPage::RegisterCallBack(void)
{
    BZ_RegBTCallBack(ID_PHONEBOOK,BZ_MODULE_PBMGR,OnBTPBMgr);
    BZ_RegBTCallBack(ID_PHONEBOOK,BZ_MODULE_OCBT,OnBTPBSwitch);
    BZ_RegBTCallBack(ID_PHONEBOOK,BZ_MODULE_CONNECTION,OnBTPBConnect);
}

void CPoneBookPage::OnBTPBMgr(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
    
}
void CPoneBookPage::OnBTPBSwitch(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
    
}
void CPoneBookPage::OnBTPBConnect(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
    
}