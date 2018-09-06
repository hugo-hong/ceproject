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


/**
* @file GPrefs.h
*
*
* @author mcn03041 juntang@mtk.com.cn
*
*/

#ifndef _GPREFS_H_
#define _GPREFS_H_

#include "GDef.h"
#include "GFs.h"
#include "GCfg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SECTION_SIZE        256
#define MAX_ENTRY_SIZE          256

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#if GB_NEW_INI
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define MCF_DEBUG 0

#if MCF_DEBUG
    #define MCFASSERT ASSERT
#else
    #define MCFASSERT
#endif
    
#define RET_MCF_CHECKSUM_ERROR MAKE_GERR(MOD_PAL_PREFS, 1)

#define MAX_SECTION_NUM        1024

#define MAX_SIZE_OF_ONE_LINE   1023

#define MCF_SAVEAS_BINARY      0x00000001

#define MEMORY_MODE_MATERIALIZER_ALLOC_AND_FREE 0

typedef GUINT16  TOKEN;

typedef struct _tagKEY_TABLE
{
    GTCHAR       *szAddr;                       // address of text
    GINT32       i4Hash;                        // -1 for Err, 0 for Null-string
    GUINT32      u4OffsetAtFile;                // -1 for Value-String is Dynamic-Allocated
    GUINT32      u4ValueOffset;                 // [-1 for Value-String is Dynamic-Allocated]
    GUINT16      u2ModStatus;                   // modification flag
    GBOOL        fgDynamicAlloc;                // indicate the flag of creation
}KEY_TABLE;                                     // Node data

typedef struct _tagDataList
{
    KEY_TABLE           data;
    struct _tagDataList *prev;
    struct _tagDataList *next;
}ListNode;                                      // ListNode

typedef struct _tagNODE_MEMPOOL                 //
{
    GVOID         *pvAddr;                      // ValueString
    GVOID         *pvFreeList;                  // FreeList
    GUINT32       u2OneBlkSize;                 // NodeByteCount
    GUINT32       u2BlksCount;                  // NodeCount
    GUINT32       u2BlkFreeCount;               // FreeNodeCount
//  GUINT32       u2ID;                         // MemPoolId
}NODE_MEMPOOL;                                  // NodePoolDescriptor

typedef struct _tagGBASE
{
    GTCHAR        szFileName[GMAX_PATH]; 
    GBOOL         fgBinaryFmt;           
    GBOOL         fgHasOpened;                  // for handle reentrant
    GBOOL         fgOrigExists;                 // 
    GUINT8        fgModified;                   //
    GUINT32       u4CharCount;
    GUINT32       u4FileCursor;                 // current cursor position
    GTCHAR       *szTxtBuf;                     // TxtBuf
    NODE_MEMPOOL *_NodePoolDes;                 // NodePoolDes
    ListNode     *_SectionTbl[MAX_SECTION_NUM]; // SectionTbl
}MCF, *HMCF;                                    // MCF handle

// MCF Interfaces
// Interfaces 0001 GOpenPrefs

#define GOpenPrefsEx(a, b, c) GOpenPrefsExI(a, b, c, __FILE__, __LINE__)

GAPI GRESULT GOpenPrefsExI(HMCF *ppMcf, const GTCHAR *szFileName,
                           GUINT32 u4OpenFlag,
                           GCHAR *szName, GUINT32 u4FileLine);

// Interfaces 0002 GOpenPrefs
#define GOpenPrefs(a, b) GOpenPrefsI(a, b, __FILE__, __LINE__)
GAPI GRESULT GOpenPrefsI(HMCF *ppMcf, const GTCHAR *szFileName,
                         GCHAR *szName, GUINT32 u4FileLine);

// Interface 000 GOpenPrefsFromBuf
#define GOpenPrefsFromBuf(a, b, c, d) GOpenPrefsFromBufI(a, b, c, d, __FILE__, __LINE__)
GRESULT GOpenPrefsFromBufI(HMCF *ppMcf, const GTCHAR *szTxtBuf,
                           GUINT32 u4CharCount, GUINT32 u4OpenFlag,
                           GCHAR *szName, GUINT32 u4FileLine);

// Interfaces 0003 GFlushMcf
GAPI GRESULT GFlushMcf(HMCF hMcf);

// Interfaces 0004 GClosePrefs
GAPI GRESULT GClosePrefs(HMCF *ppMcf);

// Interfaces 0005 GWritePrefsStringEx
GAPI GRESULT GWritePrefsStringEx(HMCF hMcf, const GTCHAR *szSection,
                                 const GTCHAR* szKey, const GTCHAR *szValue);

