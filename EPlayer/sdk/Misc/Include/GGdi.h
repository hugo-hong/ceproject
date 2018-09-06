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

/*****************************************************************************
*
* Filename:
* ---------
*   $Workfile:  $ 
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
*   mcn05013
*
* Last changed:
* ------------- 
* $Author: zeng.zhang $ 
*
* $Modtime: $  
*
* $Revision: #1 $
****************************************************************************/

#ifndef _GGDI_H_
#define _GGDI_H_

#include "GDef.h"
#include "GRect.h"

#ifdef __cplusplus
extern "C" {
#endif


/** 
* @defgroup Graphics
* @brief Provides simple 2D geometric rendering capability.
*
* <p><b>Graphics</b> supports source color keying and source constant 
* alpha blending for blits and overlay surfaces, 
* palettes and overlay surface.</p>
*
* <p>A <b>surface</b> represents a linear area of 
* display memory. A <b>surface</b> usually resides in the display memory of 
* the display card, although surfaces can exist in system memory. A Layer can
* contain one primary surface with or without back buffer.</p>
*
* <p>The surface contains several attributes.</p>
* <ul>
*    <li>Color keying</li>
*
*    <li>Alpha blending</li>
*   
*    <li>Palettes</li>
* </ul>
*
* <p>The collection of state attributes associated with <b>GC</b> is referred 
* to as the graphics rendering context. To display text, shapes, or images, 
* you set up the graphics rendering context and then call one of the GC 
* rendering methods, such as draw or fill.</p>
*
* <p>The <b>GC</b> contains several attributes.</p>
* <ul>
*    <li>The <b>pen</b> style that is applied to the outline of a shape. This attribute 
*        enables you to draw lines with any point size and dashing pattern.</li>
*
*    <li>The <b>brush</b> style that is applied to a shape's interior. This 
*        attribute enables you to fill shapes with solid colors, gradients, 
*        and patterns.</li>
*
*    <li>The <b>clip</b>, which restricts rendering to the area within the outline of 
*        the Shape used to define the clipping path.</li>
*
*    <li>The <b>font</b> used to convert text strings to glyphs.</li>
*
*    <li>The <b>text color</b>.</li>
* </ul>
* <p>To set an attribute in the Graphics rendering context, 
*    you use the set Attribute methods: </p>
* <ul>
*    <li><code>GSetPen</code></li>
*    <li><code>GSetBrush</code></li>
*    <li><code>GSetClip</code></li>
*    <li><code>GSetFont</code></li>
*    <li><code>GSetTextColor</code></li>
* </ul>
*
* <p>The following code illustrates common usage.</P>
* <p>
* <TABLE BORDER="0">
*  <TR>
*      <TD ROWSPAN="1" COLSPAN="1">
*          <pre><code>
*   
*              rRect.i2Left = 0;
*              rRect.i2Top = 0;
*              rRect.i2Right = 320;
*              rRect.i2Bottom = 240;
*
*              rSurfaceDesc.prRect = &amp;rRect;
*              rSurfaceDesc.u4Caps = SURFACE_PRIMARY;
*              rSurfaceDesc.ucLayerID = 0;
*              GDI_HANDLE = GCreateSurface( &rSurfaceDesc );
*              
*              rRectGC.i2Left = 20;
*              rRectGC.i2Top = 20;
*              rRectGC.i2Right = 220;
*              rRectGC.i2Bottom = 140;
*
*              hGC = GCreateGC( GDI_HANDLE, &rRectGC );
*
*              GCreateSolidPen(&rPen, RGB(255,0,0), 1);
*              GCreateSolidBrush(&rBrush, COLORINDEX(200));
*       
*              GSetPen(hGC, &rPen);
*              GSetBrush(hGC, &rBrush);
*
*              GRoundRect( hGC, 0, 0, 100, 120, 20, 20 );
*              GDrawImg(hGC, 20, 70, IDB_PROGRESS, RGB(255, 0, 255));
*            
*              GDeleteGC( hGC );
*              GDeleteSurface( GDI_HANDLE );
*
*          </code> </pre>
*      </TD>
*  </TR>
* </TABLE>
* </p>
* @ingroup PAL
*/

/*@{*/

/*@{*/

#if DDRAW_PANEL_MOVE
typedef GVOID (*PFN_GDIRENDERCB)(HWND m_hWnd);
#endif 

#define MAX_FONTNAME_LEN            100

#define DDOVER_SHOWATREAR           0x00200000

#define DISPLAY_SET_REAR_RECT                    (0x0002000c)

#define DISPLAY_SET_PRIMARY_SURFACE_SRC_CLRKEY      (0x00020012)

#define DISPLAY_SET_PRIMARY_SURFACE_ALPHA_BLENDING   (0x20013)

/**
* @name Bitblt source over rule
*/
#define ROPCODE_SRCCOPY             0   /**< source copy. */
/*@}*/

/*@{*/
/**
* @name Font type
*/
enum
{
    FONT_SMALL,     /**< small font */
    FONT_MEDIUM,    /**< medium font */
    FONT_LARGE,     /**< large font */
    FONT_MAP_S,     /**< map font */
    FONT_MAP_M,     /**< map font */
    FONT_MAP_L,     /**< map font */
    FONT_EXT_1,     /**< extension font */
    FONT_EXT_2,     /**< extension font */
    FONT_EXT_3,     /**< extension font */
    FONT_EXT_4,     /**< extension font */
    FONT_EXT_5, 
    FONT_EXT_6,
    MAX_FONTNUM
};
/*@}*/

/*@{*/
/**
* @name Pen style
*/
enum
{
    PEN_NULL,           /**< Null pen */
    PEN_SOLID,          /**< Solid color */
    PEN_DASH,
    PEN_STIPPLED,       /**< Stippled line */
    PEN_PATTERN
};
/*@}*/

/*@{*/
/**
* @name Brush style
*/
enum
{
    BRUSH_NULL,         /**< Null brush */
    BRUSH_SOLID,        /**< Solid color */
    BRUSH_PATTERN,      /**< Bitmap pattern */
    BRUSH_PATTERNEX,
    BRUSH_HATCH,        /**< Hatch fill */
    BRUSH_GRADIENT      /**< Gradient fill */
};
/*@}*/

/*@{*/
/**
* @name Gradient fill type
*/
enum
{
    GRADIENT_HORIZONTAL,    /**< horizontal gradient */
    GRADIENT_VERTICAL,      /**< vertical gradient */
    GRADIENT_DIAGONAL,      /**< diagonal gradient */
    GRADIENT_DIAGINVERT,     /**< diagonal invert gradient */
    GRADIENT_FLIP           /**< flip gradient */
};
/*@}*/

/*@{*/
/**
* @name Hatch fill type.
*/
enum
{
    HATCH_HORIZONTAL,       /**< horizontal hatch */
    HATCH_VERTICAL,         /**< vertical hatch */
    HATCH_CROSS,            /**< cross hatch */
    HATCH_DIAGCROSS         /**< diagonal cross hatch */
};
/*@}*/


typedef enum
{
    IFMODE_IMAGE0,
    IFMODE_IMAGE1,
    IFMODE_IMAGE3,
    IFMODE_IMAGE9
} GIMAGEFILLMODE_T;

/*@{*/
/**
* @name Brush style
*/
enum
{
    SOURCE_NULL,
    FRONT_DVD,      
    FRONT_USB,         
    FRONT_SD,        
    REAR_DVD,
    REAR_USB,
    REAR_SD
};
/*@}*/


typedef GVOID *          HGC;

#if defined(WIN32) || defined(_WIN32_WCE)
typedef HDC             HSURFACE;
#endif


/**
* @brief Pen structure.
*/
typedef struct
{
    GUINT32  u4Style;  /**< pen sytle */
    union
    {
        struct 
        {
            GUINT32  u4Color;        /**< pen color */
            GUINT32  u4Width;        /**< pen width */
            GUINT32  u4Dummy[2];
        } rSolid;             
        struct
        {
            GUINT32  u4Color;        /**< pen color */
            GUINT32  u4Width;        /**< pen width */
            GUINT32  u4DashSpaceLens[2];
        } rDash;
        struct
        {
            GUINT32  u4ImgID;
        } rPattern;
        struct
        {
            GHANDLE  hImage;        /**< image id */
        } rPatternEx;
        struct 
        {
            GUINT32  u4Color;        /**< pen color */
            GUINT32  u4Width;
            GUINT32  u4Cycle;        /**< cycle */
            GUINT32 *pu4DashSpaceLens; /**< pattern */
        } rStippled;
    } uContent;
} GPEN_T;


/**
* @brief Brush structure.
*/
typedef struct
{
    GUINT32  u4Style;               /**< brush type */
    union 
    {
        struct
        {
            GUINT32      u4Color;
            GUINT32      u4Dummy[2];
        } rSolid;
        struct
        {
            GUINT32      u4Type;  /**< gradient type */
            GUINT32      u4StartColor; /**< start color */
            GUINT32      u4EndColor;   /**< end color */
        } rGradient;
        struct
        {
            GUINT32      u4Type;  /**< hatch type */
            GUINT32      u4BgColor;    /**< background color */
            GUINT32      u4HatchColor; /**< foreground color */
        } rHatch;
        struct
        {
            GUINT32      u4ImgID;
            GUINT32      u4ImgCount;
        } rPattern;
        struct  
        {
            GHANDLE      hImage;  /**< image id */
        } rPatternEx;  
    } uContent;
} GBRUSH_T;


typedef enum 
{
    NORMAL_FONT,
    BORDER_FONT,
    SHADOW_FONT
} GFONTEFFECT_T;


typedef enum
{
    DITYPE_NORMAL,
    DITYPE_ALPHABLTIMAGE
} GDRAWIMGTYPE_T;


typedef enum
{
    DIAM_NOALPHA,
    DIAM_TRANSIMGALPHA,
    DIAM_TRANSBLTIMGALPHA,
    DIAM_BLTIMGALPHA,
    DIAM_ALPHABLTIMGALPHA
} GDIAMETHOD_T;


typedef struct
{
    GUINT32  u4Font;
    GTCHAR   szFontName[MAX_FONTNAME_LEN];
    GINT32   i4Height;
    GINT32   i4Width;
    GINT32   i4Weight;
    GFONTEFFECT_T eEffect;
    GUINT32 u4EffectColor;
} GGDIFONTENTRY_T;


typedef struct
{
    GGDIFONTENTRY_T arFontEntries[MAX_FONTNUM];
    GUINT32      u4FontNum;
} GGDIARG_T;

typedef struct
{
   long nCodePage;
   UINT16 uStartCode;
   UINT16 uEndCode;
} GGDILANGUAGE;

#if DDRAW_PANEL_MOVE
GAPI BOOL DeInitDDraw(HWND hWnd, BOOL bFullScreen);
GAPI BOOL InitDDraw(HWND hWnd, BOOL bFullScreen);                    // for ddraw pwz
GAPI BOOL InitDDrawSurface(HWND hWnd, BOOL bFullScreen);

GAPI BOOL DeInitDDrawPrimary(HWND hWnd, BOOL bFullScreen);
GAPI BOOL InitDDrawPrimary(HWND hWnd, BOOL bFullScreen);

/*
GAPI BOOL GetDDrawOverlay(HWND hWnd, BOOL bFront);
GAPI BOOL ReleaseDDrawOverlay(HWND hWnd,BOOL bFront);
*/

GAPI BOOL GetDDrawOverlay(HWND hWnd, BOOL bFront,RECT rSrc,RECT rDest, BOOL bFullScreen);
GAPI BOOL ReleaseDDrawOverlay(HWND hWnd,BOOL bFront);

GAPI BOOL SaveBakeSurfaceToSceenReal(void);

GAPI BOOL InitDDrawOverlay(HWND hWnd);
GAPI BOOL DeInitDDrawOverlay(HWND hWnd, BOOL bFullScreen);

GAPI BOOL InitAddReplaceCode(UINT16 uCode);
GAPI BOOL InitAddSupportLanguage(long nCodePage,UINT16 uStartCode, UINT16 uEndCode);
GAPI BOOL CheckStringForLanguage(GCHAR *szString);
 
GAPI BOOL ShowEffect(HWND m_hWnd,GEFFECTARRAY Effect[],UINT32 uIndex,BOOL bWithBkgrd,BOOL bHaveCallBack);

GAPI BOOL MoveSurface(HDC hdcEffect, BOOL bWithBkgrd,int startx, int starty,int i_HdcWith, int i_Height, int iShowLeft, int iShowTop);
GAPI GVOID  GRegMoveRenderFunc(PFN_GDIRENDERCB pfnEffectCb); // ddraw


GAPI BOOL MoveEffect(HWND m_hWnd,GEFFECTMOVEARRAY Effect[],UINT32 uIndex, BOOL bWithBkgrd,BOOL bHaveCallBack);
GAPI BOOL GetShowEffectState();

GAPI GVOID       GBltLayers2ScreenDdrawReal(GRECT_T *prRect);    // for ddraw   pwz   20100823

GAPI GVOID       GBltLayers2ScreenDdrawOverlayReal(BOOL bFront,GRECT_T *prRect);    // for ddraw   pwz   20100823

GAPI HDC         GGetOffScreenDc(GVOID);                      // ddraw
GAPI HDC         GSetOffScreenDc(HDC m_Hdc);                  // ddraw
GAPI GVOID       GRegGDIRenderFunc(PFN_GDIRENDERCB pfnEffectCb); // ddraw

BOOL UpdateDDrawOverlayFlag(BOOL bShowOverlay);

#endif 


/**
* @brief  Initial gdi
*
* @param[in] prArg GGDIARG_T * diarg
* 
* @return GBOOL define in GDef.h
*  
* @see GGDIARG_T
* 
**/
GAPI GBOOL       GInitGdi(GGDIARG_T *prArg);
/**
* @brief  deInitial gdi
*
**/
GAPI GVOID       GDeinitGdi(GVOID);
/**
* @brief  change gdi
*
* @param[in] prArg GGDIARG_T * diarg
* 
* @see GGDIARG_T
* 
**/
GAPI GVOID       GChngGdi(GGDIARG_T *prArg);

/**
* @brief  GDI Lock Saffin
**/
GAPI GVOID       GLockFrameBuffer();
/**
* @brief  GDI unLock Saffin
**/
GAPI GVOID       GUnLockFrameBuffer();

/**
* @brief  get base surface
**/
GAPI HSURFACE   GGetBaseSurface();
/**
* @brief  get gdi surface surface
*
* @param[in] u2Width width
* @param[in] u2Height height
*
* @return HSURFACE define in GDef.h
*
**/
GAPI HSURFACE   GGetGDISurface(GUINT16 u2Width, GUINT16 u2Height);
/**
* @brief  Need to Release GDI Surface once MiGO is in BG Mode
*
**/
GAPI GVOID      GReleaseGDISurface();
/**
* @brief  create surface 
*
* @param[in] u2Width width
* @param[in] u2Height height
*
* @return HSURFACE define in GDef.h
*
**/
GAPI HSURFACE   GCreateSurface(GUINT16 u2Width, GUINT16 u2Height);
/**
* @brief  create surface 
*
* @param[in] u2Width width
* @param[in] u2Height height
*
* @return HSURFACE define in GDef.h
*
**/
GAPI HSURFACE   GCreateSurfaceEx(GUINT16 u2Width, GUINT16 u2Height);
/**
* @brief  delete surface
*
* @param[in] hSurface surface handle
*
* @return HSURFACE define in GDef.h
*
**/
GAPI GVOID      GDeleteSurface(HSURFACE hSurface);
/**
* @brief  delete share surface
*
* @param[in] hSurface surface handle
*
* @return HSURFACE define in GDef.h
*
**/
GAPI HSURFACE   GDeleteShareSurface(HSURFACE hSurface);

/* following 2 functions just take effect for WCP Feature Phone */
/**
* @brief  set surface size
*
* @param[in] hSurface surface handle
* @param[in] u2X x-coordinate
* @param[in] u2Y y-coordinate
*
**/
GAPI GVOID       GSetSurfacePos(HSURFACE hSurface, GUINT16 u2X, GUINT16 u2Y);
/**
* @brief  get surface size
*
* @param[in] hSurface surface handle
* @param[out] pu2X x-coordinate
* @param[out] pu2Y y-coordinate
*
**/
GAPI GVOID       GGetSurfacePos(HSURFACE hSurface, GUINT16 *pu2X, GUINT16 *pu2Y);
/**
* @brief  get surface size
*
* @param[in] hSurface surface handle
* @param[out] prSize GSIZE_T * buffer size
*
**/
GAPI GVOID       GGetSurfaceSize(HSURFACE hSurface, GSIZE_T *prSize);

GAPI GVOID      *GGetSurfaceBufferPtr(HSURFACE hSurface, GBOOL *pfgXInvert, GBOOL *pfgYInvert);

//Graphics Context
GAPI HGC        GCreateGC(HSURFACE hSurface, GRECT_T *prBoundRect);
GAPI GVOID       GDeleteGC(HGC hGC);

/**
* @brief  set surface
*
* @param[in] hGC graphics context handle
* @param[out] prSize GSIZE_T * buffer size
*
**/
GAPI GVOID       GGetGCBufferSize(HGC hGC, GSIZE_T *prSize);
GAPI GVOID      *GGetGCBufferPtr(HGC hGC, GBOOL *pfgXInvert, GBOOL *pfgYInvert);

/**
* @brief  set surface
*
* @param[in] hGC graphics context handle
* @param[in] hSurface surface info
*
**/
GAPI GVOID       GSetSurface(HGC hGC, HSURFACE hSurface);
/**
* @brief  get bound rect
*
* @param[in] hGC graphics context handle
*
* @return HSURFACE define in GDef.h
**/
GAPI HSURFACE   GGetSurface(HGC hGC);

/**
* @brief  set bound rect
*
* @param[in] hGC graphics context handle
* @param[in] prBoundRect GRECT_T * bound rect
*
* @see GRECT_T
*
**/
GAPI GVOID       GSetBoundRect(HGC hGC, GRECT_T *prBoundRect);
/**
* @brief  get bound rect
*
* @param[in] hGC graphics context handle
* @param[out] prBoundRect GRECT_T * bound rect
*
* @see GRECT_T
**/
GAPI GVOID       GGetBoundRect(HGC hGC, GRECT_T *prBoundRect);

/**
* @brief  set clip info
*
* @param[in] hGC graphics context handle
* @param[in] prRect GRECT_T * clip info
*
* @see GRECT_T
*
**/
GAPI GVOID       GSetClip(HGC hGC, GRECT_T *prRect);
/**
* @brief  get clip info
*
* @param[in] hGC graphics context handle
* @param[out] prRect GRECT_T * clip info
*
*
**/
GAPI GVOID       GGetClip(HGC hGC, GRECT_T *prRect);

/**
* @brief  set pen info
*
* @param[in] hGC graphics context handle
* @param[in] pPen GPEN_T * pen info
*
* @see GPEN_T
*
**/
GAPI GVOID       GSetPen(HGC hGC, GPEN_T *pPen);
/**
* @brief  get pen info
*
* @param[in] hGC graphics context handle
*
* @return GPEN_T define in GDef.h
*
**/
GAPI GPEN_T    *GGetPen(HGC hGC);

/**
* @brief  set brush
*
* @param[in] hGC graphics context handle
* @param[in] pBrush GBRUSH_T * brush info
*
* @see GBRUSH_T
*
**/
GAPI GVOID       GSetBrush(HGC hGC, GBRUSH_T *pBrush);
/**
* @brief  get Brush info
*
* @param[in] hGC graphics context handle
*
* @return GBRUSH_T define in GDef.h
*
**/
GAPI GBRUSH_T  *GGetBrush(HGC hGC);

/**
* @brief  set text color
*
* @param[in] hGC graphics context handle
* @param[in] u4TextColor  color value
*
**/
GAPI GVOID       GSetTextColor(HGC hGC, GUINT32 u4TextColor);
/**
* @brief  get text color
*
* @param[in] hGC graphics context handle
*
* @return GUINT32 define in GDef.h
*
**/
GAPI GUINT32     GGetTextColor(HGC hGC);

/**
* @brief  set font
*
* @param[in] hGC graphics context handle
* @param[in] u4Font font value
*
**/
GAPI GVOID       GSetFont(GHANDLE hGC, GUINT32 u4Font);
GAPI GVOID       GSetFontEx(GHANDLE hGC, GUINT32 u4Font, GINT32 i4Escapement);
/**
* @brief  get font
*
* @param[in] hGC graphics context handle
*
* @return GUINT32 define in GDef.h
*
**/
GAPI GUINT32     GGetFont(GHANDLE hGC);

//Pen
/**
* @brief  create solid pen
*
* @param[in] prPen pen info
* @param[in] u4Color pen color
* @param[in] u4Width pen width
*
* @see GPEN_T
*
**/
GAPI GVOID       GCreateSolidPen(GPEN_T *prPen, GUINT32 u4Color, GUINT32 u4Width);
/**
* @brief  create dash pen
*
* @param[in] prPen pen info
* @param[in] u4Color pen color
* @param[in] u4Width pen width
* @param[in] u4DashLen dash length
* @param[in] u4SpaceLen space length
*
* @see GPEN_T
*
**/
GAPI GVOID       GCreateDashPen(GPEN_T *prPen, GUINT32 u4Color, GUINT32 u4Width,
                               GUINT32 u4DashLen, GUINT32 u4SpaceLen);
/**
* @brief  create pattern pen
*
* @param[in] prPen pen info
* @param[in] u4ImgID image id
*
* @see GPEN_T
*
**/
GAPI GVOID       GCreatePatternPen(GPEN_T *prPen, GUINT32 u4ImgID);
/**
* @brief  create pattern pen
*
* @param[in] prPen pen info
* @param[in] hImage image handle
*
* @see GPEN_T
*
**/
GAPI GVOID       GCreatePatternPenEx(GPEN_T *prPen, GHANDLE hImage);
/**
* @brief  create stippled pen
*
* @param[in] prPen pen info
* @param[in] u4Color pen color
* @param[in] u4Cycle pen cycle
* @param[in] pu4DashSpaceLens dash space length
*
* @see GPEN_T
*
**/
GAPI GVOID       GCreateStippledPen(GPEN_T *prPen, GUINT32 u4Color, GUINT32 u4Cycle,
                                   GUINT32 *pu4DashSpaceLens);

//Brush
/**
* @brief  create solid brush
*
* @param[in] prBrush brush info
* @param[in] u4Color color value
*
* @see GBRUSH_T
*
**/
GAPI GVOID       GCreateSolidBrush(GBRUSH_T *prBrush, GUINT32 u4Color);
/**
* @brief  create pattern brush
*
* @param[in] prBrush brush info
* @param[in] u4ImgID image id
*
* @see GBRUSH_T
*
**/
GAPI GVOID       GCreatePatternBrush(GBRUSH_T *prBrush, GUINT32 u4ImgID);
GAPI GVOID       GCreatePatternBrushEx(GBRUSH_T *prBrush, GHANDLE hImage);
/**
* @brief  create gradient brush
*
* @param[in] prBrush brush info
* @param[in] u4StartColor start color value
* @param[in] u4EndColor end color value
* @param[in] u4Type type value
*
* @see GBRUSH_T
*
**/
GAPI GVOID       GCreateGradientBrush(GBRUSH_T *prBrush, GUINT32 u4StartColor,
                                     GUINT32 u4EndColor, GUINT32 u4Type);
/**
* @brief  create hatch brush
*
* @param[in] prBrush brush info
* @param[in] u4BgColor background color
* @param[in] u4HatchColor hatch color
* @param[in] u4Type type value
*
* @see GBRUSH_T
*
**/
GAPI GVOID       GCreateHatchBrush(GBRUSH_T *prBrush, GUINT32 u4BgColor,
                                  GUINT32 u4HatchColor, GUINT32 u4Type);

//Font
/**
* @brief  get font size
*
* @param[in] uFont font type
* @param[in] prSize GSIZE_T * font size
*
* @see GSIZE_T
*
**/
GAPI GVOID       GGetFontSize(GUINT8 uFont, GSIZE_T *prSize);

//Primitive drawing
/**
* @brief  draw pixel
*
* @param[in] hGC graphic context handle
* @param[in] i4X x-coordinate
* @param[in] i4Y y-coordinate
* @param[in] u4Color pixel color
*
*
**/
GAPI GVOID       GDrawPixel(GHANDLE hGC, GINT32 i4X, GINT32 i4Y, GUINT32 u4Color);
/**
* @brief  draw line
*
* @param[in] hGC graphic context handle
* @param[in] i4X1 start x-coordinate
* @param[in] i4Y1 start y-coordinate
* @param[in] i4X2 end x-coordinate
* @param[in] i4Y2 end y-coordinate
*
**/
GAPI GVOID       GDrawLine(HGC hGC, GINT32 i4X1, GINT32 i4Y1, GINT32 i4X2, GINT32 i4Y2);
/**
* @brief  draw  rectangle
*
* @param[in] hGC graphic context handle
* @param[in] i4Left left-top x-coordinate
* @param[in] i4Top left-top y-coordinate
* @param[in] i4Right right-bottom x-coordinate
* @param[in] i4Bottom right-bottom y-coordinate
*
**/
GAPI GVOID       GRectangle(HGC hGC, GINT32 i4Left, GINT32 i4Top,
                           GINT32 i4Right, GINT32 i4Bottom);
/**
* @brief  draw  round rectangle
*
* @param[in] hGC graphic context handle
* @param[in] prRect rect info
* @param[in] u4RoundW round width
* @param[in] u4RoundH round height
*
* @see GRECT_T
*
**/
GAPI GVOID       GRoundRect(HGC hGC, GRECT_T *prRect, GUINT32 u4RoundW, 
                           GUINT32 u4RoundH);
/**
* @brief  draw ellipse
*
* @param[in] hGC graphic context handle
* @param[in] i4Left left-top x-coordinate
* @param[in] i4Top left-top y-coordinate
* @param[in] i4Right right-bottom x-coordinate
* @param[in] i4Bottom right-bottom y-coordinatet
*
*
**/
GAPI GVOID       GEllipse(HGC hGC, GINT32 i4Left, GINT32 i4Top, GINT32 i4Right, 
                         GINT32 i4Bottom);
/**
* @brief  draw polygon
*
* @param[in] hGC graphic context handle
* @param[in] prPoints points info
* @param[in] u2Count count number
*
* @see GPOINT_T
**/
GAPI GVOID       GPolygon(HGC hGC, GPOINT_T *prPoints, GUINT16 u2Count);
/**
* @brief  draw polyline
*
* @param[in] hGC graphic context handle
* @param[in] prPoints points info
* @param[in] u2Count count number
*
* @see GPOINT_T
**/
GAPI GVOID       GPolyline(HGC hGC, GPOINT_T *prPoints, GUINT16 u2Count);

//Image
/**
* @brief  draw image
*
* @param[in] hGC graphic context handle
* @param[in] x x-coordinate
* @param[in] y y-coordinate
* @param[in] hImage image hande
*
**/
GAPI GVOID       GDrawImg(GHANDLE hGC, GINT32 x, GINT32 y, GHANDLE hImage);
/**
* @brief  draw image with trans color
*
* @param[in] hGC graphic context handle
* @param[in] x x-coordinate
* @param[in] y y-coordinate
* @param[in] hImage image hande
* @param[in] u4TransColor trans color
*
**/
GAPI GVOID       GDrawImgWithTransColor(HGC hGC, GINT32 x, GINT32 y, GHANDLE hImage,
                                       GUINT32 u4TransColor);
GAPI GVOID       GDrawImgEx(GHANDLE hGC, GINT32 x, GINT32 y, GUINT32 u4ImgID);
GAPI GVOID       GDrawImgWithTransColorEx(HGC hGC, GINT32 x, GINT32 y, GUINT32 u4ImgID,
                                         GUINT32 u4TransColor);

GAPI GVOID       GDrawPartImage(GHANDLE hGC, GINT32 x, GINT32 y, GHANDLE hImage, 
                               GRECT_T *prPartRect);
GAPI GVOID       GDrawPartImageWithTransColor(GHANDLE hGC, GINT32 x, GINT32 y, 
                                             GHANDLE hImage, GRECT_T *prPartRect,
                                             GUINT32 u4TransColor);

/**
* @brief  fill rect with image
*
* @param[in] hGC graphic context handle
* @param[in] prRect rect info
* @param[in] hImage image handle
* @param[in] prImageRect image rect info
* @param[in] eImageFillMode image fill mode
*
* @see GRECT_T
* @see GIMAGEFILLMODE_T
*
**/
GAPI GVOID       GFillRectWithImage(GHANDLE hGC, GRECT_T *prRect, GHANDLE hImage,
                                   GRECT_T *prImageRect,
                                   GIMAGEFILLMODE_T eImageFillMode);
/**
* @brief  fill rect with trans image
*
* @param[in] hGC graphic context handle
* @param[in] prRect rect info
* @param[in] hImage image handle
* @param[in] prImageRect image rect info
* @param[in] u4TransColor trans color
* @param[in] eImageFillMode image fill mode
*
* @see GRECT_T
* @see GIMAGEFILLMODE_T
*
**/
GAPI GVOID       GFillRectWithTransImage(GHANDLE hGC, GRECT_T *prRect, GHANDLE hImage,
                                        GRECT_T *prImageRect, GUINT32 u4TransColor,
                                        GIMAGEFILLMODE_T eImageFillMode);

GAPI GBOOL       GDrawImgStretchTransColor(GHANDLE hGC, GINT32 x, GINT32 y, 
                                GHANDLE hImage, GUINT32 crTransColor,
                                GINT32 i4Ratio);

GAPI GBOOL       GDrawImgStretchTransColorEx(GHANDLE hGC, GINT32 x, GINT32 y, 
                                 GHANDLE hImage, GUINT32 u4TransColor,
                                 GINT32        i4Width,
                                 GINT32        i4Height);                            

GAPI GVOID       GDrawImgEx2(GHANDLE hGC, GINT32 x, GINT32 y, GHANDLE hImage, 
                            GDRAWIMGTYPE_T eDIType, 
                            GDIAMETHOD_T eDIAMethod);
GAPI GVOID       GDrawImgWithTransColorEx2(HGC hGC, GINT32 x, GINT32 y, 
                                          GHANDLE hImage, GUINT32 u4TransColor,
                                          GDRAWIMGTYPE_T eDIType,
                                          GDIAMETHOD_T eDIAMethod);
GAPI GVOID       GFillRectWithImageEx(GHANDLE hGC, GRECT_T *prRect, GHANDLE hImage,
                                     GRECT_T *prImageRect,
                                     GIMAGEFILLMODE_T eImageFillMode,
                                     GDRAWIMGTYPE_T eDIType,
                                     GDIAMETHOD_T eDIAMethod);
GAPI GVOID       GFillRectWithTransImageEx(GHANDLE hGC, GRECT_T *prRect, GHANDLE hImage,
                                          GRECT_T *prImageRect, GUINT32 u4TransColor,
                                          GIMAGEFILLMODE_T eImageFillMode,
                                          GDRAWIMGTYPE_T eDIType,
                                          GDIAMETHOD_T eDIAMethod);

//add by mcn07012
GAPI GVOID       GDrawStretchImg(HGC hGC, GINT32 i4X, GINT32 i4Y, const GTCHAR *lpBmpName, 
                                GINT32 i4Width, GINT32 i4Height);
/**
* @brief  get image width
*
* @param[in] hImage image handle
*
* @return GINT16 define in GDef.h
*
**/
GAPI GINT16      GGetImgWidth(GHANDLE hImage);
/**
* @brief  get image height
*
* @param[in] hImage image handle
*
* @return GINT16 define in GDef.h
*
**/
GAPI GINT16      GGetImgHeight(GHANDLE hImage);

/**
* @brief  get image size
*
* @param[in] hImage image handle
* @param[out] prSize GSIZE_T * size info
*
* @return GBOOL define in GDef.h
*
**/
GAPI GBOOL       GGetImgSize(GHANDLE hImage, GSIZE_T *prSize);
GAPI GBOOL       GGetImgSizeEx(GUINT32 u4ImgID, GSIZE_T *prSize);

//add by mcn07047
/**
* @brief  get image effect color
*
* @param[in] hGC graphic context handle
*
* @return GUINT32 define in GDef.h
*
**/
GAPI GUINT32     GGetEffectColor(GHANDLE hGC);
//String
GAPI GVOID       GDrawStringA(HGC hGC, GINT32 i4X, GINT32 i4Y, GCHAR *szString);

//add by mcn07047
GAPI GVOID       GDrawStringNormalA(HGC hGC, GINT32 i4X, GINT32 i4Y, GCHAR *szString);

//add by mcn07047
GAPI GVOID       GDrawStringWithShadowA(HGC hGC, GINT32 i4X, GINT32 i4Y, 
                                       GCHAR *szString, GUINT32 U4ShadowColor);
GAPI GVOID       GDrawStringWithBorderA(HGC hGC, GINT32 i4X, GINT32 i4Y,
                                       GCHAR *szString, GUINT32 u4BdrColor);
GAPI GINT32      GGetStrWidthA(HGC hGC, GCHAR *szString);
GAPI GINT32      GGetStrHeightA(HGC hGC, GCHAR *szString);

GAPI GVOID       GDrawStringW(HGC hGC, GINT32 i4X, GINT32 i4Y, GWCHAR *szString);

//add by mcn07047
GAPI GVOID       GDrawStringNormalW(HGC hGC, GINT32 i4X, GINT32 i4Y, GWCHAR *szString);

//add by mcn07047
GAPI GVOID       GDrawStringWithShadowW(HGC hGC, GINT32 i4X, GINT32 i4Y, 
                                       GWCHAR *szString, GUINT32 U4ShadowColor);
GAPI GVOID       GDrawStringWithBorderW(HGC hGC, GINT32 i4X, GINT32 i4Y,
                                       GWCHAR *szString, GUINT32 u4BdrColor);
GAPI GINT32      GGetStrWidthW(HGC hGC, GWCHAR *szString);
GAPI GINT32      GGetStrHeightW(HGC hGC, GWCHAR *szString);

//mtk71372 2012.01.06 --------------------------------------------------------------- str

//----------------------------------------------------------------------------------------
//Description:
//    Set the format for draw text.
//
//Parameters:
//    uFormat : [in] The text format.The value is the same with uFormat of DrawText api
//-----------------------------------------------------------------------------------------
GVOID GSetDrawTextFormat(UINT uFormat);


//mtk71372 2012.01.06 --------------------------------------------------------------- end

#if defined(UNICODE)
#define GDrawString                     GDrawStringW
#define GDrawStringWithBorder           GDrawStringWithBorderW
#define GGetStrWidth                    GGetStrWidthW
#define GGetStrHeight                   GGetStrHeightW
#else
#define GDrawString                     GDrawStringA
#define GDrawStringWithBorder           GDrawStringWithBorderA
#define GGetStrWidth                    GGetStrWidthA
#define GGetStrHeight                   GGetStrHeightA
#endif


// Blt between Surfaces
GAPI GVOID       GBitBlt(HSURFACE hDstSurface, GINT32 i4DstX, GINT32 i4DstY, 
                        GUINT32 u4Width, GUINT32 u4Height,
                        HSURFACE hSrcSurface, GINT32 i4SrcX, GINT32 i4YSrcY, 
                        GUINT32 u4RopCode);

GAPI GVOID       GStretchBlt(HSURFACE hDstSurface, GINT32 i4DstX, GINT32 i4DstY,
                            GUINT32 u4DstW, GUINT32 u4DstH, 
                            HSURFACE hSrcSurface, GINT32 i4SrcX, GINT32 i4SrcY, 
                            GUINT32 u4SrcW, GUINT32 u4SrcH, GUINT32 u4RopCode); 

GAPI GVOID       GTransparentBlt(HSURFACE hDstSurf, GINT32 i4DstX, GINT32 i4DstY,
                                GUINT32 u4DstW, GUINT32 u4DstH,
                                HSURFACE hSrcSurf, GINT32 i4SrcX, GINT32 i4SrcY,
                                GUINT32 u4TransColor);

GAPI GVOID       GAlphaBlt(HSURFACE hDstSurf, GINT32 i4DstX, GINT32 i4DstY, 
                          GUINT32 u4Width, GUINT32 u4Height, 
                          HSURFACE hSrcSurf, GINT32 i4SrcX, GINT32 i4SrcY, 
                          GUINT8 uAlpha);


/*@}*/

GVOID  GGradientFillFont(HDC hDC,PTRIVERTEX pVertex, GINT32 nVertex,PVOID pMesh, GINT32 nCount, GINT32 ulMode);
GVOID  GSaveSurface(GHANDLE hGC, GRECT_T *prRect, HSURFACE hDesSurface);
GVOID  GRecoverSurface(GHANDLE hGC, GRECT_T *prRect, HSURFACE hDesSurface);
HSURFACE  GInitBackSurface(GHANDLE hGC, GUINT32 u4SurfW, GUINT32 u4SurfH);

BOOL UpdateDDrawOverlay(BOOL bFront,RECT rSrc,RECT rDest,GUINT32 fgOverlayFlag, PVOID  pDDrawParam,GUINT32 iSource);
/*@}*/

/************************MTK70886 2012.04.13 add start********************/
GVOID *GGetDDSPrimary(GVOID);
/************************MTK70886 2012.04.13 add end********************/

#ifdef __cplusplus
}
#endif

#endif /* _GGDI_H_ */