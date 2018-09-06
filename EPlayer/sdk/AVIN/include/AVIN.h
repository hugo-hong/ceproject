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
* file AVIN.h
*
* Project:
* --------
*   CNB
*
* Description:
* ------------
*
*
* Author:MTK40494
* -------
*
*
*------------------------------------------------------------------------------
* $Revision: #1 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/


#ifndef _AVIN_H_
#define _AVIN_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "Windows.h"
#include "ddraw.h"

/**avin source type.*/
typedef enum
{
    AVIN_SOURCE_NONE, /**< none source */
    AVIN_SOURCE_A,    /**< audio line in source*/
    AVIN_SOURCE_V,    /**< video line in source*/
    AVIN_SOURCE_AV    /**< av line in source*/
}E_AVIN_SOURCE_TYPE_T;

/**avin destination type.*/
typedef enum
{
    AVIN_DEST_FRONT,      /**<destination is front seat*/
    AVIN_DEST_REAR,       /**<destination is rear seat*/
    AVIN_DEST_FRONT_REAR  /**<destination both front and rear seat*/
}E_AVIN_DEST_TYPE_T;

/**video mirror type.*/
typedef enum
{
    AVIN_MIRROR_FLIP_NONE, /**<do not flip*/
    AVIN_MIRROR_FLIP_H,    /**<mirror flip horizon*/
    AVIN_MIRROR_FLIP_V,    /**<mirror flip vertical*/
    AVIN_MIRROR_FLIP_HnV   /**<mirror flip horizon and vertical */
}E_AVIN_MIRROR_TYPE_T;

/**video signal mode.*/
typedef enum
{
    AVIN_SIGNAL_PAL=1,     /**< PAL mode signal*/
    AVIN_SIGNAL_NTSC,      /**< ntsc mode signal*/
    AVIN_SIGNAL_SECAM,     /**< secam mode signal*/
    AVIN_SIGNAL_UNSTABLE,  /**< unstable mode signal*/
    AVIN_SIGNAL_NONE,      /**< none signal*/
}E_AVIN_SIGNAL_MODE_T;

/**video rear path type,if dram mode. 
* dram mode:front and rear seat must play the same channel source,that is front and rear show 
*           the same video.because the video overlay can merge with UI overlay,so the rear 
*           can show UI and video in the same time.
* bypass mode:front and rear seat can play different channel source,
*             but rear seat can not show UI.
*/
typedef enum
{
    AVIN_PATH_DRAM,
    AVIN_PATH_BYPASS,
}E_AVIN_REAR_PATH_T;

/** video overlay attribute. 
*  u4Flags:the value might be DDOVER_HIDE,DDOVER_SHOW DDOVER_KEYDESTOVERRIDE
*           DDOVER_KEYSRCOVERRIDE,
*  rOverlayFX: 
*@see DDOVERLAYFX
*/
typedef struct
{
    UINT32      u4Flags;
    DDOVERLAYFX rOverlayFX;   
}AVIN_VIDEO_INFO_T;

/**video signal state.*/
typedef enum
{
    AVIN_SIGNAL_READY,/**<video signal ready*/
    AVIN_SIGNAL_LOST, /**<video signal lost*/
    AVIN_SIGNAL_CHANGE/**<video signal mode change*/
}E_AVIN_SIGNAL_STATE;

/**avin channel index.
* channel index from one to five
*/
typedef enum
{
    AVIN_CHANNEL_NONE,
    AVIN_CHANNEL_1,
    AVIN_CHANNEL_2,
    AVIN_CHANNEL_3,
    AVIN_CHANNEL_4,
    AVIN_CHANNEL_5,
}E_AVIN_SOURCE_CHANNEL_T;


typedef VOID*  HAVINST;
typedef UINT32 RESULT;

#define AVINAPI _declspec(dllexport)
/*
#define S_AVIN_OK              1
#define E_AVIN_INVALID_PARAM   2
#define E_AVIN_FAILED          3
*/

enum 
{
    S_AVIN_OK = 0,
    E_AVIN_INVALID_PARAM, 
    E_AVIN_FAILED, 
    S_AVIN_RET_END,
};
/**
*   @brief This method initializes avin. 
*
*   @return RESULT  define in avin.h        
*
*   @note    must be called before msdk api used
*   
**/
AVINAPI RESULT  AVIN_Init();