// Interfaces 0006 GGetPrefsStringEx
GAPI GINT32  GGetPrefsStringEx(HMCF hMcf, const GTCHAR *szSection,
                               const GTCHAR *szKey, GTCHAR *Buffer,
                               GUINT32 u4Size);

// Interfaces 0007 GWritePrefsIntEx
GAPI GRESULT GWritePrefsIntEx(HMCF hMcf, const GTCHAR *szSection,
                              const GTCHAR *szKey, GINT32 i4Value);

// Interfaces 0008 GGetPrefsIntEx
GAPI GINT32  GGetPrefsIntEx(HMCF hMcf, const GTCHAR *szSection,
                            const GTCHAR *szKey, GINT32 i4Default);

// Interfaces 0009 GWritePrefsDataEx
GAPI GRESULT GWritePrefsDataEx(HMCF hMcf, const GTCHAR *szSection,
                               const GTCHAR *szKey, GUINT8 *pData,
                               GUINT32 u4Bytes);

// Interfaces 0010 GGetPrefsDataEx
GAPI GUINT32 GGetPrefsDataEx(HMCF hMcf, const GTCHAR *szSection,
                             const GTCHAR *szKey, GUINT8 *pData,
                             GUINT32 u4Bytes);

// Interfaces 0011 GQueryPrefsSectionEx
GAPI GBOOL   GQueryPrefsSectionEx(HMCF hMcf, const GTCHAR *szSection);

// Interfaces 0012 GQueryPrefsKeyEx
GAPI GBOOL   GQueryPrefsKeyEx(HMCF hMcf, const GTCHAR *szSection,
                              const GTCHAR *szKey);

// Interfaces 0013 GClearPrefsSectonEx
GAPI GRESULT GClearPrefsSectonEx(HMCF hMcf, const GTCHAR *szSection);

// Interfaces 0014 GClearPrefsKeyEx
GAPI GRESULT GClearPrefsKeyEx(HMCF hMcf, const GTCHAR *szSection,
                              const GTCHAR *szKey);

// Interface 0015 
GAPI GRESULT GGetKeyCountOfSection(HMCF hMcf, const GTCHAR* szSection,
                                   GINT32 *u4SectionCount);

// Interface 0016
GAPI GRESULT GGetKeyNameByIndexAtSection(HMCF hMcf, const GTCHAR* szSection,
                                         GUINT32 u4Index, GTCHAR* buffer
    , GUINT32 u4size);

//|----------------|
//| GPrefs         |
//|---------------------------------------------------------------------------|
//|                                                                           |
//| |----------------|       |-----------|                                    |
//| | Register Table |       |  Ini file |                                    |
//| |--------------------|   |---------------------------------------------|  |
//| |                    |   |   |-----------|        |-----------|        |  |
//| |--------------------|   |   | hash      |        | DualList  |        |  |
//|                          |   |--------------|     |------------------| |  |
//|                          |   |              |     |                  | |  |
//|                          |   |--------------|     |------------------| |  |
//|                          |                                             |  |
//|                          |   |-----------|        |-----------|        |  |
//|                          |   | Ini oper  |        | NodePool  |        |  |
//|                          |   |--------------|     |------------------| |  |
//|                          |   |              |     |                  | |  |
//|                          |   |              |     |------------------| |  |
//|                          |   |              |                          |  |
//|                          |   |              |     |-----------|        |  |
//|                          |   |              |     | StrTrim   |        |  |
//|                          |   |              |     |------------------| |  |
//|                          |   |              |     |                  | |  |
//|                          |   |--------------|     |------------------| |  |
//|                          |---------------------------------------------|  |
//|                                                                           |
//|---------------------------------------------------------------------------|

#define  GINI_START(lpfile)
#define  GINI_END()

GAPI GINT32  GetRegCurrUser(const GTCHAR *szSection, const GTCHAR* szKey,
                            GINT32 i4Default);
GAPI GBOOL   SetRegCurrUser(const GTCHAR* szSection, const GTCHAR* szKey,
                            GINT32 i4Value);

GAPI GBOOL   GInitPrefs(GVOID *pvParam);

GAPI GTCHAR* GGetDefaultPrefsName();

// Handle registry
GAPI GBOOL   GWritePrefsInt(const GTCHAR* szSection, const GTCHAR* szKey,
                            GINT32 i4Value);
GAPI GINT32  GGetPrefsInt(const GTCHAR* szSection, const GTCHAR* szKey,
                          GINT32 i4Default);

GAPI GBOOL   GWritePrefsString(const GTCHAR* szSection, const GTCHAR* szKey,
                               const GTCHAR* szValue);
GAPI GINT32  GGetPrefsString(const GTCHAR* szSection, const GTCHAR* szKey,
                             GTCHAR* lpBuffer, GUINT32 u4Size);

