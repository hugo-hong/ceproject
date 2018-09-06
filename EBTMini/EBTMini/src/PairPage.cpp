#include "stdafx.h"
#include "PairPage.h"
#include "ecommon.h"
#include "EBTMini.h"
#include "eutil.h"

extern INSTANCE_ENTRYDATA g_tEntryData;

BOOL CALLBACK PairDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    CPairPage *pThis = (CPairPage *)GetWindowLong(hDlg, GWL_USERDATA);    

    switch (message)
    {
    case WM_INITDIALOG:
        ASSERT(lParam != NULL);
        SetWindowLong(hDlg, GWL_USERDATA, lParam);      
        pThis = (CPairPage *)lParam;
        pThis->m_hWnd = hDlg;
        pThis->DoInitDialog(wParam, lParam);
        return TRUE;
        break;

    case WM_NOTIFY:
        break;

    case WM_TIMER:
        ASSERT(pThis != NULL);
        pThis->DoTimerEvent(wParam, lParam);
        break;

    case WM_SIZE:
        ASSERT(pThis != NULL);
        pThis->DoResizeWindow(wParam, lParam);      
        break;

    case WM_COMMAND:
        ASSERT(pThis != NULL);
        pThis->DoCommand(wParam, lParam);
        break;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        SendMessage(g_tEntryData.m_hMainWnd, message, wParam, lParam);
        return TRUE;
        break;
    }
    
    return FALSE;
}

CPairPage::CPairPage()
         :CPage()
{     
    m_dwDiscoverTime = 0;
}
   

CPairPage::~CPairPage()
{

}

BOOL CPairPage::Init(HWND hwndParent)
{
    TCITEM tie = {0}; 

    ASSERT(hwndParent != NULL);

    tie.mask = TCIF_TEXT | TCIF_IMAGE;
    tie.iImage = -1;
    tie.pszText = _T("Pair/Connect");
    if(-1 == TabCtrl_InsertItem(hwndParent, TAB_PAIR, &tie))
    {
        return FALSE;
    }
    else
    {
        m_hWnd = CreateDialogParam(g_tEntryData.m_hInstance, 
            MAKEINTRESOURCE(IDD_DLG_PAIR),
            hwndParent,
            PairDlgProc,
            (LPARAM)this);

        //ShowWindow(m_hWnd, SW_SHOWNORMAL);
    }    

    return TRUE;
}

void CPairPage:: DeInit()
{
    //add code here
}

void  CPairPage::Activate()
{
    //add code here    

    RefreshUI(STATE_OPEN, 0);    

    CPage::Activate();
}

void  CPairPage::DeActivate()
{
    CPage::DeActivate();
}

void CPairPage::RefreshUI(UINT32 u4Para1, UINT32 u4Para2)
{
    int eBTState = STATE_UNKNOW;
    HWND hwndWidget = NULL; 
    UINT32 u4ID = u4Para1;

    eBTState = EBTManager::GetInstance()->GetBTState();

    switch (u4ID)
    {
    case STATE_OPEN:
        //first open bt
        InitLocalDevice();

        //start search bt device
        SearchRemoteDevice();
        break;

    case STATE_CLOSE:
        break;

    case STATE_SEARCH:
        hwndWidget = ::GetDlgItem(m_hWnd, IDC_BT_STATE);
        ASSERT(hwndWidget != NULL);
        if (eBTState & STATE_SEARCH)
        {
            ::SetWindowText(hwndWidget, _T("Searching...")); 
        }
        else
        {
            ::SetWindowText(hwndWidget, _T("")); 
        }
        break;

    case STATE_DISCOVER:
        break;

    }


    if ((STATE_INIT | STATE_OPEN) == eBTState)
    {
        
    }
    else if (eBTState & STATE_SEARCH)
    {              
        hwndWidget = ::GetDlgItem(m_hWnd, IDC_BT_STATE);
        ASSERT(hwndWidget != NULL);
               
    }
    else if (eBTState & STATE_PAIR)
    {
        hwndWidget = ::GetDlgItem(m_hWnd, IDC_BT_STATE);
        ASSERT(hwndWidget != NULL);
        ::SetWindowText(hwndWidget, _T("Pairing..."));

        if (m_dwDiscoverTime > 0)
        {
            TCHAR szTime[16] = {0};

            hwndWidget = GetDlgItem(m_hWnd, IDC_DISCOVER_TIME);

            Tick2String(m_dwDiscoverTime, szTime, DIM(szTime));

            ::SetWindowText(hwndWidget, szTime);
        }
        
    }

}


