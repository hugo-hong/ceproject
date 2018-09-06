#pragma once

#include "disk.h"

enum
{
	FAT_12 = 12,
	FAT_16 = 16,
	FAT_32 = 32,
};

enum
{
	FAT12_MAX_CLST_NUM = 4085,
	FAT16_MAX_CLST_NUM = 65525,
	FAT32_MAX_CLST_NUM = 0XFFFFFFFF
};

enum
{
	MODE_DIR,
	MODE_FILE,
};

enum
{
	DIR_SHORT,
	DIR_LONG,
	DIR_UNKNOW = 0XFF,
};

enum
{
	ENTRY_PNEXT,//used,and entry point to next file cluster
	ENTRY_FREE,//free cluster
	ENTRY_LAST,//last cluster of file
	ENTRY_RESV,//reserved cluster
	ENTRY_BAD //bad cluster
};

enum
{
	EN_OK,//NO ERROR
	EN_NOENT,//NO SUCH ENTITY
	EN_IO, //I/O ERROR
	EN_INVAL, //INVALID ARGUMENT
	EN_EOF, //END OF FILE
};
enum
{
	ATTR_READ_ONLY = 0X01,
	ATTR_HIDDEN = 0X02,
	ATTR_SYSTEM = 0X04,
	ATTR_VOLUME_ID = 0X08,
	ATTR_DIRECTORY = 0X10,
	ATTR_ARCHIVE = 0X20,
	ATTR_LONG_NAME = 0X0F,
	ATTR_LONG_NAME_MASK = 0X3F
};
 
#define LAST_LONG_ENTRY						0x40
#define MAX_LDIRENTRY_NAME				13

///due to FAT spec, decide the cluster type
#define ISFREE_FAT12(dwCluster)		(0x000 == dwCluster)		
#define ISBAD_FAT12(dwCluster)		(0XFF7 == dwCluster)
#define ISALLOC_FAT12(dwCluster)	(dwCluster >= 0x002 && dwCluster <= 0xFEF)
#define ISRSV_FAT12(dwCluster)		(dwCluster >= 0xFF0 && dwCluster <= 0xFF6)
#define ISEND_FAT12(dwCluster)		(dwCluster >= 0xFF8 && dwCluster <= 0xFFF)

#define ISFREE_FAT16(dwCluster)		(0x0000 == dwCluster)		
#define ISBAD_FAT16(dwCluster)		(0xFFF7 == dwCluster)
#define ISALLOC_FAT16(dwCluster)	(dwCluster >= 0x0002 && dwCluster <= 0xFFEF)
#define ISRSV_FAT16(dwCluster)		(dwCluster >= 0xFFF0 && dwCluster <= 0xFFF6)
#define ISEND_FAT16(dwCluster)		(dwCluster >= 0xFFF8 && dwCluster <= 0xFFFF)

#define ISFREE_FAT32(dwCluster)		(0x0000000 == dwCluster)		
#define ISBAD_FAT32(dwCluster)		(0xFFFFFF7 == dwCluster)
#define ISALLOC_FAT32(dwCluster)	(dwCluster >= 0x0000002 && dwCluster <= 0xFFFFFEF)
#define ISRSV_FAT32(dwCluster)		(dwCluster >= 0xFFFFFF0 && dwCluster <= 0xFFFFFF6)
#define ISEND_FAT32(dwCluster)		(dwCluster >= 0xFFFFFF8 && dwCluster <= 0xFFFFFFF)
/////////////////////////////////////////////////////////////////////////////////////////////////
//fatfs struct declare

typedef struct
{
	DWORD dwClusterNum;//cluster number
	DWORD dwClusterSeq;//cluster sequence of file
	DWORD dwOffsetCluster; //position inside cluster
}FATFS_DATA_POS, *PFATFS_DATA_POS;

typedef struct
{
	TCHAR filename[32 + 1];//file name
	DWORD mode; //mode 
	DWORD size; //file size in bytes
	BYTE  win_data; //reserve for winNT
	DWORD cluster; //first cluster number
	DWORD parent_cluster;//first cluster of parent dentry
	FATFS_DATA_POS disk_pos; //position of dir entry on disk
}FATFS_DIR_ENTRY, *PFATFS_DIR_ENTRY;

typedef struct 
{	
	FATFS_DIR_ENTRY		dir;      // Directory to search
	const TCHAR       *path;     // Path to follow
	FATFS_DIR_ENTRY		node;     // Node found
	const TCHAR       *name;     // Last name fragment used
	int               len;			// Name fragment length
	BOOL              bLast;     // Last name in path?
	BOOL              bFind;     // the path find?
} FATFS_DIR_SEARCH, *PFATFS_DIR_SEARCH;

typedef struct
{
	FATFS_DIR_ENTRY node;
	FATFS_DATA_POS  pos;
}FATFS_FD_ENTRY, *PFATFS_FD_ENTRY;

typedef struct
{
	DWORD dwOffset; //current offset of file
	FATFS_FD_ENTRY fd;
}FATFS_FILE_HANDLE, *PFATFS_FILE_HANDLE;


typedef struct 
{
	BYTE  name[8];   // 00h : Name
	BYTE  ext[3];    // 08h : Extension
	BYTE  attr;        // 0Bh : Attribute
	BYTE  nt_reserved; // 0Ch : Win NT Reserved field
	BYTE  crt_sec_teenth; // 0Dh : Creation time ms stamp 0 - 199
	WORD 	crt_time;    // 0Eh : Creation time
	WORD 	crt_date;    // 10h : Creation date
	WORD 	last_acc_date;    // 12h : Last access date
	WORD 	stclst_HI;  // 14h : Starting cluster HI WORD (FAT32)
	WORD 	wrt_time;    // 16h : Time
	WORD 	wrt_date;    // 18h : Date
	WORD 	stclst_LO;     // 1Ah : Starting cluster
	DWORD size;        // 1Ch : Size of the file
} FAT_SHORT_DIR_ENTRY, *PFAT_SHORT_DIR_ENTRY;

