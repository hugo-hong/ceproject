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
        pThis->m_hDlgWnd = hDlg;
        return pThis->DoInitDialog(wParam, lParam);
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
}

CProcesses::~CProcesses()
{
    
}

BOOL CProcesses::DoInitDialog(WPARAM wParam, LPARAM lParam)
{
    HWND hListViewWnd = NULL;

    hListViewWnd = GetDlgItem(m_hDlgWnd, IDC_LIST_PROCESS);

    ASSERT(hListViewWnd != NULL);

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
    HWND hListViewWnd = NULL;
    int nWidth = LOWORD(lParam);
    int nHeight = HIWORD(lParam);    
   
    SetRect(&rcTab, 0, 0, nWidth, nHeight);

    hBtnWnd = GetDlgItem(m_hDlgWnd, IDC_BUTTON_END);    
    GetWindowRect(hBtnWnd, &rcBtn);

    hListViewWnd = GetDlgItem(m_hDlgWnd, IDC_LIST_PROCESS);
    GetWindowRect(hListViewWnd, &rcLView);

    hdwp = BeginDeferWindowPos(3);

    //adjust list view position
    rcLView.left = DEFAULT_SPACE;
    rcLView.top = DEFAULT_SPACE;
    rcLView.right = rcTab.right - rcTab.left - DEFAULT_SPACE;
    rcLView.bottom = rcTab.bottom - rcTab.top - (rcBtn.bottom - rcBtn.top) - DEFAULT_SPACE;
    DeferWindowPos(hdwp,
        hListViewWnd, 
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

    return TRUE;
}

BOOL CProcesses::InitListViewColumns()
{        
    LVCOLUMN lvColumn = {0};
    int columnIndex = 0;   
    HWND hListViewWnd = GetDlgItem(m_hDlgWnd, IDC_LIST_PROCESS);

    lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

    // Process Image
    lvColumn.iSubItem = columnIndex;
    lvColumn.pszText = _T("Image Name");
    lvColumn.fmt = LVCFMT_LEFT;
    if (ListView_InsertColumn(hListViewWnd, columnIndex++, &lvColumn) == -1) 
    {
        return FALSE;
    }

    // Process ID
    lvColumn.iSubItem = columnIndex;
    lvColumn.pszText = _T("PID");
    lvColumn.fmt = LVCFMT_CENTER;
    if (ListView_InsertColumn(hListViewWnd, columnIndex++, &lvColumn) == -1)
    {
        return FALSE;
    }

    // Process Threads number
    lvColumn.iSubItem = columnIndex;
    lvColumn.pszText = _T("Threads");
    lvColumn.fmt = LVCFMT_RIGHT;
    if (ListView_InsertColumn(hListViewWnd, columnIndex++, &lvColumn) == -1)
    {
        return FALSE;
    }

    // Process heap used
    lvColumn.iSubItem = columnIndex;
    lvColumn.pszText = _T("Heap");
    lvColumn.fmt = LVCFMT_RIGHT;
    if (ListView_InsertColumn(hListViewWnd, columnIndex++, &lvColumn) == -1)
    {
        return FALSE;
    }

    //compute the size of column
    HDC hDC = GetDC(hListViewWnd);
    SIZE sz = {0};
    GetTextExtentPoint(hDC, _T("0000000000"), 10, &sz);
    ReleaseDC(hListViewWnd, hDC);

    ListView_SetColumnWidth(hListViewWnd, 0, LVSCW_AUTOSIZE_USEHEADER);
    ListView_SetColumnWidth(hListViewWnd, 1, sz.cx);
    ListView_SetColumnWidth(hListViewWnd, 2, sz.cx);  
    ListView_SetColumnWidth(hListViewWnd, 3, sz.cx);

    return TRUE;
}

BOOL CProcesses::DrawProcessView()
{
    BOOL fgRet = 0;
    HANDLE hSnapShot = INVALID_HANDLE_VALUE;
    HWND hListViewWnd = ::GetDlgItem(m_hDlgWnd, IDC_LIST_PROCESS);

    hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapShot)
    {
        return FALSE;
    }

    //enum processes
    int iIndex = 0;
    
    PROCESSENTRY32 pe32 = {0};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    m_PIDList.clear();

    fgRet = ::Process32First(hSnapShot, &pe32);
    ASSERT(fgRet != FALSE);    
    do 
    {       
        InsertItem(&pe32);
        m_PIDList.push_back(pe32.th32ProcessID);
    } while (::Process32Next(hSnapShot, &pe32));

    UpdateItems();

    ::CloseToolhelp32Snapshot(hSnapShot);

    if (0 == GetSelectedItemLParam())
    {
        ListView_SetItemState(hListViewWnd, 0, LVIS_SELECTED, LVIS_SELECTED);
    }

    //update status bar
    TCHAR szFormat[16] = {0};
    _stprintf(szFormat, _T("Processes:%d"), m_PIDList.size());
    ::SendMessage(g_taskParam.m_hStatusWnd, SB_SETTEXT, 0, (LPARAM)szFormat);

    //test waveform    
    WAVEFORMATEX wfx;
    wfx.cbSize = sizeof(WAVEFORMATEX);
    waveOutOpen(NULL, 0, &wfx, 0, NULL, WAVE_FORMAT_QUERY);

    return TRUE;
}

