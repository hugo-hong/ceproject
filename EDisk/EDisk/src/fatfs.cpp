#include "stdafx.h"
#include "storemgr.h"
#include "fatfs.h"
#include "dosbpb.h"
#include "bootsec.h"
#include "eutil.h"



CEFatfs::CEFatfs()
{	
	memset(&m_vd, 0, sizeof(m_vd));
	m_bMatch = 0;
	m_bFind = 0;
	m_byChkSum = 0;
	m_dwIndex = 0;
	m_dwLEntryCnt = 0;
	m_dwCurrentEntryCnt = 0;
	m_fEntryInval = 0;
	m_fCurrentEntryType = DIR_UNKNOW;
	m_fLastEntryType = DIR_UNKNOW;
}

CEFatfs::~CEFatfs()
{

}

BOOL CEFatfs::GetPartData(PVOLUME_DATA pVd)
{
	ASSERT(pVd != NULL);

	memcpy(pVd, &m_vd, sizeof(m_vd));

	return TRUE;
}

HANDLE CEFatfs::OpenPart(LPCTSTR pszDisk, LPCTSTR pszPart)
{	
	DWORD dwErr = 0;
	HANDLE hStore = INVALID_HANDLE_VALUE;
	HANDLE hPart = INVALID_HANDLE_VALUE;

	ASSERT(pszDisk != NULL && pszPart != NULL);

	hStore = ::OpenStore(pszDisk);
	if (INVALID_HANDLE_VALUE == hStore)
	{
		dwErr = GetLastError();
		goto EXIT;
	}
	else
	{
		hPart = OpenPartition(hStore, pszPart);
		if (INVALID_HANDLE_VALUE == hPart)
		{
			dwErr = GetLastError();
			goto EXIT;
		}
	}

	InitDiskInterface(hPart);

EXIT:
	if (hStore != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hStore);
	}

	SetLastError(dwErr);
	return hPart;
}

BOOL CEFatfs::InitDiskInterface(HANDLE hPart)
{
	DISK_INFO di = {0};

	m_disk.Init(hPart);
	m_disk.GetDiskInfo(&di);	
	m_disk.SetParameter(di.di_bytes_per_sect, &m_vd);

	return TRUE;
}

BOOL CEFatfs::GetDiskInfo(PDISK_INFO pdi)
{
	ASSERT(pdi != NULL);

	return m_disk.GetDiskInfo(pdi);
}

BOOL CEFatfs::ReadSectors(DWORD dwStart, PBYTE pData, DWORD dwNum)
{
	return m_disk.ReadSectors(dwStart, pData, dwNum);
}

BOOL CEFatfs::GetBootSectorData(PBYTE pBootSector)
{
	ASSERT(pBootSector != NULL);

	return m_disk.ReadSectors(0, pBootSector, 1);
}


BOOL CEFatfs::ParseFatInfo(PBYTE pBootSectorData)
{	
	BOOL bRet = 0;
	PBOOTSEC pBootSec = (PBOOTSEC)pBootSectorData;

	if (pBootSec->bsBPB.BPB_SectorsPerFAT != 0)//FAT12/FAT16
	{		
		PBPB pBpb = &(pBootSec->bsBPB);

		m_vd.dwStartFatSector = pBpb->BPB_ReservedSectors;
		m_vd.dwFatType = (memcmp(pBootSec->bsFileSysType, "TFAT", 4) == 0) ? 
											FATTYPE_TFAT_FAT0 : FATTYPE_NORMAL;
		m_vd.dwNumFats = pBpb->BPB_NumberOfFATs;
		if (m_vd.dwNumFats == 0) 
		{
			if (m_vd.dwFatType == FATTYPE_TFAT_FAT0) 
			{            
				// If NumFats = 0, then FAT1 is the last known good FAT
				m_vd.dwFatType = FATTYPE_TFAT_FAT1;
				m_vd.dwNumFats = 2;
			}
			else
			{
				// If not TFAT, then invalid volume
				goto EXIT;
			}				
		} 		

		m_vd.dwSectorsPerFat = pBpb->BPB_SectorsPerFAT;
		if (m_vd.dwSectorsPerFat == 0)
			goto EXIT;

		m_vd.dwBytesPerSector = pBpb->BPB_BytesPerSector;
		m_vd.dwReserveSectors = pBpb->BPB_ReservedSectors;
		m_vd.dwStartRootDirSector = pBpb->BPB_ReservedSectors + pBpb->BPB_NumberOfFATs * pBpb->BPB_SectorsPerFAT;
		m_vd.dwRootEntries = pBpb->BPB_RootEntries;
		m_vd.dwRootDirSectors = (m_vd.dwRootEntries * DIRENTRY_SIZE + pBpb->BPB_BytesPerSector - 1) / pBpb->BPB_BytesPerSector;
		m_vd.dwRootCluster = 0;

		m_vd.dwStartDataSector = m_vd.dwStartRootDirSector + m_vd.dwRootDirSectors;
		m_vd.dwTotalSectors = (pBpb->BPB_TotalSectors == 0) ? pBpb->BPB_BigTotalSectors : pBpb->BPB_TotalSectors;
		m_vd.dwDataSectors = m_vd.dwTotalSectors - m_vd.dwStartDataSector;
		m_vd.dwSectorsPerCluster = pBpb->BPB_SectorsPerCluster;
		if (m_vd.dwSectorsPerCluster == 0)
			goto EXIT;

		m_vd.dwNumClusters = m_vd.dwDataSectors / m_vd.dwSectorsPerCluster;
		m_vd.dwMaxCluster = m_vd.dwNumClusters + DATA_CLUSTER - 1;
		m_vd.dwFatVersion = (m_vd.dwNumClusters < FAT12_MAX_CLST_NUM) ? FAT_12 : FAT_16;
		m_vd.dwBytesPerCluster = m_vd.dwSectorsPerCluster * m_vd.dwBytesPerSector;


	}
	else//FAT32
	{
		PBIGFATBOOTSEC pBigBootSec = (PBIGFATBOOTSEC)pBootSectorData;  
		PBPB pBpb = &(pBigBootSec->bgbsBPB.oldBPB);

		m_vd.dwStartFatSector = pBpb->BPB_ReservedSectors;
		m_vd.dwFatType = (memcmp(pBigBootSec->bgbsFileSysType, "TFAT", 4) == 0) ? 
											FATTYPE_TFAT_FAT0 : FATTYPE_NORMAL;
		m_vd.dwNumFats = pBpb->BPB_NumberOfFATs;
		if (m_vd.dwNumFats == 0)
		{
			if (m_vd.dwFatType == FATTYPE_TFAT_FAT0)
			{            
				// If NumFats = 0, then FAT1 is the last known good FAT
				m_vd.dwFatType = FATTYPE_TFAT_FAT1;
				m_vd.dwNumFats = 2;
			}
			else 
			{
				// If not TFAT, then invalid volume
				goto EXIT;
			}
		}		

		m_vd.dwSectorsPerFat = pBigBootSec->bgbsBPB.BGBPB_BigSectorsPerFAT;
		if (m_vd.dwSectorsPerFat == 0)
			goto EXIT;

		m_vd.dwBytesPerSector = pBpb->BPB_BytesPerSector;
		m_vd.dwReserveSectors = pBpb->BPB_ReservedSectors;
		m_vd.dwStartDataSector = pBpb->BPB_ReservedSectors + m_vd.dwNumFats * m_vd.dwSectorsPerFat;
		m_vd.dwTotalSectors = pBpb->BPB_BigTotalSectors;
		m_vd.dwDataSectors = pBpb->BPB_BigTotalSectors - m_vd.dwStartDataSector;

		m_vd.dwSectorsPerCluster = pBpb->BPB_SectorsPerCluster;
		if (m_vd.dwSectorsPerCluster == 0)
			goto EXIT;

		m_vd.dwNumClusters = m_vd.dwDataSectors / m_vd.dwSectorsPerCluster;
		m_vd.dwMaxCluster = m_vd.dwNumClusters + DATA_CLUSTER - 1;
		m_vd.dwFatVersion = FAT_32;

		m_vd.dwRootEntries = 0;
		m_vd.dwRootDirSectors = 0;
		m_vd.dwRootCluster = pBigBootSec->bgbsBPB.BGBPB_RootDirStrtClus;

		m_vd.dwBytesPerCluster = m_vd.dwSectorsPerCluster * m_vd.dwBytesPerSector;
	}

	bRet = TRUE;

EXIT:
	return bRet;
}

