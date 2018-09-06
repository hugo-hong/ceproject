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
 *   GContainer.h
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


#ifndef _GCONTAINER_H_
#define _GCONTAINER_H_


#include "GWidget.h"
#include "GStyle.h"
#include <vector> //Yu.Mo 2011-10-13 Added
/**
* @defgroup groupContainer Container
* @ingroup groupWidget
* @{
*/

#define CFG_PANEL_DAMP_SLIDE 1
#define PANEL_MOVE_THREAD_METHOD //MTK70886, 2012-03-23

#define PANLE_SLIDE_HIDE_EFFECT 1

#define PANEL_SWITCH_EFFECT	//Yu.Mo 2011.10.11 added.

class GScrollBar;

typedef GBOOL (*PFN_WIDGETENUMCALLBACK)(GWidget* pWidget, GUINT32 u4Param);


class GContainer : public GWidget
{
public:
    GContainer(GVOID);
    GContainer(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED | WF_TABSTOP,
               GUINT32 u4ID = 0xFFFFFFFF);
    virtual ~GContainer();
    
    GBOOL Create(const GWGTTEMPLATE_T *prTemplate);

    virtual GBOOL InitComponent(GVOID *pvArg = NULL, GVOID *pvHistory = NULL);

    GBOOL        Add(GWidget *pChild, GBOOL fgUpdate = TRUE);
    GVOID        Remove(GWidget *pChild, GBOOL fgUpdate = TRUE);
    GVOID        RemoveAll(GBOOL fgUpdate = TRUE);
    GVOID        Delete(GWidget *pChild, GBOOL fgUpdate = TRUE);
    GVOID        DeleteAll(GBOOL fgUpdate = TRUE);

    GBOOL       AddDockChild(GWidget *pDockChild, GUINT32 u4ZOrder);
    GVOID       RemoveDockChild(GWidget *pDockChild);
    GVOID       RemoveAllDockChild(GVOID);

    GWidget    *GetFocusedChild(GVOID) { return (m_pFocused); }
    GVOID       SetFocusedChild(GWidget *pChild);
    GBOOL        EnumChild(PFN_WIDGETENUMCALLBACK pfnCallback, GUINT32 u4Param);
    GWidget    *GetChild(GUINT32 u4ID);
    GBOOL        RegChildNotify(GUINT32 u4ID, GObject *pReceiver,
                               PFN_MEMBERCALLBACK pfnNotify);
    GBOOL        RegChildNotify(GUINT32 u4ID, PFN_NORMALCALLBACK pfnNotify);
    GWidget    *GetNextTabItem(GBOOL fgPrevious);
    GWidget    *GetNextCanFocusItem();
    virtual GWidget *TouchHitTest(GPOINT_T *prPt);
    GBOOL        AutoChngFocus(GUINT32 u4Key, GINT32 i4Col = 0);
    GWidget    *FindWidgetByRowCol(GUINT16 u4Row, GUINT16 u4Col);
    GINT32       GetAddOrder(GWidget *pWidget);
    GWidget    *FindChildByOrder(GINT32 i4Order);

    GVOID        BringChildToTop(GWidget *pChild);
    //Reg child Callback
    GBOOL       RegChildCb(GObject *pReceiver, PFN_MEMBERCALLBACK pCbFunc, GUINT32 u4ID );
    GBOOL       RegChildCb(GObject *pReceiver, PFN_MEMBERCALLBACK pCbFunc);

    GVOID        SetScrollOrigin(GPOINT_T *prOrigin);
    GVOID        GetScrollOrigin(GPOINT_T *prOrigin);

    GVOID        SetScrollSize(GSIZE_T *prSize);
    GVOID        GetScrollSize(GSIZE_T *prSize);

    GVOID        SetBarWidth(GUINT8 uBarW);
    GUINT8       GetBarWidth(GVOID);

