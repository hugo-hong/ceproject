/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
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
*   GMG_Module.h 
*
* Project:
* --------
*   MiGO
*
* Description:
* ------------
*   This file define API for attach/detach every module of MiGO.
*   
* Author:
* -------
*   MTK40043 Daowen Deng
*
* Last changed:
* ------------- 
* $Author: zeng.zhang $ 
*
* $Modtime: $  
*
* $Revision: #1 $
****************************************************************************/
#ifndef __GMG_MODULE_H__
#define __GMG_MODULE_H__

#include "GDef.h"

#define MG_MOD_DESC_STR_SZ 8 // must be 4-byte alignment

/*
 * MiGO module attributes
 */
enum
{
   /* PART I
    * System Kernel Initialization Error
    * FileSystem
    */
   MG_MOD_KAL = 0,   //GInitKal
   MG_MOD_OBJECT,  // GInitObject();   
   MG_MOD_INI,       //GInitPrefs
   
   MG_MOD_CFGCHECK,
   /* Application platform configuration and setting history Data MiGO.dat */ 
   MG_MOD_PLATFORMENV, //g_rSetting.Init()  platform.ini  Only
   MG_MOD_HISTROYENV,  //m_pPersist->LoadMMFInfo();  MiGO.dat, fieldtry_u.mcf MiGO_CFG.ini

   /* System Debug Environment */
   MG_MOD_DEBUGENV, //GInitLogEx(szLogFileName);   

   /* System based module Initialization */
   MG_MOD_GPS,           //  GPS_OPEN() instead of _gGpsManager.Init ?
   MG_MOD_HANDWRITE,     //    GHwInitInstance();
   MG_MOD_INITSOUND,   // GInitSound();
   MG_MOD_PLAYSOUND,  // GInitPlayService(NULL);
   MG_MOD_PLAYTTS,       //GPlayTTSInit();

   /* System based Architeche initialization */
   MG_MOD_UIEVTCTRL,   //InitEventHdr
   MG_MOD_GUIKITRENDER, //GRegGUIKitRenderFunc(RenderMiGOUI);  Only
   MG_MOD_GISMAPRENDER, //GRegGisMapRenderFunc(RenderMiGOUI);  Only

   MG_MOD_UITIMER,     // InitUITimer

   MG_MOD_INIRES, //GIniResInit(&rResArg);   auto-match different display format?
   MG_MOD_PARSER,//GInitParse(&rParserArg);
   MG_MOD_GDI,       // GInitGdi(&rGdiArg);
   MG_MOD_GDILAYER,              //GInitGdiLayer(&rGFXMODE);  


   MG_MOD_GUIKIT,     // GInitGUIKit(&rArg);
   MG_MOD_UIFASHION,  // set Alpha, color key and so on. Gdilayer attribute   Only

   MG_MOD_GUIKITTHEME, // GLoadTheme(&_rPNCTheme);   Only
   MG_MOD_SURFACE,     // InitSurface();  How to implement WaterMark ? 
   
   MG_MOD_GISHISTROYENV, //GInitDataPath(szTemp1, szTemp2, szTemp3); Only Route Data, Track Data, WayPoint Data
   MG_MOD_GISMAPRES,     //GInitThemePath(szTemp1);   Only *.mcf and *.bmp
   MG_MOD_GIS,           //GInitGIS(szTemp1, &rCurrentLoc.rPos);  Only MiGO_GIS.mcf
   MG_MOD_LANGUAGE,      //g_rSetting.SetTextLang(eText);  Only

   MG_MOD_LOGO,        //ShowStartLogo   Only

   /* Middle-ware initialization 
    * It is pure software module, we should slove these errors during design phase. 
    */
   MG_MOD_VOICEGUIDANCE, //   GVoicePromptInit(FALSE);
   
   MG_MOD_MAX
};


typedef struct __structMG_Module_   // must be dword alignment
{
  // private space, default for internel operation.
//  GDWORD dwDummy[3];
  
  // set by user
  GDWORD dwMagic; // MIGOMOD_MAGIC_NUM
  GDWORD dwStructSz; // this data structure's size, 
  GWORD  wMainVer; // main version, increase when interface changed
  GWORD  wSubVer; // sub version, increase when bug fixed..etc
  GTCHAR  pbDesc[MG_MOD_DESC_STR_SZ]; // module name
  GRESULT (*dwInit)(GVOID*);  // init routine
  GRESULT (*dwExit)(GDWORD dwCause); // exit routine  dwCause to identify different exit case. card memory plug out etc.
  GTCHAR*(*szInitCause)(GDWORD); // if init fail, this function will give some failed comment.
  GTCHAR*(*szExitCause)(GDWORD); // if Exit fail, this function will give some failed comment.
  GDWORD dwAttribute;  // assigned by user, indicate properties of module
  GDWORD dwInitOrderNum;     // init order number.
} MGMOD_T, * PMGMOD_T;

typedef struct _structPBVerInfor_T
{
    GTCHAR * szSinature;
    GTCHAR * szProjectName;
    GTCHAR * szVersionNum;
    GTCHAR * szReleaseNum;
    GTCHAR * szMapVendor;
    GTCHAR * szMapVersion;
    GTCHAR * szCustomer;
}GPBVERINFOR_T, *PGPBVERINFOR_T;

#define RET_VERINFOR_UNEXPECTED MAKE_GERR(MOD_APP, 1)
#define RET_CFGFILE_UNEXPECTED MAKE_GERR(MOD_APP, 2)

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*  when init/exit fail. 
**********************************************
  [module name](Magic Number)
  Mainver number
  SubVer  number
  by cause: 
  [szInitCause/szExitCause]
  
  (type any key to exit)
 *********************************************
 */
 
 #endif  /* __GMG_MODULE_H__ */