GAPI GBOOL   GWritePrefsData(const GTCHAR* szSection, const GTCHAR* szKey,
                             GUINT8* pData, GUINT32 u4Bytes);
GAPI GUINT32 GGetPrefsData(const GTCHAR* szSection, const GTCHAR* szKey,
                           GUINT8* pData, GUINT32 u4Bytes);

GAPI GBOOL   GQueryPrefsSection(const GTCHAR* szSection);
GAPI GBOOL   GClearPrefsSection(HMCF hMcf,const GTCHAR* szSection);

GAPI GUINT32 GQueryPrefsNumSection(GTCHAR* lpBuffer, GUINT32 u4Size);

GAPI GVOID   GDeinitPrefs(GVOID);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Old implement code for ini Operation

/**
 * @defgroup Preference
 * @brief Provides persistent service for setting, configuration etc.
 * @ingroup PAL
 */
/*@{*/

GAPI GBOOL   GInitPrefs(GVOID *pvParam);
GAPI GVOID   GDeinitPrefs(GVOID);

GAPI GTCHAR* GGetDefaultPrefsName();

#ifdef _WIN32_WCE
// Handle registry
GAPI GBOOL   GWritePrefsInt(const GTCHAR* lpszSection, const GTCHAR* lpszEntry, GINT32 i4Value);
GAPI GINT32  GGetPrefsInt(const GTCHAR* lpszSection, const GTCHAR* lpszEntry, GINT32 i4Default);

GAPI GBOOL   GWritePrefsString(const GTCHAR* lpszSection, const GTCHAR* lpszEntry, const GTCHAR* lpszValue);
GAPI GINT32  GGetPrefsString(const GTCHAR* lpszSection, const GTCHAR* lpszEntry, GTCHAR* lpBuffer, GUINT32 u4Size);

GAPI GBOOL   GWritePrefsData(const GTCHAR* lpszSection, const GTCHAR* lpszEntry, GUINT8* pData, GUINT32 u4Bytes);
GAPI GUINT32 GGetPrefsData(const GTCHAR* lpszSection, const GTCHAR* lpszEntry, GUINT8* pData, GUINT32 u4Bytes);

GAPI GBOOL   GQueryPrefsSection(const GTCHAR* lpszSection);
GAPI GBOOL   GClearPrefsSection(const GTCHAR* lpszSection);

GAPI GUINT32 GQueryPrefsNumSection(GTCHAR* lpBuffer, GUINT32 u4Size);
#endif

// Extended version. for handle .ini file
GAPI GBOOL   GINI_START(const GTCHAR* lpfile);
GAPI GVOID   GINI_END();

GAPI GBOOL   GWritePrefsIntEx(const GTCHAR* lpszSection, const GTCHAR* lpszEntry, GINT32 i4Value
    , const GTCHAR* lpFileName);
GAPI GINT32  GGetPrefsIntEx(const GTCHAR* lpszSection, const GTCHAR* lpszEntry, GINT32 i4Default
    , const GTCHAR* lpFileName);

GAPI GBOOL   GWritePrefsStringEx(const GTCHAR* lpszSection, const GTCHAR* lpszEntry, const GTCHAR* lpszValue
    , const GTCHAR* lpFileName);
GAPI GINT32  GGetPrefsStringEx(const GTCHAR* lpszSection, const GTCHAR* lpszEntry, GTCHAR* lpBuffer, GUINT32 u4Size
    , const GTCHAR* lpFileName);

GAPI GBOOL   GQueryPrefsSectionEx(const GTCHAR* lpszSection, const GTCHAR* lpFileName);
GAPI GBOOL   GClearPrefsSectionEx(const GTCHAR* lpszSection, const GTCHAR* lpFileName);

GAPI GUINT32 GQueryPrefsNumSectionEx(GTCHAR* lpBuffer, GUINT32 u4Size, const GTCHAR* lpFileName);

GAPI GBOOL   GWritePrefsDataEx(const GTCHAR* lpszSection, const GTCHAR* lpszEntry, GUINT8* pData, GUINT32 u4Bytes
    , const GTCHAR* lpFileName);
GAPI GUINT32 GGetPrefsDataEx(const GTCHAR* lpszSection, const GTCHAR* lpszEntry, GUINT8* pData, GUINT32 u4Bytes
    , const GTCHAR* lpFileName);


GAPI GUINT32 GGetSectionKeyCntEx(const GTCHAR* lpszSection, const GTCHAR* lpFileName);
GAPI GBOOL GGetKeyNameEx(const GTCHAR* lpszSection, GUINT32 u4Index, GTCHAR* buffer, GUINT32 u4size
    , const GTCHAR* lpFileName);
/*@}*/

#endif

#ifdef __cplusplus
}
#endif

#endif //_GPREFS_H_
