// CEFat.cpp : implementation file
//

#include "stdafx.h"
#include "EDisk.h"
#include "EFat.h"
#include <sipapi.h>
#include "dosbpb.h"
#include "bootsec.h"

// CEFat dialog

IMPLEMENT_DYNAMIC(CEFat, CPropertyPage)

CEFat::CEFat()
	: CPropertyPage(CEFat::IDD)
{
	m_pfnFormatVolumeUI = NULL;

	m_bDumpFAT = FALSE;
	m_dwSectorNum = 0;
	m_strFSType = _T("");
	m_strSectorSize = _T("");
	m_strClusterSize = _T("");
	m_strDiskSize = _T("");
	m_strReserveSector = _T("");
	m_strFatSector = _T("");
	m_strStartDataSector = _T("");
	m_strSectorData = _T("");

    m_strDumpBinPath = "";

	m_fsType = UNKNOW_FS;

	memset(&m_vd, 0 , sizeof(VOLUME_DATA));
}

CEFat::~CEFat()
{
	SipShowIM(SIPF_OFF);
}

void CEFat::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DSK_INDEX, m_diskIndex);
	DDX_Control(pDX, IDC_PART_INDEX, m_partIndex);
	DDX_Check(pDX, IDC_DUMP_FAT, m_bDumpFAT);
	DDX_Text(pDX, IDC_SECTOR_NUM, m_dwSectorNum);
	DDX_Text(pDX, IDC_FSTYPE, m_strFSType);
	DDX_Text(pDX, IDC_SECOTRSIZE, m_strSectorSize);
	DDX_Text(pDX, IDC_CLUSTERSIZE, m_strClusterSize);
	DDX_Text(pDX, IDC_DISKSIZE, m_strDiskSize);	
	DDX_Text(pDX, IDC_RESERVE_SECTOR, m_strReserveSector);
	DDX_Text(pDX, IDC_FAT_SIZE, m_strFatSector);
	DDX_Text(pDX, IDC_STARTDATA_SECTOR, m_strStartDataSector);
	DDX_Text(pDX, IDC_SECTORDATA, m_strSectorData);
}


BEGIN_MESSAGE_MAP(CEFat, CPropertyPage)
	ON_BN_CLICKED(IDC_GET_INFO, &CEFat::OnBnClickedGetInfo)
	ON_BN_CLICKED(IDC_FORMAT_VOL, &CEFat::OnBnClickedFormatDisk)
	ON_CBN_SELENDOK(IDC_DSK_INDEX, &CEFat::OnSelectDisk)
END_MESSAGE_MAP()

void CEFat::InitFatParameters(void)
{
	m_strFSType.Empty();
	m_strSectorSize.Empty();
	m_strClusterSize.Empty();
	m_strDiskSize.Empty();
	m_strReserveSector.Empty();
	m_strFatSector.Empty();
	m_strStartDataSector.Empty();	

	m_fsType = UNKNOW_FS;
}

