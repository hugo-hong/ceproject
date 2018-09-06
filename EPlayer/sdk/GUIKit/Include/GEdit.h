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
 *   GEdit.h 
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
 *   mcn05008
 *
 * Last changed:
 * ------------- 
 * $Author: zeng.zhang $ 
 *
 * $Modtime: $  
 *
 * $Revision: #1 $
****************************************************************************/

#ifndef _GEDIT_H
#define _GEDIT_H

#include "GGdi.h"
#include "GWidget.h"
#include "GStyle.h"


#define ET_MAX_TXT_LEN   100
#define GES_LEFT         0x00000001
#define GES_CENTER       0x00000002
#define GES_RIGHT        0x00000004
#define GES_READONLY     0x00000008
#define GES_MAGNIFIER    0x00000010
#define GES_MULTILINE    0x00000020

typedef struct _GEDITSPECDATA
{  
    GStringData  *szText;        
#if CFG_GUIKIT_NEW_STYLE
    GEDITSTYLE_T *prStyle;
#endif
} GEDITSPECDATA_T;

class GEdit : public GWidget
{
public:    
    GVOID SetStyle(GEDITSTYLE_T *prStyle);
    GVOID GetStyle(GEDITSTYLE_T *prStyle);
    GVOID GetText(GTCHAR *szText);
    GVOID SetText(GTCHAR *szText);
    GVOID SetText(GStringData *szText);
    GVOID SetText(GUINT32  u4StrID);
    GEdit(GVOID);
    GEdit(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED | WF_TABSTOP,
              GUINT32 u4ID = 0xFFFFFFFF);
    virtual ~GEdit(GVOID);
    GBOOL   GetModify(GVOID);
    GVOID   SetModify(GBOOL fgModify = TRUE);
    GUINT32 GetLimitText(GVOID) const;
    GVOID   SetLimitText(GUINT32 u4Max);
    GINT32  GetCurEditPos(GVOID);
    GBOOL   SetCurEditPos(GINT32 i4NewPos);
    GBOOL   InsertText(GINT32 i4Pos, GTCHAR *szToInsTxt);
    GBOOL   InsertChar(GINT32 i4Pos, GTCHAR tc);
    GBOOL   ReplaceChar(GINT32 i4Pos, GTCHAR tc);          
    GVOID   DeleteText(GINT32 i4Start, GINT32 i4End);
    GVOID   SetMagnifierFont(GUINT32 u4Font);
    GVOID   SetMagnifierIcon(GUINT16 u2IconID);

    enum
    {
        CURSOR_UNSHOWN    = 0,       // 0: do not show cursor
        CURSOR_SHOWFOCUS  = 1,       // 1: show cursor only when focused
        CURSOR_SHOWALWAYS = 2        // 2: show cursor always
    };

    GVOID   TurnOnCursor(GBOOL fgForceOn, GUINT32 u2CursorMode = CURSOR_SHOWALWAYS);
    GVOID   SetReadOnly(GBOOL fgReadOnly = TRUE);
    GVOID *operator new(size_t u4Size);
    GVOID  operator delete(GVOID *pvObj); 

    inline GVOID SetCursorColor(GUINT32 u4Color) 
    { 
        m_u4CursorColor = u4Color;
    }

    static GWidget *CreateInstance(GVOID *pvSpecData);

protected:
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    GBOOL KeyDown( GUINT32 u4Param1, GUINT32 u4Param2);
    GBOOL TouchDown(GUINT32 u4Param1, GUINT32 u4Param2);
    GBOOL TouchUp(GUINT32 u4Param1, GUINT32 u4Param2);
    GBOOL InternalSetCurEditPos(GHANDLE hGC, GINT32 i4NewPos);

    GVOID Paint(GHANDLE hGC);

    GBOOL ShowMagnifier(GUINT32 u4X, GUINT32 u4Y, GBOOL fgShow);
    GBOOL MoveMagnifier(GUINT32 u4X, GUINT32 u4Y);

    HSURFACE  m_hSurf; 
    GBOOL     m_fgMgfShown;
    GRECT_T   m_rOldRect;
    GUINT32   m_u4MgfFont;
    GUINT16   m_u2MgfIconID;

private:
    GINT16 GetStartPos(GVOID);
    GVOID  StopCursorTimer(GVOID);
    GVOID  StartCursorTimer(GVOID);
    GVOID  DrawMultiLineString(GHANDLE hGC);
    friend GVOID  CursorTimer(GVOID *pvParam);
    
    GBOOL   m_fgTimerStart;
    GUINT32 m_u4LimitText;
    GBOOL   m_fgModify;
    GINT32  m_i4CurEditPos;    
    GINT32  m_i4TextStarX;
    GINT32  m_i4StartIndx;
    GBOOL   m_fgErase;
    GTCHAR  m_szText[ET_MAX_TXT_LEN + 1];
    
    GEDITSTYLE_T *m_prStyle;
    GTIMER_T  m_rTimer;   
    GBOOL     m_fgCurAlways;

    GUINT16   m_u2ShowCurMode;   
    GUINT32   m_u4CursorColor;
    GRECT_T   m_rCursorRect;
};



#endif

