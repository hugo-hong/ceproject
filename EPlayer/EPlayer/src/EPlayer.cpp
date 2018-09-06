#include "stdafx.h"
#include "MSDK_Global.h"
#include "eutil.h"
#include "EPlayer.h"
#include "commctrl.h"

extern HWND	g_hWndCommandBar;	// command bar handle
extern INSTANCE_ENTRYDATA g_tEntryData;

MediaPlayer::MediaPlayer()
{
    
    m_fgPlay = FALSE;
    m_eMediaSrc = MEDIA_SRC_NULL;

    m_pPlayer = NULL;
}

MediaPlayer::~MediaPlayer()
{   
    Exit();
}

void MediaPlayer::HandleMediaEvent(UINT32 u4Event, UINT32 u4Param1, UINT32 u4Param2)
{
    switch (m_eMediaSrc)
    {
    case MEDIA_SRC_DISK:
        if (m_pPlayer)
        {
            m_pPlayer->HandleMediaEvent(u4Event, u4Param1, u4Param2);
        }        
        break;
    }
}

void MediaPlayer::DoInitDialog(WPARAM wParam, LPARAM lParam)
{
    ASSERT(g_tEntryData.m_hMainWnd != NULL);

    m_hWnd = g_tEntryData.m_hMainWnd;

    HWND hwndProgressBar = ::GetDlgItem(m_hWnd, IDC_SLIDER_PROGRESS); 

    ASSERT(hwndProgressBar != NULL);

    ::SendMessage(hwndProgressBar, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, 0xffff));

    ::PostMessage(g_tEntryData.m_hMainWnd, APP_RESIZE_MSG, 0, 0);
}

