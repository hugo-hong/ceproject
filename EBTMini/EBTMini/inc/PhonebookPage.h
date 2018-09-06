#ifndef __PHONEBOOKPAGE_H__
#define __PHONEBOOKPAGE_H__
#include "page.h"

class CPoneBookPage : public CPage
{
public:
    CPoneBookPage();
    virtual ~CPoneBookPage();

    BOOL Init(HWND hwndParent);
    void DeInit();   

    void Sync();
    void Call();


    virtual void DoInitDialog(WPARAM wParam, LPARAM lParam);
    virtual void DoTimerEvent(WPARAM wParam, LPARAM lParam);
    virtual void DoResizeWindow(WPARAM wParam, LPARAM lParam);
    virtual void RegisterCallBack(void);

public:
    static void CALLBACK OnBTPBMgr(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
    static void CALLBACK OnBTPBSwitch(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
    static void CALLBACK OnBTPBConnect(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
    

protected:
    BOOL InitListViewColumns();

friend BOOL PhoneBookDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


private:    

    int m_iIndex;

};

#endif //__PHONEBOOKPAGE_H__