/************************************************************************
progrecces.h: head file of the CProcesses page class.
************************************************************************/
#ifndef _PROCESSES_H_
#define _PROCESSES_H_
#include "page.h"

typedef struct  
{
    DWORD dwCommitSize;
    DWORD dwReservSize;
    DWORD dwFreeSize;
}MEM_STATUS_T, *PMEM_STATUS_T;

class CProcesses : public CPage
{
public:
    CProcesses();
    virtual ~CProcesses();

    BOOL Init();
    BOOL DeInit();   

    virtual BOOL DoInitDialog(WPARAM wParam, LPARAM lParam);
    virtual void DoTimerEvent(WPARAM wParam, LPARAM lParam);
    virtual void DoResizeWindow(WPARAM wParam, LPARAM lParam);

protected:
    BOOL InitListViewColumns();
    BOOL DrawProcessView();
    BOOL InsertItem(LPVOID lpParam);
    BOOL UpdateItems(void);
    LPARAM GetSelectedItemLParam(void);

    BOOL KillSelectedProcess(void);
    DWORD GetProcessHeapSize(DWORD dwProcessId);
    BOOL GetProcessMemoryStatus(LPVOID lpParam);


    friend BOOL ProcessDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

private:
    
    HWND m_hListViewWnd;
    vector<DWORD>    m_PIDList;
    MEM_STATUS_T m_memStatus;
};

#endif//_PROCESSES_H_