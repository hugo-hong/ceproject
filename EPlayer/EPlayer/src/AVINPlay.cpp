#include "stdafx.h"
#include "ecommon.h"
#include "AVINPlay.h"
#include "MSDK_Global.h"
#include "msdk_error.h"
#include "MSDK_MediaGraph.h "
#include "MSDK_MediaCtrl.h"

MAVINPlayer::MAVINPlayer(void)
{    
    m_hMediaGraphA = NULL;
    m_hMediaGraphV = NULL;
    m_fgInited = FALSE;
    m_eSrcId = MEDIA_SRC_NULL;
    memset(&m_tSinkInfo, 0, sizeof(MSDK_AVIN_SINK_INFO_T));
}

MAVINPlayer::~MAVINPlayer(void)
{
    Stop();
    DeInit();
}

BOOL MAVINPlayer::Init(void)
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
        RETAILMSG(LOG_ON, (_T("[EPlayer][AVIN]: MSDK_Init error\r\n")));
        return FALSE;
    }

    rt = MediaGraph_Create(&m_hMediaGraphA);
    if (S_MSDK_OK != rt)
    {
        RETAILMSG(LOG_ON, (_T("[EPlayer][AVIN]:create A media graph error\r\n")));
        goto EXIT;
    }

    rt = MediaGraph_Create(&m_hMediaGraphV);
    if (S_MSDK_OK != rt)
    {
        RETAILMSG(LOG_ON, (_T("[EPlayer][AVIN]:create V media graph error\r\n")));
        goto EXIT;
    }

    fgRet = TRUE;
    m_fgInited = TRUE;

EXIT:
    if (!fgRet)
    {
        DeInit();
    }
    return fgRet;
}

void MAVINPlayer::DeInit(void)
{
    if (m_hMediaGraphA)
    {
        MediaGraph_Release(m_hMediaGraphA);
        m_hMediaGraphA = NULL;
    }

    if (m_hMediaGraphV)
    {
        MediaGraph_Release(m_hMediaGraphV);
        m_hMediaGraphV = NULL;
    }

    MSDK_Uninit();

    m_eSrcId = MEDIA_SRC_NULL;

    m_fgInited = FALSE;
}

void MAVINPlayer::Play(void)
{
    if (m_hMediaGraphA != NULL)
    {
        MediaCtrl_PlayAvin(m_hMediaGraphA);
    }

    if (m_hMediaGraphV != NULL)
    {
       MediaCtrl_PlayAvin(m_hMediaGraphV); 
    }    
}

void MAVINPlayer::Pause(void)
{
    Stop();
}

void MAVINPlayer::Stop(void)
{
    if (m_hMediaGraphA != NULL)
    {
        MediaCtrl_StopAvin(m_hMediaGraphA);
    }

    if (m_hMediaGraphV != NULL)
    {
        MediaCtrl_StopAvin(m_hMediaGraphV);
    }   
}

