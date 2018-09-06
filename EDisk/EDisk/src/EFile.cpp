// EFile.cpp : implementation file
//

#include "stdafx.h"
#include "storemgr.h"
#include "disk.h"
#include "EDisk.h"
#include "EFile.h"
#include "eutil.h"


// CEFile dialog

IMPLEMENT_DYNAMIC(CEFile, CPropertyPage)

CEFile::CEFile()
	: CPropertyPage(CEFile::IDD)
	, m_strFileData(_T(""))
	, m_strFilePath(_T(""))
{
	m_nIndex = 0;
	m_hPart = INVALID_HANDLE_VALUE;
	ZeroMemory(m_fd, sizeof(m_fd));
}

CEFile::~CEFile()
{
	if (m_hPart != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hPart);
		m_hPart = INVALID_HANDLE_VALUE;
	}
}

void CEFile::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILE_DATA, m_strFileData);
	DDX_Text(pDX, IDC_FILE_PATH, m_strFilePath);
}


BEGIN_MESSAGE_MAP(CEFile, CPropertyPage)
	ON_BN_CLICKED(IDC_FILE_SELECT, &CEFile::OnBnClickedFileSelect)
END_MESSAGE_MAP()

BOOL CEFile::EnumDiskInfo(void)
{
	STOREINFO si = {0};
	HANDLE hFind = INVALID_HANDLE_VALUE;
	
	si.cbSize = sizeof(STOREINFO);
	hFind = FindFirstStore(&si);
	if (INVALID_HANDLE_VALUE != hFind)
	{		
		do 
		{
			_tcsncpy(m_fd[m_nIndex].szDisk, si.szDeviceName, MAX_DISK_NAME_LEN);
			EnumPartInfo(si.szDeviceName);
			m_nIndex++;
		} while (FindNextStore(hFind, &si));
		
		FindCloseStore(hFind);
	}
	else
	{
		return FALSE;
	}

	return TRUE; 
}

