#include "stdafx.h"
#include "TrackPage.h"
#include "EGNSSProber.h"
#include "CommPage.h"

extern INSTANCE_ENTRYDATA g_tEntryData;

#define SV_BALL_RADIUIS         (10)
#define SV_MAX_SNR_VIEW         (55)

enum
{
    POSITION_TIMER = 1,
    REBOOT_TIMER,

};

DWORD CALLBACK GNSSDataInt(LPVOID lpPara)
{
    CTrackPage *pThis = (CTrackPage *)lpPara;

    if (NULL == pThis)
    {
        return 0;
    }
    else
    {
        ::SetThreadPriority(pThis->m_hGNSSThread, THREAD_PRIORITY_ABOVE_NORMAL);
    }

    RETAILMSG(1, (_T("[EGNSSProber]GNSSDataInt: run\r\n")));

    MSG msg = {0};
    DWORD dwResult = 0;
    int datalen = 0;
    char *pData = NULL;
    char buf[DEFAULT_BUF_LEN] = {0};

    while (::GetMessage(&msg, NULL, 0, 0))
    {
        if (GNSS_DATA_MSG == msg.message)
        {
            LPVOID pLock = ENTER_LOCK(NULL);

            pData = (char *)msg.lParam;
            datalen = msg.wParam;

            if (NULL == pData)
            {
                continue;
            }

            memset(buf, 0, DEFAULT_BUF_LEN);            
            memmove((char *)buf, pData, datalen);	            
            dwResult = pThis->m_gnssParser.ParseNMEA((char *)buf, datalen);  
            pThis->m_gnssParser.UpdateSatelliteInfo();
            
            //Notify UI new nmea data incomming
            ::PostMessage(pThis->m_hWnd, GNSS_FIX_MSG, 0, 0);

            delete []pData;
            pData = NULL;

            EXIT_LOCK(pLock);
        }
    }

    DELETE_LOCK();

    RETAILMSG(1, (_T("[CGPS]GNSSDataInt: exit\r\n")));

    return 0;
}

BOOL TrackDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    CTrackPage *pThis = (CTrackPage *)GetWindowLong(hDlg, GWL_USERDATA);       

    switch (message)
    {
    case WM_INITDIALOG:
        ASSERT(lParam != NULL);         
        pThis = (CTrackPage *)lParam;
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
        ::SendMessage(g_tEntryData.m_hMainWnd, message, wParam, lParam);
        return TRUE;
        break;

    case WM_CTLCOLORSTATIC:
        {
            HDC hDC = (HDC)wParam;
            HWND hWnd = (HWND)lParam;            
            if (hWnd == GetDlgItem(pThis->m_hWnd, IDC_STATIC_FIX_VALU))
            {     
                HBRUSH hBrush = (HBRUSH)::GetStockObject(LTGRAY_BRUSH);
                switch (pThis->m_gnssPosData.uFixType)
                {
                case GNSS_FIX_TYPE_INVALID:
                    ::SetTextColor(hDC, RGB(255, 0, 0));
                    break;

                case GNSS_FIX_TYPE_2D:
                    ::SetTextColor(hDC, RGB(0, 255, 0));
                    break;

                case GNSS_FIX_TYPE_3D:
                    ::SetTextColor(hDC, RGB(0, 0, 255));
                    break;
                }      

                ::SetBkMode(hDC, TRANSPARENT);
                ::SetWindowLong(hDlg, DWL_USER, (LONG)hBrush);
                return (BOOL)hBrush;
            }            
        }
        break;

    case GNSS_FIX_MSG:
        pThis->GetPositionData(&pThis->m_gnssPosData, 0xffffffff, 0);
        pThis->RefreshUI(message);
        break;
    }



    return FALSE; 
}

void CTrackPage::GetPositionData(GNSS_POS_DATA *pGPSPosition, DWORD dwMaximumAge, DWORD dwFlags)
{
    if (NULL == pGPSPosition)
    {
        return;
    }

    memset(pGPSPosition, 0, sizeof(GNSS_POS_DATA));

    pGPSPosition->dwSize = sizeof(GNSS_POS_DATA);
    pGPSPosition->dwVersion = GNSS_VERSION_1;

    m_gnssParser.FillGNSSPosition(pGPSPosition, dwMaximumAge);
}

