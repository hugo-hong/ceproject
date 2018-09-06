#pragma once
#include "resource.h"
#include "page.h"

#define IDC_STATUSWND   100

enum _E_TabMode
{
    TAB_APP,
    TAB_PROCESS,
    TAB_PERFORMANCE,
    TAB_MAX
};

typedef struct _TaskParam
{
    HINSTANCE m_hInstance;
    HWND m_hMainDlg;
    HWND m_hStatusWnd;
    HWND m_hTabWnd;
    UINT m_uTabIndex;

    DWORD m_dwInterval;

    CPage *m_pPage[TAB_MAX];

    SYSTEM_INFO m_sysInfo;

}TASK_PARAM, *PTASK_PARAM;