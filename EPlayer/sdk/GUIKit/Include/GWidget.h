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
 *   GWidget.h
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

#ifndef _GWIDGET_H_
#define _GWIDGET_H_

#include "GObject.h"
#include "GRect.h"
#include "GGdi.h"
#include "GEvent.h"
#include "GUIKitCfg.h"

#define INVOKE_CALLBACK_FOR_DRAW_TEXT	//mtk71372 2012.02.23

/**
* @defgroup groupWidget Base Widget
* @ingroup groupGUIKit
* @{
*/


/** 
 * @brief Specifies alignment of content on the drawing surface.
 */
typedef enum _GALIGNMENT
{
    /** @brief Content is vertically aligned at the bottom, and horizontally aligned at the center. */
    BOTTOM_CENTER,
    /** @brief Content is vertically aligned at the bottom, and horizontally aligned on the left. */
    BOTTOM_LEFT,
    /** @brief Content is vertically aligned at the bottom, and horizontally aligned on the right. */
    BOTTOM_RIGHT,
    /** @brief Content is vertically aligned in the middle, and horizontally aligned at the center. */
    MIDDLE_CENTER,
    /** @brief Content is vertically aligned in the middle, and horizontally aligned on the left. */
    MIDDLE_LEFT,
    /** @brief Content is vertically aligned in the middle, and horizontally aligned on the right. */
    MIDDLE_RIGHT,
    /** @brief Content is vertically aligned at the top, and horizontally aligned at the center. */
    TOP_CENTER,
    /** @brief Content is vertically aligned at the top, and horizontally aligned on the left. */
    TOP_LEFT,
    /** @brief Content is vertically aligned at the top, and horizontally aligned on the right. */
    TOP_RIGHT
} GALIGNMENT_T;


typedef enum _WGTSTATE
{
    WST_NORMAL,
    WST_PRESSED, 
    WST_FOCUSED,
    WST_SELECTED,
    WST_DISABLE,
    WST_UNKNOWN    
} GWGTSTATE_T;


/**
 * @brief The _WGTITEM structure defines the dimensions and style of a child
 * widget in a container widget. One or more of these structures are combined with 
 * a _WGTTEMPLATE structure to form a standard template for a container widget.
 */
typedef struct _WGTITEM
{
    /** @brief Specifies the widget type, for example WT_BUTTON, WT_FORM, and so on. */
    GUINT32 u4Type;
    /**
     * @brief Specifies the flags of the widget. This member can be a combination
     * of base widget flag values (such as GWidget::WF_NOBKGND) and one or more of
     * the special widget flag values (such as GButton::WF_HSCROLLTEXT and GButton::WF_TOGGLE).
     */
    GUINT32 u4WFlags;
    /** @brief Specifies the widget identifier. */
    GUINT32 u4ID;
    
    /** 
     * @brief Specifies the x-coordinate, in pixel units, of the upper-left
     * corner of the widget. This coordinate is always relative to the upper-left
     * corner of the parent widget's client area.
     */
    GINT32  i4X;
    /**
     * @brief Specifies the y-coordinate, in pixel units, of the upper-left
     * corner of the widget. This coordinate is always relative to the upper-left
     * corner of the parent widget's client area.
     */
    GINT32  i4Y;
    /** @brief Specifies the width, in pixel units, of the widget. */
    GINT32  i4Width;
    /** @brief Specifies the height, in pixel units, of the widget. */
    GINT32  i4Height;

    /** 
     * @brief Pointer to the special widget's data, this member is depend on u4Type, for example,
     * if the member u4Type is WT_BUTTON, this member should be a pointer to a GBUTTONSPECDATA_T
     * structure.
     */
    GVOID  *pvSpecData;
} GWGTITEM_T;


/**
 * @brief The _WGTTEMPLATE structure defines the dimensions and styles of all
 * child widgets in a container widget. This structure specifies the number of
 * widgets in the container widget and therefore specifies a pointer to the number of subsequent _WGTITEM structures in the template. 
 */
typedef struct _WGTTEMPLATE
{
    /** @brief Specifies the number of items in the container widget. */
    GUINT32 u2Num;

    /** @brief Pointer to the number of _WGTITEM structure */
    GWGTITEM_T *prItem;
} GWGTTEMPLATE_T;


/**
 * @brief Provides the base functionality of all widget classes in the GUIKit.
 */
class GWidget : public GCmdSource
{
public:
    /** 
     * @brief Constructs a GWidget object. 
     *
     * @param None.
     */
    GWidget(GVOID);
    
