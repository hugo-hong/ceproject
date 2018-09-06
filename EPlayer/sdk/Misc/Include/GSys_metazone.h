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
* file GSysServices.h
*
* Project:
* --------
*   CNB
*
* Description:
* ------------
*
*
* Author:
* -------
*
*
*------------------------------------------------------------------------------
* $Revision: #1 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/

#ifndef _GSYS_METAZONE_H_
#define _GSYS_METAZONE_H_

#include "windows.h"
#include "GDef.h"
#include "GKal.h"
#include "winioctl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
*  @defgroup SystemServices  SystemServices
*
*  @brief The module for AV switch between front and rear,
*  @{
*/

typedef enum {
    GSYS_DFLT_DW_SET = 0,
    GSYS_CUR_DW_SET,
    GCLT_DFLT_DW_SET,
    GCLT_CUR_DW_SET,
    GDVP_DFLT_DW_SET,
    GDVP_CUR_DW_SET,
    G_DFLT_BINARY_SET,
    G_CUR_BINARY_SET,
    MAX_GSET_TYPES
}MetaZoneListType;

/* Composer for Module specific error code */
#define MAKE_GERR_META(code)                 \
    ((GUINT32)                          \
     ((GUINT32)(0x80000000) |           \
     (GUINT32)((code) & 0xffff))        \
    )

typedef enum
{
    RET_GSYS_OK  =  RET_OK,

    RET_GSYS_INVALID_PARAM = MAKE_GERR_META(1),

    RET_GSYS_SEEK_IDX_ERR,

    RET_GSYS_READ_DWDATA_ERROR,

    RET_GSYS_READ_BINARY_ERROR,

    RET_GSYS_WRITE_DWDATA_ERROR,

    RET_GSYS_WRITE_BINARY_ERROR,

    RET_GSYS_FLUSH_ERROR,

    REG_GSYS_OPEN_MEZO_ERROR,
    
    RET_GSYS_UNKNOWN_ERROR ,

} GSYS_ERR_T;

GUINT32 MetaZoneOpen(void);

GUINT32 MetaZoneQueryValue( 
  CHAR* lpValueName, 
  MetaZoneListType eType,
  LPBYTE lpData, 
  UINT32 lpcbData 
);

GUINT32 MetaZoneSetValue( 
  CHAR* lpValueName, 
  MetaZoneListType eType,
  LPBYTE lpData, 
  UINT32 lpcbData,
  BOOL fgConCentrateFLush
);

GUINT32 MetaZoneClose(void);
GUINT32 MetaZoneFlush(void);

#ifdef __cplusplus
}
#endif

#endif