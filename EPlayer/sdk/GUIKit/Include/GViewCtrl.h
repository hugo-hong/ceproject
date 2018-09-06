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
 *   GViewCtrl.h
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


#ifndef _GVIEWCTRL_H_
#define _GVIEWCTRL_H_

#include "GStyle.h"
#include "GWidget.h"
#include "GScrollBar.h"
/**
* @defgroup groupViewCtrl Base View Control
* @ingroup groupWidget
* @{
*/

#define BT_SPECIAL_ALIGN

class GViewCtrl;

/**view control history buffer size*/
#define VIEWCTRL_HISTORY_SZ  8
/**store the view control history*/
typedef struct _GVIEWCTRLHISTORY
{
    GUINT32 pbBuffer[(VIEWCTRL_HISTORY_SZ + 3)/4];/**< view control history buffer*/

} GVIEWCTRLHISTORY_T;

/**Enumerable value for View Control Item state. bbbbb*/
enum 
{
    VCIST_FOCUSED  = 1 << 0,/**< focused state*/
    VCIST_SELECTED = 1 << 1,/**< selected state*/
    VCIST_DISABLED = 1 << 7 /**< disabled state*/
};


enum
{
    DRAWVCI_BKG = 1 << 0,
    DRAWVCI_ICON = 1 << 1,
    DRAWVCI_TEXT = 1 << 2
};




#define MASK_VIEWCTRLITEM_STATE          0x00000001L/**< enable symbol ucState mask*/
#define MASK_VIEWCTRLITEM_ICON_N         0x00000010L/**< enable symbol u2IconN mask*/
#define MASK_VIEWCTRLITEM_ICON_F         0x00000020L/**< enable symbol u2IconF mask*/
#define MASK_VIEWCTRLITEM_ICON_S         0x00000040L/**< enable symbol u2IconS mask*/
#define MASK_VIEWCTRLITEM_ICON_D         0x00000080L/**< enable symbol u2IconD mask*/
#define MASK_VIEWCTRLITEM_TEXT           0x00000100L/**< enable symbol pstrText mask*/
#define MASK_VIEWCTRLITEM_DATA           0x00000200L/**< enable symbol u4Data mask*/
#define MASK_VIEWCTRLITEM_ICON_P         0x00000400L/**< enable symbol u4IconP mask*/


#define INVALID_VIEWCTRLITEM_DATA       ((GUINT32)(-1))


typedef struct _GVIEWCTRLITEM
{
    /**
    * Mask for item 
    * @see   MASK_VIEWCTRLITEM_STATE
    *        MASK_VIEWCTRLITEM_ICON_N
    *        MASK_VIEWCTRLITEM_ICON_F
    *        MASK_VIEWCTRLITEM_ICON_S
    *        MASK_VIEWCTRLITEM_ICON_D
    *        MASK_VIEWCTRLITEM_TEXT
    *        MASK_VIEWCTRLITEM_DATA
    */
    GUINT32 u4Mask; /**< Mask for item */
    /**
    * item's state 
    * @see   VCIST_FOCUSED
    *        VCIST_SELECTED
    *        VCIST_DISABLED
    */
    GUCHAR  ucState; /**< item's state */
    GUINT32 u2IconN; /**< item normal icon ID*/
    GUINT32 u2IconP; /**< item pressed icon ID*/
    GUINT32 u2IconF; /**< item focused icon ID*/
    GUINT32 u2IconS; /**< item selected icon ID*/
    GUINT32 u2IconD; /**< item disabled icon ID*/
    GStringData *pstrText; /**< Menu item text string */
    GUINT32 u4Data; /**< custom data for item*/
} GVIEWCTRLITEM_T;



#define MASK_VIEWCTRLDCE_GETITEM       0x00000001L/**< enable callback getitem*/
#define MASK_VIEWCTRLDCE_GETCHUNK      0x00000002L/**< enable callback getchunk*/
#define MASK_VIEWCTRLDCE_INSERTITEM    0x00000004L/**< enable callback insertitem*/
#define MASK_VIEWCTRLDCE_SETITEM       0x00000008L/**< enable callback setitem*/
#define MASK_VIEWCTRLDCE_SETITEMDATA   0x00000010L/**< enable callback setitemdata*/
#define MASK_VIEWCTRLDCE_GETITEMDATA   0x00000020L/**< enable callback getitemdata*/
#define MASK_VIEWCTRLDCE_REMOVEITEM    0x00000040L/**< enable callback removeitem*/
#define MASK_VIEWCTRLDCE_LOADITEMNUM    0x00000080L /**< dynamic load item num */




/**< Get item info. function pointer type*/
typedef GBOOL   (*PFN_GETITEM)(GViewCtrl *pViewCtrl, GINT32 i4Index,
                              GVIEWCTRLITEM_T *prItem);
/**< get a lot of items info. function pointer type*/
typedef GBOOL   (*PFN_GETCHUNK)(GViewCtrl *pViewCtrl, GINT32 i4Start,
                               GUINT32 u4Count, GVIEWCTRLITEM_T **pprChunk);
/**< set item info. function pointer type*/
typedef GBOOL   (*PFN_SETITEM)(GViewCtrl *pViewCtrl, GINT32 i4Index,
                              GVIEWCTRLITEM_T *prItem);
/**< get item data symbol function pointer type*/
typedef GUINT32 (*PFN_GETITEMDATA)(GViewCtrl *pViewCtrl, GINT32 i4Index);
/**< set item data symbol function pointer type*/
typedef GBOOL   (*PFN_SETITEMDATA)(GViewCtrl *pViewCtrl, GINT32 i4Index,
                                  GUINT32 u4Data);
