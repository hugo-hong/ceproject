/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
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
 * Filename:
 * ---------
 *   $Workfile:  GParseConfig.h $ 
 *
 * Project:
 * --------
 *   MiGO Parser
 *
 * Description:
 * ------------
 *   This file is for Parser Configuration Options.
 *   
 * Author:
 * -------
 *   mcn03011
 *
 * Last changed:
 * ------------- 
 * $Author: zeng.zhang $ 
 *
 * $Modtime: $  
 *
 * $Revision: #1 $
 *
 * Ver1.29, 2009/03/27:
 * 1. Change FORM_MAX = 110 
 * 2. Add u4IconDownID, u4IconFocusID, u4IconDisableID to GBUTTONSPECDATA_T.
 * 
****************************************************************************/

#ifndef _GPARSE_CFG_H_
#define _GPARSE_CFG_H_

#include "GDef.h"

//////////////////////////////////////////////////////////////////////////
// Parse Data File mode.
// 1: When MiGOFormRes.dat be merge to a MRF file, enable this option.
// 0: When MiGOFormRes.dat is a independence file, disable this option.
#define CFG_GPARSE_DAT_FILE_FROM_MRF 1

//////////////////////////////////////////////////////////////////////////
// Parse Form Memory Mode.
// 0: Parse one form from data file - this mode for compact memory. 
// 1: Parse all form from data file - this mode for big memory.
#define CFG_GPARSE_LOAD_ALL_FORM     0

//////////////////////////////////////////////////////////////////////////
// Enable Check Sum
// 0: Disable Check Sum.
// 1: Enable Check Sum.
#define CFG_GPARSE_CHECK_SUM         1


///////////////////////////////////////////////////////////////
// For INI Format - Parse one or more INI Files
// 1: Parse more INI Files.
// 0: Parse one INI File.
#define CFG_PARSE_ONE_INI        0

//////////////////////////////////////////////////////////////
// For INI Format - Print debug information
// 1: Enable Parse Print Module for dump debug file.
// 0: Disable Parse Print Module for dump debug file.
#define CFG_PARSE_DUMP_INI       0

/////////////////////////////////////////////////////////////
// For INI Format - Configurations for Section/Key Name Buffer Length
#define PARSER_KEY_NAME_LEN      64
#define PARSER_KEY_NAME_MID      128
#define PARSER_WDT_NAME_LEN      256
#define PARSER_WDT_ITEM_LEN      1024

// Configurations for Widget Memory Allocate 
#define PWGT_FORM_MAX         3
#define PWGT_PANEL_MAX        2
#define PWGT_BUTTON_MAX       35
#define PWGT_STATIC_MAX       20
#define PWGT_CHECK_MAX        20
#define PWGT_RADIO_MAX        20
#define PWGT_RADIO_GROUP_MAX  10
#define PWGT_EDIT_MAX         20
#define PWGT_LVIEW_MAX        20
#define PWGT_MVIEW_MAX        15
#define PWGT_TIME_MAX         2
#define PWGT_DATE_MAX         2
#define PWGT_PROGRESS_MAX     10
#define PWGT_SCROLL_MAX       5
#define PWGT_SLIDER_MAX       10
#define PWGT_COMBOBOX_MAX     10
#define PWGT_TAB_MAX          2
#define PWGT_ANIMATE_MAX      10
#define PWGT_ANIMATE_ICON_MAX 20
#define PWGT_POPUPMENU_MAX    2
#define PWGT_MSGBOX_MAX       10
#define PWGT_VCI_ITEM_MAX     35
#define PWGT_WGT_ITEM_MAX     40

//////////////////////////////////////////////////////////////////////////
// How many Form is supported in this utility.
#define FORM_MAX           110 
#define FORM_NAME_LEN      64 

//////////////////////////////////////////////////////////////////////////
// How many MView/LView is supported in a Form.
#define MAX_MVIEW_IN_FORM  5

//////////////////////////////////////////////////////////////////////////
// Binary File Head Info. 
#define FILE_HEAD_VERSION  ("Ver1.29")
#define FILE_HEAD_DATE     ("2009/03/27")
#define FILE_HEAD_VENDOR   ("(C) MediaTek,Inc. All rights reserved.")

//////////////////////////////////////////////////////////////////////////
// Binary Default File Name
#define FILE_HEAD_FORM_FILENAME ("MiGOResForm.dat")
#define FILE_HEAD_ICON_FILENAME ("MiGOResIcon.dat")
#define FILE_HEAD_TEXT_FILENAME ("MiGOResText.dat")

//////////////////////////////////////////////////////////////////////////
// Binary File Head Info Length
#define HEAD_FILENAME_LEN  32
#define HEAD_VERSION_LEN   8
#define HEAD_DATE_LEN      10
#define HEAD_VENDOR_LEN    40

//////////////////////////////////////////////////////////////////////////
// Invalid pointer address.
#define INVALID_ADDRESS    0x00000000

//////////////////////////////////////////////////////////////////////////
// For Save a Binary File Head Info
typedef struct _GFORMDAT_FILE_HEAD
{
    GCHAR  szFileName[HEAD_FILENAME_LEN];
    GCHAR  szVersion[HEAD_VERSION_LEN];
    GCHAR  szDate[HEAD_DATE_LEN];
    GCHAR  szVendor[HEAD_VENDOR_LEN];

    GUINT32 u4FormNumber;
    GUINT32 u4FileSize;

    GUINT32 u4ResWgtIDNumber;
    GUINT32 u4ResWgtIDOffset;

    GUINT32 u4ResTextIDNumber;
    GUINT32 u4ResTextIDOffset;

    GUINT32 u4ResIconIDNumber;
    GUINT32 u4ResIconIDOffset;
} GFORMDAT_FILE_HEAD_T;

//////////////////////////////////////////////////////////////////////////
// For Save a Binary Form Head Data
typedef struct _GWGTHEAD_INFO
{
    GCHAR   szFormName[FORM_NAME_LEN]; 

    GUINT32 u4FormTemplateOffset;
    GUINT32 u4FormCaptionTextOffset;
    GUINT32 u4FormStyleOffset;
    GUINT32 u4FormCaptonStyleOffset;
    GUINT32 u4FormFrameStyleOffset;

    GUINT32 u4WgtNumber;
    GUINT32 u4WgtItemOffset;

    GUINT32 u4DataLen;
} GWGTHEAD_INFO_T;

//////////////////////////////////////////////////////////////////////////
// For Resource ID Data Info
typedef struct _GRES_DATA
{
    GUINT64        u4Hash;
    GUINT32        u4Data;
    GUINT32        u4Padding; //\\ JK
} GRES_DATA_T;

#endif /* _GPARSE_CFG_H_ */