    enum
    {
        WF_CANSCROLL = 1 << 15,
        WF_HSCROLL = 1 << 14,
        WF_VSCROLL = 1 << 13,
        WF_AUTOHSCROLL = 1 << 12,
        WF_AUTOVSCROLL = 1 << 11,
        WF_TOUCHSCROLL = 1 << 10
    };

#ifdef CFG_GUIKIT_BACKUP_SURFACE
	GBOOL BackUpBgEnable(GVOID);
	HSURFACE GetBackupSurface(GVOID) {return m_hBackUpSurface;}
#endif

protected:
    virtual     GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    virtual     GBOOL IsContainer(GVOID);
    virtual     GVOID InternalRedraw(GRECT_T *prUpdate, GUINT32 u4Flags = RDF_ERASE);
    virtual     GVOID InternalUpdate(GBOOL fgErase = TRUE);

#if DDRAW_PANEL_MOVE
	virtual     GVOID InternalUpdateWithOutShow(GBOOL fgErase);
#endif 

    GWidget    *m_pFirstChild;
    GWidget    *m_pLastChild;
    GWidget    *m_pFocused;

    GPOINT_T    m_rScrollOrgin;
    GSIZE_T     m_rScrollSize;

    GScrollBar *m_pVBar;
    GScrollBar *m_pHBar;
    GUINT8       m_uBarW;
#ifdef CFG_GUIKIT_BACKUP_SURFACE
	HSURFACE m_hBackUpSurface;
#endif

private:
    GBOOL    IsShowVBar(GVOID);
    GBOOL    IsShowHBar(GVOID);
    GBOOL    OnVScroll(GWidget *pWidget, GUINT32 u4Evt, GUINT32 u4Param1,
                      GUINT32 u4Param2);
    GBOOL    OnHScroll(GWidget *pWidget, GUINT32 u4Evt, GUINT32 u4Param1,
                      GUINT32 u4Param2);

#define MAX_DOCKCHILD_NUM       5
    GWidget    *m_pDockWgts[MAX_DOCKCHILD_NUM];

#if CFG_PANEL_DAMP_SLIDE
    GTIMER_T         m_rDampSlideTimer;
    GUINT32           m_u4Direction;
    GUINT32           m_u4OffsetMax;
    GUINT32           m_u4OffsetCnt;
    GBOOL             m_fgTimerStart;
    GVOID GContainer::StartDampSlideTimer(GUINT32 u4Direction, GUINT32 u4Offset);
    GVOID GContainer::StopDampSlideTimer(GVOID);
    static GVOID GContainer::DampSlideTimerCb(GVOID *pvParam);
#endif
	
    friend class GWidget;
};

/**@}*/


/**
* @defgroup groupPanel Panel
* @ingroup groupContainer
* @{
*/


typedef struct _PANELSPECDATA
{
    GPOINT_T    rScrollOrigin;
    GSIZE_T     rScrollSize;
    GUINT32      u4BarW;
    GWGTTEMPLATE_T *prWgtTemplate;
    GPANELSTYLE_T *prStyle;
    GSCROLLSTYLE_T *prBarStyle;
} GPANELSPECDATA_T;


class GPanel : public GContainer
{
public:
    GPanel(GVOID);
    GPanel(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED | WF_TABSTOP,
           GUINT32 u4ID = 0xFFFFFFFF);
    virtual ~GPanel();

    GVOID *operator new(size_t u4Size);
    GVOID operator delete(GVOID *pvObj);

    GVOID   SetStyle(GPANELSTYLE_T *prStyle);
    GVOID   GetStyle(GPANELSTYLE_T *prStyle);

    static GWidget *CreateInstance(GVOID *pvSpecData);
    static GWidget *CreateInstanceEx(GRECT_T *prRect, GUINT32 u4WFlags,
                                     GUINT32 u4ID, GVOID *pvSpecData);

	GVOID  GotoNextPage(GVOID);

	GVOID  GotoPrePage(GVOID);

#if DDRAW_PANEL_MOVE
	GVOID   MovePanelAfterEffect(HWND m_hWnd);
	GVOID   ShowPanelAfterEffect(HWND m_hWnd);

	GINT32   GetPanelTotalPageNum(GVOID);                    //  获取 Panel 的总页数
	GINT32   GetPanelCurPageNum(GVOID);                      //  获取 Panel 的当前页
	GVOID   SetPanelCurPageNum(GINT32 iCurNum);              //  设置 Panel 的当前页

