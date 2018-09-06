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
 *   GPERes.h
 *
 * Project:
 * --------
 *   SDK
 *
 * Description:
 * ------------
 *
 *
 *
 * Author:
 * -------
 *   Daowen Deng, MTK40043
 *
 *------------------------------------------------------------------------------
 * $Revision: #1 $
 * $Modtime:$
 * $Log:$
 *
 *******************************************************************************/

#ifndef _GPERES_H_
#define _GPERES_H_

#include "GDef.h"

//
// Little endian.
//
#define IMAGE_DOS_SIGNATURE                 0x5A4D      // MZ
#define IMAGE_NT_SIGNATURE                  0x00004550  // PE00

#define IMAGE_ENTRY_CNT                     16

#define IMG_SHORT_NAME_LEN                  8

typedef struct GRES_DATA_TAG
{
    GUINT   dwOft;
    GUINT   dwSize;
}GRES_DATA_T, *GPRES_DATA_T;

/*
 * Directory format.
 */
typedef struct GIMG_DATA_DIR_TAG
{
    GUINT   VirtualAddress;
    GUINT   Size;
}GIMG_DATA_DIR_T, *PGIMG_DATA_DIR_T;

/*
 * File header format.
 */
typedef struct GIMG_OPT_HDR_TAG
{
    // Standard fields.
    GUINT16   Magic;
    GUCHAR    MajorLinkerVersion;
    GUCHAR    MinorLinkerVersion;
    GUINT     SizeOfCode;
    GUINT     SizeOfInitializedData;
    GUINT     SizeOfUninitializedData;
    GUINT     AddressOfEntryPoint;
    GUINT     BaseOfCode;
    GUINT     BaseOfData;
    
    // NT additional fields.
    GUINT     ImageBase;
    GUINT     SectionAlignment;
    GUINT     FileAlignment;
    GUINT16   MajorOperatingSystemVersion;
    GUINT16   MinorOperatingSystemVersion;
    GUINT16   MajorImageVersion;
    GUINT16   MinorImageVersion;
    GUINT16   MajorSubsystemVersion;
    GUINT16   MinorSubsystemVersion;
    GUINT     Win32VersionValue;
    GUINT     SizeOfImage;
    GUINT     SizeOfHeaders;
    GUINT     CheckSum;
    GUINT16   Subsystem;
    GUINT16   DllCharacteristics;
    GUINT     SizeOfStackReserve;
    GUINT     SizeOfStackCommit;
    GUINT     SizeOfHeapReserve;
    GUINT     SizeOfHeapCommit;
    GUINT     LoaderFlags;
    GUINT     NumberOfRvaAndSizes;
    GIMG_DATA_DIR_T DataDirectory[IMAGE_ENTRY_CNT];
} GIMG_OPT_HDR_T, *PGIMG_OPT_HDR_T;

typedef struct GIMG_FILE_HDR_TAG
{
    GUINT16   Machine;
    GUINT16   NumberOfSections;
    GUINT32   TimeDateStamp;
    GUINT32   PointerToSymbolTable;
    GUINT32   NumberOfSymbols;
    GUINT16   SizeOfOptionalHeader;
    GUINT16   Characteristics;
}GIMG_FILE_HDR_T, *PGIMG_FILE_HDR_T;

typedef struct GIMG_NT_HDR_TAG
{
    GUINT32           Signature;
    GIMG_FILE_HDR_T FileHeader;
    GIMG_OPT_HDR_T  OptHdr;
} GIMG_NT_HDR_T, *PGIMG_NT_HDR_T;


/*
 * The flowlling structures are copied from winnt.h
 */
typedef struct GIMG_RES_DIR_ENTRY_TAG
{
    union
    {
        GUINT        Name;
        GUINT16      Id;
    }u1;
    union
    {
        GUINT        OffsetToData;
        struct
        {
            GUINT    OffsetToDirectory:31;
            GUINT    DataIsDirectory:1;
        }s2;
    }u2;
} GIMG_RES_DIR_ENTRY_T, *PGIMG_RES_DIR_ENTRY_T;


//
// Resource Format.
//

//
// Resource directory consists of two counts, following by a variable length
// array of directory entries.  The first count is the number of entries at
// beginning of the array that have actual names associated with each entry.
// The entries are in ascending order, case insensitive strings.  The second
// count is the number of entries that immediately follow the named entries.
// This second count identifies the number of entries that have 16-bit integer
// Ids as their name.  These entries are also sorted in ascending order.
//
// This structure allows fast lookup by either name or number, but for any
// given resource entry only one form of lookup is supported, not both.
// This is consistant with the syntax of the .RC file and the .RES file.
//

typedef struct GIMG_RES_DIR_TAG
{
    GUINT    Characteristics;
    GUINT    TimeDateStamp;
    GUINT16  MajorVersion;
    GUINT16  MinorVersion;
    GUINT16  NumberOfNamedEntries;
    GUINT16  NumberOfIdEntries;
} GIMG_RES_DIR_T, *PGIMG_RES_DIR_T;


//
// Each resource data entry describes a leaf node in the resource directory
// tree.  It contains an offset, relative to the beginning of the resource
// directory of the data for the resource, a size field that gives the number
// of UCHARs of data at that offset, a CodePage that should be used when
// decoding code point values within the resource data.  Typically for new
// applications the code page would be the unicode code page.
//

typedef struct GIMG_RES_DATA_ENTRY_TAG
{
    GUINT   OffsetToData;
    GUINT   Size;
    GUINT   CodePage;
    GUINT   Reserved;
} GIMG_RES_DATA_ENTRY_T, *PGIMG_RES_DATA_ENTRY_T;

//
// Section header format.
//
typedef struct GIMG_SEC_HDR_TAG
{
    GUCHAR    Name[IMG_SHORT_NAME_LEN];
    union
    {
        GUINT   PhysicalAddress;
        GUINT   VirtualSize;
    } Misc;
    GUINT32   VirtualAddress;
    GUINT32   SizeOfRawData;
    GUINT32   PointerToRawData;
    GUINT32   PointerToRelocations;
    GUINT32   PointerToLinenumbers;
    GUINT16   NumberOfRelocations;
    GUINT16   NumberOfLinenumbers;
    GUINT32   Characteristics;
} GIMG_SEC_HDR_T, *PGIMG_SEC_HDR_T;

typedef struct _BITMAPINFOHEADER
{
    GUINT32      biSize;
    GINT32       biWidth;
    GINT32       biHeight;
    GUINT16      biPlanes;
    GUINT16      biBitCount;
    GUINT32      biCompression;
    GUINT32      biSizeImage;
    GINT32       biXPelsPerMeter;
    GINT32       biYPelsPerMeter;
    GUINT32      biClrUsed;
    GUINT32      biClrImportant;
} GBITMAPINFOHEADER_T;

DECLARE_HANDLE(HRESLIST);

typedef struct RES_LIST_HDR_TAG
{
    GUINT16 u16ResNum;
    GUINT16 u16Reserved;
    GHANDLE hFileHandle;
} RES_LIST_HDR_T, *PRES_LIST_HDR_T;

typedef struct RES_INDEX_TAG
{
    GDWORD   uid;
    GDWORD   offset;
    GDWORD   size;
    GDWORD   reserved;
}RES_INDEX_T, *PRES_INDEX_T;

HRESLIST LoadPELibrary(const GTCHAR *m_FileNamePE);
HBITMAP GetImgFromPEById(HRESLIST hResHandle, GUINT16 uid);
GBOOL FreePELibrary(HRESLIST hResHandle);

#endif // #if _GPERES_H_
