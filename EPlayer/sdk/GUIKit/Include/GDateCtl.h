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
 *   GDateCtl.h
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


#ifndef _GDATECTL_H_
#define _GDATECTL_H_


#include "GWidget.h"
#include "GComboBox.h"
#include "GContainer.h"
#include "GButton.h"

class GDateCtl : public GContainer
{
public:
    GDateCtl(GVOID);
    GDateCtl(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED | WF_TABSTOP,
             GUINT32 u4ID = 0xFFFFFFFF);
    virtual ~GDateCtl();

#if CFG_GUIKIT_NEW_STYLE
    GVOID SetStyle(GDATECTRLSTYLE_T *prStyle);
    GVOID GetStyle(GDATECTRLSTYLE_T *prStyle);
#endif

    GINT16 GetYear() const;
    GINT16 GetMonth() const;
    GINT16 GetDay() const;
    GVOID  SetYear(GINT16 i2Y);
    GVOID  SetMonth(GINT16 i2M);
    GVOID  SetDay(GINT16 i2D);
    
    GVOID *operator new(size_t u4Size);
    GVOID operator delete(GVOID *pvObj);
    
    static GWidget *CreateInstance(GVOID *pvSpecData);

protected:
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    GVOID Paint(GHANDLE hGC);
#if CFG_GUIKIT_NEW_STYLE
    GVOID ShowFinetuneButton(GINT16 u2Index);
    GVOID HideFinetuneButton(GVOID);
    GBOOL FinetuneBtnCb(GCmdSource *pobjSender,
                       GUINT32      u4Event, 
                       GUINT32      u4Param1,
                       GUINT32      u4Param2);
#endif

private:
    GComboBox  m_year;
    GComboBox  m_month;
    GComboBox  m_day;
#if CFG_GUIKIT_NEW_STYLE
    GButton    *m_prBtnUp;
    GButton    *m_prBtnDown;
    GUINT16     m_u2BtnH;
    GUINT16     m_u2CurIndex;
    GDATECTRLSTYLE_T *m_prStyle;
    GUINT16     m_u2BoxGap; // For CustomTool only!!
#endif
    GINT16      m_y;
    GINT16      m_m;
    GINT16      m_d;
    GTCHAR      m_buf[100*(4+1) + 12*(2+1) + 31*(2+1)];

    GVOID setDate(GBOOL fgInit);
    GBOOL CallBack(GCmdSource *pobjSender,
        GUINT32      u4Event, 
        GUINT32      u4Param1,
        GUINT32      u4Param2);

    static const GINT16 m_num_days[];
    static const GINT16 m_leap_num_days[];

};

typedef struct
{
    GINT16      m_y;
    GINT16      m_m;
    GINT16      m_d;
    GDATECTRLSTYLE_T *prStyle;
} GDATECTLSPECDATA_T;
#endif //_GDATECTL_H_