/**< inset item function pointer type*/
typedef GBOOL   (*PFN_INSERTITEM)(GViewCtrl *pViewCtrl, GINT32 i4Index,
                                 GVIEWCTRLITEM_T *prItems);
/**< remove item function pointer type*/
typedef GBOOL   (*PFN_REMOVEITEM)(GViewCtrl *pViewCtrl, GINT32 i4Index);

/** set call back function prototype structure */
typedef struct _GVIEWCTRLDCE
{
    GUINT32 u4MaxItems;

    GUINT32     u4LoadItemNum;

    PFN_GETITEM        pfnGetItem; /**< Get menu item handler */
    PFN_GETCHUNK       pfnGetChunk; /**< Get menu item chunk handler */
    PFN_INSERTITEM     pfnInsertItem; /**< Insert menu item handler */

    PFN_SETITEM        pfnSetItem; /**< Set menu item handler */
    PFN_GETITEMDATA    pfnGetItemData; /**< Get menu item data handler */
    PFN_SETITEMDATA    pfnSetItemData; /**< Set menu item data handler */
    
    PFN_REMOVEITEM     pfnRemoveItem; /**< Remove menu item handler */
} GVIEWCTRLDCE_T;


typedef struct _DRAWVIEWCTRLITEM
{
    GUINT32   u4DrawSet;
    GINT32    i4Index;
    GRECT_T *prRect;
    GBOOL     fgErase;
    GBOOL     fgPressed;
	GBOOL     fgMoved;
} GDRAWVIEWCTRLITEM_T;

/**the view control class*/
class GViewCtrl : public GWidget
{
public:
    GViewCtrl(GVOID);
    /** 
    *  destruct function
    *
    *  @param  [in] prRect  the view rectangle
    *
    *  @param  [in] u4WFlags the view type
    *
    *  @param  [in] u4ID  the view ID
    *
    *  @see   WF_DYNAMIC
    *         WF_LOOP
    *         WF_VSCROLL
    *         WF_HSCROLL
    *         WF_AUTOVSCROLL
    *         WF_AUTOHSCROLL
    *         WF_VWRAP
    *         WF_TWOSTATEITEM
    */
    GViewCtrl(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED | WF_TABSTOP, 
              GUINT32 u4ID = 0xFFFFFFFF);
    virtual ~GViewCtrl(GVOID);

    /** 
    *  set item memory space
    *
    *  @param  [in] pbBuf  the memory handle.if pbBuf==NULL,memory will dynamic create
    *
    *  @param  [in] pu4MemSize  the memory size,if return false, it will return the right size
    *
    *  @param  [in] u4MaxItems  the max item value
    *
    *  @return  true or false
    */
    GBOOL SetVCIChunk(GUCHAR *pbBuf, GUINT32 *pu4MemSize, GUINT32 u4MaxItems);

    /** 
    *  init function
    *
    *  @param  [in] u4Count  the init count item
    *
    *  @param  [in] prHistory  the history handle
    *
    *  @return  true or false
    */
    GBOOL Init(GUINT32 u4Count, GVIEWCTRLHISTORY_T *prHistory);

    /** 
    *  save the view control history
    *
    *  @param  [in] prHistory  history handle
    *
    *  @return  NONE
    */
    GVOID SaveHistory(GVIEWCTRLHISTORY_T *prHistory);

    /** 
    *  restore the view control history
    *
    *  @param  [in] prHistory  history handle
    *
    *  @return  NONE
    */
    GVOID RestoreHistory(GVIEWCTRLHISTORY_T *prHistory);

    /** 
    *  add item after the view last item
    *
    *  @param  [in] prItem  item parameter handle
    *
    *  @return  true or false
    */
    GBOOL AppendItem(GVIEWCTRLITEM_T *prItem);

    /** 
    *  insert item 
    *
    *  @param  [in] i4Index  the item position index
    *
    *  @param  [in] prItem  the item parameter handle
    *
    *  @return  true or false
    */
    GBOOL InsertItem(GINT32 i4Index, GVIEWCTRLITEM_T *prItem);

    /** 
    *  change the specify item info.
    *
    *  @param  [in] i4Index  the item position index
    *
    *  @param  [in] prItem  the item parameter handle
    *
    *  @return  true or false
    */
    GBOOL SetItem(GINT32 i4Index, GVIEWCTRLITEM_T *prItem);

    /** 
    *  get the specify item info.
    *
    *  @param  [in] i4Index  the item position index
    *
    *  @param  [in] prItem  the item parameter handle
    *
    *  @return  true or false
    */
    GBOOL GetItem(GINT32 i4Index, GVIEWCTRLITEM_T *prItem);

    /** 
    *  set the custom data
    *
    *  @param  [in] i4Index  the item position index
    *
    *  @param  [in] u4Data  data value
    *
    *  @return  true or false
    */
    GBOOL SetItemData(GINT32 i4Index, GUINT32 u4Data);

    /** 
    *  get the custom data
    *
    *  @param  [in] i4Index  the item position index
    *
    *  @return  the custom data value
    */
    GUINT32 GetItemData(GINT32 i4Index);

    /** 
    *  Set the item focus state
    *
    *  @param  [in] i4Index  the item position index
    *
    *  @return  True or False
    */
    GBOOL SetItemFocus(GINT32 i4Index);
    GBOOL ClrItemFocus(GINT32 i4Index);

    /** 
    *  Set the item selected state
    *
    *  @param  [in] i4Index  the item position index
    *
    *  @return  True or False
    */
    GBOOL SetItemSelected(GINT32 i4Index);

