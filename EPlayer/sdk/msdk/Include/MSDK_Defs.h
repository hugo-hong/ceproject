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
* file MSDK_Defs.h
*
* Project:
* --------
*   CNB
*
* Description:
* ------------
*
*
* Author:
* -------
*
*
*------------------------------------------------------------------------------
* $Revision: #53 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/

#ifndef _MSDK_DEFS_H_
#define _MSDK_DEFS_H_

#include <GDef.h>
#include "ddraw.h"

#define ENABLE_ISDBT (1)
#if ENABLE_ISDBT
#define BSP_HAS_ISDBT
#endif

typedef     GUINT32         MRESULT;

typedef     GVOID *         HMEDIAGRAPH;      /**< mediagraph handle      */
typedef     GVOID *         HVIDEOINFO;       /**< videoinfo handle         */
typedef     GVOID *         HAUDIOINFO;       /**< audioinfo handle         */
typedef     GVOID *         HSUBTITLEINFO;    /**< subtitleinfo handle        */
typedef     GVOID *         HDVDINFO;         /**< dvdinfo handle           */
typedef     GVOID *         HMP3INFO;         /**< mp3info handle               */
typedef     GVOID *         HDVXMENUINFO;     /**< divxmenuinfo handle       */
typedef     GVOID *         HVIDEOCTRL;       /**< divxmenuinfo handle       */
typedef     GVOID *         HAUDIOCTRL;       /**< audiocontrol handle         */
typedef     GVOID *         HSUBTITLECTRL;    /**< subtitle control handle      */
typedef     GVOID *         HDVDCTRL;         /**< dvd control handle      */
typedef     GVOID *         HLYRICINFO;       /**< lyricinfo handle    */

typedef     GUINT32         MSDK_IID;

#define     INVALID_MSDK_HANDLE     (NULL)

#define     MAX_AUDIO_ID3INFO_LEN    (27)


#define MSDK_SWI_INFO_SHOW              0x00000001
#define MSDK_SWI_INFO_HIDE              0x00000002
#define MSDK_SWI_INFO_DESTCOLKEY        0x00000004
#define MSDK_SWI_INFO_SRCCOLKEY         0x00000008
#define MSDK_SWI_INFO_ALPHA             0x00000010
#define MSDK_SWI_INFO_RESTORE           0x00000020

#define DEPRECATED(Replacement) _declspec(deprecated("This method is deprecated,Consider using "#Replacement" instead"))

/**mediagraph state
*
*/
typedef enum
{
    MSDK_MGSTATE_PLAYING,/**<mediagraph playing sate*/
    MSDK_MGSTATE_PAUSE,  /**<mediagraph pause sate*/
    MSDK_MGSTATE_STOP    /**<mediagraph stop sate*/
} E_MSDK_MGSTATE;

/**
* MSDK source type
*/
typedef enum
{
    MSDK_SOURCE_NONE,    /**<none source*/
    MSDK_SOURCE_DVD,     /**<dvd source */
    MSDK_SOURCE_FILE,    /**<file source*/
    MSDK_SOURCE_STRM_AV, /**<line in*/
    MSDK_SOURCE_STRM_A,  /**<audio in*/
    MSDK_SOURCE_STRM_V   /**<video in*/
} E_MSDK_SOURCE_TYPE;

typedef enum
{
    MSDK_AVIN_ANONE,
    MSDK_AVIN_A0,
    MSDK_AVIN_A1,
    MSDK_AVIN_A2,
    MSDK_AVIN_A3,
    MSDK_AVIN_A4
}E_MSDK_AVIN_AINDEX;

typedef enum
{
    MSDK_AVIN_VNONE,
    MSDK_AVIN_V1,
    MSDK_AVIN_V2,
    MSDK_AVIN_V3,
    MSDK_AVIN_V4,
    MSDK_AVIN_V0
}E_MSDK_AVIN_VINDEX;

//msdk destination type
typedef enum
{
    MSDK_SINK_NONE,     /**<msdk no target*/
    MSDK_SINK_FRONT,    /**<msdk target to front*/
    MSDK_SINK_REAR ,    /**<msdk target to rear*/
    MSDK_SINK_FRONT_REAR/**<msdk target to front and rear*/
}E_MSDK_SINK_TYPE;


//@deprecated
typedef enum
{
    MSDK_DRAM,
    MSDK_BYPASS
}E_MSDK_MODE;

//@deprecated
typedef struct
{
    E_MSDK_SINK_TYPE eSinkType;
    RECT             rRect;
    GBOOL            fgAbortScale;
}MSDK_SCALE_INFO_T;

