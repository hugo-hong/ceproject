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
 *   GEvent.h
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


#ifndef _GEVENT_H_
#define _GEVENT_H_

#include "GDef.h"
#include "GEvtDef.h"

/**
* @defgroup groupEvent Event
* @ingroup groupGUIKit
* @{
*/

class GWidget;

//Notify event header
typedef struct _GNEHDR
{
    GWidget *pWidget;
    GUINT32   u4ID;
    GUINT32   u4Code;
} GNEHDR_T;


typedef struct _GVIEWCTRLNEITEMEVENT
{
    GNEHDR_T   rHdr;
    GINT32      i4Index;
} GVIEWCTRLNEITEMEVENT_T;


/** 
 * Destory widget.
 *
 * @Param u4Param1 -- Not use. 
 * @Param u4Param2 -- Not use.
 */
#define EVT_GUI_DESTROY         (EVT_GUI_FIRST)

/**
 * Show the widget.
 *
 * @param u4Param1 -- Specifies the flag for show.
 * @Param u4Param2 -- Not use.
 *
 * @note The flag: \n
 *         0 -- set to normal state without draw.
 *         1 -- set to normal state with draw.
 *         2 -- keep original state and paint it if visible.
 */
#define EVT_GUI_SHOW            (EVT_GUI_FIRST + 1)

/**
 * Hide the widget.
 *
 * @param u4Param1 -- Specifies the flag for hide.
 * @Param u4Param2 -- Not use.
 *
 * @note The flag: \n
 *         0 -- set to invisible state without background erase.
 *         1 -- set to invisible state with background erase.
 *         2 -- keep original state without background erase. \n
 *              (used for notification only!)
 */
#define EVT_GUI_HIDE            (EVT_GUI_FIRST + 2)

/** 
 * Set focus for the widget.
 *
 * @param u4Param1 -- Specifies is setup only(TRUE) or not(FALSE).
 * @param u4Param2 -- Not use.
 *
 * @note "Setup only" means the widget just do logic process 
 *       (for example the state modification) without real painting. 
 */
#define EVT_GUI_SETFOCUS        (EVT_GUI_FIRST + 3)

/** 
 * Kill focus for the widget.
 *
 * @param u4Param1 -- Specifies is setup only(TRUE) or not(FALSE).
 * @param u4Param2 -- Not use.
 *
 * @note "Setup only" means the widget just do logic process 
 *       (for example the state modification) without real painting. 
 */
#define EVT_GUI_KILLFOCUS       (EVT_GUI_FIRST + 4)

/**
 * Enable the widget.
 *
 * @param u4Param1 -- Specifies is setup only(TRUE) or not(FALSE). 
 * @Param u4Param2 -- Not use.
 *
 * @note "Setup only" means the widget just do logic process 
 *       (for example the state modification) without real painting. 
 */
#define EVT_GUI_ENABLE          (EVT_GUI_FIRST + 5)

/**
 * Disable the widget.
 *
 * @param u4Param1 -- Specifies is setup only(TRUE) or not(FALSE). 
 * @Param u4Param2 -- Not use.
 *
 * @note "Setup only" means the widget just do logic process 
 *       (for example the state modification) without real painting. 
 */
#define EVT_GUI_DISABLE         (EVT_GUI_FIRST + 6)

/** 
 * Paint the widget.
 *
 * @param u4Param1 -- Not use.
 * @param u4Param2 -- Pointer to a RECT_T structure that contains the 
 *                    coordinates, in device units, of the update rectangle.\n
                      If this parameter is NULL, the entire area of the
                      widget is redrawn. 
 */
#define EVT_GUI_PAINT           (EVT_GUI_FIRST + 7)


/** 
 * Informs a widget to adjust its size.
 *
 * @param u4Param1 -- New width.
 * @param u4Param2 -- New height.
 */
#define EVT_GUI_RESIZE          (EVT_GUI_FIRST + 8)

#define EVT_GUI_MOVE            (EVT_GUI_FIRST + 9)

//mtk71372 2011.10.11 ------------------------------------------ str
/** 
 * Switch the current panel to another
 *
 * @param u4Param1 -- The Object GPanel::SwitchProperty
 */
#define EVT_GUI_PANEL_SWITCH	(EVT_GUI_FIRST + 10)

/** 
* Informs the callback it would draw the text
*
* @param u4Param1 -- The hGC handle
* @param u4Param2 -- The text string
*/
#define EVT_GUI_DRAW_TEXT	(EVT_GUI_FIRST + 11)
//mtk71732 2011.10.11 ------------------------------------------ end


