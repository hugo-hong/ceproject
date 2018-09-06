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
 *   GPopupMenu.h
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

#ifndef _GPOPUPMENU_H
#define _GPOPUPMENU_H

#include "GWidget.h"
#include "GGdi.h"
#include "GStyle.h"

/* Define Flags*/

#define PMF_SUBMENU       0x0001

/* Menu Item Type */
#define PMF_SIMPLE       0X0001
#define PMF_TOGGLE       0x0002
#define PMF_POPUP        0X0004
#define PMF_SEPARATOR    0x0008

#define PMF_BYCOMMAND    0x0010
#define PMF_BYPOSITION   0x0020
#define PM_MAX_ITEM_CNT  20

enum
{
    TRACK_ALIGN_TOPLEFT,
    TRACK_ALIGN_BOTTOMLEFT
};

class GPopupMenu;

typedef struct _MENUITEMEXTPL
{
    GUCHAR    ucType;
    GStringData *szText;
    GUINT32   u4ID;
    GUINT32   u4IconID;
} GMENUITEMTPL_T;

typedef struct _MENUITEMTPL
{
    GUCHAR    ucType;
    GUINT32   u4StrID;
    GUINT32   u4ID;
    GUINT32   u4IconID;
} GMENUITEMTPL_ID_T;


typedef struct _GPMTEMPLATE
{
    GUCHAR           ucType;
    GUCHAR           ucMenuItemCnt;
    GMENUITEMTPL_ID_T *prMenuItem;  
} GPMTEMPLATE_ID_T;



typedef struct _GPMTEMPLATE_EX
{
    GUCHAR           ucType;
    GUCHAR           ucMenuItemCnt;
    GMENUITEMTPL_T *prMenuItem;  
} GPMTEMPLATE_T;


struct GMENUITEMINFO_T : public GObject
{
  GSIZE_T     rItemSize;
  GUINT32      u4Mask; 
  GUINT32      u4Flags; 
  GUINT32      u4State; 
  GBOOL        fgToggle;
  GUINT32      u4ID; 
  GUINT32      u4IconID;
  GStringData     szText;
  GPopupMenu *prSubMenu; 
}; 

typedef struct _DRAWITEM
{    
    GUINT32    u4State;    
    GUINT32    u4Flags;
    GBOOL      fgToggle;
    GHANDLE    hGC;
    GRECT_T   rRect;
    GUINT32    u4Data;
    GUINT32    u4IconID;
    GStringData   *szText;
} DRAWITEM_T, *PDRAWITEM_T;

class GPopupMenu : public GWidget
{
public:    
    GVOID SetStyle(GPOPUPMENUSTYLE_T *prStyle);
    GVOID GetStyle(GPOPUPMENUSTYLE_T *prStyle);
    
    GVOID Hide();
    GPopupMenu(GUINT32 u4Flags = 0);
    virtual ~GPopupMenu(GVOID);
    GBOOL   AppendMenu(GUINT32 u4Flags, GUINT32 u4NewItemID = 0, 
                      GTCHAR *szNewItemText = NULL, GUINT32 u4IconID = 0);
    GBOOL   AppendMenu(GUINT32 u4Flags, GUINT32 u4NewItemID, 
                      GUINT32 u4StrNewItemText, GUINT32 u4IconID);
    GBOOL   AppendMenu(GUINT32 u4Flags, GUINT32 u4NewItemID, 
                      GStringData *szNewItemText, GUINT32 u4IconID);
    GBOOL   InsertMenu(GUINT32 u4Pos, GUINT32 u4Flags, GUINT32 u4NewItemID = 0, 
                      GTCHAR *szNewItemText = NULL, GUINT32 u4IconID = 0);
    GBOOL   RemoveMenu(GUINT32 u4Pos, GUINT32 u4Flag = PMF_BYPOSITION);
    GBOOL   ModifyMenu(GUINT32 u4Pos, GTCHAR *szMenuItemText);
    GBOOL   ModifyMenu(GUINT32 u4Pos, GUINT32 u4MenuItemTextID);
    GBOOL   ModifyMenu(GUINT32 u4Pos, GStringData *szMenuItemText);
    
