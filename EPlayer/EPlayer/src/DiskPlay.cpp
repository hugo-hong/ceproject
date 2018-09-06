#include "stdafx.h"
#include <CommDlg.h>
#include "ecommon.h"
#include "DiskPlay.h"
#include "MSDK_Global.h"
#include "MSDK_Error.h"
#include "MSDK_MediaGraph.h "
#include "MSDK_MediaCtrl.h"
#include "MSDK_MediaEvent.h"
#include "GSysServices.h"
#include "MSDK_StrmIID.h"
#include "MSDK_Video.h"

extern INSTANCE_ENTRYDATA g_tEntryData; 

static const TCHAR g_szFilter[] = _T("Media Files\0*.mp3;\
                                                   *.wma;\
                                                   *.wmv;\
                                                   *.avi;\
                                                   *.rm;\
                                                   *.rmvb;\
                                                   *.mp4;\
                                                   *.dat;\
                                                   *.mpg;\
                                                   *.mp2;\
                                                   *.3gp\
                                                   \0\0,\
                                     All Files(*.*)\0*.*\0\0");

MRESULT MediaEvent_Listener(HMEDIAGRAPH hMediaGraph, 
                            UINT32 u4Evt, 
                            UINT32 u4Param1, 
                            UINT32 u4Param2, 
                            UINT32 u4CustomData)
{
    MRESULT mr = S_MSDK_OK;
    MDiskPlayer *pThisPlayer = (MDiskPlayer *)u4CustomData;

    if (!pThisPlayer)
    {
        return mr;
    }
    RETAILMSG(0, (_T("[EPlayer][DISK]: MediaEvent_Listener u4Evt=%d\r\n"), u4Evt));
    switch (u4Evt)
    {
    case EVT_MSDK_FINISHED:
        break;

    case EVT_MSDK_PLAYBACK_POSITION_STATUS:
        //refresh progress bar
        pThisPlayer->HandlePlaybackPostionChange(u4Param1);
        break;

    case EVT_MSDK_PLAYED:
        break;

    case EVT_MSDK_PAUSED:
        break;

    case EVT_MSDK_STOPED:
        MediaGraph_Clear(pThisPlayer->m_hMediaGraph);
        Sleep(500);
        pThisPlayer->PlayMediaFile(pThisPlayer->m_szFileName);
        break;

    case EVT_MSDK_GRAPH_CHANGED:
        break;

    case EVT_MSDK_GRAPH_BUILT:        
        pThisPlayer->PrePlay();
        pThisPlayer->Play();
        break;
         
    case EVT_MSDK_VIDEO_SIZE_CHANGED:
        break;

    case EVT_MSDK_SET_AUDIO_FAILED:
        break;

    case EVT_MSDK_SET_AUDIO_SUCCEED:
        break;

    case EVT_MSDK_SET_SUBTITLE_FAILED:
        break;

    case EVT_MSDK_SET_SUBTITLE_SUCCEED:
        break;

    case EVT_MSDK_TIP_WARNING:
        break;

    case EVT_MSDK_ERROR_OCCURED:
        break;
    }

    return (mr);
}

void MDiskPlayer::HandleMediaEvent(UINT32 u4Event, UINT32 u4Param1, UINT32 u4Param2)
{
    if (MEDIA_MSDK_EVT == u4Event)
    {
        ASSERT(m_hMediaGraph != NULL);
        MediaGraph_HandleEvent(m_hMediaGraph);
    }
}

MDiskPlayer::MDiskPlayer(void)
{
    m_hMediaGraph = NULL;
    m_hPlayWnd = NULL;
    m_eSrcId = MEDIA_SRC_NULL;
    m_eSinkId = MEDIA_SINK_NULL;
    m_fgInited = FALSE;

    memset(&m_tSinkInfo, 0, sizeof(MSDK_SINK_INFO_T));
    memset(&m_rcPlayWnd, 0, sizeof(RECT));
    memset(m_szFileName, 0, sizeof(m_szFileName));
}

MDiskPlayer::~MDiskPlayer(void)
{
    DeInit();
}

