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
*   GIniRes.h 
*
* Project:
* --------
*   MiGO
*
* Description:
* ------------
*   
*   
* Author:
* -------
*   zywang@mtk.com.cn
*
* Last changed:
* ------------- 
* $Author: zeng.zhang $ 
*
* $Modtime: $  
*
* $Revision: #1 $
****************************************************************************/


#ifndef ___GINIRES_H___
#define ___GINIRES_H___

#include "GDef.h"

#include "GResource.h"

///////////////////////////////////////////////////////////////////////////////
// 1: Load String/Icon Resource from Binary File
// 0: Load String/Icon Resource from INI File
#define CFG_LOAD_RES_FROM_DAT            1

#define INIRES_ICONRES_RESIDENT          1
#define INIRES_STRINGRES_RESIDENT        1
#define INIRES_PITCH_WIDTH               (30*1024L)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum __enumImageStyle__
{
    IMAGE_NONE = 0,
    IMAGE_SMART,
    IMAGE_CLIP
}IMAGESTYLE_T, *PIMAGESTYLE_T;

typedef struct __structSmartImage__
{
    GDWORD  dwMethod;
}SMARTIMAGE_T, *PSMARTIMAGE_T;

typedef struct __structClipImage__
{
    GWORD  x;
    GWORD  y;
    GWORD  w;
    GWORD  h;
}CLIPIMAGE_T, *PCLIPIMAGE_T;

typedef struct __structImgeAttribute__
{
    IMAGESTYLE_T  eStyle;
    union {
        struct __structSmartImage__ rSmart;
        struct __structClipImage__  rClip;
    }Do; 
}IMGEATTR_T,*PIMGEATTR_T;

// Global Functions
GAPI GRESULT     GIniSetLanguage( GUINT32 u4Type);
GAPI GRESULT     GIniSetLanguageEx(const GTCHAR* lpTextResName);

GAPI GRESULT     GGetLanguageRes();

GAPI GHANDLE     GIniLoadString(GUINT16 u2Id);
GAPI GVOID       GIniDestroyString(GHANDLE hStr);
 
GAPI GHANDLE     GIniLoadImage(GUINT16 u2Id);
GAPI GVOID       GIniDestroyImage(GHANDLE hImage);

GAPI GUINT32     GIniImgMakeIntDllID(GUINT16 u2ID);

GAPI GBOOL       GIniImageAttribute(GUINT16 u2Id, PIMGEATTR_T prImgAttr);


#if !CFG_LOAD_RES_FROM_DAT
#define GGETSTRINGID(ID)                ((GUINT16)GIniStrMakeIntID(L#ID))
#define GGETICONID(ID)                  ((GUINT16)GIniImgMakeIntID(L#ID))
GAPI GUINT16     GIniImgMakeIntID(const GTCHAR * wcsKeyString);
GAPI GUINT32     GIniImgMakeIntDllIDByAlias(const GTCHAR * wcsKeyString);
GAPI GUINT16     GIniStrMakeIntID(const GTCHAR * wcsKeyString);
GBOOL  GIniTextResInit(GTCHAR* lpTextResName);
GVOID  GIniTextResDeinit(GTCHAR *szOutFileName);

GBOOL  GIniIconResInit(GTCHAR* lpIconResName);
GVOID  GIniIconResDeinit(GTCHAR *szOutFileName);
#endif

#ifdef __cplusplus
}
#endif

#endif /* ___GINIRES_H___ */
