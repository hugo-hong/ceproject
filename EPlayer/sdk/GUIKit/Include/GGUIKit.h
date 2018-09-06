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
 *   GGUIKit.h 
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


#ifndef _GGUIKIT_H_
#define _GGUIKIT_H_


#include "GForm.h"
#include "GButton.h"
#include "GCheckButton.h"
#include "GRadioButton.h"
#include "GScrollBar.h"
#include "GSlider.h"
#include "GProgressBar.h"
#include "GRadioGroup.h"
#include "GEdit.h"
#include "GComboBox.h"
#include "GViewCtrl.h"
#include "GTab.h"
#include "GTabPage.h"
#include "GDateCtl.h"
#include "GTimeCtl.h"
#include "GPopupMenu.h"
#include "GStatic.h"
#include "GMsgBox.h"
#include "GStyle.h"
#include "GAnimate.h"
#include "GEvent.h"

#include "GView.h"
#include "GStringData.h"


#define CLASS_SIZE(x) (sizeof(x))
#define INVALID_POINT  0x0000FFFF 

#define GUIKIT_VERSION        TEXT("MT3360_CE60_GUIKIT_W1207_01")

enum
{
    WT_FORM,
    WT_BUTTON,
    WT_RADIO,
    WT_CHECK,
    WT_SLIDER,
    WT_PROGRESSBAR,
    WT_SCROLLBAR,
    WT_LISTVIEW,
    WT_MATRIXVIEW,
    WT_EDIT,
    WT_COMBOBOX,
    WT_TAB,
    WT_DATECTL,
    WT_TIMECTL,
    WT_STATIC,
    WT_ANIMATE,
    WT_PANEL,
    WT_MAX
};


enum
{
    MEMORY_DEFAULT
};

#if defined(GUIKIT_INTERNALMEMORY_DYNAMIC)

typedef struct
{
    GUINT32 u4FormNum;
    GUINT32 u4LVNum;
    GUINT32 u4MVNum;
    GUINT32 u4ComboBoxNum;
    GUINT32 u4EditNum;
    GUINT32 u4PopupMenuNum;
    GUINT32 u4ScrollBarNum;
    GUINT32 u4SliderNum;
    GUINT32 u4ProgressBarNum;
    GUINT32 u4RadioGroupNum;
    GUINT32 u4RadioButtonNum;
    GUINT32 u4CheckButtonNum;
    GUINT32 u4ButtonNum;
    GUINT32 u4TabNum;
    GUINT32 u4TabPageNum;
    GUINT32 u4TimeCtlNum;
    GUINT32 u4DateCtlNum;
    GUINT32 u4StaticNum;
    GUINT32 u4AnimateNum;
} MEMORYSPEC_T;

#elif  defined(GUIKIT_EXTERNALMEMORY)

typedef struct
{
    GUINT32  u4StartAddress;
    GUINT32  u4Size;
} OUTMEMORYARG_T;


typedef struct
{
    OUTMEMORYARG_T rFormClass;
    OUTMEMORYARG_T rLVClass;
    OUTMEMORYARG_T rMVClass;
#if GUIKIT_SUPPORTED_GCOMBOBOX
    OUTMEMORYARG_T rComboBoxClass;
#endif
    OUTMEMORYARG_T rEditClass;
#if GUIKIT_SUPPORTED_GPOPUPMENU
    OUTMEMORYARG_T rPopupMenuClass;
#endif
    OUTMEMORYARG_T rScrollBarClass;
    OUTMEMORYARG_T rSliderClass;
    OUTMEMORYARG_T rProgressBarClass;
    OUTMEMORYARG_T rRadioGroupClass;
    OUTMEMORYARG_T rRadioButtonClass;
    OUTMEMORYARG_T rCheckButtonClass;
    OUTMEMORYARG_T rButtonClass;
#if GUIKIT_SUPPORTED_GTABPAGE
    OUTMEMORYARG_T rTabClass;
    OUTMEMORYARG_T rTabPageClass;
#endif
#if GUIKIT_SUPPORTED_GTIMECTRL
    OUTMEMORYARG_T rTimeCtlClass;
#endif
#if GUIKIT_SUPPORTED_GDATECTRL
    OUTMEMORYARG_T rDateCtlClass;
#endif
    OUTMEMORYARG_T rStaticClass;
#if GUIKIT_SUPPORTED_GANIMATE
    OUTMEMORYARG_T rAnimateClass;
#endif
#if GUIKIT_SUPPORTED_GPANEL
    OUTMEMORYARG_T rPanelMem;
#endif
} MEMORYSPEC_T;

#elif defined(GUIKIT_INTERNALMEMORY_STATIC)

typedef struct
{
} MEMORYSPEC_T;

#endif