/**
*   @brief This method deinitializes avin. 
*
*   @return RESULT  define in avin.h        
*
*   @note   must be called after avin exit,sure that all resource be released
**/
AVINAPI RESULT  AVIN_DeInit();

/**
*   @brief This method creates avin instace. the instace used for api parameter
*
*   @return RESULT define in avin.h
**/
AVINAPI HAVINST AVIN_Create();

/**
*   @brief This method release avin instace.
*   
*   @param[in]  hInst AVIN instance handle
*
*   @return RESULT define in avin.h
**/
AVINAPI RESULT  AVIN_Release(HAVINST hInst);

/**
*   @brief This method sets avin source for play.if the source parameter is valid, 
*          the stream will be played!
*   
*   @param[in]  hInst AVIN instance handle
*   @param[in]  eSrcType source type(A,V or AV) defined in E_AVIN_SOURCE_TYPE_T.
*   @param[in]  eVChannel the channel index for video line in.
*   @param[in]  eAChannel the channel index for audio line in.
*
*   @return RESULT define in avin.h
*
*   @see E_AVIN_SOURCE_TYPE_T
*   @see E_AVIN_SOURCE_CHANNEL_T
**/
AVINAPI RESULT  AVIN_SetSource( HAVINST hInst,
                        E_AVIN_SOURCE_TYPE_T eSrcType,
                        E_AVIN_SOURCE_CHANNEL_T eVChannel,
                        E_AVIN_SOURCE_CHANNEL_T eAChannel);

/**
*   @brief This method switch avin in to play state, 
*          the stream will be played!
*   
*   @param[in]  hInst AVIN instance handle
*   @return RESULT define in avin.h
*
**/
AVINAPI RESULT  AVIN_Play( HAVINST hInst);


/**
*   @brief This method switch avin in to stop state, 
*          the stream will be played!
*   
*   @param[in]  hInst AVIN instance handle
*   @return RESULT define in avin.h
*
**/
AVINAPI RESULT  AVIN_Stop( HAVINST hInst);

/**
*   @brief This method sets destination for played.you can select front seat rear seat 
*          or front and seat for played on.if this method not be called,the default 
*          destination is front seat.
*   
*   @param[in]  hInst AVIN instance handle
*   @param[in]  eDestType destination type(front,rear or front and rear).
*
*   @return RESULT define in avin.h
*
*   @see E_AVIN_DEST_TYPE_T
**/
AVINAPI RESULT  AVIN_SetDestnation(HAVINST hInst,E_AVIN_DEST_TYPE_T eDestType); 

/**
*   @brief This method sets the source rectangle and is clipped against the available video. 
*   
*   @param[in]  hInst AVIN instance handle
*   @param[in]  eDestType which video will be clipped
*   @param[in]  prRect the rectangle be used for clipped
*
*   @return RESULT define in avin.h
*
*   @see E_AVIN_DEST_TYPE_T
**/
AVINAPI RESULT  AVIN_SetSourceRect(HAVINST hInst,E_AVIN_DEST_TYPE_T eDestType,RECT *prRect);

/**
*   @brief This method sets the destination rectangle in window coordinates. 
*   
*   @param[in]  hInst AVIN instance handle
*   @param[in]  eDestType destination type(front,rear or front and rear)
*   @param[in]  prRect  the rectangle of the destination
*
*   @return RESULT define in avin.h
*
*   @see E_AVIN_SOURCE_TYPE_T
**/
AVINAPI RESULT  AVIN_SetDestinationRect(HAVINST hInst,E_AVIN_DEST_TYPE_T eDestType,RECT *prRect);

/**
*   @brief This method sets video visible. 
*   
*   @param[in]  hInst AVIN instance handle
*   @param[in]  eDestType destination type(front,rear or front and rear)
*   @param[in]  fgVisible  true is show video,false is hide vide
*
*   @return RESULT define in avin.h
*
*   @see E_AVIN_SOURCE_TYPE_T
**/
AVINAPI RESULT  AVIN_SetVideoVisible(HAVINST hInst,E_AVIN_DEST_TYPE_T eDestType, BOOL fgVisible);

