// EDiskDlg.h : header file
//

#pragma once

#include "EFat.h"
#include "EFile.h"


// CEDiskDlg dialog
class CEDiskDlg : public CPropertySheet
{
// Construction
public:
	CEDiskDlg(LPCTSTR pszCaption, CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EDisk_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
	
protected:
	CEFat m_cEFat;
	CEFile m_cEFile;

};