void CPairPage::DoInitDialog(WPARAM wParam, LPARAM lParam)
{
    HWND hWidgetWnd = NULL;

    ASSERT(m_hWnd != NULL);
    
    //Init list view
    hWidgetWnd = GetDlgItem(m_hWnd, IDC_LIST_BTADDRESS);
    ASSERT(hWidgetWnd != NULL);

    InitListViewColumns();

    //ShowWindow(hWidgetWnd, SW_HIDE);

    //Init button
    hWidgetWnd = GetDlgItem(m_hWnd, IDC_BT_CONNECT_DISCONECT);
    ASSERT(hWidgetWnd != NULL);
    //ShowWindow(hWidgetWnd, SW_HIDE);    
}

void CPairPage::DoResizeWindow(WPARAM wParam, LPARAM lParam)
{
    HDWP hdwp = NULL;
    RECT rcNew = {0};
    RECT rcWidget[4] = {0};   
    HWND hWidgetWnd[4] = {NULL};    
    int iWidth = LOWORD(lParam);
    int iHeight = HIWORD(lParam);    
    int iListWidth = 0;
    int iListHeight = 0;
    int iButtonWidth = 0;
    int iButtonHeight = 0;
    int i = 0;

    ASSERT(m_hWnd != NULL);  

    hdwp = BeginDeferWindowPos(4);   

    hWidgetWnd[3] = GetDlgItem(m_hWnd, IDC_BT_CONNECT_DISCONECT);
    GetWindowRect(hWidgetWnd[3], &rcWidget[3]);
    iButtonWidth = rcWidget[3].right - rcWidget[3].left;
    iButtonHeight = rcWidget[3].bottom - rcWidget[3].top;

    //adjust check box position
    i = 0;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_CHECK_DISCOVERED);
    GetWindowRect(hWidgetWnd[i], &rcWidget[i]);
    rcNew.left = DEFAULT_SPACE;
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

    //adjust discover time static position
    i++;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_DISCOVER_TIME);
    GetWindowRect(hWidgetWnd[i], &rcWidget[i]);
    rcNew.left = (rcWidget[i-1].right - rcWidget[i-1].left) + DEFAULT_SPACE * 5;
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

    //adjust list view position
    i++;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_LIST_BTADDRESS);

    //Get list view width    
    iListWidth = ListView_GetColumnWidth(hWidgetWnd[i], 0);
    iListWidth += ListView_GetColumnWidth(hWidgetWnd[i], 1);
    iListWidth += ListView_GetColumnWidth(hWidgetWnd[i], 2);  
    
    iListHeight = (iHeight - DEFAULT_SPACE - iButtonHeight) - (rcWidget[i - 1].bottom + DEFAULT_SPACE); 

    rcNew.left = DEFAULT_SPACE;
    rcNew.top = rcWidget[i - 1].bottom + DEFAULT_SPACE;
    rcNew.right = rcNew.left + iListWidth + DEFAULT_SPACE;
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

    //adjust connect/disconnect button position
    i++;
    hWidgetWnd[i] = GetDlgItem(m_hWnd, IDC_BT_CONNECT_DISCONECT);
    
    rcNew.right = rcWidget[i - 1].right;
    rcNew.left = rcNew.right - iButtonWidth;
    rcNew.top = rcWidget[i - 1].bottom + DEFAULT_SPACE;
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


    EndDeferWindowPos(hdwp);
    
}