BOOL CEFatfs::InitRootDirEntry(PFATFS_DIR_ENTRY pdentry)
{
	ASSERT(pdentry != NULL);
	memset(pdentry, 0, sizeof(FATFS_DIR_ENTRY));
	pdentry->mode = MODE_DIR;
	pdentry->size = m_vd.dwRootDirSectors * m_vd.dwBytesPerSector;
	return TRUE;
}

int CEFatfs::InitPos(PFATFS_DIR_ENTRY pNode, PFATFS_DATA_POS pPos)
{	
	pPos->dwClusterNum = pNode->cluster;
	pPos->dwClusterSeq = 0;
	pPos->dwOffsetCluster = 0;

	return EN_OK;
}

BOOL CEFatfs::CloseFile(HANDLE hFile)
{
	PFATFS_FILE_HANDLE pFile = (PFATFS_FILE_HANDLE)hFile;

	if (NULL == pFile)
	{
		return FALSE;
	}
	else
	{
		delete pFile;	
		pFile = NULL;
	}	

	return TRUE;
}

HANDLE CEFatfs::CreateFile(LPTSTR pszFileName)
{
	int err = 0;
	FATFS_DIR_SEARCH ds = {0};
	FATFS_DIR_ENTRY rootDir = {0};
	PFATFS_FILE_HANDLE pFile = NULL;
	HANDLE hFile = INVALID_HANDLE_VALUE;

	//covert chars to uppercase
	_tcsupr(pszFileName);

	InitRootDirEntry(&rootDir);	
	
	ds.dir = rootDir;
	ds.path = pszFileName;
	ds.node = ds.dir;	
	
	err = Find(&ds);
	if (err != EN_OK)
	{
		//no such file, now create it...
		hFile = DoCreateFile(&ds);		
	}
	else
	{
		//open exist file
		pFile = new FATFS_FILE_HANDLE;
		if (NULL == pFile) return hFile;
		
		pFile->dwOffset = 0;
		pFile->fd.node = ds.node;

		InitPos(&pFile->fd.node, &pFile->fd.pos);

		hFile = (HANDLE)pFile;		
	}

	return hFile;
}

DWORD CEFatfs::ReadFile(HANDLE hFile, LPBYTE pBuf, DWORD dwLen)
{
	PFATFS_FILE_HANDLE pFile = (PFATFS_FILE_HANDLE)hFile;
	PFATFS_FD_ENTRY pFd = &pFile->fd;
	PFATFS_DIR_ENTRY pNode = &pFd->node;
	DWORD dwRet = 0;
	int err = 0;

	if (NULL == pFile || NULL == pBuf)
	{
		return (-1);
	}

	if (dwLen > 0 && pFile->dwOffset < pNode->size)
	{
		dwLen = min(dwLen, pNode->size - pFile->dwOffset);
		err = ReadData(pBuf, dwLen, &pFd->pos);
		pFile->dwOffset += dwLen;

		if (EN_OK == err)
		{
			dwRet = dwLen;
		} 
		else if (EN_EOF == err)
		{
			dwRet = -1;
		}
	}

	return dwRet;
}