//@deprecated
typedef struct
{
    E_MSDK_SINK_TYPE    eSink;
    RECT                rFrontDestRect;
    RECT                rRearDestRect;
    UINT32              u4FrontFlag;
    UINT32              u4RearFlag;
    DDOVERLAYFX         rFrontOvfx;
    DDOVERLAYFX         rRearOvfx;
    E_MSDK_MODE         eMode;//just for rear video choose bypass(not support now) or dram,
}MSDK_AVIN_SINK_INFO_T;

//msdk sink information
typedef struct
{
    E_MSDK_SINK_TYPE    eSink;   /**<destination type front or rear*/
    RECT                DestRect;/**<video destination rectangle*/
    UINT32              u4Flag;  /**<flags for video as hide show or alpha etc*/
    DDOVERLAYFX         ovfx;    /**<values for colorkey alpha*/
    E_MSDK_MODE         eMode;   /**<just for video in path mode*/
}MSDK_SINK_INFO_T;

typedef enum
{
    MSDK_PLAY_SPEED_NORAML, /**<normal play*/
    MSDK_PLAY_SPEED_2X,     /**<play at twice the speed*/
    MSDK_PLAY_SPEED_4X,     /**<play at four times the speed*/
    MSDK_PLAY_SPEED_8X,     /**<play at eight times the speed*/
    MSDK_PLAY_SPEED_16X,    /**<play at sixteen times the speed*/
    MSDK_PLAY_SPEED_32X,    /**<play at thirty-two times the speed*/
    MSDK_PLAY_SPEED_RW_2X,
    MSDK_PLAY_SPEED_RW_4X,
    MSDK_PLAY_SPEED_RW_8X,
    MSDK_PLAY_SPEED_RW_16X,
    MSDK_PLAY_SPEED_RW_32X
}E_MSDK_PLAY_SPEED;

//media graph config
typedef struct
{
    GBOOL       fgRepeat;             /**<if true the video repeat play*/
    GUINT32     u4MediaPosNotifyFreq; /**<set notify frequency         */
    GUINT32     u4TimeOut;            /**<timeout for load and play    */
}MG_CONFIG_T;


typedef enum
{
    MSDK_AVCODEC_MP4
} E_MSDK_AVCODEC;

//subtitle type
typedef enum
{
    MSDK_SUBTITLE_INTERNAL_SUBPIC,/**<internal subtitle is picture type*/
    MSDK_SUBTITLE_INTERNAL_TEXT,  /**<internal subtitle is text type*/
    MSDK_SUBTITLE_EXTERNAL_SUBPIC,/**<external subtitle is picture type*/
    MSDK_SUBTITLE_EXTERNAL_TEXT   /**<external subtitle is text type*/
} E_MSDK_SUBTITLE_TYPE;


/*
typedef enum
{
    MSDK_LANG_UNKNOWN,
    MSDK_LANG_CHINESE,
    MSDK_LANG_ENGLISH
}E_MSDK_AUDIO_LANG;
*/
    
typedef enum
{
    MSDK_TIMEFMT_FRAME,
    MSDK_TIMEFMT_MEDIA_TIME,
    MSDK_TIMEFMT_SAMPLE,
    MSDK_TIMEFMT_FIELD,
    MSDK_TIMEFMT_DEFAULT = MSDK_TIMEFMT_MEDIA_TIME
} E_MSDK_TIME_FORMAT;

