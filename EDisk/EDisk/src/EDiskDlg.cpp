// EDiskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EDisk.h"
#include "EDiskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CEDiskDlg dialog

CEDiskDlg::CEDiskDlg(LPCTSTR pszCaption, CWnd* pParent /*=NULL*/)
	: CPropertySheet(pszCaption, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	AddPage(&m_cEFat);
	AddPage(&m_cEFile);
}

void CEDiskDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertySheet::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEDiskDlg, CPropertySheet)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()



// CEDiskDlg message handlers

BOOL CEDiskDlg::OnInitDialog()
{
	CPropertySheet::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CEDiskDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_EDisk_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_EDisk_DIALOG));
	}
}
#endif


