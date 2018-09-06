/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
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
 *   GStyle.h 
 *
 * Project:
 * --------
 *   GUIKit
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

#ifndef _GSTYLE_H_
#define _GSTYLE_H_

#include "GDef.h"
#include "GStringData.h"
#include "GGdi.h"

#define CFG_GUIKIT_NEW_STYLE   1 // Added by mcn03011 @2007/04/20

#define CFG_GUIKIT_DRAGDROP_SUPPORTED
#define CFG_GUIKIT_DRAGDROP_SUPPORTED_SPEEDUP
#define	CFG_GUIKIT_DRAGDROP_AUTO_SCROLL_SUPPORTED
#define LIST_VIEW_LYRIC_SUPPORT
#define CFG_GUIKIT_BACKUP_SURFACE

#ifdef __cplusplus
extern "C" {
#endif

#define DCF_NORMAL      0
#define DCF_ROUND       1
#define DCF_IMAGE       2
#define DCF_GPSPILOT1   3


#define DFF_NORMAL      0
#define DFF_ROUND       1
#define DFF_IMAGE       2


#define DCST_ACTIVE     0
#define DCST_INACTIVE   1

#define DCB_CLOSE       0

#define TYPE_DEFAULT         0  /**< Draw Defualt */
#define TYPE_SOLID           1  /**< Draw by Solid color */
#define TYPE_PATTERN         2  /**< Draw by Bitmap pattern */
#define TYPE_CUSTOM          3  /**< Draw by Custom Callback */
#define TYPE_PAGESCROLL      4  /**< Draw Page/Down btn for Scroll Bar */
#define TYPE_SHOWSPIN        5  /**< Draw Spin Button for Time/Date Ctrl */

enum
{
    DCBST_NORMAL,
    DCBST_DOWN,
    DCBST_MAX
};

enum
{
    STATE_NORMAL,
    STATE_PRESSED,
    STATE_FOCUSED,
    STATE_DISABLED
};

enum
{
    ITEM_TEXT_LEFT,
    ITEM_TEXT_CENTRE,
	ITEM_TEXT_RIGHT
};

enum
{
	MATRIX_PRE_PAGE,
	MATRIX_NEXT_PAGE
};


enum
{
    ARROW_LEFT,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN
};

#define DFST_ACTIVE     0
#define DFST_INACTIVE   1

enum
{
    BORDER_NULL = 0,
    BORDER_NORMAL,
    BORDER_ROUND,
    BORDER_SHADOW,
    BORDER_3DRAISED,
    BORDER_3DSUNKED,
    BORDER_BUTTON,
    BORDER_BUTTONDOWN,
    BORDER_PROGERSS,
    BORDER_SLIDER
};

typedef struct _BORDER 
{
    GUINT32  u4Style;
    GRECT_T rRect;
    GUINT32  crColor;
} GBORDER_T;


class GWidget;


typedef GVOID (*PFN_GDRAWBORDER)(GWidget *pWidget, GUINT32 u4WType, GHANDLE hGC,
                                GRECT_T *prRect, GUCHAR ucState, 
                                GBORDER_T *prBorder);
typedef GVOID (*PFN_GDRAWARROW)(GWidget *pWidget, GUINT32 u4WType, GHANDLE hGC,
                               GRECT_T *prRect, GUCHAR ucState,
                               GUCHAR ucDirection);
typedef GVOID (*PFN_GDRAWSLIDER)(GWidget *pWidget, GUINT32 u4WType, GHANDLE hGC,
                                GRECT_T *prRect, GUCHAR ucState);

typedef GVOID (*PFN_LOAD_IMG_ID)(GVOID *pvMsgBoxTheme);


typedef struct _GCAPTION
{
    GUINT32  u4Flags;

    GUCHAR   ucHeight;
    GINT16   i2IconX;
    GINT16   i2TextX;
    GUCHAR   ucIconTextSpacing;
    GUINT32  crTextActive;
    GUINT32  crTextInactive;
    
    union
    {
        struct 
        {
            GUINT32  crActive;
            GUINT32  crInactive;
            GUINT32  crGradientActive;
            GUINT32  crGradientInactive;
            GUCHAR   ucRoundW;
            GUCHAR   ucRoundH;
        } rNormal;
        struct  
        {
            GUINT16  u2ActiveLeftImg;
            GUINT16  u2ActiveTileImg;
            GUINT16  u2ActvieRightImg;
            GUINT16  u2InactiveLeftImg;
            GUINT16  u2InactiveTileImg;
            GUINT16  u2InactiveRightImg;
        } rImg;
    };
    
    GUCHAR    ucBtnMargin;
    GUCHAR    ucBtnSpacing;
    GSIZE_T  rBtnSize;
    GUINT32   u2CloseIcon[DCBST_MAX];
} GCAPTION_T;


typedef struct _GFRAME
{
    GUINT32 u4Flags;

    GUCHAR ucWidth;
    
    union
    {
        struct 
        {
            GUINT32  crActive;
            GUINT32  crInactive;
            GUINT32  crGradientActive;
            GUINT32  crGradientInactive;
            GUCHAR   ucRoundW;
            GUCHAR   ucRoundH;
        } rNormal;
        struct  
        {
            GUINT16  u2ActiveTileImg;
            GUINT16  u2ActiveLBImg;
            GUINT16  u2ActvieRBImg;
            GUINT16  u2InactiveTileImg;
            GUINT16  u2InactiveLBImg;
            GUINT16  u2InactiveRBImg;
        } rImg;
    };
} GFRAME_T;

enum
{
    COLOR_LIGHTLIGHT,
    COLOR_LIGHT,
    COLOR_DARK,
    COLOR_DARKDARK,
    COLOR_3DMAX
};

typedef struct _GCOMMONSTYLE
{
    GUINT32          acr3DBdr[COLOR_3DMAX];
    GUCHAR           uc3DBdrWidth;
    GUCHAR           ucBdrRoundW;
    GUCHAR           ucBdrRoundH;
    GBRUSH_T        rNormalBg;
    GBRUSH_T        rPressedBg;
    GBRUSH_T        rActiveBg;
    GBRUSH_T        rSelectedBg;
    GBRUSH_T        rDisabledBg;
    
    GUINT32          crNormalText;
    GUINT32          crPressedText;
    GUINT32          crActiveText;
    GUINT32          crSelectedText;
    GUINT32          crDisabledText;

    GUINT32          crLightFg;
    GUINT32          crDarkFg;
    GUINT32          crBlackFg;
    GUINT32          crDisableFg;
    GUINT32          crNormalFg;
   
    //The following information is for widget caption
    GCAPTION_T      rCaption;
    GFRAME_T        rFrame;

    PFN_GDRAWBORDER DrawBorder;
    PFN_GDRAWARROW  DrawArrow;
    PFN_GDRAWSLIDER DrawSlider;
} GCOMMONSTYLE_T;


typedef struct _GFORMSTYLE
{
    GBRUSH_T        rBg;
    GBORDER_T       rBdr;
    PFN_GDRAWBORDER DrawBorder;
} GFORMSTYLE_T;


typedef struct _PANELSTYLE
{
    GBRUSH_T        rBg;
    GBORDER_T       rBdr;
    PFN_GDRAWBORDER DrawBorder;
} GPANELSTYLE_T;


#define STATE_MAX 3

typedef struct _GBUTTONSTYLE
{
    GINT16           i2Margin;
    GINT16           i2MarginY;
    GINT16           i2IconTextGap;
    GINT16           i2IconAlign;  // 1:ICON_LEFT, 2:ICON_RIGHT 3:ICON_TOP, 4:ICON_BOTTOM
    GBORDER_T       prBorder[STATE_MAX];   //normal, focus, disable
    GBORDER_T       prBorderDown;
    GUCHAR           ucFontUp[STATE_MAX];  //normal, focus, disable
    GUCHAR           ucFontDown[STATE_MAX - 1];  //normal, focus
    GUINT32          crTextUp[STATE_MAX];  //normal, focus, disable
    GUINT32          crTextDown[STATE_MAX - 1];  //normal, focus
    GBRUSH_T        rBgUp[STATE_MAX];  //normal, focus, disable
    GBRUSH_T        rBgDown[STATE_MAX - 1];  //normal, focus
    PFN_GDRAWBORDER DrawBorder;
    PFN_LOAD_IMG_ID    pfnLoadImgID;

} GBUTTONSTYLE_T;


typedef struct _GCHECKBUTTONSTYLE
{
    GUCHAR      ucType;

    GINT16      i2Margin;
    GINT16      i2IconTextGap;
    GINT16      i2Dem;

    GUINT32     u2Icon[STATE_MAX];  //normal, focus, disable
    GUINT32     u2IconCheck[STATE_MAX];  //normal, focus, disable
    GUCHAR      ucFont[STATE_MAX];  //normal, focus, disable
    GUINT32     crText[STATE_MAX];  //normal, focus, disable
    GBRUSH_T   rBg[STATE_MAX];  //normal, focus, disable

} GCHECKBUTTONSTYLE_T;

typedef struct _GRADIOBUTTONSTYLE
{
    GUCHAR      ucType;

    GINT16      i2Margin;
    GINT16      i2IconTextGap;
    GINT16      i2Dem;

    GUINT32     u2Icon[STATE_MAX];  //normal, focus, disable
    GUINT32     u2IconCheck[STATE_MAX];  //normal, focus, disable
    GUCHAR      ucFont[STATE_MAX];  //normal, focus, disable
    GUINT32     crText[STATE_MAX];  //normal, focus, disable
    GBRUSH_T   rBg[STATE_MAX];  //normal, focus, disable
} GRADIOBUTTONSTYLE_T;

typedef struct _GPROGRESSSTYLE
{
    GUCHAR      ucType;
    GUINT16     u2HozIconFill;
    GUINT16     u2VerIconFill;
    GBRUSH_T   rBg;
    GBRUSH_T   rFg;
    GBORDER_T  rBorder;
    PFN_LOAD_IMG_ID pfnLoadImgID;
} GPROGRESSSTYLE_T;

typedef struct _GSLIDERSTYLE
{
    GUCHAR     ucType;

    GINT16     i2Dem;
    GUINT32    u2IconVer;
    GUINT32    u2IconHoz;
    GUINT32    u2IconVerF;
    GUINT32    u2IconHozF;
    GUINT32    u2IconVerDown;
    GUINT32    u2IconHozDown;
    GBRUSH_T  rUpFg[STATE_MAX];  //normal, focus, disable
    GBRUSH_T  rDownFg[STATE_MAX];  //normal, focus, disable
    GBRUSH_T  rBg[STATE_MAX];  //normal, focus, disable
    GBORDER_T rBorder;
#if CFG_GUIKIT_NEW_STYLE
    GUINT32    u2IconBarID;
    GUINT16    u2IconVerAdjustY;
    GUINT16    u2IconHozAdjustX;
#endif    
} GSLIDERSTYLE_T;

typedef struct _GSCROLLSTYLE
{
    GUCHAR      ucType;

    GUINT32     u2IconLeftArrow;
    GUINT32     u2IconRightArrow;
    GUINT32     u2IconUpArrow;
    GUINT32     u2IconDownArrow;
    GUINT32     u2IconKnobH;
    GUINT32     u2IconKnobV;
    GUINT32     u2IconLeftArrowDown;
    GUINT32     u2IconRightArrowDown;
    GUINT32     u2IconUpArrowDown;
    GUINT32     u2IconDownArrowDown;
    GUINT32     u2IconKnobHozDown;
    GUINT32     u2IconKnobVerDown;
    GBRUSH_T   rBg;
    GBRUSH_T   rFillBg;
    GBRUSH_T   rFillDownBg;
    GBORDER_T  rBorder;
    GUCHAR       ucKnobLimit;
} GSCROLLSTYLE_T;

typedef struct _GTABSTYLE
{
    GUINT32      prBorder;
    GUINT32      prSelectedFg;
    GBRUSH_T    *prBg;
    GBRUSH_T    *prSelectedBg;
    GBRUSH_T    *prNormalBg;
    GUCHAR       ucFont[STATE_MAX - 1];  //normal, selected
    GUINT32      *pcrText[STATE_MAX];  //normal, selected, disable
    GBORDER_T   rBorder;
    GINT16       i2TabButtonWidth;  //60
    GINT16       i2TabButtonHeight; //20
    GINT16       i2ArrowWidth  ;    //10
    GINT16       i2LableLeading;    //6
} GTABSTYLE_T;


typedef struct _GEDITSTYLE
{
    GBRUSH_T   rBg[STATE_MAX];/* 0 -- Normal, 1 -- Focused, 2 -- Disabled */
    GUINT32     crText[STATE_MAX];/* 0 -- Normal, 1 -- Focused, 2 -- Disabled */
    GBORDER_T  rBdr;
    GUCHAR      ucTextMargin; 
    GUCHAR      ucFont;
} GEDITSTYLE_T;


typedef struct _GCOMBOBOXSTYLE
{       
    GBRUSH_T  rBg[STATE_MAX]/* 0 -- Normal, 1 -- Focused, 2 -- Disabled */;
    GUINT32    crText[STATE_MAX]/* 0 -- Normal, 1 -- Focused, 2 -- Disabled */;
    GBORDER_T rBdr;
    GBORDER_T rDownBtnBdr;
    GUINT16    u2DownBtnW;
    GUINT32    crDownBtnBg;
    GUINT32    crFlagBg;
    GUINT32    u2DowBtnImg;
    GUCHAR     ucTextMargin;
    GUCHAR     ucFont;
} GCOMBOBOXSTYLE_T;


typedef struct _GPOPUPMENUSTYLE
{
    GBRUSH_T  rBg[STATE_MAX]/* 0 -- Normal, 1 -- Focused, 2 -- Disabled */;
    GUINT32    crText[STATE_MAX]/* 0 -- Normal, 1 -- Focused, 2 -- Disabled */;
    GUINT32    acrSeparator[STATE_MAX - 1];/* 0 -- Upper Line , 1 -- Bottom Line*/
    GSIZE_T   rItemDim;
    GUCHAR     ucSepHeight;
    GUCHAR     ucIconStPos;
    GPOINT_T  rTextStPos;
    GBORDER_T rBdr;
    GBORDER_T rItemFocusBdr;
    GUCHAR     ucFont;
} GPOPUPMENUSTYLE_T;



typedef struct _GSTATICSTYLE
{
    GBRUSH_T   rBg[STATE_MAX - 1]/* 0 -- Normal, 1 -- Disabled */;
    GUINT32     crText[STATE_MAX - 1]/* 0 -- Normal, 1 -- Disabled */;
    GBORDER_T  rBdr;
    GUCHAR      ucFont;
    GUINT16     u2LeftMargin;
} GSTATICSTYLE_T;


typedef struct _GMSGBOXSTYLE
{
    GUINT32 u2WarnImg;
    GUINT32 u2QuesImg;
    GUINT32 u2InfoImg;
#if CFG_GUIKIT_NEW_STYLE
    GUINT16 u2BtnStrIDOk;
    GUINT16 u2BtnStrIDNo;
    GUINT16 u2BtnStrIDYes;
    GUINT16 u2BtnStrIDRetry;
    GUINT16 u2BtnStrIDCancel;
    GUINT16 u2BtnStrIDContinue;
    GRECT_T        *prRect;
    GBUTTONSTYLE_T *prBtnStyle;
    GFORMSTYLE_T   *prFrmStyle;
    GCAPTION_T     *prCapStyle;
#endif
    PFN_LOAD_IMG_ID    pfnLoadImgID;
}GMSGBOXSTYLE_T;


 /*@{*/
/**
 * @name Flags for View Control Item.
 */
#define VCISF_CENTER_ICON_X         0x00000001L
#define VCISF_CENTER_ICON_Y         0x00000002L
#define VCISF_CENTER_TEXT_X         0x00000004L
#define VCISF_CENTER_TEXT_Y         0x00000008L
#define VCISF_CENTER_ICONTEXT_X     0x00000010L
#define VCISF_CENTER_ICONTEXT_Y     0x00000020L
#define VCISF_DISABLE_BKG           0x00000100L
#define VCISF_PRESSED_WITHBKG       0x00000200L
#define VCISF_AUTO_SCROLLTEXT       0x00000400L
#define VCISF_PRESSED_WITHICON      0x00000800L
#define VCISF_RIGHT_ICON_X			0x00001000L //mtk71372 2011.11.28 added
/*@}*/

typedef struct _GVCISTYLE
{
    /**
     * Menu item background brush, arBg[0] is normal background brush, arBg[1]
     * is background brush when menu item is focused, arBg[2] is background
     * brush when menu item is selected, arBg[3] is background brush when menu
     * item is disabled.
     * see VCISST_NORMAL
     */
    GBRUSH_T rBg[6];
    /**
     * Menu item foreground color, arFg[0] is normal foreground color, arFg[1]
     * is foreground color when menu item is focused, arFg[2] is foreground
     * color when menu item is selected, arFg[3] is foreground color when menu
     * item is disabled.
     * see VCISST_NORMAL
     */
    GUINT32  crText[6];
    GUINT32  ucFont[6];
    
    GBORDER_T *prFocusedBdr;
    GRECT_T   *prMarkedRect;

    GUINT32   u4Flags; /**< Common menu item's flags */
    
    GINT16    i2Width; /**< Common menu item's width */
    GINT16    i2Height; /**< Common Menu item's height */
    
    GINT16    i2IconX; /**< Horizontal position of the common menu item icon */
    GINT16    i2IconY; /**< Vertical position of the common menu item icon */
    GINT16    i2IconW; /**< Common menu item icon's width */
    GINT16    i2IconH; /**< Common menu item icon's height */
    
    GINT16    i2TextX; /**< Horizontal position of the common menu item text string */
    GINT16    i2TextY; /**< Vertical position of the common menu item text string */
} GVCISTYLE_T;


typedef struct _GLVSTYLE
{
    GBRUSH_T        rBg;
    GBORDER_T       rBdr;
    
    /**
     * The distance between the left boundary of list menu and menu item.
     */
    GUCHAR           ucLeft;
    /**
     * The distance between the top boundary of the list menu and the 
     * first displayed menu item
     */
    GUCHAR           ucTop;

    GINT16           i2Spacing;

    /**
     * The width of vertical scroll bar
     */
    GUCHAR           ucVBW;
#if CFG_GUIKIT_NEW_STYLE
    GUINT32          fgPageScroll;
#endif
    GVCISTYLE_T     rItem;

    PFN_GDRAWBORDER DrawBorder;
} GLVSTYLE_T;


typedef struct _GMVSTYLE
{
    GBRUSH_T        rBg;
    GBORDER_T       rBdr;
    /**
     * The distance between the left boundary of the matrix menu and the first
     * displayed menu item.
     */
    GUCHAR           ucLeft;
    /**
     * The distance between the top boundary of the matrix menu and the first
     * displayed menu item.
     */
    GUCHAR           ucTop;

    GINT16           i2XSpacing;
    GINT16           i2YSpacing;

    /**
     * The width of vertical scroll bar
     */
    GUCHAR           ucVBW;
    /**
     * The height of horizontal scroll bar
     */
    GUCHAR           ucHBH;
    
    GVCISTYLE_T     rItem;
    
    /**
    * The flag of Flash Focus effect with a normal item icon
    */
    GUINT32        fgFlashFocus;

    PFN_GDRAWBORDER DrawBorder;
	GBOOL			fgPageScroll;
} GMVSTYLE_T;

#if CFG_GUIKIT_NEW_STYLE
typedef struct _GDATECTRLSTYLE
{
    GUCHAR  ucType;
    GUINT16 u2BoxWidth;
    GUINT16 u2BoxHeight;
    GUINT16 u2BoxGap;

    GUINT16 u2BtnWidth;
    GUINT16 u2BtnHeight;
    GUINT32 u2BtnUpImg_N;
    GUINT32 u2BtnUpImg_F;
    GUINT32 u2BtnDownImg_N;
    GUINT32 u2BtnDownImg_F;
    GUINT16 u2BtnBoxGap;
}GDATECTRLSTYLE_T;

typedef struct _GTIMECTRLSTYLE
{
    GUCHAR  ucType;
    GUINT16 u2BoxWidth;
    GUINT16 u2BoxHeight;
    GUINT16 u2BoxGap;

    GUINT16 u2BtnWidth;
    GUINT16 u2BtnHeight;
    GUINT32 u2BtnUpImg_N;
    GUINT32 u2BtnUpImg_F;
    GUINT32 u2BtnDownImg_N;
    GUINT32 u2BtnDownImg_F;
    GUINT16 u2BtnBoxGap;
}GTIMECTRLSTYLE_T;
#endif

typedef enum
{
    GTHEME_MEMORY,
    GTHEME_NVRAM,
    GTHEME_FILE
} GTHEME_TYPE_T;


typedef struct _GMEMTHEME
{
    GCOMMONSTYLE_T      *prCommonTheme;
    GFORMSTYLE_T        *prFormTheme;
    GSTATICSTYLE_T      *prStaticTheme;
    GBUTTONSTYLE_T      *prButtonTheme;
    GCHECKBUTTONSTYLE_T *prCheckButtonTheme;
    GRADIOBUTTONSTYLE_T *prRadioButtonTheme;
    GPROGRESSSTYLE_T    *prProgressTheme;
    GSLIDERSTYLE_T      *prSliderTheme;
    GSCROLLSTYLE_T      *prScrollTheme;
    GTABSTYLE_T         *prTabTheme;
    GEDITSTYLE_T        *prEditTheme;
    GCOMBOBOXSTYLE_T    *prComboBoxTheme;
    GLVSTYLE_T          *prLVTheme;
    GMVSTYLE_T          *prMVTheme;
    GPOPUPMENUSTYLE_T   *prPopupMenuTheme;
    GMSGBOXSTYLE_T      *prMsgBoxTheme;
    GDATECTRLSTYLE_T    *prDataCtrlTheme;
    GTIMECTRLSTYLE_T    *prTimeCtrlTheme;
} GMEMTHEME_T;


typedef struct _GTHEME
{
    /**
     * String identify of GUI Kit theme.
     */
    GTCHAR *szName;

    /**
     * GUI Kit theme type.
     */
    GTHEME_TYPE_T eType;
    
    union
    {
        GMEMTHEME_T *prMemTheme; /**< Pointer to GUI Kit memory theme. */
        GTCHAR *szFileName; /**< Configure file which GUI Kit theme data will be loaded from. */
    };
    
} GTHEME_T;


GAPI GBOOL  GLoadTheme(const GTHEME_T *prTheme);

GAPI GVOID  GDrawBorder(GHANDLE hGC, GRECT_T *prRect, GBORDER_T *prBorder);
GAPI GVOID  GRectExcludeBorder(GRECT_T *prRect, GBORDER_T *prBorder);
GAPI GVOID  GGetBorderWidth(GBORDER_T *prBorder, GRECT_T *prRect);
GAPI GVOID  GScrollText(GHANDLE hGC, GINT16 i2ClientWidth, GTCHAR *pszTxt);
GAPI GBOOL  GFillRectEx(GHANDLE hGC, GRECT_T *prRect, GBRUSH_T *prBrush, GRECT_T *prImgRect);
GAPI GBOOL  GFillRectEx2(GHANDLE hGC, GRECT_T *prRect, GBRUSH_T *prBrush, 
                        GRECT_T *prImgRect, GDRAWIMGTYPE_T eDIType,
                        GDIAMETHOD_T eDIAMode);

#ifdef __cplusplus
}
#endif

#endif //_GSTYLE_H_  