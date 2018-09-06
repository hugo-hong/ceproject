#include "stdafx.h"
#include "CommPage.h"
#include "EGNSSProber.h"
#include <sipapi.h>

extern INSTANCE_ENTRYDATA g_tEntryData;

static const TCHAR *g_pCmd[] = {_T("$PMTK103*30\r\n"), //cold 
                                _T("$PMTK102*31\r\n"), //warm
                                _T("$PMTK101*32\r\n"), //hot
                                _T("$PMTK104*37\r\n") //full cold
                                };

INT_PTR CALLBACK SettingsBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{    
    CCommPage *pThis = (CCommPage *)DemoManager::GetInstance()->GetSubInstance(TAB_COMM);

    ASSERT(pThis != NULL);

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

            pThis->m_hSettingsBox = hDlg;
            pThis->ResetComm();
        }
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        {
            int wmId    = LOWORD(wParam); 
            int wmEvent = HIWORD(wParam); 

            switch (wmId)
            {
            case IDC_BUTTON_OK:
                //get comm property value
                pThis->GetCommDCB();
                ::EndDialog(hDlg, 0);
                return TRUE;
                break;

            case IDC_BUTTON_CANCEL:
                //do nothing
                ::EndDialog(hDlg, 0);
                return TRUE;
                break;
            }                        
        }       
        break;

    case WM_CLOSE:
        ::EndDialog(hDlg, message);
        return TRUE;

    }
    return (INT_PTR)FALSE;
}


BOOL CommDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    CCommPage *pThis = (CCommPage *)GetWindowLong(hDlg, GWL_USERDATA);       

    switch (message)
    {
    case WM_INITDIALOG:
        ASSERT(lParam != NULL);
        ::SetWindowLong(hDlg, GWL_USERDATA, lParam);  
        pThis = (CCommPage *)lParam;
        pThis->m_hWnd = hDlg;
        pThis->DoInitDialog(wParam, lParam);
        return TRUE;
        break;

    case WM_NOTIFY:
        ASSERT(pThis != NULL);
        pThis->DoNotify(wParam, lParam);
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
        ::SendMessage(g_tEntryData.m_hMainWnd, message, wParam, lParam);
        return TRUE;
        break;

    case UART_RX_DATA_MSG:
        ASSERT(pThis != NULL);
        pThis->ReceiveData((LPCSTR)lParam, (DWORD)wParam);
        return TRUE;
        break;

    }

    return FALSE; 
}

void CALLBACK OnRxDataInd(void *pCaller, PBYTE pData, DWORD dwLen)
{
    CCommPage *pThis = (CCommPage *)pCaller;

    ASSERT(pThis != NULL && pData != NULL);   

    PBYTE pBuf1 = NULL;
    PBYTE pBuf2 = NULL;

    pBuf1 = new BYTE[dwLen];
    pBuf2 = new BYTE[dwLen];
    if (NULL == pBuf1 || NULL == pBuf2)
    {
        RETAILMSG(1, (_T("[EGNSSProber]OnRxDataInd:no memory!!!\r\n")));
        return;
    }
    else
    {
        memcpy(pBuf1, pData, dwLen);
        memcpy(pBuf2, pData, dwLen);
    }

    //dispatch data to register caller
    pThis->DispatchMsg(dwLen, (DWORD)pBuf1);

    //Notity UI to show data 
    ::PostMessage(pThis->m_hWnd, UART_RX_DATA_MSG, (WPARAM)dwLen, (LPARAM)pBuf2);
}

CCommPage::CCommPage()
          :CPage()
{
     m_ePortStatus = PORT_IDLE;
     m_hSettingsBox = NULL;
     m_dwRxBytes = 0;
     m_dwTxBytes = 0;

     m_hLog = NULL;
     m_fgSaveLog = FALSE;
}

CCommPage::~CCommPage()
{
    DeInit();
}

