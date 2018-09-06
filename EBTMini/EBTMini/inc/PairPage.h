#ifndef __PAIRPAGE_H__
#define __PAIRPAGE_H__
#include "page.h"


#define BT_RECONNECT_TIME           3      //重新连接尝试次数
#define BT_RECONNECT_INTERVAL       2000    //重新连接毫秒间隔 
#define BT_DISCOVER_TIME            (2 * 60 * 1000)//2 MIN

#define BT_MAX_LEN                  64

typedef struct
{
    TCHAR szBTName[BT_MAX_LEN];
    TCHAR szBTID[BT_MAX_LEN];
    TCHAR szBTProfile[BT_MAX_LEN];
}BT_RECORD;


class CPairPage : public CPage
{
public:
    CPairPage();
    virtual ~CPairPage();

    virtual BOOL Init(HWND hwndParent);
    virtual void DeInit();   

    virtual void Activate(); 
    virtual void DeActivate();   

    virtual void DoInitDialog(WPARAM wParam, LPARAM lParam);
    virtual void DoTimerEvent(WPARAM wParam, LPARAM lParam);
    virtual void DoResizeWindow(WPARAM wParam, LPARAM lParam);
    virtual void DoCommand(WPARAM wParam, LPARAM lParam);

    virtual void RegisterCallBack(void);

    void RefreshUI(UINT32 u4Para1, UINT32 u4Para2 = 0);

public:    
    static void CALLBACK OnBTPair(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
    

    static void CALLBACK OnBTCallConnect(DWORD dwIndID, WPARAM wParam, LPARAM lParam);

    static void CALLBACK OnBTSearchMgr(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
    static void CALLBACK OnBTSearchSwitch(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
    static void CALLBACK OnBTSearchConnect(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
    static void CALLBACK OnBTSearchHeadSet(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
    static void CALLBACK OnBTSearchPair(DWORD dwIndID, WPARAM wParam, LPARAM lParam);

protected:
    BOOL InitListViewColumns();

    void InitLocalDevice(void);
    void SearchRemoteDevice(void);

    void Insert2List(BT_RECORD btRecord);
    int GetSelectedItemIndex(void);

friend BOOL PairDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


private:    
    DWORD m_dwDiscoverTime;
    UINT32 m_u4BTIndex;
    UINT32 m_u4SearchRecord;

};

#endif //__PAIRPAGE_H__