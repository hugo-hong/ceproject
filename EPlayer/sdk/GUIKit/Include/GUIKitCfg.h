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

//#define GUIKIT_INTERNALMEMORY_STATIC
//#define GUIKIT_INTERNALMEMORY_DYNAMIC
#define GUIKIT_EXTERNALMEMORY

// 1: Supported Animate Widget;
// 0: Don't Supported Animate Widget. 
#define GUIKIT_SUPPORTED_GANIMATE    1

#define GUIKIT_SUPPORTED_GDATECTRL   1
#define GUIKIT_SUPPORTED_GTIMECTRL   1
#define GUIKIT_SUPPORTED_GTABPAGE    1
#define GUIKIT_SUPPORTED_GPOPUPMENU  1
#define GUIKIT_SUPPORTED_GCOMBOBOX   1
#define GUIKIT_SUPPORTED_GPANEL      1

// 1: Enable Alpha for Widget.
// 0: Disable Alpha for Widget. 
#define CFG_GUIKIT_ALPHA_SUPPORTED 0


#include "GResource.h"

#define MAX_FORM_NUM            10 //20
#define MAX_BUTTON_NUM          35 //20
#define MAX_EDIT_NUM            20
#define MAX_COMBOBOX_NUM        5  //20
#define MAX_POPUPMENU_NUM       5  //20
#define MAX_STATIC_NUM          50 //20

#define MAX_TAB_NUM             20
#define MAX_SCROLLBAR_NUM       10 //20
#define MAX_SLIDER_NUM          10 //20
#define MAX_PROGRESSBAR_NUM     5  //20
#define MAX_RADIOBUTTON_NUM     15 //20
#define MAX_CHECKBUTTON_NUM     10 //20
#define MAX_TABPAGE_NUM         10 //20
#define MAX_RADIOGROUP_NUM      10 //20
#define MAX_DATECTL_NUM         5  //20
#define MAX_TIMECTL_NUM         5  //20

#define MAX_LV_NUM              5 //20
#define MAX_MV_NUM              5 //20

#if GUIKIT_SUPPORTED_GANIMATE
#define MAX_ANIMATE_NUM         5 //10
#endif