DWORD CEFatfs::WriteFile(HANDLE hFile, LPBYTE pBuf, DWORD dwLen)
{
	PFATFS_FILE_HANDLE pFile = (PFATFS_FILE_HANDLE)hFile;
	PFATFS_FD_ENTRY pFd = &pFile->fd;
	PFATFS_DIR_ENTRY pNode = &pFd->node;
	DWORD dwRet = 0;
	int err = 0;

	if (NULL == pFile || NULL == pBuf)
	{
		return (-1);
	}

	DWORD dwBytesRemain = dwLen;
	DWORD dwOnceWrite = 0;
	while (dwBytesRemain > 0)
	{
		dwOnceWrite = min(dwLen, m_vd.dwBytesPerSector);
		err = WriteData(pBuf, dwOnceWrite, &pFd->pos);
		if (err != EN_OK) break;

		pFile->dwOffset += dwOnceWrite;
		dwBytesRemain -= dwOnceWrite;
		dwRet += dwOnceWrite;
	}

	return dwRet;
}

BOOL CEFatfs::SeekFile(HANDLE hFile, DWORD dwOffset)
{
	PFATFS_FILE_HANDLE pFile = (PFATFS_FILE_HANDLE)hFile;
	PFATFS_FD_ENTRY pFd = &pFile->fd;
	PFATFS_DIR_ENTRY pNode = &pFd->node;
	BOOL bRet = 0;
	int err = 0;

	InitPos(pNode, &pFd->pos);
	pFile->dwOffset = dwOffset;
	while (dwOffset >= m_vd.dwBytesPerCluster)
	{
		err = Get_NextChainCluster(&pFd->pos);
		if (err != EN_OK)
		{
			goto EXIT;
		}

		dwOffset -= m_vd.dwBytesPerCluster;
	}

	pFd->pos.dwOffsetCluster = pFile->dwOffset % m_vd.dwBytesPerCluster;
	bRet = TRUE;

EXIT:
	return bRet;
}

HANDLE CEFatfs::DoCreateFile(PFATFS_DIR_SEARCH pDs)
{
	PFATFS_FILE_HANDLE pFile = NULL;
	BYTE dirEntry[DIRENTRY_SIZE] = {0};
	BYTE sName[16] = {0};
	PFAT_LONG_DIR_ENTRY plDirEntry = NULL;
	PFAT_SHORT_DIR_ENTRY psDirEntry = NULL;
	int err = 0;
	int len = 0;
	BYTE checkSum = 0;
	
	if (!NameConvert((LPTSTR)pDs->name, (LPSTR)sName))
	{
		goto EXIT;
	}

	//create file long dir entry
	plDirEntry = (PFAT_LONG_DIR_ENTRY)dirEntry;
	len = _tcslen(pDs->name);
	checkSum = DoChkSum((PBYTE)sName);
	m_dwIndex = m_dwLEntryCnt = (len + MAX_LDIRENTRY_NAME - 1) / MAX_LDIRENTRY_NAME;
	for (UINT i = 1; i <= m_dwIndex; i++)
	{
		memset(plDirEntry, 0, DIRENTRY_SIZE);

		memcpy(plDirEntry->name1, &pDs->name[(m_dwIndex - i) * 13], sizeof(plDirEntry->name1));
		memcpy(plDirEntry->name2, &pDs->name[(m_dwIndex - i) * 13 + 5], sizeof(plDirEntry->name2));
		memcpy(plDirEntry->name3, &pDs->name[(m_dwIndex - i) * 13 + 11], sizeof(plDirEntry->name3));

		if (i < m_dwIndex)
		{
			plDirEntry->order = LAST_LONG_ENTRY | (m_dwIndex - i + 1);
		}
		else
		{
			plDirEntry->order = 1;
		}

		plDirEntry->attr = ATTR_LONG_NAME;		 
		plDirEntry->chksum = checkSum;

		err = CreateDirEntry(&pDs->node.disk_pos, (PBYTE)plDirEntry);
		if (err != EN_OK) goto EXIT; 
	}

	//create file short directory entry
	psDirEntry = (PFAT_SHORT_DIR_ENTRY)dirEntry;
	memset(psDirEntry, 0, DIRENTRY_SIZE);
	memcpy(psDirEntry->name, sName, sizeof(psDirEntry->name));
	memcpy(psDirEntry->ext, &sName[9], sizeof(psDirEntry->ext));
	psDirEntry->attr = ATTR_ARCHIVE;

	err = CreateDirEntry(&pDs->node.disk_pos, (PBYTE)psDirEntry);
	if (err != EN_OK) goto EXIT; 
		
	//create file handle
	pFile = new FATFS_FILE_HANDLE;
	if (NULL == pFile)
	{
		goto EXIT;
	}
	else
	{
		memset(pFile, 0, sizeof(FATFS_FILE_HANDLE));

		_tcscpy(pFile->fd.node.filename, pDs->name);
		pFile->fd.node.disk_pos = pDs->node.disk_pos;
		
		InitPos(&pFile->fd.node, &pFile->fd.pos);				
	}

EXIT:
	return (HANDLE)pFile;

}

int CEFatfs::Find(PFATFS_DIR_SEARCH pDs)
{
	int err = 0;

	//if empty path,return ok
	if (*(pDs->path) == _T('\0'))
	{
		return EN_OK;
	}

	while (TRUE)
	{
		err = Find_DEntry(pDs);
		if (err != EN_OK) return err;
		
		//find the last name, ok
		if (pDs->bLast) return EN_OK;

		//update dir search object to next directory
		pDs->dir = pDs->node;

		pDs->path += pDs->len;

		//skip pDir name separators
		if (pDs->path[0] == _T('\\'))
		{
			pDs->path++;
		}		
	}

	return err;
}

int CEFatfs::Find_DEntry(PFATFS_DIR_SEARCH pDs)
{	
	const TCHAR *path = pDs->path;
	const TCHAR *name = path;
	int len = 0;
	int err = 0;

	//calculate the name length
	while (*name != _T('\0') && 
		   *name != _T('\\'))
	{
		name++;
		len++;
	}

	//if terminated on a null, set last flag
	if (*name == _T('\0'))
	{
		pDs->bLast = TRUE;
	}

	//update name in pDir search object
	pDs->name = path;
	pDs->len = len;
	pDs->bFind = FALSE;

	//calculate the long dir entry count
	m_dwLEntryCnt = (len + MAX_LDIRENTRY_NAME - 1) / MAX_LDIRENTRY_NAME;	

	m_bFind = m_bMatch = FALSE;
	
	err = Find_DirEntry(pDs);
	
EXIT:
	pDs->bFind == TRUE ? err = EN_OK : err = EN_NOENT;		

	return err;
}

