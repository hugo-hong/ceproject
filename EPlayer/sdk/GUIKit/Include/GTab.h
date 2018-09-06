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
 *   GTab.h
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


#ifndef _GTAB_H_
#define _GTAB_H_


#include "GWidget.h"
#include "GTabPage.h"
#include "GStyle.h"

#define MAXTABPAGE  10

class GTab : public GWidget
{
public:
    GTab(GVOID);
    GTab(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED | WF_TABSTOP | WF_NOBKGND ,
         GUINT32 u4ID = 0xFFFFFFFF);
    virtual ~GTab();

#if CFG_GUIKIT_NEW_STYLE
    GVOID SetStyle(GTABSTYLE_T *prStyle);
    GVOID GetStyle(GTABSTYLE_T *prStyle);
#endif
    GVOID Add(GTabPage *pTabPage);
    GVOID ReMove(GTabPage *pTabPage); 
    GTabPage *GetTabPageByIndex(GINT16 i2Index);
    GTabPage *GetTabPage(GTCHAR *szLable);
    GTabPage *GetTabPage(GUINT16 u2StrID);
    virtual GWidget *TouchHitTest(GPOINT_T *prPt);
    

    GVOID *operator new(size_t u4Size);
    GVOID operator delete(GVOID *pvObj);
    
    static GWidget *CreateInstance(GVOID *pvSpecData);

protected:
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    GVOID Paint(GHANDLE hGC);
    virtual GVOID InternalRedraw(GRECT_T *prUpdate, GUINT32 u4Flags);
    virtual GVOID InternalUpdate(GBOOL fgErase);

private:
    GTabPage *m_pArray[MAXTABPAGE];
    GINT16     m_i2Focus;
    GINT16     m_i2Count;
    GINT16     m_i2LastFocus;
    GINT16     i2Offset;

    GTABSTYLE_T *m_prStyle;

    GBOOL CallBack(GCmdSource *pobjSender,
                    GUINT32      u4Event, 
                    GUINT32      u4Param1,
                    GUINT32      u4Param2);
    GVOID ResizeImpl();
    GVOID GTab::PaintLable(GHANDLE hGC, GINT16 x1, GINT16 y1, GINT16 x2, 
                GINT16 y2, GINT16 i2LableHeight, GStringData *szLable, GBOOL fgSelected);
    GBOOL IsScroll(GHANDLE hGC);
    GVOID PaintArrow(GHANDLE hGC, GINT16 x1, GINT16 y1, GINT16 x2,
                GINT16 y2, GBOOL fgdirection, GBOOL fgDisable);
    GINT16 GetLableStartX(GHANDLE hGC, GINT16 i2Index);
    GINT16 GTab::GetLableEndX(GHANDLE hGC, GINT16 i2Index);
    GINT16 GTab::GetLableWidth(GHANDLE hGC, GINT16 i2Index);
    GBOOL GTab::IsLeft();
    GVOID GTab::ChangeTabPage(GBOOL fgdirection);
    GUINT32 SelectTextColor(GBOOL fgSelected);
    GUINT32 SelectFont(GBOOL fgSelected);
};

typedef struct
{
    GUINT16              u2ItemNum;
    GTABPAGESPECDATA_T *prTabPageTemplate;
#if CFG_GUIKIT_NEW_STYLE
    GTABSTYLE_T        *prStyle;
#endif
} GTABSPECDATA_T;
#endif //_GTAB_H_