BOOL MDiskPlayer::Init(void)
{
    BOOL fgRet = 0;
    MRESULT     rt = 0;

    if (m_fgInited)
    {
        return TRUE;
    }    

    rt = MSDK_Init();
    if (S_MSDK_OK != rt)
    {
        RETAILMSG(LOG_ON, (_T("[EPlayer][DISK]: MSDK_Init error\r\n")));
        return FALSE;
    }

    rt = MediaGraph_Create(&m_hMediaGraph);
    if (S_MSDK_OK != rt)
    {
        RETAILMSG(LOG_ON, (_T("[EPlayer][DISK]:create media graph error\r\n")));
        goto EXIT;
    }
    
    rt = MediaGraph_RegisterNotifyWindow(m_hMediaGraph, g_tEntryData.m_hMainWnd, MEDIA_MSDK_MSG);
    if (S_MSDK_OK != rt)
    {
        RETAILMSG(LOG_ON, (_T("[EPlayer][DISK]:set msdk notify window error\r\n")));
        goto EXIT;
    }

    rt = MediaGraph_AddListener(m_hMediaGraph, MediaEvent_Listener, (GUINT32)this);
    if (S_MSDK_OK != rt)
    {
        RETAILMSG(LOG_ON, (_T("[EPlayer][DISK]:set msdk listener error\r\n")));
        goto EXIT;
    }

    //Set Image decoder message

    //Init audio control
    GHANDLE hAudioDev = GOpenAudioDev();
    GChooseSpdifOutput(hAudioDev,MISC_AUD_AOUT1);
    GCloseAudioDev(hAudioDev);

    fgRet = TRUE;
    m_fgInited = TRUE;

EXIT:
    if (!fgRet)
    {
        DeInit();
    }
    return fgRet;
}

void MDiskPlayer::DeInit(void)
{
    MRESULT     rt = 0;

    if (m_hMediaGraph)
    {
        rt = MediaGraph_Release(m_hMediaGraph);
        m_hMediaGraph = NULL;
    }

    rt = MSDK_Uninit();
    
    m_eSrcId = MEDIA_SRC_NULL;

    m_fgInited = FALSE;
    
}

void MDiskPlayer::SetSource(int SrcId, int SinkId, BOOL fgShow)
{
    if (!GetSourceFile())
    {
        RETAILMSG(LOG_ON, (_T("[EPlayer][DISK]:no slect media file!\r\n")));
        return;
    }   
    else
    {
        m_eSrcId = SrcId;
        m_eSinkId = SinkId;
        m_fgShow = fgShow;
        PlayMediaFile(m_szFileName);
    }    
}

void MDiskPlayer::SetPlayWindow(HWND hPlayWnd, RECT rcPlayWnd)
{
    MRESULT     mr = S_MSDK_OK;
    MSDK_IID    iid = 0;
    HVIDEOCTRL  hVideoCtrl = NULL;

    ASSERT(m_hMediaGraph != NULL);

    m_hPlayWnd = hPlayWnd;
    memcpy(&m_rcPlayWnd, &rcPlayWnd, sizeof(RECT));   

    //video control
    iid = IID_MSDK_VIDEOCTRL;
    mr = MediaGraph_QueryInterface(m_hMediaGraph, &iid, &hVideoCtrl);
    if (S_MSDK_OK != mr)
    {
        return;
    }

    //set sink information
    if (S_MSDK_OK != VideoCtrl_SetOwner(hVideoCtrl, m_hPlayWnd) ||
        S_MSDK_OK != VideoCtrl_SetWindowRect(hVideoCtrl, (GRECT_T*)&m_rcPlayWnd) ||
        S_MSDK_OK != VideoCtrl_SetDestinationAttr(hVideoCtrl, 
        0,
        0,
        (m_rcPlayWnd.right - m_rcPlayWnd.left),
        (m_rcPlayWnd.bottom - m_rcPlayWnd.top)))
    {
        RETAILMSG(LOG_ON, (_T("[EPlayer][DISK]:set video attr failed!\r\n")));
        return;
    }      

    memcpy(&m_tSinkInfo.DestRect, &rcPlayWnd, sizeof(RECT));        
    MediaGraph_SetSinkInfo(m_hMediaGraph, &m_tSinkInfo);
}