	/** 
	*  Clear the item selected state
	*
	*  @param  [in] i4Index  the item position index
	*
	*  @return  True or False
	*/
	GBOOL ClrItemSelected(GINT32 i4Index);

    /** 
    *  get item count
    *
    *  @return  the item count value
    */
    GUINT32 GetCount(GVOID);

    /** 
    *  remove the specify item
    *
    *  @param  [in] i4Index  the item position index
    *
    *  @return  true or false
    */
    GBOOL RemoveItem(GINT32 i4Index);

    /** 
    *  remove all items
    *
    *  @return  true or false
    */
    GBOOL RemoveAllItems(GVOID);

    /** 
    *  get the focused item position
    *
    *  @return  the focused item position index value
    */
    inline GINT32 GetFocusedIndex(GVOID)
    { 
        return (m_i4Focused);
    }

    //Get the origin position index
    /** 
    *  Get the origin position index
    *
    *  @return  the origin position index value
    */
    inline GUINT32 GetOriginIndex(GVOID) 
    { 
        return (m_u4Origin); 
    }

    /** 
    *  set origin position need to redraw
    *
    *  @param  [in] i4Index  the item position index
    *
    *  @return  true or false
    */
    inline GBOOL SetOriginIndex(GINT32 i4Index)
    {
        if ((i4Index < 0) || (i4Index >= (GINT32)m_u4Count))
        {
            return (FALSE);
        }
        m_u4Origin = i4Index;

        return (TRUE);
    }


	/** 
	*  Get force get item
	*
	*  @return  fgForceGetItem
	*/
	inline GBOOL GetForceGetItem(GVOID) 
	{ 
		return (fgForceGetItem); 
	}

	/** 
	*  Set force get item
	*
    *  @return  true or false
	*/
	inline GBOOL SetForceGetItem(GBOOL fgForce)
	{
		fgForceGetItem = fgForce;

		return (TRUE);
	}

    /** 
    *  set callback function prototypes for Getting item info. and space, if view have too many items
    *
    *  @param  [in] u4Mask  the parameter type mask
    *
    *  @param  [in] prDCE   the function pointer structure handle
    *
    *  @return  NONE
    *
    *  @see     GVIEWCTRLDCE_T
    *           MASK_VIEWCTRLDCE_GETITEM
    *           MASK_VIEWCTRLDCE_GETCHUNK
    *           MASK_VIEWCTRLDCE_INSERTITEM
    *           MASK_VIEWCTRLDCE_SETITEM
    *           MASK_VIEWCTRLDCE_SETITEMDATA
    *           MASK_VIEWCTRLDCE_GETITEMDATA
    *           MASK_VIEWCTRLDCE_REMOVEITEM
    */

    GVOID  SetDCE(GUINT32 u4Mask, GVIEWCTRLDCE_T *prDCE);

    enum
    {
        WF_DYNAMIC = 1 << 0,/**< dynamic memory symbol*/
        WF_LOOP = 1 << 1,/**< support key  symbol*/
        WF_VSCROLL = 1 << 2,/**< vertical scroll symbol*/
        WF_HSCROLL = 1 << 3,/**< horizontal scroll symbol*/
        WF_AUTOVSCROLL = 1 << 4,/**< auto vertical scroll symbol*/
        WF_AUTOHSCROLL = 1 << 5,/**< auto horizontal scroll symbol*/
        WF_VWRAP = 1 << 6,/**< wrap for mv symbol*/
        WF_TWOSTATEITEM = 1 << 7, /**< only normal and focus state symbol*/
        WF_VSLIDE = 1 << 8,  /** Vertical Slide */
        WF_HSLIDE = 1 << 9,   /** Horizontal Slide */
        WF_AUTOSCROLLTEXT = 1 << 10,
        WF_PAGESCROLL = 1 << 11,
        WF_ITEMWITHALPHABKG = 1 << 12,
        WF_CHANGEFOCUSITEMWHENPAGESCROLL = 1 << 13
#ifdef BT_SPECIAL_ALIGN
		,WF_BT_SPECIAL_ALIGN = 1 << 14
#endif
    };

    GVOID   SetItemState(GINT32 i4Index, GUCHAR ucState);

	virtual GVOID RedrawItem(GUINT32 u4Index){}
protected:

    typedef struct _GVIEWCTRLDB
    {
        GINT32     i4HeadItem; /**< Menu item index of the dynamic buffer's head */
        GINT16     i2Head; /**< Head index of the dynmaic buffer */ 
        GINT16     i2Tail; /**< Tail index of the dynamic buffer */
        GUINT16    u2Count; /**< Menu item count in the dynamic buffer */
        
        GVIEWCTRLDCE_T rDCE; /**< Dynamic callback engine */
    } GVIEWCTRLDB_T;
    
    struct GVIEWCTRLITEMDATA_T : public GObject
    {
        GUINT32  u2IconN; /**< Menu item normal icon */
        GUINT32  u2IconP; /**< Menu item pressed icon */
        GUINT32  u2IconF; /**< Menu item focused icon */
        GUINT32  u2IconS; /**< Menu item focus icon */
        GUINT32  u2IconD; /**< Menu item disabled icon */
    
        GStringData strText; /**< Menu item text string */

        GUINT32 u4Data; /**< A 32-bit data associated with menu item */

		GBOOL   bSetStrText;
    };
    //handle event
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    //
    GUCHAR  GetItemState(GINT32 i4Index);
    GBOOL   ItemInDB(GINT32 i4Index);
    GUINT16 DBLoadChunk(GINT32 i4Start, GUINT16 u2Items);
    GVIEWCTRLITEMDATA_T *InternalGetItem(GINT32 i4Index);