typedef enum
{
    MSDK_CPAGE_INVAL = 0x00,     /**<invalid code page index */
    MSDK_CPAGE_1250  = 0x01,     /**<cp1250 Central Europe */
    MSDK_CPAGE_1251  = 0x02,     /**<cp1251 Cyrillic */
    MSDK_CPAGE_1252  = 0x03,     /**<cp1252 Latin I */
    MSDK_CPAGE_1253  = 0x04,     /**<cp1253 Greek */
    MSDK_CPAGE_1254  = 0x05,     /**<cp1254 Turkish */
    MSDK_CPAGE_1255  = 0x06,     /**<cp1255 Hebrew (from right to left)*/
    MSDK_CPAGE_1255_2= 0x07,     /**<cp1255_2 Hebrew (from left to right)*/
    MSDK_CPAGE_1256  = 0x08,     /**<cp1256 Arabic */
    MSDK_CPAGE_1257  = 0x09,     /**<cp1257 Baltic */
    MSDK_CPAGE_1258  = 0x0A,     /**<cp1258 Vietnam */
    MSDK_CPAGE_874   = 0x0B,     /**<cp874 Thai */
    MSDK_CPAGE_932   = 0x0C,     /**<cp932 Shift JIS */
    MSDK_CPAGE_936   = 0x0D,     /**<cp936 Simplified Chinese GBK */
    MSDK_CPAGE_949   = 0x0E,     /**<cp949 Korean */
    MSDK_CPAGE_950       = 0x0F,     /**<cp950 Traditional Chinese BIG5 */
    MSDK_CPAGE_8859_1    = 0x10,     /**<ISO 8859-1 Western European */
    MSDK_CPAGE_8859_2    = 0x11,     /**<ISO 8859-2 Eastern and Central European languages */
    MSDK_CPAGE_8859_3    = 0x12,     /**<ISO 8859-3 Southern European languages */
    MSDK_CPAGE_8859_4    = 0x13,     /**<ISO 8859-4 Northern European languages */
    MSDK_CPAGE_8859_5    = 0x14,     /**<ISO 8859-5 Latin/Cyrillic language */
    MSDK_CPAGE_8859_6    = 0x15,     /**<ISO 8859-6 Latin/Arabic language */
    MSDK_CPAGE_8859_7    = 0x16,     /**<ISO 8859-7 Latin/Greek language */
    MSDK_CPAGE_8859_8    = 0x17,     /**<ISO 8859-8 Latin/Hebrew language */
    MSDK_CPAGE_8859_9    = 0x18,     /**<ISO 8859-9 Turkish language */
    MSDK_CPAGE_KANJI     = 0x19,     /**<ARIB TR-B15 Japanese kanji character set */
    MSDK_CPAGE_FULL      = 0x7F      /**<ISO 10636-1 unicode (UCS) */
} E_MSDK_CODEPAGE_SET;

typedef struct
{
    GTCHAR  szTitle[MAX_AUDIO_ID3INFO_LEN + 1];
    GTCHAR  szYear[MAX_AUDIO_ID3INFO_LEN + 1];
    GTCHAR  szAlbum[MAX_AUDIO_ID3INFO_LEN + 1];
    GTCHAR  szAuthor[MAX_AUDIO_ID3INFO_LEN + 1];
    GTCHAR  szGenre[MAX_AUDIO_ID3INFO_LEN + 1];
}MSDK_AUDIO_ID3INFO_T;

typedef struct
{
    GBYTE   *pAPicBuf;
    GUINT32 u4APicDataLen;
}MSDK_ID3INFO_APIC_T;

typedef enum
{
    PIXFMT_32_ALPHARGB,
    PIXFMT_32_RGB,
    PIXFMT_16_RGB565          
}E_MSDK_PIXELFORMAT;

