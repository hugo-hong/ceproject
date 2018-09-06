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
 *   GResource.h
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
 *   Yuankang Li (mcn03005)
 *
 *------------------------------------------------------------------------------
 * $Revision: #1 $
 * $Modtime:$
 * $Log:$
 *
 *******************************************************************************/
#ifndef _GRESOURCE_H_
#define _GRESOURCE_H_

#include "GDef.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
    * @defgroup Resource Resource
    * @brief Provides resource access service of platform.
    * @ingroup Utilities
    */
    /*@{*/

    /* Resource Special Error Code */
    #define RET_RES_FAIL            MAKE_GERR(MOD_PAL_RES, ERR_FAIL)
    #define RET_RES_ID_NOT_ENOUGH   MAKE_GERR(MOD_PAL_RES, 2)
    #define RET_RES_CHECKSUM_FAIL   MAKE_GERR(MOD_PAL_RES, 3)
    #define RET_RES_INVAILD_ID      MAKE_GERR(MOD_PAL_RES, 4)
	#define RET_RES_BACK_LANGUAGE   MAKE_GERR(MOD_PAL_RES, 5)

    /* Init/Deinit Functions */
    typedef enum
    {
        RES_CACHE_ALL = 0,
        RES_CACHE_INDEX,
        RES_CACHE_NONE
    } RES_CACHE_MODE_T;


    typedef enum _LanguageType
    {
        LANG_EN = 0,
        LANG_CH = 1,
        LANG_TC = 2,
        LANG_JP = 3,
        LANG_PG = 4,
        LANG_SA = 5,
        MAXLANGUAGETYPE
    } LANGUAGETYPE_T;


    typedef struct _IniResEntryParam_
    {
        //GTCHAR* wcsIconScriptFile;
        //GTCHAR* wcsStringScriptFile;

        GTCHAR* wcsDataBaseFile;
        GUINT32 u4LangResId;
    } INIRESENTRYPARAM_T, *PINIRESENTRYPARAM_T;


    typedef GVOID (*PFN_IMAGEDECODE_NOTIFY)(GHANDLE hImage, GUINT32 u4Evt,
                                            GUINT32 u4CustomData, GUINT32 u4Result);


    GAPI GRESULT GInitResource(OUT GUINT32 *pu4ResId, const GTCHAR *pFilePath, 
                               RES_CACHE_MODE_T eCacheMode);
    GAPI GRESULT GDeinitResource(GUINT32 u4ResId);

    /* 
     * u4Id = Resource ID(return from GInitResource) + String/Image Id 
     */
    #define GRES_ID(u4Id)    (u4Id&0xFFFF0000)
    #define GSUB_ID(u4Id)    (u4Id&0x0000FFFF)
    #define G_ID(u4ResId, u4SubId)  ((u4ResId&0xFFFF0000) + (u4SubId&0x0000FFFF))

    /*
    * pResTypeName
    *      String    - VERSION, STRING, TEXT, 
    *      Data file - BITMAP, LAYOUT, LANG(UAGE)
    */
    GAPI GHANDLE GLoadRes(const GTCHAR *pResTypeName, GUINT32 u4Id, GUINT32 *pu4RawSize);
    GAPI GUINT32 GGetResSize(const GTCHAR *pResTypeName, GUINT32 u4Id);
    GAPI GUINT32 GDestroyRes(GHANDLE hRes);

    /* Resource - String */
    GAPI GHANDLE     GLoadString(GUINT32 u4Id);
    GAPI GUINT32     GDestroyString(GHANDLE hStr);

    /* Resource - Image */
    GAPI GHANDLE     GLoadImage(GUINT32 u4Id);
    GAPI GHANDLE     GLoadImageByName(GTCHAR* lpszBmpName);
    GAPI GUINT32     GDestroyImage(GHANDLE hImage);

    /* Resource - Sound */
    GAPI GHANDLE     GLoadSound(GUINT32 u4Id);
    GAPI GUINT32     GDestroySound(GHANDLE hSound);

    GAPI GHANDLE     GLoadRawData(const GTCHAR *pResTypeName, 
                         GUINT32 u4Id, 
                         GUINT32 u4StartPos, 
                         GUINT32 u4Len, 
                         GUINT32 *pu4ReadBytes);

    /* Draw Image File from MRF Resource */
    GAPI GVOID       GDrawMRFImage(GHANDLE hGC, 
                         GINT32 x, GINT32 y,
                         GTCHAR *pFilePath,  
                         GTCHAR *pResTypeName, GUINT32 u4Id);

    // Global Functions
	GAPI GRESULT     GGetLanguage();
    GAPI GRESULT     GSetLanguage(LANGUAGETYPE_T eType);
    GAPI GRESULT     GSetLanguageEx(const GTCHAR* lpszFullName);
    GAPI GRESULT     GSetImageResource(const GTCHAR* lpszFullName);
    GAPI GRESULT     GDeinitAppResource(GVOID);

    GAPI GUINT32     GGetLanguageResId(GVOID);
    GAPI GUINT32     GGetImageResId(GVOID);

    // Resource: String
    GAPI GHANDLE     GLoadAppString(GUINT16 u2Id);
    GAPI GVOID       GDestroyAppString(GHANDLE hStr);
    // Resource: Image    
    GAPI GHANDLE     GLoadAppImage(GUINT16 u2Id);
    GAPI GVOID       GDestroyAppImage(GHANDLE hImage);

    GAPI GRESULT     GIniResInit(PINIRESENTRYPARAM_T prParam);
    GAPI GVOID       GIniResDeinit(GVOID);

    /*@}*/ 

#ifdef __cplusplus
}
#endif

#endif /* _GRESOURCE_H_ */