    GUINT32 m_u4MaxItems;
    GUINT32 m_u4Origin;
    GINT32  m_i4Focused;
    GINT32  m_i4PressedItem;
    GUINT32 m_u4Count;
    GINT8    m_iTxtScrollID;
    GINT32   m_i4TxtScrollItem;
#if CFG_GUIKIT_NEW_STYLE
    GUINT32 m_u4PageNo;
#endif
    GVIEWCTRLDB_T       *m_prDB;
	GBOOL		fgForceGetItem; //force callback function, call pfnGetItem
    enum
    {
        VCIIST_NORMAL,
        VCIIST_SELECTED,
        VCIIST_DISABLED
    };

    enum
    {
        WIF_DYNAMIC = 1 << 0,
        WIF_VWRAP = 1 << 1,
        WIF_ALLOCMEM = 1 << 2,
        WIF_TWOSTATEITEM = 1 << 3
    };

    #define VIEWCTRLDB_HEADSIZE       sizeof(GVIEWCTRLDB_T)
    #define VIEWCTRLDB_MAX_ITEMS      32
    #define VIEWCTRLDB_MASK_ITEMS     (VIEWCTRLDB_MAX_ITEMS - 1)

private:
    GVIEWCTRLITEMDATA_T *DBGetItem(GINT32 i4Index);
    GBOOL   OnKeyUp(GUINT32 u4KeyCode);

    GVOID SinkItem(GVIEWCTRLITEMDATA_T *prInner, GVIEWCTRLITEM_T *prOuter);
    GVOID RetrieveItem(GVIEWCTRLITEM_T *prOuter, GVIEWCTRLITEMDATA_T *prInner);

    GVOID   InitDB(GVOID);

    GVOID   RemoveItemState(GINT32 i4Index);
    GINT32  GetItemIndex(GVIEWCTRLITEMDATA_T *prItem);

    GVOID   InternalInsertItem(GINT32 i4Index, GVIEWCTRLITEMDATA_T *prItem);
    GVOID   InternalInsertItemToDB(GINT32 i4Index, GVIEWCTRLITEMDATA_T *prItem);
    GVOID   InternalRemoveItem(GINT32 i4Index);
    GVOID   InternalRemoveItemFromDB(GINT32 i4Index);

    GUCHAR            *m_pbDataBuffer;
    GUCHAR            *m_pbStateData;
    GVIEWCTRLITEMDATA_T *m_prItems;
    GTCHAR           *m_aszTextBuf[VIEWCTRLDB_MAX_ITEMS];
};

/**@}*///groupViewControl

/**
* @defgroup groupLView List View Control
* @ingroup groupViewCtrl
* @{
*/
/**list view special data*/
typedef struct _GLVIEWSPECDATA
{
    GUINT32              u4ItemNum;    /**< item number */
    GUCHAR              *pbVIChunkBuf; /**< APP give the space buffer,if it is NULL,memory will dynamic create*/
    GUINT32             *pu4VIChunkSz; /**< APP give the space buffer size */
    GUINT32              u4MaxItems;   /**< max item number */
    GVIEWCTRLITEM_T    *prItem;   /**< item number */
#if CFG_GUIKIT_NEW_STYLE
    GLVSTYLE_T         *prStyle;
    GSCROLLSTYLE_T     *prScrollStyle;
#endif
} GLVIEWSPECDATA_T;

#ifdef LIST_VIEW_LYRIC_SUPPORT
typedef union _LYRIC_SCROLL_PARA_  
{
	struct
	{
		GINT8 iSildeMode;
		GINT8 iMaxItemCnt;
		GINT8 iOriginalItemIdx;
		GINT8 iIdxFocus;
	} LyricInitPara;
	GINT32 u4ScrollParam;
} LYRIC_SCROLL_PARA;
#endif

/**the list view class*/
class GLView : public GViewCtrl
{
public:
    GLView(GVOID);
    /** 
    *  destruct function
    *
    *  @param  [in] prRect  the view rectangle
    *
    *  @param  [in] u4WFlags the view type
    *
    *  @param  [in] u4ID  the view ID
    *
    *  @see   WF_DYNAMIC
    *         WF_LOOP
    *         WF_VSCROLL
    *         WF_HSCROLL
    *         WF_AUTOVSCROLL
    *         WF_AUTOHSCROLL
    *         WF_VWRAP
    *         WF_TWOSTATEITEM
    *         WF_CANFOCUSED
    *         WF_TABSTOP
    */
    GLView(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED | WF_TABSTOP, 
           GUINT32 u4ID = 0xFFFFFFFF);
    virtual ~GLView(GVOID);

    /** 
    *  set list view style
    *
    *  @param  [in] prStyle  the handle of style parameter
    *
    *  @return NONE
    *
    *  @see   GLVSTYLE_T
    */
    GVOID SetStyle(GLVSTYLE_T *prStyle);

    /** 
    *  get list view style
    *
    *  @param  [in] prStyle  the handle of style parameter
    *
    *  @return NONE
    *
    *  @see   GLVSTYLE_T
    */
    GVOID GetStyle(GLVSTYLE_T *prStyle);

    GVOID SetScrollStyle(GSCROLLSTYLE_T *prStyle);

    GVOID GetScrollStyle(GSCROLLSTYLE_T *prStyle);

    /** 
    *  get the rows number of a page
    *
    *  @return the rows number
    */
    GUINT16 GetRows(GVOID);

    GScrollBar *GetVBar(GVOID)
    {
        return (&m_vbar);
    }