    GBOOL   EnableMenuItem(GUINT32 u4Pos, GBOOL fgEnable);
    GINT32  GetMenuString(GUINT32 u4Pos, GTCHAR *szMenuItemText, GUINT32 u4MaxLen);
    GINT32  GetMenuString(GUINT32 u4Pos, GStringData *szText);
    GBOOL   SetMenuItemBitmaps(GUINT32 u4Pos, GUINT32 aBmpID[2]);
    GBOOL   SetDefaultItem(GUINT32 u2Item);
    GUINT32 GetDefaultItem(GVOID) const;
    GUINT32 GetMenuItemCount(GVOID) const;
    GVOID   SetMenuItemDim(GUINT16 u2Width, GUINT16 u2Height);
    GVOID   Track(GINT32 x, GINT32 y, GWidget *prWgt = NULL);
    GVOID   Track(GINT32 i4Align, GINT32 x, GINT32 y, GWidget *prWgt = NULL);    
    GVOID  *operator new(size_t u4Size);
    GVOID   operator delete(GVOID *pvObj);
    GVOID   RegCallback(PFN_NORMALCALLBACK pfnCb);
    GVOID   RegCallback(GObject *obj, PFN_MEMBERCALLBACK pfnCb);
    
protected:
    GBOOL TouchMove(GUINT32 u4Param1, GUINT32 u4Param2);
    GBOOL TouchDown(GUINT32 u4Param1, GUINT32 u4Param2);
    GBOOL TouchUp(GUINT32 u4Param1, GUINT32 u4Param2);
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    GVOID DrawItem(PDRAWITEM_T prDrawItem);    
    GBOOL KeyDown(GUINT32 u4Param1, GUINT32 u4Param2);
    GVOID Paint(GHANDLE hGC);

private:
    GVOID SetMenuItemInfo(GMENUITEMINFO_T *prMenuItemInfo, 
                         GUINT32           u4Flags,
                         GUINT32           u4NewItemID,
                         GTCHAR           *szNewItemText,  
                         GUINT32           u4IconID);

    GVOID SetMenuItemInfo(GMENUITEMINFO_T *prMenuItemInfo, 
                         GUINT32           u4Flags,
                         GUINT32           u4NewItemID,
                         GUINT32           u4StrNewItem,  
                         GUINT32           u4IconID);

    GVOID SetDrawItemInfo(PDRAWITEM_T      prDrawItem,
                         GMENUITEMINFO_T *prMenuItem);
    GVOID ChangeFocusedItem(GINT32 i4NewFItem);
    GVOID ChangeFocus(GBOOL fgDesc);
    GINT32 HitTestItem(GUINT32 u4Param);    

    GINT32                 m_i4DefaultItem;
    GMENUITEMINFO_T       m_arItemInfo[PM_MAX_ITEM_CNT];
    GUINT32                m_u4ItemCnt;
    GUINT32                m_u4MaxItemCnt;
    GWidget              *m_prOwer;
    GINT32                 m_i4FocusedItem;
    GUINT16                m_u2MenuItemWidth;
    GUINT16                m_u2MenuItemHeight;
    GPOPUPMENUSTYLE_T    *m_prStyle;
    enum
    {
        PMIS_NORNAL  = 0x00,
        PMIS_FOCUSED = 0x01,
        PMIS_GRAYED  = 0x02,        
        PMIS_TOGGLED = 0x10
    };
};

GPopupMenu *GCreatePMFromTPL(const GPMTEMPLATE_T *prTemplate);
GPopupMenu *GCreatePMFromTPL(const GPMTEMPLATE_ID_T *prTemplate);

#endif