int CEFatfs::Find_DirEntry(PFATFS_DIR_SEARCH pDs)
{
	FATFS_DATA_POS pos = {0};
	int err = 0;

	InitPos(&pDs->dir, &pos);

	while (TRUE)
	{
		err = Read_DEntry(&pDs->dir, &pos, &pDs->node);
		if (err != EN_OK)
		{
			err == EN_EOF ? EN_OK : err;
			break;
		}		

		_tcsupr(pDs->node.filename);		

		if (0 == _tcsncmp(pDs->node.filename, pDs->name, pDs->len))
		{
			m_bFind = TRUE;
		}	

		if ( TRUE == m_bFind && TRUE == m_bMatch)
		{
			pDs->bFind = TRUE;
			break;
		}		
	}

	return err;
}

int CEFatfs::Read_DEntry(PFATFS_DIR_ENTRY pDir, 
						PFATFS_DATA_POS pPos, 
						PFATFS_DIR_ENTRY pNode)
{	
	BYTE dir_entry[DIRENTRY_SIZE] = {0};
	int err = 0;

	err = Read_NextDEntry(pPos, dir_entry);
	if (err != EN_OK) return err;
	
	Raw2Entry(dir_entry, pPos, pNode);

	pNode->parent_cluster = pDir->cluster;

	//increment position for next call
	pPos->dwOffsetCluster += DIRENTRY_SIZE;

	return err;
}

int CEFatfs::CreateDirEntry(PFATFS_DATA_POS pPos, PBYTE pDirEntry)
{
	int err = 0;

	if (NULL == pPos || NULL == pDirEntry)
	{
		return EN_INVAL;
	}

	pPos->dwOffsetCluster += DIRENTRY_SIZE;
	//change the cluster if needed
	if (pPos->dwOffsetCluster >= m_vd.dwBytesPerCluster)
	{
		err = Get_NextChainCluster(pPos);
		if (err != EN_OK)
		{
			return err;
		}
	}

	err = WriteCluster(pDirEntry, DIRENTRY_SIZE, pPos->dwClusterNum, pPos->dwOffsetCluster);

	return err;
}

int CEFatfs::Read_NextDEntry(PFATFS_DATA_POS pPos, PBYTE pDirEntry)
{
	int err = 0;

	if (NULL == pPos || NULL == pDirEntry)
	{
		return EN_INVAL;
	}

	//if we are reading the root pDir on FAT32
	if (FAT_32 == m_vd.dwFatVersion && 0 == pPos->dwClusterNum)
	{
		pPos->dwClusterNum = m_vd.dwRootCluster;
	}

	while (TRUE)
	{
		//FAT12/16 root pDir check
		if (0 == pPos->dwClusterNum)
		{
			if (pPos->dwOffsetCluster >= m_vd.dwRootEntries * DIRENTRY_SIZE)
			{
				err = EN_EOF;
			}			
		}
		else
		{
			//change the cluster if needed
			if (pPos->dwOffsetCluster >= m_vd.dwBytesPerCluster)
			{
				err = Get_NextChainCluster(pPos);
			}
		}

		if (err != EN_OK) break;

		err = ReadDirEntry(pPos, pDirEntry);
		if (err != EN_OK) break;

		if (END_DIRENTRY == pDirEntry[0])
		{
			//end pDirectory
			err = EN_EOF;
			break;
		} 
		else if (FREE_DIRENTRY != pDirEntry[0] &&
						_T('.') != pDirEntry[0])					
		{
			//find a file
			break;
		}			

		pPos->dwOffsetCluster += DIRENTRY_SIZE;
	}

	return err;
}

int CEFatfs::ReadDirEntry(PFATFS_DATA_POS pPos, PBYTE pDirEntry)
{		
	BYTE data[DIRENTRY_SIZE] = {0};
	DWORD dwLen = DIRENTRY_SIZE;
	int err = 0;	

	if (NULL == pPos || NULL == pDirEntry)
	{
		return EN_INVAL;
	}

	if (0 == pPos->dwClusterNum)
	{
		//FAT12/16 root directory
		DWORD dwSecNum = 0;
		DWORD dwOffset = 0;
		DWORD dwPos = 0;

		dwPos = pPos->dwOffsetCluster / m_vd.dwBytesPerSector;
		dwOffset = pPos->dwOffsetCluster % m_vd.dwBytesPerSector;
		dwSecNum = m_vd.dwStartRootDirSector + dwPos;
		err = ReadSector(dwSecNum, dwOffset, data, dwLen);
	} 
	else
	{
		//directory access by cluster
		err = ReadCluster(data, dwLen, pPos->dwClusterNum, pPos->dwOffsetCluster);		
	}

	if (err != EN_OK)	return err;	

	if (((data[11] & ATTR_LONG_NAME_MASK) == ATTR_LONG_NAME))
	{
		//long directory entry	
		m_fCurrentEntryType = DIR_LONG;				
		if (!m_bFind)
		{
			err = FillDirEntry(data, pDirEntry);
		}				
	}
	else 
	{
		//short directory entry		
		m_fCurrentEntryType = DIR_SHORT;
		err = FillDirEntry(data, pDirEntry);
	}	

	return err;
}

