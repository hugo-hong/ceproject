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
* file GImaging.h
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

#ifndef _GMTKIMAGING_H_
#define _GMTKIMAGING_H_

#include <GDef.h>

#include <GGdi.h>

#include  <imaging.h>
#define DRAW_IMAGE_VIDEO_OVERLAY 0
#define DECODE_SOURCE_RESOLUTION 0 // 1  decode with  raw  0 decode with no raw
#define PIC_OP_ZOOM1 0
#define PIC_OP_ZOOM2 0

#define MIN_ZOOM_IN		(1.0/4)
#define MAX_ZOOM_OUT	(4)

typedef enum
{
	CLOCKWISE_90,
	CLOCKWISE_180,
	CLOCKWISE_270,
	CLOCKWISE_360
}GRotateLevel;

typedef enum
{
	RESOLUTION_CHANGE,
	RESOLUTION_UNCHAGE,
	RESOLUTION_EQUAL_RATIO
}GRotateResolution;

#define WINDOW_SHUTTER_NUM       10

#define PIC_RESIZE_TO_4_TIMES		4
#define PIC_RESIZE_TO_2_TIMES		2
#define PIC_RESIZE_TO_1_2			(1.0 / 2)
#define PIC_RESIZE_TO_1_4			(1.0 / 4)


typedef struct
{
    GUINT32     u4Width;
	GUINT32     u4Height;
}GIMAGEINFO_T;

enum
{
	EVT_IMGDEC_OK,
	EVT_IMGDEC_ERROR
};

#if PIC_OP_ZOOM2
typedef enum __IMAGE_ZOOM_RESULT__
{
	IMAGE_ZOOM_IN,
	IMAGE_NORMAL,
	IMAGE_ZOOM_OUT
}Image_zoom_result;
#endif

typedef GVOID (*PFN_IMAGEDECODE_NOTIFY_EX)(GHANDLE hImage, GUINT32 u4Evt,
                                           GUINT32 u4CustomData, GUINT32 u4Result);

typedef GVOID (*PFN_IMAGESHOW_NOTIFY_EX)();


typedef enum __IMAGE_SHOW_EFF__
{	
	WINDOW_NONE,
	FROM_LEFT_TO_RIGHT,
	FROM_RIGHT_TO_LEFT,
	FROM_TOP_TO_BOTTOM,
	FROM_BOTTOM_TO_TOP,
	EXTEND_FROM_CENTER_H,
	EXTEND_FROM_CENTER_V,
	MOVE_IN_FROM_TOP,
	MOVE_IN_FROM_BOTTOM,
	MOVE_IN_FROM_LEFT,
	MOVE_IN_FROM_RIGHT,
	WINDOW_SHUTTER,
	SLIDE_SHOW_MAX
}Image_Show_Eff;

typedef enum _IMG_OUTPUT_T_
{
    IMG_OUTPUT_FRONT,
    IMG_OUTPUT_REAR,
    IMG_OUTPUT_FRONT_REAR,
} IMG_OUTPUT_T;

typedef struct
{
	IMG_OUTPUT_T	eImgOutput;
	Image_Show_Eff 	eImageShowEff;
	RECT 			rFrontDestRect;
	RECT 			rRearDestRect;
}IMG_SHOW_T;

extern IMG_SHOW_T		g_rImageShow;
extern PFN_IMAGESHOW_NOTIFY_EX	g_pfnImgShowNotify;


