/*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2008 MediaTek Inc.
 *
 *  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 *  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
 *  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 *  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 *  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 *  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
 *  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
 *  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
 *  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 *  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 *  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
 *  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
 *  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
 *  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
 *  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
 *  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
 *
 *****************************************************************************/ 

/*****************************************************************************
 *
 * szFileName:
 * ---------
 *   $Workfile:  $ 
 *
 * Project:
 * --------
 *   GPS Pilot
 *
 * Description:
 * ------------
 *   
 *   
 * Author:
 * -------
 *   mcn03001
 *
 * Last changed:
 * ------------- 
 * $Author: zeng.zhang $ 
 *
 * $Modtime: $  
 *
 * $Revision: #1 $
****************************************************************************/
/*
 * Remark: 
 * 
 *   1. The GFS just define necessary APIs for GIS and APP instead of cover 
 *      everything of file system.
 *   2. The GFS almost mapping functions to corresponding of MTK Navigation
 *      platform one by one!
 */ 

#ifndef _GFS_H_
#define _GFS_H_

#include "GKal.h"
#include "GErr.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup FileSystem 
 * @brief Provides file system service.
 * @ingroup PAL
 */
/*@{*/

#ifndef GMAX_PATH
#define GMAX_PATH  256
#endif

/**
 *  Structure is used to get file information.
 */
typedef struct
{
    GUINT32      u4Attributes;   /**<File Attributes    */
    GDATETIME_T rCreationTime;   /**<File Creation Time    */
    GDATETIME_T rLastAccessTime; /**<File Last Access Time    */
    GDATETIME_T rLastWriteTime;  /**<File Last Write Time    */
    GUINT32      u4Size;         /**<File Size    */
} GFILEINFO_T;

/**
 *  Structure is used for  finding  file.
 */
typedef struct
{
    GUINT32      u4Attributes;          /**<File Attributes    */  
    GDATETIME_T rCreationTime;          /**<File Creation Time    */
    GDATETIME_T rLastAccessTime;        /**<File Last Access Time    */
    GDATETIME_T rLastWriteTime;         /**<File Last Write Time    */
    GUINT32      u4Size;                /**<File Size    */
    GTCHAR       szFullName[GMAX_PATH]; /**<File Full Name    */
    GTCHAR       szShortName[14];       /**<File Short Name    */
} GFINDFILEDATA_T;

#if defined(WIN32)
typedef GHANDLE  FS_HANDLE;

/* Attributes */
#define FS_ATTR_READ_ONLY        FILE_ATTRIBUTE_READONLY
#define FS_ATTR_HIDDEN           FILE_ATTRIBUTE_HIDDEN
#define FS_ATTR_SYSTEM           FILE_ATTRIBUTE_SYSTEM
#define FS_ATTR_DIR              FILE_ATTRIBUTE_DIRECTORY
#define FS_ATTR_ARCHIVE          FILE_ATTRIBUTE_ARCHIVE

/* File Open Flags */
#define FS_READ_WRITE            (GENERIC_READ | GENERIC_WRITE)
#define FS_READ_ONLY             GENERIC_READ
#define FS_CREATE                CREATE_NEW
#define FS_CREATE_ALWAYS         CREATE_ALWAYS
#define FS_OPEN_EXISTING            OPEN_EXISTING    
#define FS_OPEN_ALWAYS           OPEN_ALWAYS
#define FS_SHARD_READ            FILE_SHARE_READ
#define FS_SHARD_WRITE           FILE_SHARE_WRITE

#define FS_COMMITTED             
#define FS_CACHE_DATA            
#define FS_LAZY_DATA             
#define FS_NONBLOCK_MODE         
#define FS_PROTECTION_MODE       


enum
{
    FS_FILE_BEGIN   = FILE_BEGIN,
    FS_FILE_CURRENT = FILE_CURRENT,
    FS_FILE_END     = FILE_END
};

#else /* TODO: other platform */

// Temp put here for doxygen!

/*@{*/
/** @name File Attributes */
#define FS_ATTR_READ_ONLY        0x01L
#define FS_ATTR_HIDDEN           0x02L
#define FS_ATTR_SYSTEM           0x04L
#define FS_ATTR_VOLUME           0x08L
#define FS_ATTR_DIR              0x10L
#define FS_ATTR_ARCHIVE          0x20L
/*@}*/

