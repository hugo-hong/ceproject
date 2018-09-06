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
 *   GRadioButton.h
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


#ifndef _GRADIOBUTTON_H_
#define _GRADIOBUTTON_H_


#include "GWidget.h"
#include "GGdi.h"
#include "GStyle.h"
#include "GStringData.h"


class GRadioGroup;

typedef struct _RADIOBUTTONSPECDATA
{
    GStringData        *m_szText;
    GRadioGroup    *group;
#if CFG_GUIKIT_NEW_STYLE
    GRADIOBUTTONSTYLE_T *prStyle;
#endif
} GRADIOBUTTONSPECDATA_T;

class GRadioButton : public GWidget
{
public:
    GRadioButton(GVOID);
    GRadioButton(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED | WF_TABSTOP,
                 GUINT32 u4ID = -1);
    virtual ~GRadioButton();

#if CFG_GUIKIT_NEW_STYLE
    GVOID    SetStyle(GRADIOBUTTONSTYLE_T *prStyle);
    GVOID    GetStyle(GRADIOBUTTONSTYLE_T *prStyle);
#endif
    GVOID    SetText(GTCHAR *szText);
    GVOID    SetText(GUINT16 u2StrID);
    GVOID    SetCheck(GBOOL ucCheck);
    GBOOL    IsCheck();

    inline GStringData     *GetText(GVOID)
    {
        return (&m_szText); 
    }
    inline GRadioGroup *GetRadioGroup(GVOID)
    {
        return (m_pGroup);
    }
    GVOID *operator new(size_t u4Size);
    GVOID operator delete(GVOID *pvObj);
    
    static GWidget *CreateInstance(GVOID *pvSpecData);

    // Widget Flag
    enum
    {
        WF_INPANEL = 1 << 0,
    };

protected:
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    GVOID Paint(GHANDLE hGC);

private:
    GStringData             m_szText;
    GBOOL                    m_fgCheck;
    GRadioGroup            *m_pGroup;
    GRADIOBUTTONSTYLE_T    *m_prStyle;

    GVOID        PaintCheck(GHANDLE hGC, GINT16 x1, GINT16 y1, GINT16 x2, GINT16 y2,
                           GPEN_T *prDark, GPEN_T *prLight, GPEN_T *prBlack, 
                           GBRUSH_T *prBlackBrush, GBOOL fgCheck);
    GVOID        EventCheck();
    GUINT16      SelectIcon();
    GUINT32      SelectTextColor();
    GUINT32      SelectFont();
    GBRUSH_T   *SelectBg();

    friend class GRadioGroup;
};


#endif //_GRADIOBUTTON_H_