	//mtk71372 2011.11.28 added ------------------------------------------------------------------------------------------ str
	
	//-------------------------------------------------------
	//Description:
	//	Get the item flags
	//-------------------------------------------------------
	GBOOL GetItemFlags(GUINT32 &u4Flags);
	
	//-------------------------------------------------------
	//Description:
	//	Set the item flags
	//-------------------------------------------------------
	GBOOL SetItemFlags(GUINT32 u4Flags);

	//-------------------------------------------------------
	//Description:
	//	Set the Icon for item base on the index
	//
	//Parameters
	//	u4Index : [in] The index of item
	//-------------------------------------------------------
	GBOOL SetItemIcon(GUINT32 u4Index,GUINT16 u2IconN,GUINT16 u2IconF,GUINT16 u2IconD,GUINT16 u2IconP,GUINT16 u2IconS);



	//mtk71372 2011.11.28 added ------------------------------------------------------------------------------------------ end
#ifdef BT_SPECIAL_ALIGN

	GVOID SetItemLeftTextWidth(GUINT32 iWidth);    //  设置 Item 左边 Text 的宽度，也就是左右两边的分割线

	GVOID SetItemLeftAliagn(GUINT32 iAliagn);

	GVOID SetItemRightAliagn(GUINT32 iAliagn);

#endif

    virtual GWidget *TouchHitTest(GPOINT_T *prPt);
    
    GVOID *operator new(size_t u4Size);
    GVOID operator delete(GVOID *pvObj);

	virtual GVOID RedrawItem(GUINT32 u4Index);

    static GWidget *CreateInstance(GVOID *pvSpecData);
    static GWidget *CreateInstanceEx(GRECT_T *prRect, GUINT32 u4WFlags,
                                     GUINT32 u4ID, GVOID *pvSpecData);
#ifdef CFG_GUIKIT_DRAGDROP_SUPPORTED
	enum
	{
		WF_FADE_IN_OUT_VERTICAL = ((GUINT32) 1 << 0),
		WF_SLIDE_HIGHLIGHT_ITEM = ((GUINT32) 1 << 1),
		WF_SLIDE_SPEED_CONTROL_BY_USER = ((GUINT32)1 << 2),
		WF_SLIDE_REBOUND_STOP = ((GUINT32)1 << 3)
	};
    GVOID SetSlideBg(GUINT32 u4IconID);
    GBOOL IsSlideStop(GVOID);
	GBOOL vSetSlideSpeed(GUINT32* u4TimerDuration, GUINT32 u4StepPixel, GUINT32 u4LyricCnt);
	GVOID vSetSlideFlag(GUINT32 u4Flag);
	GUINT32 vGetSlideFlag(GVOID) { return m_u4SlideFlag;}
#endif

#ifdef LIST_VIEW_LYRIC_SUPPORT
	enum
	{
		SCROLL_ITEM_INIT = 0x01,
		SCROLL_ITEM_STYLE_SET,
		SCROLL_ITEM_REFRESH,
		SCROLL_ITEM_STOP
	};
#endif

protected:
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    virtual GVOID InternalRedraw(GRECT_T *prUpdate, GUINT32 u4Flags);
    virtual GVOID InternalUpdate(GBOOL fgErase = TRUE);
    virtual GVOID UpdateClientRect(GVOID);

    virtual GVOID UpdateDataByWFlags(GUINT32 u4WFlags);

private:
    static GVOID ScrollTextTimerCb(GVOID *pvParam);
  
    GBOOL   IsShowVBar(GLVSTYLE_T *prStyle);
    GVOID   UpdateVBarRect(GLVSTYLE_T *prStyle);
    GVOID   UpdateVBarInfo(GLVSTYLE_T *prStyle);
    GVOID   RedrawVBar(GLVSTYLE_T *prStyle, GRECT_T *prRect, GBOOL fgErase);
    GBOOL   GotoItem(GINT32 i4Index);
    GBOOL   GotoPreviousItem(GVOID);
    GBOOL   GotoNextItem(GVOID);
    GBOOL   GotoPreviousPage(GVOID);
    GBOOL   GotoNextPage(GVOID);
    GVOID   Paint(GHANDLE hGC);
    GUINT16 GetMaxRows(GLVSTYLE_T *prStyle);
    GINT32  GetLastVisibleItem(GLVSTYLE_T *prStyle);
    GVOID   GetSpacing(GUINT16 *pu2XSpacing, GUINT16 *pu2YSpacing);
    GVOID   MeasureItem(GLVSTYLE_T *prStyle, GUINT16 *pu2ItemW, GUINT16 *pu2ItemH);
    GVOID   DrawItem(GHANDLE hGC, GINT32 i4Index, GRECT_T *prRect,
                    GLVSTYLE_T *prStyleCur, GBOOL fgErase = FALSE,
                    GBOOL fgPressed = FALSE, GBOOL fgPaint = FALSE,GBOOL fgMoved = FALSE);
    GVOID   UpdateItem(GHANDLE hGC, GINT32 i4Index, GRECT_T *prRect,
                      GLVSTYLE_T *prStyle, GUINT32 u4OldOrigin,
                      GBOOL fgErase = FALSE);
    GVOID   UpdateAllItems(GHANDLE hGC, GLVSTYLE_T *prStyle, GUINT32 u4OldOrign);
#if CFG_GUIKIT_NEW_STYLE
    GVOID   UpdatePageItems(GHANDLE hGC, GLVSTYLE_T *prStyle, GUINT32 u4PageNo);
#endif
    GBOOL   OnKeyDown(GUINT32 u4KeyCode);
    GBOOL   OnTouchMove(GUINT32 u4Param1, GUINT32 u4Param2);
    GBOOL   OnTouchUp(GUINT32 u4Param1, GUINT32 u4Param2);
	GBOOL   ClearPressedItem();
    GBOOL   OnTouchDown(GUINT32 u4Param1, GUINT32 u4Param2);
    GBOOL   OnTouchLeave(GVOID);
    GBOOL   OnTouchDB(GUINT32 u4Param1, GUINT32 u4Param2);
    GBOOL   OnHighlight(GINT32 i4Index);
    GBOOL   OnUnhighlight(GINT32 i4Index);
    GBOOL   OnVScroll(GWidget *pWidget, GUINT32 u4Event, GUINT32 u4Param1,
                     GUINT32 u4Param2);

#ifdef CFG_GUIKIT_DRAGDROP_SUPPORTED
    GBOOL   CanDoVSlide(GINT32 i4SlideMode, GUINT32 u4ItemCount);
    GBOOL   OnVSlide(GINT32 i4SlideMode, GUINT32 u4ItemCount);
    GVOID   OnVSlideStop(GBOOL fgRedraw = FALSE);
    static GVOID SlideTimerCb(GVOID *pvParam);
    GVOID   DeleteAllSurface(GVOID);

