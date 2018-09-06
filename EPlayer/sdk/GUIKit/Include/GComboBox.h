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
 *   GComboBox.h 
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

#ifndef _GCOMBOBOX_H
#define _GCOMBOBOX_H

#include "GWidget.h"
#include "GGdi.h"
#include "GViewCtrl.h"
#include "GForm.h"
#include "GStringData.h"

#define GCB_ERR         (-1)
#define GCB_MAX_ITEM_CNT 100


#define CBF_NO_DROP_DOWN   0x0001
#define CBF_HOR_ARROW      0x0002
#define CBF_VER_ARROW      0x0004


typedef struct _COMBOBOXSPECITEM
{
    GStringData *szText;
    GUINT32   u4Data;
}GCOMBOBOXSPECITEM_T;

typedef struct _GCOMBOBOXSPECDATA
{
    GUINT16               u2ItemNum;
    GCOMBOBOXSPECITEM_T *prItem;
#if CFG_GUIKIT_NEW_STYLE
    GCOMBOBOXSTYLE_T    *prStyle;
#endif
} GCOMBOBOXSPECDATA_T;


class GComboBox: public GWidget
{
public:
    GVOID SetStyle(GCOMBOBOXSTYLE_T *prStyle);
    GVOID GetStyle(GCOMBOBOXSTYLE_T *prStyle);

    GBOOL TouchDown(GUINT32 u4Param1, GUINT32 u4Param2);
    GComboBox(GUINT32 u4Flags = 0);
    GComboBox(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED | WF_TABSTOP,
              GUINT32 u4ID = 0xFFFFFFFF);
    virtual ~GComboBox();

    GINT32 GetCount(GVOID) const;
    GINT32 GetCurSel(GVOID) const;
    GINT32 SetCurSel(GINT32 i4Sel);    
    GINT32 SetItemData(GINT32 i4Index, GUINT32 u4ItemData);
    GUINT32 GetItemData(GINT32 i4Index) const;
    GINT32 GetTopIndex(GVOID) const;
    GINT32 SetTopIndex(GINT32 i4Index);
    GINT32 GetLBText(GINT32 i4Index, GStringData *szText) const;
    GINT32 GetLBTextLen(GINT32 i4Index) ;
    GINT32 AddString(GStringData *szStr);
    GINT32 AddString(GTCHAR *szStr);
    GINT32 AddString(GUINT32 u4StrID);
    
    GINT32 DeleteString(GINT32 i4Index);
    GINT32 InsertString(GINT32 i4Index, GStringData *szStr);
    GVOID  ResetContent(GVOID);
    GINT32 FindString(GINT32 i4StartAfter, GStringData *szStr);  
    GVOID *operator new(size_t u4Size);
    GVOID  operator delete(GVOID *pvObj); 
    
    static GWidget *CreateInstance(GVOID *pvSpecData);
    
protected:
    GBOOL KeyDown(GUINT32 u4Param1, GUINT32 u4Param2);
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    GVOID Paint(GHANDLE hGC);

private:
    GVOID DrawArrow(GBOOL fgErase);
    GINT32             m_i4CurSel;
    GUINT32            m_u4ItemCnt;
    GINT32             m_i4TopIndex;
    GUINT32            m_u4MaxItemCnt;
    GCOMBOBOXSTYLE_T *m_prStyle;
    GLView            m_rLView;
    
    struct COMBOBOXITEM_T : public GObject
    {
        GStringData   szText;
        GUINT32    u4Data;
    };  

    COMBOBOXITEM_T m_arItems[GCB_MAX_ITEM_CNT];
};

#endif