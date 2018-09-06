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
* file MSDK_Video.h
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
* $Revision: #21 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/

#ifndef _MSDK_VIDEO_H_
#define _MSDK_VIDEO_H_

#include <MSDK_Defs.h>

#include <GRect.h>
#include "ddraw.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum{
    VIDEO_NONE,
    VIDEO_STRETCH
}Video_Stretch;

typedef enum {
    VIDEO_POS_FULL_SCREEN,
    VIDEO_POS_CENTER,
    VIDEO_POS_TOP,
    VIDEO_POS_LEFT,
    VIDEO_POS_BOTTOM,
    VIDEO_POS_RIGHT,
    VIDEO_POS_TOP_LEFT,
    VIDEO_POS_BOTTOM_LEFT,
    VIDEO_POS_TOP_RIGHT,
    VIDEO_POS_BOTTOM_RIGHT
}Video_Pos;


typedef enum{
    WM_EFFECT_NONE,
    WM_EFFECT_GRADUAL_CHANGE,
    WM_EFFECT_MAX
}WindowMode_Effect;

/**
 * @defgroup MediaCtrl MediaCtrl
 * this module defined functions for config media playback
 *   
 * @{
 */
 

/**
*   @brief this method set video display attribute
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[in]  u4Top  The y-axis origin of the destination window. 
*   @param[in]  u4Left  The x-axis origin of the destination window. 
*   @param[in]  u4Width The width of the destination window.
*   @param[in]  u4Height The height display position
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/
MRESULT     VideoCtrl_SetDestinationAttr(HVIDEOCTRL hVideoCtrl, GUINT32 u4Top, GUINT32 u4Left, GUINT32 u4Width, GUINT32 u4Height);

/**
*@}
*/

/**
*@defgroup MediaInfo MediaInfo
* this module defined functions to retrieve media information 
* 
*@{
*/

/**
*   @brief This method retrieves the current video width. 
*
*   @param[in]  hVideoInfo  video info handle
*   @param[out]  pu4Width GUINT32* video width
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT     VideoInfo_GetWidth(HVIDEOINFO hVideoInfo, GUINT32 *pu4Width);

/**
*   @brief This method retrieves the current video height. 
*
*   @param[in]  hVideoInfo  video info handle
*   @param[out]  pu4Height  video height
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT     VideoInfo_GetHeight(HVIDEOINFO hVideoInfo, GUINT32 *pu4Height);

/**
*   @brief This method retrieves the cideo stream count. 
*
*   @param[in]  hVideoInfo  video info handle
*   @param[out]  pu4Count  video stream count
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT     VideoInfo_GetCount(HVIDEOINFO hVideoInfo, GUINT32 *pu4Count);
/**
*@}
*/


/**
 * @addtogroup MediaCtrl
 * @{
 */