    /** 
     * @brief Constructs a GWidget object.
     *
     * @param [in] prRect   The size and position of the widget, in client coordinates of it's parent widget.
     * @param [in] u4WFlags Specifies the flags of the widget ,for example GWidget::WF_NOBKGND, and so on.
     * @param [in] u4ID     Specifies the widget identifier.
     */
    GWidget(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED | WF_TABSTOP, 
            GUINT32 u4ID = 0xFFFFFFFF);

    /**
     * @brief Destructs a GWidget object.
     *
     * @param None.
     */
    virtual ~GWidget(GVOID);

    /**
     * @brief Allocates memory for a GWidget object from the free store and
     *        returns a suitably typed, nonzero pointer to a GWidget object.
     *
     * @param [in] u4Size the size of a widget object.
     *
     * @return A pointer to the new widget object if it is successful; otherwise, it is NULL.
     */
    GVOID *operator new(size_t u4Size);

    /**
     * @brief Deallocates a block of memory holding a widget object.
     *
     * @param [in] pvObj Pointer to a widget object.
     *
     * @return None.
     */
    GVOID  operator delete(GVOID *pvObj);

    /**
     * @brief Allocates memory for a GWidget object from the free store and
     *        returns a suitably typed, nonzero pointer to a GWidget object.
     *
     * @param [in] u4Size The size of a widget object.
     * @param [in] pvBuf  Pointer to a block of external memory, the memory size should be greater than the parameter u4Size.
     *
     * @return A pointer to the new widget object if it is successful; otherwise, it is NULL.
     */
    GVOID *operator new(size_t u4Size, GVOID *pvBuf);
//\\ Jau
#if !defined(TARGET_MT6228) && defined(WIN32)
    /**
     * @brief Deallocates a block of memory holding a widget object.
     *
     * @param [in] pvObj Pointer to a widget object.
     * @param [in] pvBuf Pointer to a block of external memory.
     *
     * @return None.
     */
    GVOID  operator delete(GVOID *pvObj, GVOID *pvBuf);
#endif

    /** 
     * @brief Sets a GBOOL value that indicates whether the widget can be deleted
     *        automatically or not when it's parent widget is deleted. 
     *
     * @param [in] fgAutoDelete When set this parameter to TRUE, the widget will
     *                          be deleted automatically after it's parent widget
     *                          is deleted, otherwise the widget will not be deleted
     *                          after it's parent widget is deleted.
     *
     * @return None.
     *
     * @remarks If the widget is an attached form in a view and set the parameter
     *          fgAutoDelete to TRUE, the attached form will be deleted automatically
     *          after the view exit, otherwise the attached form will not be deleted
     *          after the view exit.
     */
    GVOID     SetAutoDelete(GBOOL fgAutoDelete = TRUE);

#if DDRAW_PANEL_MOVE
	/** 
    *  set widget effect flag
    *
    *  @param  [in] fgAutoDelete  true or false
    *
    *  @return NONE
    */
	GVOID     SetWidgetEffect(GBOOL EffectFlag);

	/** 
    *  get widget effect flag
    *
    *  @param  [in] fgAutoDelete  true or false
    *
    *  @return NONE
    */
	GBOOL      GetWidgetEffect();

     /** 
    *  move widget from source point to destination point
    *
    *  @param  [in] iSource4X  the source point x coordinate
    *
    *  @param  [in] iSource4Y  the source point y coordinate
    *
	*  @param  [in] iDest4X   the dest point x coordinate
	*
	*  @param  [in] iDesti4Y  the dest point y coordinate
	*
    *  @return NONE
    */
    GVOID     MoveSourceToDest(GINT32 iDest4X, GINT32 iDesti4Y,GINT32 iSource4X, GINT32 iSource4Y);


	/** 
    *  move widget to the point i4X, i4Y  and do not show
    *
    *  @param  [in] i4X, i4Y
    *
    *  @return NONE
    */
	GVOID     MoveWithOutShow(GINT32 i4X, GINT32 i4Y);


	 /** 
    *  move widget to the point and do not show
    *
    *  @param  [in] prPos  the point handle
    *
    *  @return NONE
    */
    GVOID     MoveWithOutShow(GPOINT_T *prPos);

	 /** 
    *  redraw the rectangle
    *
    *  @param  [in] prUpdate  the rectangle handle
    *
    *  @param  [in] u4Flags  redraw flag
    *
    *  @return NONE
    *
    *  @see    RDF_ERASE
    *          RDF_INVALIDATE
    *          RDF_NOCHILDREN
    */ 
	GVOID     RedrawWithOutShow(GRECT_T *prUpdate, GUINT32 u4Flags = RDF_ERASE | 
                                                          RDF_INVALIDATE);

	/** 
    *  Get Widegt Move care Flag
    *
    *  @param  NULL
    *
    *  @return GBOOL: TRUE care; FALSE don't care
    *
    *  @see    
    *          
    *
    */ 
	GBOOL     GetWidegtMoveCareFlag(GVOID);

