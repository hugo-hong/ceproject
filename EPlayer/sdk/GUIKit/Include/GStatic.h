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
 *   GStatic.h
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

#ifndef _GSTATIC_H
#define _GSTATIC_H

#include "GGdi.h"
#include "GWidget.h"
#include "GStyle.h"
#include <vector>


#define STF_ICON        0x0001
#define STF_CENTER      0x0002
#define STF_RIGHT       0x0004
#define STF_MULTILINE   0x0008
#define STF_SCROLLTEXT  0x0010

typedef struct _GSTATICSPECDATA
{
    GUINT32          u2IconID;
    GStringData     *szText;        
#if CFG_GUIKIT_NEW_STYLE
    GSTATICSTYLE_T  *prStyle;
#endif
} GSTATICSPECDATA_T;


class GStatic : public GWidget
{
public:
    GStatic(GUINT32 u4Flags = WF_NOBKGND);
    GStatic(GRECT_T *prRect, GUINT32 u4WFlags = WF_CANFOCUSED | WF_TABSTOP,
              GUINT32 u4ID = 0xFFFFFFFF);
    virtual ~GStatic(GVOID);

    GVOID SetStyle(GSTATICSTYLE_T *prStyle);
    GVOID GetStyle(GSTATICSTYLE_T *prStyle);
    GVOID SetText(GTCHAR *szText);
    GVOID SetText(GStringData *szText);
    GVOID SetText(GUINT32 u4StrID);
    GVOID GetText(GTCHAR *szText);
    GVOID SetIcon(GUINT16 u2IconID);
    GVOID SetIcon(GTCHAR *szIconName);
    GVOID SetTextAndIcon(GTCHAR *szText, GUINT16 u2IconID);
	GVOID SetTextAndIcon(GUINT32 u4StrID, GUINT16 u2IconID);
	GINT32 GetIconToTextSpan(GVOID);
	GVOID  SetIconToTextSpan(GINT32 iSpan);

	GBOOL StopScrollText(GVOID);

	GBOOL StartScrollText(GVOID);

    inline GUINT16 GetIcon(GVOID)
    {
        return (m_u2IconID);
    }

	inline GVOID SetTextAlign(GUINT16 m_iAlign)
	{
		if ((m_iAlign <= ITEM_TEXT_RIGHT) && (ITEM_TEXT_LEFT<=m_iAlign))
		{
			m_TextAlign = m_iAlign;
		}
		else
		{
			m_TextAlign = ITEM_TEXT_LEFT;
		}
	}

	inline GUINT16 GetItemTextAlign(GVOID)
	{
		return m_TextAlign;
	}

    GVOID *operator new(size_t u4Size);
    GVOID  operator delete(GVOID *pvObj); 
    static GWidget *CreateInstance(GVOID *pvSpecData);

	//mtk71372 2012.02.07 added  --------------------------------------- str
	inline GVOID SetTextVerticalCenter(BOOL bVerticalCenter)
	{
		m_bTxtVerticalCenter = bVerticalCenter;
	}

	inline GVOID SetGapBetweenLineForMultiLine(GINT32 u4Gap)
	{
		m_u4TxtGapBetweenLineForMultiLine = u4Gap;
	}
	//mtk71372 2012.02.07 added  --------------------------------------- end

protected:
    virtual GBOOL HandleEvent(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);
    GVOID    Paint(GHANDLE hGC);
    
private:
	GVOID DrawMultilineString(GHANDLE hGC, GRECT_T *prRect, const GTCHAR *szTxt, GUINT32 u4Flags); //mtk71372 2012.02.07 added 
#if 0
    GVOID GetStartPt(GUINT32 u4Flags, GRECT_T *prRect, 
                    GSIZE_T *prItem, GPOINT_T *prStPt);
#endif

	std::vector<GTCHAR> m_vtText;
    GUINT32         m_u4TextID;
    GSTATICSTYLE_T  *m_prStyle;
    GUINT16          m_u2IconID;
    GTCHAR          *m_szIconName;
    GINT8            m_iScrID;
    GUINT32          m_iIconToTextSpan;
	GUINT32          m_TextAlign;
	//mtk71372 2012.02.07 added  --------------------------------- str
	BOOL			m_bTxtVerticalCenter; 
	GINT32			m_u4TxtGapBetweenLineForMultiLine;
	//mtk71372 2012.02.07 added ---------------------------------- end
};
#endif