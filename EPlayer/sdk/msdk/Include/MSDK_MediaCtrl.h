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
* file MSDK_MediaCtrl.h
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
* $Revision: #27 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/

#ifndef _MSDK_MEDIACTRL_H_
#define _MSDK_MEDIACTRL_H_

#include <MSDK_Defs.h>


#ifdef __cplusplus
extern "C" {
#endif


/**@defgroup PlaybackControl PlaybackControl
*  
*  @brief the api of this module used as play back control. play pause stop seek and so on.
*  @{
*/

/**
*   @brief This method switches the entire filter graph into a running state. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaCtrl_Play(HMEDIAGRAPH hMediaGraph);

/**
*   @brief This method switches the entire filter graph into a pause state. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaCtrl_Pause(HMEDIAGRAPH hMediaGraph);

/**
*   @brief This method switches the entire filter graph into a stop state. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaCtrl_Stop(HMEDIAGRAPH hMediaGraph);

/**
*   @brief This method sets playback rate. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[in]  rRate new playback rate.where 1 is the normal rate, 2 is twice as fast, and so on. 
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaCtrl_SetRate(HMEDIAGRAPH hMediaGraph, E_MSDK_PLAY_SPEED rRate);

/**
*   @brief This method retrieves playback rate. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[out] prRate current rate 1  is normal play,2 is twice as fast, and so on. 
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaCtrl_GetRate(HMEDIAGRAPH hMediaGraph, E_MSDK_PLAY_SPEED *prRate);

/**
*   @brief This method retrieves playback capabilities. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[out] pCapabilities if zero,the media can not seek,otherise can use MediaCtrl_Seek 
*
*   @return MRESULT  define in msdk_error.h
*   
*   @see    msdk_defs.h  E_MSDK_CAPABILITY_T
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaCtrl_GetCapabilites(HMEDIAGRAPH hMediaGraph, DWORD *pCapabilities);

/**
*   @brief This method retrieves supported max playback rate(fast forward or fast rewind). 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[out] peMaxFFRate the file supported max fast forward rate.
*   @param[out] peMaxFFRate the file supported max rewind rate.
*
*   @return MRESULT  define in msdk_error.h
*   
*   @see    msdk_defs.h  E_MSDK_PLAY_SPEED
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaCtrl_GetSupportedMaxRate(HMEDIAGRAPH hMediaGraph, E_MSDK_PLAY_SPEED *peMaxFFRate,E_MSDK_PLAY_SPEED *peMaxRwRate);

/**
*   @brief This method set playback position. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[in]  eCurTimeFmt time format
*   @param[in]  u8Current new position set
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    E_MSDK_TIME_FORMAT
**/
MRESULT     MediaCtrl_Seek(HMEDIAGRAPH hMediaGraph, 
                           E_MSDK_TIME_FORMAT eCurTimeFmt,
                           UINT64 u8Current);

/**
*   @brief This method set playback start position and end position. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[in]  eStartTimeFmt start time format
*   @param[in]  u8Start new start position set
*   @param[in]  eEndTimeFmt   end time format
*   @param[in]  u8End new end position set
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    E_MSDK_TIME_FORMAT
**/
MRESULT     MediaCtrl_SetPlaybackRange(HMEDIAGRAPH hMediaGraph,  
                                       E_MSDK_TIME_FORMAT eStartTimeFmt,
                                       GUINT64 u8Start, 
                                       E_MSDK_TIME_FORMAT eEndTimeFmt, 
                                       GUINT64 u8End);

/**
*   @brief This method retrieves the length of time that the media stream will play. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[out] pu8Duration current media stream length in time.
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    E_MSDK_TIME_FORMAT
**/
MRESULT     MediaCtrl_GetDuration(HMEDIAGRAPH hMediaGraph, GUINT64 *pu8Duration);

/**
*   @brief This method retrieves the current position in terms of the total length of the media stream. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[out] pu8Current Current position in current time format units. 
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    E_MSDK_TIME_FORMAT
**/
MRESULT     MediaCtrl_GetCurrentPosition(HMEDIAGRAPH hMediaGraph, 
                                         GUINT64 *pu8Current);

/**
*   @brief This method set playback position. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[in]  eCurTimeFmt time format
*   @param[in]  u8Current new position set
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    E_MSDK_TIME_FORMAT
**/
MRESULT MediaCtrl_LastMemStart(HMEDIAGRAPH hMediaGraph, E_MSDK_TIME_FORMAT eCurTimeFmt, GUINT64 u8Current);



/*******************************************************************************
@Deprecate
以下函数不建议使用,其功能可以有其他函数替代，在以后的版本中将被取消
目前保留是为了兼容客户和公版
*******************************************************************************/
/** @deprecated
*        the method is deprecated,Consider using MediaCtrl_GetCapabilites instead.
*   @brief This method check the media fast forward capability. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[out] fgIsNoSupportFF true is sopportted fast forward
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*/
DEPRECATED(MediaCtrl_GetCapabilites)
MRESULT MediaCtrl_GetFgIsNoSupportFF(HMEDIAGRAPH hMediaGraph, BOOL *fgIsNoSupportFF);

/** @deprecated
*        the method is deprecated,Consider using AVIN_Play instead.
*   @brief This method switches the AVIN into a running state. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*/
DEPRECATED(AVIN_Play)
MRESULT MediaCtrl_PlayAvin(HMEDIAGRAPH hMediaGraph);

/** @deprecated
*        the method is deprecated,Consider using AVIN_Stop instead.
*   @brief This method switches the AVIN into a pause state. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*/
DEPRECATED(AVIN_Stop)
MRESULT MediaCtrl_StopAvin(HMEDIAGRAPH hMediaGraph);
/**
*@}
*/


#ifdef __cplusplus
}
#endif

#endif //_MSDK_MEDIACTRL_H_