typedef struct
{
    MEMORYSPEC_T   *prMemorySpec;
    GINT16           i2ScreenWidth;
    GINT16           i2ScreenHeight;
    GUCHAR           ucLayerID;
    GUCHAR           ucAlphaLayerID;
    GUINT8           uPrimaryBpp;
    GUINT8           uAlphaBpp;
} GGUIKITARG_T;


typedef enum
{
    GFSM_NORMAL,
    GFSM_GLIDELEFT2RIGHT,
    GFSM_GLIDERIGHT2LEFT,
    GFSM_GLIDETOP2BOTTOM,
    GFSM_GLIDEBOTTOM2TOP,
    GFSM_ZOOM,
    GFSM_FLYZOOMIN,
    GFSM_FLYZOOMOUT,
    GFSM_FLYWIDGETS,
    GFSM_FADEIN,
    GFSM_FADEOUT,
    GFSM_SHUTTER,
    GFSM_TURNCLOCKWISE,
    GFSM_TURNANTICLOCKWISE,
    GFSM_ROTATE_WIDGETS,
    GFSM_CUSTOM
} GGUIKITFSMODE_T;


typedef struct
{
    GRECT_T     rRawFrameRect;
    GUINT32      u4Frames;
    GRECT_T    *prFrameRects;
} GFLYZOOMPARAM_T;

typedef struct
{
    GWidget *pWidgets[10];
    GPOINT_T rPos[10][10];
    GUINT32  u4WidgetNum;
    GUINT32  u4Frames;
}GFLYWIDGETSPARAM_T;

typedef GVOID (*PFN_GUIKITROTATECB)(GWidget *pWidget, GUINT32 u4Param1, GUINT32 u4Param2);

typedef struct
{
    GWidget *pWidgets[10];
    PFN_GUIKITROTATECB pfnCb;  
    GUINT32  u4WidgetNum;
}GROTATEWIDGETSPARAM_T;

enum
{
    GCM_NORMAL,
    GCM_FOREGROUND,
    GCM_BACKGROUND
};


typedef GVOID (*PFN_GUIKITRENDERCB)(GINT32 i4Left, GINT32 i4Top, GINT32 i4Right, 
                                   GINT32 i4Bottom);

GVOID   GEnableGUIKitLog(GBOOL fgEnable);

GBOOL   GetGuikitVersion(TCHAR* szReturnedString, GUINT32 iStringLen);

GBOOL   GInitGUIKit(GGUIKITARG_T *pvArg);
GBOOL   GChngGUIKit(GGUIKITARG_T *pvArg);
GVOID   GDeinitGUIKit(GVOID);
GVOID   GRegGUIKitRenderFunc(PFN_GUIKITRENDERCB pfnRenderCb);
GVOID   GSetGUIKitFSMode(GGUIKITFSMODE_T eFSMode);
GVOID   GSetGUIKitFSDuration(GUINT16 u2Millisecond);


GVOID   GGUIKitFlushScreen(GINT32 i4Left, GINT32 i4Top, GINT32 i4Right,
                          GINT32 i4Bottom, GBOOL fgSyncRender);

GHANDLE GBeginPaint(GWidget *pWidget);
GVOID   GEndPaint(GWidget *pWidget, GHANDLE hGC);

GVOID   GSetFlyZoomParam(GFLYZOOMPARAM_T *prFlyZoomParam, GBOOL fgZoomIn = TRUE);
GVOID   GSetFlyWidgetsParam(GFLYWIDGETSPARAM_T *prFlyParam);
GVOID   GSetRotateWidgetsParam(GROTATEWIDGETSPARAM_T *prRotateParam);
GVOID   GSetFSRect(GRECT_T *prRect);
GVOID   GSetFSWidget(GWidget *pWidget);
GVOID   GSetFSLayerID(GUINT8 u4LayerID);
GUINT8  GGetFSLayerID(GVOID);
GVOID   GSetFSLayerBpp(GUINT32 u4Bpp);
GUINT32 GGetFSLayerBpp(GVOID);


GINT8    GStartScrollText(GWidget *pWidget, GRECT_T *prScrExtent, GINT32 i4TextW);
GVOID    GStopScrollText(GUINT8 uScrollID);
GINT32   GGetScrollTextX(GUINT8 uScrollID);
GUINT32  GGetScrollTextW(GUINT8 uScrollID);
GBOOL    GSetScrollExtent(GUINT8 uScrollID, GRECT_T *prScrExtent);
GBOOL    GGetScrollExtent(GUINT8 uScrollID, GRECT_T *prScrExtent);

GVOID   GSetGUIKitCooperativeMode(GUINT8 uCoopMode);
GUINT8  GGetGUIKitCooperativeMode(GVOID);

#endif //_GGUIKIT_H_