BOOL CEFat::EnumPartIndex(LPCTSTR lpDiskName)
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	HANDLE hStore = INVALID_HANDLE_VALUE;	
	PARTINFO partInfo = {0};
	CE_VOLUME_INFO ceVolumeInfo = {0};
	int index = 0;

	ASSERT(lpDiskName != NULL);
	
	partInfo.cbSize = sizeof(PARTINFO);
	hStore = OpenStore(lpDiskName);
	if (INVALID_HANDLE_VALUE != hStore)
	{
		hFind = FindFirstPartition(hStore, &partInfo);
		if (INVALID_HANDLE_VALUE != hFind)
		{
			m_partIndex.ResetContent();
			m_partIndex.InsertString(index++, _T("MBR"));
			do 
			{
				m_partIndex.InsertString(index++, partInfo.szPartitionName);				
			} while (FindNextPartition(hFind, &partInfo));

			m_partIndex.SetCurSel(0);
			FindClosePartition(hFind);
		}

		CloseHandle(hStore);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CEFat::EnumDiskIndex(void)
{
	STOREINFO si = {0};
	HANDLE hFind = INVALID_HANDLE_VALUE;
	int index = 0;

	si.cbSize = sizeof(STOREINFO);
	hFind = FindFirstStore(&si);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		m_diskIndex.ResetContent();
		do 
		{
			m_diskIndex.InsertString(index, si.szDeviceName);
			index++;
		} while (FindNextStore(hFind, &si));

		m_diskIndex.SetCurSel(0);
		FindCloseStore(hFind);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CEFat::RefreshFatInfo(PBYTE pData)
{
	PBOOTSEC pBootSec = (PBOOTSEC)pData;
	TCHAR szData[MAX_PATH] = {0};
	LONGLONG llData = 0;

	if (m_fatfs.ParseFatInfo(pData) && 
			m_fatfs.GetPartData(&m_vd))
	{
		llData = m_vd.dwBytesPerSector;
		if (Size2String(llData, szData, MAX_PATH))
		{
			m_strSectorSize.Format(_T("%s"), szData);
		}

		memset(szData, 0, sizeof(szData));
		llData = m_vd.dwBytesPerCluster;
		if (Size2String(llData, szData, MAX_PATH))
		{
			m_strClusterSize.Format(_T("%s"), szData);
		}

		memset(szData, 0, sizeof(szData));
		llData = m_vd.dwTotalSectors * m_vd.dwBytesPerSector;
		if (Size2String(llData, szData, MAX_PATH))
		{
			m_strDiskSize.Format(_T("%s"), szData);
		}

		m_strStartDataSector.Format(_T("%d Sectors"), m_vd.dwStartDataSector);
		m_strReserveSector.Format(_T("%d Sectors"), m_vd.dwReserveSectors);
		m_strFatSector.Format(_T("%d Sectors"), m_vd.dwSectorsPerFat);
		switch (m_vd.dwFatVersion)
		{
		case 12:
			if (m_vd.dwNumFats == 2)
			{
				m_strFSType.Format(_T("TFAT12"));
			}
			else if (1 == m_vd.dwNumFats)
			{
				m_strFSType.Format(_T("FAT12"));
			}
			else
			{
				m_strFSType.Format(_T("Unknown FS"));
			}
			break;

		case 16:
			if (m_vd.dwNumFats == 2)
			{
				m_strFSType.Format(_T("TFAT16"));
			}
			else if (1 == m_vd.dwNumFats)
			{
				m_strFSType.Format(_T("FAT16"));
			}
			else
			{
				m_strFSType.Format(_T("Unknown FS"));
			}
			break;

		case 32:
			if (m_vd.dwNumFats == 2)
			{
				m_strFSType.Format(_T("TFAT32"));
			}
			else if (1 == m_vd.dwNumFats)
			{
				m_strFSType.Format(_T("FAT32"));
			}
			else
			{
				m_strFSType.Format(_T("Unknown FS"));
			}
			break;

		default:
			m_strFSType.Format(_T("Unknown FS"));
			break;
		}
		
		UpdateData(FALSE);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
		
}

void CEFat::PrintSectorData(PBYTE pData, DWORD dwLen)
{
	if (NULL == pData || 0 == dwLen)
	{
		return;
	}

	DWORD i = 0;	
	TCHAR temp[8] = {0};
	
	m_strSectorData.Format(_T("%.4xH: "), i);
		
	do 
	{		
		_stprintf(temp, _T("%.2X "), pData[i]);
		m_strSectorData += temp;
		if (0 == (++i % 16))
		{			
			m_strSectorData += _T("\r\n");
			_stprintf(temp, _T("%.4XH: "), i);
			m_strSectorData += temp;
			memset(temp, 0, sizeof(temp));
		}
		
	} while (i < dwLen);

	UpdateData(FALSE);

	if (m_bDumpFAT)
	{
        if (0 == m_strDumpBinPath.size())
        {
            TCHAR path[MAX_PATH] = {0};
            GetCurrentDirectory(path, MAX_PATH);
            LPSTR pPath = WCharToMByte(path);
            if (pPath)
            {
                m_strDumpBinPath.append(pPath);
                DeleteBuffer(pPath);
            }
            m_strDumpBinPath.append("boot.bin");
        }

		Write2File(pData, dwLen, m_strDumpBinPath.c_str());
	}
}

BOOL CEFat::UpdateDiskInfo(DISK_INFO di, PARTINFO pi)
{	
	BOOL bRet = 0;
	TCHAR szData[MAX_PATH] = {0};
	LONGLONG llData = 0;	

	llData = di.di_bytes_per_sect;
	if (Size2String(llData, szData, MAX_PATH))
	{
		m_strSectorSize.Format(_T("%s"), szData);
	}

	memset(szData, 0, sizeof(szData));
	llData = di.di_bytes_per_sect * di.di_total_sectors;
	if (Size2String(llData, szData, MAX_PATH))
	{
		m_strDiskSize.Format(_T("%s"), szData);
	}

	switch (m_fsType)
	{
	case BINFS:
		m_strFSType.Format(_T("BINFS"));
		break;

	case EXFAT:
		for (int i = 0; i < DIM(DskTableExfat); i++)
		{
			if (llData < DskTableExfat[i].DiskSize)
			{
				memset(szData, 0, sizeof(szData));
				llData = DskTableExfat[i].SecPerClusVal * 512;
				if (Size2String(llData, szData, MAX_PATH))
				{
					m_strClusterSize.Format(_T("%s"), szData);
				}

				break;
			}
		}

		//update files system type
		CE_VOLUME_INFO vi = {0};
		vi.cbSize = sizeof(vi);				
		CeGetVolumeInfo(pi.szVolumeName, CeVolumeInfoLevelStandard, &vi);
		if (vi.dwFlags & CE_VOLUME_TRANSACTION_SAFE)
		{
			m_strFSType.Format(_T("TEXFAT"));
		} 
		else
		{
			m_strFSType.Format(_T("EXFAT"));
		}
		
		break;
	}

	UpdateData(FALSE);

	return bRet;
	
}

BOOL CEFat::ReadMBR(LPCTSTR pszDisk)
{
	HANDLE hStore = INVALID_HANDLE_VALUE;
	STOREINFO si = {0};
	PBYTE pData = NULL;
	BOOL bRet = 0;
	
	hStore = OpenStore(pszDisk);
	if (INVALID_HANDLE_VALUE == hStore)
	{
		TRACE(_T("Error: open store error=%d\r\n"), GetLastError());
		goto EXIT;
	}
	else
	{
		si.cbSize = sizeof(si);
		GetStoreInfo(hStore, &si);
		pData = new BYTE[si.dwBytesPerSector];
		if (NULL == pData)
		{
			TRACE(_T("Error: out of memory\r\n"));
			goto EXIT;
		}

		m_fatfs.InitDiskInterface(hStore);
		if (!m_fatfs.GetBootSectorData(pData))
		{
			TRACE(_T("Error: read boot sector fail\r\n"));
			goto EXIT;
		}

		PrintSectorData(pData, SECTOR_SIZE);
		bRet = TRUE;
	}

EXIT:
	if (hStore != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hStore);		
	}

	if (pData)
	{
		delete []pData;
		pData = NULL;
	}

	return bRet;
}

BOOL CEFat::ReadPart(LPCTSTR pszDisk, LPCTSTR pszPart)
{
	BOOL bRet = 0;
	PBYTE pData = NULL;
	DISK_INFO di = {0};
	PARTINFO pi = {0};	
	HANDLE hPart = INVALID_HANDLE_VALUE;

	hPart = m_fatfs.OpenPart(pszDisk, pszPart);
	if (INVALID_HANDLE_VALUE == hPart)
	{
		TRACE(_T("Error: open part error=%d\r\n"), GetLastError());
		goto EXIT;
	}
	else
	{		
		pi.cbSize = sizeof(pi);
		GetPartitionInfo(hPart, &pi);
		if (_tcsstr(pi.szFileSys, _T("binfs")) ||
			_tcsstr(pi.szFileSys, _T("BINFS")))
		{
			m_fsType = BINFS;
		}
		else
		{
			m_fsType = FAT;
		}		
	}

	if (!m_fatfs.GetDiskInfo(&di))
	{
		TRACE(_T("Error: get disk info error=%d"), GetLastError());
		goto EXIT;
	}

	pData = new BYTE[di.di_bytes_per_sect];
	if (NULL == pData)
	{
		TRACE(_T("Error: out of memory\r\n"));
		goto EXIT;
	}	
	else
	{
		memset(pData, 0, di.di_bytes_per_sect);
	}

	//read partition boot sector data	
	if (!m_fatfs.GetBootSectorData(pData))
	{
		TRACE(_T("Error: read boot sector fail\r\n"));
		goto EXIT;
	}

	if (BINFS == m_fsType)
	{
		UpdateDiskInfo(di, pi);
	} 
	else
	{
		PBOOTSEC pBootSec = (PBOOTSEC)pData;
		if (strstr((char *)pBootSec->bsOemName, "exfat") ||
			strstr((char *)pBootSec->bsOemName, "EXFAT"))
		{
			m_fsType = EXFAT;
		}

		if (EXFAT == m_fsType)
		{
			UpdateDiskInfo(di, pi);
		}
		else
		{
			//update FAT information
			RefreshFatInfo(pData);
		}
	}	

	if (m_dwSectorNum != 0)
	{
		if (!m_fatfs.ReadSectors(m_dwSectorNum, pData, 1))
		{
			TRACE(_T("read %d sectors error"), m_dwSectorNum);
			goto EXIT;
		}
	}

	//display sector data
	PrintSectorData(pData, di.di_bytes_per_sect);

	bRet = TRUE;

EXIT:	
	if (hPart != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hPart);
	}

	if (pData)
	{
		delete []pData;
		pData = NULL;
	}

	return bRet;
}

// CEFat message handlers

BOOL CEFat::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	HINSTANCE hDll = LoadLibrary(_T("FATUTIL.DLL"));
	if (NULL == hDll)
	{
		TRACE(_T("Load fatutil.dll error = %d\r\n"), GetLastError());
		return FALSE;
	}

	m_pfnFormatVolumeUI = (PFN_FORMATVOLUMEUI)GetProcAddress(hDll, _T("FormatVolumeUI"));

	if (!EnumDiskIndex())
	{
		TRACE(_T("No disk in current device\r\n"));
		return FALSE;
	}
	
	OnSelectDisk();

	
	FreeLibrary(hDll);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEFat::OnSelectDisk()
{
	int index = m_diskIndex.GetCurSel();
	TCHAR diskName[MAX_PATH] = {0};

	m_diskIndex.GetLBText(index, diskName);
	EnumPartIndex((LPCTSTR)diskName);
}

void CEFat::OnBnClickedGetInfo()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SipShowIM(SIPF_OFF);	
	InitFatParameters();
	
	TCHAR diskName[32] = {0};	
	TCHAR partName[32] = {0};	
	
	m_diskIndex.GetLBText( m_diskIndex.GetCurSel(), diskName);	
	m_partIndex.GetLBText(m_partIndex.GetCurSel(), partName);
	if (0 == _tcscmp(partName, _T("MBR")))
	{
		ReadMBR(diskName);
	}
	else
	{
		ReadPart(diskName, partName);
	}	
}