	/** 
    *  Set Widegt Move care Flag
    *
    *  @param  NULL
    *
    *  @return GVOID
    *
    *  @see    
    *          
    *
    */ 
	GVOID     SetWidegtMoveCareFlag(GBOOL bMoveCareFlag);

	/** 
    *  Get Widegt SelsectFlag
    *
    *  @param  NULL
    *
    *  @return GBOOL
    *
    *  @see    
    *          
    *
    */ 
	GBOOL     GetWidegtSelsectFlag(GVOID);

	/** 
    *  Set Widegt SelsectFlag
    *   
    *
    *  @param  GBOOL bSelsectFlag
    *
    *  @return GVOID      
    *
    */ 
	GVOID     SetWidegtSelsectFlag(GBOOL bSelsectFlag);

	/** 
    *  Get Widegt SlideDirection
    *
    *  @param  NULL
    *
    *  @return int
    *
    *  @see    
    *          
    *
    */ 
	int     GetSlideDirection(GVOID);

	/** 
    *  Set Widegt SlideDirection
    *   
    *
    *  @param  GBOOL bSelsectFlag
    *
    *  @return GVOID      
    *
    */ 
	GVOID     SetSlideDirection(int iDirection);
#endif 


    /** 
     * @brief Check whether the widget is a top-level widget (a widget that has no
     *        parent widget) or not.  
     *
     * @param None.
     *
     * @return If the widget is a top-level widget, return value is TRUE, otherwise
     *         return value is FALSE.
     */
    GBOOL     IsToplevel(GVOID) const;
    /** 
     * @brief Retrieve the top-level widget in the widget hierarchy which contain the current widget.
     *
     * @param None
     *
     * @return A pointer to the top-level widget if it is successful; otherwise, it is NULL.
     */
    GWidget *ToplevelWidget(GVOID) const;

    /** 
     * @brief Changes the parent widget of the specified child widget.
     *
     * @param [in] pParent  Pointer to the new parent widget.
     *
     * @return None.
     */
    inline GVOID SetParent(GWidget *pParent)
    {
        m_pParent = pParent;
    }
    /** 
     * @brief Retrieve the parent widget of the current widget.
     *
     * @param None
     *
     * @return A pointer to the parent widget if it is successful; otherwise, it is NULL.
     */
    GWidget *ParentWidget(GVOID) const;

    /**
     * @brief Sets the current widget's owner to the specified widget object.
     *
     * @param [in] pOwner  Pointer to the new owner of the current widget object.
     *                     If this parameter is NULL, the current widget object
     *                     has no owner.
     *
     * @return None.
     */
    inline GVOID SetOwner(GWidget *pOwner)
    {
        m_pOwner = pOwner;
    }

    /**
     * @brief Retrieve the owner widget of the current widget.
     *
     * @param None.
     *
     * @return If the current widget has owner, return a pointer to the owner widget;
     *         otherwise return NULL.
     */
    inline GWidget *GetOwner(GVOID) CONST
    {
        return (m_pOwner);
    }

    /** 
     * @brief Sets the identifier of the current widget.
     *
     * @param [in] u4ID  The new identifier of the current widget.
     *
     * @return None.
     */
    GVOID     SetID(GUINT32 u4ID);

    /** 
     * @brief Retrieves the identifier of the current widget.
     *
     * @param None.
     *
     * @return Return the identifier of the current widget.
     */
    GUINT32   GetID(GVOID) const;

    /** 
     * @brief Sets the widget type of the current widget.
     *
     * @param [in] u4Type Specifies the new widget type of the current widget.
     *
     *  @return None.
     */
    GVOID     SetType(GUINT32 u4Type);
    
    /** 
     * @brief Retrieves the widget type of the current widget.
     *
     * @param None.
     *
     * @return Return the widget type of the current widget.
     */
    GUINT32   GetType(GVOID);

    /** 
     * @brief Set the widget flags.
     *
     * @param [in] u4WFlags  Specifies the new flags of the current widget, this
     *                       parameter can be GWidget::WF_NOBKGND, GWidget::WF_TABSTOP,
     *                       and so on.
     *
     * @return None
     *
     * @see GetWFlags.
     */
    GVOID     SetWFlags(GUINT32 u4WFlags);
    
    /** 
     * @brief Retrieves the widget flags.
     *
     * @param None.
     *
     * @return Return the widget flags.
     *
     * @see SetWFlags
     */
    GUINT32   GetWFlags(GVOID) const;
   