/**
*   @brief This method sets source position
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[in]      i4X The x-axis origin of the source rectangle
*   @param[in]  i4Y The y-axis origin of the source rectangle
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT     VideoCtrl_SetSourcePosition(HVIDEOCTRL hVideoCtrl, INT32 i4X, INT32 i4Y);

/**
*   @brief This method sets source size
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[in]      i4Width new width of the source rectangle
*   @param[in]  i4Height new height of the source rectangle
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT     VideoCtrl_SetSourceSize(HVIDEOCTRL hVideoCtrl, INT32 i4Width, INT32 i4Height);

/**
*   @brief This method retrieves source position
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[out]     pi4X the x-axis origin of the source rectangle
*   @param[out]     pi4Y the y-axis origin of the source rectangle
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT     VideoCtrl_GetSourcePosition(HVIDEOCTRL hVideoCtrl, INT32 *pi4X, INT32 *pi4Y);

/**
*   @brief This method retrieves source size
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[out]     pi4Width the width of the source rectangle
*   @param[out]     pi4Height the height of the source rectangle
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT     VideoCtrl_GetSourceSize(HVIDEOCTRL hVideoCtrl, INT32 *pi4Width, INT32 *pi4Height);

/**
*   @brief This method sets destination position
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[in]      i4X the x-axis origin of the destination rectangle
*   @param[in]  i4Y the y-axis origin of the destination rectangle
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT     VideoCtrl_SetDestinationPosition(HVIDEOCTRL hVideoCtrl, INT32 i4X, INT32 i4Y);

/**
*   @brief This method sets destination size
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[in]      i4Width the width of the destnation rectangle
*   @param[in]  i4Height the height of the destnation rectangle
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT     VideoCtrl_SetDestinationSize(HVIDEOCTRL hVideoCtrl, INT32 i4Width, INT32 i4Height);

/**
*   @brief This method retrieves destination position
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[out]     pi4X the x-axis origin of the destination rectangle
*   @param[out]     pi4Y the y-axis origin of the destination rectangle
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT     VideoCtrl_GetDestinationPosition(HVIDEOCTRL hVideoCtrl, INT32 *pi4X, INT32 *pi4Y);

/**
*   @brief This method retrieves destination size.
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[out]     pi4Width the width of the destination rectangle
*   @param[out]     pi4Height the height of the destination rectangle
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT     VideoCtrl_GetDestinationSize(HVIDEOCTRL hVideoCtrl, INT32 *pi4Width, INT32 *pi4Height);


/**
*   @brief This method sets an owning parent for the video window. 
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[in]      hOwner Handle of new owner window. 
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT     VideoCtrl_SetOwner(HVIDEOCTRL hVideoCtrl, HWND hOwner);

/**
*   @brief This method retrives an owning parent for the video window. 
*
*   @param[in]  hVideoCtrl  video control handle.
*   @param[in]      phOwner Retrieved window handle. 
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT     VideoCtrl_GetOwner(HVIDEOCTRL hVideoCtrl, HWND *phOwner);

/**
*   @brief This method sets video window position by prRect. 
*
*   @param[in]  hVideoCtrl  video control handle.
*   @param[in]      prRect  GRECT_T pointer specifies  the video window rectangle. 
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*   @see    GRECT_T
*
**/
MRESULT     VideoCtrl_SetWindowRect(HVIDEOCTRL hVideoCtrl, GRECT_T *prRect);

/**
*   @brief This method retrieves video window rectangle. 
*
*   @param[in]  hVideoCtrl  video control handle.
*   @param[out]     prRect  retrivees GRECT_T value of  the video window rectangle. 
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*   @see    GRECT_T
*
**/
MRESULT     VideoCtrl_GetWindowRect(HVIDEOCTRL hVideoCtrl, GRECT_T *prRect);

/**
*   @brief  This method changes the visibility of the video window. 
*
*   @param[in]  hVideoCtrl  video control handle.
*   @param[in]      fgVisible  GBOOL value for video window visibility. 
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*   
*   @note   If the Visible parameter is set to TRUE , the window is shown. If it is set to FALSE , the window is hidden
**/
MRESULT     VideoCtrl_SetVideoVisible(HVIDEOCTRL hVideoCtrl, GBOOL fgVisible);

/**
*   @brief  This method retrieves the visibility of the video window. 
*
*   @param[in]  hVideoCtrl  video control handle.
*   @param[in]      pfgVisible  video window visibility. 
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*   
**/
MRESULT     VideoCtrl_IsVideoVisible(HVIDEOCTRL hVideoCtrl, GBOOL *pfgVisible);

/**
*   @brief  This method sets destination colorkey of the video. 
*
*   @param[in]  hVideoCtrl  video control handle.
*   @param[in]  ddoverlay   video overlay color key values. 
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*   
**/
MRESULT     VideoCtrl_SetDestColorKey(HVIDEOCTRL hVideoCtrl,DDOVERLAYFX ddoverlay);