void CEFat::OnBnClickedFormatDisk()
{
	int index = 0;
	TCHAR diskName[32] = {0};	
	TCHAR partName[32] = {0};	
	HANDLE hVolume = INVALID_HANDLE_VALUE;
	HANDLE hPart = INVALID_HANDLE_VALUE;

	index = m_diskIndex.GetCurSel();
	m_diskIndex.GetLBText(index, diskName);
	hVolume = OpenStore(diskName);
	if (INVALID_HANDLE_VALUE == hVolume)
	{
		TRACE(_T("Error: open disk error=%d\r\n"), GetLastError());
		goto EXIT;
	}	

	index = m_partIndex.GetCurSel();
	m_partIndex.GetLBText(index, partName);
	hPart = OpenPartition(hVolume, partName);
	if (INVALID_HANDLE_VALUE == hPart)
	{
		TRACE(_T("Error: open part error=%d\r\n"), GetLastError());
		goto EXIT;
	}
	else
	{
		if (m_pfnFormatVolumeUI && DismountPartition(hPart))
		{
			m_pfnFormatVolumeUI(hPart, m_hWnd);
			MountPartition(hPart);
		}
	}

EXIT:
	if (hVolume != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hVolume);
	}

	if (hPart != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hPart);
	}
}
BOOL CEFat::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_LBUTTONDBLCLK == pMsg->message && 
			pMsg->hwnd == (GetDlgItem(IDC_SECTOR_NUM)->m_hWnd))
	{
		SipShowIM(SIPF_ON);
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}
