

#ifndef _METAZONEEX_H_
#define _METAZONEEX_H_

//#include "x_typedef.h"

#define MZ_SUCCESS  0x00000000
#define MZ_FAILURE  0x80000000
#define MZ_NO_INIT  0x80000001

#define MZ_RD_ONLY_IDX_START  0x0000
#define MZ_RD_ONLY_DWORD_NUM   0
#define MZ_RD_ONLY_BINARY_NUM  0
#define MZ_BINARY_MAX_SIZE     100 // Size in bytes

#define MZ_WR_IDX_START  0x10000
#define MZ_WR_DWORD_NUM   200
#define MZ_WR_BINARY_NUM  10



#define MZ_FS_IDX_START  0x20000
#define MZ_FS_DWORD_NUM   2000
#define MZ_FS_BINARY_NUM  500
#define MZ_FS_BINARY_SIZE  100

#define MZ_MSDK_START  0x20000  
#define MZ_MSDK_END    0x20

#define MZ_WC_DW_IDX_START  (MZ_WR_IDX_START)  // Recording writing code of metazone 

// Backlight index
#define MZ_BACKLIGHT_DW_IDX_START  (MZ_WR_IDX_START + 1)  // Backlight setting . 


// Index for BT Transport Layer Driver
#define MZ_BT_ADDR         MZ_WR_IDX_START // Index for BT Address in binary data
#define MZ_BT_ADDR_DONE   (MZ_WR_IDX_START + 8)
#define MZ_BT_STATUS      (MZ_FS_IDX_START + 8)


// Index for Touch panel
#define MZ_TP_DW_IDX_START  (MZ_WR_IDX_START + 10)
#define MZ_TP_DW_IDX_PRIORITY     (MZ_TP_DW_IDX_START+1)
#define MZ_TP_DW_IDX_MAXERROR     (MZ_TP_DW_IDX_START+2)
#define MZ_TP_DW_IDX_SPL          (MZ_TP_DW_IDX_START+3)
#define MZ_TP_DW_IDX_RISTHRESHOLE (MZ_TP_DW_IDX_START+4)
#define MZ_TP_DW_IDX_END    (MZ_WR_IDX_START + 18)

#define MZ_TP_BIN_IDX_START (MZ_WR_IDX_START + 1)
#define MZ_TP_BIN_IDX_CALIBRATION (MZ_TP_BIN_IDX_START)
#define MZ_TP_BIN_IDX_END   (MZ_WR_IDX_START + 1)



// Index for USB Driver.
#define MZ_USB_DW_IDX_START  (MZ_FS_IDX_START + 2)  // Usb setting


// Index for SDK & MSDK

#define MZ_SDK_DW_DEF_IDX_START  (MZ_WR_IDX_START + 200) // 0x10000 + 200
#define MZ_SDK_DW_DEF_IDX_END    (MZ_WR_IDX_START + 499) // 0x10000 + 499

#define MZ_SDK_DW_IDX_START      (MZ_FS_IDX_START + 100) // 0x20000 + 100
#define MZ_SDK_DW_IDX_END        (MZ_FS_IDX_START + 399) // 0x20000 + 399

#define MZ_SDK_BIN_DEF_IDX_START  (MZ_WR_IDX_START + 10) // 0x10000 + 10
#define MZ_SDK_BIN_DEF_IDX_END    (MZ_WR_IDX_START + 19) // 0x10000 + 19

#define MZ_SDK_BIN_IDX_START     (MZ_FS_IDX_START + 10) // 0x20000 + 10
#define MZ_SDK_BIN_IDX_END       (MZ_FS_IDX_START + 19) // 0x20000 + 19


typedef struct _METAZONE_INFO_T {
    UINT32       u4RdValueNum;     // Max number of dword data in read only section.
    UINT32       u4RdBinaryNum;    // Max number of binary data in read only section.
    UINT32       u4RdBinarySize;   // Max size(in bytes) of binary data in read only section.
    UINT32       u4WrValueNum;     // Max number of dword data in writable section.
    UINT32       u4WrBinaryNum;    // Max number of binary data in writable section.
    UINT32       u4WrBinarySize;   // Max size(in bytes) of binary data.in writable section.
    UINT32       u4FsValueNum;     // Max number of dword data in file section.
    UINT32       u4FsBinaryNum;    // Max number of binary data in file section.
    UINT32       u4FsBinarySize;   // Max size(in bytes) of binary data in file section.
} METAZONE_INFO_T, *PMETAZONE_INFO_T; 