#ifdef __cplusplus
extern "C" {
#endif
GRESULT GSlideShowUI(GHANDLE hImage, RECT rect, Image_Show_Eff showEff);
GRESULT GStopSlideShowUI(GHANDLE hImage);

GRESULT GDrawImageWitchEff(GHANDLE hImage, HDC hDC, RECT rect, Image_Show_Eff showEff);
BITMAPINFO* GGetBitmapHeader(GHANDLE hImage);
GVOID GReleaseBitmapHeader(void *bmiptr);
GVOID GDrawBitmapDC(HDC hDC, GHANDLE hBitmap, RECT *prRect);

GRESULT GDrawImgWithSlideShow(HGC hGC, GHANDLE hBitmap, PFN_IMAGESHOW_NOTIFY_EX pfnNotify, IMG_SHOW_T rImageShow);

GBOOL GResize(GHANDLE hImage, GFLOAT level);

GBOOL GRotate(GHANDLE hImage, GRotateLevel eLevel, GRotateResolution eResolution);

GBOOL GRotateEx(GHANDLE hImage, GBOOL fgClockwise, GUINT32 i4DstW, GUINT32 i4DstH);

GVOID *GImage_QueryIImageInterface(GHANDLE hImage);

GVOID GImage_ReleaseIImageInterface(GVOID *pImage);

GBOOL GImage_GetBitmapDataStruct(GHANDLE hImage, GVOID *pbpData);

GBOOL GImage_ReleaseBitmapDataStruct(GVOID *pbpData);

/**
* @brief  init imaging
*
* @return GBOOL define in GDef.h
*
**/		
GBOOL       GInitImaging(GVOID);
/**
* @brief  deinit imaging
*
* @return GBOOL define in GDef.h
*
**/	
GVOID       GDeinitImaging(GVOID);

/**
* @brief  create image from file
*
* @param[in] szFileName GTCHAR * file name
*
* @return GHANDLE define in GDef.h
*
**/		
GHANDLE     GCreateImageFromFile(GTCHAR *szFileName);

/**
* @brief  create image from buffer
*
* @param[in] pvBuf GVOID * the buffer address
* @param[in] u4Size buffer size
*
* @return GHANDLE define in GDef.h
*
**/	
GHANDLE     GCreateImageFromBuffer(CONST GVOID *pvBuf, GUINT32 u4Size);
/**
* @brief  get image thumbnail
*
* @param[in] hImage image handle
* @param[in] u4Width image width
* @param[in] u4Height image height
*
* @return GHANDLE define in GDef.h
*
**/	
GHANDLE     GGetImageThumbnail(GHANDLE hImage, GUINT32 u4Width, GUINT32 u4Height);
/**
* @brief  get image info extension
*
* @param[in] hImage image handle
* @param[in] prImageInfo GIMAGEINFO_T * image info
*
* @return GBOOL define in GDef.h
*
* @see GIMAGEINFO_T
*
**/	
GBOOL       GGetImageInfoEx(GHANDLE hImage, GIMAGEINFO_T *prImageInfo); 

/**
* @brief  set bitmap image sink
*
* @param[in] hImage image handle
* @param[in] hBitmap bitmap handle
*
* @return GBOOL define in GDef.h
*
**/
GBOOL       GSetBitmapImageSink(GHANDLE hImage, GHANDLE hBitmap);
/**
* @brief  get bitmap image sink
*
* @param[in] hImage image handle
*
* @return GHANDLE define in GDef.h
*
**/
GHANDLE     GGetBitmapImageSink(GHANDLE hImage);
#if DRAW_IMAGE_VIDEO_OVERLAY
GBOOL       GIsJPG(GHANDLE hImage);
/**
* @brief  decode image
*
* @param[in] hImage image handle
* @param[in] i4DstW width
* @param[in] i4DstH height
* @param[in] pfnNotify decode notify function
* @param[in] u4CustomData custom data
* @param[in] pixelFormat pixel format
*
* @return GBOOL define in GDef.h
*
* @see PFN_IMAGEDECODE_NOTIFY_EX
*
**/
GBOOL       GDecodeImage(GHANDLE hImage, GINT32 i4DstW, GINT32 i4DstH, 
						 PFN_IMAGEDECODE_NOTIFY_EX pfnNotify, GUINT32 u4CustomData, UINT32 pixelFormat);
#else
/**
* @brief  decode image
*
* @param[in] hImage image handle
* @param[in] i4DstW width
* @param[in] i4DstH height
* @param[in] pfnNotify decode notify function
* @param[in] u4CustomData custom data
*
* @return GBOOL define in GDef.h
*
* @see PFN_IMAGEDECODE_NOTIFY_EX
*
**/
GBOOL       GDecodeImage(GHANDLE hImage, GINT32 i4DstW, GINT32 i4DstH, 
                           PFN_IMAGEDECODE_NOTIFY_EX pfnNotify, GUINT32 u4CustomData, UINT32 pixelFormat);
#endif
/**
* @brief  decode image thumbnail
*
* @param[in] hImage image handle
* @param[in] i4DstW width
* @param[in] i4DstH height
* @param[in] pfnNotify decode notify function
* @param[in] u4CustomData custom data
*
* @return GBOOL define in GDef.h
*
* @see PFN_IMAGEDECODE_NOTIFY_EX
*
**/
GBOOL       GDecodeImageThumbnail(GHANDLE hImage, GINT32 i4DstW, GINT32 i4DstH,
                                  PFN_IMAGEDECODE_NOTIFY_EX pfnNotify, GUINT32 u4CustomData);
GBOOL       GIsImageDecoded(GHANDLE hImage);
/**
* @brief  terminate image decoding
*
* @param[in] hImage image handle
*
* @return GBOOL define in GDef.h
*
**/
GBOOL       GTerminateImageDecoding(GHANDLE hImage);
/**
* @brief  create bitmap from image
*
* @param[in] hImage image handle
*
* @return GHANDLE define in GDef.h
*
**/
GHANDLE     GCreateBitmapFromImage(GHANDLE hImage);
/**
* @brief  clone bitmap
*
* @param[in] hBitmap bitmap handle
*
* @return GHANDLE define in GDef.h
*
**/
GHANDLE     GCloneBitmap(GHANDLE hBitmap);
/**
* @brief  rotate bitmap
*
* @param[in] hBitmap bitmap handle
* @param[in] i2Angle the angle of rotate
*
* @return GHANDLE define in GDef.h
*
**/
GHANDLE     GRotateBitmap(GHANDLE hBitmap, GINT16 i2Angle);
/**
* @brief  get bitmap info
*
* @param[in] hBitmap bitmap handle
* @param[out] prImageInfo GIMAGEINFO_T * image info
*
* @return GBOOL define in GDef.h
*
* @see GIMAGEINFO_T
*
**/

GBOOL GGetBitmapData(GHANDLE hImage, GCHAR *puPicData);

GBOOL       GGetBitmapInfo(GHANDLE hBitmap, GIMAGEINFO_T *prImageInfo); 
GBOOL GetGifFrameSize(GHANDLE hImage,UINT* size);
GBOOL GetGifPropertyItem(GHANDLE hImage,UINT* size,PropertyItem* pi,UINT* frm_cnt);
GBOOL SelectCurFrame(GHANDLE hImage,UINT count);
/**
* @brief  draw image
*
* @param[in] hGC image info
* @param[in] hImage image handle
* @param[in] prRect GRECT_T * image location
*
* @see GRECT_T
*
**/
GVOID GDrawImageDC(HDC hDC, GHANDLE hImage, GRECT_T *prRect, UINT32 pixelformat);

GVOID GReleaseBitmapInfo(BITMAPINFO *bmiptr);

#if PIC_OP_ZOOM1
GVOID GSetZoomLevel(GUINT uLevel);

HRESULT GZoom(GHANDLE hImage, GUINT uLevel);

HRESULT GDrawBitmapZoom(HGC hGC, GHANDLE hBitmap, GRECT_T *prDestRect, GRECT_T *prSrcRect);

HRESULT GDrawBitmap(HGC hGC, GHANDLE hBitmap, GRECT_T *prDestRect, GRECT_T *prSrcRect);

HRESULT GDrawImageEx(HGC hGC, GHANDLE hImage, GRECT_T *prDestRect, GRECT_T *prSrcRect, UINT32 pixelformat);

#else 
#if PIC_OP_ZOOM2
Image_zoom_result GGetImageZoomResult(HANDLE hImage);
HRESULT GZoom(GHANDLE hImage, GFLOAT fRoomRatio, POINT *prMiddlePoint);
HRESULT GMove(HANDLE hImage, POINT rMove);
GVOID GDrawBitmap(HGC hGC, GHANDLE hImage, GRECT_T *prDestRect, SIZE *prScreenSize);
#else
GVOID GDrawBitmap(HGC hGC, GHANDLE hBitmap, GRECT_T *prRect);
#endif
GVOID       GDrawImageEx(HGC hGC, GHANDLE hImage, GRECT_T *prRect, UINT32 pixelformat);
#endif

GRESULT GShowImage(GHANDLE hImage);
GRESULT GHideImage(GHANDLE hImage);

/**
* @brief  destroy image extension
*
* @param[in] hImage image handle
*
**/
GVOID       GDestroyImageEx(GHANDLE hImage);
/**
* @brief  destroy bitmap
*
* @param[in] hBitmap bitmap handle
*
**/
GVOID       GDestroyBitmap(GHANDLE hBitmap);
/**
* @brief  register imaging notify window
*
* @param[in] hWnd window handle
* @param[in] u4Msg the message
*
**/
GVOID       GRegisterImagingNotifyWnd(HWND hWnd, GUINT32 u4Msg);
GVOID       GUnRegisterImagingNotifyWnd(HWND hWnd);


#ifdef __cplusplus
}
#endif

#endif //_GIMAGING_H_