CTrackPage::CTrackPage()
           :CPage()
{
    m_eGNSSStatus = GNSS_IDLE;
    m_dwFixTime = 0;
}

CTrackPage::~CTrackPage()
{
    DeInit();
}

BOOL CTrackPage::Init(HWND hwndParent)
{
    TCITEM tie = {0};   

    ASSERT(hwndParent != NULL);

    tie.mask = TCIF_TEXT | TCIF_IMAGE;
    tie.iImage = -1;
    tie.pszText = _T("Track");
    if(-1 == TabCtrl_InsertItem(hwndParent, TAB_TRACK, &tie))
    {
        return FALSE;
    }
    else
    {
        m_hWnd = ::CreateDialogParam(g_tEntryData.m_hInstance, 
            MAKEINTRESOURCE(IDD_DLG_TRACK),
            hwndParent,
            TrackDlgProc,
            (LPARAM)this);        
    }

    return TRUE;
}

void CTrackPage:: DeInit()
{
    //add code here 
    if (m_hGNSSThread && m_dwGNSSThreadId != 0)
    {
        ::PostThreadMessage(m_dwGNSSThreadId, WM_QUIT, 0, 0);

        ::CloseHandle(m_hGNSSThread);
        m_hGNSSThread = NULL;
        m_dwGNSSThreadId = NULL;
    }

    m_gnssParser.DeInit();

    ::PostMessage(m_hWnd, WM_QUIT, 0, 0);
}

void CTrackPage::DoInitDialog(WPARAM wParam, LPARAM lParam)
{
    //add code here   
    ASSERT(m_hWnd != NULL);
    ::SetWindowLong(m_hWnd, GWL_USERDATA, lParam); 
}