int CEFatfs::Raw2Entry(PBYTE pRawEntry,
					    PFATFS_DATA_POS pPos,
					    PFATFS_DIR_ENTRY pNode)
{
	int err = 0;

	if (NULL == pRawEntry || NULL == pPos || NULL == pNode)
	{
		return EN_INVAL;
	}

	if (DIR_LONG == m_fCurrentEntryType)
	{
		//long directory entry
		if (!m_bFind)
		{
			PFAT_LONG_DIR_ENTRY plDirEntry = (PFAT_LONG_DIR_ENTRY)pRawEntry;
			GetFileName(plDirEntry);
			m_byChkSum = plDirEntry->chksum;
			if (m_dwIndex == m_dwCurrentEntryCnt &&	m_dwCurrentEntryCnt != 0)
			{
				for (UINT i = m_dwIndex; i > 0; i--)
				{
					memcpy(&pNode->filename[(m_dwIndex - i) * 13], m_plName[i - 1].name1, sizeof(m_plName[i - 1].name1));
					memcpy(&pNode->filename[(m_dwIndex - i) * 13 + 5], m_plName[i - 1].name2, sizeof(m_plName[i - 1].name2));
					memcpy(&pNode->filename[(m_dwIndex - i) * 13 + 11], m_plName[i - 1].name3, sizeof(m_plName[i - 1].name3));
				}
				m_dwIndex = m_dwCurrentEntryCnt = 0;
				delete []m_plName;
				m_plName = NULL;
			}
		}
	}
	else
	{
		//short directory entry
		PFAT_SHORT_DIR_ENTRY psDirEntry = (PFAT_SHORT_DIR_ENTRY)pRawEntry;
		if (DIR_SHORT == m_fLastEntryType)
		{			
			m_bMatch = TRUE;

			for (int i = 0; i < DIM(psDirEntry->name); i++)
			{
				if (psDirEntry->name[i] == 0x20)
				{
					psDirEntry->name[i] = 0;
					break;
				}
			}
			LPWSTR pName = MByteToWChar((LPCSTR)psDirEntry->name);
			_tcscpy(pNode->filename, pName);
			DeleteBuffer((PVOID)pName);
		}
		else
		{
			BYTE sum = DoChkSum(psDirEntry->name);
			if (m_bFind && sum == m_byChkSum)
			{
				m_bMatch = TRUE;
			}
		}	

		if (psDirEntry->attr & ATTR_DIRECTORY)
		{
			pNode->mode = MODE_DIR;
		}
		else
		{
			pNode->mode = MODE_FILE;
		}

		pNode->size = psDirEntry->size;
		pNode->win_data = psDirEntry->nt_reserved;
		pNode->cluster = psDirEntry->stclst_LO | (psDirEntry->stclst_HI << 16);
		pNode->disk_pos = *pPos;
	}	

	m_fLastEntryType = m_fCurrentEntryType;

	return err;
}

int CEFatfs::FillDirEntry(LPBYTE pData, LPBYTE pDirEntry)
{
	if (NULL == pData || NULL == pDirEntry)
	{
		return EN_INVAL;
	}
	
	if (DIR_SHORT == m_fCurrentEntryType)
	{
		PFAT_SHORT_DIR_ENTRY psDirEntry = (PFAT_SHORT_DIR_ENTRY)pDirEntry;
		
		GET_BYTS(pData, psDirEntry->name, 8, 0);
		GET_BYTS(pData, psDirEntry->ext, 3, 8);
		GET_BYTE(pData, psDirEntry->attr, 11);
		GET_BYTE(pData, psDirEntry->nt_reserved, 12);
		GET_BYTE(pData, psDirEntry->crt_sec_teenth, 13);
		GET_WORD(pData, psDirEntry->crt_time, 14);
		GET_WORD(pData, psDirEntry->crt_date, 16);
		GET_WORD(pData, psDirEntry->last_acc_date, 18);
		GET_WORD(pData, psDirEntry->stclst_HI, 20);
		GET_WORD(pData, psDirEntry->wrt_time, 22);
		GET_WORD(pData, psDirEntry->wrt_date, 24);
		GET_WORD(pData, psDirEntry->stclst_LO, 26);
		GET_DWORD(pData, psDirEntry->size, 28);	
	}
	else
	{
		PFAT_LONG_DIR_ENTRY plDirEntry = (PFAT_LONG_DIR_ENTRY)pDirEntry;

		GET_BYTE(pData, plDirEntry->order, 0);
		GET_BYTS(pData, plDirEntry->name1, 10, 1);
		GET_BYTE(pData, plDirEntry->attr, 11);
		GET_BYTE(pData, plDirEntry->type, 12);
		GET_BYTE(pData, plDirEntry->chksum, 13);
		GET_BYTS(pData, plDirEntry->name2, 12, 14);
		GET_WORD(pData, plDirEntry->cluL0, 26);
		GET_BYTS(pData, plDirEntry->name3, 4, 28);
	}

	return EN_OK;
}

int CEFatfs::GetFileName(PFAT_LONG_DIR_ENTRY plDirEntry)
{
	if (NULL == plDirEntry || 0 == m_dwLEntryCnt)
	{
		return EN_INVAL;
	}

	if ((plDirEntry->order & LAST_LONG_ENTRY) == LAST_LONG_ENTRY)
	{
		m_dwCurrentEntryCnt = plDirEntry->order & 0x0f;
		if (m_dwCurrentEntryCnt != m_dwLEntryCnt)
		{
			m_fEntryInval = TRUE;
			return EN_NOENT;
		}
		else
		{
			m_fEntryInval = FALSE;
			m_plName = new LENTRY_NAME[m_dwCurrentEntryCnt];
		}
	}

	if (m_fEntryInval != TRUE)
	{
		memcpy(m_plName[m_dwIndex].name1, plDirEntry->name1, sizeof(plDirEntry->name1));
		memcpy(m_plName[m_dwIndex].name2, plDirEntry->name2, sizeof(plDirEntry->name2));
		memcpy(m_plName[m_dwIndex].name3, plDirEntry->name3, sizeof(plDirEntry->name3));
		m_dwIndex++;		
	}	

	return EN_OK;
}

BYTE CEFatfs::DoChkSum(PBYTE pFcbName)
{
	int len = 0;
	BYTE sum = 0;

	for (len = 11; len > 0; len--)
	{
		sum = ((sum & 1) ? 0x80 : 0) + (sum >> 1) + *pFcbName++;
	}

	return sum;
}