#define EVT_GUI_KEYFIRST        (EVT_KEYDOWN)

/*@{*/
/**
 * @name Widget Messages: (2) Generic Command Messages
 */
/** 
 * Press a key down.
 *
 * @param u4Param1 -- Key code. 
 * @param u4Param2 -- Not use.
 */
#define EVT_GUI_KEYDOWN         (EVT_KEYDOWN)

/** 
 * Release up a key.
 *
 * @param u4Param1 -- Key code.
 * @param u4Param2 -- Not use.
 */
#define EVT_GUI_KEYUP           (EVT_KEYUP)

/** 
 * Long press up a key.
 *
 * @param u4Param1 -- Key code.
 * @param u4Param2 -- Not use.
 */
#define EVT_GUI_KEYLONGPRESS    (EVT_KEYLONGPRESS)

/** 
 * Repeat a key.
 *
 * @param u4Param1 -- Key code 
 * @param u4Param2 -- Not use.
 */
#define EVT_GUI_KEYREPEAT       (EVT_KEYREPEAT)

#define EVT_GUI_KEYLAST         (EVT_GUI_KEYFIRST + 3)

#define EVT_GUI_TOUCHFIRST      (EVT_TOUCHPRESS)
#define EVT_GUI_TOUCHPRESS      (EVT_TOUCHPRESS)
#define EVT_GUI_TOUCHRELEASE    (EVT_TOUCHRELEASE)
#define EVT_GUI_TOUCHMOVE       (EVT_TOUCHMOVE)
#define EVT_GUI_TOUCHREPEAT     (EVT_TOUCHREPEAT)
#define EVT_GUI_TOUCHLONGPRESS  (EVT_TOUCH_LONG_PRESS)
#define EVT_GUI_TOUCH_DB_CLICK  (EVT_TOUCH_DB_CLICK)
#define EVT_GUI_TOUCH_SLIDE     (EVT_TOUCH_SLIDE)
#define EVT_GUI_TOUCHLAST       (EVT_GUI_TOUCHFIRST + 6)


/** 
 * Indicates that the widget is clicked.
 *
 * @param u4Param1 -- Not use.
 * @param u4Param2 -- Not use.
 */
#define EVT_GUI_CLICKED         (EVT_GUI_FIRST + 0x14)

/** 
 * Indicates that the widget is checked/un-checked.
 *
 * @Param u4Param1 -- Specifies is checked (TRUE) or un-checked (FALSE).
 * @Param u4Param2 -- Not use.
 */
#define EVT_GUI_CHECKED         (EVT_GUI_FIRST + 0x15)

/** 
 * Indicates that the widget is toggled/un-toggled.
 *
 * @Param u4Param1 -- Specifies is toggled (TRUE) or un-toggled (FALSE).
 * @Param u4Param2 -- Not use.
 */
#define EVT_GUI_TOGGLED         EVT_GUI_CHECKED

/** 
 * Indicates that the widget is selected/un-selected
 *
 * @Param u4Param1 -- Specifies is selected (TRUE) or un-selected (FALSE).
 * @Param u4Param2 -- Not use.
 */
#define EVT_GUI_SELECTED        (EVT_GUI_FIRST + 0x16)

#define EVT_GUI_NCPAINT         (EVT_GUI_FIRST + 0x17)

#define EVT_GUI_WGTSTYLE        (EVT_GUI_FIRST + 0x18)

#define EVT_GUI_NEXTITEM        (EVT_GUI_FIRST + 0x19)

#define EVT_GUI_ACTIVATE        (EVT_GUI_FIRST + 0x1A)
#define EVT_GUI_DEACTIVATE      (EVT_GUI_FIRST + 0x1B)

#define EVT_GUI_ERASEBKG        (EVT_GUI_FIRST + 0x1C)

#define EVT_GUI_VSCROLL         (EVT_GUI_FIRST + 0x1D)
#define EVT_GUI_HSCROLL         (EVT_GUI_FIRST + 0x1E)

//mtk71372 2011.11.28 add comment
//EVT_GUI_ITEMCLICKED is special for GLView and the u4Param2 is meaning.
//@Param u4Param2 -- Specifies the ICON of item is clicked (TRUE) or un-clicked (FALSE).
#define EVT_GUI_ITEMCLICKED     (EVT_GUI_FIRST + 0x1F)

#define EVT_GUI_ITEMUNHIGHLIGHT (EVT_GUI_FIRST + 0x20)
#define EVT_GUI_ITEMHIGHLIGHT   (EVT_GUI_FIRST + 0x21)

