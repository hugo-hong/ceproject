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
 *   GButton.h 
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
 *   mcn03046 mcn05013
 *
 * Last changed:
 * ------------- 
 * $Author: zeng.zhang $ 
 *
 * $Modtime: $  
 *
 * $Revision: #1 $
****************************************************************************/


#ifndef _GBUTTON_H_
#define _GBUTTON_H_

#include "GWidget.h"
#include "GGdi.h"
#include "GStyle.h"
#include "GStringData.h"

// Added by mcn03011 @2007/07/13
// 1: Enable Scroll the button text when the text overstep
//    button width.
// 0: Disable Scroll. 
#define CFG_SCROLL_BTN_TEXT 1


//Added by Yu.Mo 2011-09-28 --------------------------------------------- str

//如果该宏被定义，当按钮的文字支持换行
//#define CFG_BTN_MULTILINE_TEXT 



//如果该宏被定义，则字长度大于按钮宽度则自动换行，否则仅仅是遇到“\n”时换行。注意，即使该宏被定义，遇到“\n”也是换行的
//该宏不能和CFG_BTN_AUTO_LINE_FEED同时使用
//#define CFG_BTN_AUTO_LINE_FEED

//如果一行的长度大于按钮宽度，则用“...”表示。
//该宏不能和CFG_BTN_AUTO_LINE_FEED同时使用
//#define CFG_BTN_MULTILINE_LINE_ELLIPSIS



//换行的方向向上伸长，该宏不能和CFG_BTN_MULTILINE_BREAK_DOWN,CFG_BTN_MULTILINE_BREAK_MIDDLE同时定义
//#define CFG_BTN_MULTILINE_BREAK_UP

//换行的方向向下伸长，该宏不能和CFG_BTN_MULTILINE_BREAK_UP,CFG_BTN_MULTILINE_BREAK_MIDDLE同时定义
//#define CFG_BTN_MULTILINE_BREAK_DOWN

//换行以中间为准线向上下延伸,该宏不能和CFG_BTN_MULTILINE_BREAK_DOWN,CFG_BTN_MULTILINE_BREAK_UP同时定义
//#define CFG_BTN_MULTILINE_BREAK_MIDDLE


//Addy by Yu.Mo 2011-09-28 --------------------------------------------- end
typedef enum
{
    GZOOM_LEFT2RIGHT,
    GZOOM_RIGHT2LEFT,
    GZOOM_CENTER2LR
} GBTNZOOMMODE_T;

#if CFG_GUIKIT_NEW_STYLE
typedef struct _BUTTONSPECDATA_EX
{
    GUINT32         u4IconID;
    GUINT32         u2IconX;
    GUINT32         u2IconY;

    GStringData   *pstrText1;
    GUINT32         u2Text1X;
    GUINT32         u2Text1Y;
    GUINT32          ucText1Font;

    GStringData   *pstrText2;
    GUINT32         u2Text2X;
    GUINT32         u2Text2Y;
    GUINT32          ucText2Font;

    GStringData   *pstrText3;
    GUINT32         u2Text3X;
    GUINT32         u2Text3Y;
    GUINT32          ucText3Font;
} GBUTTONSPECDATA_EX_T;
#endif

typedef struct _BUTTONSPECDATA
{
    GUINT32               u2IconID;
    GUINT32               u4IconDownID;    // Add by mtk40006 for Button Foreground Icon
    GUINT32               u4IconFocusID;   // Add by mtk40006 for Button Foreground Icon
    GUINT32               u4IconDisableID; // Add by mtk40006 for Button Foreground Icon
    GStringData          *pstrText;
#if CFG_GUIKIT_NEW_STYLE
    GBUTTONSTYLE_T       *prStyle;
    GBUTTONSPECDATA_EX_T *prSDataEx;
#endif
} GBUTTONSPECDATA_T;

class GButton : public GWidget
{
public:
    GButton(GVOID);
    GButton(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED|WF_TABSTOP,
            GUINT32 u4ID = 0xFFFFFFFF);
    virtual ~GButton();