/*@{*/
/** @name File Open Parameters */
#define FS_READ_WRITE            0x00000000L
#define FS_READ_ONLY             0x00000100L
#define FS_CREATE                0x00010000L
#define FS_CREATE_ALWAYS         0x00020000L 
#define FS_COMMITTED             0x01000000L
#define FS_CACHE_DATA            0x02000000L
#define FS_LAZY_DATA             0x04000000L
#define FS_NONBLOCK_MODE         0x10000000L
#define FS_PROTECTION_MODE       0x20000000L
/*@}*/

enum
{
    FS_FILE_BEGIN   = 0,
    FS_FILE_CURRENT,
    FS_FILE_END
};

#endif 

EXTERN GBOOL _fgCardOut;

#define GIsCardOut() _fgCardOut
#define GSetCadOut(fgIsCardOut) (_fgCardOut = fgIsCardOut)

//Part 1 -- File Only


/**
 * Opens or creates a file.
 *
 * @param[in]  szFileName  Pointer to a null-terminated string that Specifies
                        the name of the file to open or create.
 * @param[in]  u4Flag     Flags
 * @param[in]  phFile     Pointer to the file handle to be opened or created.
 *
 * @return GRESULT define in Gdef.h
 */
GAPI GRESULT GOpenFile(const GTCHAR *szFileName, GUINT32 u4Flag, FS_HANDLE *phFile);

/**
*  Opens or creates a file in share mode.
* 
* @param[in]  szFileName  Pointer to a null-terminated string that Specifies
                 the name of the file to open or create.
* @param[in]  u4Flag      Flags
* @param[in]  phFile      Pointer to the file handle to be opened or created.  
*
* @return GRESULT define in Gdef.h
*/
GAPI GRESULT GOpenFileEx(const GTCHAR *szFileName, GUINT32 u4Flag, FS_HANDLE *phFile);


/**
 *  Close a file.
 *
 * @param[in]  hFile  The file handle.
 *
 * @return GRESULT define in Gdef.h
 */
GAPI GRESULT GCloseFile(FS_HANDLE hFile);

/**
 * Reads data from a file.
 *
 * @param[in]  hFile    The file handle.
 * @param[out]  pvData   Pointer to the buffer that receives the data read from the file.
 * @param[in]  u4Len    Number of bytes to be read from the file.
 * @param[in]  pu4Read  Pointer to the number of bytes read.
 *
 * @return GRESULT define in Gdef.h
 */
GAPI GRESULT GReadFile(FS_HANDLE hFile, GVOID *pvData, GUINT32 u4Len, 
                    GUINT32   *pu4Read); // *pu4Read == 0 : EOF


/**
 *  Writes data to a file.
 *
 * @param[in]  hFile       The file handle.
 * @param[in]  pvData     Pointer to the buffer containing the data to be
 *                       written to the file.
 * @param[in]  u4Len       Number of bytes to be written to the file.
 * @param[in]  pu4Written  Pointer to the number of bytes written.
 *
 * @return GRESULT define in Gdef.h
 */
GAPI GRESULT GWriteFile(FS_HANDLE hFile, GVOID *pvData, GUINT32 u4Len, 
                     GUINT32   *pu4Written);


/**
 *  Flushes file data.
 *
 * @param[in]  hFile  The file handle.
 *
 * @return Returns TRUE if the function succeeds, FALSE or not.
 */
GAPI GBOOL GFlushFile(FS_HANDLE hFile);

/**
 *  Moves the file pointer to the specified position for a open file.
 *
 * @param[in]  hFile     The file handle.
 * @param[in]  i4Offset  The number of bytes to move the file pointer.
 * @param[in]  i4Whence  Specifies the starting point for the file pointer move.
 *
 * @return GRESULT define in Gdef.h
 */
GAPI GRESULT GSeekFile(FS_HANDLE hFile, GINT32 i4Offset, GINT32 i4Whence);
#define GSeekFileToBegin(hFile)  GSeekFile(hFile, 0, FS_FILE_BEGIN)
#define GSeekFileToEnd(hFile)    GSeekFile(hFile, 0, FS_FILE_END)

/**
 * Moves the file pointer to the end for a open file.
 *
 * @param[in]  hFile     The file handle.
 *
 * @return Returns TRUE if the function succeeds, FALSE or not.
 */
GAPI GBOOL GSetEndOfFile(FS_HANDLE hFile);


/**
 * Gets current file pointer position for a opend file.
 *
 * @param[in]  hFile  The file handle.
 * @param[out]  pu4Pos Pointer to the variable that receives the file position.
 *
 * @return GRESULT define in Gdef.h
 */
