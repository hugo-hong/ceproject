#ifndef __BASICPLAYER_H__
#define __BASICPLAYER_H__
#include "ecommon.h"
#include "msdk_defs.h"

enum PLAYER_STATE
{
    EP_UNKNOW,
    EP_PLAY,
    EP_PAUSE,
    EP_STOP,
    EP_IDLE

};

class BasicPlayer
{
public:
    BasicPlayer(void);
    virtual ~BasicPlayer(void);

    virtual BOOL Init(void) = 0;
    virtual void DeInit(void) = 0;

    virtual void SetSource(int SrcId, int SinkId = MEDIA_SINK_FORNT, BOOL fgShow = TRUE) = 0; 
    virtual void SetPlayWindow(HWND hPlayWnd, RECT rcPlayWnd) = 0;

    virtual void Play(void) = 0;
    virtual void Pause(void) = 0;
    virtual void Stop(void) = 0;   

    virtual void HandleMediaEvent(UINT32 u4Event, UINT32 u4Param1, UINT32 u4Param2);

protected:
   
public:
    int m_eSrcId;
    int m_eSinkId;
    BOOL m_fgShow;
    BOOL m_fgInited;
    int m_eStatus;

    HWND m_hPlayWnd;
    RECT m_rcPlayWnd;
     
};

#endif//__BASICPLAYER_H__