    GVOID   OnVMove(GINT32 i4SlideMode, GINT32 i4X, GINT32 i4Y);
    GVOID   OnVMove2Origin(GVOID);
    GVOID   OnVMoveUpdateOriginIndex(GVOID);
    GVOID   OnVMoveStop(GBOOL fgRedraw = FALSE);
    GBOOL   OnUnTouchDown(GUINT32 u4Param1, GUINT32 u4Param2);
    GVOID   ResetSlideParam(GVOID);
    GVOID   ShowVSlideBar(GUINT32 u42CurValue);
#ifdef LIST_VIEW_LYRIC_SUPPORT
	GVOID	ListViewLyricInit(GUINT8 uItemFocusIdx);
	GVOID	ListViewLyricEffect(HDC hSurface, GUINT32 u4YIncrement);
	GBOOL	ListViewLyricSlide(GUINT32 i4SlideMode, GINT8 u4LyricOrignal, GUINT8 u4ItemCount);
	GVOID	ListViewLyricScrollCommand(GUINT32 iScrollCmd, GUINT32 Param);
#endif
    GBOOL             m_fgTouchRelease;
    GBOOL             m_fgMoveIsCreated;
    GPOINT_T          m_rMoveOrigin;
#ifdef CFG_GUIKIT_DRAGDROP_AUTO_SCROLL_SUPPORTED
	#define	GLV_AUTO_SCROLL_TIME_THRESHOLD	1500
	GUINT32			  m_LVTickCnt;
#endif
    GBOOL             m_fgIsMoved;
    GINT32            m_i4ItemPixelOffset;
    GINT16            m_i2MoveYCnt;
    HSURFACE          m_hDragSurf;
    GHANDLE           m_hDragGC;

#ifdef	CFG_GUIKIT_DRAGDROP_SUPPORTED_SPEEDUP
	HSURFACE          m_hScreenImage;
#endif

    GINT32            m_i4SlideStepY;
    GUINT32           m_u4SurfH;
    GUINT32           m_u4SurfW;
    HSURFACE          m_hSlideSurf1;
    HSURFACE          m_hSlideSurf2;
    GHANDLE           m_hSurf1GC;
    GHANDLE           m_hSurf2GC;
    GTIMER_T          m_rSlideTimer;
    GBOOL             m_fgSlideTimerStart;
    GINT32            m_i4SlideMode;
    GUINT32           m_u4SlideItemMax;
	GUINT32			  m_u4SlideDrawItemMax;
    GUINT32           m_u4ItemCnt;
    GINT32            m_i4ItemPixelCnt;
#ifdef	CFG_GUIKIT_DRAGDROP_SUPPORTED_SPEEDUP

	GINT32            m_i4ItemDrawingPixelCnt;
	GUINT32           m_u4DrawingPageCnt;
	GINT32            m_i4ItemShowingPixelCnt;
	GINT32            m_u4ShowPixelTotal;
	GUINT32           m_u4ShowItemCnt;
	GUINT32			  m_u4SlideFlag;
#endif

    GINT32           m_u4PixelCnt;
    GINT32           m_u4PageCnt;
    GUINT32           m_u4SlideOldOrigin;
    GINT32           m_u4SlideNewOrigin;

    GUINT32           m_u4SlideBgIconID;
    GUINT32           m_u4PrevItemIdx;
#ifdef LIST_VIEW_LYRIC_SUPPORT
	TRIVERTEX        m_VertTop[2];
	TRIVERTEX        m_VertBottom[2];
	TRIVERTEX        m_VertFocusItem[2];
	GUINT32			 m_u4LyricFocusItem;
#endif
#endif

    GScrollBar  m_vbar;
    GLVSTYLE_T *m_prStyle;
    BOOL              m_fgScroll; //Temporary solotion for Listview scrollbar performance issue. mtk40162 2009-3-2

#ifdef BT_SPECIAL_ALIGN
	GVOID Special_Align_String(GHANDLE hGC, GTCHAR* szText, GINT32 i4MaxLength);

	int iItemLeftWidth;        //  -1  默认是左右两边等宽

	GUINT32 iItemLeftAliagn;   //  Item 左边的 Text 的对齐方式
    
	GUINT32 iItemRightAliagn;  //  Item 右边的 Text 的对齐方式

	//BOOL  m_Special_Align;
#endif

	GRECT_T m_rIcon; //mtk71372 2011.11.28 added
    
