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
 *[File]             GLog.h
 *[Version]          v1.0
 *[Revision Date]    2007-09-11
 *[Author]           Jau, mtk01843
 *[Description]      export definitions and interfaces of log module
 ******************************************************************************/
#ifndef GBASE_LOG_H
#define GBASE_LOG_H

#include "GDef.h"


#ifdef __cplusplus
  extern "C" {
#endif

/*************************************************************************
 *             GBASE LOG CONSTANT DEFINITIONS
 *************************************************************************/
/* LOG TAG FOR EACH MODULE */
typedef enum
{
    LOG_MOD_NONE        = 0x0001,   /* "  " */
    LOG_MOD_GIS         = 0x0002,   /* "GIS" : GIS engine */
    LOG_MOD_KAL         = 0x0004,   /* "KAL" : Kernel Abstraction Layer */
    LOG_MOD_DB          = 0x0008,   /* "DB"  : map DB */
    LOG_MOD_DISP        = 0x0010,   /* "MD"  : Map Display */
    LOG_MOD_MM          = 0x0020,   /* "MM"  : Map Matching */
    LOG_MOD_SRCH        = 0x0040,   /* "MS"  : Map Search */
    LOG_MOD_RP          = 0x0080,   /* "RP"  : Route Planning */
    LOG_MOD_RG          = 0x0100,   /* "RG"  : Route Guidance */
    LOG_MOD_UT          = 0x0200,   /* "UT"  : Ulities */
    LOG_MOD_TK          = 0x0400,   /* "TK"  : Tool Kits*/
    LOG_MOD_GUIK        = 0x0800,   /* "GUI"  : Gui Kits */
    LOG_MOD_APP         = 0x1000,   /* "APP"  : UI Application */
    LOG_MOD_GPS         = 0x2000,   /* "GPS"  : GPS signal related */
    LOG_MOD_ALL         = 0xFFFF    /* log all module */
} LOG_MOD_ID_T;

#define MIGO_MOD_GIS    LOG_MOD_GIS    /* GIS engine */
#define MIGO_MOD_KAL    LOG_MOD_KAL    /* Kernel Abstraction Layer */
#define MIGO_MOD_DB     LOG_MOD_DB     /* map DB */
#define MIGO_MOD_DISP   LOG_MOD_DISP   /* Map Display */
#define MIGO_MOD_MM     LOG_MOD_MM     /* Map Matching */
#define MIGO_MOD_SRCH   LOG_MOD_SRCH   /* Map Search */
#define MIGO_MOD_RP     LOG_MOD_RP     /* Route Planning */
#define MIGO_MOD_RG     LOG_MOD_RG     /* Route Guidance */
#define MIGO_MOD_UT     LOG_MOD_UT     /* Ulities */
#define MIGO_MOD_TK     LOG_MOD_TK     /* Tool Kits*/

/*************************************************************************
 *             GBASE LOG CONSTANT DEFINITIONS
 *************************************************************************/
/* LOG LEVEL */
#define LOG_LVL_ALL        0xFF
#define LOG_LVL_FATAL      0x40
#define LOG_LVL_FORCE      0x20
#define LOG_LVL_ERROR      0x10     /* default */
#define LOG_LVL_WARN       0x08
#define LOG_LVL_DEBUG      0x04
#define LOG_LVL_INFO       0x02
#define LOG_LVL_BASE       0x01


/* LOG OUTPUT METHOD */
#define LOG_OP_NONE           0x00
#define LOG_OP_TST         0x01     /* NU/MAUI platform trace system  */
#define LOG_OP_CONSOLE     0x02     /* WIN32  platform console window */
#define LOG_OP_FILE        0x04     /* log to file, no matter which platform */
#define LOG_OP_ASYNC       0x08     /* disable */
#define LOG_OP_MEMORY      0x10     /* disable */

/* LOG LAYOUT TYPE */
#define LOG_LAYOUT_TEXT       1     /* default */
#define LOG_LAYOUT_HTML       2     /* not support */
#define LOG_LAYOUT_XML        3     /* not support */

/*************************************************************************
 *                 GBASE LOG BASIC INTERFACES
 *************************************************************************/
/* Init/Deinit Logger Service */
GAPI GVOID GInitLog(GVOID);
GAPI GVOID GInitLogEx(GWCHAR *wszLogFilePath);
GAPI GVOID GDeinitLog(GVOID);

/*************************************************************************
 *             GBASE LOG CONFIGURATION INTERFACES
 *************************************************************************/
/* Setting/Enable/Disable Logger mode */
#define LOG_ENABLE_FATAL()      LOG_SetLevel(LOG_LVL_FATAL, TRUE)
#define LOG_ENABLE_FORCE()      LOG_SetLevel(LOG_LVL_FORCE, TRUE)
#define LOG_ENABLE_ERROR()      LOG_SetLevel(LOG_LVL_ERROR, TRUE)
#define LOG_ENABLE_WARN()       LOG_SetLevel(LOG_LVL_WARN,  TRUE)
#define LOG_ENABLE_DEBUG()      LOG_SetLevel(LOG_LVL_DEBUG, TRUE)
#define LOG_ENABLE_INFO()       LOG_SetLevel(LOG_LVL_INFO,  TRUE)
#define LOG_ENABLE_BASE()       LOG_SetLevel(LOG_LVL_BASE,  TRUE)
#define LOG_DISABLE_FATAL()     LOG_SetLevel(LOG_LVL_FATAL, FALSE)
#define LOG_DISABLE_FORCE()     LOG_SetLevel(LOG_LVL_FORCE, FALSE)
#define LOG_DISABLE_ERROR()     LOG_SetLevel(LOG_LVL_ERROR, FALSE)
#define LOG_DISABLE_WARN()      LOG_SetLevel(LOG_LVL_WARN,  FALSE)
#define LOG_DISABLE_DEBUG()     LOG_SetLevel(LOG_LVL_DEBUG, FALSE)
#define LOG_DISABLE_INFO()      LOG_SetLevel(LOG_LVL_INFO,  FALSE)
#define LOG_DISABLE_BASE()      LOG_SetLevel(LOG_LVL_BASE,  FALSE)


#define LOG_ENABLE_FILE()       LOG_SetOutput(LOG_OP_FILE, TRUE);
#define LOG_DISABLE_FILE()      LOG_SetOutput(LOG_OP_FILE, FALSE);

#define LOG_ENABLE_CONSOLE()    LOG_SetOutput(LOG_OP_CONSOLE, TRUE);
#define LOG_DISABLE_CONSOLE()   LOG_SetOutput(LOG_OP_CONSOLE, FALSE);

#define LOG_ENABLE_TST()        LOG_SetOutput(LOG_OP_TST, TRUE);
#define LOG_DISABLE_TST()       LOG_SetOutput(LOG_OP_TST, FALSE);

GAPI GVOID LOG_SetLevel(GUINT16 u2Level, GBOOL bEnable);
GAPI GVOID LOG_SetOutput(GUINT16 u2OpType, GBOOL bEnable);
GAPI GVOID LOG_SetLogNew(GBOOL bEnable);
GAPI GVOID LOG_SetLogEnabler(GBOOL bEnable);
GAPI GVOID LOG_FiltModule(LOG_MOD_ID_T tModId, GBOOL bEnable);

/*************************************************************************
 *                GBASE LOG SERVICE INTERFACES
 *************************************************************************/
#if GB_LOGGER_ENABLE
    #define LOG_ASSERT  LOG_Assert
    /* GCHAR foramt message */
    #define LOG_BASE    if(LOG_Disable(LOG_LVL_BASE)){;}  else LOG_Cmsg
    #define LOG_INFO    if(LOG_Disable(LOG_LVL_INFO)){;}  else LOG_Cmsg
    #define LOG_DEBUG   if(LOG_Disable(LOG_LVL_DEBUG)){;} else LOG_Cmsg
    #define LOG_WARN    if(LOG_Disable(LOG_LVL_WARN)){;}  else LOG_Cmsg
    #define LOG_ERROR   if(LOG_Disable(LOG_LVL_ERROR)){;} else LOG_Cmsg
    #define LOG_FORCE   if(LOG_Disable(LOG_LVL_FORCE)){;} else LOG_Cmsg
    #define LOG_FATAL   if(LOG_Disable(LOG_LVL_FATAL)){;} else LOG_Cmsg
    /* WIDE GCHAR format message */
    #define WLOG_BASE   if(LOG_Disable(LOG_LVL_BASE)){;}  else LOG_Wmsg
    #define WLOG_INFO   if(LOG_Disable(LOG_LVL_INFO)){;}  else LOG_Wmsg
    #define WLOG_DEBUG  if(LOG_Disable(LOG_LVL_DEBUG)){;} else LOG_Wmsg
    #define WLOG_WARN   if(LOG_Disable(LOG_LVL_WARN)){;}  else LOG_Wmsg
    #define WLOG_ERROR  if(LOG_Disable(LOG_LVL_ERROR)){;} else LOG_Wmsg
    #define WLOG_FORCE  if(LOG_Disable(LOG_LVL_FORCE)){;} else LOG_Wmsg
    #define WLOG_FATAL  if(LOG_Disable(LOG_LVL_FATAL)){;} else LOG_Wmsg
    /* MODULE embedded GCHAR format message */
    #define MLOG_BASE   if(LOG_Disable(LOG_LVL_BASE)){;}  else LOG_ModCmsg
    #define MLOG_INFO   if(LOG_Disable(LOG_LVL_INFO)){;}  else LOG_ModCmsg
    #define MLOG_DEBUG  if(LOG_Disable(LOG_LVL_DEBUG)){;} else LOG_ModCmsg
    #define MLOG_WARN   if(LOG_Disable(LOG_LVL_WARN)){;}  else LOG_ModCmsg
    #define MLOG_ERROR  if(LOG_Disable(LOG_LVL_ERROR)){;} else LOG_ModCmsg
    #define MLOG_FORCE  if(LOG_Disable(LOG_LVL_FORCE)){;} else LOG_ModCmsg
    #define MLOG_FATAL  if(LOG_Disable(LOG_LVL_FATAL)){;} else LOG_ModCmsg
    /* MODULE embedded WIDE GCHAR format message */
    #define MWLOG_BASE   if(LOG_Disable(LOG_LVL_BASE)){;}  else LOG_ModWmsg
    #define MWLOG_INFO   if(LOG_Disable(LOG_LVL_INFO)){;}  else LOG_ModWmsg
    #define MWLOG_DEBUG  if(LOG_Disable(LOG_LVL_DEBUG)){;} else LOG_ModWmsg
    #define MWLOG_WARN   if(LOG_Disable(LOG_LVL_WARN)){;}  else LOG_ModWmsg
    #define MWLOG_ERROR  if(LOG_Disable(LOG_LVL_ERROR)){;} else LOG_ModWmsg
    #define MWLOG_FORCE  if(LOG_Disable(LOG_LVL_FORCE)){;} else LOG_ModWmsg
    #define MWLOG_FATAL  if(LOG_Disable(LOG_LVL_FATAL)){;} else LOG_ModWmsg
  #else
  #if defined(KAL_ON_OSCAR)
  #define LOG_ASSERT
    /* GCHAR foramt message */
  #define LOG_BASE
  #define LOG_INFO
  #define LOG_DEBUG
  #define LOG_WARN
  #define LOG_ERROR
  #define LOG_FORCE
  #define LOG_FATAL
    /* WIDE GCHAR format message */
  #define WLOG_BASE
  #define WLOG_INFO
  #define WLOG_DEBUG
  #define WLOG_WARN 
  #define WLOG_ERROR
  #define WLOG_FORCE
  #define WLOG_FATAL
    /* MODULE embedded GCHAR format message */
  #define MLOG_BASE 
  #define MLOG_INFO 
  #define MLOG_DEBUG
  #define MLOG_WARN 
  #define MLOG_ERROR
  #define MLOG_FORCE
  #define MLOG_FATAL
    /* MODULE embedded WIDE GCHAR format message */
  #define MWLOG_BASE
  #define MWLOG_INFO
  #define MWLOG_DEBUG
  #define MWLOG_WARN 
  #define MWLOG_ERROR
  #define MWLOG_FORCE
  #define MWLOG_FATAL
#else
  #define LOG_ASSERT(...) 
    /* GCHAR foramt message */
  #define LOG_BASE(...) 
  #define LOG_INFO(...) 
  #define LOG_DEBUG(...) 
  #define LOG_WARN(...) 
  #define LOG_ERROR(...) 
  #define LOG_FORCE(...) 
  #define LOG_FATAL(...) 
    /* WIDE GCHAR format message */
  #define WLOG_BASE(...) 
  #define WLOG_INFO(...) 
  #define WLOG_DEBUG(...) 
  #define WLOG_WARN(...)  
  #define WLOG_ERROR(...) 
  #define WLOG_FORCE(...) 
  #define WLOG_FATAL(...) 
    /* MODULE embedded GCHAR format message */
  #define MLOG_BASE(...) 
  #define MLOG_INFO(...) 
  #define MLOG_DEBUG(...) 
  #define MLOG_WARN(...)  
  #define MLOG_ERROR(...) 
  #define MLOG_FORCE(...) 
  #define MLOG_FATAL(...) 
    /* MODULE embedded WIDE GCHAR format message */
  #define MWLOG_BASE(...) 
  #define MWLOG_INFO(...) 
  #define MWLOG_DEBUG(...) 
  #define MWLOG_WARN(...) 
  #define MWLOG_ERROR(...) 
  #define MWLOG_FORCE(...) 
  #define MWLOG_FATAL(...) 
#endif
#endif //GB_LOGGER_ENABLE

/* User please don't use following APIs directly ! */
GAPI GBOOL LOG_Disable(GUINT8 u1LogLevel);
GAPI GVOID LOG_Assert(const GCHAR *csFmt, ...);
GAPI GVOID LOG_Cmsg(const GCHAR *csFmt, ...);
GAPI GVOID LOG_Wmsg(const GWCHAR *wsFmt, ...);
GAPI GVOID LOG_ModCmsg(LOG_MOD_ID_T tModId, const GCHAR *csFmt, ...);
GAPI GVOID LOG_ModWmsg(LOG_MOD_ID_T tModId, const GWCHAR *wsFmt, ...);


/*************************************************************************
 *                GBASE LOG ADDITION INTERFACES
 *************************************************************************/
typedef  GUINT32  LOG_FHANDLE;

GAPI GBOOL LOG_OpenFile(GWCHAR *wszFileName, LOG_FHANDLE *ptHandle);
GAPI GVOID LOG_LogFile(LOG_FHANDLE *ptHandle, GCHAR *csFmt, ...);
GAPI GVOID LOG_LogWFile(LOG_FHANDLE *ptHandle, GWCHAR *wsFmt, ...);
GAPI GBOOL LOG_CloseFile(LOG_FHANDLE *ptHandle);

#ifdef __cplusplus
  }
#endif

#endif // GBASE_LOG_H