    enum
    {
        ICON_LEFT = 1,
        ICON_RIGHT = 2,
        ICON_TOP = 3,
        ICON_BOTTOM = 4,
#if CFG_GUIKIT_NEW_STYLE
        TEXT_LEFT = 5
#endif
    };
    
#if CFG_GUIKIT_NEW_STYLE
    GVOID    SetStyle(GBUTTONSTYLE_T *prStyle);
    GVOID    GetStyle(GBUTTONSTYLE_T *prStyle);
#endif
    GVOID    SetText(GTCHAR *szText);
    GVOID    SetText(GUINT16 u2StrID);
    GVOID    SetText(GStringData *pstrText);
    inline GStringData *GetText(GVOID) { return (&m_strText); }
    GVOID    SetIconBgUp(GUINT16 u2IconID);
    GUINT32  GetIconBgUp(GVOID);
    GVOID    SetIconFocusBgUp(GUINT16 u2IconID);
    GUINT32  GetIconFocusBgUp(GVOID);
    GVOID    SetIconBgDown(GUINT16 u2IconID);
    GUINT32  GetIconBgDown(GVOID);
    GVOID    SetIcon(GUINT16 u2IconID, GBOOL fgRedraw = TRUE);
    GUINT16  GetIcon(GVOID);
    GVOID    SetFocusIcon(GUINT32 u4IconID); //Jie Chen, Add properties for GButton, 2009-3-23. 
    GUINT32  GetFocusIcon(GVOID);            //Jie Chen, Add properties for GButton, 2009-3-23.
    GVOID    SetDownIcon(GUINT16 u2IconID);
    GUINT16  GetDownIcon(GVOID);
    GVOID    SetDisableIcon(GUINT32 u4IconID);//Jie Chen, Add properties for GButton, 2009-3-23.
    GUINT32  GetDisableIcon(GVOID);           //Jie Chen, Add properties for GButton, 2009-3-23.
    GBOOL    IsPressed(GVOID);
    GVOID    UndoPressed(GVOID);
#if CFG_GUIKIT_NEW_STYLE
    GVOID    SetSpecDateEx(GBUTTONSPECDATA_EX_T *prSpecDataEx);
    GBOOL    GetSpecDateEx(GBUTTONSPECDATA_EX_T **prSpecDataEx);
    GBOOL    Zoom(GBOOL fgZoomIn, GBTNZOOMMODE_T eMode);
#endif

    //For Toggle Button, add by mcn03046
    GVOID    SetToggle(GBOOL fgToggled);
    GBOOL    IsToggled(GVOID);
   
    GWGTSTATE_T GetState(VOID) const;

    GVOID    StopTextScroll(GVOID);

    GVOID *operator new(size_t u4Size);
    GVOID operator delete(GVOID *pvObj);
    
    static GWidget *CreateInstance(GVOID *pvSpecData);

    // Widget Flag
    enum
    {
        WF_HSCROLLTEXT = 1 << 0,
        WF_VSCROLLTEXT = 1 << 1,
        WF_MULTILINE   = 1 << 2, // For Draw the Text3 of GBUTTONSPECDATA_EX_T!
        WF_TOGGLE      = 1 << 3  //Toggle Button
    };

protected:
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    GVOID Paint(GHANDLE hGC);

    enum
    {
        WIF_DOWN = 1 << 0,
        WIF_TOGGLED = 1 << 1
    };

private:
    GUINT16       m_u2IconID;
    GUINT16       m_u2IconDownID;
    GUINT32       m_u4FocusIcon;   //Jie Chen, Add properties for GButton, 2009-3-23.
    GUINT32       m_u4DisableIcon; //Jie Chen, Add properties for GButton, 2009-3-23.
    GStringData      m_strText;
    GBUTTONSTYLE_T *m_prStyle;

#if CFG_GUIKIT_NEW_STYLE
    GBUTTONSPECDATA_EX_T *m_prSpecDataEx;
#endif

    GVOID PaintButtonDown(GHANDLE hGC, GINT16 i2Width, GINT16 i2Height,
        GPEN_T *prDark, GPEN_T *prLight, GBRUSH_T *prDown);
    GVOID PaintButtonBorder(GHANDLE hGC, GINT16 i2Width, GINT16 i2Height,
        GPEN_T *prDark, GPEN_T *prLight);
    GUINT32 SelectTextColor(GBUTTONSTYLE_T *prStyle);
    GUINT32 SelectFont(GBUTTONSTYLE_T *prStyle);
    GBRUSH_T *SelectBg(GBUTTONSTYLE_T *prStyle);
    GUINT32        SelectIcon(GBUTTONSTYLE_T *prStyle); //Jie Chen, Add properties for GButton, 2009-3-23.

    GINT8    m_iTxtScrollID;
    GINT8    m_aiTxtScrollIDs[3];

//Yu.Mo add ------------------------------------------------------------------- str
#ifdef CFG_BTN_MULTILINE_TEXT

private:
	GVOID GDrawStringW(GHANDLE hGC, GINT32 i4X, GINT32 i4Y, GWCHAR *szString);
	GVOID GScrollText(GHANDLE hGC, GINT16 i2ClientWidth, GTCHAR *pszTxt);
#endif //#ifdef CFG_BTN_MULTILINE_TEXT
//Yu.Mo add -------------------------------------------------------------------- end
};


#endif //_GBUTTON_H_