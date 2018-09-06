/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
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

/*****************************************************************************
*
* Filename:
* ---------
*   GGdiLayer.h 
*
* Project:
* --------
*   GPS Pilot
*
* Description:
* ------------
*   
*   
* Author:
* -------
*   mcn03046
*
* Last changed:
* ------------- 
* $Author: zeng.zhang $ 
*
* $Modtime: $  
*
* $Revision: #1 $
****************************************************************************/

#ifndef _GGDILAYER_H_
#define _GGDILAYER_H_

#include "GDef.h"
#include "GRect.h"
#include "GGdi.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
*@addtogroup Graphics
*
*/

enum
{
    GLCAP_COLORKEY = 1 << 0,
    GLCAP_ALPHA = 1 << 1,
    GLCAP_COLORALPHA = 1 << 2,
    GLCAP_LOCK = 1 << 3,
    GLCAP_EXCLUSIVE = 1 << 4,
    GLCAP_DUMMY = 1 << 5,
    GLCAP_ALPHACHANNELON = 1 << 6
};


typedef struct
{
    GUINT16  u2PixelsWidth;
    GUINT16  u2PixelsHeight;
    GUINT32  u4BitsPerPixel;
} GGFXMODE_T;


/**
* @brief  init gdi layer
*
* @param[in] prGfxMode GGFXMODE_T * mode
* 
* @return GBOOL define in GDef.h
*  
* @see GGFXMODE_T
* 
**/
GAPI GBOOL       GInitGdiLayer(GGFXMODE_T *prGfxMode);
/**
* @brief  change gdi layer
*
* @param[in] prGfxMode GGFXMODE_T * mode
* 
* @return GBOOL define in GDef.h
*  
* @see GGFXMODE_T
* 
**/
GAPI GBOOL       GChngGdiLayer(GGFXMODE_T *prGfxMode);

/**
* @brief  deinit gdi layer
**/
GAPI GVOID       GDeinitGdiLayer(GVOID);

GAPI GVOID       GBlt2Layer(GUINT32 u4LayerID, GINT32 i4DstX, GINT32 i4DstY, 
                           GUINT32 u4Width, GUINT32 u4Height, 
                           HSURFACE hSurf, GINT32 i4SrcX, GINT32 i4SrcY);
GAPI GVOID       GBltLayers2Screen(HSURFACE hSurf, GRECT_T *prRect);

#if DDRAW_PANEL_MOVE
GAPI GVOID       GBltLayers2ScreenDdraw(GRECT_T *prRect);    // for ddraw   pwz   20100823
GAPI GVOID       GBltLayers2ScreenDdrawOverlay(BOOL bFront,GRECT_T *prRect);    // for ddraw   pwz   20100823
#endif 

GAPI GVOID       GBltLayers2Surface(HSURFACE hSurf, GINT32 i4X, GINT32 i4Y, GRECT_T *prRect);
GAPI GVOID       GBltImgFile2Screen(GWCHAR *wszImgFile, GRECT_T *prRect);

GAPI GVOID       GSuspendBltToScreen();
GAPI GVOID       GResumeBltToScreen();

GAPI BOOL        SaveBakeSurfaceToSceen(void);

//Layer
GAPI GVOID       GSetLayer(GUINT32 u4LayerID, HSURFACE hSurf);
/**
* @brief  get layer surface
*
* @param[in] u4LayerID layer id
* 
* @return HSURFACE define in GDef.h
*  
**/
GAPI HSURFACE    GGetLayerSurface(GUINT32 u4LayerID);
/**
* @brief  move layer to (x,y)
*
* @param[in] u4LayerID layer id
* @param[in] i4X x-coordinate
* @param[in] i4Y y-coordinate
* 
**/
GAPI GVOID       GMoveLayer(GUINT32 u4LayerID, GINT32 i4X, GINT32 i4Y);
/**
* @brief  get layer position
*
* @param[in] u4LayerID layer id
* @param[in] prPos GPOINT_T * layer position
* 
* @see GPOINT_T
* 
**/
GAPI GVOID       GGetLayerPos(GUINT32 u4LayerID, GPOINT_T *prPos);
/**
* @brief  set layer color key
*
* @param[in] u4LayerID layer id
* @param[in] u4Color color key
* 
**/
GAPI GVOID       GSetLayerColorKey(GUINT32 u4LayerID, GUINT32 u4Color);
/**
* @brief  get layer color key
*
* @param[in] u4LayerID layer id
* 
* @return GUINT32 define in GDef.h
*
**/
GAPI GUINT32     GGetLayerColorKey(GUINT32 u4LayerID);
/**
* @brief  set layer color alpha
*
* @param[in] u4LayerID layer id
* @param[in] u4Color color value
* @param[in] uAlpha alpha value
* 
* @return GBOOL define in GDef.h
*
**/
GAPI GBOOL       GSetLayerColorAlpha(GUINT32 u4LayerID, GUINT32 u4Color,
                                    GUINT8 uAlpha);
/**
* @brief  get layer color alpha
*
* @param[in] u4LayerID layer id
* @param[in] u4Color color value
* 
* @return GUINT8 define in GDef.h
* 
**/
GAPI GUINT8      GGetLayerColorAlpha(GUINT32 u4LayerID, GUINT32 u4Color);
/**
* @brief  clear layer all color alpha
*
* @param[in] u4LayerID layer id
* 
**/
GAPI GVOID       GClearLayerAllColorAlpha(GUINT32 u4LayerID);
/**
* @brief  clear layer color alpha
*
* @param[in] u4LayerID layer id
* @param[in] u4Color color value
* 
**/
GAPI GVOID       GClearLayerColorAlpha(GUINT32 u4LayerID, GUINT32 u4Color);
/**
* @brief  set layer alpha
*
* @param[in] u4LayerID layer id
* @param[in] uAlpha alpha value
* 
**/
GAPI GVOID       GSetLayerAlpha(GUINT32 u4LayerID, GUINT8 uAlpha);
/**
* @brief  get layer alpha
*
* @param[in] u4LayerID layer id
* 
* @return GUINT8 define in GDef.h
*
**/
GAPI GUINT8      GGetLayerAlpha(GUINT32 u4LayerID);
/**
* @brief  set layer on/off
*
* @param[in] u4LayerID layer id
* @param[in] fgOn on/off
* 
**/
GAPI GVOID       GSetLayerOn(GUINT32 u4LayerID, GBOOL fgOn);
/**
* @brief  judge layer on/off
*
* @param[in] u4LayerID layer id
* 
* @return GBOOL define in GDef.h
*
**/
GAPI GBOOL       GIsLayerOn(GUINT32 u4LayerID);
/**
* @brief  set layers caps
*
* @param[in] u4LayerID layer id
* @param[in] u4Caps layer caps
*
*
**/
GAPI GVOID       GSetLayerCaps(GUINT32 u4LayerID, GUINT32 u4Caps);
/**
* @brief  get layers caps
*
* @param[in] u4LayerID layer id
*
* @return GUINT32 define in GDef.h
*
**/
GAPI GUINT32     GGetLayerCaps(GUINT32 u4LayerID);


GAPI GVOID       GSetLayersScrOrg(GINT32 i4X, GINT32 i4Y);
GAPI GVOID       GGetLayersScrOrg(GPOINT_T *prOrgPt);
/**
* @brief  get merge surface
*
*
* @return HSURFACE define in GDef.h
*			   
**/
GAPI HSURFACE    GGetMergeSurface(GVOID);

#ifdef __cplusplus
}
#endif

#endif /* _GGDILAYER_H_ */