typedef struct
{
	BYTE order; //00h:long directory entry sequence
	BYTE name1[10]; //01h:long dir name1(1 - 5), unicode
	BYTE attr;		//0bh:attribute
	BYTE type;		//0ch:if 0, long dir sub entry, else other dir type
	BYTE chksum;	//0dh:short dir check sum
	BYTE name2[12];//0eh:long dir name2(6 - 11)
	WORD cluL0;		//1ah: 0
	BYTE name3[4]; //1ch:long dir name3(12 - 13)
}FAT_LONG_DIR_ENTRY, *PFAT_LONG_DIR_ENTRY;

typedef struct
{
	TCHAR name1[5];
	TCHAR name2[6];
	TCHAR name3[2];
}LENTRY_NAME, *PLENTRY_NAME;

/////////////////////////////////////////////////////////////////////////////////////////////////
//fatfs class declare

class CEFatfs
{
public:
	CEFatfs();
	virtual ~CEFatfs();

	BOOL InitDiskInterface(HANDLE hPart);

	HANDLE OpenPart(LPCTSTR pszDisk, LPCTSTR pszPart);	

	BOOL GetPartData(PVOLUME_DATA pVd);

	BOOL GetDiskInfo(PDISK_INFO pdi);

	BOOL GetBootSectorData(PBYTE pBootSector);

	BOOL ReadSectors(DWORD dwStart, PBYTE pData, DWORD dwNum);

	BOOL ParseFatInfo(PBYTE pBootSectorData);

	HANDLE CreateFile(LPTSTR pszFileName);	

	BOOL CloseFile(HANDLE hFile);

	DWORD ReadFile(HANDLE hFile, LPBYTE pBuf, DWORD dwLen);

	DWORD WriteFile(HANDLE hFile, LPBYTE pBuf, DWORD dwLen);

	BOOL SeekFile(HANDLE hFile, DWORD dwOffset);	

	BOOL CreateDirectory(LPTSTR pszDirName);

protected:		

	BOOL InitRootDirEntry(PFATFS_DIR_ENTRY pDirEntry);

	int InitPos(PFATFS_DIR_ENTRY pNode, PFATFS_DATA_POS pPos);

	int Find(PFATFS_DIR_SEARCH pDs);

	int Find_DEntry(PFATFS_DIR_SEARCH pDs);

	int Find_DirEntry(PFATFS_DIR_SEARCH pDs);

	int Read_DEntry(PFATFS_DIR_ENTRY pDir, 
									PFATFS_DATA_POS pPos, 
									PFATFS_DIR_ENTRY pNode);

	int Read_NextDEntry(PFATFS_DATA_POS pPos, PBYTE pDirEntry);

	int ReadDirEntry(PFATFS_DATA_POS pPos, PBYTE pDirEntry);

	int Read_FatEntry(DWORD dwClusterNum, DWORD &dwEntryData);

	int Write_FatEntry(DWORD dwClusterNum, DWORD dwEntryData);

	int Get_FatEntryType(DWORD dwEntryVal);

	int Get_NextChainCluster(PFATFS_DATA_POS pos);	

	int Get_NextFreeCluster(PFATFS_DATA_POS pos);	

	int ReadCluster(LPBYTE pBuf, 
									DWORD dwLen,
									DWORD dwCluster,
									DWORD dwOffsetClst);

	int ReadSector(DWORD dwSecNum,
								DWORD dwOffset, 
								PBYTE pData, 
								DWORD dwLen);

	int WriteCluster(LPBYTE pBuf, 
									DWORD dwLen,
									DWORD dwCluster,
									DWORD dwOffsetClst);

	int WriteSector(DWORD dwSecNum,
									DWORD dwOffset, 
									PBYTE pData, 
									DWORD dwLen);

	int Raw2Entry(PBYTE pRawEntry, 
								PFATFS_DATA_POS pPos,
								PFATFS_DIR_ENTRY pNode);	

	int GetFileName(PFAT_LONG_DIR_ENTRY plDirEntry);

	int ReadData(LPBYTE pBuf, DWORD &dwLen, PFATFS_DATA_POS pPos);

	int WriteData(LPBYTE pBuf, DWORD &dwLen, PFATFS_DATA_POS pPos);

	int FillDirEntry(LPBYTE pData, LPBYTE pDirEntry);

	BYTE DoChkSum(PBYTE pFcbName);

	HANDLE DoCreateFile(PFATFS_DIR_SEARCH pDs);

	int CreateDirEntry(PFATFS_DATA_POS pPos, PBYTE pDirEntry);

	BOOL NameConvert(LPTSTR plName, LPSTR psName);

	BOOL IsLegalShortNameChar(BYTE data);

	
private:
	CEDiskInterface m_disk;
	VOLUME_DATA m_vd;
	DWORD m_fEntryInval;
	DWORD m_fLastEntryType;
	DWORD m_fCurrentEntryType;
	DWORD m_dwLEntryCnt;
	DWORD m_dwCurrentEntryCnt;
	DWORD m_dwIndex;
	BYTE m_byChkSum;
	BOOL m_bFind;
	BOOL m_bMatch;
	PLENTRY_NAME m_plName;

};