BOOL CCommPage::Init(HWND hwndParent)
{
    TCITEM tie = {0};   

    ASSERT(hwndParent != NULL);

    tie.mask = TCIF_TEXT | TCIF_IMAGE;
    tie.iImage = -1;
    tie.pszText = _T("COMM");
    if(-1 == TabCtrl_InsertItem(hwndParent, TAB_COMM, &tie))
    {
        return FALSE;
    }
    else
    {
        m_hWnd = CreateDialogParam(g_tEntryData.m_hInstance, 
            MAKEINTRESOURCE(IDD_DLG_COMM),
            hwndParent,
            CommDlgProc,
            (LPARAM)this);        
    }

    return TRUE;
}

void CCommPage:: DeInit()
{
    //add code here 
    m_serial.ClosePort();

    ::PostMessage(m_hWnd, WM_QUIT, 0, 0);
}

void CCommPage::RegisterMsg(DWORD dwId, DWORD dwMsg)
{
    m_msgMap.insert(make_pair(dwId, dwMsg));
}

void CCommPage::UnRegisterMsg(DWORD dwId, DWORD dwMsg)
{
    map<DWORD, DWORD>::iterator map_it;

    map_it = m_msgMap.find(dwId);
    m_msgMap.erase(map_it);
}

void CCommPage::DispatchMsg(DWORD dwPara1, DWORD dwPara2, DWORD eMsg)
{
    map<DWORD, DWORD>::const_iterator map_it = m_msgMap.begin();

    switch (eMsg)
    {
    case DATA_MSG:
        while (map_it != m_msgMap.end())
        {
            if (GNSS_DATA_MSG == map_it->second)
            {
                ::PostThreadMessage(map_it->first, map_it->second, dwPara1, dwPara2);
            }

            map_it++;
        }
        break;

    case CMD_MSG:
        while (map_it != m_msgMap.end())
        {
            if (GNSS_CMD_MSG == map_it->second)
            {
                ::PostMessage((HWND)map_it->first, map_it->second, dwPara1, dwPara2);
            }

            map_it++;
        }
        break;         
    }
}

void CCommPage::ReceiveData(LPCSTR pData, DWORD dwLen)
{
    if (NULL == pData || 0 == dwLen)
    {
        return;
    }

    if (PORT_CONNECTED == m_ePortStatus)
    {
        m_ePortStatus = PORT_COMMUNICATE;
    }    

    tstring strRx = _T("");
    HWND hWidgetWnd = ::GetDlgItem(m_hWnd, IDC_EDIT_RX);
    LPWSTR pWData =  MByteToWChar((LPCSTR)pData);
    ASSERT(hWidgetWnd != NULL);;
    if (pWData)
    {
        DWORD dwStart, dwEnd;       

        ::SendMessage(hWidgetWnd, EM_SETSEL, 0, -1);
        ::SendMessage(hWidgetWnd, EM_GETSEL, (WPARAM)&dwStart, (LPARAM)&dwEnd);
        if (dwEnd > (16 * KB_SIZE))
        {
            ::SetWindowText(hWidgetWnd, strRx.c_str());
            ::SendMessage(hWidgetWnd, EM_SETSEL, 0, 0);
        }
        else
        {
            ::SendMessage(hWidgetWnd, EM_SETSEL, -1, -1);
        }

        strRx.append(pWData);
        strRx += _T("\r\n");

        ::SendMessage(hWidgetWnd, EM_REPLACESEL , 0, (LPARAM)strRx.c_str());    

        //free buffer
        DeleteBuffer(pWData);        
    }  

    //save log to file
    if (m_fgSaveLog && m_strLogPath.length() > 0)
    {
        m_hLog = Write2File(m_hLog, (LPBYTE)pData, dwLen, (LPCTSTR)m_strLogPath.c_str());
        ::FlushFileBuffers(m_hLog);
    }

    //notify status bar to update
    strRx.empty();
    m_dwRxBytes += dwLen;
    _stprintf((TCHAR *)strRx.c_str(), _T("RX(Bytes):%d"), m_dwRxBytes);
    DemoManager::GetInstance()->RefreshUI(UART_RX_DATA_MSG, (LPARAM)strRx.c_str());   

    delete []pData;
    pData = NULL;   
}