	GINT32   GetPanelPageWidth(GVOID);                       //  获取显示在屏上的 Panel 的宽度
	GVOID    SetPanelPageWidth(GINT32 iWidth);               //  设置显示在屏上的 Panel 的宽度

	GINT32   GetPanelPageHeight(GVOID);                      //  获取显示在屏上的 Panel 的高度
	GVOID    SetPanelPageHeight(GINT32 iHeight);             //  设置显示在屏上的 Panel 的高度
#endif 

#if PANLE_SLIDE_HIDE_EFFECT
	enum
	{
		PANEL_SHOW_HIDE_NORMAL,
		PANEL_SHOW_EFFECT_FLYIN_TOP,
		PANEL_SHOW_EFFECT_FLYIN_BOTTOM,
		PANEL_SHOW_EFFECT_FLYIN_LEFT,
		PANEL_SHOW_EFFECT_FLYIN_RIGHT,
		PANEL_HIDE_EFFECT_FLYOUT_TOP,
		PANEL_HIDE_EFFECT_FLYOUT_BOTTOM,
		PANEL_HIDE_EFFECT_FLYOUT_LEFT,
		PANEL_HIDE_EFFECT_FLYOUT_RIGHT
	};

static	GBOOL fgGetSyncEffectStatus(GVOID) {return m_SyncEffect;}
#endif



//Yu.Mo 2011.10.11 ---------------------------------------------------------------------------------- str
#ifdef PANEL_SWITCH_EFFECT
enum SwitchMode
{
	PANEL_SWITCH_LEFT_OUT_RIGHT_IN,
	PANEL_SWITCH_LEFT_IN_RIGHT_OUT,
};

struct SwitchProperty
{
	SwitchMode mode;  //切换的模式
	GPanel *pPanelOut; //切换出去的窗口
	GPanel *pPanelIn; //切换进来的窗口
	GINT32 iStep;     //步进的像素点
	GINT32 iInternal; //每次移动的时间间隔，因为CPU速度不同，可能会切换得太快或太慢，可以微调该数值
};
#endif //#ifdef PANEL_SWITCH_EFFECT


//Yu.Mo 2011.10.11 ---------------------------------------------------------------------------------- end

protected:
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);

private:
    GPANELSTYLE_T *m_prStyle;

#if DDRAW_PANEL_MOVE
	GINT32  iPanelShowX;              //   只支持 panel 对应整屏的移动，所以显示位置不变
	GINT32  iPanelShowY;              //   只支持 panel 对应整屏的移动，所以显示位置不变
	int  iPanelScrollOriginalX;
	int  iPanelScrollOriginalY;

	GINT32  iPanelTotalPageNum;          //   Panel 支持的总页数
	GINT32  iPanelCurPageNum;            //   Panel 当前页数
	int  iPanelPageWidth;             //   每页的宽度
	int  iPanelPageHeight;            //   每页的高度 
	int  iPanelActualMove;            //   Panel 实际的移动方向: > 0 向右； < 0 向左

	int  iPanelMoveDestX;             //   Panel 移动的目标位置X
	int  iPanelMoveDestY;             //   Panel 移动的目标位置Y
	int  iPanelMoveStartX;            //   Panel 移动的起点位置X
	int  iPanelMoveStartY;            //   Panel 移动的起点位置Y

	int  iMoveStartX;                 //   滑动起点 X
	int  iMoveStartY;                 //   滑动起点 Y  暂不支持 Y 方向滑动

	int  iPanelSelectOriginalX;       //   选中 panel 拖动时的起点
    //int  iPanelSletectReleaseX;       //   选中 panel 拖动释放时的终点
	int  iPanelSelectActualMoveX;     //   选中 panel 拖动的实际位移

	GBOOL  bHaveDownFlag;             //  曾 down 过
#endif 

#ifdef PANEL_MOVE_THREAD_METHOD
	HANDLE 	m_hGPanelMoveThread;
	DWORD	m_idThread;
	HANDLE	ms_hEventWakePanelMove;
	static DWORD	PanelMoveProc(VOID *pParam);
	int m_iMoveThreadPriority; //mtk71372 2012.08.22