    friend class GComboBox;
};


/**@}*///groupLView

/**
* @defgroup groupMView Matrix View Control
* @ingroup groupViewCtrl
* @{
*/
/** matrix view special data*/
typedef struct _GMVIEWSPECDATA
{
    GUINT32           u4ItemNum;/**< item number*/
    //a line rows
    GUINT16           u2Rows;/**< row items number of a matrix view surface*/
    GUINT16           u2Cols;/**< col items number of a matrix view surface*/
    
    GUCHAR           *pbVIChunkBuf;/**< APP give the space buffer,if it is NULL,memory will dynamic create*/
    GUINT32          *pu4VIChunkSz;/**< the buffer size*/
    GUINT32           u4MaxItems;/**< max items number*/
    GVIEWCTRLITEM_T *prItem; /**< items parameter*/
#if CFG_GUIKIT_NEW_STYLE
    GMVSTYLE_T      *prStyle;
    GSCROLLSTYLE_T  *prScrollStyle;
#endif
} GMVIEWSPECDATA_T;

/**the matrix view class*/
class GMView : public GViewCtrl
{
public:
    GMView(GVOID);
    /** 
    *  destruct function
    *
    *  @param  [in] prRect  the view rectangle
    *
    *  @param  [in] u4WFlags the view type
    *
    *  @param  [in] u4ID  the view ID
    *
    *  @see   WF_DYNAMIC
    *         WF_LOOP
    *         WF_VSCROLL
    *         WF_HSCROLL
    *         WF_AUTOVSCROLL
    *         WF_AUTOHSCROLL
    *         WF_VWRAP
    *         WF_TWOSTATEITEM
    *         WF_CANFOCUSED
    *         WF_TABSTOP
    */
    GMView(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED | WF_TABSTOP, 
           GUINT32 u4ID = 0xFFFFFFFF);
    virtual ~GMView(GVOID);
    /** 
    *  set matrix view style
    *
    *  @param  [in] prStyle  the handle of style parameter
    *
    *  @return NONE
    *
    *  @see   GMVSTYLE_T
    */
    GVOID SetStyle(GMVSTYLE_T *prStyle);

    GVOID SetScrollStyle(GSCROLLSTYLE_T *prStyle);

    GVOID GetScrollStyle(GSCROLLSTYLE_T *prStyle);
	inline GVOID SetItemTextAlign(GUINT16 m_iAlign)
	{
		if ((m_iAlign <= ITEM_TEXT_RIGHT) && (ITEM_TEXT_LEFT<=m_iAlign))
		{
			m_ItemTextAlign = m_iAlign;
		}
		else
		{
			m_ItemTextAlign = ITEM_TEXT_LEFT;
		}
	}

	inline GUINT16 GetItemTextAlign(GVOID)
	{
		return m_ItemTextAlign;
	}

    /** 
    *  get matrix view style
    *
    *  @param  [in] prStyle  the handle of style parameter
    *
    *  @return NONE
    *
    *  @see   GMVSTYLE_T
    */
    GVOID GetStyle(GMVSTYLE_T *prStyle);

    /** 
    *  set matrix view x coordinate item number
    *
    *  @param  [in] i2Num  the item number
    *
    *  @return NONE
    */
    GVOID SetWrapNum(GINT16 i2Num);

    /** 
    *  setup row and col item number of a matrix view surface
    *
    *  @param  [in] u2Rows  the row item number
    *
    *  @param  [in] u2Cols  the col item number
    *
    *  @return NONE
    */
    GVOID Setup(GUINT16 u2Rows, GUINT16 u2Cols);

    /** 
    *   set row item number of a matrix view surface
    *
    *  @param  [in] u2Rows  the item number
    *
    *  @return NONE
    */
    GVOID SetRows(GUINT16 u2Rows);

    /** 
    *   set col item number of a matrix view surface
    *
    *  @param  [in] u2Cols  the item number
    *
    *  @return NONE
    */
    GVOID SetCols(GUINT16 u2Cols);

    /** 
    *   get row item number of a matrix view surface
    *
    *  @return  the item number
    */
    inline GUINT16 GetRows(GVOID)
    {
        return (m_u2Rows);
    }

    /** 
    *   get col item number of a matrix view surface
    *
    *  @return  the item number
    */
    inline GUINT16 GetCols(GVOID)
    {
        return (m_u2Cols);
    }

    virtual GWidget *TouchHitTest(GPOINT_T *prPt);

    GVOID *operator new(size_t u4Size);
    GVOID operator delete(GVOID *pvObj);

	GVOID  DisableItemBkg( GBOOL bFlag);
    

	virtual GVOID RedrawItem(GUINT32 u4Index);

    static GWidget *CreateInstance(GVOID *pvSpecData);
    static GWidget *CreateInstanceEx(GRECT_T *prRect, GUINT32 u4WFlags,
                                     GUINT32 u4ID, GVOID *pvSpecData);

#ifdef CFG_GUIKIT_DRAGDROP_SUPPORTED
    GBOOL   OnHSlide(GINT32 i4SlideMode, GUINT32 u4ItemCount);
#endif

	GScrollBar *GetVBar(GVOID)
	{
		return (&m_vbar);
	}

protected:
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    virtual GVOID InternalRedraw(GRECT_T *prUpdate, GUINT32 u4Flags);
    virtual GVOID InternalUpdate(GBOOL fgErase = TRUE);
    virtual GVOID UpdateClientRect(GVOID);
    virtual GBOOL MoChangeFocus(GUINT32 u4Evt, GUINT32 u4Param1, GUINT32 u4Param2);

private:
#define ROWTEXT_MAX_LEN   20
#define MAX_ROWS          5