void CCommPage::TransmitData(LPCTSTR pData, DWORD dwLen)
{
    if (NULL == pData || 0 == dwLen)
    {
        return;
    }

    if (PORT_CONNECTED == m_ePortStatus)
    {
        m_ePortStatus = PORT_COMMUNICATE;
    }

    DWORD dwLength = 0;
    LPSTR pMData = WCharToMByte((LPCWSTR)pData);

    if (pMData)
    {
        dwLength = strlen(pMData);
        m_serial.WritePort(pMData, dwLength);

        //free buffer
        DeleteBuffer(pMData);       
    }   

    //notify status bar to update
    TCHAR szTx[32] = {0};
    m_dwTxBytes += dwLength;
    _stprintf((TCHAR *)szTx, _T("TX(Bytes):%d"), m_dwTxBytes);
    DemoManager::GetInstance()->RefreshUI(UART_TX_DATA_MSG, (LPARAM)szTx);
}

void CCommPage::ResetComm(void)
{
    ASSERT(m_hSettingsBox != NULL);

    TCHAR szTemp[8] = {0};
    int index = 0;
    HWND hWidgetWnd = NULL;

    //reset port   
    hWidgetWnd = ::GetDlgItem(m_hSettingsBox, IDC_COMBO_PORT);
    ASSERT(hWidgetWnd != NULL);
    ::SendMessage(hWidgetWnd, CB_RESETCONTENT, 0, 0);
    for (index = 0; index < 10; index++)
    {
        _stprintf(szTemp, _T("COM%d:"), index);
        ::SendMessage(hWidgetWnd, CB_INSERTSTRING, (WPARAM)index, (LPARAM)szTemp);        
    }
    ::SendMessage(hWidgetWnd, CB_SETCURSEL, m_commDCB.portNo, 0);

    //reset baudrate
    hWidgetWnd = ::GetDlgItem(m_hSettingsBox, IDC_COMBO_BAUDRATE);
    ASSERT(hWidgetWnd != NULL);
    index = 0;
    ::SendMessage(hWidgetWnd, CB_RESETCONTENT, 0, 0);
    ::SendMessage(hWidgetWnd, CB_INSERTSTRING, (WPARAM)index++, (LPARAM)_T("9600"));
    ::SendMessage(hWidgetWnd, CB_INSERTSTRING, (WPARAM)index++, (LPARAM)_T("38400"));
    ::SendMessage(hWidgetWnd, CB_INSERTSTRING, (WPARAM)index++, (LPARAM)_T("57600"));
    ::SendMessage(hWidgetWnd, CB_INSERTSTRING, (WPARAM)index++, (LPARAM)_T("115200"));
    ::SendMessage(hWidgetWnd, CB_INSERTSTRING, (WPARAM)index++, (LPARAM)_T("921600"));
    switch (m_commDCB.baud)
    {
    case 9600:
        ::SendMessage(hWidgetWnd, CB_SETCURSEL, 0, 0);
        break;

    case 38400:
        ::SendMessage(hWidgetWnd, CB_SETCURSEL, 1, 0);
        break;

    case 57600:
        ::SendMessage(hWidgetWnd, CB_SETCURSEL, 2, 0);
        break;

    case 115200:
        ::SendMessage(hWidgetWnd, CB_SETCURSEL, 3, 0);
        break;

    case 921600:
        ::SendMessage(hWidgetWnd, CB_SETCURSEL, 4, 0);
        break;
    }


    //reset parity
    hWidgetWnd = ::GetDlgItem(m_hSettingsBox, IDC_COMBO_PARITY);
    ASSERT(hWidgetWnd != NULL);
    index = 0;
    ::SendMessage(hWidgetWnd, CB_RESETCONTENT, 0, 0);
    ::SendMessage(hWidgetWnd, CB_INSERTSTRING, (WPARAM)index++, (LPARAM)_T("NOPARITY"));
    ::SendMessage(hWidgetWnd, CB_INSERTSTRING, (WPARAM)index++, (LPARAM)_T("ODDPARITY"));
    ::SendMessage(hWidgetWnd, CB_INSERTSTRING, (WPARAM)index++, (LPARAM)_T("EVENPARITY"));
    ::SendMessage(hWidgetWnd, CB_SETCURSEL, 0, 0);

    //reset databits
    hWidgetWnd = ::GetDlgItem(m_hSettingsBox, IDC_COMBO_DATABITS);
    ASSERT(hWidgetWnd != NULL);
    index = 0;
    ::SendMessage(hWidgetWnd, CB_RESETCONTENT, 0, 0);
    ::SendMessage(hWidgetWnd, CB_INSERTSTRING, (WPARAM)index++, (LPARAM)_T("8"));
    ::SendMessage(hWidgetWnd, CB_SETCURSEL, 0, 0);

    //reset stopbits
    hWidgetWnd = ::GetDlgItem(m_hSettingsBox, IDC_COMBO_STOPBITS);
    ASSERT(hWidgetWnd != NULL);
    index = 0;
    ::SendMessage(hWidgetWnd, CB_RESETCONTENT, 0, 0);
    ::SendMessage(hWidgetWnd, CB_INSERTSTRING, (WPARAM)index++, (LPARAM)_T("ONESTOPBIT"));
    ::SendMessage(hWidgetWnd, CB_INSERTSTRING, (WPARAM)index++, (LPARAM)_T("ONE5STOPBITS"));
    ::SendMessage(hWidgetWnd, CB_INSERTSTRING, (WPARAM)index++, (LPARAM)_T("TWOSTOPBITS"));
    ::SendMessage(hWidgetWnd, CB_SETCURSEL, 0, 0);

}

