/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008 MediaTek Inc.
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
 *   GEvt.h 
 *
 * Project:
 * --------
 *   GPS Pilot
 *
 * Description:
 * ------------
 *   
 *   
 * Author:
 * -------
 *   mcn03001
 *
 * Last changed:
 * ------------- 
 * $Author: zeng.zhang $ 
 *
 * $Modtime: $  
 *
 * $Revision: #1 $
****************************************************************************/

#ifndef _GEVTDEF_H_
#define _GEVTDEF_H_


/**
 * Event ID (GUINT32) layout
 */

// generic events begins from following ID
#define EVT_GEN_FIRST       0x00L   /* 0x00 ~ 0xFFF */

// GPS events begins from following ID
#define EVT_GPS_FIRST       0x1000L /* 0x1000 ~ 0x1FFF */

// GUI Kit events begins from following ID
#define EVT_GUI_FIRST       0x3000L /* 0x3000 ~ 0x3FFF */

// UI/APP events begins from flollowing ID
#define EVT_APP_FIRST       0x4000L /* 0x4000 ~ 0x4FFF */

// #define .... 
// PLEASE ADD YOUR EVT IF NEEDED

// User events begins from following ID
#define EVT_USR_FIRST       0x8000L   /*  */


/* !!! module ID has only 7 bits, so the maximum ID value is 0x7FL !!! */


/**
 *
 * Define generic events
 *
 */
#define EVT_KEYDOWN          (EVT_GEN_FIRST + 0x01L)
#define EVT_KEYUP            (EVT_GEN_FIRST + 0x02L)
#define EVT_KEYLONGPRESS     (EVT_GEN_FIRST + 0x03L)
#define EVT_KEYREPEAT        (EVT_GEN_FIRST + 0x04L)
#define EVT_TOUCHPRESS       (EVT_GEN_FIRST + 0x05L)
#define EVT_TOUCHMOVE        (EVT_GEN_FIRST + 0x06L)
#define EVT_TOUCHRELEASE     (EVT_GEN_FIRST + 0x07L)
#define EVT_TOUCHREPEAT      (EVT_GEN_FIRST + 0x08L)
#define EVT_TOUCH_LONG_PRESS (EVT_GEN_FIRST + 0x09L)
#define EVT_TOUCH_DB_CLICK   (EVT_GEN_FIRST + 0x0AL)
#define EVT_TOUCH_SLIDE      (EVT_GEN_FIRST + 0x0BL)

#define EVT_RUNGIS           (EVT_GEN_FIRST + 0x0CL)
#define EVT_CHANGESOLUTION   (EVT_GEN_FIRST + 0x0EL)
//\\ Nick Fatal Begin
#define EVT_HOTPLUGOUT       (EVT_GEN_FIRST + 0x0DL)
//\\ Nick Fatal End

#define EVT_NOKEY       (EVT_GEN_FIRST + 0x100L)
#define EVT_DVDCMD       (EVT_GEN_FIRST + 0x101L)
#define EVT_UARTOK    (EVT_GEN_FIRST + 0x102L)
#define EVT_DVDDISPLAY    (EVT_GEN_FIRST + 0x102L)

#endif // _GEVTDEF_H_