BOOL CEFatfs::IsLegalShortNameChar(BYTE data)
{
	BYTE illegal[] = {0x22, 0x2a, 0x2b, 0x2c, 0x2e, 0x2f, 
					  0x3a,	0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
					  0x5b, 0x5c, 0x5d, 0x7c};
	int i = 0;
	int len = DIM(illegal);

	for (i = 0; i < len; i++)
	{
		if (illegal[i] == data)
		{
			break;
		}
	}

	return (i == len);
}

BOOL CEFatfs::NameConvert(LPTSTR plName, LPSTR psName)
{	
	LPSTR pName = NULL;
	BOOL bLossyConversion = 0;

	if (NULL == plName || NULL == psName)
	{
		return FALSE;
	}

	//convert to oem code 
	pName = WCharToMByte(plName);
	if (NULL == pName)
	{
		return FALSE;
	}
	
	int i = 0, j = 0;
	int len = strlen(pName);

	//get the basic name length and extend name
	for (i = len, j = 12; i > 0; i--, j--)
	{
		psName[j] = pName[i];
		if (pName[i] == '.')
		{
			len = i;
			break;
		}		
	}

	for (i = len - 1; i > 0; i--)
	{
		if (pName[i] != ' ')
		{
			len = i + 1;
			break;
		}
	}

	for (i = 0, j = 0; i < len; i++)
	{
		if (!IsLegalShortNameChar(pName[i]))
		{
			pName[i] = '_';
			bLossyConversion = TRUE;
		}

		if (pName[i] != '.' && pName[i] != ' ' && j < 8)
		{
			psName[j++] = pName[i];
		}

		if (j == 8) break;		
	}

	if (!bLossyConversion && j < 8 )
	{
		for (i = j; i < 8; i++)
		{
			psName[i] = ' ';
		}
	}
	else 
	{
		psName[6] = '~';
		psName[7] = '1';
	}

	return TRUE;
}

int CEFatfs::Get_NextFreeCluster(PFATFS_DATA_POS pPos)
{
	ASSERT(pPos != NULL);

	int err = 0;
	DWORD dwEntry = 0;
	DWORD dwClusterNum = pPos->dwClusterNum;
	DWORD dwClusterSeq = pPos->dwClusterSeq;
	DWORD dwOffsetCluster = pPos->dwOffsetCluster;

	BOOL bFind = 0;
	do 
	{
		err = Read_FatEntry(dwClusterNum, dwEntry);
		if (err != EN_OK) break;

		if (ENTRY_FREE == Get_FatEntryType(dwEntry))
		{
			bFind = TRUE;

			dwClusterNum = dwEntry;
			dwClusterSeq++;
			dwOffsetCluster = 0;

			//update fat chain
			Write_FatEntry(pPos->dwClusterNum, dwEntry);

		}
		else
		{
			dwClusterNum++;
		}

	} while (!bFind && m_vd.dwMaxCluster > dwClusterNum);
	

	pPos->dwClusterNum = dwClusterNum;
	pPos->dwClusterSeq = dwClusterSeq;
	pPos->dwOffsetCluster = dwOffsetCluster;

	return err;
}

int CEFatfs::Get_NextChainCluster(PFATFS_DATA_POS pPos)
{
	ASSERT(pPos != NULL);

	int err = 0;
	DWORD dwEntry = 0;
	DWORD dwClusterNum = pPos->dwClusterNum;
	DWORD dwClusterSeq = pPos->dwClusterSeq;
	DWORD dwOffsetCluster = pPos->dwOffsetCluster;

	err = Read_FatEntry(dwClusterNum, dwEntry);
	if (err != EN_OK) return err;

	switch (Get_FatEntryType(dwEntry))
	{
	case ENTRY_PNEXT:
		dwClusterNum = dwEntry;
		dwClusterSeq++;
		dwOffsetCluster = 0;
		break;

	case ENTRY_LAST:
		err = EN_EOF;
		break;

	default:
		break;
	}

	pPos->dwClusterNum = dwClusterNum;
	pPos->dwClusterSeq = dwClusterSeq;
	pPos->dwOffsetCluster = dwOffsetCluster;

	return err;
}


int CEFatfs::Get_FatEntryType(DWORD dwEntryVal)
{
	int type = 0;

	switch (m_vd.dwFatVersion)
	{
	case FAT_12:
		if (ISFREE_FAT12(dwEntryVal))
		{
			type = ENTRY_FREE;
		}
		else if (ISALLOC_FAT12(dwEntryVal))
		{
			type = ENTRY_PNEXT;
		}
		else if (ISRSV_FAT12(dwEntryVal))
		{
			type = ENTRY_RESV;
		}
		else if (ISBAD_FAT12(dwEntryVal))
		{
			type = ENTRY_BAD;
		}		
		else if (ISEND_FAT12(dwEntryVal))
		{
			type = ENTRY_LAST;
		}	
		break;

	case FAT_16:
		if (ISFREE_FAT16(dwEntryVal))
		{
			type = ENTRY_FREE;
		}
		else if (ISALLOC_FAT16(dwEntryVal))
		{
			type = ENTRY_PNEXT;
		}
		else if (ISRSV_FAT16(dwEntryVal))
		{
			type = ENTRY_RESV;
		}
		else if (ISBAD_FAT16(dwEntryVal))
		{
			type = ENTRY_BAD;
		}		
		else if (ISEND_FAT16(dwEntryVal))
		{
			type = ENTRY_LAST;
		}	
		break;

	case FAT_32:
		if (ISFREE_FAT32(dwEntryVal))
		{
			type = ENTRY_FREE;
		}
		else if (ISALLOC_FAT32(dwEntryVal))
		{
			type = ENTRY_PNEXT;
		}
		else if (ISRSV_FAT32(dwEntryVal))
		{
			type = ENTRY_RESV;
		}
		else if (ISBAD_FAT32(dwEntryVal))
		{
			type = ENTRY_BAD;
		}		
		else if (ISEND_FAT32(dwEntryVal))
		{
			type = ENTRY_LAST;
		}	
		break;

	default:		
		type = ENTRY_BAD;
	}

	return type;
}

