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
 *   GScrollBar.h
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


#ifndef _GSCROLLBAR_H_
#define _GSCROLLBAR_H_


#include "GWidget.h"
#include "GGdi.h"
#include "GStyle.h"


typedef struct _SCROLLBARSPECDATA
{
    GBOOL            m_fgOrientation;
#if CFG_GUIKIT_NEW_STYLE
    GSCROLLSTYLE_T *prStyle;
#endif
} GSCROLLBARSPECDATA_T;


class GScrollBar : public GWidget
{
public:
    GScrollBar(GVOID);
    GScrollBar(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED | WF_TABSTOP,
               GUINT32 u4ID = -1);
    virtual ~GScrollBar();

#if CFG_GUIKIT_NEW_STYLE
    GVOID SetStyle(GSCROLLSTYLE_T *prStyle);
    GVOID GetStyle(GSCROLLSTYLE_T *prStyle);
#endif
    GVOID SetMinimum(GINT16 i2Minimum);
    GVOID SetMaximum(GINT16 i2Maximum);
    GVOID SetExtent(GINT16 i2Extent);
    GVOID SetCurValue(GINT16 i2CurValue);
    GVOID SetBlockIncrement(GINT16 i2BlockIncrement);
    GVOID SetUnitIncrement(GINT16 i2UnitIncrement);
    GVOID SetOrientation(GBOOL fgOrientation);

    GINT16 GetMinimum();
    GINT16 GetMaximum();
    GINT16 GetExtent();
    GINT16 GetCurValue();
    GINT16 GetBlockIncrement();
    GINT16 GetUnitIncrement();

    GVOID   SetUpArrowDisableIcon(GUINT16 u2Icon);
    GVOID   SetDownArrowDisableIcon(GUINT16 u2Icon);

    enum
    {
        WF_HORIZONTAL = 1 << 0
    };

    GVOID *operator new(size_t u4Size);
    GVOID operator delete(GVOID *pvObj);
    
    static GWidget *CreateInstance(GVOID *pvSpecData);

protected:
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    GVOID Paint(GHANDLE hGC);

private:
    enum
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    enum
    {
        WIF_PRESSED = 1 << 0
    };

    GINT16       m_i2Minimum;
    GINT16       m_i2Maximum;
    GINT16       m_i2Extent;
    GINT16       m_i2CurValue;
    GINT16       m_i2BlockIncrement;
    GINT16       m_i2UnitIncrement;
    GINT16       m_i2LastX;
    GINT16       m_i2LastY;
    GBOOL        m_fgState;
    GBOOL        m_fgStateUL;
    GBOOL        m_fgStateDR;
    GSCROLLSTYLE_T *m_prStyle;
    GBOOL        m_fgUpDisable;
    GBOOL        m_fgDownDisable;
    GINT32       m_i4UpDownCnt; //Mtk40006, 2009-3-10, vScrollbar Pageup. 

    GUINT16     m_u2UpArrowDisableIcon;
    GUINT16     m_u2DownArrowDisableIcon;

    GVOID Event(GBOOL fgAdd, GINT16 i2Event);
    GVOID PaintArrow(GHANDLE hGC, GINT16 x1, GINT16 y1, GINT16 x2, GINT16 y2, 
                    GUCHAR ucDirection, GPEN_T *prBlack,
                    GBRUSH_T *prBlackBrush, GBRUSH_T *prDisableBrush);
    GVOID PaintArrowEx(GHANDLE hGC, GSCROLLSTYLE_T *prStyle, GRECT_T *prRect,
                      GUCHAR ucState, GUCHAR ucDirection);
    GVOID PaintBorder(GHANDLE hGC, GINT16 i2Width, GINT16 i2Height,
                     GPEN_T *prDark, GPEN_T *prLight);
    GVOID PaintKnob(GHANDLE hGC, GSCROLLSTYLE_T *prStyle, GRECT_T *prRect,
                   GUCHAR ucState);
    
    GVOID SetUpIconState(GHANDLE hGC, GUINT32 u4State);
    GVOID SetDownIconState(GHANDLE hGC, GUINT32 u4State);

    friend class GLView;
    friend class GMView;
};

#endif //_GSCROLLBAR_H_