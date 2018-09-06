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
* file MSDK_Lyrics.h
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
* $Revision: #3 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/

#ifndef _MSDK_LYRICS_H_
#define _MSDK_LYRICS_H_

#include <MSDK_Defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @addtogroup MediaInfo
* @{
**/

/**
*   @brief This method get lyric item count. 
*
*   @param[in]  hLyricInfo lyric infomation handle
*   @param[out] *pu4Count  the count of the lyric
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT LyricInfo_GetCount(HLYRICINFO hLyricInfo, UINT32 *pu4Count);

/**
*   @brief This method get display time of the item by index. 
*   @param[in]  hLyricInfo lyric infomation handle
*   @param[in]  i4Index  the index of the lyric
*   @param[in]  *pu4Pts  the display time of the lyric item 
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT LyricInfo_GetPts(HLYRICINFO hLyricInfo, INT32 i4Index, UINT32 *pu4Pts);

/**
*   @brief This method get lyric item text by index. 
*   @param[in]  hLyricInfo lyric infomation handle
*   @param[in]  i4Index  the index of the lyric
*   @param[out] *szLyric  the lyric item text 
*   @param[in] u4MaxLen  the maximum length of the lyric item 
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT LyricInfo_GetText(HLYRICINFO hLyricInfo, INT32 i4Index, GTCHAR *szLyric, UINT32 u4MaxLen);

/**
*   @brief This method get index by current time. 
*   @param[in]  hLyricInfo lyric infomation handle
*   @param[in]  u8CurTime  current playback time
*   @param[out] *pu4Index  the index of lyric 
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_QueryInterface()
*   @see    msdk_error.h
*
**/
MRESULT LyricInfo_GetIndexByPts(HLYRICINFO hLyricInfo, GUINT64 u8CurTime, UINT32 *pu4Index);

/**
*@}
*/
#ifdef __cplusplus
}
#endif

#endif //_MSDK_LYRICS_H_