    /** 
     * @brief Set the widget extended flags.
     *
     * @param [in] u4WFlags  Specifies the new extended flags of the current widget, this
     *                       parameter can be GWidget::WF_EX_NONORMALBKGND, and so on.
     *
     * @return None
     *
     * @see GetWFlagsEx.
     */
    GVOID     SetWFlagsEx(GUINT32 u4WFlags);
    
    /** 
     * @brief Retrieves the widget extended flags.
     *
     * @param None.
     *
     * @return Return the widget extended flags.
     *
     * @see SetWFlagsEx
     */
    GUINT32   GetWFlagsEx(GVOID) const; 

    /** 
     * @brief Changes the position of the current widget.
     *
     * @param [in] i4X  Specifies the new position of the left side of the current widget.
     * @param [in] i4Y  Specifies the new position of the top side of the current widget.
     *
     * @return None.
     *
     * @remarks The i4X and i4Y parameters is relatived to the client area of
     *          the current widget's parent widget. If the current widget has
     *          no parent widget, the i4X and i4Y parameter is relatived to the
     *          drawing surface.
     */
    GVOID     Move(GINT32 i4X, GINT32 i4Y);
    
    /** 
     * @brief Changes the position of the current widget.
     *
     * @param [in] prPos  Pointer to a GPOINT_T structure that specifies the new
     *                    position of the left and top side of the current widget.
     *
     * @return None.
     */
    GVOID     Move(GPOINT_T *prPos);
    
    /** 
     * @brief Changes the size of the current widget.
     *
     * @param [in] i4Width  Specifies the new width of the widget.
     * @param [in] i4Height Specifies the new height of the widget.
     *
     * @return None.
     */
    GVOID     Resize(GINT32 i4Width, GINT32 i4Height);
    
    /** 
     * @brief Changes the size of the current widget.
     *
     * @param [in] prSize  Pointer to a GSIZE_T structure that specifies the
     *                     new width and new height of the widget.
     *
     * @return None.
     */
    GVOID     Resize(GSIZE_T *prSize);
    
    /** 
     * @brief Gets the position of the current widget. 
     *
     * @param [out] pi4X  Pointer to a GINT32 variable that receives the position
     *                    of the left side of the current widget.
     * @param [out] pi4Y  Pointer to a GINT32 variable that receives the position
     *                    of the top side of the current widget.
     *
     * @return None.
     */
    GVOID     GetPos(GINT32 *pi4X, GINT32 *pi4Y);
    
    /** 
     * @brief Gets the position of the current widget. 
     *
     * @param [out] prPos  Pointer to a GPOINT_T structure that receives the position
     *                     of the left and top side of the current widget.
     *
     * @return None.
     */
    GVOID     GetPos(GPOINT_T *prPos);
    
    /** 
     * @brief Gets the size of the current widget.
     *
     * @param [out] pi4Width  Pointer to a GINT32 variable that receives the width
     *                        of the current widget.
     * @param [out] pi4Height Pointer to a GINT32 variable that receives the height
     *                        of the current widget.
     *
     * @return None.
     */
    GVOID     GetSize(GINT32 *pi4Width, GINT32 *pi4Height); 
    
    /** 
     * @brief Gets the size of the current widget.
     *
     * @param [out] prSize Pointer to a GSIZE_T structure that receives the width
     *                     and height of the current widget.
     *
     * @return None.
     */
    GVOID     GetSize(GSIZE_T *prSize);
    
    /** 
     * @brief Retrieves the size of the client area of the current widget.
     *
     * @param [out] pi4Width  Pointer to a GINT32 variable that receives the width
     *                        of the client area of the current widget.
     * @param [out] pi4Height Pointer to a GINT32 variable that receives the height
     *                        of the client area of the current widget.
     *
     * @return None.
     */
    GVOID     GetClientSize(GINT32 *pi4Width, GINT32 *pi4Height);
    
    /** 
     * @brief Retrieves the size of the client area of the current widget.
     *
     * @param [out] prSize  Pointer to a GSIZE_T structure that receives the
     *                      width and height of the client area of the 
     *                      current widget.
     *
     * @return None.
     */
    GVOID     GetClientSize(GSIZE_T *prSize);

    /** 
     * @brief Sets the size of the client area of the current widget.
     *
     * @param [in] prSize  Pointer to a GSIZE_T structure that specifies the new
     *                     width and new height of the client area of the current
     *                     widget.
     *
     * @return None.
     */
    GVOID     SetClientSize(GSIZE_T *prSize);

#if DDRAW_PANEL_MOVE
	 /** 
     * @brief Gets the size of the Window area of the current widget.
     *
     * @param [in] prSize  Pointer to a GSIZE_T structure that specifies the new
     *                     width and new height of the client area of the current
     *                     widget.
     *
     * @return None.
     */
	GVOID     GetWindowSize(GSIZE_T *prSize);