void CTrackPage::DoResizeWindow(WPARAM wParam, LPARAM lParam)
{
    HDWP hdwp = NULL;
    RECT rcNew = {0};
    RECT rcWidget[16] = {0};   
    HWND hWidgetWnd[16] = {NULL};    
    int iWidth = LOWORD(lParam);
    int iHeight = HIWORD(lParam); 
    int iPicWidth = 0;
    int iPicHeight = 0;
    int iStaticWidth = 0;
    int iStaticHeight = 0;
    int cx = 0, cy = 0;      
    int dx = 0, dy = 0;
    int i = 0;

    ASSERT(m_hWnd != NULL);     

    dx = dy = DEFAULT_SPACE;

    hdwp = ::BeginDeferWindowPos(DIM(hWidgetWnd));

    //adjust picture    
    hWidgetWnd[i] = ::GetDlgItem(m_hWnd, IDC_STATIC_BMP);   
    ::GetWindowRect(hWidgetWnd[i], &rcWidget[i]);
    iPicWidth = rcWidget[i].right - rcWidget[i].left;
    iPicHeight = rcWidget[i].bottom - rcWidget[i].top;
    rcNew.left = dx;
    rcNew.bottom = iHeight;
    rcNew.right = rcNew.left + iPicWidth;
    rcNew.top = rcNew.bottom - iPicHeight;
    rcWidget[i] = rcNew;
    ::DeferWindowPos(hdwp,
        hWidgetWnd[i], 
        HWND_TOP,
        rcNew.left,
        rcNew.top,
        rcNew.right - rcNew.left,
        rcNew.bottom - rcNew.top,
        0);

    //adjust static control
    i++;
    for (int id = IDC_STATIC_FIXTYPE; id < IDC_STATIC_DIR_VALU; )
    {
        hWidgetWnd[i] = ::GetDlgItem(m_hWnd, id);  
        ::GetWindowRect(hWidgetWnd[i], &rcWidget[i]);
        iStaticWidth = rcWidget[i].right - rcWidget[i].left;
        iStaticHeight = rcWidget[i].bottom - rcWidget[i].top;

        dx = 3 * DEFAULT_SPACE; 
        dy = (iPicHeight - iStaticHeight * 7) / 6;

        if (IDC_STATIC_FIXTYPE == id)
        {
            rcNew.left = rcWidget[0].right + dx;
            rcNew.top = rcWidget[0].top;
        }
        else
        {
            rcNew.left = rcWidget[0].right + dx;
            rcNew.top = rcWidget[i - 1].bottom + dy;
        }
       
        rcNew.right = rcNew.left + iStaticWidth;
        rcNew.bottom = rcNew.top + iStaticHeight;
        rcWidget[i] = rcNew;
        ::DeferWindowPos(hdwp,
            hWidgetWnd[i], 
            HWND_TOP,
            rcNew.left,
            rcNew.top,
            rcNew.right - rcNew.left,
            rcNew.bottom - rcNew.top,
            0);

        i++, id++;

        hWidgetWnd[i] = ::GetDlgItem(m_hWnd, id);  
        ::GetWindowRect(hWidgetWnd[i], &rcWidget[i]);
        iStaticWidth = rcWidget[i].right - rcWidget[i].left;
        iStaticHeight = rcWidget[i].bottom - rcWidget[i].top;

        rcNew.left = rcWidget[i - 1].right + dx;
        rcNew.top = rcWidget[i - 1].top;
        rcNew.right = rcNew.left + iStaticWidth;
        rcNew.bottom = rcNew.top + iStaticHeight;
        rcWidget[i] = rcNew;
        ::DeferWindowPos(hdwp,
            hWidgetWnd[i], 
            HWND_TOP,
            rcNew.left,
            rcNew.top,
            rcNew.right - rcNew.left,
            rcNew.bottom - rcNew.top,
            0);

        i++, id++;
    }

    //adjust fix time control
    hWidgetWnd[i] = ::GetDlgItem(m_hWnd, IDC_FIX_TIME); 
    ::GetWindowRect(hWidgetWnd[i], &rcWidget[i]);
    iStaticWidth = rcWidget[i].right - rcWidget[i].left;
    iStaticHeight = rcWidget[i].bottom - rcWidget[i].top;
    rcNew.left = rcWidget[2].right + dx;
    rcNew.top = rcWidget[2].top;
    rcNew.right = iWidth;
    rcNew.bottom = rcNew.top + iStaticHeight;   
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

void CTrackPage::DoTimerEvent(WPARAM wParam, LPARAM lParam)
{
    //add code here
    int wmId = (int)wParam;

    switch (wmId)
    {
    case POSITION_TIMER:
        if (GNSS_FIX_TYPE_INVALID == m_gnssPosData.uFixType)
        {
            RefreshUI(wmId, ++m_dwFixTime);            
        }
        else
        {
            ::KillTimer(m_hWnd, wmId);
            m_dwFixTime = 0;
        }
        break;

    case REBOOT_TIMER:
        {            
            ::KillTimer(m_hWnd, wmId);   
             m_dwFixTime = 0;
            ::SetTimer(m_hWnd, POSITION_TIMER, 1000, NULL);           
        }        
        break;
    }

}

void CTrackPage::DoCommand(WPARAM wParam, LPARAM lParam)
{
    int wmId    = LOWORD(wParam); 
    int wmEvent = HIWORD(wParam); 

    switch (wmId)
    {
    case ID_COMMAND_COLD_BOOT:
    case ID_COMMAND_WARM_BOOT:       
    case ID_COMMAND_HOT_BOOT:       
    case ID_COMMAND_FULLCOLD_BOOT:
        {
            //reboot, calucate fix time
            ::SetTimer(m_hWnd, REBOOT_TIMER, 3000, NULL);
            ::KillTimer(m_hWnd, POSITION_TIMER);

            RefreshUI(wmId, 0);
        }
        
        break;
    }
}

void CTrackPage::Activate(void)
{
    if (GNSS_IDLE == m_eGNSSStatus)
    {
        m_hGNSSThread = ::CreateThread(NULL, 0, GNSSDataInt, this, 0, &m_dwGNSSThreadId);
        if (NULL == m_hGNSSThread)
        {
            RETAILMSG(1, (_T("[EGNSSProber]Init:create thread error=%d!\r\n"), GetLastError()));            
            return;
        } 
        else
        {
            m_gnssParser.Init();

            CCommPage *pCommPage = (CCommPage *)DemoManager::GetInstance()->GetSubInstance(TAB_COMM);
            ASSERT(pCommPage != NULL);
            pCommPage->RegisterMsg(m_dwGNSSThreadId, GNSS_DATA_MSG);
        }
        
    }

    m_eGNSSStatus = GNSS_ACTIVE;

    CPage::Activate();
}

void CTrackPage::DeActivate(void)
{
    m_eGNSSStatus = GNSS_DEACTIVE;

    CPage::DeActivate();
}

void CTrackPage::RefreshUI(UINT32 u4Para1, UINT32 u4Para2)
{
    UINT32 wmId = u4Para1;

    switch (wmId)
    {
    case POSITION_TIMER:
    case ID_COMMAND_COLD_BOOT:
    case ID_COMMAND_WARM_BOOT:       
    case ID_COMMAND_HOT_BOOT:       
    case ID_COMMAND_FULLCOLD_BOOT:
        {
            HWND hwndWidget = NULL;
            TCHAR szTime[16] = {0};

            _stprintf(szTime, _T("%d s"), u4Para2);
            hwndWidget = ::GetDlgItem(m_hWnd, IDC_FIX_TIME);
            ASSERT(hwndWidget != NULL);
            ::SetWindowText(hwndWidget, szTime);
        }        
        break;

    case GNSS_FIX_MSG:
        if (m_eGNSSStatus != GNSS_ACTIVE)
        {
            return;
        }
        else
        {
            RefreshPositionInfo();
            RefreshSateliteView();
        }
        break;
    }
  
}

void CTrackPage::RefreshPositionInfo(void)
{
    TCHAR szText[32] = {0};
    HWND hWidgetWnd = NULL;

    //update fix type
    hWidgetWnd = GetDlgItem(m_hWnd, IDC_STATIC_FIX_VALU);
    ASSERT(hWidgetWnd != NULL);
    switch (m_gnssPosData.uFixType)
    {
    case GNSS_FIX_TYPE_INVALID:
        _stprintf(szText, _T("NoFix"));
        break;

    case GNSS_FIX_TYPE_2D:
        _stprintf(szText, _T("2D"));
        break;

    case GNSS_FIX_TYPE_3D:
        _stprintf(szText, _T("3D"));
        break;
    }
    ::SetWindowText(hWidgetWnd, szText);

    //update UTC
    hWidgetWnd = GetDlgItem(m_hWnd, IDC_STATIC_UTC_VALU);
    ASSERT(hWidgetWnd != NULL);
    memset(szText, 0, sizeof(szText));
    _stprintf(szText, _T("%d/%.2d/%.2d %.2d:%.2d:%.2d"),
        m_gnssPosData.UTC_Time.year, 
        m_gnssPosData.UTC_Time.month, 
        m_gnssPosData.UTC_Time.mday, 
        m_gnssPosData.UTC_Time.hour,
        m_gnssPosData.UTC_Time.min, 
        m_gnssPosData.UTC_Time.sec);
    ::SetWindowText(hWidgetWnd, szText);

     //update latitude
     hWidgetWnd = GetDlgItem(m_hWnd, IDC_STATIC_LAT_VALU);
     ASSERT(hWidgetWnd != NULL);
     memset(szText, 0, sizeof(szText));
     _stprintf(szText, _T("%.6f бу"), m_gnssPosData.dbLatitude);
     ::SetWindowText(hWidgetWnd, szText);

     //update longtitude
     hWidgetWnd = GetDlgItem(m_hWnd, IDC_STATIC_LOG_VALU);
     ASSERT(hWidgetWnd != NULL);
     memset(szText, 0, sizeof(szText));
     _stprintf(szText, _T("%.6f бу"), m_gnssPosData.dbLongtitude);
     ::SetWindowText(hWidgetWnd, szText);

     //update altitude
     hWidgetWnd = GetDlgItem(m_hWnd, IDC_STATIC_ALT_VALU);
     ASSERT(hWidgetWnd != NULL);
     memset(szText, 0, sizeof(szText));
     _stprintf(szText, _T("%.6f m"), m_gnssPosData.dbAltitude);
     ::SetWindowText(hWidgetWnd, szText);

     //update speed
     hWidgetWnd = GetDlgItem(m_hWnd, IDC_STATIC_SPEED_VALU);
     ASSERT(hWidgetWnd != NULL);
     memset(szText, 0, sizeof(szText));
     _stprintf(szText, _T("%.6f km/s"), m_gnssPosData.flGSpeed);
     ::SetWindowText(hWidgetWnd, szText);

     //update heading
     hWidgetWnd = GetDlgItem(m_hWnd, IDC_STATIC_DIR_VALU);
     ASSERT(hWidgetWnd != NULL);
     memset(szText, 0, sizeof(szText));
     if (m_gnssPosData.flHeading <= 22.5 || m_gnssPosData.flHeading > 337.5)
     {
        _stprintf(szText, _T("%.6f бу %s"), m_gnssPosData.flHeading, _T("north"));
     }
     else if (m_gnssPosData.flHeading > 22.5 || m_gnssPosData.flHeading <= 67.5)
     {
         _stprintf(szText, _T("%.6f бу %s"), m_gnssPosData.flHeading, _T("northeast"));
     }
     else if (m_gnssPosData.flHeading > 67.5 || m_gnssPosData.flHeading <= 112.5)
     {
         _stprintf(szText, _T("%.6f бу %s"), m_gnssPosData.flHeading, _T("east"));
     }
     else if (m_gnssPosData.flHeading > 112.5 || m_gnssPosData.flHeading < 157.5)
     {
         _stprintf(szText, _T("%.6f бу %s"), m_gnssPosData.flHeading, _T("southeast"));
     }
     else if (m_gnssPosData.flHeading > 157.5 || m_gnssPosData.flHeading <= 202.5)
     {
         _stprintf(szText, _T("%.6f бу %s"), m_gnssPosData.flHeading, _T("south"));
     }
     else if (m_gnssPosData.flHeading > 202.5 || m_gnssPosData.flHeading <= 247.5)
     {
         _stprintf(szText, _T("%.6f бу %s"), m_gnssPosData.flHeading, _T("southwest"));
     }
     else if (m_gnssPosData.flHeading > 247.5 || m_gnssPosData.flHeading <= 292.5)
     {
         _stprintf(szText, _T("%.6f бу %s"), m_gnssPosData.flHeading, _T("west"));
     }
     else if (m_gnssPosData.flHeading > 292.5 || m_gnssPosData.flHeading <= 337.5)
     {
         _stprintf(szText, _T("%.6f бу %s"), m_gnssPosData.flHeading, _T("northwest"));
     }
     ::SetWindowText(hWidgetWnd, szText);

     //delete object
     HGDIOBJ hObj = (HGDIOBJ)::GetWindowLong(m_hWnd, DWL_USER);
     ::DeleteObject(hObj);
}

void CTrackPage::RefreshSateliteView(void)
{   
    RECT rcDlg = {0};
    RECT rcBar = {0};    
    RECT rcSky = {0};
    SIZE size = {0};
    HWND hwndSky = NULL;   
    HDC hdcDlg = NULL;
    HDC hdcSky = NULL;
    HDC hdcMemDlg = NULL;
    HDC hdcMemSky = NULL;  
    HBITMAP hMemBmpDlg = NULL;
    HBITMAP hMemBmpSky = NULL;
    HDC hdcSkyBk = NULL;
    HBITMAP hBmpSkyBk = NULL;
    HGDIOBJ hOldObjDlg = NULL;
    HGDIOBJ hOldObjSky = NULL;
    HGDIOBJ hOldObjSkyBk = NULL;
    HGDIOBJ hOldObjBrush = NULL;
    HBRUSH hBrush = NULL;
    RECT tSkyRect = {0};  
    RECT tBarRect = {0};
    COLORREF colorRef = 0;
    int cxDlg = 0, cyDlg = 0; 
    int cxBar = 0, cyBar = 0;
    int nBarCount = 0, nBarH = 0;
    int dx =0, dy = 0;

    hdcDlg = ::GetDC(m_hWnd);
    GetTextExtentPoint(hdcDlg, _T("0000"), 4, &size);

    hwndSky = ::GetDlgItem(m_hWnd, IDC_STATIC_BMP);   
    ::GetClientRect(hwndSky, &rcSky);
    ::memcpy(&tSkyRect, &rcSky, sizeof(RECT));
    ::MapWindowPoints(hwndSky, m_hWnd, (LPPOINT) &tSkyRect, 2);

    ::GetClientRect(m_hWnd, &rcDlg);    
    cxDlg = rcDlg.right - rcDlg.left;
    cyDlg = rcDlg.bottom - rcDlg.top;

    for (int i = 0; i < MAX_GNSS_SV_VIEW; i++)
    {
        if (m_gnssPosData.sv_in_view[i].prn > 0 &&
            m_gnssPosData.sv_in_view[i].snr > 0)
        {
            nBarCount++;
        }
    }

    if (nBarCount > 0)
    {
        dx = DEFAULT_SPACE * 5;
        dy = DEFAULT_SPACE;
        cxBar = (cxDlg - (nBarCount+1) * dx) / nBarCount;
        cyBar = nBarH = cyDlg - (rcSky.bottom - rcSky.top) - size.cy * 2 - DEFAULT_SPACE * 2;

        while (cxBar < size.cx)
        {
            dx -= DEFAULT_SPACE;
            cxBar = (cxDlg - (nBarCount+1) * dx) / nBarCount;

            if (dx == DEFAULT_SPACE && cxBar < size.cx)
            {
                cxBar = size.cx;
                dx = 2;
                break;
            }
        }

    }
    else
    {
        cxBar = cyBar = nBarH = 0;       
    }    
    
    //create bar memory dc    
    hdcMemDlg = ::CreateCompatibleDC(hdcDlg);
    hMemBmpDlg = ::CreateCompatibleBitmap(hdcDlg, cxDlg, cyDlg - (rcSky.bottom - rcSky.top));
    hBrush = (HBRUSH)::GetStockObject(LTGRAY_BRUSH);
    hOldObjDlg = (HBITMAP)::SelectObject(hdcMemDlg, hMemBmpDlg);
    hOldObjBrush = (HBRUSH)::SelectObject(hdcMemDlg, hBrush);
    ::SetRect(&tBarRect, 0, 0, cxDlg, cyDlg - (rcSky.bottom - rcSky.top));
    ::Rectangle(hdcMemDlg, tBarRect.left, tBarRect.top, tBarRect.right, tBarRect.bottom);    
    ::FillRect(hdcMemDlg, &tBarRect, hBrush); 
    ::SetTextColor(hdcMemDlg, RGB(255, 255, 255));
    ::SetBkMode(hdcMemDlg, TRANSPARENT);

    ::SelectObject(hdcMemDlg, hOldObjBrush);
    ::DeleteObject(hBrush);

    //create sky backgrond picture
    hdcSky = ::GetDC(hwndSky);
    hdcSkyBk = ::CreateCompatibleDC(hdcSky);
    hBmpSkyBk = ::LoadBitmap(g_tEntryData.m_hInstance, MAKEINTRESOURCE(IDB_BMP_SKY));
    hOldObjSkyBk = ::SelectObject(hdcSkyBk, hBmpSkyBk);

    //draw backgrond on memory dc
    hdcMemSky = ::CreateCompatibleDC(hdcSky);
    hMemBmpSky = ::CreateCompatibleBitmap(hdcSky, rcSky.right-rcSky.left, rcSky.bottom-rcSky.top);
    hOldObjSky = ::SelectObject(hdcMemSky, hMemBmpSky); 

    ::SetTextColor(hdcMemSky, RGB(255, 255, 255));
    ::SetBkMode(hdcMemSky, TRANSPARENT);

    ::BitBlt(hdcMemSky, 0, 0, rcSky.right-rcSky.left, rcSky.bottom-rcSky.top, 
        hdcSkyBk, 0, 0, SRCCOPY);   

    ::SelectObject(hdcSkyBk, hOldObjSkyBk);     
    ::DeleteObject(hBmpSkyBk);
    ::DeleteDC(hdcSkyBk);  

    for (int i = 0; i < MAX_GNSS_SV_VIEW; i++)
    {
        if (m_gnssPosData.sv_in_view[i].fix)
        {
            colorRef = RGB(0, 0, 128);
        }
        else
        {
            colorRef = RGB(0, 0, 255);
        }

        hBrush = ::CreateSolidBrush(colorRef);

        if (m_gnssPosData.sv_in_view[i].prn > 0 &&
            m_gnssPosData.sv_in_view[i].snr > 0)
        {

            //update snr&prn
            //1.draw on memory dc
           
            rcBar.bottom = tSkyRect.top - dy * 2 - size.cy;
            rcBar.left += dx;
            cyBar = nBarH * m_gnssPosData.sv_in_view[i].snr / SV_MAX_SNR_VIEW;
            rcBar.top =  rcBar.bottom - cyBar;
            rcBar.right = rcBar.left + cxBar;
            
            ::Rectangle(hdcMemDlg, rcBar.left, rcBar.top, rcBar.right, rcBar.bottom);            
            ::FillRect(hdcMemDlg, &rcBar, hBrush);  

            //draw prn & snr number
            TCHAR szText[4] = {0};
            RECT rcText = {0};          

            _stprintf(szText, _T("%d"), m_gnssPosData.sv_in_view[i].prn);            
            rcText.left = rcBar.left;
            rcText.right = rcText.left + size.cx;
            rcText.top = rcBar.bottom;
            rcText.bottom = rcText.top + size.cy;           
            ::DrawText(hdcMemDlg, szText,  _tcslen(szText), &rcText, DT_VCENTER | DT_LEFT); 

            _stprintf(szText, _T("%d"), m_gnssPosData.sv_in_view[i].snr);            
            rcText.left = rcBar.left;
            rcText.right = rcText.left + size.cx;
            rcText.bottom = rcBar.top;
            rcText.top = rcText.bottom - size.cy;           
            ::DrawText(hdcMemDlg, szText,  _tcslen(szText), &rcText, DT_VCENTER | DT_LEFT);                       

            rcBar.left += cxBar;

            //update sky view    
            int centerX = (rcSky.left + rcSky.right) / 2;
            int centerY = (rcSky.bottom + rcSky.top) / 2;
            double scaleX = ((rcSky.right - rcSky.left)/2)/90.0;
            double scaleY = ((rcSky.bottom - rcSky.top)/2)/90.0;  
            int cX = centerX, cY = centerY;  

            cX += int(sin(m_gnssPosData.sv_in_view[i].azim/180.0*3.14)*(90-m_gnssPosData.sv_in_view[i].elevation)*0.8*scaleX);
            cY -= int(cos(m_gnssPosData.sv_in_view[i].azim/180.0*3.14)*(90-m_gnssPosData.sv_in_view[i].elevation)*0.8*scaleY);
            if (cX >= rcSky.left && cX <= rcSky.right && cY >= rcSky.top && cY <= rcSky.bottom)
            {                
                RECT tRect = {cX - SV_BALL_RADIUIS, cY - SV_BALL_RADIUIS, cX + SV_BALL_RADIUIS, cY + SV_BALL_RADIUIS};

                HGDIOBJ hOldObj = ::SelectObject(hdcMemSky, hBrush);
                ::Ellipse(hdcMemSky, tRect.left, tRect.top, tRect.right, tRect.bottom);                
                ::SelectObject(hdcMemSky, hOldObj);       
                              
                _stprintf(szText, _T("%d"), m_gnssPosData.sv_in_view[i].prn);    

                ::DrawText(hdcMemSky, szText,  _tcslen(szText), &tRect, DT_VCENTER | DT_CENTER); 
            }
        }

        ::DeleteObject(hBrush);
    }

    //draw on screen

    ::BitBlt(hdcDlg, 0, 0, cxDlg, cyDlg - (rcSky.bottom - rcSky.top), 
        hdcMemDlg, 0, 0, SRCCOPY); 

    ::BitBlt(hdcSky, 0, 0, rcSky.right-rcSky.left, rcSky.bottom-rcSky.top, 
        hdcMemSky, 0, 0, SRCCOPY);  


    ::SelectObject(hdcMemDlg, hOldObjDlg);
    ::DeleteObject(hMemBmpDlg);
    ::DeleteDC(hdcMemDlg);

    ::SelectObject(hdcMemSky, hOldObjSky);
    ::DeleteObject(hMemBmpSky);
    ::DeleteDC(hdcMemSky); 

    ::ReleaseDC(hwndSky, hdcSky);
    ::ReleaseDC(m_hWnd, hdcDlg);
}