int CEFatfs::WriteData(LPBYTE pBuf, DWORD &dwLen, PFATFS_DATA_POS pPos)
{
	DWORD dwSize = dwLen;
	DWORD dwSecNum = 0;
	DWORD dwPos = 0;
	DWORD dwOffset = 0;
	int err = 0;

	if (NULL == pBuf || NULL == pPos)
	{
		return EN_INVAL;
	}

	while (dwSize)
	{
		DWORD dwWriteLen = 0;

		//check if still inside current cluster
		if (pPos->dwOffsetCluster >= m_vd.dwBytesPerCluster)
		{
			err = Get_NextChainCluster(pPos);
			switch (err)
			{
			case EN_OK:
				break;

			case EN_EOF:
				//find new free cluster
				err = Get_NextFreeCluster(pPos);
				break;

			default:
				goto EXIT;
			}				
		}

		dwWriteLen = min(dwSize, m_vd.dwBytesPerCluster - pPos->dwOffsetCluster);

		err = WriteCluster(pBuf, dwWriteLen, pPos->dwClusterNum, pPos->dwOffsetCluster);
		if (err != EN_OK)	goto EXIT;

		pBuf += dwWriteLen;
		pPos->dwOffsetCluster += dwWriteLen;
		dwSize -= dwWriteLen;
	}

EXIT:
	dwLen -= dwSize;
	return err;
}


int CEFatfs::ReadData(LPBYTE pBuf, DWORD &dwLen, PFATFS_DATA_POS pPos)
{
	DWORD dwSize = dwLen;
	DWORD dwSecNum = 0;
	DWORD dwPos = 0;
	DWORD dwOffset = 0;
	int err = 0;

	if (NULL == pBuf || NULL == pPos)
	{
		return EN_INVAL;
	}

	while (dwSize)
	{
		DWORD dwReadLen = 0;

		//check if still inside current cluster
		if (pPos->dwOffsetCluster >= m_vd.dwBytesPerCluster)
		{
			err = Get_NextChainCluster(pPos);
			if (err != EN_OK) 
				goto EXIT;
		}

		dwReadLen = min(dwSize, m_vd.dwBytesPerCluster - pPos->dwOffsetCluster);

		err = ReadCluster(pBuf, dwReadLen, pPos->dwClusterNum, pPos->dwOffsetCluster);
		if (err != EN_OK)	goto EXIT;

		pBuf += dwReadLen;
		pPos->dwOffsetCluster += dwReadLen;
		dwSize -= dwReadLen;
	}

EXIT:
	dwLen -= dwSize;
	return err;
}

int CEFatfs::Write_FatEntry(DWORD dwClusterNum, DWORD dwEntryData)
{
	BYTE data[4] = {0};
	DWORD num = 0;
	DWORD dwSecNum = 0;
	DWORD dwSectorOffset = 0;	
	DWORD val = 0, dwLen = 0;
	int err = 0;

	switch (m_vd.dwFatVersion)
	{
	case FAT_12:
		dwLen = 2;
		num = dwClusterNum + (dwClusterNum / 2);
		dwSecNum = m_vd.dwReserveSectors + (num / m_vd.dwBytesPerSector);
		dwSectorOffset = (num % m_vd.dwBytesPerSector);
		err = ReadSector(dwSecNum, dwSectorOffset, data, dwLen);
		if (err != EN_OK) return err;

		GET_DWORD(data, val, 0);
		if (0 == (dwClusterNum & 0x0001))
		{
			//cluster number is even			
			val = (val & 0xFFFFF000) | (dwEntryData & 0x00000FFF);
		} 
		else 
		{
			//cluster number is odd
			val = (val & 0xFFFF000F) | (dwEntryData << 4);				 
		}
		GET_BYTS(data, &val, 4, 0);
		err = WriteSector(dwSecNum, dwSectorOffset, data, 4);
		break;

	case FAT_16:
		dwLen = 2;
		num = dwClusterNum * 2;
		dwSecNum = m_vd.dwReserveSectors + (num / m_vd.dwBytesPerSector);
		dwSectorOffset = (num % m_vd.dwBytesPerSector);
		err = ReadSector(dwSecNum, dwSectorOffset, data, dwLen);
		if (err != EN_OK) return err;

		GET_DWORD(data, val, 0);
		val = (val & 0xFFFF0000) | (dwEntryData & 0x0000FFFF);
		GET_BYTS(data, &val, 4, 0);
		err = WriteSector(dwSecNum, dwSectorOffset, data, 4);
		break;

	case FAT_32:
		dwLen = 4;
		num = dwClusterNum * 4;
		dwSecNum = m_vd.dwReserveSectors + (num / m_vd.dwBytesPerSector);
		dwSectorOffset = (num % m_vd.dwBytesPerSector);
		err = ReadSector(dwSecNum, dwSectorOffset, data, dwLen);
		if (err != EN_OK) return err;

		GET_DWORD(data, val, 0);
		val = (val & 0xF0000000) | (dwEntryData & 0x0FFFFFFF);
		GET_BYTS(data, &val, 4, 0);
		err = WriteSector(dwSecNum, dwSectorOffset, data, 4);
		break;

	default:		
		break;
	}

	return err;
}