BOOL MDiskPlayer::GetSourceFile(void)
{
    BOOL fgRet = 0;
    OPENFILENAME tOpenFileName = {0};
    TCHAR szFile[MAX_PATH] = {0};

    tOpenFileName.lStructSize = sizeof(OPENFILENAME);
    tOpenFileName.hwndOwner = g_tEntryData.m_hMainWnd;
    tOpenFileName.lpstrFile = szFile;

    tOpenFileName.lpstrFile[0] = _T('\0');  
    tOpenFileName.nMaxFile = sizeof(szFile);  
    tOpenFileName.lpstrFilter = g_szFilter;  
    tOpenFileName.nFilterIndex = 1;  
    tOpenFileName.lpstrFileTitle = NULL;  
    tOpenFileName.nMaxFileTitle = 0;  
    tOpenFileName.lpstrInitialDir = NULL;  
    tOpenFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;  

    // Open file select dialog   
    if ( GetOpenFileName(&tOpenFileName) )  
    {  
         _tcsncpy(m_szFileName, szFile, MAX_PATH);
         fgRet = TRUE;
    }  

    return fgRet;
}

BOOL MDiskPlayer::PlayMediaFile(TCHAR *szMediaFile)
{
    BOOL fgRet = 0;
    MRESULT       mr = S_MSDK_OK;

    ASSERT(m_hMediaGraph != NULL);

    if (NULL == szMediaFile)
    {
        RETAILMSG(LOG_ON, (_T("[EPlayer][DISK]:not select media file\r\n")));
        return FALSE;
    }

    mr = MediaGraph_SetSourceFile(m_hMediaGraph, szMediaFile);

    fgRet = ((mr == S_MSDK_OK) ? TRUE : FALSE);

    return fgRet;
}

BOOL MDiskPlayer::SetNotifyFreq(UINT32 u4Freq)
{
    BOOL fgRet = 0;
    MRESULT       mr = S_MSDK_OK;
    MG_CONFIG_T tMGConfig = {0};

    ASSERT(m_hMediaGraph != NULL);

    mr = MediaGraph_GetConfig(m_hMediaGraph, &tMGConfig);
    if (S_MSDK_OK == mr)
    {
        tMGConfig.u4MediaPosNotifyFreq = u4Freq;
        mr = MediaGraph_SetConfig(m_hMediaGraph, &tMGConfig);
    }

    fgRet = ((mr == S_MSDK_OK) ? TRUE : FALSE);

    return fgRet;
}

BOOL MDiskPlayer::PrePlay(void)
{
    BOOL fgRet = 0;
    MRESULT       mr = S_MSDK_OK;
    MSDK_IID    iid = 0;
    HVIDEOCTRL  hVideoCtrl = NULL;

    ASSERT(m_hMediaGraph != NULL);

    
    SetPlayWindow(m_hPlayWnd, m_rcPlayWnd);

    switch (m_eSinkId)
    {
    case MEDIA_SINK_FORNT:
        mr = MediaGraph_SetDataSink(m_hMediaGraph, MSDK_SINK_FRONT);

        m_tSinkInfo.eSink = MSDK_SINK_FRONT;
        memcpy(&m_tSinkInfo.DestRect, &m_rcPlayWnd, sizeof(RECT));
        if (!m_fgShow)
        {
            m_tSinkInfo.u4Flag = MSDK_SWI_INFO_HIDE;
        }
        else
        {
            m_tSinkInfo.u4Flag = MSDK_SWI_INFO_SHOW;
        }

        m_tSinkInfo.ovfx.dckSrcColorkey.dwColorSpaceHighValue = 0;
        m_tSinkInfo.ovfx.dckSrcColorkey.dwColorSpaceLowValue = 0;
        mr = MediaGraph_SetSinkInfo(m_hMediaGraph, &m_tSinkInfo);
        break;

    case MEDIA_SINK_REAR:
        mr = MediaGraph_SetDataSink(m_hMediaGraph, MSDK_SINK_REAR);

        m_tSinkInfo.eSink = MSDK_SINK_REAR;
        SetRect(&m_tSinkInfo.DestRect, 0, 0, MAX_WINDOW_W_R, MAX_WINDOW_H_R);
        m_tSinkInfo.u4Flag = MSDK_SWI_INFO_SHOW;
        mr = MediaGraph_SetSinkInfo(m_hMediaGraph, &m_tSinkInfo);
        break;

    case MEDIA_SINK_FORNT_AND_REAR:
        mr = MediaGraph_SetDataSink(m_hMediaGraph, MSDK_SINK_FRONT_REAR);

        //FRONT SINK
        m_tSinkInfo.eSink = MSDK_SINK_FRONT_REAR;
        memcpy(&m_tSinkInfo.DestRect, &m_rcPlayWnd, sizeof(RECT));
        if (!m_fgShow)
        {
            m_tSinkInfo.u4Flag = MSDK_SWI_INFO_HIDE;
        }
        else
        {
            m_tSinkInfo.u4Flag = MSDK_SWI_INFO_SHOW;
        }
        m_tSinkInfo.ovfx.dckSrcColorkey.dwColorSpaceHighValue = 0;
        m_tSinkInfo.ovfx.dckSrcColorkey.dwColorSpaceLowValue = 0;
        mr = MediaGraph_SetSinkInfo(m_hMediaGraph, &m_tSinkInfo);

        //REAR SINK
        SetRect(&m_tSinkInfo.DestRect, 0, 0, MAX_WINDOW_W_R, MAX_WINDOW_H_R);
        m_tSinkInfo.u4Flag = MSDK_SWI_INFO_SHOW;
        mr = MediaGraph_SetSinkInfo(m_hMediaGraph, &m_tSinkInfo);
        break;
    }

    if (S_MSDK_OK == mr)
    {
        SetNotifyFreq(2);
    }
    

    fgRet = ((mr == S_MSDK_OK) ? TRUE : FALSE);

EXIT:
    return fgRet;
}