void MediaPlayer::DoResizeDialog(WPARAM wParam, LPARAM lParam)
{
    HWND hDlg = g_tEntryData.m_hMainWnd;
    HDWP hdwp = NULL;
    int nWidth = LOWORD(lParam);
    int nHeight = HIWORD(lParam); 
    int xWidth = 0;
    int yHeight = 0;
    HWND hWidgetWnd[6] = {NULL};
    RECT rcWidget[6] = {0};
    RECT rcMenuBar = {0};

    ASSERT(hDlg != NULL);

    //get widgets size
    hWidgetWnd[0] = GetDlgItem(hDlg, IDC_PLAYWINDOW);
    ASSERT(hWidgetWnd[0] != NULL);
    GetWindowRect(hWidgetWnd[0], &rcWidget[0]);

    hWidgetWnd[1] = GetDlgItem(hDlg, IDC_SLIDER1);
    ASSERT(hWidgetWnd[1] != NULL);
    GetWindowRect(hWidgetWnd[1], &rcWidget[1]);

    hWidgetWnd[2] = GetDlgItem(hDlg, IDC_SLIDER2);
    ASSERT(hWidgetWnd[2] != NULL);
    GetWindowRect(hWidgetWnd[2], &rcWidget[2]);

    hWidgetWnd[3] = GetDlgItem(hDlg, IDC_PREV);
    ASSERT(hWidgetWnd[3] != NULL);
    GetWindowRect(hWidgetWnd[3], &rcWidget[3]);

    hWidgetWnd[4] = GetDlgItem(hDlg, IDC_PLAY_PAUSE);
    ASSERT(hWidgetWnd[4] != NULL);
    GetWindowRect(hWidgetWnd[4], &rcWidget[4]);

    hWidgetWnd[5] = GetDlgItem(hDlg, IDC_NEXT);
    ASSERT(hWidgetWnd[5] != NULL);
    GetWindowRect(hWidgetWnd[5], &rcWidget[5]);

    //get menu bar size
    if (g_hWndCommandBar)
    {
        GetWindowRect(g_hWndCommandBar, &rcMenuBar);
    }


    hdwp = BeginDeferWindowPos(6);

    MapWindowPoints(HWND_DESKTOP, hDlg, (LPPOINT) &rcMenuBar, 2);

    //adjust static widget position
    MapWindowPoints(HWND_DESKTOP, hDlg, (LPPOINT) &rcWidget[0], 2);

    rcWidget[0].left = DEFAULT_SPACE;
    rcWidget[0].top = rcMenuBar.bottom - rcMenuBar.top + DEFAULT_SPACE;
    rcWidget[0].right = nWidth - DEFAULT_SPACE;
    rcWidget[0].bottom = (nHeight - DEFAULT_SPACE) - (rcWidget[2].bottom - rcWidget[2].top) - 
        (rcWidget[1].bottom - rcWidget[1].top) - DEFAULT_SPACE;

    DeferWindowPos(hdwp,
        hWidgetWnd[0], 
        HWND_TOP,
        rcWidget[0].left,
        rcWidget[0].top,
        rcWidget[0].right - rcWidget[0].left,
        rcWidget[0].bottom - rcWidget[0].top,
        0);

    //adjust play progress(slider1) position
    MapWindowPoints(HWND_DESKTOP, hDlg, (LPPOINT) &rcWidget[1], 2);

    yHeight = rcWidget[1].bottom - rcWidget[1].top;

    rcWidget[1].left = DEFAULT_SPACE;
    rcWidget[1].top = rcWidget[0].bottom + 2;
    rcWidget[1].right = rcWidget[0].right - 10;
    rcWidget[1].bottom = rcWidget[1].top + yHeight;

    DeferWindowPos(hdwp,
        hWidgetWnd[1], 
        HWND_TOP,
        rcWidget[1].left,
        rcWidget[1].top,
        rcWidget[1].right - rcWidget[1].left,
        rcWidget[1].bottom - rcWidget[1].top,
        0);   

    //adjust volume control(slider2) position
    MapWindowPoints(HWND_DESKTOP, hDlg, (LPPOINT) &rcWidget[2], 2);

    xWidth = rcWidget[2].right - rcWidget[2].left;
    yHeight = rcWidget[2].bottom - rcWidget[2].top;

    rcWidget[2].right = rcWidget[1].right;
    rcWidget[2].left = rcWidget[2].right - xWidth;
    rcWidget[2].top = rcWidget[1].bottom + 2;   
    rcWidget[2].bottom = rcWidget[2].top + yHeight;

    DeferWindowPos(hdwp,
        hWidgetWnd[2], 
        HWND_TOP,
        rcWidget[2].left,
        rcWidget[2].top,
        rcWidget[2].right - rcWidget[2].left,
        rcWidget[2].bottom - rcWidget[2].top,
        0);

    //adjust play prev button position
    MapWindowPoints(HWND_DESKTOP, hDlg, (LPPOINT) &rcWidget[3], 2);

    xWidth = rcWidget[3].right - rcWidget[3].left;
    yHeight = rcWidget[3].bottom - rcWidget[3].top;

    rcWidget[3].left = DEFAULT_SPACE;
    rcWidget[3].right = rcWidget[3].left + xWidth;   
    rcWidget[3].top = rcWidget[2].top;    
    rcWidget[3].bottom = rcWidget[3].top + yHeight;

    DeferWindowPos(hdwp,
        hWidgetWnd[3], 
        HWND_TOP,
        rcWidget[3].left,
        rcWidget[3].top,
        rcWidget[3].right - rcWidget[3].left,
        rcWidget[3].bottom - rcWidget[3].top,
        0);

    //adjust play/pause button position
    MapWindowPoints(HWND_DESKTOP, hDlg, (LPPOINT) &rcWidget[4], 2);

    xWidth = rcWidget[4].right - rcWidget[4].left;
    yHeight = rcWidget[4].bottom - rcWidget[4].top;

    rcWidget[4].left = rcWidget[3].right + DEFAULT_SPACE;
    rcWidget[4].right = rcWidget[4].left+ xWidth;   
    rcWidget[4].top = rcWidget[3].top;    
    rcWidget[4].bottom = rcWidget[4].top + yHeight;

    DeferWindowPos(hdwp,
        hWidgetWnd[4], 
        HWND_TOP,
        rcWidget[4].left,
        rcWidget[4].top,
        rcWidget[4].right - rcWidget[4].left,
        rcWidget[4].bottom - rcWidget[4].top,
        0);

    //adjust play next button position
    MapWindowPoints(HWND_DESKTOP, hDlg, (LPPOINT) &rcWidget[5], 2);

    xWidth = rcWidget[5].right - rcWidget[5].left;
    yHeight = rcWidget[5].bottom - rcWidget[5].top;

    rcWidget[5].left = rcWidget[4].right + DEFAULT_SPACE;
    rcWidget[5].right = rcWidget[5].left + xWidth;    
    rcWidget[5].top = rcWidget[4].top;    
    rcWidget[5].bottom = rcWidget[5].top + yHeight;

    DeferWindowPos(hdwp,
        hWidgetWnd[5], 
        HWND_TOP,
        rcWidget[5].left,
        rcWidget[5].top,
        rcWidget[5].right - rcWidget[5].left,
        rcWidget[5].bottom - rcWidget[5].top,
        0);

    EndDeferWindowPos(hdwp);   

    //adjust play window
    ResetPlayWindow();
}


void MediaPlayer::Entry(void)
{
    //init COM 
    ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
}

void MediaPlayer::Exit(void)
{
    Stop();

    if (m_pPlayer)
    {
        m_pPlayer->DeInit();

        delete m_pPlayer;
        m_pPlayer = NULL;
    }

    m_eMediaSrc = MEDIA_SRC_NULL;  

    ::CoFreeUnusedLibraries();

    ::CoUninitialize();
}

