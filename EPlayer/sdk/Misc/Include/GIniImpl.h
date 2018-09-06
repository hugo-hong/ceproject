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
* GIniImpl.h
*
* Project:
* --------
*   MIGO
*
* Description:
* ------------
*  This header file is API for Ini file operation.
*
* Author:
* -------
*  Daowen.Deng@Mediatek.com 82228 MTK40043
*
*------------------------------------------------------------------------------
* $Revision: #1 $ 
* $Modtime:$ 
* $Log:$
*
*******************************************************************************/
#ifndef _GINIIMPL_H_
#define _GINIIMPL_H_

#define WIN32_LEAN_AND_MEAN
#include "GDef.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#if GB_NEW_INI
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

GRESULT  Ini_OpenPrefs(HMCF *pMcf, const GTCHAR *szFileName, GUINT32 u4OpenFlag);

GRESULT Ini_OpenPrefsFromBuf(HMCF *ppMcf, GTCHAR *szTxtBuf, GUINT32 u4CharCount, GUINT32 u4OpenFlag);

// Fill data value string to Key of Section of MCF object handled.
GRESULT  Ini_SetKeyData(HMCF hMcf, const GTCHAR *szSection, const GTCHAR *szKey,  const GTCHAR *szValue);

// Retrieve data from the Key of section of hMcf
GUINT32  Ini_GetKeyData(HMCF hMcf, const GTCHAR *szSection, const GTCHAR *szKey,  GTCHAR *pBuffer, GINT32 u4Size);

// Check the specified section exist or not.
GRESULT  Ini_FindSection(HMCF hMcf, const GTCHAR *szSection);

// Check the specified key exist or not.
GRESULT  Ini_FindKey(HMCF hMcf, const GTCHAR *szSection, const GTCHAR *szKey);

GRESULT  Ini_RemoveSection(HMCF hMcf, const GTCHAR *szSection);

GRESULT  Ini_RemoveKey(HMCF hMcf, const GTCHAR *szSection, const GTCHAR *szKey);

GRESULT Ini_FlushMcf(HMCF hMcf);

GRESULT Ini_ClosePrefs(HMCF *ppMcf);

#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define  ERROR_NOFOUN_DATA  (-1)

#define  SAFE_FREE(p)       { if (p) GFree(p); p = NULL; }

#ifdef __cplusplus
extern "C" {
#endif

GBOOL    INI_START(const GTCHAR* iniFile);
GVOID    INI_END();

// Operations
int     Ini_GetData(const GTCHAR* section, const GTCHAR* key,  GTCHAR* buffer, GUINT32 u4size);
GBOOL    Ini_SetData(const GTCHAR* section, const GTCHAR* key,  const GTCHAR* value);

// Helper function
GBOOL    Ini_AppendKey(const GTCHAR* section, const GTCHAR* key, const GTCHAR* value);
GBOOL    Ini_RemoveKey(const GTCHAR* section, const GTCHAR* key);
GBOOL    Ini_RemoveSection(const GTCHAR* section);
GBOOL    Ini_FindSection(const GTCHAR* section);
GUINT32  Ini_GetSectionName(GTCHAR* buffer, GUINT32 u4Size);
GUINT32  Ini_GetSection(const GTCHAR* section, GTCHAR* buffer, GUINT32 u4Size);
GINT32   Ini_GetKeyCount(const GTCHAR* section);
GINT32   Ini_GetKeyName(const GTCHAR* section,  GUINT32 u4Index, GTCHAR* buffer, GUINT32 u4size);
//GBOOL    Ini_EnumKey(const GTCHAR* section);
GBOOL    Ini_Save(const GTCHAR* iniFile);

#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif // _GINIIMPL_H_
