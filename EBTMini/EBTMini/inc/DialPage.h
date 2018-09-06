#ifndef __DIALPAGE_H__
#define __DIALPAGE_H__
#include "page.h"

class CDialPage : public CPage
{
public:
    CDialPage();
    virtual ~CDialPage();

    BOOL Init(HWND hwndParent);
    void DeInit(void);  

    BOOL Call(void);
    BOOL End(void);
    BOOL AudioSwitch(void);   

    virtual void DoInitDialog(WPARAM wParam, LPARAM lParam);
    virtual void DoTimerEvent(WPARAM wParam, LPARAM lParam);
    virtual void DoResizeWindow(WPARAM wParam, LPARAM lParam);
    virtual void RegisterCallBack(void);

public:    
    static void CALLBACK OnBTCall(DWORD dwIndID, WPARAM wParam, LPARAM lParam);

protected:

friend BOOL DialDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

private:    
   
};

#endif //__DIALPAGE_H__