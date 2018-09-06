/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by copyright and the information contained
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

/*******************************************************************************
*
* Filename:
* ---------
* file AVSwi_Err.h
*
* Project:
* --------
*   CNB
*
* Description:
* ------------
*
*
* Author: jian liang mtk40099
* -------
*
*
*------------------------------------------------------------------------------
* $Revision: #1 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/

#ifndef _AVSWI_ERR_H_
#define _AVSWI_ERR_H_


#define SWI_LOG_ERR(arg)			RETAILMSG(1, arg)
#define SWI_LOG_WRN(arg)			RETAILMSG(1, arg)
#define SWI_LOG_INFO(arg)			RETAILMSG(1, arg)
#define SWI_LOG_DBG(arg)			RETAILMSG(1, arg)
#define SWI_LOG_DUMP(arg)			RETAILMSG(1, arg)


/*************************************************************************
 * Error code structure :
 *
 * | 31(1 bit) |30-24(7 bits) | 23-16(8 bits) | 15-0(16 bits) |
 * | Indicator | Module ID    |   Extened     |     Code      |
 *
 * Indicator : 0 - success, 1 - error
 * Module ID : module ID, defined below
 *             only 7 bits, so the maximum ID value is 0x7FL !!!
 * Extended  : module dependent, but providee macro to add partial line info
 * Code      : is the module's status code
 *************************************************************************/
/* Composer for Module specific error code */
#define MAKE_GERR(mod, code)            \
    ((GUINT32)                          \
     ((GUINT32)(0x80000000) |           \
     (GUINT32)(((mod) & 0x7f) << 24) |  \
     (GUINT32)((code) & 0xffff))        \
    )


#define MOD_SWI          0x10L


typedef enum
{
    RET_SWI_OK  =  RET_OK,

    RET_SWI_INVALID_PARAM           = MAKE_GERR(MOD_SWI, 1),

    RET_SWI_DD_ERR                  = MAKE_GERR(MOD_SWI, 2),

    RET_SWI_NO_MEM                  = MAKE_GERR(MOD_SWI, 3),

    RET_SWI_NO_VDEC                 = MAKE_GERR(MOD_SWI, 4),

    RET_SWI_UNKNOWN_ERROR           = MAKE_GERR(MOD_SWI, 255),

} SWI_ERR_T;

#endif //_AVSWI_ERR_H_

