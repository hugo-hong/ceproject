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


class CPage
{
public:
    CPage(void);
    virtual ~CPage(void);

public:
    // Sent when page is being created
    virtual BOOL Init(HWND hwndParent)  PURE;

    // Send when page is being shut down
    virtual void DeInit(void)   PURE;

    // Sent when page is being displayed
    virtual void Activate(void);                                

    // Sent when page is being hidden
    virtual void DeActivate(void);                                 

    // Refresh page UI
    virtual void RefreshUI(UINT32 u4Para1, UINT32 u4Para2 = 0);    

   
    //WM_INITDIALOG
    virtual void DoInitDialog(WPARAM wParam, LPARAM lParam);

    //WM_TIMER
    virtual void DoTimerEvent(WPARAM wParam, LPARAM lParam);    

    //WM_SIZE
    virtual void DoResizeWindow(WPARAM wParam, LPARAM lParam);

    //WM_COMMAND
    virtual void DoCommand(WPARAM wParam, LPARAM lParam);

    //WM_NOTIFY
    virtual void DoNotify(WPARAM wParam, LPARAM lParam);    

public:
    HWND m_hWnd;

};


#endif//_PAGE_H_