BOOL CEFile::EnumPartInfo(LPCTSTR szDisk)
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	HANDLE hStore = INVALID_HANDLE_VALUE;	
	PARTINFO partInfo = {0};
	CE_VOLUME_INFO ceVolumeInfo = {0};
	
	ASSERT(szDisk != NULL);

	partInfo.cbSize = sizeof(PARTINFO);
	hStore = OpenStore(szDisk);
	if (INVALID_HANDLE_VALUE != hStore)
	{
		hFind = FindFirstPartition(hStore, &partInfo);
		if (INVALID_HANDLE_VALUE != hFind)
		{			
			do 
			{
				_tcsncpy(m_fd[m_nIndex].szFolder, partInfo.szVolumeName, MAX_DISK_NAME_LEN);
				_tcsncpy(m_fd[m_nIndex].szPartition, partInfo.szPartitionName, MAX_DISK_NAME_LEN);				
			} while (FindNextPartition(hFind, &partInfo));
			
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

UINT CEFile::FindDisk(LPCTSTR szFolder)
{
	UINT index = 0;

	for (index = 0; index < MAX_DISK_NAME_LEN; index++)
	{
		if (0 == _tcscmp(m_fd[index].szFolder, szFolder))
		{
			break; 
		}
	}

	return index;
}

void CEFile::PrintFileData(PBYTE pData, DWORD dwLen)
{
	if (NULL == pData || 0 == dwLen)
	{
		return;
	}	

	DWORD i = 0;	
	TCHAR temp[8] = {0};
	static UINT nCnt = 0;

	if (m_strFileData.IsEmpty())		
	{
		m_strFileData.Format(_T("%.4xH: "), i);
		nCnt = 0;
	}	

	while (i < dwLen && nCnt < KB_SIZE) 
	{		
		_stprintf(temp, _T("%.2X "), pData[i]);
		m_strFileData += temp;
		if (0 == (++i % 16))
		{			
			m_strFileData += _T("\r\n");
			_stprintf(temp, _T("%.4XH: "), i);
			m_strFileData += temp;
			memset(temp, 0, sizeof(temp));
		}

		nCnt++;
	}

	UpdateData(FALSE);

	//write date to file
	Write2File(pData, dwLen, "\\Flash Disk\\myData.bin");
}

BOOL CEFile::Read(HANDLE hFile)
{
	PFATFS_FILE_HANDLE pFile = (PFATFS_FILE_HANDLE)hFile;
	DWORD dwLen = pFile->fd.node.size;
	DWORD dwReadLen = 0;
	PBYTE pData = NULL;

	if (dwLen <= MB_SIZE)
	{
		pData = new BYTE[dwLen];
		if (NULL == pData)
		{
			return FALSE;
		}
		else
		{
			memset(pData, 0, dwLen);
		}
		
		dwReadLen = m_fatfs.ReadFile(hFile, pData, dwLen);
		if (dwReadLen != 0)
		{
			//read ok, process read data
			PrintFileData(pData, dwReadLen);
		}		
	}
	else
	{
		pData = new BYTE[MB_SIZE];
		if (NULL == pData) 
		{
			TRACE(_T("Error: no enough memory\r\n"));
			return FALSE;
		}

		DWORD dwRealLen = 0;
		do 
		{				
			dwRealLen = min(dwLen, MB_SIZE);
			memset(pData, 0, dwRealLen);
			dwReadLen = m_fatfs.ReadFile(hFile, pData, dwRealLen);
			if (dwReadLen != 0)
			{
				//read ok, process read data
				PrintFileData(pData, dwReadLen);
			}
			dwLen -= dwReadLen;
		} while (dwLen > 0);		

	}

	if (pData)
	{
		delete []pData;
	}	

	return TRUE;
}

BOOL CEFile::ReadFileData(LPCTSTR szFile)
{
	HANDLE hFile = INVALID_HANDLE_VALUE;		
	BOOL bRet = TRUE;

	if (szFile[0] == _T('\\')) szFile++;

	CString strName = szFile;
	int n = strName.Find(_T('\\'));
	CString strFile = strName.Right(strName.GetLength() - n - 1);

	hFile = m_fatfs.CreateFile((LPTSTR)(LPCTSTR)strFile);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		TRACE(_T("Error:open file fail\r\n"));
		return FALSE;
	}
	else
	{		
		bRet = Read(hFile);
	}
	
	CloseHandle(hFile);

	return bRet;	
}

BOOL CEFile::InitPart(LPCTSTR szPath)
{
	BOOL bRet = TRUE;
	TCHAR *szTemp = NULL;
	TCHAR folder[MAX_DISK_NAME_LEN] = {0};
	UINT index = 0;

	//find the part of file
	if (szPath[0] == _T('\\')) szPath++;

	szTemp = (TCHAR *)_tcsstr(szPath, _T("\\"));
	_tcsncpy(folder, szPath, szTemp - szPath);
	index = FindDisk(folder);

	//init part of the file in	
	m_hPart = m_fatfs.OpenPart(m_fd[index].szDisk, 
		m_fd[index].szPartition);

	if (m_hPart == INVALID_HANDLE_VALUE)
	{
		TRACE(_T("Error:open part error=%d"), GetLastError());
		return FALSE;
	}

	DISK_INFO di = {0};
	PBYTE pData = NULL;
	m_fatfs.GetDiskInfo(&di);
	pData = new BYTE[di.di_bytes_per_sect];
	if (NULL == pData)
	{
		TRACE(_T("Error:no enough memory\r\n"));
		return FALSE;
	}

	if (!m_fatfs.GetBootSectorData(pData) ||
		!m_fatfs.ParseFatInfo(pData))
	{
		TRACE(_T("Error:disk read error\r\n"));
		bRet = FALSE;
	}

	delete []pData;

	return bRet;
}


// CEFile message handlers

BOOL CEFile::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	EnumDiskInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEFile::OnBnClickedFileSelect()
{
	// TODO: Add your control notification handler code here
	CFileDialog fileDlg(TRUE, 
											NULL, 
											NULL,  
											OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,											
											_T("All Files (*.*)|*.*||"),
											GetSafeOwner(),
											0);
	if (IDOK == fileDlg.DoModal())
	{
		m_strFilePath = fileDlg.GetPathName();
		UpdateData(FALSE);

		if (m_strFilePath.GetLength() > 0)
		{
			if (InitPart((LPCTSTR)m_strFilePath))
			{
				m_strFileData.Empty();
				ReadFileData((LPCTSTR)m_strFilePath);
			}			
		}
	}
}


