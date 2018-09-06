#ifndef __MUSICPAGE_H__
#define __MUSICPAGE_H__
#include "page.h"

class CMusicPage : public CPage
{
public:
    CMusicPage();
    virtual ~CMusicPage();

    BOOL Init(HWND hwndParent);
    void DeInit(void);   
    void Play(void);
    void Pause(void);
    void Stop(void);   
    void PlayPrev(void);
    void PlayNext(void);

    virtual void DoInitDialog(WPARAM wParam, LPARAM lParam);
    virtual void DoTimerEvent(WPARAM wParam, LPARAM lParam);
    virtual void DoResizeWindow(WPARAM wParam, LPARAM lParam);
    virtual void RegisterCallBack(void);

public:    
    static void CALLBACK OnBTMusicConnect(DWORD dwIndID, WPARAM wParam, LPARAM lParam);

protected:

    friend BOOL MusicDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

private:    

};

#endif //__MUSICPAGE_H__