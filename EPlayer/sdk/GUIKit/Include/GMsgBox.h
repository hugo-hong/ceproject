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
 *   GMsgBox.h
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

#ifndef _MSGBOX_H
#define _MSGBOX_H

#define CFG_GUIKIT_MSGBOX_SUPPORTED 0

#include "GStringData.h"
#include "GWidget.h"

#define MSGB_CANCELTRYCONTINUE    0x00000001L /**< The message box contains three push buttons: Abort, Retry, and Continue. */
#define MSGB_OK                   0x00000002L /**< The message box contains one push button: OK. This is the default. */
#define MSGB_OKCANCEL             0x00000003L /**< The message box contains two push buttons: OK and Cancel. */
#define MSGB_RETRYCANCEL          0x00000004L /**< The message box contains two push buttons: Retry and Cancel. */
#define MSGB_YESNO                0x00000005L /**< The message box contains two push buttons: Yes and No. */
#define MSGB_YESNOCANCEL          0x00000006L /**< The message box contains three push buttons: Yes, No, and Cancel. */


#define MSGB_ICONWARNING      0x00000100L /**< An exclamation-point icon appears in the message box. */
#define MSGB_ICONINFORMATION  0x00000200L /**< An icon consisting of a lowercase letter i in a circle appears in the message box. */
#define MSGB_ICONQUESTION     0x00000300L /**< A question-mark icon appears in the message box. */
#define MSGB_ICONSTOP         0x00000400L /**< A stop-sign icon appears in the message box. */


#define MSGB_DEFBUTTON1     0x00010000L /**< The first button is the default button. Default setting.*/
#define MSGB_DEFBUTTON2     0x00020000L /**< The second button is the default button. */
#define MSGB_DEFBUTTON3     0x00030000L /**< The third button is the default button. */


#define MSGB_ERROR       0x0000L /**< Something wrong happened. */
#define MSGB_IDABORT     0x0001L /**< Abort button was selected.*/
#define MSGB_IDCANCEL    0x0002L /**< Cancel button was selected. */
#define MSGB_IDCONTINUE  0x0003L /**< Continue button was selected. */
#define MSGB_IDNO        0x0004L /**< No button was selected. */
#define MSGB_IDOK        0x0005L /**< OK button was selected. */
#define MSGB_IDRETRY     0x0006L /**< Retry button was selected. */
#define MSGB_IDTRYAGAIN  0x0007L /**< Try Again button was selected. */
#define MSGB_IDYES       0x0008L /**< Yes button was selected. */
#define MSGB_TIMEOUT     0x0009L /**< Time out. No button was selected. */


typedef GVOID (*PFN_MSGBOXPROC)(GINT16 i2Sel);
typedef GVOID (*PFN_MSGBOXPROC_EX)(GINT16 i2Sel, GUINT32 u4Param);

#if CFG_GUIKIT_MSGBOX_SUPPORTED

GVOID GMsgBox(GWidget *prWgt, GStringData *pstrCaption, GStringData *pstrText, 
            GUINT32 u4Type, GUINT32 u4Wait = 0, PFN_MSGBOXPROC pfnNotify = NULL);

GVOID GMsgBoxEx(GWidget *prWgt, GStringData *pstrCaption, GStringData *pstrText, 
               GUINT32 u4Type, GUINT32 u4Wait = 0, PFN_MSGBOXPROC_EX pfnNotify = NULL, GUINT32 u4Param = -1);

GBOOL GMsgBoxIsShow(GVOID);

GBOOL GMsgBoxInit(GVOID);

GVOID GMsgBoxDeinit(GVOID);

#endif  /* CFG_GUIKIT_MSGBOX_SUPPORTED */

#endif /* _MSGBOX_H */