void MAVINPlayer::SetSource(int SrcId, int SinkId, BOOL fgShow)
{
    ASSERT(m_hMediaGraphA != NULL);
    ASSERT(m_hMediaGraphV != NULL);

    if (m_eSrcId != NULL)
    {
        //stop last source
        Stop();
    }

    //set sink infomation
    m_eSinkId = SinkId;
    switch (SinkId)
    {
    case MEDIA_SINK_FORNT:
        m_tSinkInfo.eSink = MSDK_SINK_FRONT;
        SetRect(&(m_tSinkInfo.rFrontDestRect), 0, 0, 800, 480);

        if(fgShow == TRUE)
        {
            m_tSinkInfo.u4FrontFlag = DDOVER_SHOW | DDOVER_KEYSRCOVERRIDE;
        }
        else if(fgShow == FALSE)
        {
            m_tSinkInfo.u4FrontFlag = DDOVER_HIDE;
        }
        m_tSinkInfo.rFrontOvfx.dckSrcColorkey.dwColorSpaceHighValue = 0;
        m_tSinkInfo.rFrontOvfx.dckSrcColorkey.dwColorSpaceLowValue = 0;
        break;

    case MEDIA_SINK_REAR:
        m_tSinkInfo.eSink = MSDK_SINK_REAR;
        m_tSinkInfo.eMode = MSDK_BYPASS;
        SetRect(&(m_tSinkInfo.rRearDestRect), 0, 0, 720, 480);

        m_tSinkInfo.u4RearFlag = DDOVER_SHOW | DDOVER_KEYSRCOVERRIDE;
        m_tSinkInfo.rRearOvfx.dckSrcColorkey.dwColorSpaceHighValue = 0;
        m_tSinkInfo.rRearOvfx.dckSrcColorkey.dwColorSpaceLowValue = 0;
        break;

    case MEDIA_SINK_FORNT_AND_REAR:
        m_tSinkInfo.eSink = MSDK_SINK_FRONT_REAR;
        m_tSinkInfo.eMode = MSDK_BYPASS;

        SetRect(&(m_tSinkInfo.rFrontDestRect), 0, 0, 800, 480);
        if(fgShow == TRUE)
        {
            m_tSinkInfo.u4FrontFlag = DDOVER_SHOW | DDOVER_KEYSRCOVERRIDE;
        }
        else if(fgShow == FALSE)
        {
            m_tSinkInfo.u4FrontFlag = DDOVER_HIDE;
        }
        m_tSinkInfo.rFrontOvfx.dckSrcColorkey.dwColorSpaceHighValue = 0;
        m_tSinkInfo.rFrontOvfx.dckSrcColorkey.dwColorSpaceLowValue = 0;

        SetRect(&(m_tSinkInfo.rRearDestRect), 0, 0, 720, 480);
        m_tSinkInfo.u4RearFlag = DDOVER_SHOW | DDOVER_KEYSRCOVERRIDE;
        m_tSinkInfo.rRearOvfx.dckSrcColorkey.dwColorSpaceHighValue = 0;
        m_tSinkInfo.rRearOvfx.dckSrcColorkey.dwColorSpaceLowValue = 0;
        break;
    }    

    //set source information
    m_eSrcId = SrcId;
    switch (SrcId)
    {
    case MEDIA_SRC_AVIN1:
        MediaGraph_SetVideoInSource(m_hMediaGraphV, MSDK_AVIN_V1, &m_tSinkInfo);
        MediaGraph_SetAudioInSource(m_hMediaGraphA, MSDK_AVIN_A0, &m_tSinkInfo);
        break;

    case MEDIA_SRC_AVIN2:
        MediaGraph_SetVideoInSource(m_hMediaGraphV, MSDK_AVIN_V2, &m_tSinkInfo);
        MediaGraph_SetAudioInSource(m_hMediaGraphA, MSDK_AVIN_A1, &m_tSinkInfo);
        break;

    case MEDIA_SRC_AVIN3:
        MediaGraph_SetVideoInSource(m_hMediaGraphV, MSDK_AVIN_V3, &m_tSinkInfo);
        MediaGraph_SetAudioInSource(m_hMediaGraphA, MSDK_AVIN_A2, &m_tSinkInfo);
        break;

    case MEDIA_SRC_AVIN4:
        MediaGraph_SetVideoInSource(m_hMediaGraphV, MSDK_AVIN_V4, &m_tSinkInfo);
        MediaGraph_SetAudioInSource(m_hMediaGraphA, MSDK_AVIN_A3, &m_tSinkInfo);
        break;

    case MEDIA_SRC_AVIN5:
        MediaGraph_SetVideoInSource(m_hMediaGraphV, MSDK_AVIN_V0, &m_tSinkInfo);
        MediaGraph_SetAudioInSource(m_hMediaGraphA, MSDK_AVIN_A4, &m_tSinkInfo);
        break;

    case MEDIA_SRC_NULL:
        MediaGraph_SetVideoInSource(m_hMediaGraphV, MSDK_AVIN_VNONE, &m_tSinkInfo);
        MediaGraph_SetAudioInSource(m_hMediaGraphA, MSDK_AVIN_ANONE, &m_tSinkInfo);
        break; 
    }   

#if 0 //do scale test

    MSDK_SCALE_INFO_T tScaleInfo;

    tScaleInfo.eSinkType = MSDK_SINK_FRONT;
    tScaleInfo.fgAbortScale = 0;
    tScaleInfo.rRect.left = 15;
    tScaleInfo.rRect.right = 720;
    tScaleInfo.rRect.top = 0;
    tScaleInfo.rRect.bottom = 576;

    MediaGraph_SetScale(m_hMediaGraphV, &tScaleInfo);

#endif

}

void MAVINPlayer::SetPlayWindow(HWND hPlayWnd, RECT rcPlayWnd)
{
    ASSERT(m_hMediaGraphV != NULL);

    m_hPlayWnd = hPlayWnd;

    switch (m_eSinkId)
    {
    case MEDIA_SINK_FORNT:
        m_tSinkInfo.eSink = MSDK_SINK_FRONT;
        memcpy(&(m_tSinkInfo.rFrontDestRect), &rcPlayWnd, sizeof(RECT));        
        break;

    case MEDIA_SINK_REAR:
        m_tSinkInfo.eSink = MSDK_SINK_REAR;
        memcpy(&(m_tSinkInfo.rRearDestRect), &rcPlayWnd, sizeof(RECT)); 
        break;

    case MEDIA_SINK_FORNT_AND_REAR:
        m_tSinkInfo.eSink = MSDK_SINK_FRONT_REAR;
        memcpy(&(m_tSinkInfo.rFrontDestRect), &rcPlayWnd, sizeof(RECT)); 
        memcpy(&(m_tSinkInfo.rRearDestRect), &rcPlayWnd, sizeof(RECT)); 
        break;
    }

    MediaGraph_SetVideoInInfo(m_hMediaGraphV, &m_tSinkInfo);
}