	 /** 
     * @brief Sets the size of the Window area of the current widget.
     *
     * @param [in] prSize  Pointer to a GSIZE_T structure that specifies the new
     *                     width and new height of the client area of the current
     *                     widget.
     *
     * @return None.
     */
	GVOID     SetWindowSize(GSIZE_T *prSize);
#endif 

    /** 
     * @brief Enables touch screen and keyboard input to the current widget.
     *
     * @param None.
     *
     * @return None.
     */
    GVOID     Enable(GVOID);

	GVOID     SetEnable(GVOID);
    
    /** 
     * @brief Disables touch screen and keyboard input to the current widget.
     *
     * @param None.
     *
     * @return None.
     */
    GVOID     Disable(GVOID);
    
    /** 
     * @brief Check whether the current widget is visible or not.
     *
     * @param None.
     *
     * @return If the current widget is visible, return TRUE, otherwise return FALSE.
     */
    GBOOL     IsVisible(GVOID) const;
    
    /** 
     * @brief Check whether the current widget is enabled or not.
     *
     * @param None.
     *
     * @return If the current widget is enabled, return TRUE, otherwise return FALSE.
     */
    GBOOL     IsEnabled(GVOID);

	GBOOL     IsWigetEnabled(GVOID) const;
    
    /** 
     * @brief Check whether the current widget is focused or not.
     *
     * @param None.
     *
     * @return If the current widget is focused, return TRUE, otherwise return FALSE.
     */
    GBOOL     IsFocused(GVOID) const;

    /** 
     * @brief Check whether the current widget is pressed or not by the user's touch screen input.
     *
     * @param None.
     *
     * @return If the current widget is pressed, return TRUE, otherwise return FALSE.
     */
    GBOOL    IsTouchPressed(GVOID) const;
   
    virtual GWGTSTATE_T GetState(VOID) const;

    /** 
     * @brief Retrieves the dimensions of the bounding rectangle of the current widget.
     *
     * @param [out] prRect Pointer to a GRECT_T structure that receives the
     *                     screen coordinates of the upper-left and lower-right
     *                     corners of the current widget.
     *
     * @return None.
     */
    GVOID     GetWindowRect(GRECT_T *prRect) const;
    
    /** 
     * @brief Retrieves the coordinates of the current widget's client area.
     *        The client coordinates specify the upper-left and lower-right
     *        corners of the client area. Because client coordinates are relative
     *        to the upper-left corner of the widget's client area, the
     *        coordinates of the upper-left corner are (0,0). 
     *
     * @param [out] prRect  Pointer to a GRECT_T structure that receives the
     *                      client coordinates. The left and top members are
     *                      zero. The right and bottom members contain the
     *                      width and height of the widget.
     *
     * @return None.
     */
    GVOID     GetClientRect(GRECT_T *prRect) const;
    
    /** 
     * @brief Sets the keyboard focus to the current widget. 
     *
     * @param None.
     *
     * @return If sets the key focus to the current widget successfully, return TRUE,
     *         otherwise return FLASE.
     */
    GBOOL     SetFocus(GVOID);
    /** 
     * @brief Sets the touch screen input capture to the current widget. This
     *              member function captures touch screen input when the touch
     *              screen was pressed while the touch event was over the capturing
     *              widget and the touch screen is still pressed. Only one widget at
     *              a time can capture the touch screen input.
     *
     * @param None.
     *
     * @return None.
     */
    GVOID     CaptureTouch(GVOID);
    /** 
     * @brief Releases the touch screen input capture from the current widget
     *              and restores normal touch screen input processing.
     *
     * @param None.
     *
     * @return None.
     */
    GVOID     ReleaseTouch(GVOID);

	GVOID     SetShowFlag(GVOID);
    
    /** 
     * @brief Sets the current widget to be visible.
     *
     * @brief @param None.
     *
     * @return None.
     */
    GVOID     Show(GVOID);

	GVOID     SetHideFlag(GVOID);

    /** 
     * @brief Sets the current widget to be invisible.
     *
     * @param None.
     *
     * @return None.
     */
    GVOID     Hide(GVOID);
    
    /** 
     * @brief Destroys the current widget.
     *
     * @param None.
     *
     * @return None.
     */
    GVOID     Destroy(GVOID);
    
