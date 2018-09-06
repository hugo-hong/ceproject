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
 *   GView.h
 *
 * Project:
 * --------
 *   GUIKit
 *
 * Description:
 * ------------
 *   GView manage the form info.
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


#ifndef _GVIEW_H_
#define _GVIEW_H_

#include "GObject.h"

class GForm;

/**
* @defgroup groupView View Manager
* @ingroup groupGUIKit
* @{
*/

/**GView Class  It is a base class to inherit */
class GView : public GCmdSource
{
public:
    /** 
    *  Get the  top level view handle
    *
    *  @return  the view handle
    */
    static inline GView *GetTopmost(GVOID) { return (pTopmost); }

    /** 
    *  Get the  special view handle
    *
    *  @param  [in] u4ID  the view ID
    *
    *  @return  the view handle
    */
    static GView *GetView(GUINT32 u4ID);

    /** 
    *  Go back the special form layer
    *
    *  @param  [in] u2Level  the level number that want to go back , -1 denote the top form
    *
    *  @return  NONE
    */
    static GVOID   GoBack(GUINT16 u2Level = 1, GBOOL fgRedraw = TRUE);

    /** 
    *  Go back the special form layer
    *
    *  @param  [in] pView  the view handle that want to go back 
    *
    *  @return  NONE
    */
    static GVOID   GoBack2(GView *pView, GBOOL fgRedraw = TRUE);

    /** 
    *  Go back the special form layer
    *
    *  @param  [in] u4ID  the view ID that want to go back 
    *
    *  @return  NONE
    */
    static GVOID   GoBack3(GUINT32 u4ID, GBOOL fgRedraw = TRUE);

    GView(GUINT32 u4ID);
    virtual ~GView(GVOID);

#if DDRAW_PANEL_MOVE
	virtual GVOID ShowPanelAfterMove(HWND m_hWnd);
#endif 

    GVOID *operator new(size_t u4Size);
    GVOID  operator delete(GVOID *pvObj);
    GVOID *operator new(size_t u4Size, GVOID *pvBuf);

    /** 
    *  The view entry ,it will call OnRealize ,OnInit, OnActivate function
    *
    *  @param  [in] pvArg  UI parameter 
    *
    *  @return  true or false
    */
    GBOOL   Entry(GVOID *pvArg, BOOL bShow=TRUE);
    
    /** 
    *  Exit view and delete form and view
    *
    *  @param  [in] fgRedraw  redraw the Activate form  
    *
    *  @return  NONE
    */
    GVOID   Exit(GBOOL fgRedraw = TRUE);

    /** 
    *  Show the view's UI
    *
    *  @return  true or false
    */
    GBOOL   Show(GVOID);

    /** 
    *  Hide the view's UI
    *
    *  @return  true or false
    */
    GBOOL   Hide(GVOID);

    /** 
    *  Get the active status
    *
    *  @return  true or false
    */
    GBOOL   IsActived(GVOID);

    /** 
    *  Get the visible status
    *
    *  @return  true or false
    */
    GBOOL   IsVisible(GVOID);

    /** 
    *  Add the view's info. 
    *
    *  @param  [in] pvHistory info. handle
    *
    *  @return  NONE
    */    
    GVOID   AddHistory(GVOID *pvHistory) { m_pvHistory = pvHistory; }

    /** 
    *  Get the view's info. 
    *
    *  @return  the view's info. handle
    */    
    GVOID  *GetHistory(GVOID) { return (m_pvHistory); }

    /** 
    *  Change the view ID
    *
    *  @param  [in] u4ID ID value
    *
    *  @return  NONE
    */    
    GVOID   SetID(GUINT32 u4ID) { m_u4ID = u4ID; }

    /** 
    *  Get the view's ID. 
    *
    *  @return  ID value
    */    
    GUINT32 GetID(GVOID) { return (m_u4ID); }

    /** 
    *  Get the view's form handle 
    *
    *  @return  the form handle
    */    
    GForm *GetForm(GVOID) { return (m_pAttached); }

    /** 
    *  release the view's form memory space
    *
    *  @return  true or false
    */    
    GBOOL   FreeForm(GVOID);

    /** 
    *  Send the view's event
    *
    *  @param  [in] pReceiver the take over view
    *
    *  @param  [in] u4Evt the event ID
    *
    *  @param  [in] u4Param the event parameter
    *
    *  @return  true or false
    */    
    GBOOL   SendEvent(GView *pReceiver, GUINT32 u4Evt, GUINT32 u4Param, GUINT32 u4Param1 = NULL);
    
