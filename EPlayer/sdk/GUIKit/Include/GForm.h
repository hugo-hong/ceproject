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
 *   GForm.h
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


#ifndef _GFORM_H_
#define _GFORM_H_

#include "GStringData.h"
#include "GStyle.h"
#include "GContainer.h"


/**
* @defgroup groupForm Form
* @ingroup groupContainer
* @{
*/

#define FORM_BTN_MAX 10

enum
{
    FBF_VISIBLE = 1 << 0,
    FBF_ICON    = 1 << 1,
    FBF_TEXT    = 1 << 2,
    FBF_CENTER  = 1 << 3,
    FBF_NCUNHIT = 1 << 4,
};

enum
{
    FORMBTNST_NORMAL = 0,
    FORMBTNST_DOWN,
    MAX_FORMBTNST
};

typedef struct _FORMBTNINFO
{
    GUINT32  u4Flags;
    GUINT32  u4ID;
    GUINT32  u2Icon[MAX_FORMBTNST];
    GTCHAR  *szText;
    GUINT32  u4Data;
#if CFG_GUIKIT_NEW_STYLE
    GRECT_T *prRect;
#endif
} GFORMBTNINFO_T;


typedef struct _FORMTEMPLATE
{
    GUINT32 u4WFlags;
    GUINT32 u4ID;
    GUINT32 u2Icon;
    GStringData *pstrCaption;

    GINT32  i4X;
    GINT32  i4Y;
    GINT32  i4Width;
    GINT32  i4Height;
    
    GWGTTEMPLATE_T *prWgtTemplate;

    GFORMSTYLE_T   *prFormStyle;
    GCAPTION_T     *prCaptionStyle;
    GFRAME_T       *prFrameStyle;
} GFORMTEMPLATE_T;


class GView;

class GForm : public GContainer
{
public:
    static GForm *GetForm(GUINT32 u4ID);

    inline static GForm *GetActived(GVOID) { return (pActived); }
    inline static GForm *GetTopmost(GVOID) { return (pTopmost); }

    GForm(GVOID);
    GForm(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED | WF_TABSTOP,
          GUINT32 u4ID = -1);
    virtual ~GForm();

    GVOID   SetView(GView *pView) { m_pView = pView; }
    GView *GetView(GVOID) { return (m_pView); }

    GVOID   SetStyle(GFORMSTYLE_T *prStyle);
    GVOID   GetStyle(GFORMSTYLE_T *prStyle);
#if CFG_GUIKIT_NEW_STYLE
    GVOID   SetCaptionStyle(GCAPTION_T *prCaptionStyle);
    GVOID   GetCaptionStyle(GCAPTION_T *prCaptionStyle);
    GVOID   SetNCClient(GWidget *pvWidget);
    GVOID   SetFrame(GFRAME_T *prFrame);
    GVOID   GetFrame(GFRAME_T *prFrame);
#endif
    GBOOL   SetParent(GWidget *pParent);
    GVOID   SetText(GStringData *pstrText);
    GVOID   SetText(GTCHAR *szText);
    GVOID   SetText(GUINT16 u2TextID);
  
#if CFG_GUIKIT_NEW_STYLE
    GVOID   SetText(GUINT16 u2X, GTCHAR *szText);
    GVOID   SetText(GUINT16 u2X, GUINT16 u2TextID);
    GVOID   SetText(GUINT16 u2X, GStringData *pstrText);
#endif

    GVOID   GetText(GStringData *pstrText);
    GVOID   SetIcon(GUINT16 u2Icon);
    GUINT16 GetIcon(GVOID);
    GBOOL   IsActive(GVOID);
    
    GBOOL   SetButtonChunk(GFORMBTNINFO_T *prButtons, GUINT16 u2MaxCount);
    GBOOL   SetButtons(GFORMBTNINFO_T *prButtons, GUINT16 u2Count);
    GBOOL   SetButton(GFORMBTNINFO_T *prButton);
    GVOID   AddButton(GUINT32 u4ID, GUINT16 u2Icon,
                     GUCHAR ucState = FORMBTNST_NORMAL);
    GVOID   AddButton(GUINT32 u4ID, GTCHAR *szText);
#if CFG_GUIKIT_NEW_STYLE
    GVOID   AddButton(GFORMBTNINFO_T *prFormBtnInfo);
    GUINT32 GetButtonID(GVOID);
    GVOID   SetButtonHit(GUINT32 u4ID, GBOOL fgHit = TRUE);
#endif
    GVOID   RemoveButton(GUINT32 u4ID);
    GVOID   SetButtonIcon(GUINT32 u4ID, GUINT16 u2Icon,
                         GUCHAR ucState = FORMBTNST_NORMAL);
    GVOID   SetButtonText(GUINT32 u4ID, GTCHAR *szText);
    GVOID   SetButtonFlags(GUINT32 u4ID, GUINT32 u4Flags);
    GVOID   ShowButton(GUINT32 u4ID);
    GVOID   HideButton(GUINT32 u4ID);