/**
*   @brief This method sets video attribute. 
*   
*   @param[in]  hInst AVIN instance handle
*   @param[in]  eDestType destination type(front,rear or front and rear).
*   @param[in]  prVideoInfo  video attribute as hide, show, source color key,destnation color key etc.
*
*   @return RESULT define in avin.h
*
*   @see E_AVIN_SOURCE_TYPE_T
*   @see AVIN_VIDEO_INFO_T
**/
AVINAPI RESULT  AVIN_SetVideoInfo(HAVINST hInst,E_AVIN_DEST_TYPE_T eDestType,AVIN_VIDEO_INFO_T *prVideoInfo);

/**
*   @brief This method sets notify window. 
*   
*   @param[in]  hInst AVIN instance handle
*   @param[in]  hWnd  the window will obtain the message from avin module.
*   @param[in]  u4Msg the message used for avin notify
*
*   @return RESULT define in avin.h
*
*   @see E_AVIN_SOURCE_TYPE_T
*   @see AVIN_VIDEO_INFO_T
**/
AVINAPI RESULT  AVIN_SetNotifyWindow(HAVINST hInst ,HWND hWnd,UINT32 u4Msg);

/**
*   @brief This method sets rear video path mode.we support two mode(dram and bypass)
*          if the dram is selected,front and rear seat must play the same channel source,
*          if bypass mode is selected,front and rear seat can play different channel source,
*          but rear seat can not show UI.
*   
*   @param[in]  hInst AVIN instance handle
*   @param[in]  ePathMode  the mode for rear video line in play
*
*   @return RESULT define in avin.h
*
*   @see E_AVIN_REAR_PATH_T
**/
AVINAPI RESULT  AVIN_SetRearVideoPath(HAVINST hInst,E_AVIN_REAR_PATH_T ePathMode);

/**
*   @brief This method retrieves video signal state(signal or no signal).
*   
*   @param[in]  hInst AVIN instance handle
*   @param[out]  fgSigReady false is no signal otherwise signal is ready
*
*   @return RESULT define in avin.h
**/
AVINAPI RESULT  AVIN_IsVideoSignalReady(HAVINST hInst,BOOL* fgSigReady);

/**
*   @brief This method retrieves video signal mode.
*   
*   @param[in]  hInst AVIN instance handle
*   @param[out]  eSigMode signal mode be retrieved
*
*   @return RESULT define in avin.h
*   @see E_AVIN_SIGNAL_MODE_T
**/
AVINAPI RESULT  AVIN_GetVideoSignalMode(HAVINST hInst,E_AVIN_SIGNAL_MODE_T *eSigMode);

/**
*   @brief This method sets video hue.
*   
*   @param[in]  hInst AVIN instance handle
*   @param[in]  u4Hue hue be setted.
*
*   @return RESULT define in avin.h
**/
AVINAPI RESULT  AVIN_SetHue(HAVINST hInst,UINT32 u4Hue);

/**
*   @brief This method sets video brightness.
*   
*   @param[in]  hInst AVIN instance handle
*   @param[in]  u4Brightness brightness be setted
*
*   @return RESULT define in avin.h
**/
AVINAPI RESULT  AVIN_SetBrightness(HAVINST hInst,UINT32 u4Brightness);

/**
*   @brief This method sets video saturation.
*   
*   @param[in]  hInst AVIN instance handle
*   @param[in]  u4Saturation hue be setted
*
*   @return RESULT define in avin.h
**/
AVINAPI RESULT  AVIN_SetSaturation(HAVINST hInst,UINT32 u4Saturation);

/**
*   @brief This method sets video mirror type.
*   
*   @param[in]  hInst AVIN instance handle
*   @param[in]  eMirror mirror type for flipping
*
*   @return RESULT define in avin.h
*
*   @see E_AVIN_SIGNAL_MODE_T
**/
AVINAPI RESULT  AVIN_SetMirror(HAVINST hInst,E_AVIN_MIRROR_TYPE_T eMirror);

/**
*   @brief This method retrives audio line in spectrum.
*   
*   @param[in]  hInst AVIN instance handle
*   @param[out]  pu4SValues array for store short spectrum
*   @param[out]  pu4LValues array for store long spectrum
*
*   @return RESULT define in avin.h
**/
AVINAPI RESULT  AVIN_GetSpectrum(HAVINST hInst,UINT32* pu4SValues,UINT32 *pu4LValues);

#ifdef __cplusplus
}
#endif

#endif //_MSDK_AVIN_H_