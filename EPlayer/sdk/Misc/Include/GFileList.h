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
* file GFileList.h
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
#ifndef _GFILELIST_H_
#define _GFILELIST_H_

#include "windows.h"
#include "GDef.h"
#include "GKal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
*  @defgroup FileList  FileList 
*
*  @brief The module for searching file to form list and play list management.
*  @{
*/
#ifdef MAX_PATH
  #undef MAX_PATH
#endif
#define MAX_PATH 300

#ifdef MAX_TYPE_NUM
	#undef MAX_TYPE_NUM
#endif

#define MAX_TYPE_NUM 50
#define MAX_TYPE_LEN 10

#define INVALID_INDEX 0xFFFFFFFF
#define INVALID_PARAM 0xFFFFFFFF

	
	typedef void(*GCALL_BACK)(GHANDLE hwnd,GHANDLE usr_data, GUINT32 u4Param1,GUINT32 u4Param2,GUINT32 u4Evt);
/**
* @brief  The method must be called before file list module to initializes the file list.
*
* @return GRESULT define in GDef.h
*
**/
	GRESULT GFileListInit();
/**
* @brief  The method be called while exit file list module to release unused resources.
*
* @return GRESULT define in GDef.h
*
**/
	GRESULT GFileListDeInit();

/**
* @brief  The method is searching files under the folder,these files include media files and flolders.
*
* @param[in] dir All search result under this directory
* @param[in] types no used, set NULL
* @param[in] extends The string contains one or more media file extend names,these extentions be devide by comma or semicolon
* @param[in] user_data The user defined parameter for callback function.
* @param[in] call_func The function is register while method be called worked as callback function.
*
* @return GHANDLE define in GDef.h
*
**/		
	GHANDLE   GSearchFileAll(GTCHAR *dir,GTCHAR *types,GTCHAR *extends,GHANDLE user_data,GCALL_BACK call_func);
/**
* @brief  The method is searching media files under the folder and it's sub folder 
*
* @param[in] dir All search result under this directory and sub folders under this directory.
* @param[in] types no used, set NULL
* @param[in] extends The string contains one or more media file extend names,these extentions be devide by comma or semicolon
* @param[in] user_data The user defined parameter for callback function.
* @param[in] call_func The function is register while method be called worked as callback function.
*
* @return GHANDLE define in GDef.h
*
**/	
	GHANDLE   GSearchFileByType(GTCHAR *dir,GTCHAR *types,GTCHAR *extends,GHANDLE user_data,GCALL_BACK call_func);
/**
* @brief  If the search command is executing,the new new search command is comming,this method must called to stop old one.
*
* @param[in] hFileList file list handle
*
* @return GRESULT define in GDef.h
*
**/	
	GRESULT   GStopSearch(GHANDLE hFileList);
/**
* @brief  The search is asynchronous,so,if we are playing media,we should refresh play list which formed before.
*
* @param[in] hPlayList play list handle
*
* @return GRESULT define in GDef.h
*
**/	
	GRESULT	  GRefreshPlayList(GHANDLE hPlayList);

/**
* @brief  Exit searching,must release the serach result 
*
* @param[in] hFileList file list handle
*
* @return GRESULT define in GDef.h
*
**/	
	GRESULT GReleaseFileList(GHANDLE hFileList);
/**
* @brief  Sort serarch result. 
*
* @param[in] hFileList file list handle
*
* @return GRESULT define in GDef.h
*
**/
	GRESULT GSortFileList(GHANDLE hFileList);

/**
* @brief  The method filter search results to form new play list for playback.
*
* @param[in] hFileList The handle of file list searched before
* @param[in] playTypes The media type be selected for play 
*
* @return GRESULT define in GDef.h
*
**/
	GHANDLE GFilterList(GHANDLE hFileList,GTCHAR *playTypes);
/**
* @brief  Abort the filter,release play list.
*
* @param[in] hFileList play list handle
*
* @return GRESULT define in GDef.h
*
* @note unused
**/
	GRESULT GAbortFilter(GHANDLE hFileList);
/**
* @brief  The method retieves  index in play list  by index of file list.
*
* @param[in] hFileList file list handle
* @param[out] u4PlayIndex The current file index in play list
* @param[in] u4FileIndex index in file list
*
* @return GRESULT define in GDef.h
*
**/
	GRESULT GGetPlayIndex(GHANDLE hFileList,GUINT32 *u4PlayIndex,GUINT32 u4FileIndex);
/**
* @brief  The method retrieves index in file list by the file index in play list.
*
* @param[in] hFileList file list handle
* @param[in] u4PlayIndex Index of file in  play list
* @param[out] u4FileIndex Index of file in file list
*
* @return GRESULT define in GDef.h
*
**/
	GRESULT GGetFileIndex(GHANDLE hFileList,GUINT32 u4PlayIndex,GUINT32 *u4FileIndex);

/**
* @brief  The method retrieves file information from file list.
*
* @param[in] hFileList file list handle
* @param[in] u4Index The index of file be retrieved in file list
*
* @return GHANDLE define in GDef.h
*
**/
	GHANDLE  GGetFileItem(GHANDLE hFileList,GUINT32 u4Index);

/**
* @brief  The method retrieves file name from file
*
* @param[in] hFileInfo The handle of file
* @param[out] fileName The name of file  
*
* @return GRESULT define in GDef.h
*
**/
	GRESULT GGetFileName(GHANDLE hFileInfo,GTCHAR **fileName);
/**
* @brief  get file directory
*
* @param[in] hFileInfo The handle of file
* @param[out] fileDir  The parent directory of file
*
* @return GRESULT define in GDef.h
*
**/
	GRESULT GGetFileDir(GHANDLE hFileInfo,GTCHAR *fileDir);
/**
* @brief  The method retrieves full path include directory and file name.
*
* @param[in] hFileInfo The handle of file
* @param[out] filePath The full path of file
*
* @return GRESULT define in GDef.h
*
**/
	GRESULT GGetFilePath(GHANDLE hFileInfo,GTCHAR *filePath);
/**
* @brief  The method retrieves file's extension
*
* @param[in] hFileInfo The handle of file
* @param[out] fileExt The extension of file
*
* @return GRESULT define in GDef.h
*
**/
	GRESULT GGetFileExt(GHANDLE hFileInfo,GTCHAR *fileExt);
/**
* @brief  The method retrieves file's attribute of directory or not.
*
* @param[in] hFileInfo The handle of file
* @param[out] fgDir  True is directory,False or not.
*
* @return GRESULT define in GDef.h
*
**/
	GRESULT GIsDirectory(GHANDLE hFileInfo,GBOOL *fgDir);

/**
* @brief  The method for querying current path.
*
* @param[out] curPath The path of current directory.
*
* @return GUINT32 define in GDef.h
*
**/		
	GUINT32 GQueryCurPath(GTCHAR *curPath);
/**
* @brief  The method for querying parent path by current path.
*
* @param[in] curPath  The string of current path
* @param[out] parentPath  The string of parent path
*
* @return GUINT32 define in GDef.h
*
**/	
	GUINT32 GQueryParentPath(GTCHAR *curPath,GTCHAR *parentPath);
/**
* @brief  The method for querying  the count of the file list.
*
* @param[in] hFileList file list handle
*
* @return GUINT32 define in GDef.h
*
**/	
	GUINT32 GQueryTotalNum(GHANDLE hFileList);

/**
* @brief  The method set root path of searching.
*
* @param[in] root The path is root directory to be searched.
*
* @return GRESULT define in GDef.h
*
**/	
	GRESULT GSetRootPath(GTCHAR *root);

/**
* @brief  The method set cur path of searching.
*
* @param[in] curpath The path is cur directory to be searched.
*
* @return GRESULT define in GDef.h
*
**/ 
GRESULT GSetCurPath(GTCHAR *curPath);

/**
* @brief  The method responsibles for register window for handling message from searching thread.  
*
* @param[in] hWnd The handle of window
*
* @return GRESULT define in GDef.h
*
**/
	GRESULT GSetFileSearchNotifyWindow(HWND hWnd);
/**
* @brief  The method sets the count of one page,the searching result not be displayed until the count,
*
* @param[in] u4Num The page count to be setted
*
* @return GRESULT define in GDef.h
*
**/
	GRESULT GSetPageNum(GUINT32 u4Num);

/**
*@}
*/
	
  

#ifdef __cplusplus
}
#endif

#endif