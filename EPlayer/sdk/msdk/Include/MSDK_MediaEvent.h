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
* file MSDK_MediaEvent.h
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
* $Revision: #28 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/

#ifndef _MSDK_MEDIAEVENT_H_
#define _MSDK_MEDIAEVENT_H_

#include <MSDK_Defs.h>


#ifdef __cplusplus
extern "C" {
#endif
/**
 * @addtogroup PlaybackMechanism
 * @{
 *
 */


//msdk event enumrator
enum
{
    EVT_MSDK_FINISHED,                  /**<play finished                                    */
    EVT_MSDK_PLAYBACK_POSITION_STATUS,  /**<playback position change                         */  
    EVT_MSDK_PLAYED,                    /**<play state                                       */
    EVT_MSDK_PAUSED,                    /**<pause state                                      */
    EVT_MSDK_STOPED,                    /**<stop state                                       */
    EVT_MSDK_GRAPH_CHANGED,             /**<media graph changed                              */
    EVT_MSDK_GRAPH_BUILT,               /**<graph build successful                           */
    EVT_MSDK_LYRICITEM_CHANGED,         /**<u4Param1 is the index of the lyric item          */
    EVT_MSDK_VIDEO_SIZE_CHANGED,        /**<vido size change                                 */
    EVT_MSDK_ERROR_OCCURED,             /**<error occurred                                   */
    EVT_MSDK_TIP_WARNING,               /**<tip warning for app tips                         */

    EVT_MSDK_SET_SUBTITLE_SUCCEED,      /**<change subtitle stream succed                    */
    EVT_MSDK_SET_SUBTITLE_FAILED,       /**<change subtitle stream failed                    */
    EVT_MSDK_SET_AUDIO_SUCCEED,         /**<change audio stream succed                       */
    EVT_MSDK_SET_AUDIO_FAILED,          /**<change audio stream failed                       */
    EVT_MSDK_SEEK_SUCCEED,              /**<seek operation succed                            */
    EVT_MSDK_SEEK_FAILED,               /**<seek operation failed                            */
    EVT_MSDK_SET_RATE_SUCCEED,          /**<set rate succed                                  */
    EVT_MSDK_SET_RATE_FAILED,           /**<set rate faileded                                */
   
    EVT_MSDK_AVIN_SIGNAL_READY,         /**video in signal ready                             */
    EVT_MSDK_AVIN_SIGNAL_LOST,          /**video in signal lost                              */
    
#if ENABLE_ISDBT
    EVT_MSDK_DTV_SCAN_NOTIFY,
    EVT_MSDK_DTV_SCAN_FINISH,
    EVT_MSDK_DTV_RESCAN_NOTIFY,
    EVT_MSDK_DTV_SELECT_OK,
    EVT_MSDK_DTV_SELECT_FAIL,
    EVT_MSDK_DTV_SIGNAL_LEVEL,
    EVT_MSDK_DTV_PCR_CHANGE,
    EVT_MSDK_DTV_EPG_OK,
    EVT_MSDK_DTV_EPG_FAIL,
    EVT_MSDK_DTV_TIME_CHANGE,
    EVT_MSDK_DTV_NO_DATA,
    EVT_MSDK_DTV_SD_NOSPACE,
#endif
};

/**
*   @brief   This method retrieves the next notification event. 
*
*   @param[in]  hMediaGraph  mediagraph handle.
*   @param[out] pu4EventCode next event notification.
*   @param[out] pu4Param1 First parameter of the event.
*   @param[out] pu4Param2 Second parameter of the event. 
*   @param[in]  i4TimeOut in milliseconds, to wait before assuming that there are no events. 
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaGraph_GetEvent(HMEDIAGRAPH hMediaGraph, GUINT32 *pu4EventCode, 
                                GUINT32 *pu4Param1, GUINT32 *pu4Param2, 
                                GINT32 i4TimeOut);

/**
*   @brief   This method add listener to media graph. 
*
*   @param[in]  hMediaGraph  mediagraph handle.
*   @param[in]  pfnListener call back function as listener.
*   @param[in]  u4CustomData listener function parameter.
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    PFN_MSDK_MEDIAEVENT_LISTENER
**/
MRESULT     MediaGraph_AddListener(HMEDIAGRAPH hMediaGraph, 
                                   PFN_MSDK_MEDIAEVENT_LISTENER pfnListener,
                                   GUINT32 u4CustomData);

/**
*   @brief   This method remove listener from media graph. 
*
*   @param[in]  hMediaGraph  mediagraph handle.
*   @param[in]  pfnListener the istener will be removed.
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    PFN_MSDK_MEDIAEVENT_LISTENER
**/
MRESULT     MediaGraph_RemoveListener(HMEDIAGRAPH hMediaGraph, 
                                      PFN_MSDK_MEDIAEVENT_LISTENER pfnListener);

/**
*   @brief   This method dispatch event. 
*
*   @param[in]  hMediaGraph  mediagraph handle.
*   @param[in]  u4EventCode event notification code.
*   @param[in]  u4Param1 first event parameter
*   @param[in]  u4Param2 second event parameter
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaGraph_DispatchEvent(HMEDIAGRAPH hMediaGraph, GUINT32 u4EventCode,
                                     GUINT32 u4Param1, GUINT32 u4Param2);

/**
*   @brief   This method handle media graph event. 
*
*   @param[in]  hMediaGraph  mediagraph handle.
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaGraph_HandleEvent(HMEDIAGRAPH hMediaGraph);

/**
*   @brief   This method registers a window that will handle messages in response to all events from an object. 
*
*   @param[in]  hMediaGraph  mediagraph handle.
*   @param[in]  hWnd Handle of window to notify. Pass NULL to stop notification.
*   @param[in]  u4Message Window message to be passed as the notification. 
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaGraph_RegisterNotifyWindow(HMEDIAGRAPH hMediaGraph, HWND hWnd, GUINT32 u4Message);

/**
*@}
*/

#ifdef __cplusplus
}
#endif

#endif //_MSDK_MEDIAEVENT_H_