void MediaPlayer::SetSource(UINT SourceId)
{    
    switch (SourceId)
    {
    case ID_AVIN_AV1:  
        m_eMediaSrc = MEDIA_SRC_AVIN1;
        m_pPlayer = new MAVINPlayer;
        if (m_pPlayer)
        {
            m_pPlayer->Init();
            ResetPlayWindow();
            m_pPlayer->SetSource(m_eMediaSrc);
            m_fgPlay = TRUE;
        }        
        break;

    case ID_AVIN_AV2:
        m_eMediaSrc = MEDIA_SRC_AVIN2;
        m_pPlayer = new MAVINPlayer;
        if (m_pPlayer)
        {
            m_pPlayer->Init();
            ResetPlayWindow();
            m_pPlayer->SetSource(m_eMediaSrc);
            m_fgPlay = TRUE;
        }        
        break;

    case ID_AVIN_AV3:
        m_eMediaSrc = MEDIA_SRC_AVIN3;
        m_pPlayer = new MAVINPlayer;
        if (m_pPlayer)
        {
            m_pPlayer->Init();
            ResetPlayWindow();
            m_pPlayer->SetSource(m_eMediaSrc);
            m_fgPlay = TRUE;
        }   
        break;

    case ID_AVIN_AV4:
        m_eMediaSrc = MEDIA_SRC_AVIN4;
        m_pPlayer = new MAVINPlayer;
        if (m_pPlayer)
        {
            m_pPlayer->Init();
            ResetPlayWindow();
            m_pPlayer->SetSource(m_eMediaSrc);
            m_fgPlay = TRUE;
        }   
        break;

    case ID_AVIN_AV5:
        m_eMediaSrc = MEDIA_SRC_AVIN5;
        m_pPlayer = new MAVINPlayer;
        if (m_pPlayer)
        {
            m_pPlayer->Init();
            ResetPlayWindow();
            m_pPlayer->SetSource(m_eMediaSrc);
            m_fgPlay = TRUE;
        }   
        break;

    case ID_DISK_FILE:
        m_eMediaSrc = MEDIA_SRC_DISK;
        m_pPlayer = new MDiskPlayer;
        if (m_pPlayer)
        {            
            m_pPlayer->Init();
            ResetPlayWindow();
            m_pPlayer->SetSource(m_eMediaSrc);
            m_fgPlay = TRUE;
        }           
        break;
    }
}

void MediaPlayer::ResetPlayWindow(void)
{
    RECT rcPlayWnd = {0};
    HWND hPlayWnd = NULL;

    if (m_pPlayer)
    {
        hPlayWnd = GetDlgItem(g_tEntryData.m_hMainWnd, IDC_PLAYWINDOW);

        ASSERT(hPlayWnd != NULL);

        GetWindowRect(hPlayWnd, &rcPlayWnd);

        //update video size
        if (g_tEntryData.m_fgFullSreen)
        {
            SetRect(&rcPlayWnd, 
                0,
                0,
                g_tEntryData.m_dwWidthPixels,
                g_tEntryData.m_dwHeightPixels);
        }

        m_pPlayer->SetPlayWindow(hPlayWnd, rcPlayWnd);
    }  
}

void MediaPlayer::Play(void)
{
    if (m_fgPlay)
    {
        return;
    }

    if (m_pPlayer)
    {
        m_pPlayer->Play();

         m_fgPlay = TRUE;
    }     
}

void MediaPlayer::Pause(void)
{
    if (!m_fgPlay)
    {
        return;
    }

    if (m_pPlayer)
    {
        m_pPlayer->Pause();

        m_fgPlay = FALSE;
    }     
}

void MediaPlayer::Stop(void)
{
    if (!m_fgPlay)
    {
        return;
    }

    if (m_pPlayer)
    {
        m_pPlayer->Stop();

        m_fgPlay = FALSE;
    }     
}

void MediaPlayer::RefreshProgressBar(LPVOID lpParam)
{
    MSG_PARAM *pParam = (MSG_PARAM *)lpParam;

    if (NULL == pParam)
    {
        return;
    }

    UINT32 u4CurTime = 0;
    UINT64 u8TotalTime = 0;        

    GET_DWORD(pParam->lpData, u4CurTime, 0);
    GET_DWORD64(pParam->lpData, u8TotalTime, sizeof(DWORD));

    LONG lMaxValue = 0;
    LONG lCurValue = 0;
    LONG lNextValue = 0;
    HWND hwndProgressBar = ::GetDlgItem(m_hWnd, IDC_SLIDER_PROGRESS); 

    ASSERT(hwndProgressBar != NULL);

    lMaxValue = (LONG)SendMessage(hwndProgressBar, TBM_GETRANGEMAX, 0, 0);
    lCurValue = (LONG)SendMessage(hwndProgressBar, TBM_GETPOS, 0, 0);
    lNextValue = lMaxValue * ((UINT64)u4CurTime / u8TotalTime);

    if (lCurValue != lNextValue)
    {
        SendMessage(hwndProgressBar,TBM_SETPOS, (WPARAM)1,(LPARAM)lNextValue); 
    }
}