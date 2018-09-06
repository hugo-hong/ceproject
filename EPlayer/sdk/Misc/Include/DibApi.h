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
 *   DibApi.h
 *
 * Project:
 * --------
 *   SDK
 *
 * Description:
 * ------------
 *
 *
 *
 * Author:
 * -------
 *   Daowen Deng, MTK40043
 *
 *------------------------------------------------------------------------------
 * $Revision: #1 $
 * $Modtime:$
 * $Log:$
 *
 *******************************************************************************/

#ifndef _DIBAPI_H_
#define _DIBAPI_H_

#define NEW_LOAD_IMG          1

// DIB Handle(DIB: Device-Independent Bitmap)
DECLARE_HANDLE(HDIB);

// DIB Header flag(String "BM")
#define DIB_HEADER_MARKER     ((WORD) ('M' << 8) | 'B')

// Check DIB is Win 3.0 or not.
#define IS_WIN30_DIB(lpbi)    ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))

// DIB constant
#define PALVERSION            0x300

// Calculate width of rectangle area.
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)

// Calculate high of rectangle area.
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)


#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief 
 *
 * @param
 *
 * @return 
 *
 * @note
 */
#if !NEW_LOAD_IMG
HBITMAP CreateBitmapFromBuf(GLPCSTR pDIBBits, GDWORD dwBitsSize);
#else
HBITMAP CreateBitmapEx(GLPCSTR pDIBBits, GDWORD dwBitsSize);
#endif
#ifdef __cplusplus
}
#endif

#endif // #ifndef _DIBAPI_H_