    GBOOL  IsShowVBar(GVOID);
    GBOOL  IsShowHBar(GVOID);
    GVOID  UpdateVBarRect(GMVSTYLE_T *prStyle);
    GVOID  UpdateHBarRect(GMVSTYLE_T *prStyle);
    GVOID  UpdateVBarInfo(GVOID);
    GVOID  UpdateHBarInfo(GVOID);
    GVOID  RedrawVBar(GRECT_T *prRect, GBOOL fgErase);
    GVOID  RedrawHBar(GRECT_T *prRect, GBOOL fgErase);
    GBOOL  GotoPreviousRow(GVOID);
    GBOOL  GotoNextRow(GVOID);
    GBOOL  GotoPreviousColumn(GVOID);
    GBOOL  GotoNextColumn(GVOID);
    GBOOL  GotoPreviousItem(GVOID);
    GBOOL  GotoNextItem(GVOID);
    GBOOL  GotoPreviousPage(GBOOL fgHorz);
    GBOOL  GotoNextPage(GBOOL fgHorz);
    GVOID  GetRCN(GUINT16 *pu2RN, GUINT16 *pu2CN);
    GVOID  GetItemMN(GINT32 i4Index, GINT16 *pi2M, GINT16 *pi2N);
    GVOID  GetItemRC(GINT32 i4Index, GINT16 *pi2Row, GINT16 *pi2Col);
    GINT32 GetItemIndex(GUINT16 u2Row, GUINT16 u2Col);
    GINT32 GetLastVisibleItem(GVOID);
    GVOID  GetSpacing(GMVSTYLE_T *prStyle, GUINT16 *pu2XSpacing,
                     GUINT16 *pu2YSpacing);
    GVOID  MeasureItem(GMVSTYLE_T *prStyle, GUINT16 *pu2ItemW,
                      GUINT16 *pu2ItemH);
    GVOID  DrawItem(GHANDLE hGC, GINT32 i4Index, GRECT_T *prRect,
                   GMVSTYLE_T *prStyle, GBOOL fgErase = FALSE,
                   GBOOL fgPressed = FALSE, GBOOL fgPaint = FALSE,GBOOL fgMoved = FALSE);
    GVOID  UpdateItem(GHANDLE hGC, GINT32 i4Index, GRECT_T *prRect,
                     GMVSTYLE_T *prStyle, GUINT32 u4OldOrigin,
                     GBOOL fgErase = FALSE);
    GVOID  UpdateAllItems(GHANDLE hGC, GMVSTYLE_T *prStyle, GUINT32 u4OldOrign);
    GVOID  Paint(GHANDLE hGC);
    GBOOL  OnKeyDown(GUINT32 u4KeyCode);
    GBOOL  OnKeyUp(GUINT32 u4KeyCode);
    GBOOL  OnTouchMove(GUINT32 u4Param1, GUINT32 u4Param2);
    GBOOL  OnTouchDown(GUINT32 u4Param1, GUINT32 u4Param2);
    GBOOL  OnTouchUp(GUINT32 u4Param1, GUINT32 u4Param2);
    GBOOL  OnTouchLeave(GVOID);
    GBOOL  OnVScroll(GWidget *pWidget, GUINT32 u4Evt, GUINT32 u4Param1,
                    GUINT32 u4Param2);
    GBOOL  OnHScroll(GWidget *pWidget, GUINT32 u4Evt, GUINT32 u4Param1,
                    GUINT32 u4Param2);

    GUCHAR SplitText(GTCHAR *szText, GTCHAR szSubText[][ROWTEXT_MAX_LEN]);
    GUCHAR AutoSplitText(GHANDLE hGC, GMVSTYLE_T *prStyle, GTCHAR *szText,
                        GTCHAR szSubText[][ROWTEXT_MAX_LEN]);
    GBOOL  GetTextExtent(GHANDLE hGC, GMVSTYLE_T *prStyle, GTCHAR *szText,
                        GBOOL fgAuto, GSIZE_T *prExtent);
    GVOID  DrawItemText(GHANDLE hGC, GTCHAR *szText, GMVSTYLE_T *prStyle,
                       GRECT_T *prTextBox, GBOOL fgAuto);

#ifdef CFG_GUIKIT_DRAGDROP_SUPPORTED
    GVOID   OnHSlideStop(GBOOL fgRedraw);
    static GVOID SlideTimerCb(GVOID *pvParam);

    GINT32            m_i4SlideStepX;
	GINT32            m_i4SlideStepY;
    GINT32            m_i4SlideLayer1ID;
    GINT32            m_i4SlideLayer2ID;
    GUINT32           m_u4SurfH;
    GUINT32           m_u4SurfW;
    HSURFACE         m_hSlideSurf1;
    HSURFACE         m_hSlideSurf2;
    GTIMER_T         m_rSlideTimer;
    GBOOL             m_fgSlideTimerStart;
    GINT32            m_i4SlideMode;
    GUINT32           m_u4PagePixelCnt;
    GBOOL             m_fgHSliderDone;
#endif

    GScrollBar  m_vbar;
    GScrollBar  m_hbar;
    GMVSTYLE_T *m_prStyle;
    GINT16       m_i2WrapNum;
    GUINT16      m_u2Rows;
    GUINT16      m_u2Cols;
    GUINT16      m_u2M;
    GUINT16      m_u2N;

    GBOOL        m_fgIsFirstFocus;
    GUINT16      m_ItemTextAlign; 
};
/**@}*///groupMView

#endif //_GVIEWCTRL_H_