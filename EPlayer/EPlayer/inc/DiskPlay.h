#ifndef __DISKPLAY_H__
#define __DISKPLAY_H__
#include "msdk_defs.h"
#include "ecommon.h"
#include "BasicPlayer.h"

class MDiskPlayer : public BasicPlayer
{
public:
    MDiskPlayer(void);
    ~MDiskPlayer(void);
   
    BOOL Init(void);
    void DeInit(void);

    void SetSource(int SrcId, int SinkId = MEDIA_SINK_FORNT, BOOL fgShow = TRUE); 
    void SetPlayWindow(HWND hPlayWnd, RECT rcPlayWnd);

    void Play(void);
    void Pause(void);
    void Stop(void); 

    void HandleMediaEvent(UINT32 u4Event, UINT32 u4Param1, UINT32 u4Param2);

protected:
    BOOL GetSourceFile(void);    
    BOOL PlayMediaFile(TCHAR *szMediaFile);    
    BOOL SetNotifyFreq(UINT32 u4Freq);

    BOOL PrePlay(void);

    void HandlePlaybackPostionChange(UINT32 u4Pos);
         

friend MRESULT MediaEvent_Listener(HMEDIAGRAPH hMediaGraph, 
        UINT32 u4Evt, 
        UINT32 u4Param1, 
        UINT32 u4Param2, 
        UINT32 u4CustomData);
         
private:
    HMEDIAGRAPH     m_hMediaGraph;
    MSDK_SINK_INFO_T m_tSinkInfo;
   
    TCHAR m_szFileName[MAX_PATH];   

};

#endif//__DISKPLAY_H__