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
* file MSDK_Audio.h
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
* $Revision: #16 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/

#ifndef _MSDK_AUDIO_H_
#define _MSDK_AUDIO_H_

#include <MSDK_Defs.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup MediaCtrl
 *
 * @{
 */

/**
*   @brief  set volume for current audio
*
*   @param[in]   hAudioCtrl handle of audio control
*   @param[in]     i4Volume integer value be used for set
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/
MRESULT     AudioCtrl_SetVolume(HAUDIOCTRL hAudioCtrl, GINT32 i4Volume);

/**
*   @brief  get current audio volume
*
*   @param[in]   hAudioCtrl handle of audio control
*   @param[out]   pi4Volume integer pointer be used for store audio volume
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT     AudioCtrl_GetVolume(HAUDIOCTRL hAudioCtrl, GINT32 *pi4Volume);

/**
*   @brief  set balance for audio
*
*   @param[in]   hAudioCtrl handle of audio control
*   @param[out]   i4Balance integer value to be set
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT     AudioCtrl_SetBalance(HAUDIOCTRL hAudioCtrl, GINT32 i4Balance);

/**
*   @brief  Get balance for audio
*
*   @param[in]   hAudioCtrl handle of audio control
*   @param[out]   pi4Balance integer pointer be used for store audio balance
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT     AudioCtrl_GetBalance(HAUDIOCTRL hAudioCtrl, GINT32 *pi4Balance);

/**
*@}
*/


/**
 * @addtogroup MediaInfo
 *
 * @{
 */

/**
*   @brief  get audio spectrums of all band,values from 0 to 0x20000
*
*   @param[in]   hAudioInfo  audio info handle
*   @param[out]   pu4SValues UINT32 array for low values
*   @param[out]   pu4LValues UINT32 array for bar values
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT AudioInfo_GetSourceSpectrums(HAUDIOINFO hAudioInfo,UINT32 *pu4SValues,UINT32 *pu4LValues);

/**
*   @brief  get audio spectrum of specify band,values from 0 to 0x20000
*
*   @param[in]   hAudioInfo  audio info handle
*   @param[in]   u1Band  specify band
*   @param[out]   pu4SValue UINT32  pointer to low value
*   @param[out]   pu4LValue UINT32 pointer to bar value
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT AudioInfo_GetSourceSpectrum(HAUDIOINFO hAudioInfo,UINT8 u1Band,UINT32 *pu4SValue,UINT32 *pu4LValue);

/**
*   @brief  get audio spectrum's bands number
*
*   @param[in]   hAudioInfo  audio info handle
*   @param[out]   pu1BandsNum UINT8  pointer
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT AudioInfo_GetSpectrumBands(HAUDIOINFO hAudioInfo,UINT8 *pu1BandsNum);
/**
*@}
*/

/**
 * @addtogroup MediaCtrl
 *
 * @{
 */

/**
*   @brief get  audio stream num
*
*   @param[in]   hAudioCtrl  audio control handle
*   @param[out]      u1Cnt UINT8  pointer
*
*   @return    MRESULT  define in msdk_error.h
*
*   @note one audio file might have multiple audio stream,like english france chinese and so on
*      you can obtain the count by the method
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT AudioCtrl_GetCount(HAUDIOCTRL hAudioCtrl,UINT8 *u1Cnt);

/**
*   @brief get  audio stream index which is playing
*
*   @param[in]   hAudioCtrl  audio control handle
*   @param[out]  u1Index UINT8  pointer value in range(1~num)
*
*   @return    MRESULT  define in msdk_error.h
*
*   @see     AudioCtrl_GetCount()
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT AudioCtrl_GetCurrentAudio(HAUDIOCTRL hAudioCtrl,UINT8 *u1Index);

/**
*   @brief set  play audio stream
*
*   @param[in]   hAudioCtrl  audio control handle
*   @param[in]   u1Index UINT8  value in range(1~num)
*
*   @return    MRESULT  define in msdk_error.h
*
*   @see     AudioCtrl_GetCount()
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT AudioCtrl_SetCurrentAudio(HAUDIOCTRL hAudioCtrl,UINT8 u1Index);

/**
*   @brief get audio language code by index
*
*   @param[in]   hAudioCtrl  audio control handle
*   @param[in]     u1Index UINT8  value in range(1~stream num)
*   @param[out]     pu4LangID UINT32 pointer for language ID
*
*   @return    MRESULT  define in msdk_error.h
*
*   @see     AudioCtrl_GetCount()
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
*   @note   not implement
**/
MRESULT AudioCtrl_GetLanguage(HAUDIOCTRL hAudioCtrl,UINT8 u1Index,UINT32 *pu4LangID);
/**
*@}
*/






/*******************************************************************************
@Deprecate
以下函数不建议使用,其功能可以有其他函数替代，在以后的版本中将被取消
目前保留是为了兼容客户和公版
*******************************************************************************/

/**
 * @addtogroup MediaCtrl
 *
 * @{
 */

/** 
*      This function may be delete future. Consider using MediaCtrl_GetCapabitlity instead.
*   @brief  the method check if the audio supportted 
*
*   @param[in]   hAudioCtrl  audio control handle
*   @param[out]   pBfgAudSupport true is supportted
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*   @deprecated
**/
DEPRECATED(MediaCtrl_GetCapabilites) 
MRESULT AudioCtrl_GetAudFgSupport(HAUDIOCTRL hAudioCtrl, BOOL *pBfgAudSupport);
/**
*@}
*/

/**
 * @addtogroup MediaInfo
 *
 * @{
 */

/** @deprecated
*        The value range is fixed from 0 to 0x20000.
*   @brief  get audio spectrum's max value
*
*   @param[in]   hAudioInfo  audio info handle
*   @param[out]   pu1MaxVal UINT8  pointer
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/

DEPRECATED(0X20000)
MRESULT AudioInfo_GetSpectrumMaxVal(HAUDIOINFO hAudioInfo,UINT8 *pu1MaxVal);

/** @deprecated
*       The max value can not be set,the value is fixed to 0x20000
*   @brief set  max value for audio spectrum's
*
*   @param[in]   hAudioInfo  audio info handle
*   @param[in]   u1MaxVal max value
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/

//_declspec(deprecated("spectrum max value is fixed(0X20000),not support seeting"))
DEPRECATED(0x20000)
MRESULT AudioInfo_SetSpectrumMaxVal(HAUDIOINFO hAudioInfo,UINT8 u1MaxVal);



/** @deprecated
*        This function get the values has been mapped. Consider using AudioInfo_GetSourceSpectrums instead.
*   @brief  get audio spectrums of all band
*
*   @param[in]   hAudioInfo  audio info handle
*   @param[out]   pu1SValues UINT8  array for low values
*   @param[out]   pu1LValues UINT8 array for  bar values
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
DEPRECATED(AudioInfo_GetSourceSpectrums) 
MRESULT AudioInfo_GetSpectrums(HAUDIOINFO hAudioInfo,UINT8 *pu1SValues,UINT8 *pu1LValues);

/** @deprecated
*      This function get the values has been mapped. Consider using AudioInfo_GetSourceSpectrum instead.
*   @brief  get audio spectrum of specify band
*
*   @param[in]   hAudioInfo  audio info handle
*   @param[in]   u1Band  specify band
*   @param[out]   pu1SValue UINT8  pointer to low value
*   @param[out]   pu1LValue UINT8 pointer to bar value
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
DEPRECATED(AudioInfo_GetSourceSpectrum) 
MRESULT AudioInfo_GetSpectrum(HAUDIOINFO hAudioInfo,UINT8 u1Band,UINT8 *pu1SValue,UINT8 *pu1LValue);
/**
*@}
*/



#ifdef __cplusplus
}
#endif

#endif //_MSDK_AUDIO_H_