BOOL CProcesses::InsertItem(LPVOID lpParam)
{
    DWORD dwIndex = 0;
    TCHAR szFmt[16] = {0};
    PROCESSENTRY32 *pPE32 = (PROCESSENTRY32 *)lpParam;
    HWND hListViewWnd = ::GetDlgItem(m_hDlgWnd, IDC_LIST_PROCESS);

    if (NULL == pPE32)
    {
        return FALSE;
    }

    // serch item
    LVFINDINFO finditem;
    memset(&finditem, 0, sizeof(LVFINDINFO));

    finditem.flags = LVFI_PARAM;
    finditem.lParam = pPE32->th32ProcessID;

    dwIndex = ListView_FindItem(hListViewWnd, -1, &finditem);
    if( dwIndex != -1 )
    {
        //this item alrady in listview,just update item

        // threads
        _stprintf( szFmt, _T("%d"), pPE32->cntThreads );
        ListView_SetItemText( hListViewWnd, dwIndex, 2, szFmt);

        //heap
        if (Size2String(GetProcessHeapSize(pPE32->th32ProcessID), szFmt, DIM(szFmt)))
        {
            ListView_SetItemText( hListViewWnd, dwIndex, 3, szFmt);
        }  

        GetProcessMemoryStatus((LPVOID)pPE32);
    }
    else
    {
        //insert new item
        LVITEM lvItem;
        memset(&lvItem, 0, sizeof(LVITEM));

        //image name
        lvItem.mask = LVIF_TEXT | LVIF_PARAM;
        lvItem.iItem = 0;
        lvItem.iSubItem = 0;
        lvItem.pszText = pPE32->szExeFile;
        lvItem.lParam = pPE32->th32ProcessID;

        dwIndex = ListView_InsertItem(hListViewWnd, &lvItem);

        if( dwIndex == 1 )
        {
            dwIndex = 0;
            return FALSE;
        }

        // Add subitems
        // id
        _stprintf( szFmt, _T("%08X"), pPE32->th32ProcessID );
        ListView_SetItemText( hListViewWnd, dwIndex, 1, szFmt );

        // threads
        _stprintf( szFmt, _T("%d"), pPE32->cntThreads );
        ListView_SetItemText( hListViewWnd, dwIndex, 2, szFmt);

        //heap
        if (Size2String(GetProcessHeapSize(pPE32->th32ProcessID), szFmt, DIM(szFmt)))
        {
            ListView_SetItemText( hListViewWnd, dwIndex, 3, szFmt);
        }

    }   
    
    return TRUE;
}

BOOL CProcesses::UpdateItems(void)
{
    int nIndex = -1;
    vector<DWORD>::size_type i = 0; 
    HWND hListViewWnd = GetDlgItem(m_hDlgWnd, IDC_LIST_PROCESS);

    nIndex = ListView_GetNextItem(hListViewWnd, nIndex, 0);

    LVITEM lvItem;
    memset(&lvItem, 0, sizeof(LVITEM));

    lvItem.mask = LVIF_PARAM;

    while( nIndex != -1 )
    {
        lvItem.iItem = nIndex;
        ListView_GetItem(hListViewWnd, &lvItem);
        
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
            ListView_DeleteItem(hListViewWnd, nIndex);
            nIndex = -1;
        }

        nIndex = ListView_GetNextItem(hListViewWnd, nIndex, 0);
    }

    return TRUE;
}

