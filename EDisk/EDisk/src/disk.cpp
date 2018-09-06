#include "stdafx.h"
#include "disk.h"
#include "pkfuncs.h"


CEDiskInterface::CEDiskInterface()
{
	m_hVolume = NULL;
	m_dwBytesPerSector = 0;
}

CEDiskInterface::~CEDiskInterface()
{
	
}

BOOL CEDiskInterface::GetDiskInfo(PDISK_INFO pdi)
{
	DWORD dwBytes = 0;

	return DeviceIoControl(m_hVolume, DISK_IOCTL_GETINFO, pdi, sizeof(DISK_INFO), 
		pdi, sizeof(DISK_INFO), &dwBytes, NULL);
}

BOOL CEDiskInterface::WriteSectors(DWORD dwSector, LPBYTE lpBuf, DWORD dwNumSectors)
{
	SG_REQ sg;
	BOOL bRet;

	sg.sr_start = dwSector;
	sg.sr_num_sec = dwNumSectors; 
	sg.sr_num_sg = 1;
	sg.sr_status = ERROR_NOT_SUPPORTED;  // not used by ATADisk
	sg.sr_callback = NULL;
	sg.sr_sglist[0].sb_buf = lpBuf;
	sg.sr_sglist[0].sb_len = m_dwBytesPerSector * dwNumSectors;

	ASSERT(m_hVolume != NULL);
	bRet = DeviceIoControl(m_hVolume, DISK_IOCTL_WRITE, &sg, sizeof(sg), 
		NULL, 0, NULL, NULL);

	if (!bRet) 
	{
		TRACE(_T("Write Sector failed (%d) when writing to Sector %d\r\n"), sg.sr_status, dwSector);
		SetLastError(ERROR_WRITE_FAULT);
	}

	return bRet;
}

BOOL CEDiskInterface::ReadSectors(DWORD dwSector, LPBYTE lpBuf, DWORD dwNumSectors)
{
	SG_REQ sg = {0};
	BOOL bRet = 0;	
	DWORD dwLen = 0;
	
	sg.sr_start = dwSector;
	sg.sr_num_sec = dwNumSectors; 
	sg.sr_num_sg = 1;
	sg.sr_status = ERROR_NOT_SUPPORTED;  // not used by ATADisk
	sg.sr_callback = NULL;
	sg.sr_sglist[0].sb_buf = lpBuf;
	sg.sr_sglist[0].sb_len =  m_dwBytesPerSector * dwNumSectors;

	ASSERT(m_hVolume != NULL);
	bRet = DeviceIoControl(m_hVolume, DISK_IOCTL_READ, &sg, sizeof(sg),
		NULL, 0, NULL, NULL);

	if (!bRet)
	{		
		TRACE(_T("Read Sector failed (%d) when reading from Sector %d error=%d\r\n"), sg.sr_status, dwSector, GetLastError());
		SetLastError(ERROR_READ_FAULT);
	}
	
	return bRet;
}

BOOL CEDiskInterface::WriteCluster(DWORD dwCluster, LPBYTE lpBuf)
{
	if (!m_pvd)
		return FALSE;

	// Determine the sector number where first cluster starts
	DWORD dwFirstSectorOfCluster = ((dwCluster - 2) * m_pvd->dwSectorsPerCluster) +
		m_pvd->dwStartDataSector;

	return WriteSectors (dwFirstSectorOfCluster, lpBuf, m_pvd->dwSectorsPerCluster);    
}

BOOL CEDiskInterface::ReadCluster(DWORD dwCluster, LPBYTE lpBuf)
{
	if (!m_pvd)
		return FALSE;

	// Determine the sector number where first cluster starts
	DWORD dwFirstSectorOfCluster = ((dwCluster - 2) * m_pvd->dwSectorsPerCluster) +
		m_pvd->dwStartDataSector;

	return ReadSectors (dwFirstSectorOfCluster, lpBuf, m_pvd->dwSectorsPerCluster);    
}