void CPairPage::DoTimerEvent(WPARAM wParam, LPARAM lParam)
{
    //add code here
    int wmId = (int)wParam;

    switch (wmId)
    {
    case DISCOVER_TIMER:
        if (m_dwDiscoverTime == 0 || 
            m_dwDiscoverTime > BT_DISCOVER_TIME)
        {
            ::KillTimer(m_hWnd, wmId);
            EBTManager::GetInstance()->ClearBTState(STATE_DISCOVER);
            break;
        }
        else
        {
            m_dwDiscoverTime--;

            TCHAR szTime[16] = {0};
            HWND hWidgetWnd = GetDlgItem(m_hWnd, IDC_DISCOVER_TIME);

            Tick2String(m_dwDiscoverTime, szTime, DIM(szTime));

            ::SetWindowText(hWidgetWnd, szTime);
        }
        break;

    case SEARCH_TIMER:
        ::KillTimer(m_hWnd, wmId);     
        
        break;
    }
}

void CPairPage::DoCommand(WPARAM wParam, LPARAM lParam)
{
    int wmId    = LOWORD(wParam); 
    int wmEvent = HIWORD(wParam); 
    HWND hwndCtl = (HWND)lParam;

    switch (wmId)
    {
    case IDC_CHECK_DISCOVERED:
        {
            LRESULT rt = SendMessage(hwndCtl, BM_GETCHECK, 0, 0);
            if (BST_CHECKED == rt)
            {
                BZ_StartPairing();              
            }
            else if (BST_UNCHECKED == rt)
            {
                BZ_StopPairing();
                m_dwDiscoverTime = 0;
                KillTimer(m_hWnd, DISCOVER_TIMER);
                EBTManager::GetInstance()->ClearBTState(STATE_PAIR);
            }
        }
        break;

    case IDC_BT_CONNECT_DISCONECT:   
        {
            //OnButtonConnectDisconnect
            int eState = EBTManager::GetInstance()->GetBTState();
            if (eState & STATE_CONNECT)
            {
                //BT already connected
                BZ_DisConnect();
            }
            else
            {
                int index = GetSelectedItemIndex();
                if (index != -1 &&
                    index < m_u4SearchRecord)
                {
                    BZ_ConnectByDeviceRec(index);
                }    
                else
                {
                    BZ_Connect();
                }
            }
            
        }
        break;
    }
}

BOOL CPairPage::InitListViewColumns()
{
    LVCOLUMN lvColumn = {0};
    int columnIndex = 0;   
    HWND hListViewWnd = GetDlgItem(m_hWnd, IDC_LIST_BTADDRESS);

    lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

    // BT name 
    lvColumn.iSubItem = columnIndex;
    lvColumn.pszText = _T("BT Name");
    lvColumn.fmt = LVCFMT_LEFT;
    if (ListView_InsertColumn(hListViewWnd, columnIndex++, &lvColumn) == -1) 
    {
        return FALSE;
    }

    // BT ID
    lvColumn.iSubItem = columnIndex;
    lvColumn.pszText = _T("BT ID");
    lvColumn.fmt = LVCFMT_CENTER;
    if (ListView_InsertColumn(hListViewWnd, columnIndex++, &lvColumn) == -1)
    {
        return FALSE;
    }

    //BT connected protocol
    lvColumn.iSubItem = columnIndex;
    lvColumn.pszText = _T("Connected Protocol");
    lvColumn.fmt = LVCFMT_LEFT;
    if (ListView_InsertColumn(hListViewWnd, columnIndex++, &lvColumn) == -1)
    {
        return FALSE;
    }   

    //compute the size of column
    HDC hDC = GetDC(hListViewWnd);
    SIZE sz = {0};
    GetTextExtentPoint(hDC, _T("00-00-00-00-00-00"), 17, &sz);
    ReleaseDC(hListViewWnd, hDC);

    ListView_SetColumnWidth(hListViewWnd, 0, sz.cx);
    ListView_SetColumnWidth(hListViewWnd, 1, sz.cx);
    ListView_SetColumnWidth(hListViewWnd, 2, LVSCW_AUTOSIZE_USEHEADER);      

    return TRUE;
}

void CPairPage::InitLocalDevice(void)
{
    HWND hWidgetWnd = NULL;

    //get device name
    TCHAR szLocalName[MAX_LEN_DEVICE_NAME] = {0};    
    BZ_GetLocalDevName(szLocalName);      
    hWidgetWnd = GetDlgItem(m_hWnd, IDC_DEVICE_NAME);
    SetWindowText(hWidgetWnd, szLocalName);

    //get device pin code
    TCHAR szPincode[MAX_LEN_PINCODE + 3] = {0};    
    BZ_GetPinCode(szPincode, MAX_LEN_PINCODE + 3);
    hWidgetWnd = GetDlgItem(m_hWnd, IDC_PIN_CODE);
    SetWindowText(hWidgetWnd, szPincode);
}