void CCommPage::GetCommDCB(void)
{
    ASSERT(m_hSettingsBox != NULL);

    TCHAR szTemp[16] = {0};
    TCHAR szData[16] = {0};
    int index = 0;
    HWND hWidgetWnd = NULL;

    //get port
    hWidgetWnd = ::GetDlgItem(m_hSettingsBox, IDC_COMBO_PORT);
    ASSERT(hWidgetWnd != NULL);
    index = ::SendMessage(hWidgetWnd, CB_GETCURSEL, 0, 0);
    ::SendMessage(hWidgetWnd, CB_GETLBTEXT, index, (LPARAM)szTemp);
    _stscanf(szTemp, _T("COM%d:"), &m_commDCB.portNo);
  
    //get baudrate
    hWidgetWnd = ::GetDlgItem(m_hSettingsBox, IDC_COMBO_BAUDRATE);
    ASSERT(hWidgetWnd != NULL);
    index = ::SendMessage(hWidgetWnd, CB_GETCURSEL, 0, 0);
    ::SendMessage(hWidgetWnd, CB_GETLBTEXT, index, (LPARAM)szTemp);
    m_commDCB.baud = _ttoi(szTemp);

    //get parity
    hWidgetWnd = ::GetDlgItem(m_hSettingsBox, IDC_COMBO_PARITY);
    ASSERT(hWidgetWnd != NULL);
    index = ::SendMessage(hWidgetWnd, CB_GETCURSEL, 0, 0);
    ::SendMessage(hWidgetWnd, CB_GETLBTEXT, index, (LPARAM)szTemp);
    m_commDCB.parity = _ttoi(szTemp);

    //get databits
    hWidgetWnd = ::GetDlgItem(m_hSettingsBox, IDC_COMBO_DATABITS);
    ASSERT(hWidgetWnd != NULL);
    index = ::SendMessage(hWidgetWnd, CB_GETCURSEL, 0, 0);
    ::SendMessage(hWidgetWnd, CB_GETLBTEXT, index, (LPARAM)szTemp);
    m_commDCB.databits = _ttoi(szTemp);

    //get stopbits
    hWidgetWnd = ::GetDlgItem(m_hSettingsBox, IDC_COMBO_STOPBITS);
    ASSERT(hWidgetWnd != NULL);
    index = ::SendMessage(hWidgetWnd, CB_GETCURSEL, 0, 0);
    ::SendMessage(hWidgetWnd, CB_GETLBTEXT, index, (LPARAM)szTemp);
    m_commDCB.stopbits = _ttoi(szTemp);


}

