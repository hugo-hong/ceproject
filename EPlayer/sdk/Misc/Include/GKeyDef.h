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
 *   GKeyDef.h 
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

#ifndef _GKEYDEF_H_
#define _GKEYDEF_H_


/* Note: Key definition is platform dependent! */

enum 
{
    GK_0 = 0,
    GK_1,                
    GK_2,                
    GK_3,                
    GK_4,                
    GK_5,                
    GK_6,                
    GK_7,                
    GK_8,                
    GK_9,                
    GK_LEFT,        
    GK_UP,            
    GK_RIGHT,       
    GK_DOWN,             
    GK_ENTER,
    GK_CLEAR,
    GK_LSK,
    GK_RSK,
    GK_STAR,
    GK_POUND,
    GK_VOL_UP,
    GK_VOL_DOWN,
    GK_CALL,
    GK_END,     //\\ Jau : add for 252->266
    GK_POWER,
    GK_POSITION,
    GK_ZOOMIN,
    GK_ZOOMOUT,
    GK_INFO,
    GK_ESCAPE,
    GK_ROOT,
    GK_F1,
    GK_F2,
    GK_F3,
    GK_F4,
    GK_F5,
    GK_F6,
    GK_MAX_KEYS,            
    GK_INVALID = 0xFE
};

#endif  // _GKEYDEF_H_
