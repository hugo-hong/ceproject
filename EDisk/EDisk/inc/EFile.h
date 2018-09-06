#pragma once
#include "fatfs.h"

#define MAX_FOLED_DISK_NUM					8
#define MAX_DISK_NAME_LEN						32

typedef struct
{
	TCHAR szFolder[MAX_DISK_NAME_LEN];
	TCHAR szPartition[MAX_DISK_NAME_LEN];
	TCHAR szDisk[MAX_DISK_NAME_LEN];
}FOLDER_DISK, *PFOLDER_DISK;

// CEFile dialog

class CEFile : public CPropertyPage
{
	DECLARE_DYNAMIC(CEFile)

public:
	CEFile();
	virtual ~CEFile();

// Dialog Data
	enum { IDD = IDD_EFILE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CString m_strFileData;
	CString m_strFilePath;

	UINT	m_nIndex;
	FOLDER_DISK m_fd[MAX_FOLED_DISK_NUM];

	HANDLE m_hPart;
	CEFatfs m_fatfs;

protected:
	BOOL EnumDiskInfo(void);
	BOOL EnumPartInfo(LPCTSTR szDisk);	
	BOOL ReadFileData(LPCTSTR szFile);
	BOOL InitPart(LPCTSTR szPath);


	UINT FindDisk(LPCTSTR szFolder);
	BOOL Read(HANDLE hFile);

	void PrintFileData(PBYTE pData, DWORD dwLen);

public:
	afx_msg void OnBnClickedFileSelect();

	virtual BOOL OnInitDialog();
};