typedef enum
{
    MSDK_AVCODEC_ID_NONE,
    MSDK_AVCODEC_ID_UNKNOWN,
    MSDK_AVCODEC_ID_INVALID_VALUE,
    MSDK_AVCODEC_ID_MPEG1_2,//vodeo codec type
    MSDK_AVCODEC_ID_DIVX311,
    MSDK_AVCODEC_ID_MP4V,
    MSDK_AVCODEC_ID_H263,
    MSDK_AVCODEC_ID_H264,
    MSDK_AVCODEC_ID_WVC1,
    MSDK_AVCODEC_ID_DIVX4,
    MSDK_AVCODEC_ID_DIVX6,
    MSDK_AVCODEC_ID_MPEG1,            ///< MPEG1 mode
    MSDK_AVCODEC_ID_MPEG2,            ///< MPEG2 mode
    MSDK_AVCODEC_ID_DIVX3,            ///< DIVX3 mode
    MSDK_AVCODEC_ID_DIVX5,            ///< DIVX5 mode
    MSDK_AVCODEC_ID_MPEG4,            ///< MPEG4 mode
    MSDK_AVCODEC_ID_MPEG4_AVC,        ///< mpeg4 avc video stream
    MSDK_AVCODEC_ID_SMPTE_VC1,        ///< smpte vc-1 video stream
    MSDK_AVCODEC_ID_MVC1,
    MSDK_AVCODEC_ID_WMV1,
    MSDK_AVCODEC_ID_WMV2,
    MSDK_AVCODEC_ID_WMV3,
    MSDK_AVCODEC_ID_WMVA,
    MSDK_AVCODEC_ID_MP43,
    MSDK_AVCODEC_ID_VC1,              //audio codec type
    MSDK_AVCODEC_ID_MP2V,
    MSDK_AVCODEC_ID_VP6,
    MSDK_AVCODEC_ID_RV,
    MSDK_AVCODEC_ID_FLV,
    MSDK_AVCODEC_ID_AC3,
    MSDK_AVCODEC_ID_MPEG,
    MSDK_AVCODEC_ID_SORENSON,
    MSDK_AVCODEC_ID_MP3,
    MSDK_AVCODEC_ID_DTS,
    MSDK_AVCODEC_ID_MLP,
    MSDK_AVCODEC_ID_CDDA,
    MSDK_AVCODEC_ID_LPCM,
    MSDK_AVCODEC_ID_WMA,
    MSDK_AVCODEC_ID_AAC,
    MSDK_AVCODEC_ID_DTS_CD,
    MSDK_AVCODEC_ID_VORBIS,
    MSDK_AVCODEC_ID_HDCD,
    MSDK_AVCODEC_ID_SACD,
    MSDK_AVCODEC_ID_DTSHD_PRI_XLL,
    MSDK_AVCODEC_ID_AAC_PURE,
    MSDK_AVCODEC_ID_PCM,
    MSDK_AVCODEC_ID_EAC3,              ///< EAC3 mode
    MSDK_AVCODEC_ID_EAC3SEC,              ///< EAC3SEC mode
    MSDK_AVCODEC_ID_SDDS,             ///< SDDS mode
    MSDK_AVCODEC_ID_WAV,
    MSDK_AVCODEC_ID_DTSCD,            ///< DTS CD mode
    MSDK_AVCODEC_ID_APE,
    MSDK_AVCODEC_ID_FLAC,

    MSDK_AVCODEC_ID_HDMV_LPCM,        ///< HDMV LPCM mode
    MSDK_AVCODEC_ID_DOLBY_LOSSLESS,   ///< Dolby Lossless mode
    MSDK_AVCODEC_ID_DOLBY_DIGITAL_PLUS,   ///< Dolby Digital Plus mode
    MSDK_AVCODEC_ID_DTSHD_NO_XLL,          ///< DTS-HD except XLL mode
    MSDK_AVCODEC_ID_DTSHD_XLL,             ///< DTS-HD XLL mode
    MSDK_AVCODEC_ID_DOLBY_DIGITAL_PLUS_SECONDARY,   ///Dolby Digital Plus audio stream for Secondary audio.
    MSDK_AVCODEC_ID_DTSHD_SECONDARY,  ///DTS-HD audio stream for Secondary audio.
    MSDK_AVCODEC_ID_DTSESMATRIX_6_1_CHAN,            /* DTS Extended Surround Matrix 6.1 channel */
    MSDK_AVCODEC_ID_DTSESDISCRETE_6_1_CHAN,         /* DTS Extended Surround Discrete 6.1 channel */
    MSDK_AVCODEC_ID_DTSESDISCRETE_8_CHAN,              /* DTS Extended Surround Discrete 8 channel */
    MSDK_AVCODEC_ID_DTS_96_24,                                 /* DTS 96/24 */
    MSDK_AVCODEC_ID_DTS_96_24_ES_MATRIX,                 /* DTS 96/24 Extended Surround Matrix */
    MSDK_AVCODEC_ID_DtsHd_Es_Matirx_6_1_Chan,
    MSDK_AVCODEC_ID_DtsHd_Es_Discrete_6_1_Chan,
    MSDK_AVCODEC_ID_DtsHd_ES_Discrete_8_Chan,
    MSDK_AVCODEC_ID_DtsHd_96_24,
    MSDK_AVCODEC_ID_DtsHd_96_24_Es_Matrix,
    MSDK_AVCODEC_ID_PPCM,              ///< Packed PCM mode
    MSDK_AVCODEC_ID_DOLBY_DIGITAL,     ///< Dolby Digitai
    MSDK_AVCODEC_ID_MPEG2_EX,        ///< MPEG_2 with extention
    MSDK_AVCODEC_ID_RA_COOK,
    MSDK_AVCODEC_ID_DVDAPPCM,        //< dvd audio PPCM
    MSDK_AVCODEC_ID_DVDALPCM,         //< dvd audio LPCM

    MSDK_AVCODEC_ID_TEXT_SRT,
    MSDK_AVCODEC_ID_TEXT,
    MSDK_AVCODEC_ID_SUB,
    MSDK_AVCODEC_ID_XSUB,
    MSDK_AVCODEC_ID_XSUB_PLUS,
} E_MSDK_AVCODECID_T;

