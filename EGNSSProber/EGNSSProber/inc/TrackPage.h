/************************************************************************
page.h: head file of the CPage class.
************************************************************************/
#ifndef __TRACKPAGE_H__
#define __TRACKPAGE_H__

#include "page.h"
#include "gnssparser.h"

enum GNSS_STATUS
{
    GNSS_IDLE,
    GNSS_ACTIVE,
    GNSS_DEACTIVE,    
};

using namespace std;

class CTrackPage : public CPage
{
public:
    CTrackPage(void);
    ~CTrackPage(void);


    BOOL Init(HWND hwndParent);
    void DeInit(void);

    virtual void DoInitDialog(WPARAM wParam, LPARAM lParam);
    virtual void DoTimerEvent(WPARAM wParam, LPARAM lParam);
    virtual void DoResizeWindow(WPARAM wParam, LPARAM lParam);
    virtual void DoCommand(WPARAM wParam, LPARAM lParam);

    virtual void Activate(void); 
    virtual void DeActivate(void); 
    virtual void RefreshUI(UINT32 u4Para1 = 0, UINT32 u4Para2 = 0);



protected:

    void GetPositionData(GNSS_POS_DATA *pGPSPosition, DWORD dwMaximumAge, DWORD dwFlags);
    
    void RefreshSateliteView(void);
    void RefreshPositionInfo(void);

    friend DWORD GNSSDataInt(LPVOID lpPara);
    friend BOOL TrackDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


private:
    int m_eGNSSStatus;
    HANDLE m_hGNSSThread;
    DWORD m_dwGNSSThreadId;
    CGNSSParser m_gnssParser;
    GNSS_POS_DATA m_gnssPosData;
    
    DWORD m_dwFixTime; //ms
};


#endif//__TRACKPAGE_H__