LPARAM CProcesses::GetSelectedItemLParam(void)
{
    int nIndex = -1;
    LVITEM lvItem = {0};
    HWND hListViewWnd = GetDlgItem(m_hDlgWnd, IDC_LIST_PROCESS);
    
    lvItem.mask = LVIF_PARAM;
    nIndex = ListView_GetNextItem(hListViewWnd, nIndex, 0);

    while( nIndex != -1 )
    {
        lvItem.iItem = nIndex;
        if (ListView_GetItemState(hListViewWnd, nIndex, LVIS_SELECTED))
        {
            ListView_GetItem(hListViewWnd, &lvItem);
            return lvItem.lParam;
        }
        nIndex = ListView_GetNextItem(hListViewWnd, nIndex, 0);
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
        

    if( IDOK == MessageBox(m_hDlgWnd,
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

DWORD CProcesses::GetProcessHeapSize(DWORD dwProcessId)
{
    DWORD dwRet = MAXDWORD;

    if ( 0 == dwProcessId ||
        MAXDWORD == dwProcessId)
    {
        return MAXDWORD;
    }      
      
    HANDLE hSnapShot = INVALID_HANDLE_VALUE;    
    DWORD dwTotalHeapSize = 0;

    hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, dwProcessId);
    if (INVALID_HANDLE_VALUE == hSnapShot)
    {
        return MAXDWORD;
    }	

    HEAPLIST32 heapList32 = {0};    
    heapList32.dwSize = sizeof(HEAPLIST32);
    if( !::Heap32ListFirst(hSnapShot, &heapList32) )
    {
        goto EXIT;       
    }
    
    do
    {
        HEAPENTRY32 heapEntry32 = {0};        
        heapEntry32.dwSize = sizeof(HEAPENTRY32);       

        if (!::Heap32First(hSnapShot, &heapEntry32, heapList32.th32ProcessID, heapList32.th32HeapID))
        {
            goto EXIT;
        }
        else
        {            
            do
            {
                //if( heapEntry32.dwFlags != LF32_FREE )
                {
                    dwTotalHeapSize += heapEntry32.dwBlockSize;
                }
                    
            }while (::Heap32Next(hSnapShot, &heapEntry32));
        }
        
        if( GetLastError() != ERROR_NO_MORE_FILES )
        {
            goto EXIT;
        }       

    }while (::Heap32ListNext(hSnapShot, &heapList32));

    if(::GetLastError() != ERROR_NO_MORE_FILES )
    {
        dwTotalHeapSize = 0;
    } 
    else
    {
        dwRet = dwTotalHeapSize;
    }    

EXIT:
    if (hSnapShot != INVALID_HANDLE_VALUE)
    {
        ::CloseToolhelp32Snapshot(hSnapShot);        
    }    

    return dwRet;
}

BOOL CProcesses::GetProcessMemoryStatus(LPVOID lpParam)
{
    PROCESSENTRY32 *pPE32 = (PROCESSENTRY32 *)lpParam;
    DWORD dwStartAddress = pPE32->th32MemoryBase;
    DWORD dwEndAddress = dwStartAddress + 0x40000000;
    DWORD dwCurAddress = dwStartAddress;
    DWORD dwRet = 0;

#if (_WIN32_WCE < 0x600) && defined(UNDER_CE)
#else
    //HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pPE32->th32ProcessID);
#endif
    memset(&m_memStatus, 0, sizeof(MEM_STATUS_T));
    while (dwCurAddress < dwEndAddress)
    {
        MEMORY_BASIC_INFORMATION memoryInfo;
#if (_WIN32_WCE < 0x600) && defined(UNDER_CE)
        dwRet = ::VirtualQuery((LPVOID)dwCurAddress, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));
#else
        //dwRet = ::VirtualQueryEx(hProcess, (LPVOID)dwCurAddress, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));
        dwRet = ::VirtualQuery((LPVOID)dwCurAddress, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));
#endif
        
        if (dwRet != 0)
        {
            if (memoryInfo.State == MEM_COMMIT)
            {
                m_memStatus.dwCommitSize += memoryInfo.RegionSize;
            }                
            else if (memoryInfo.State == MEM_RESERVE)
            {
                m_memStatus.dwReservSize += memoryInfo.RegionSize;
            }                
            else if (memoryInfo.State == MEM_FREE)
            {
                m_memStatus.dwFreeSize += memoryInfo.RegionSize;
            }                

            dwCurAddress += memoryInfo.RegionSize;
        }
        else
        {
            break;
        }
    }

    dwRet = m_memStatus.dwCommitSize;

    MEMORYSTATUS memStatus;
    memStatus.dwLength = sizeof(MEMORYSTATUS);
    ::GlobalMemoryStatus(&memStatus);

    return dwRet;
}