    /** 
     * @brief Adds a rectangle to the current widget's update region. The update
     *        region represents the portion of the widget's client area that
     *        must be redrawn. 
     *
     * @param [in] prRect  Pointer to a GRECT_T structure that contains the client
     *                     coordinates of the rectangle to be added to the update
     *                     region. If this parameter is NULL, the entire client area
     *                     is added to the update region.
     * @param [in] fgErase Specifies whether the background within the update region
     *                     is to be erased when the update region is processed. If
     *                     this parameter is TRUE, the background is erased before the
     *                     EVT_GUI_NCPAINT and EVT_GUI_PAINT event is sent. If this
     *                     parameter is FALSE, the background remains unchanged.
     *
     * @return None.
     */
    GVOID     InvalidateRect(GRECT_T *prRect, GBOOL fgErase = TRUE);
    
    /** 
     * @brief Suspends or resume the painting for the current widget.
     *
     * @param [in] fgSuspend If this parameter is TRUE, suspends the painting
     *                       for the current widget, otherwise resume
     *                       the painting for the current widget.
     *
     * @return None.
     */
    GVOID     SuspendPaint(GBOOL fgSuspend);
    
    /** 
      * @brief Brings the current widget to the front of the z-order.
      *
      * @param None.
      *
      * @return None.
      */
    GVOID     BringToFront(GVOID);
    
    /** 
     * @brief Sends the specified event to the current widget. It does not return
     *        until the current widget has processed the event.
     *
     * @param [in] u4Event   Specifies the event to be sent.
     * @param [in] u4Param1  Specifies additional event-specific information.
     * @param [in] u4Param2  Specifies additional event-specific information.
     *
     * @return The return value specifies the result of the event processing. If
     *         the current widget process the event successfully, return TRUE,
     *         otherwise return FALSE.
     */
    GBOOL     SendEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    
    /** 
     * @brief Converts the client-area coordinates of a specified point to screen coordinates.
     *
     * @param [in/out] prPt  Pointer to a GPOINT_T structure that contains the client
     *                       coordinates to be converted. The new screen coordinates
     *                       are copied into this structure if the function succeeds. 
     *
     * @return None.
     */
    GVOID     ClientToScreen(GPOINT_T *prPt) const;
    
    /** 
     * @brief Converts the client-area coordinates of a specified rectangle to screen coordinates.
     *
     * @param [in/out] prRect  Pointer to a GRECT_T structure that contains the client
     *                         coordinates to be converted. The new screen coordinates
     *                         are copied into this structure if the function succeeds. 
     *
     *  @return None.
     */
    GVOID     ClientToScreen(GRECT_T *prRect) const;
    
    /** 
     *  change the point value from screen to client
     *
     *  @param  [in] prPt  point handle
     *
     *  @return NONE
     */
    GVOID     ScreenToClient(GPOINT_T *prPt) const;
    /** 
    *  change the rectangle value from screen to client
    *
    *  @param  [in] prRect  rectangle handle
    *
    *  @return NONE
    */
    GVOID     ScreenToClient(GRECT_T *prRect) const;
    /** 
    *  get the point value from point to widget
    *
    *  @param  [in] pWidget  the widget handle
    *
    *  @param  [in] prPt     the point handle
    *
    *  @return NONE
    */
    GVOID     MapPoint(GWidget *pWidget, GPOINT_T *prPt) const;
    /** 
    *  get the rectangle value from rectangle to widget
    *
    *  @param  [in] pWidget  the widget handle
    *
    *  @param  [in] prRect   the rectangle handle
    *
    *  @return NONE
    */
    GVOID     MapRect(GWidget *pWidget, GRECT_T *prRect) const;
    /** 
    *  get the client point value from point to widget
    *
    *  @param  [in] pWidget  the widget handle
    *
    *  @param  [in] prPt     the point handle
    *
    *  @return NONE
    */
    GVOID     MapClientPoint(GWidget *pWidget, GPOINT_T *prPt) const;
    /** 
    *  get the client rectangle value from rectangle to widget
    *
    *  @param  [in] pWidget  the widget handle
    *
    *  @param  [in] prRect   the rectangle handle
    *
    *  @return NONE
    */ 
    GVOID     MapClientRect(GWidget *pWidget, GRECT_T *prRect) const;


    virtual GWidget *TouchHitTest(GPOINT_T *prPt);
    /** 
    *  get the widget window GC
    *
    *  @return the GC handle
    */ 
    GHANDLE   GetWindowGC(GVOID);
    /** 
    *  get the widget client GC
    *
    *  @return the GC handle
    */ 
    GHANDLE   GetClientGC(GVOID);
    /** 
    *  release the GC
    *
    *  @param  [in] hGC  the widget GC
    *
    *  @return NONE
    */ 
    GVOID     ReleaseGC(GHANDLE hGC);
    
    /** 
     * @brief Sets the 32-bit application-specific value associated with the
     *        current widget.
     *
     * @param [in] uCustom4Data  A 32-bit value to be associated with the current widget.
     *
     * @return None.
     */ 
    GVOID     SetCustomData(GUINT32 uCustom4Data);
    