void CCommPage::DoInitDialog(WPARAM wParam, LPARAM lParam)
{
    //add code here     
    //Init comm property
    m_commDCB.portNo = 1;
    m_commDCB.baud = 115200;
    m_commDCB.parity = NOPARITY;
    m_commDCB.databits = 8;
    m_commDCB.stopbits = ONESTOPBIT;  
}

void CCommPage::DoResizeWindow(WPARAM wParam, LPARAM lParam)
{
    HDWP hdwp = NULL;
    RECT rcNew = {0};
    RECT rcWidget[3] = {0};   
    HWND hWidgetWnd[3] = {NULL};    
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

    dx = dy = DEFAULT_SPACE;

    hdwp = ::BeginDeferWindowPos(DIM(hWidgetWnd));

    i = 1;
    hWidgetWnd[i] = ::GetDlgItem(m_hWnd, IDC_EDIT_TX);   
    ::GetWindowRect(hWidgetWnd[i], &rcWidget[i]);
    iEditWidth = rcWidget[i].right - rcWidget[i].left;
    iEditHeight = rcWidget[i].bottom - rcWidget[i].top;

    i++;
    hWidgetWnd[i] = ::GetDlgItem(m_hWnd, IDC_BUTTON_SEND);
    ::GetWindowRect(hWidgetWnd[i], &rcWidget[i]);
    iButtonWidth = rcWidget[i].right - rcWidget[i].left;
    iButtonHeight = rcWidget[i].bottom - rcWidget[i].top;

    //adjust rx edit    
    i = 0;
    hWidgetWnd[i] = ::GetDlgItem(m_hWnd, IDC_EDIT_RX);
    rcNew.left = dx;
    rcNew.top = dy;
    rcNew.right = iWidth - dx;
    rcNew.bottom = iHeight - iEditHeight - 2 * dy;
    rcWidget[i] = rcNew;
    ::DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);

    //adjust tx edit
    i++;
    rcNew.left = dx;
    rcNew.top = rcWidget[i - 1].bottom + dy;
    rcNew.right = iWidth - iButtonWidth - 2 * dx;    
    rcNew.bottom = rcNew.top + iEditHeight;
    rcWidget[i] = rcNew;
    ::DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);

    //adjust send button 
    i++;
    rcNew.left = rcWidget[i - 1].right + dx;
    rcNew.top = rcWidget[i - 1].top;
    rcNew.right = rcNew.left + iButtonWidth;
    rcNew.bottom = rcNew.top + iButtonHeight;
    rcWidget[i] = rcNew;
    ::DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);   
 
    ::EndDeferWindowPos(hdwp);
}

void CCommPage::DoTimerEvent(WPARAM wParam, LPARAM lParam)
{
    //add code here
}

