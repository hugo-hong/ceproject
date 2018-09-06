/************************************************************************
page.h: head file of the CPage class.
************************************************************************/
#ifndef _PAGE_H_
#define _PAGE_H_

#include <Windows.h>
#include <CommCtrl.h>
#include <vector>
#include "eutil.h"

#define MM_WAV_NOTIFY_PROCESS_ID    (WM_USER + 0x61)     // #define WM_USER  0x400
#define MM_WAV_AUDIO_PID            (WM_APP + 0x11)

using namespace std;

#define DEFAULT_SPACE                   5

class CPage
{
public:
    CPage(void);
    ~CPage(void);

public:
    // Sent when page is being created

    virtual BOOL     Init()                                     PURE;

    //// Sent when page is being displayed

    //virtual BOOL     Activate()                                  PURE;

    //// Sent when page is being hidden

    //virtual void        Deactivate()                                PURE;

    //// Send when page is being shut down

    //virtual BOOL     Destroy()                                   PURE;

    //// Returns the title of the page for use on the tab control

    //virtual void        GetTitle(LPTSTR pszText, size_t bufsize)    PURE;

    //// Returns the handle to the page's main dialog

    //virtual HWND        GetPageWindow()                             PURE;

    //// Sent when a timer event (update display) occurs

    virtual BOOL    DoInitDialog(WPARAM wParam, LPARAM lParam)  PURE;

    virtual void    DoTimerEvent(WPARAM wParam, LPARAM lParam)  PURE;

    virtual void    DoResizeWindow(WPARAM wParam, LPARAM lParam) PURE;

public:
    HWND m_hDlgWnd;

};


#endif//_PAGE_H_