    /** 
     * @brief Retrieves the 32-bit application-specific value associated with
     *        the current widget.
     *
     * @param None.
     *
     * @return Return a 32-bit application-specific value associated with the
     *         current widget.
     */ 
    GUINT32   GetCustomData(GVOID) const;
    /** 
     * @brief Redraws the specified rectangle in the current widget.
     *
     * @param  [in] prUpdate  Pointer to a GRECT_T structure containing the
     *                        coordinates, in pixel units, of the update rectangle.
     * @param  [in] u4Flags   Specifies one or more redraw flags. This parameter
     *                        can be GWidget::RDF_ERASE, GWidget::RDF_INVALIDATE
     *                        and GWidget::RDF_NOCHILDREN.
     *
     * @return None.
     */ 
    GVOID     Redraw(GRECT_T *prUpdate, GUINT32 u4Flags = RDF_ERASE | 
                                                          RDF_INVALIDATE);
    /** 
    *  update widget
    *
    *  @param  [in] fgErase  true or false
    *
    *  @return NONE
    */ 
    GVOID     Update(GBOOL fgErase = TRUE);

    GVOID     EraseBkg(GRECT_T *prErase);

    GWidget*  GetPrevWidget(GVOID);
    GWidget  *GetNextWidget(GVOID);

    GVOID     Set5KeyOrder(GUINT16 u2Row, GUINT16 u2Col);

    GVOID    SetSurface(HSURFACE hSurf);
    HSURFACE GetSurface(GVOID);

    GBOOL    AddSlave(GWidget *pSlave);
    GVOID    RemoveSlave(GWidget *pSlave);
    GVOID    RemoveAllSlave(GVOID);

    GBOOL    SetPopup(GBOOL fgPopup);

    GVOID    SetDIAMethod(GDIAMETHOD_T eDIAMethod);
    GDIAMETHOD_T GetDIAMethod(GVOID);

    GBOOL     IsSuspendPaint(GVOID);

#if DDRAW_PANEL_MOVE
	GWidget* GetParent();

	GBOOL     ClearEffectArrayLen();

	int       GetEffectArrayLen();

	GINT32    SetEffectMode(HDC  hdc,int mode, int startx, int starty,int endx, int endy, int span,int timer,int i_HdcWith, int i_Height, int iShowLeft, int iShowTop);

	GEFFECTMOVEARRAY   aEffectShow[2];  //   widget 移动的描述数组
#endif 

    virtual  GBOOL IsContainer(GVOID);

	//mtk71372 2012.02.23 --------------------------------------------------------------- str
	#ifdef INVOKE_CALLBACK_FOR_DRAW_TEXT
	GVOID GDrawString(GHANDLE hGC, GINT32 i4X, GINT32 i4Y, GWCHAR *szString);		
	#endif
	//mtk71372 2012.02.23 --------------------------------------------------------------- end
	
    enum
    {
        WF_NOBKGND = ((GUINT32)1 << 16),/**< Widget no background*/
        WF_TABSTOP = ((GUINT32)1 << 17),/**< Focus can be moved to the widget by left&right key*/
        WF_CANFOCUSED = ((GUINT32)1 << 18),/**< Widget can be focused*/
        WF_NOTIFY = ((GUINT32)1 << 19),/**< widget event notify parent widget*/
        WF_BORDER = ((GUINT32)1 << 20),/**< border or not*/
        WF_OWNERDRAW = ((GUINT32)1 << 21),/**< app draw oneself*/
        WF_DISABLE = ((GUINT32)1 << 22),/**< reserved*/
        WF_TOUCHNOCHANGEFOCUS = ((GUINT32)1 << 23),/**< touch no change focus*/
        WF_POPUP = ((GUINT32)1 << 24), /**< pop menu container*/
        WF_MIGO5KEY = ((GUINT32)1 << 25),
        WF_NOTFLUSHSCREEN = ((GUINT32)1 << 26),
        WF_ALPHA = ((GUINT32)1 << 27),  /**< Widget Alpha Effect */
        WF_WITHALPHABKG = ((GUINT32)1 << 28), /**< Hold Widget Alpha Area */
        WF_NOAUTOHIDEPOPUP = ((GUINT32)1 << 29),
        WF_NOTALPHABLTIMAGE = ((GUINT32)1 << 30),
        WF_PRESSWITHBKG = ((GUINT32)1 << 31)
    }WIDGET_FLAG_E;
         
    enum
    {
        WF_EX_NONORMALBKGND = ((GUINT32)1 << 16),
		WF_EX_NOTSAVEBKGNDINSUF = ((GUINT32)1 << 17)
    };  

