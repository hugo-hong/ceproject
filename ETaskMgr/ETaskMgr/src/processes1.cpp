#include "stdafx.h"
#include "page.h"
#include "processes.h"
#include "ETaskMgr.h"
#include <Tlhelp32.h>

extern TASK_PARAM g_taskParam;
//PROCESS_INFORMATION pi = {0}; 
BOOL CALLBACK ProcessDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    CProcesses *pThis = (CProcesses *)g_taskParam.m_pPage[TAB_PROCESS];

    ASSERT(pThis != NULL);

    switch (message)
    {
    case WM_INITDIALOG:
        {
           /* pThis->m_hListViewWnd = GetDlgItem(hDlg, IDC_LIST_PROCESS);
            pThis->InitListViewColumns();
            pThis->DrawProcessView();

            ListView_SetExtendedListViewStyle(pThis->m_hListViewWnd, LVS_EX_FULLROWSELECT);         
                    
            return TRUE;*/            

            return pThis->DoInitDialog(hDlg, wParam, lParam);
        }
        break;

    case WM_NOTIFY:
        break;

    case WM_TIMER:
        pThis->DoTimerEvent(wParam, lParam);        
        break;

    case WM_SIZE:
        pThis->DoResizeWindow(wParam, lParam);      
        break;

    case WM_COMMAND:
        {
            int wmId    = LOWORD(wParam); 
            int wmEvent = HIWORD(wParam); 

            switch (wmId)
            {
            case IDC_BUTTON_END:
                pThis->KillSelectedProcess();
                break;
            }
        }
        break;
    }

   


    return FALSE;
}

CProcesses::CProcesses()
{
    m_hDlgWnd = NULL;
    m_hListViewWnd = NULL;
}

CProcesses::~CProcesses()
{
    
}

BOOL CProcesses::DoInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
    HWND hListViewWnd = NULL;

    m_hListViewWnd = hListViewWnd = GetDlgItem(hDlg, IDC_LIST_PROCESS);

    InitListViewColumns();

    DrawProcessView();

    ListView_SetExtendedListViewStyle(hListViewWnd, LVS_EX_FULLROWSELECT);

    return TRUE;    
}

void CProcesses::DoTimerEvent(WPARAM wParam, LPARAM lParam)
{
    DrawProcessView();
}

