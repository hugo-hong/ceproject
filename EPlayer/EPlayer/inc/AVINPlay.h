#ifndef __AVINPLAY_H__
#define __AVINPLAY_H__
#include "msdk_defs.h"
#include "ecommon.h"
#include "AVINPlay.h"
#include "DiskPlay.h"

class MAVINPlayer : public BasicPlayer
{
public:
    MAVINPlayer(void);
    ~MAVINPlayer(void);

    BOOL Init(void);
    void DeInit(void);

    void Play(void);
    void Pause(void);
    void Stop(void); 

    void SetSource(int SrcId, int SinkId = MEDIA_SINK_FORNT, BOOL fgShow = TRUE);
    void SetPlayWindow(HWND hPlayWnd, RECT rcSize);

protected:
private:
    HMEDIAGRAPH     m_hMediaGraphA;
    HMEDIAGRAPH     m_hMediaGraphV;
    MSDK_AVIN_SINK_INFO_T m_tSinkInfo;
   
       
};


#endif //__AVINPLAY_H__