void CCommPage::DoCommand(WPARAM wParam, LPARAM lParam)
{
    int wmId    = LOWORD(wParam); 
    int wmEvent = HIWORD(wParam); 

    switch (wmId)
    {
    case ID_FILE_SAVELOG:
        if (m_fgSaveLog)
        {
            if (m_hLog != NULL &&
                m_hLog != INVALID_HANDLE_VALUE)
            {
                ::FlushFileBuffers(m_hLog);

                ::CloseHandle(m_hLog);
                m_hLog = NULL;                
            }

            m_fgSaveLog = FALSE;            
        }
        else
        {
            if (m_strLogPath.length() == 0)
            {
                TCHAR szPath[MAX_PATH] = {0};
                GetCurrentDirectory(szPath, MAX_PATH); 
                _stprintf(szPath, _T("%sCOM%d_log.txt"), szPath, m_commDCB.portNo);
                m_strLogPath.append(szPath);  
                m_fgSaveLog = TRUE;
            }
        }
        break;

    case ID_EDIT_CLEARRX:
        {
            HWND hWidgetWnd = ::GetDlgItem(m_hWnd, IDC_EDIT_RX);
            ASSERT(hWidgetWnd != NULL);
            ::SetWindowText(hWidgetWnd, _T(""));
            ::SendMessage(hWidgetWnd, EM_SETSEL, 0, 0);
            m_dwRxBytes = 0;
        }
        break;

    case ID_EDIT_CLEARTX:
        {
            HWND hWidgetWnd = ::GetDlgItem(m_hWnd, IDC_EDIT_TX);
            ASSERT(hWidgetWnd != NULL);
            ::SetWindowText(hWidgetWnd, _T(""));
            m_dwTxBytes = 0;            
        }
        break;

    case ID_EDIT_CLEARALL:
        {
            HWND hWidgetWnd = ::GetDlgItem(m_hWnd, IDC_EDIT_RX);

            ASSERT(hWidgetWnd != NULL);
            ::SetWindowText(hWidgetWnd, _T(""));
            ::SendMessage(hWidgetWnd, EM_SETSEL, 0, 0);
            m_dwRxBytes = 0;

            hWidgetWnd = ::GetDlgItem(m_hWnd, IDC_EDIT_TX);
            ASSERT(hWidgetWnd != NULL);
            ::SetWindowText(hWidgetWnd, _T(""));
            m_dwTxBytes = 0; 
        }
        break;

    case ID_COMMAND_CONNECT:
        if (!m_serial.OpenPort(m_commDCB.portNo, 
                                m_commDCB.baud,
                                m_commDCB.parity,
                                m_commDCB.databits,
                                m_commDCB.stopbits))
        {
            MessageBox(m_hWnd, _T("Open com port failed!!!"), NULL, MB_ICONERROR);
        }
        else
        {            
            m_serial.RegisterPort(this, OnRxDataInd);
            m_ePortStatus = PORT_CONNECTED;
        }
        break;

    case ID_COMMAND_DISCONNECT:
        m_serial.ClosePort();
        m_ePortStatus = PORT_DISCONNECTED;
        break;

    case ID_COMMAND_SETTINGS:
        DialogBox(g_tEntryData.m_hInstance, (LPCTSTR)IDD_DLG_SETTINGS, m_hWnd, SettingsBox);
        break;

    case ID_COMMAND_COLD_BOOT:       
    case ID_COMMAND_WARM_BOOT:       
    case ID_COMMAND_HOT_BOOT:       
    case ID_COMMAND_FULLCOLD_BOOT:
         TransmitData(g_pCmd[wmId - ID_COMMAND_COLD_BOOT], 
             _tcslen(g_pCmd[wmId - ID_COMMAND_COLD_BOOT]) + sizeof(TCHAR));
        break;            

    case IDC_EDIT_TX:
        if (EN_SETFOCUS == wmEvent)
        {
            ::SipShowIM(SIPF_ON);
        }
        else if (EN_KILLFOCUS == wmEvent)
        {
            ::SipShowIM(SIPF_OFF);
        }
        break;

    case IDC_BUTTON_SEND:
        {
            HWND hWidgetWnd = ::GetDlgItem(m_hWnd, IDC_EDIT_TX);
            TCHAR text[MAX_TX_BUF_LEN] = {0};
            int nMaxCount = DIM(text);

            ASSERT(hWidgetWnd != NULL);
            nMaxCount = ::GetWindowText(hWidgetWnd, text, nMaxCount);
            TransmitData(text, (nMaxCount + 1) * sizeof(TCHAR));
        }
        break;
    }
}