void CPairPage::SearchRemoteDevice(void)
{
    E_DEVICE_SEARCH_STATE_T eDevSearchState;

    BZ_GetSearchState(eDevSearchState);
    if (DEVICE_SEARCH_STATE_SEARCHING == eDevSearchState)            
    {
        return;
    }

    m_u4SearchRecord = 0;
    BZ_StartSearch();

    EBTManager::GetInstance()->SetBTState(STATE_SEARCH);

    RefreshUI(STATE_SEARCH);
    //SetTimer(m_hWnd, SEARCH_TIMER, 100, NULL);
}


void CPairPage::RegisterCallBack(void)
{
    //register callback function   

    BZ_RegBTCallBack(ID_PAIR, BZ_MODULE_PAIRING, OnBTPair);

    BZ_RegBTCallBack(ID_PAIR, BZ_MODULE_CONNECTION, OnBTCallConnect);    
   
    BZ_RegBTCallBack(ID_PAIR, BZ_MODULE_SEARCH, OnBTSearchMgr);
    BZ_RegBTCallBack(ID_PAIR, BZ_MODULE_OCBT, OnBTSearchSwitch);
    BZ_RegBTCallBack(ID_PAIR, BZ_MODULE_CONNECTION, OnBTSearchConnect);
}

void CPairPage::OnBTPair(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
    EBTManager *pBTManager = EBTManager::GetInstance();
    CPairPage *pThis = (CPairPage *)pBTManager->GetSubInstance(TAB_PAIR);

    switch(dwIndID)
    {
    case BZ_IND_PAIR_SUCCESS:
        {
            int eState = pBTManager->GetBTState();
            if (eState & STATE_CONNECT)
            {
            }
            else
            {
                
            }
        }
        break;

    case BZ_IND_PAIR_FAIL:
        {
           
        }
        break;

    case BZ_IND_SET_LOCALDEVNAME_SUCCESS:
        {
            
        }
        break;

    case BZ_IND_SET_LOCALDEVNAME_FAIL:
        {
            
        }
        break;

    case BZ_IND_START_PAIRING_FAIL:
        pThis->m_dwDiscoverTime = 0;
        ::KillTimer(pThis->m_hWnd, DISCOVER_TIMER);
        pBTManager->ClearBTState(STATE_PAIR);
        break;

    case BZ_IND_START_PAIRING_SUCCESS:
        pThis->m_dwDiscoverTime = BT_DISCOVER_TIME;
        ::SetTimer(pThis->m_hWnd, DISCOVER_TIMER, 1000, NULL);
        pBTManager->SetBTState(STATE_PAIR);
        break;


    default:
        break;
        //BZ_CloseBlueTooth();
    }
}



void CPairPage::OnBTCallConnect(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
    
}

void CPairPage::OnBTSearchMgr(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
    EBTManager *pBTManager = EBTManager::GetInstance();
    CPairPage *pThis = (CPairPage *)pBTManager->GetSubInstance(TAB_PAIR);

    switch (dwIndID)
    {
    case BZ_IND_DEVICESEARCH_SUCCESS:
        {
            BT_RECORD btRecord = {0};
            P_BT_DEVICE_INFO_NODE_T  pRecord = NULL;

            BZ_GetDeviceRec(pThis->m_u4SearchRecord, &pRecord);
            if (pRecord->DeviceInfo.name && 
                _tcslen((TCHAR *)(pRecord->DeviceInfo.name)) > 0)
            {
                _stprintf(btRecord.szBTName, _T("%s"), (TCHAR *)(pRecord->DeviceInfo.name));
            }
            if (pRecord->wszDevAddr &&
                _tcslen(pRecord->wszDevAddr) > 0)
            {
                _stprintf(btRecord.szBTID, _T("%s"), (TCHAR *)(pRecord->wszDevAddr));
            }
            pThis->Insert2List(btRecord);
            pThis->m_u4SearchRecord++;
        }        
        break;

    case BZ_IND_DEVICESEARCH_FINISHED:        
        pBTManager->ClearBTState(STATE_SEARCH);
        pBTManager->SetBTState(STATE_PAIR); //enter next state
        pThis->RefreshUI(STATE_SEARCH);
        break;
    }
}