#if __cplusplus
extern "C" {
#endif

/**
*	@brief  Initialize MetaZone API.  Please call this function first before calling other functions.
*	           
*      @param  
*      @param  
*
*
*	@return	   return MZ_SUCCESS if succeeds or MZ_FAILURE if failure.
*			   
*
*	@see	 
*     @note   
**/

UINT32  MetaZone_Init(void);

/**
*	@brief  De-initialize MetaZone API. Call this function to release resource of MetaZone API.
*	           
*      @param  
*      @param  
*
*
*	@return	   return MZ_SUCCESS if succeeds or MZ_FAILURE if failure.
*			   
*
*	@see	 
*     @note   
**/

UINT32  MetaZone_Deinit(void);

/**
*	@brief  Read MetaZone configuration inforation..
*	           
*      @param  prInfo -- Pointer to structure METAZONE_INFO_T to receive data
*      @param  
*
*
*	@return	   return MZ_SUCCESS if succeeds or MZ_FAILURE if failure.
*			   
*
*	@see	 
*     @note   
**/
UINT32  MetaZone_ReadInfo(METAZONE_INFO_T *prInfo);


/**
*	@brief  Read a DWORD value of given index.
*	           
*      @param u4Idx -- index of DWORD to be read. 
*      @param  pu4Data -- Pointer to receive the value.
*
*
*	@return	   return 4 ( 4 bytes, a dword) if succeeds or 0 if failure.
*			   
*
*	@see	 
*   @note  Index of Read only section from 0x0000 to u4RdValueNum -1. The  u4RdValueNum is return by MetaZone_ReadInfo.
*          Index of writable section from 0x10000 to 0x10000 + u4WrValueNum -1. The u4WrValueNum return by MetaZone_ReadInfo.
*          Index of file section from 0x20000 to 0x20000 + u4FsValueNum -1 . The u4FsValueNum return by MetaZone_ReadInfo.
**/

UINT32  MetaZone_Read(UINT32 u4Idx, UINT32 *pu4Data);

/**
*	@brief  Write a DWORD value of given index.
*	           
*      @param u4Idx -- index of DWORD to be written.
*      @param  u4Data -- Data to be written.
*
*
*	@return	   return MZ_SUCCESS if succeeds or other values if failure.
*			   
*
*	@see	 
*   @note  Index of Read only section from 0x0000 to u4RdValueNum -1. The  u4RdValueNum is return by MetaZone_ReadInfo.
*          Index of writable section from 0x10000 to 0x10000 + u4WrValueNum -1. The u4WrValueNum return by MetaZone_ReadInfo.
*          Index of file section from 0x20000 to 0x20000 + u4FsValueNum -1 . The u4FsValueNum return by MetaZone_ReadInfo.
**/

UINT32  MetaZone_Write(UINT32 u4Idx, UINT32 u4Data);

/**
*	@brief  Read binary value of given index.
*	           
*      @param u4Idx   -- index of binary data. 
*      @param  pbData -- buffer to receive data.
*      @param  u4Size -- size of buffer (in bytes).
*
*
*	@return	 reture the read size of binary data if succeeds or return value large than 0x8000000.
*			   
*
*	@see	 
*   @note  Index of Read only section from 0x0000 to u4RdBinarySize -1. The u4RdBinarySize is return by MetaZone_ReadInfo.
*          Max size of binary data in read only section is specified by u4RdBinarySize which is return by MetaZone_ReadInfo.
*          Index of writable section from 0x10000 to 0x10000 + u4WrBinaryNum -1. The u4WrBinaryNum return by MetaZone_ReadInfo.
*          Max size of binary data in writable section is specified by u4WrBinarySize which is return by MetaZone_ReadInfo.
*          Index of file section from 0x20000 to 0x20000 + u4FsBinaryNum -1 . The u4FsBinaryNum return by MetaZone_ReadInfo.
*          Max size of binary data in file section is specified by u4FsBinarySize which is return by MetaZone_ReadInfo.
**/

UINT32  MetaZone_ReadBinary(UINT32 u4Idx, BYTE *pbData, UINT32 u4Size);

/**
*	 @brief  Write binary value of given index.
*	           
*      @param u4Idx   -- index of binary data. 
*      @param  pbData -- pointer to data to be written..
*      @param  u4Size -- size of data (in bytes).
*
*
*	@return	   return MZ_SUCCESS if succeeds or other values if failure.
*			   
*
*	@see	 
*   @note  Index of Read only section from 0x0000 to u4RdValueNum -1. The  u4RdValueNum is return by MetaZone_ReadInfo.
*          Index of writable section from 0x10000 to 0x10000 + u4WrValueNum -1. The u4WrValueNum return by MetaZone_ReadInfo.
*          Index of file section from 0x20000 to 0x20000 + u4FsValueNum -1 . The u4FsValueNum return by MetaZone_ReadInfo.
**/
UINT32  MetaZone_WriteBinary(UINT32 u4Idx, BYTE *pbData, UINT32 u4Size);

/**
*	@brief  Read all reserved data.
*      @param  pbData -- Buffer to received data.
*      @param  u4Size -- size of buffer (in bytes). 
*	           
*	@return	 reture the read size of reserved data if succeeds or return value large than 0x8000000.
*			   
*
*	@see	 
*   @note  Reserved data in writable section is reserved for customer.
**/

UINT32  MetaZone_ReadReserved(BYTE *pbData, UINT32 u4Size);

/**
*	@brief  Write reserved data.
*      
*      @param  pbData -- Buffer to reserved data.
*      @param  u4Size -- size of data (in bytes). 
*	           
*	@return	 return MZ_SUCCESS if succeeds or other values if failure.
*			   
*
*	@see	 
*     @note   
**/

UINT32  MetaZone_WriteReserved(BYTE *pbData, UINT32 u4Size);

/**
*	@brief  Write data to storage..
*	           
*	@return	 return MZ_SUCCESS if succeeds or other values if failure.
*			   
*
*	@see	 
*     @note   
**/
UINT32  MetaZone_Flush(VOID);

#if __cplusplus
}
#endif

#endif