void CProcesses::DoResizeWindow(WPARAM wParam, LPARAM lParam)
{
    HDWP hdwp = NULL;
    RECT rcTab = {0};
    RECT rcLView = {0};
    RECT rcBtn = {0};
    HWND hBtnWnd = NULL;
    int nWidth = LOWORD(lParam);
    int nHeight = HIWORD(lParam);    
   
    SetRect(&rcTab, 0, 0, nWidth, nHeight);

    hBtnWnd = GetDlgItem(m_hDlgWnd, IDC_BUTTON_END);    
    GetWindowRect(hBtnWnd, &rcBtn);

    hdwp = BeginDeferWindowPos(3);

    //adjust list view position
    rcLView.left = DEFAULT_SPACE;
    rcLView.top = DEFAULT_SPACE;
    rcLView.right = rcTab.right - rcTab.left - DEFAULT_SPACE;
    rcLView.bottom = rcTab.bottom - rcTab.top - (rcBtn.bottom - rcBtn.top) - DEFAULT_SPACE;
    DeferWindowPos(hdwp,
        m_hListViewWnd, 
        HWND_TOP,
        rcLView.left,
        rcLView.top,
        rcLView.right - rcLView.left,
        rcLView.bottom - rcLView.top,
        0);
    

    //adjust buttern position
    MapWindowPoints(HWND_DESKTOP, m_hDlgWnd, (LPPOINT) &rcBtn, 2);
    int dx = rcTab.right - rcBtn.right - DEFAULT_SPACE;
    int dy = rcTab.bottom - rcBtn.bottom - DEFAULT_SPACE;
    DeferWindowPos(hdwp,
        hBtnWnd, 
        HWND_TOP,
        rcBtn.left + dx,
        rcBtn.top + dy,
        0,
        0,
        SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

    EndDeferWindowPos(hdwp);
}

BOOL CProcesses::Init()
{
    TCITEM tie = {0};

    tie.mask = TCIF_TEXT | TCIF_IMAGE;
    tie.iImage = -1;
    tie.pszText = _T("Processes");
    if(-1 == TabCtrl_InsertItem(g_taskParam.m_hTabWnd, TAB_PROCESS, &tie))
    {
        return FALSE;
    }
    else
    {
        m_hDlgWnd = CreateDialog(g_taskParam.m_hInstance, 
            MAKEINTRESOURCE(IDD_PROCESS_DLG),
            g_taskParam.m_hTabWnd,
            ProcessDlgProc);

        ShowWindow(m_hDlgWnd, SW_SHOWNORMAL);
    }
}

BOOL CProcesses::InitListViewColumns()
{    
    LVCOLUMN lvColumn = {0};
    int columnIndex = 0;    

    lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

    // Process Image
    lvColumn.iSubItem = columnIndex;
    lvColumn.pszText = _T("Image Name");
    lvColumn.fmt = LVCFMT_LEFT;
    if (ListView_InsertColumn(m_hListViewWnd, columnIndex++, &lvColumn) == -1) 
    {
        return FALSE;
    }

    // Process ID
    lvColumn.iSubItem = columnIndex;
    lvColumn.pszText = _T("PID");
    lvColumn.fmt = LVCFMT_CENTER;
    if (ListView_InsertColumn(m_hListViewWnd, columnIndex++, &lvColumn) == -1)
    {
        return FALSE;
    }

    // Process Threads
    lvColumn.iSubItem = columnIndex;
    lvColumn.pszText = _T("Threads");
    lvColumn.fmt = LVCFMT_RIGHT;
    if (ListView_InsertColumn(m_hListViewWnd, columnIndex++, &lvColumn) == -1)
    {
        return FALSE;
    }

    //compute the size of column
    HDC hDC = GetDC(m_hListViewWnd);
    SIZE sz = {0};
    GetTextExtentPoint(hDC, _T("0000000000"), 10, &sz);
    ReleaseDC(m_hListViewWnd, hDC);

    ListView_SetColumnWidth(m_hListViewWnd, 0, LVSCW_AUTOSIZE_USEHEADER);
    ListView_SetColumnWidth(m_hListViewWnd, 1, sz.cx);
    ListView_SetColumnWidth(m_hListViewWnd, 2, sz.cx);         

    return TRUE;
}

BOOL CProcesses::DrawProcessView()
{
    BOOL fgRet = 0;
    HANDLE hSnapShot = INVALID_HANDLE_VALUE;

    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapShot)
    {
        return FALSE;
    }

    //enum processes
    int iIndex = 0;
    
    PROCESSENTRY32 pe32 = {0};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    m_PIDList.clear();

    fgRet = Process32First(hSnapShot, &pe32);
    ASSERT(fgRet != FALSE);    
    do 
    {       
        InsertItem(&pe32);
        m_PIDList.push_back(pe32.th32ProcessID);
    } while (Process32Next(hSnapShot, &pe32));

    UpdateItems();

    CloseToolhelp32Snapshot(hSnapShot);

    if (0 == GetSelectedItemLParam())
    {
        ListView_SetItemState(m_hListViewWnd, 0, LVIS_SELECTED, LVIS_SELECTED);
    }

    return TRUE;
}

BOOL CProcesses::InsertItem(LPVOID lpParam)
{
    DWORD dwIndex = 0;
    PROCESSENTRY32 *pPE32 = (PROCESSENTRY32 *)lpParam;

    if (NULL == pPE32)
    {
        return FALSE;
    }

    // serch item
    LVFINDINFO finditem;
    memset(&finditem, 0, sizeof(LVFINDINFO));

    finditem.flags = LVFI_PARAM;
    finditem.lParam = pPE32->th32ProcessID;

    dwIndex = ListView_FindItem(m_hListViewWnd, -1, &finditem);
    if( dwIndex != -1 )
    {
        return FALSE;
    }

    LVITEM lvItem;
    memset(&lvItem, 0, sizeof(LVITEM));

    //image name
    lvItem.mask = LVIF_TEXT | LVIF_PARAM;
    lvItem.iItem = 0;
    lvItem.iSubItem = 0;
    lvItem.pszText = pPE32->szExeFile;
    lvItem.lParam = pPE32->th32ProcessID;

    dwIndex = ListView_InsertItem(m_hListViewWnd, &lvItem);

    if( dwIndex == 1 )
    {
        dwIndex = 0;
        return FALSE;
    }

    // Add subitems
    TCHAR szFmt[256] = {0};

    // id
    _stprintf( szFmt, _T("%08X"), pPE32->th32ProcessID );
    ListView_SetItemText( m_hListViewWnd, dwIndex, 1, szFmt );

    // threads
    _stprintf( szFmt, _T("%d"), pPE32->cntThreads );
    ListView_SetItemText( m_hListViewWnd, dwIndex, 2, szFmt);
    
    return TRUE;
}