    /**Enum Values of Redraw Widget Flags*/
    enum
    {
        RDF_ERASE = 1 << 0,/**< erase flag*/
        RDF_INVALIDATE = 1 << 1,/**< invalidate flag*/
        RDF_NOCHILDREN = 1 << 2 /**< no children flag*/
    }RENDER_FLAG_E;

protected:
    virtual  GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    virtual  GVOID InternalRedraw(GRECT_T *prUpdate, GUINT32 u4Flags);
    virtual  GVOID InternalUpdate(GBOOL fgErase = TRUE);
#if DDRAW_PANEL_MOVE
	virtual  GVOID InternalUpdateWithOutShow(GBOOL fgErase = TRUE);
#endif 
    virtual  GVOID UpdateClientRect(GVOID);

    virtual  GVOID UpdateDataByWFlags(GUINT32 u4WFlags);

    GVOID   InternalRedrawEx(HSURFACE hSurf, GRECT_T *prUpdate, 
                             GUINT32 u4Flags);
    GVOID   InternalRedrawTop(HSURFACE hSurf, GRECT_T *prUpdate,
                              GUINT32 u4Flags);

    GVOID     Paint(GHANDLE hGC);
    GVOID     SetToplevel(GBOOL fgToplevel);
    GBOOL     IsPopup(GVOID);
    GBOOL     IsRealVisible(GVOID);
    GBOOL     NeedFlushScreen(GVOID);
    GBOOL     CanTabStop(GVOID);

    GVOID RemovePopup(GVOID);
    GVOID    HideAllPopup(GWidget *pLaunchWgt);
    //Widget's Internal Flags
    enum
    {
        WIF_TOPLEVEL = 1 << 16,
        WIF_POPUP = 1 << 17,
        WIF_CHILD = 1 << 18,
        WIF_DISABLED = 1 << 19,
        WIF_VISIBLE = 1 << 20,
        WIF_SUSPENDPAINT = 1 << 21,
        WIF_ONLYFREE = 1 << 22,
        WIF_FORBIDFREE = 1 << 23,
        WIF_DYNAMICALLOC = 1 << 24,
        WIF_ALLOCFROMPOOL = 1 << 25,
        WIF_ALLOCFROMOUT = 1 << 26,
        WIF_AUTODELETE = 1 << 27,
        WIF_NOTERASEPARENT = 1 << 28,
        WIF_DESTROY = 1 << 29,
        WIF_INSIDECONTAINER = 1 << 30
    };

    GUINT32        m_u4FiveKeyOrder;
    GUINT32        m_u4Type;
    GUINT32        m_u4ID;
    GUINT32        m_u4WFlags;
    GUINT32        m_u4WIFlags;
    GUINT32        m_u4WFlagsEx; 
    GRECT_T       m_rWindow;
    GRECT_T       m_rClient;
    GRECT_T       m_rUpdate;
    GWidget      *m_pParent;
    GWidget    *m_pOwner;
    GWidget      *m_pPrev;
    GWidget      *m_pNext;

#define MAX_SLAVE_NUM       5
    GWidget    *m_pSlaveWgts[MAX_SLAVE_NUM];
    GUINT8       m_uSlaveNum;

    GDIAMETHOD_T m_eDIAMethod;

    typedef struct _CACHEDDATA
    {
        GWidget *pWidget;
        GUINT32   u4WIFlags;
    } GCACHEDDATA_T;
    
    static GCACHEDDATA_T rCachedData;

private:
    GUINT32      m_u4CustomData;
    HSURFACE    m_hSurf;

    friend class GContainer;
    friend class GForm;
    friend class GTab;
    friend GWidget *GTouchHitTest(GPOINT_T *prPt);
    friend GBOOL     GDispatchEvent(GWidget *pWidget, GUINT32 u4Evt, 
                                   GUINT32 u4Param1, GUINT32 u4Param2);
    friend GHANDLE   GBeginPaint(GWidget *pWidget);
    friend GVOID     GEndPaint(GWidget *pWidget, GHANDLE hGC);
    friend GVOID     ScrollTextFunc(GVOID *pvParam);

#if DDRAW_PANEL_MOVE
	GBOOL   use_SurfaceFlag;        //   是否使用 widget surface 标志   
	GBOOL   gb_SelsectFlag;         //   是否选中标志
	GBOOL   gb_MoveCareFlag;        //   关心 move 消息标志: 除了 button 被选中的情况，其他情况下， button 不关心 move 事件
	int     m_iSlideDirection;      //   控件允许拖动的方向：GSLIDEX 允许 x 方向拖动; GSLIDEY,允许 y 方向拖动; GSLIDEXY  允许 x y方向拖动	
	GINT32  uCurEffectIndex;
#endif 
};

/**@}*/
#endif //_GWIDGET_H_
