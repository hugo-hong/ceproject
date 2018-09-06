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
 *   GProgressBar.h
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


#ifndef _GPROGRESSBAR_H_
#define _GPROGRESSBAR_H_


#include "GWidget.h"
#include "GGdi.h"
#include "GStyle.h"

typedef struct _GPROGRESSSPECDATA
{
    //GINT16 m_i2Minimum;
    //GINT16 m_i2Maximum;
    //GINT16 m_i2CurValue;
    GBOOL  m_fgOrientation;
#if CFG_GUIKIT_NEW_STYLE
    GPROGRESSSTYLE_T *prStyle;
#endif
} GPROGRESSSPECDATA_T;

class GProgressBar : public GWidget
{
public:
    GProgressBar(GVOID);
    GProgressBar(GRECT_T *prRect, GUINT32 u4WFlags = 0,
                 GUINT32 u4ID = 0xFFFFFFFF, GBOOL fgOrientation = HORIZONTAL);
    virtual ~GProgressBar();

#if CFG_GUIKIT_NEW_STYLE
    GVOID SetStyle(GPROGRESSSTYLE_T *prStyle);
    GVOID GetStyle(GPROGRESSSTYLE_T *prStyle);
#endif

    GVOID SetMinimum(GINT16 i2Minimum);
    GVOID SetMaximum(GINT16 i2Maximum);
    GVOID SetCurValue(GINT16 i2CurValue);
    GVOID SetPaintString(GBOOL fgPaintString);
    GVOID SetPaintBorder(GBOOL fgPaintBorder);
    GVOID SetOrientation(GBOOL fgOrientation);

    GBOOL  GetOrientation();
    GBOOL  GetPaintString();
    GBOOL  GetPaintBorder();
    GINT16 GetMinimum();
    GINT16 GetMaximum();
    GINT16 GetCurValue();
    
    enum
    {
        VERTICAL = FALSE,
        HORIZONTAL = TRUE
    };

    GVOID *operator new(size_t u4Size);
    GVOID operator delete(GVOID *pvObj);
    
    static GWidget *CreateInstance(GVOID *pvSpecData);

protected:
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    GVOID Paint(GHANDLE hGC);

private:
    GINT16 m_i2Minimum;
    GINT16 m_i2Maximum;
    GINT16 m_i2CurValue;
    GBOOL  m_fgOrientation;
    GBOOL  m_fgPaintString;
    GBOOL  m_fgPaintBorder;

    GPROGRESSSTYLE_T *m_prStyle;

    GVOID GProgressBar::PaintBorder(GHANDLE hGC, GINT16 i2Width, GINT16 i2Height,
        GPEN_T *prDark, GPEN_T *prDisable);
};

#endif //_GPROGRESSBAR_H_