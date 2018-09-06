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
/******************************************************************************
 *[File]             GCfg.h
 *[Version]          v1.0
 *[Revision Date]    2007-09-11
 *[Author]           Jau, mtk01843
 *[Description]      configurations for gbase
 ******************************************************************************/
#ifndef _GCFG_H_
#define _GCFG_H_

#define GB_NEW_INI             1

// 1(ON) : debug version, performance will be slower
// 0(OFF): real release version, will improve performance
#define GB_DEBUG               (_DEBUG)

/***************************************************************************/
/*                      GBASE  COMPILE OPTIONS                             */
/***************************************************************************/
#if GB_DEBUG
  #define GB_ASSERT_DEBUG      1
  #define GB_MEMORY_DIAGNOSE   1
  #define GB_LOGGER_ENABLE     1
  #define GB_CRTMEM_ADAPTOR    1
  #define GB_STACK_CHECK       1
  #define GB_MEM_CHECK         1

#else /* GB_DEBUG */
  #define GB_ASSERT_DEBUG      0
  #define GB_MEMORY_DIAGNOSE   0
  #define GB_LOGGER_ENABLE     0
  #if defined(WIN32)
    #define GB_CRTMEM_ADAPTOR    0  // PND : set to 0 will increase performance
  #endif
  #define GB_STACK_CHECK       0
  #define GB_MEM_CHECK         0
#endif


#define AVSSET                    1

#endif  // #ifndef _GCFG_H_