/*******************************************************************************
@Deprecate
以下函数不建议使用,其功能可以有其他函数替代，在以后的版本中将被取消
目前保留是为了兼容客户和公版
*******************************************************************************/
#define DEPRECATED_VIDEO _declspec(deprecated("This method is deprecated, The function not supported"))
/** @deprecated
*       This method is deprecated.The function is not supportted.
*   @brief this method set video display attribute
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[in]  eVideoStretch video stretch flag
*   @param[in]  eVideoPos   video display position
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/
DEPRECATED_VIDEO
MRESULT     VideoCtrl_SetVideoAttr(HVIDEOCTRL hVideoCtrl, Video_Stretch eVideoStretch, Video_Pos eVideoPos);

/** @deprecated
 *      This method is deprecated.The function is not supportted.
 *   @brief this method Get video display attribute
 *
 *   @param[in]  hVideoCtrl  video control handle
 *   @param[in]  u4Width video width
 *   @param[in]  u4Height video height
 *   @param[in]  eVideoStretch video stretch flag
 *   @param[in]  eVideoPos   video display position
 *   @param[out] prRect video rect
 *
 *   @return MRESULT  define in msdk_error.h
 *              
 *   @see     MediaGraph_QueryInterface()
 *   @see     msdk_error.h
 *
 **/
 DEPRECATED_VIDEO
 MRESULT VideoCtrl_GetVideoAttr(HVIDEOCTRL hVideoCtrl, GUINT32 u4Width,GUINT32 u4Height,Video_Stretch eVideoStretch, Video_Pos eVideoPos,RECT *prRect);


/** @deprecated
*       This method is deprecated.The function is not supportted.
*   @brief this method move window
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[in]  prDestPoint  move window to destination position
*
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/
DEPRECATED_VIDEO
MRESULT     VideoCtrl_MoveWindow(HVIDEOCTRL hVideoCtrl, GPOINT_T *prDestPoint);

/** @deprecated
*       This method is deprecated.The function is not supportted.
*   @brief this method judge window mode
*
*   @param[in]  hVideoCtrl  video control handle
*   
*   @return GBOOL  TRUE or FALSE
*              
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/
DEPRECATED_VIDEO
GBOOL       VideoCtrl_IsWindowMode(HVIDEOCTRL hVideoCtrl);

/** @deprecated
*       This method is deprecated.The function is not supportted.
*   @brief this method set window mode effect
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[in]  eEffect  window mode effect to set
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/
DEPRECATED_VIDEO
MRESULT     VideoCtrl_SetWindowModeEffect(HVIDEOCTRL hVideoCtrl, WindowMode_Effect eEffect);

/** @deprecated
*       This method is deprecated.The function is not supportted.
*   @brief this method retrieve window mode effect
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[in]  peEffect  window mode effect
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/

DEPRECATED_VIDEO
MRESULT     VideoCtrl_GetWindowModeEffect(HVIDEOCTRL hVideoCtrl, WindowMode_Effect *peEffect);


/** @deprecated
*      This method is deprecated.The function is not supportted.
*   @brief this method entry window mode
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[in]  prVideoRect  window  destination rectangular
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/
DEPRECATED_VIDEO
MRESULT     VideoCtrl_EnterWindowMode(HVIDEOCTRL hVideoCtrl, GRECT_T *prVideoRect);

/** @deprecated
*      This method is deprecated.The function is not supportted.
*   @brief this method exit window mode
*
*   @param[in]  hVideoCtrl  video control handle
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/
//_declspec(deprecated("The method deprecated,The function not supportted"))
DEPRECATED_VIDEO
MRESULT     VideoCtrl_ExitWindowMode(HVIDEOCTRL hVideoCtrl);

/** @deprecated
*      This method is deprecated.The function is not supportted.
*   @brief this method change window mode size 
*
*   @param[in]  hVideoCtrl  video control handle
*   @param[in]  prVideoRect  window  destination rectangular
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/

DEPRECATED_VIDEO
MRESULT     VideoCtrl_ChangeWindowModeSize(HVIDEOCTRL hVideoCtrl, GRECT_T *prVideoRect);

/**
*@}
*/

#ifdef __cplusplus
}
#endif

#endif //_MSDK_VIDEO_H_
