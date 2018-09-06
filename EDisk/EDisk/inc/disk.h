#ifndef _EDISK_H_
#define _EDISK_H_

#include "diskio.h"

#define DEFAULT_VOLUME_NAME TEXT("DSK1:")

#define OEMNAMESIZE             11
#define DEFAULT_SECTOR_SIZE     512
#define MEDIA_HD                0xF8    // hard disk
#define PART_BOOTABLE           0x80    // bootable partition
#define FAT32_CURR_FS_VERSION   0x0000

#define FREE_CLUSTER            0
#define DATA_CLUSTER            2
#define EOF_CLUSTER             ((unsigned)-1)
#define UNKNOWN_CLUSTER         EOF_CLUSTER
#define ERROR_CLUSTER           ((unsigned)-2)
#define FAT32_EOF_MIN           0x0FFFFFF8
#define FAT32_EOF_MAX           0x0FFFFFFF
#define MAX_CLUSTER_SIZE        32 * 1024

#define DIRENTRY_SIZE						32
#define FREE_DIRENTRY           0xE5    // stored in de_name[0] to indicate free slot
#define END_DIRENTRY            0       // stored in de_name[0] to indicate no more entries
#define MAX_DIRENTRIES          65536   // max directory entries MS-DOS can handle

typedef struct _VOLUME_DATA {
	DWORD   dwFatVersion;
	DWORD   dwDataSectors;
	DWORD   dwNumClusters;
	DWORD   dwMaxCluster;
	DWORD   dwRootEntries;
	DWORD		dwReserveSectors;
	DWORD		dwTotalSectors;
	DWORD   dwStartFatSector;
	DWORD   dwStartDataSector;
	DWORD   dwStartRootDirSector;
	DWORD   dwRootDirSectors;
	DWORD   dwBytesPerSector;
	DWORD   dwSectorsPerCluster;
	DWORD   dwBytesPerCluster;
	DWORD   dwSectorsPerFat;
	DWORD   dwNumFats;
	DWORD   dwRootCluster;  
	DWORD   dwFatType;
	DWORD   dwVolumeFlags;
} VOLUME_DATA, *PVOLUME_DATA;

// Values of dwFatType field in VOLUME_DATA
#define FATTYPE_NORMAL            0  // Volume is not transaction safe
#define FATTYPE_TFAT_FAT0         1  // Volume is transaction safe and FAT0 is last known good
#define FATTYPE_TFAT_FAT1         2  // Volume is transaction safe and FAT1 is last known good

class CEDiskInterface
{
public:
	CEDiskInterface();
	virtual ~CEDiskInterface();

	BOOL WriteSectors (DWORD dwSector, LPBYTE lpBuf, DWORD dwNumSectors);
	BOOL ReadSectors (DWORD dwSector, LPBYTE lpBuf, DWORD dwNumSectors);
	BOOL WriteCluster (DWORD dwCluster, LPBYTE lpBuf);
	BOOL ReadCluster (DWORD dwCluster, LPBYTE lpBuf);
	BOOL GetDiskInfo(PDISK_INFO pdi);

	__inline void Init(HANDLE hVolume)
	{
		m_hVolume = hVolume;			
	}

	__inline void SetParameter(DWORD dwBytesPerSector, PVOLUME_DATA pVD = NULL)
	{
		m_dwBytesPerSector = dwBytesPerSector;
		m_pvd = pVD;
	}

protected:

private:
	HANDLE          m_hVolume;
	DWORD           m_dwBytesPerSector;
	PVOLUME_DATA    m_pvd;
};














#endif //_EDISK_H_