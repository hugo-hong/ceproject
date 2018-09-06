#pragma once
#include "resource.h"
#include "ecommon.h"
#include "BasicPlayer.h"
#include "AVINPlay.h"
#include "DiskPlay.h"

#define DEFAULT_SPACE       5

class MediaPlayer
{
public:
    MediaPlayer(void);
    ~MediaPlayer(void);

    //UI Control
    void DoInitDialog(WPARAM wParam, LPARAM lParam);
    void DoResizeDialog(WPARAM wParam, LPARAM lParam); 

    void SetSource(UINT SourceId);
    void ResetPlayWindow(void);
    
    void Entry(void);
    void Exit(void);  
    void Play(void);
    void Pause(void);
    void Stop(void);    

    void RefreshProgressBar(LPVOID lpParam);

    void HandleMediaEvent(UINT32 u4Event, UINT32 u4Param1, UINT32 u4Param2);

public:
    static MediaPlayer * GetInstance()
    {
        return &m_Instance;
    }

    BOOL IsPlaying(void)
    {
        return m_fgPlay;
    }    

    void SetPlayStatus(BOOL fgPlay)
    {
        m_fgPlay = fgPlay;
    }

protected:
    

    BasicPlayer *m_pPlayer;

private:
    static MediaPlayer m_Instance;    
    volatile BOOL m_fgPlay;

    HWND m_hWnd;
    int m_eMediaSrc;
     
};