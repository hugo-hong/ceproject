/************************************************************************
page.h: head file of the CPage class.
************************************************************************/
#ifndef _PAGE_H_
#define _PAGE_H_

#include <Windows.h>
#include <CommCtrl.h>
#include <vector>
#include "eutil.h"

using namespace std;

#define DEFAULT_SPACE                   5

class CPage
{
public:
    CPage(void);
    ~CPage(void);

public:
    // Sent when page is being created

    virtual BOOL     Init(HWND hwndParent)                      PURE;

    //// Send when page is being shut down

    virtual void     DeInit()                                   PURE;

    //// Sent when page is being displayed

    virtual void     Activate();                                //PURE;

    //// Sent when page is being hidden

    virtual void     DeActivate();                               // PURE;    

    //// Refresh page UI

    virtual void   RefreshUI(UINT32 u4Para1, UINT32 u4Para2 = 0);    

   
    virtual void    DoTimerEvent(WPARAM wParam, LPARAM lParam)  PURE;

    virtual void    DoInitDialog(WPARAM wParam, LPARAM lParam)  PURE;

    virtual void    DoResizeWindow(WPARAM wParam, LPARAM lParam) PURE;

    virtual void    DoCommand(WPARAM wParam, LPARAM lParam);

    virtual void    RegisterCallBack(void)                       PURE; 

public:
    HWND m_hWnd;

};


#endif//_PAGE_H_