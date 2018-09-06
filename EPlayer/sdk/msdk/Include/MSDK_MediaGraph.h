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
* file MSDK_MediaGraph.h
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
* $Revision: #49 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/


#ifndef _MSDK_MEDIAGRAPH_H_
#define _MSDK_MEDIAGRAPH_H_


#include <MSDK_Defs.h>



#ifdef __cplusplus
extern "C" {
#endif

/**
  *@addtogroup PlaybackMechanism
 * @{
 *
 */

/**
*   @brief This method create media graph.
*
*   @param[out]     phMediaGraph pointer to media graph handle.
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    msdk_error.h
**/
MRESULT     MediaGraph_Create(HMEDIAGRAPH *phMediaGraph);

/**
*   @brief This method release media graph.
*
*   @param[in]  hMediaGraph media graph handle.
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*
*   @note the method might be called while app exit.media graph will be destroyed,all inteface be released also.
**/
MRESULT     MediaGraph_Release(HMEDIAGRAPH hMediaGraph);

/**
*   @brief This method clear media graph.
*
*   @param[in]  hMediaGraph media graph handle.
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*
*   @note the method might be called while stop play .mediagraph not be destroyed,but some members will be released .
**/
MRESULT     MediaGraph_Clear(HMEDIAGRAPH hMediaGraph);

/**
*   @brief This method set media graph config.repeat play and notify frequency
*
*   @param[in]  hMediaGraph media graph handle.
*   @param[in]  prConfig config param for mediagraph
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    MG_CONFIG_T
**/
MRESULT     MediaGraph_SetConfig(HMEDIAGRAPH hMediaGraph, MG_CONFIG_T *prConfig);

/**
*   @brief This method retrieves media graph config.
*
*   @param[in]  hMediaGraph media graph handle.
*   @param[out] prConfig current mediagraph config
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    MG_CONFIG_T
**/
MRESULT     MediaGraph_GetConfig(HMEDIAGRAPH hMediaGraph, MG_CONFIG_T *prConfig);

/**
*   @brief This method retrieves media graph state(play pause stop).
*
*   @param[in]  hMediaGraph media graph handle.
*   @param[out] peState current media graph state
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    E_MSDK_MGSTATE
**/
MRESULT     MediaGraph_GetState(HMEDIAGRAPH hMediaGraph, E_MSDK_MGSTATE *peState);

/**
*   @brief This method query interface by IID.
*
*   @param[in]  hMediaGraph media graph handle.
*   @param[in]  pIID MSDK_IID marked
*   @param[out] ppvInterface address of interface pointer
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    MSDK_IID
**/
MRESULT     MediaGraph_QueryInterface(HMEDIAGRAPH hMediaGraph, const MSDK_IID *pIID,
                                      GVOID **ppvInterface);

/**
*   @brief This method set media graph  source file.
*
*   @param[in]  hMediaGraph media graph handle.
*   @param[in]  szFileName file path will be render
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaGraph_SetSourceFile(HMEDIAGRAPH hMediaGraph, GTCHAR *szFileName);


/**
*   @brief This method force media progress stop.
*
*   @param[in]  hMediaGraph media graph handle.
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaGraph_TerminateLoad(HMEDIAGRAPH hMediaGraph);

/**
*   @brief This method set media graph display target.
*
*   @param[in]  hMediaGraph media graph handle.
*   @param[in]    eSiType  video display target to front ,rear or both
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/

MRESULT     MediaGraph_SetDataSink(HMEDIAGRAPH hMediaGraph,E_MSDK_SINK_TYPE eSiType);

/**
*     @brief This method set media graph  display information.
*
*     @param[in]    hMediaGraph media graph handle.
*     @param[in]    prSinkInfo config param for source
*
*     @return   MRESULT  define in msdk_error.h
*
*     @see       MediaGraph_Create()
*     @see       msdk_error.h
**/

MRESULT     MediaGraph_SetSinkInfo (HMEDIAGRAPH hMediaGraph,MSDK_SINK_INFO_T *prSinkInfo);

/**
*     @brief This method retrieves media infomation .
*
*     @param[in]    hMediaGraph media graph handle.
*     @param[in]    prMediaInfo media infomation struct
*
*     @return   MRESULT  define in msdk_error.h
*
*     @see       MediaGraph_Create()
*     @see       msdk_error.h
**/
MRESULT     MediaGraph_GetMediaInfo(HMEDIAGRAPH hMediaGraph,MSDK_MEDIA_INFO_T *prMediaInfo);

/**
*     @brief This method just switch video destination .
*
*     @param[in]    hMediaGraph media graph handle.
*     @param[in]    eSiType video destination
*
*     @return   MRESULT  define in msdk_error.h
*
*     @see       MediaGraph_Create()
*     @see       msdk_error.h
**/

MRESULT MediaGraph_SwitchVideo(HMEDIAGRAPH hMediaGraph, E_MSDK_SINK_TYPE eSiType);


/**********************************************************************************************
以下函数不建议使用,其功能可以有其他函数替代，在以后的版本中将被取消
目前保留是为了兼容客户和公版
************************************************************************************************/

/**
*@deprecated
*     This function is deprecated.Consider using VideoInfo_GetCount instead
**/
DEPRECATED(VideoInfo_GetCount)
MRESULT MediaGraph_GetVStmCnt(HMEDIAGRAPH hMediaGraph, UINT8 *u1VideoStreamCnt);

/**
*@deprecated
*     This function is deprecated.Consider using AVIN_SetSource instead
*@see AVIN.h
*@see AVIN_SetSource
**/
DEPRECATED(AVIN_SetSource)
MRESULT MediaGraph_SetVideoInSource(HMEDIAGRAPH hMediaGraph, E_MSDK_AVIN_VINDEX eVIndex,
                                    MSDK_AVIN_SINK_INFO_T *prSinkInfo);
/**
*@deprecated
*     This function is deprecated.Consider using AVIN_SetSource instead
*@see AVIN.h
*@see AVIN_SetSource
**/
DEPRECATED(AVIN_SetSource)
MRESULT MediaGraph_SetAudioInSource(HMEDIAGRAPH hMediaGraph,E_MSDK_AVIN_AINDEX eAIndex,
                                    MSDK_AVIN_SINK_INFO_T *prSinkInfo);

/**
*@deprecated
*     This function is deprecated.Consider using AVIN_SetSource instead
*@see AVIN.h
*@see AVIN_SetSource
**/
DEPRECATED(AVIN_SetSource)
MRESULT MediaGraph_SetAVInSource(HMEDIAGRAPH hMediaGraph,E_MSDK_AVIN_VINDEX eVIndex,
                                 E_MSDK_AVIN_AINDEX eAIndex, MSDK_AVIN_SINK_INFO_T *prSinkInfo);
/**
*@deprecated
*     This function is deprecated.Consider using AVIN_VideoInfo instead.
*@see AVIN.h
*@see AVIN_SetVideoInfo
*@see AVIN_SetDestinationRec
*@see AVIN_SetVideoVisiable
*
**/
DEPRECATED(AVIN_SetVideoInfo)
MRESULT MediaGraph_SetVideoInInfo(HMEDIAGRAPH hMediaGraph, MSDK_AVIN_SINK_INFO_T *prSinkInfo);

/**
*@deprecated
*      This function is deprecated.Consider using AVIN_SetDestnation instead.
*@see AVIN_SetDestination
**/
DEPRECATED(AVIN_SetDestnation)
MRESULT MediaGraph_SwitchAVInSink(HMEDIAGRAPH hMediaGraph,E_MSDK_SINK_TYPE eSinkType,E_MSDK_MODE eMode);

/**
*@deprecated
*      This function is deprecated.Consider using AVIN_IsVideoSignalReady instead.
*@see AVIN_IsVideoSignalReady
**/
DEPRECATED(AVIN_IsVideoSignalReady)
MRESULT MediaGraph_VdoInGetIsSignalReady(HMEDIAGRAPH hMediaGraph, BOOL *fgIsSignalReady);

/**
*@deprecated
*      This function is deprecated.Consider using AVIN_SetSourceRect instead.
*@see AVIN_SetSourceRect
**/
DEPRECATED(AVIN_SetSourceRect)
MRESULT MediaGraph_SetScale(HMEDIAGRAPH hMediaGraph,MSDK_SCALE_INFO_T *prScaleInfo);

/**
*@deprecated
*     This function is deprecated.Consider using AVIN_GetSpectrum instead.
*@see AVIN_GetSpectrum
**/
DEPRECATED(AVIN_GetSpectrum)
MRESULT MediaGraph_GetAudInSpec(HMEDIAGRAPH hMediaGraph, UINT32 *pu1SValues, UINT32 *pu1LValues);



/**
*@}
*/

#ifdef __cplusplus
}
#endif

#endif //_MSDK_MEDIAGRAPH_H_