int CEFatfs::Read_FatEntry(DWORD dwClusterNum, DWORD &dwEntryData)
{
	BYTE data[4] = {0};
	DWORD num = 0;
	DWORD dwSecNum = 0;
	DWORD dwSectorOffset = 0;	
	DWORD val = 0, dwLen = 0;
	int err = 0;
	
	switch (m_vd.dwFatVersion)
	{
	case FAT_12:
		dwLen = 2;
		num = dwClusterNum + (dwClusterNum / 2);
		dwSecNum = m_vd.dwReserveSectors + (num / m_vd.dwBytesPerSector);
		dwSectorOffset = (num % m_vd.dwBytesPerSector);
		err = ReadSector(dwSecNum, dwSectorOffset, data, dwLen);
		if (err != EN_OK) return err;

		GET_WORD(data, val, 0);
		if (0 == (dwClusterNum & 0x0001))
		{
			//cluster number is even
			dwEntryData = val & 0x0FFF;
		} 
		else 
		{
			//cluster number is odd
			dwEntryData = (val >> 4) & 0x0FFF;				 
		}
		break;

	case FAT_16:
		dwLen = 2;
		num = dwClusterNum * 2;
		dwSecNum = m_vd.dwReserveSectors + (num / m_vd.dwBytesPerSector);
		dwSectorOffset = (num % m_vd.dwBytesPerSector);
		err = ReadSector(dwSecNum, dwSectorOffset, data, dwLen);
		if (err != EN_OK) return err;

		GET_WORD(data, val, 0);
		dwEntryData = val;
		break;

	case FAT_32:
		dwLen = 4;
		num = dwClusterNum * 4;
		dwSecNum = m_vd.dwReserveSectors + (num / m_vd.dwBytesPerSector);
		dwSectorOffset = (num % m_vd.dwBytesPerSector);
		err = ReadSector(dwSecNum, dwSectorOffset, data, dwLen);
		if (err != EN_OK) return err;

		GET_DWORD(data, val, 0);
		dwEntryData = val & 0x0FFFFFFF;
		break;

	default:		
		break;
	}

	return err;
}

int CEFatfs::ReadCluster(LPBYTE pBuf, 
						DWORD dwLen,
						DWORD dwCluster,
						DWORD dwOffsetClst)
{
	DWORD dwSecNum = 0;
	DWORD dwPos = 0;
	DWORD dwOffset = 0;

	if (NULL == pBuf) return EN_INVAL;
	
	dwSecNum =(dwCluster - 2) * m_vd.dwSectorsPerCluster + m_vd.dwStartDataSector;
	dwPos = dwOffsetClst / m_vd.dwBytesPerSector;
	dwOffset = dwOffsetClst % m_vd.dwBytesPerSector;
	dwSecNum += dwPos;

	return ReadSector(dwSecNum, dwOffset, pBuf, dwLen);
}

int CEFatfs::ReadSector(DWORD dwSecNum,
						DWORD dwOffset, 
						PBYTE pData, 
						DWORD dwLen)
{
	PBYTE pBuf = NULL;
	DWORD dwSectors = 0;
	DWORD dwCopy = 0;
	int err = 0;

	if (NULL == pData) return EN_INVAL;
	
	pBuf = new BYTE[m_vd.dwBytesPerSector];
	if (NULL == pBuf)
	{
		TRACE(_T("Error: out of memory\r\n"));
		return EN_IO;
	}

	dwSectors = (dwLen / m_vd.dwBytesPerSector) + 2;

	for (DWORD i = 0; i < dwSectors; i++)
	{
		if (!m_disk.ReadSectors(dwSecNum + i, pBuf, 1))
		{
			TRACE(_T("Error: read disk eror\r\n"));
			err = EN_IO;
			goto EXIT;
		}

		if (0 == i)
		{			
			dwCopy = min(dwLen, m_vd.dwBytesPerSector - dwOffset); 
			memcpy(pData, pBuf + dwOffset, dwCopy);
		}
		else
		{
			dwCopy = min(dwLen, m_vd.dwBytesPerSector);
			memcpy(pData, pBuf, dwCopy);
		}

		pData += dwCopy;
		dwLen -= dwCopy;

		if (0 == dwLen) break;		
	}

EXIT:
	if (pBuf)
	{
		delete []pBuf;
		pBuf = NULL;
	}

	return err;
}


int CEFatfs::WriteCluster(LPBYTE pBuf, 
					    DWORD dwLen,
					    DWORD dwCluster,
					    DWORD dwOffsetClst)
{
	DWORD dwSecNum = 0;
	DWORD dwPos = 0;
	DWORD dwOffset = 0;

	if (NULL == pBuf) return EN_INVAL;
	
	dwSecNum =(dwCluster - 2) * m_vd.dwSectorsPerCluster + m_vd.dwStartDataSector;
	dwPos = dwOffsetClst / m_vd.dwBytesPerSector;
	dwOffset = dwOffsetClst % m_vd.dwBytesPerSector;
	dwSecNum += dwPos;

	return WriteSector(dwSecNum, dwOffset, pBuf, dwLen);
}

int CEFatfs::WriteSector(DWORD dwSecNum,
						DWORD dwOffset, 
						PBYTE pData, 
						DWORD dwLen)
{
	PBYTE pBuf = NULL;
	DWORD dwSectors = 0;
	DWORD dwCopy = 0;
	DWORD dwWrite = 0;
	int err = 0;

	if (NULL == pData) return EN_INVAL;

	pBuf = new BYTE[m_vd.dwBytesPerSector];
	if (NULL == pBuf)
	{
		TRACE(_T("WriteSector Error: out of memory\r\n"));
		return EN_IO;
	}

	dwSectors = (dwLen / m_vd.dwBytesPerSector) + 2;
	for (DWORD i = 0; i < dwSectors; i++)
	{
		memset(pBuf, 0, m_vd.dwBytesPerSector);

		//read sector data
		if (!m_disk.ReadSectors(dwSecNum + i, pBuf, 1))
		{
			TRACE(_T("WriteSector Error: read disk eror\r\n"));
			err = EN_IO;
			goto EXIT;
		}

		//update sector data
		if (0 == i)
		{
			dwCopy = min(dwLen, m_vd.dwBytesPerSector - dwOffset);
			memcpy(pBuf + dwOffset, pData, dwCopy);
		}
		else
		{
			dwCopy = min(dwLen, m_vd.dwBytesPerSector);
			memcpy(pBuf, pData + dwWrite, dwCopy);
		}		

		//write back the sector data
		if (!m_disk.WriteSectors(dwSecNum + i, pBuf, 1))
		{
			TRACE(_T("WriteSector Error: write disk eror\r\n"));
			err = EN_IO;
			goto EXIT;
		}

		dwLen -= dwCopy;
		dwWrite += dwCopy;

		if (0 == dwLen) break;
		
	}

	err = EN_OK;

EXIT:
	return err;
}

