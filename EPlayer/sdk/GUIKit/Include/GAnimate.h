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
 *   GAnimate.h 
 *
 * Project:
 * --------
 *   GUIKit
 *
 * Description:
 * ------------
 *   Animate Widget.
 *   
 * Author:
 * -------
 *   mcn03011
 *
 * Last changed:
 * ------------- 
 * $Author: zeng.zhang $ 
 *
 * $Modtime: $  
 *
 * $Revision: #1 $
****************************************************************************/

#ifndef _GANIMATE_H
#define _GANIMATE_H

#include "GGdi.h"
#include "GWidget.h"
#include "GStyle.h"

typedef struct _GANIMATESPECDATA
{
    GUINT32      u4Interval;
    GUINT32      u4IconNum;
    GUINT32      *pu4IconArray;      
} GANIMATESPECDATA_T;

class GAnimate : public GWidget
{
public:
    GAnimate(GUINT32 u4Flags = WF_NOBKGND);
    GAnimate(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED | WF_TABSTOP, GUINT32 u4ID = 0xFFFFFFFF);
    virtual ~GAnimate(GVOID);

    GVOID *operator new(size_t u4Size);
    GVOID  operator delete(GVOID *pvObj); 

    GVOID  SetIconArray(GUINT32 *pu4IconArray, GUINT32 u4IconNum);
    GVOID  SetInterval(GUINT32 u4Interval);
    GVOID  ShowFrame(GINT32 i4FrameIndex);
    GVOID  Play(GVOID);
    GVOID  Stop(GVOID);
    GVOID  SetNCMode(GBOOL fgIsNCMode);

    static GWidget *CreateInstance(GVOID *pvSpecData);

    // Widget Flag
    enum
    {
        WF_UNFILLEDBG = 1 << 0, /* Unused background to recover animate frame. */
    };

protected:
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    GVOID    Paint(GHANDLE hGC);
    GVOID    NcPaint(GHANDLE hGC);
    GVOID    RestoreData(GHANDLE hGC, GBOOL fgUpdata);

private:
    GUINT32 *m_pu4IconArray;
    GUINT32  m_u4Interval;
    GINT32   m_i4FrameMax;
    GINT32   m_i4FrameIndex;
    GINT32   m_i4PrevFrame;
    HSURFACE  m_hSurfaceDst;
    GBOOL    m_fgShowOneFrame;
    GBOOL    m_fgIsPlay;
    GBOOL    m_fgIsNCMode;

    static GVOID PaintAnimate(GVOID *pvParam);
    GTIMER_T    m_rAnimateTimer;
    GBOOL        m_fgTimerIsCreated;
};

#endif /* _GANIMATE_H */