void MDiskPlayer::Play(void)
{
    MRESULT       mr = S_MSDK_OK;
    E_MSDK_MGSTATE  eState;

    ASSERT(m_hMediaGraph != NULL);

    mr = MediaGraph_GetState(m_hMediaGraph, &eState);
    if (S_MSDK_OK == mr)
    {
        if (MSDK_MGSTATE_PLAYING != eState) 
        {
            mr = MediaCtrl_Play(m_hMediaGraph);
        }        
    }    
}

void MDiskPlayer::Pause(void)
{
    MRESULT       mr = S_MSDK_OK;
    E_MSDK_MGSTATE  eState;

    ASSERT(m_hMediaGraph != NULL);

    mr = MediaGraph_GetState(m_hMediaGraph, &eState);
    if (S_MSDK_OK == mr)
    {
        if (MSDK_MGSTATE_PLAYING == eState) 
        {
            mr = MediaCtrl_Pause(m_hMediaGraph);
        }        
    }    
}

void MDiskPlayer::Stop(void)
{
    MRESULT       mr = S_MSDK_OK;
    E_MSDK_MGSTATE  eState;

    ASSERT(m_hMediaGraph != NULL);

    mr = MediaGraph_GetState(m_hMediaGraph, &eState);
    if (S_MSDK_OK == mr)
    {
        if (MSDK_MGSTATE_STOP != eState)
        {
            mr = MediaCtrl_Stop(m_hMediaGraph);
        }
    }
}

void MDiskPlayer::HandlePlaybackPostionChange(UINT32 u4Pos)
{
    MRESULT       mr = S_MSDK_OK;
    UINT64        u8Duration = 0;

    ASSERT(m_hMediaGraph != NULL);

    mr = MediaCtrl_GetDuration(m_hMediaGraph, &u8Duration);  
    if (S_MSDK_OK != mr)
    {
        return;
    }

    //notify media player to refresh progress bar
    MSG_PARAM msgParam;
    DWORD dwOffset = 0;
    DWORD dwLen = 0;

    dwLen = sizeof(DWORD);
    memcpy(msgParam.lpData, &u4Pos, dwLen);
    dwOffset += dwLen;
    dwLen = sizeof(UINT64);
    memcpy((LPBYTE)msgParam.lpData + dwOffset, &u8Duration, dwLen);
    dwOffset += dwLen;
    msgParam.dwSize = dwOffset;    
    
    ::SendMessage(g_tEntryData.m_hMainWnd, MEDIA_PLAYER_MSG, (WPARAM)PLAYER_PLAYBACK_POSITION, (LPARAM)&msgParam);
}