#define EVT_GUI_SELCHANGED      (EVT_GUI_FIRST + 0x22)
#define EVT_GUI_EDITCHANGED     (EVT_GUI_FIRST + 0x23)
#define EVT_GUI_EDITCLICKED     (EVT_GUI_FIRST + 0x24)

#define EVT_GUI_CLOSE           (EVT_GUI_FIRST + 0x25)

/*
 * @Param u4Param1 0-up,1-down, 2-left, 3-right
 * @Param u4param2 current value
 */

#define EVT_GUI_SCROLL          (EVT_GUI_FIRST + 0x26)  

/*
 * @Param u4Param1 0-year,1-month,2-day
 * @Param u4Param2 value
 */
#define EVT_GUI_DATECHANGED     (EVT_GUI_FIRST + 0x27)


/*
* @Param u4Param1 0-hour,1-minute,2-second
* @Param u4Param2 value
*/
#define EVT_GUI_TIMECHANGED     (EVT_GUI_FIRST + 0x28)

#define EVT_GUI_INITCOMPONENT   (EVT_GUI_FIRST + 0x29)


#define EVT_GUI_NOTIFY          (EVT_GUI_FIRST + 0x2A)
    #define NECODE_ITEMCLICKED       0x01
    #define NECODE_ITEMHIGHLIGHT     0x02
    #define NECODE_ITEMUNHIGHLIGHT   0x03
    #define NECODE_ITEMUNCLICKED     0x04 // Added by mcn03011 

#define EVT_GUI_DRAWNCBTN       (EVT_GUI_FIRST + 0x2B)
#define EVT_GUI_NCBTNEXTENT     (EVT_GUI_FIRST + 0x2C)
#define EVT_GUI_NCBTNCLICKED    (EVT_GUI_FIRST + 0x2D)

#define EVT_GUI_DRAWITEM        (EVT_GUI_FIRST + 0x2E)

#define EVT_GUI_SWITCHPAGE      (EVT_GUI_FIRST + 0x2F)

#define EVT_GUI_NEXTROW         (EVT_GUI_FIRST + 0x30)
#define EVT_GUI_NEXTCOL         (EVT_GUI_FIRST + 0x31)
#define EVT_GUI_NEXTPAGE        (EVT_GUI_FIRST + 0x32)

#define EVT_GUI_TOUCHENTER      (EVT_GUI_FIRST + 0x33)
#define EVT_GUI_TOUCHLEAVE      (EVT_GUI_FIRST + 0x34)

#define EVT_GUI_ITEMTOUCHED     (EVT_GUI_FIRST + 0x35)
#define EVT_GUI_ITEMDBCLICKED   (EVT_GUI_FIRST + 0x36)

#define EVT_GUI_PAGECHANGED     (EVT_GUI_FIRST + 0x37)

#define EVT_GUI_ORIGINITEMCHANGED (EVT_GUI_FIRST + 0x38) //Chen Jie 2009-3-2
#define EVT_GUI_ORIGINITEMCHANG_START (EVT_GUI_FIRST + 0x39) //Chen Jie 2009-3-2

#define EVT_GUI_BRINGTOFG       (EVT_GUI_FIRST + 0x40)
#define EVT_GUI_BRINGTOBG       (EVT_GUI_FIRST + 0x41)

#define EVT_GUI_VIEWFIRST       (EVT_GUI_FIRST + 0x60)
#define EVT_GUI_BRINGVIEWTOFG   (EVT_GUI_VIEWFIRST)
#define EVT_GUI_BRINGVIEWTOBG   (EVT_GUI_VIEWFIRST + 1)
#define EVT_GUI_VIEWLAST        (EVT_GUI_VIEWFIRST + 1)

#define EVT_GUI_RESOLUTIONCHANGED (EVT_GUI_FIRST + 0x80)

#define EVT_GUI_USER            (EVT_GUI_FIRST + 0x100)
#define EVT_GUI_APP             (EVT_GUI_FIRST + 0x200)

#define EVT_GUI_VSCROLL_AUTO	(EVT_GUI_USER + 0x0100)

GBOOL GSendEvent(GWidget *pWidget, GUINT32 u4Event, GUINT32 u4Param1,
                GUINT32   u4Param2);
GBOOL GDispatchEvent(GWidget *pWidget, GUINT32 u4Event, GUINT32 u4Param1,
                    GUINT32 u4Param2);
GBOOL GBroadcastEvent(GUINT32 u4Evt, GUINT32 u4Param1, GUINT32 u4Param2);

/*@}*/ //end of Event

#endif //_GEVENT_H_