BOOL CProcesses::UpdateItems(void)
{
    int nIndex = -1;
    vector<DWORD>::size_type i = 0;    

    nIndex = ListView_GetNextItem(m_hListViewWnd, nIndex, 0);

    LVITEM lvItem;
    memset(&lvItem, 0, sizeof(LVITEM));

    lvItem.mask = LVIF_PARAM;

    while( nIndex != -1 )
    {
        lvItem.iItem = nIndex;
        ListView_GetItem(m_hListViewWnd, &lvItem);
        
        BOOL fDelete = TRUE;
        for (i = 0; i < m_PIDList.size(); i++)
        {
            if (m_PIDList[i] == (DWORD)lvItem.lParam)
            {
                fDelete = FALSE;
                break;
            }
        }       

        if( fDelete )
        {
            ListView_DeleteItem(m_hListViewWnd, nIndex);
            nIndex = -1;
        }

        nIndex = ListView_GetNextItem(m_hListViewWnd, nIndex, 0);
    }

    return TRUE;
}

LPARAM CProcesses::GetSelectedItemLParam(void)
{
    int nIndex = -1;
    LVITEM lvItem = {0};
    
    lvItem.mask = LVIF_PARAM;
    nIndex = ListView_GetNextItem(m_hListViewWnd, nIndex, 0);

    while( nIndex != -1 )
    {
        lvItem.iItem = nIndex;
        if (ListView_GetItemState(m_hListViewWnd, nIndex, LVIS_SELECTED))
        {
            ListView_GetItem(m_hListViewWnd, &lvItem);
            return lvItem.lParam;
        }
        nIndex = ListView_GetNextItem(m_hListViewWnd, nIndex, 0);
    }

    return 0;
}

BOOL CProcesses::KillSelectedProcess(void)
{
    DWORD dwProcessID = 0;

    dwProcessID = GetSelectedItemLParam();

    if (0 == dwProcessID)
    {
        return FALSE;
    }
        

    if( IDOK == MessageBox(m_hListViewWnd,
        _T("WARNING:After terminate a process,data may be lost or system becomes unstable!\
           Are you sure to terminate the process?"),
        _T("[WARN]"),
        MB_OKCANCEL | MB_DEFBUTTON2 | MB_APPLMODAL | MB_ICONEXCLAMATION ))
    {
        HANDLE hProcess;
        hProcess = OpenProcess(0, FALSE, dwProcessID);
        TerminateProcess(hProcess, 0);
    }

    return TRUE;
}

DWORD CProcesses::GetProcessHeapSzie(DWORD dwProcessId)
{
    DWORD dwRet = MAXDWORD;

    if ( 0 == dwProcessId ||
        MAXDWORD == dwProcessId)
    {
        return MAXDWORD;
    }
        

    DWORD dwUsedHeap = 0;    
    HANDLE hSnapShot =INVALID_HANDLE_VALUE;

    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, dwProcessId);
    if (INVALID_HANDLE_VALUE == hSnapShot)
    {
        return MAXDWORD;
    }	

    HEAPLIST32 hl32 = {0};    
    hl32.dwSize = sizeof(HEAPLIST32);
    if( !Heap32ListFirst(hSnapShot, &hl32) )
    {
        goto EXIT;       
    }

    DWORD dwTotalHeapByte = 0;
    do
    {
        HEAPENTRY32 he32 = {0};        
        he32.dwSize = sizeof(HEAPENTRY32);

        if (!Heap32First(hSnapShot, &he32, hl32.th32ProcessID, hl32.th32HeapID))
        {
            goto EXIT;
        }
        else
        {
            dwTotalHeapByte = 0;
            do
            {
                if( he32.dwFlags != LF32_FREE )
                {
                    dwTotalHeapByte += he32.dwBlockSize;
                }
                    
            }while (Heap32Next(hSnapShot, &he32));
        }
        
        if( GetLastError() != ERROR_NO_MORE_FILES )
        {
            goto EXIT;
        }

        dwUsedHeap += dwTotalHeapByte;

    }while (Heap32ListNext(hSnapShot, &hl32));

    if( GetLastError() != ERROR_NO_MORE_FILES )
    {
        dwUsedHeap = 0;
    } 
    else
    {
        dwRet = dwUsedHeap;
    }
    

EXIT:
    if (hSnapShot != INVALID_HANDLE_VALUE)
    {
        CloseToolhelp32Snapshot(hSnapShot);        
    }

    return dwRet;
}