    /** 
    *  Create UI in here, It should be overload
    *
    *  @return  the form handle
    */    
    virtual GForm *OnRealize(GVOID)
    {
        return (NULL);
    }

    /** 
    *  Init UI in here, It should be overload
    *
    *  @param  [in] pvArg  UI init parameter
    *
    *  @param  [in] pvHistory  history handle
    *
    *  @return  true or false
    */    
    virtual GBOOL   OnInit(GVOID *pvArg, GVOID *pvHistory) { return (TRUE); }
    // should to activate on the UI top VIEW
    /** 
    *  It will be call When UI activate, It should be overload
    *
    *  @return  NONE
    */    
    virtual GVOID   OnActivate(GVOID) {}
    //UN  ACTIVATE, should to activate the view's next view.
    /** 
    *  It will be call When UI deactivate, It should be overload
    *
    *  @return  NONE
    */    
    virtual GVOID   OnDeactivate(GVOID) {}

    /** 
    *  It will be call When UI entry, It should be overload
    *
    *  @param  [in] pvArg UI parameter
    *
    *  @return  NONE
    */    

    virtual GVOID   OnEntry(GVOID *pvArg) {}
	/** 
	*  It will be call When UI entry, It should be overload
	*
	*  @param  [in] pvArg user parameter
	*
	*  @return  NONE
	*/    
	virtual GBOOL   OnUserInit(GVOID *pvArg) {return (TRUE); }

    /** 
    *  It will be call When UI exit, It should be overload
    *
    *  @return  NONE
    */    
    virtual GVOID   OnExit(GVOID);

    /** 
    *  It will be call When event exist, It should be overload
    *
    *  @param  [in] pSender the view handle
    *
    *  @param  [in] u4Evt   the event ID
    *
    *  @param  [in] u4Param the event param
    *
    *  @return  true or false
    */    
    virtual GBOOL   OnEvent(GView *pSender, GUINT32 u4Evt, GUINT32 u4Param, GUINT32 u4Param1)
    { 
        return (FALSE);
    }

	GBOOL   GBlackUILayer(GForm *pForm);

protected:
    
    typedef struct _CACHEDDATA
    {
        GView   *pView;
        GUINT32   u4VIFlags;
    } GCACHEDDATA_T;
    
    static GCACHEDDATA_T rCachedData;

    GUINT32       m_u4ID;
    //VIEW INTERNAL FLAG,
    GUINT32       m_u4VIFlags;
    GForm       *m_pAttached;
    GVOID        *m_pvHistory;

private:
    GBOOL   IsExist(GVOID);
    GVOID   BringToTop(GVOID);
    GVOID   Push(GVOID);
    GVOID   Pop(GVOID);
    GVOID   AddToNoEntry(GVOID);
    GVOID   RemoveFromNoEntry(GVOID);
    GVOID   AdjustPrevNextForm(GVOID);
  public:
    GBOOL   Resume(GVOID);
    
private:
    static GView *pLastNoEntry;
    static GView *pTopmost;
    static GView *pActived;

    static GView *pTopmostEx;
    static GView *pTopmostEx2;

    enum
    {
        //VISIBLE FLAG
        VIF_VISIBLE = 1 << 16,
        //
        VIF_ENTRY = 1 << 17,
        //EXIT NO REDRAW
        VIF_EXITNOREDRAW = 1 << 18,
        //dynami calloc
        VIF_DYNAMICALLOC = 1 << 19,
        //alloc from pool
        VIF_ALLOCFROMPOOL = 1 << 20,
        //alloc from ui point
        VIF_ALLOCFROMOUT = 1 << 21,
        //DESTRUCT
        VIF_INDESTRUCT = 1 << 22
    };

    GView *m_pPrev;
    GView *m_pNext;
    GForm *m_pPrevForm;
    GForm *m_pNextForm;

    friend class GWidget;
    friend class GForm;

    friend GBOOL GInitViewClass(GVOID *pvArg);
    friend GVOID GDeinitViewClass(GVOID);

    friend GVOID GSetGUIKitCooperativeMode(GUINT8 uCoopMode);
    friend GBOOL GBroadcastEvent(GUINT32 u4Evt, GUINT32 u4Param1, 
                                 GUINT32 u4Param2);
};


/*@}*/ //end of View


#endif //_GVIEW_H_