#endif

#if PANLE_SLIDE_HIDE_EFFECT
	#define PANLE_MOVESTEP_X       18
	#define PANLE_MOVESTEP_Y       10
	#define PANLE_MOVE_LAYERID1  1
	#define PANLE_MOVE_LAYERID2  2
	#define PANLE_MOVE_LAYERID3  3
	#define PANLE_MOVE_LAYERID4  4

	GUINT32	m_i4MoveMode;
	GUINT32 	m_i4SlideStepX;
	GUINT32 	m_i4SlideStepY;
	GUINT32	m_u4PagePixelCnt;
	HSURFACE	m_hSlideSurf1;
	HSURFACE	m_hSlideSurf2;
	GUINT32	m_u4SurfW;
	GUINT32	m_u4SurfH;
	GBOOL	m_fgSlideTimerStart;
	GBOOL	m_fgEnableAfterMove;
	GUINT32	m_i4MoveLayer1ID;
	GUINT32	m_i4MoveLayer2ID;
	GTIMER_T	m_rMoveTimer;
	GUINT8	m_LayerGroupInUse;

	static	GBOOL	m_LayerGroupIsUsed[2];
	static	GUINT8	m_SyncEffect;

	static	GBOOL SetAllChildStyle(GWidget* pWidget, GUINT32 u4Param);
	static GVOID MoveTimerCb(GVOID *pvParam);
	GVOID OnHSlideStop(GBOOL fgRedraw);
#endif

	//Yu.Mo 2011.10.11 ---------------------------------------------------------------------------------- str
#ifdef PANEL_SWITCH_EFFECT
	#define PANLE_SWITCH_LAYERID_SCREEN  1
	#define PANLE_SWITCH_LAYERID_OUT  2
	#define PANLE_SWITCH_LAYERID_IN  3

	static GBOOL ms_bSwitchThrdRunning;
	static SwitchProperty ms_SwitchProperty;
	static HANDLE ms_hEventWakeSwitch;
	static CRITICAL_SECTION *ms_pcsPanelSwitch;

	static DWORD PanelSwitchProc(VOID *pParam);
	static HSURFACE CreateSurfaceToAttachLayer(GUINT32 u4LayerID,GUINT32 u4SurfW,GUINT32 u4SurfH);
	static void DeleteSurfaceAndDisattachLayer(HSURFACE hSurf,GUINT u4LayerID);

	//-------------------------------------------------------------------------------------------
	//Description:
	//	Copy the display data to surface
	//
	//Parameters:
	//	hSur : [in] The surface buffer for storing the data
	//	rRect : [in] The are for drawing.
	//	pvtVisible : [in] The visible panel to display
	//	pvtInvisible : [in] The visible panel to hide
	//-------------------------------------------------------------------------------------------
	static void CopyDisplayToSurface(HSURFACE hSur,GRECT_T rRect,std::vector<GPanel *> *pvtVisible,std::vector<GPanel *> *pvtInvisible);

public:
	//-------------------------------------------------------------------------------------------
	//Description:
	//	Get the optimizate value for SwitchProperty
	//
	//Parameters:
	//	prop :[out] The buffer for storing the value
	//  mode : [in] The switch mode
	//  pPanelOut : [in] The pointer to the panel-out
	//  pPanelOut : [in] The pointer to the panel-in
	//-------------------------------------------------------------------------------------------
	static void GetOptimizateSwitchProperty(SwitchProperty &prop,SwitchMode mode,GPanel *pPanelOut,GPanel *pPanelIn);

#endif //#ifdef PANEL_SWITCH_EFFECT

#ifdef PANEL_MOVE_THREAD_METHOD
	void SetMoveThreadPriority(int iThreadPriority)
	{
		m_iMoveThreadPriority = iThreadPriority;
	}
#endif
	//Yu.Mo 2011.10.11 ---------------------------------------------------------------------------------- end

};

/**@}*/
#endif //_GCONTAINER_H_