    GVOID  *operator new(size_t u4Size);
    GVOID  *operator new(size_t u4Size, GVOID *pvBuf);
    GVOID   operator delete(GVOID *pvObj);

    virtual GWidget *TouchHitTest(GPOINT_T *prPt);

    enum
    {
        WF_CAPTION = 1 << 0,
        WF_FRAME = 1 << 1,
        WF_CLOSE = 1 << 2,
        WF_MODAL = 1 << 3,
        WF_TOPMOST = 1 << 4,
        WF_TOPTOPMOST = 1 << 5,
        WF_NOACTIVE = 1 << 6
    };

protected:
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    virtual GVOID UpdateClientRect(GVOID);
    
    virtual GVOID UpdateDataByWFlags(GUINT32 u4WFlags);
    
    GVOID    DrawButton(GHANDLE hGC, GRECT_T *prRect, GUCHAR ucState,
                       GFORMBTNINFO_T *prButton);
#if CFG_GUIKIT_NEW_STYLE
    GVOID DrawCaption(
            GHANDLE      hGC, 
            GRECT_T     *prRect, 
            GUINT16      u2Icon,
            GStringData *pstrCaption, 
            GUCHAR       ucState);

    GVOID DrawCaptionEx(
            GHANDLE      hGC, 
            GRECT_T     *prRect, 
            GUINT16      u2Icon,
            GStringData *pstrCaption, 
            GUCHAR       ucState, 
            GUINT32      u4Flags);

    GVOID DrawCaptionButton(
            GHANDLE  hGC, 
            GRECT_T *prRect, 
            GUCHAR   ucButton, 
            GUCHAR   ucState);

    GVOID DrawFrame(
            GHANDLE  hGC, 
            GRECT_T *prRect, 
            GUCHAR   ucState);

    GVOID DrawFrameEx(
            GHANDLE  hGC, 
            GRECT_T *prRect, 
            GUCHAR   ucState, 
            GUINT32  u4Flags);
#endif
    GBOOL    NcHitTest(GPOINT_T *prPt);
    GVOID    NcPaint(GHANDLE hGC);

private:
    static GBOOL GetVisibleFormOrView(GForm **ppForm, GView **ppView,
                                     GUINT16 u2Level);
    GVOID    Push(GVOID);
    GVOID    Pop(GVOID);
    GVOID    BringToTop(GVOID);
    GVOID    AdjustPrevNextView(GVOID);
    GVOID    PaintButton(GHANDLE hGC);

    static  GBOOL   fgFormTopmost;
    static  GBOOL   fgFormInView;
    static  GForm *pActived;
    static  GForm *pTopmost;

    static  GBOOL   fgFormTopmostEx;
    static  GBOOL   fgFormTopmostEx2;

    static  GForm  *pTopmostEx;
    static  GForm  *pTopmostEx2;

    enum
    {
        NCHIT_CAPTION,
        NCHIT_CLOSE,
        NCHIT_NULL
    };
    
    enum
    {
        WIF_NCHITCHANGED = 1 << 0,
        WIF_DYNAMICBTNCHUNK = 1 << 1,
        WIF_PUSHED = 1 << 2
    };

    GView          *m_pView;
    GView          *m_pPrevView;
    GView          *m_pNextView;
    GFORMSTYLE_T   *m_prStyle;
#if CFG_GUIKIT_NEW_STYLE
    GCAPTION_T     *m_prCaption;
    GFRAME_T       *m_prFrame;
#endif
    GStringData     m_strText;
    GStringData     m_strInfo;
    GUINT16          m_u2InfoX;
    GUINT16          m_u2Icon;
    GUCHAR           m_ucNcTouchDownItem;
    GUCHAR           m_ucNcHitItem;
    GUINT32          m_u4NcTouchDownBtn;
    GUINT32          m_u4NcHitBtn;
    GFORMBTNINFO_T *m_prButtons;
    GUINT16          m_u2MaxBtnCount;
    GUINT16          m_u2BtnCount;

    GWidget        *m_pvNcWidget;

    friend class GWidget;
    friend class GView;

    friend GBOOL GInitFormClass(GVOID * pvArg); //\\ JK, 這個才對吧
    friend GVOID GDeinitViewClass(GVOID);
    friend GVOID GSetGUIKitCooperativeMode(GUINT8 uCoopMode);
    friend GBOOL GBroadcastEvent(GUINT32 u4Evt, GUINT32 u4Param1, 
                                 GUINT32 u4Param2);
    friend GWidget *GTouchHitTest(GPOINT_T *prPt);
};


GForm *GCreateForm(const GFORMTEMPLATE_T *prTemplate);

/**@}*/

#endif //_GFORM_H_