GAPI GRESULT GGetFilePosition(FS_HANDLE hFile, GUINT32 *pu4Pos);

/**
 * Gets file size.
 *
 * @param[in]  hFile   The file handle.
 * @param[out]  pu4Size file size .
 *
 * @return GRESULT define in Gdef.h
 */
GAPI GRESULT GGetFileSize(FS_HANDLE hFile, GUINT32 *pu4Size);

/**
 *  Deletes a file.
 *
 * @param[in]  hFile  The file name.
 *
 * @return GRESULT define in Gdef.h
 */
GAPI GRESULT GDeleteFile(const GTCHAR *szFileName);

//Part 2 -- Folder Only
/**


 *  Gets current directory.
 *
 * @param[in]  szDirNameBuf  Pointer to the buffer to receives the directory string.
 * @param[out]  u4MaxLen      Specifies the maximum length, in TCHARs, of the
 *                         buffer for the current directory string
 *
 * @return GRESULT define in Gdef.h
 */
GAPI GRESULT GGetCurrentDir(GTCHAR *szDirNameBuf, GUINT32 u4MaxLen);
/**
 *  Creates a directory.
 *
 * @param[in]  szDirName  Pointer to the string that specifies the path of the
 *                      directory to be created.
 *
 *@return GRESULT define in Gdef.h
 */
GAPI GRESULT GCreateDir(const GTCHAR *szDirName);

//Part 3 -- Both File and Folder

/**
 * Renames a file or directory.
 *
 * @param[in]  szOldName  Pointer to the string that specifies the path of the
 *                      file or directory want to be renamed.
 * @param[out]  szOldName  Pointer to the string that specifies the new name of
 *                      a file or directory.
 *
 * @return GRESULT define in Gdef.h
 */

GAPI GRESULT GRenameFile(const GTCHAR *szOldName, const GTCHAR *szNewName);


/**
 *  Searches a directory for a file or subdirectory whose name matches the
 *          specified name pattern.
 *
 * @param[in]  szNamePattern  Pointer to a string that specifies a valid
 *                          directory or path and filename.
 * @param[out]  prFindDataBuf  Pointer to the structure that receives information
 *                          about the found file or subdirectory.
 * @param[out]  phFindFile     Pointer to the variable that receives the search
 *                          handle, used in a subsequent call to GFindNextFile
 *                          or GFindClose
 *
 * @return GRESULT define in Gdef.h
 */
GAPI GRESULT GFindFirstFile(const GTCHAR     *szNamePattern, 
                         GFINDFILEDATA_T *prFindDataBuf, 
                         FS_HANDLE       *phFindFile);
/**
 *  Continues a file search from a previous call to the FindFirstFile function.
 *
 * @param[in]  hFindFile      Search handle returned by a previous call to the
 *                          GFindFirstFile function.
 * @param[out]  prFindDataBuf  Pointer to the structure that receives information
 *                          about the found file or subdirectory.
 *
 * @return GRESULT define in Gdef.h
 */
GAPI GRESULT GFindNextFile(FS_HANDLE hFindFile, GFINDFILEDATA_T *prFindDataBuf);

/**
 *  Closes the specified search handle.
 *
 * @param[in]  hFindFile Search handle. This handle must have been previously
 *                      opened by the FindFirstFile function.
 *
 *@return GRESULT define in Gdef.h
 */
GAPI GRESULT GFindClose(FS_HANDLE hFindFile);
/**
 *   This method is to get module path
 *
 * @param[out]  hFindFile      the path of file
 * @param[in]  u4Len  useless
 *
 * @return Returns TRUE if the function succeeds, FALSE or not.
 */

GAPI GBOOL  GGetModulePath(GTCHAR* lpFilePath, GUINT32 u4Len);
/**
 *   This method is to judge the file is exist or not
 *
 * @param[in]  lpszFileName      the file name
 *
 * @return Returns TRUE if the function succeeds, FALSE or not.
 */
GAPI GBOOL  GIsFileExist(const GTCHAR* lpszFileName);
/**
 *  This method is to judge the directory is exist or not
 *
 * @param[in]  lpszDirName      the directory name
 *
 * @return Returns TRUE if the function succeeds, FALSE or not.
 */
GAPI GBOOL  GIsDirExist(const GTCHAR* lpszDirName);
/**
 *  This method is to ensure the path is exist or not
 *
 * @param[in]  wszDir      the directory of file
 *
 * @return GRESULT define in Gdef.h
 */
GAPI GRESULT GEnsurePathExist(const GTCHAR *wszDir);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif //_GFS_H_


