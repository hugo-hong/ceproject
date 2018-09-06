#pragma once
#include "storemgr.h"
#include "disk.h"
#include "fatfs.h"
#include "eutil.h"

typedef void (*PFN_FORMATVOLUMEUI)(HANDLE hVolume, HWND hDlg);

// CEFat dialog

typedef enum
{
	BINFS,
	FAT,
	EXFAT,
	UNKNOW_FS
}FILE_SYS;

struct DSKSZTOSECPERCLUS {
	DWORD   DiskSize;
	BYTE    SecPerClusVal;
};

/*
* This is the table for exFAT volumes
*/
static DSKSZTOSECPERCLUS DskTableExfat [] = {
	{ 532480,    1},       /* disks up to 260 MB,  .5k cluster */
	{ 16777216,  8},       /* disks up to     8 GB,    4k cluster */
	{ 33554432, 16},       /* disks up to   16 GB,    8k cluster */
	{ 67108864, 32},       /* disks up to   32 GB,  16k cluster */
	{ 0xFFFFFFFF, 64}      /* disks greater than 32GB, 32k cluster */
};

//////////////////////////////////////////////////////////////////////

class CEFat : public CPropertyPage
{
	DECLARE_DYNAMIC(CEFat)

public:
	CEFat();
	virtual ~CEFat();

// Dialog Data
	enum { IDD = IDD_EDisk_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	PFN_FORMATVOLUMEUI m_pfnFormatVolumeUI;

	VOLUME_DATA m_vd;
	CEFatfs m_fatfs;

	CComboBox m_diskIndex;
	CComboBox m_partIndex;

	FILE_SYS m_fsType;
	DWORD m_dwSectorNum;
	BOOL m_bDumpFAT;

	CString m_strFSType;
	CString m_strSectorSize;
	CString m_strClusterSize;
	CString m_strDiskSize;
	CString m_strFatSector;
	CString m_strReserveSector;
	CString m_strStartDataSector;
	CString m_strSectorData;

    string m_strDumpBinPath;

	void InitFatParameters(void);
	BOOL EnumDiskIndex(void);
	BOOL EnumPartIndex(LPCTSTR lpDiskName);
	BOOL RefreshFatInfo(PBYTE pData);	
	void PrintSectorData(PBYTE pData, DWORD dwLen);
	BOOL UpdateDiskInfo(DISK_INFO, PARTINFO);
	BOOL ReadMBR(LPCTSTR pszDisk);
	BOOL ReadPart(LPCTSTR pszDisk, LPCTSTR pszPart);

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedGetInfo();
	afx_msg void OnBnClickedFormatDisk();
	afx_msg void OnSelectDisk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