void CPairPage::OnBTSearchSwitch(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{

}

void CPairPage::OnBTSearchConnect(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
    switch (dwIndID)
    {
    case BZ_IND_CREATE_CONNECT_SUCCESS:
    case BZ_IND_DISCONNECT_SUCCESS:
    case BZ_IND_CREATE_CONNECT_FAIL:
    case BZ_IND_DISCONNECT_FAIL:
    case BZ_IND_CONNECT_STATUS_CHANGED:
        break;
    }
}

void CPairPage::OnBTSearchHeadSet(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{

}

void CPairPage::OnBTSearchPair(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{

}

void CPairPage::Insert2List(BT_RECORD btRecord)
{  
    //insert to list view
    DWORD dwIndex = 0;
    HWND hListWnd = ::GetDlgItem(m_hWnd, IDC_LIST_BTADDRESS);

    ASSERT(hListWnd != NULL);

    if (0 == m_u4SearchRecord)
    {
        ListView_DeleteAllItems(hListWnd);
    }

    // serch item
    TCHAR szFmt[MAX_PATH] = {0};
    LVFINDINFO finditem;
    memset(&finditem, 0, sizeof(LVFINDINFO));
    finditem.flags = LVFI_PARAM;
    finditem.lParam = (LPARAM)btRecord.szBTID;
    dwIndex = ListView_FindItem(hListWnd, -1, &finditem);
    if (dwIndex != -1)
    {
        //this item alrady in listview,update item
        if (_tcslen(btRecord.szBTName) > 0)
        {
             _stprintf( szFmt, _T("%s"), btRecord.szBTName );
             ListView_SetItemText( hListWnd, dwIndex, 0, szFmt );
        }

        if (_tcslen(btRecord.szBTID) > 0)
        {
            _stprintf( szFmt, _T("%S"), btRecord.szBTID );
            ListView_SetItemText( hListWnd, dwIndex, 1, szFmt );
        }

        if (_tcslen(btRecord.szBTProfile) > 0)
        {
            _stprintf( szFmt, _T("%S"), btRecord.szBTProfile );
            ListView_SetItemText( hListWnd, dwIndex, 2, szFmt );
        }
    }
    else
    {
        //insert new item
        LVITEM lvItem;
        memset(&lvItem, 0, sizeof(LVITEM));
        lvItem.mask = LVIF_TEXT | LVIF_PARAM;
        lvItem.iItem = 0;
        lvItem.iSubItem = 0;
        lvItem.pszText = btRecord.szBTName;
        lvItem.lParam = (LPARAM)btRecord.szBTID;
        dwIndex = ListView_InsertItem(hListWnd, &lvItem);

        // Add subitems
        //BT ID        
        _stprintf( szFmt, _T("%S"), btRecord.szBTID );
        ListView_SetItemText( hListWnd, dwIndex, 1, szFmt );

        //BT Profile
        if (_tcslen(btRecord.szBTProfile) > 0)
        {
            _stprintf( szFmt, _T("%S"), btRecord.szBTProfile );
            ListView_SetItemText( hListWnd, dwIndex, 2, szFmt );
        }

        m_u4BTIndex = dwIndex + 1;
    }
}

int CPairPage::GetSelectedItemIndex(void)
{
    int nIndex = -1;
    LVITEM lvItem = {0};
    HWND hListViewWnd = ::GetDlgItem(m_hWnd, IDC_LIST_BTADDRESS);

    lvItem.mask = LVIF_PARAM;
    nIndex = ListView_GetNextItem(hListViewWnd, nIndex, 0);

    while( nIndex != -1 )
    {
        lvItem.iItem = nIndex;
        if (ListView_GetItemState(hListViewWnd, nIndex, LVIS_SELECTED))
        {
            return nIndex;
        }
        nIndex = ListView_GetNextItem(hListViewWnd, nIndex, 0);
    }

    return -1;
}