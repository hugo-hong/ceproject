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

/*******************************************************************************
*
* Filename:
* ---------
*   GMGResource.h
*
* Project:
* --------
*   MiGO
*
* Description:
* ------------
*
*
*
* Author:
* -------
*   mcn04047
*
*------------------------------------------------------------------------------
* $Revision: #1 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/
#ifndef _GMGRESOURCE_H_
#define _GMGRESOURCE_H_
#include "GDef.h"
#include "GResource.h"
#ifdef __cplusplus
extern "C" {
#endif

#if 1
#define MAX_SECTION_NAME_LEN   (8)
#define MAX_MACRO_LEN          (64)
#define SIZEOFBOM              (2)
#define MAGIC_CODE             (0x0000524D)
#define MAJOR_VER              (0x0001)
#define MINOR_VER              (0x0000)
#define INVALID_SECTION_OFFSET (0xFFFFFFFF)
#define INVALID_RESOURCE_ID    (0xFFFFFFFF)
#else
const GINT32 MAX_SECTION_NAME_LEN = 8;
const GINT32 MAX_MACRO_LEN = 64;
const GINT32 SIZEOFBOM = 2;
const GUINT32 MAGIC_CODE = 0x0000524D;
const GINT16 MAJOR_VER = 0x0001;
const GINT16 MINOR_VER = 0x0000;
const GUINT32 INVALID_SECTION_OFFSET = 0xFFFFFFFF;
const GUINT32 INVALID_RESOURCE_ID = 0xFFFFFFFF;
#endif

typedef struct _MGRC_FILE_HEADER_ 
{
    GUINT32    u4Magic;                //magic number 0x0000524D
    GUINT16    u2MajorVersion;         //major version 
    GUINT16    u2MinorVersion;         //minor version
    GUINT32    u4Timestamp;            //timestamp
    GUINT32    u4TotalSize;            //file total size
    GUINT32    u4SectionSA;            //first section start address
    GUINT16    u2SectionHeaderSize;    //section size
    GUINT16    u2SectionCount;         //section count
    GUINT32    u4SectionTotalSize;     //section total size
    GUINT32    u4Reserved;             //Reserved
    GUINT32    u4Checksum;
}MGRCFileHeader,*PMGRCFileHeader;

typedef struct _MGRC_RESOURCE_OFFSET_ 
{
    GUINT32    u4ID;
    GUINT32    u4TotalSize;
    GUINT32    u4Offset;
}MGRCOffset,*PMGRCOffset;

typedef struct _MGRC_SECTION_HEADER_
{
    GTCHAR     szSectionName[MAX_SECTION_NAME_LEN];
    GUINT32    u4ResourceVSA;
    GUINT32    u4OffsetTableSA;
    GUINT32    u4NextSectionSA;
    GUINT16    u2OffsetCount;
    GUINT16    u2OffsetSize;
    GUINT16    u2Alignment;
    GUINT16    u2Reserved;
    GUINT32    u4Checksum;
}MGRCSectionHeader, *PMGRCSectionHeader;


GRESULT        GInitMGResource(GHANDLE* hResource, const GTCHAR *pFileName, RES_CACHE_MODE_T eCacheMode);
GRESULT        GDeinitMGResource(GHANDLE hMGResource);

GRESULT        GLoadMGResource(GHANDLE* hRet, GHANDLE hMGResource,
                               const GTCHAR *pResourceName, GUINT32 u4Id,
                               GUINT32 *u4Size);
GRESULT        GLoadMGResourceEx(GHANDLE* hRet, GHANDLE hMGResource, const GTCHAR *pResourceName, GUINT32 u4Id, GUINT32 *pu4RawSz);
GRESULT        GLoadMGResRawData(GHANDLE* hRet, GHANDLE hMGResource, const GTCHAR *pResourceName, GUINT32 u4Id,
                                  GUINT32 u4StartPos, GUINT32 u4Len, GUINT32* pu4ReadBytes);
GRESULT        GGetMGResourceSize(GHANDLE hMGResource, const GTCHAR *pResourceName, GUINT32 u4Id, GUINT32* pu4ReadBytes);                                  
GUINT32        GDestroyMGResource(GHANDLE hResource);

#ifdef __cplusplus
}
#endif

#endif