typedef enum
{
    MSDK_AV_FILE_DIVX,
    MSDK_AV_FILE_MKV,
    MSDK_AV_FILE_MPEG,
    MSDK_AV_FILE_ASF,
    MSDK_AV_FILE_AVI,
    MSDK_AV_FILE_MP4,
    MSDK_AV_FILE_TSF,
    MSDK_AV_FILE_OGM,
    MSDK_AV_FILE_FLV,
    MSDK_AV_FILE_WAV,
    MSDK_AV_FILE_MP3,
    MSDK_AV_FILE_AAC,
    MSDK_AV_FILE_AC3,
    MSDK_AV_FILE_RM,
    MSDK_AV_FILE_APE,
    MSDK_AV_FILE_FLAC,
    MSDK_AV_FILE_TS,
    MSDK_AV_FILE_NONE,
} MSDK_AV_FILE_TYPE_T;

typedef struct
{
    E_MSDK_AVCODECID_T eVCodec;
    GDOUBLE            dbFrameRate;
    GUINT32            u4BitRate;
    GUINT32            u4Width;
    GUINT32            u4Height;
}MSDK_VIDEO_INFO_T;

typedef struct
{
    E_MSDK_AVCODECID_T eACodec;
    GUINT32            u4SampleRate;
    GUINT32            u4BitRate;
}MSDK_AUDIO_INFO_T;

#define MAX_PATH      260
typedef struct
{
    E_MSDK_AVCODECID_T eSubCodec;
    GUINT32            u4Width;
    GUINT32            u4Height;
}MSDK_SUBTITLE_INFO_T;

typedef struct
{
    GTCHAR              szFileName[MAX_PATH];
    MSDK_AV_FILE_TYPE_T eFileType;
    GUINT64             u8FileSize;
    GUINT32             u4Duration;
    GUINT32             u4VdoStmCnt;
    GUINT32             u4AudStmCnt;
    GUINT32             u4SpStmCnt;
    
    MSDK_VIDEO_INFO_T    rVdoInfo;
    MSDK_AUDIO_INFO_T    rAudInfo;
    MSDK_SUBTITLE_INFO_T rSubInfo;
}MSDK_MEDIA_INFO_T;

typedef enum
{
    MSDK_CAP_FILE_SEEK_SUPPORT                  = 0x00000001,
    MSDK_CAP_FILE_FF_SUPPORT                    = MSDK_CAP_FILE_SEEK_SUPPORT              << 1,
    MSDK_CAP_FILE_RW_SUPPORT                    = MSDK_CAP_FILE_FF_SUPPORT                << 1,
    MSDK_CAP_VIDEO_RESOLUTION_SUPPORT           = MSDK_CAP_FILE_RW_SUPPORT                << 1,
    MSDK_CAP_VIDEO_BITRATE_SUPPORT              = MSDK_CAP_VIDEO_RESOLUTION_SUPPORT       << 1,
    MSDK_CAP_VIDEO_FRAMERATE_SUPPORT            = MSDK_CAP_VIDEO_BITRATE_SUPPORT          << 1,
    MSDK_CAP_VIDEO_CODEC_SUPPORT                = MSDK_CAP_VIDEO_FRAMERATE_SUPPORT        << 1,
    MSDK_CAP_VIDEO_PROFILE_LEVEL_SUPPORT        = MSDK_CAP_VIDEO_CODEC_SUPPORT            << 1,
    MSDK_CAP_AUDIO_BITRATE_SUPPOPRT             = MSDK_CAP_VIDEO_PROFILE_LEVEL_SUPPORT    << 1,
    MSDK_CAP_AUDIO_SAMPLERATE_SUPPORT           = MSDK_CAP_AUDIO_BITRATE_SUPPOPRT         << 1,
    MSDK_CAP_AUDIO_CODEC_SUPPORT                = MSDK_CAP_AUDIO_SAMPLERATE_SUPPORT       << 1,
    MSDK_CAP_AUDIO_PROFILE_LEVEL_SUPPORT        = MSDK_CAP_AUDIO_CODEC_SUPPORT            << 1,
}E_MSDK_CAPABILITY_T;

typedef MRESULT (*PFN_MSDK_MEDIAEVENT_LISTENER)(HMEDIAGRAPH hMediaGraph,
                                                GUINT32 u4Evt, 
                                                GUINT32 u4Param1,
                                                GUINT32 u4Param2,
                                                GUINT32 u4CustomData);


#endif //_MSDK_DEFS_H_