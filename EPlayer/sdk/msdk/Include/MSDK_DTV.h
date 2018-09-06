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
* file MSDK_DTV.h
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
* $Revision: #12 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/

#ifndef _MSDK_DTV_H_
#define _MSDK_DTV_H_

#include <MSDK_Defs.h>

#ifdef __cplusplus
extern "C" {
#endif

#if ENABLE_ISDBT

typedef     GVOID *         HTVNCTRL;        /**< TvnCtrl handle */
typedef     GVOID *         HSVLDB;          /**< SVLDB handle */
typedef     GVOID *         HEPGCTRL;        /**< EPGINFO handle */
typedef     GVOID *         HSNAPSHOTCTRL;
typedef     GVOID *         HSERVICELIST;

#define MSDK_DTV_SIANO_OK      0
#define MSDK_SVC_NAME_MAX_LEN  32
#define MSDK_EPG_EVT_NAME_LEN  64
#define DTVAPI _declspec(dllexport)

enum
{
    MSDK_DTV_PARAM_LOCKFREQ,
    MSDK_DTV_PARAM_UNLOCKFREQ
};

//DTV enum & struct
typedef enum
{
//    SIGNAL_QUALITY_UNKNOWN,
    SIGNAL_QUALITY_FAIL,
    SIGNAL_QUALITY_POOR,
    SIGNAL_QUALITY_AVERAGE,
    SIGNAL_QUALITY_GOOD,
    SIGNAL_QUALITY_EXCELLENT
}   MSDK_TUNER_SIGNAL_QUALITY_T;


typedef struct _MSDK_SVC_DATA_ISDBT_T
{
    UINT16    u2OnId;
    UINT16    u2TsId;
}   MSDK_SVC_DATA_ISDBT_T;


typedef struct _MSDK_SVC_DATA_DVB
{
    UINT16    u2OnId;
    UINT16    u2TsId;
}   MSDK_SVC_DATA_DVB;

typedef struct _MSDK_SVC_DATA_ATSC
{
    UINT16   u2TsId;
    UINT16   u2SourceId;
}   MSDK_SVC_DATA_ATSC;

typedef struct _MSDK_SVC_DATA_ANALOG
{
    INT16    i2FreqOffset;
    UINT8    u1ChannelNum;
    UINT8    tColorSys   : 4;
    UINT8    tSoundSys   : 4;
}   MSDK_SVC_DATA_ANALOG;


/*----------------------------------------------------------------------------*/
typedef union _U_MSDK_SVC_DATA
{
    MSDK_SVC_DATA_ISDBT_T   tSvcIsdbt;
    MSDK_SVC_DATA_DVB       tSvcDvb;
    MSDK_SVC_DATA_ATSC      tSvcAtsc;
    MSDK_SVC_DATA_ANALOG    tSvcAnalog;    
}   U_MSDK_SVC_DATA;


typedef enum
{
    MSDK_UNKNOWN_BRDCST_TYPE,
    MSDK_ANALOG_BRDCST_TYPE,
    MSDK_DVB_BRDCST_TYPE,
    MSDK_ATSC_BRDCST_TYPE,
    MSDK_ISDBT_BRDCST_TYPE
}   E_MSDK_BRDCST_TYPE;

typedef enum                        /**< Typedef and constants for the service type of the service */
{
    MSDK_UNKNOWN_SVC_TYPE,               /**< Service type is unknown */
    MSDK_TV_SVC_TYPE,                    /**< Service type is TV */
    MSDK_RADIO_SVC_TYPE,                 /**< Service type is RADIO */
    MSDK_APP_SVC_TYPE                    /**< Service type is APP */
}   E_MSDK_SVC_TYPE;

typedef struct _MSDK_SVCRECHEADER_T
{
    E_MSDK_BRDCST_TYPE      tBrdcstType;
    UINT16                  u2ChannelId;
    UINT16                  u2PhyChannelId;
    UINT16                  u2ProgramID;
    UINT16                  u2MajorFreq;
    E_MSDK_SVC_TYPE         tSvcType;
    UINT8                   u1CodingFlag;
    UINT16                  au2Name[MSDK_SVC_NAME_MAX_LEN + 1];
    UINT8                   u1UserFlag;
    BOOL                    fgRegionGen;
}   MSDK_SVCRECHEADER_T;

typedef struct _MSDK_SVCREC_T
{
    MSDK_SVCRECHEADER_T        tSvcHeader;
    U_MSDK_SVC_DATA            uSvcData;
}   MSDK_SVCREC_T;


typedef struct
{
    UINT16 u2Year;
    UINT8  u1Month;         /* 1-12, month of the year. */
    UINT8  u1Day;           /* 1-31, day of the month. */
    UINT8  u1Dow;           /* 0-6, Sunday to Saturday. */
    UINT8  u1Hour;          /* 0-23 */
    UINT8  u1Min;           /* 0-59 */
    UINT8  u1Sec;           /* 0-59 */

    BOOL   fgGmt;           /* TRUE:  Universal Time Coordinate (UTC). FALSE: Local Time */
    BOOL   fgDst;           /* TRUE:  DayLight-Saving-Time on. FALSE: Day-Light_Saving Time off */
}   MSDK_TIMEINFO_T;

typedef struct
{
    MSDK_TIMEINFO_T         tStartTime;
    UINT32                  u4Duration;
}   MSDK_EPG_EVTENTRY;

typedef struct
{
    UINT16      au2Name[MSDK_EPG_EVT_NAME_LEN + 1];
    UINT16      u2TextLen;
    UINT16*     pu2Text;
}   MSDK_EPG_EVTNAME;


typedef struct
{
    MSDK_EPG_EVTNAME    tName;
}   MSDK_EPG_EVTNAMEINFO;

/*----------------------------------------------------------------------------*/
typedef struct
{
    MSDK_EPG_EVTENTRY        tEntry;
    MSDK_EPG_EVTNAMEINFO     tNameInfo;
    UINT8                    u1ContentCnt;
}   MSDK_EPGINFO_T;

typedef enum
{
    MSDK_PIXEL_FORMAT_RGB_D565,
    MSDK_PIXEL_FORMAT_ARGB_D1555,
    MSDK_PIXEL_FORMAT_ARGB_D8888,
}   E_MSDK_PIXEL_FORMAT;

typedef enum
{
    MSDK_DTV_COUNTRY_JAPANESE = 0,
    MSDK_DTV_COUNTRY_BRAZIL,
}   E_MSDK_DTV_COUNTRY;

typedef enum
{
    MSDK_CC_LANG_JAPANESE = 0,
    MSDK_CC_LANG_PORTUGUESE,
    MSDK_CC_LANG_ENGLISH,
}   E_MSDK_CC_LANG;

typedef struct
{
    E_MSDK_CC_LANG tCClang;
}   MSDK_CCINFO_T;

/**
*   @brief This method Find RenderFilter. 
*
*   @param[in]  hMediaGraph media graph handle.
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_FindRenderFilter()
*   @see    msdk_error.h
**/
DTVAPI MRESULT      MediaGraph_FindRenderFilter(HMEDIAGRAPH hMediaGraph);

/**
*     @brief This method open media graph display. 
*
*     @param[in]    hMediaGraph media graph handle.
*
*     @return   MRESULT  define in msdk_error.h
*              
*     @see       MediaGraph_Create()
*     @see       msdk_error.h
**/
DTVAPI MRESULT     MediaGraph_OpenAvs(HMEDIAGRAPH hMediaGraph);

/**
*     @brief This method set service list database file path. 
*
*     @param[in]    hMediaGraph media graph handle.
*     @param[in]    szDBPath service list database path
*
*     @return   MRESULT  define in msdk_error.h
*              
*     @see       MediaGraph_Create()
*     @see       msdk_error.h
**/
DTVAPI MRESULT    DTV_SetSvlDBPath(TCHAR *szDBPath);


/**
*     @brief This method set media graph  real time streaming. 
*
*     @param[in]    hMediaGraph media graph handle.
*
*     @return   MRESULT  define in msdk_error.h
*              
*     @see       MediaGraph_Create()
*     @see       msdk_error.h
**/
DTVAPI MRESULT    DTV_SetSourceStream(HMEDIAGRAPH hMediaGraph);

/**
*     @brief This method initializes tuner. 
*
*     @param[in]    hMediaGraph media graph handle.
*
*     @return   MRESULT  define in msdk_error.h
*              
*     @see       MediaGraph_Create()
*     @see       msdk_error.h
**/
DTVAPI MRESULT     DTV_TunerInit(HMEDIAGRAPH hMediaGraph);

/**
*     @brief This method uninitializes tuner. 
*
*     @param[in]    hMediaGraph media graph handle.
*
*     @return   MRESULT  define in msdk_error.h
*              
*     @see       MediaGraph_Create()
*     @see       msdk_error.h
**/
DTVAPI MRESULT     DTV_TunerDeInit(HMEDIAGRAPH hMediaGraph);

/**
 * @addtogroup TvnInterfaceCtrl
 * @{
 */

/**
*    @brief This method  begin to search the channel
*
*    @param[in]      hTvnCtrl  tvn info handle
*    @param[in]      tScanType  broad cast type
*    @param[in]      fgFullScan  if it`s full channel scan
*    @param[in]      u1BeginChNum  the first channel number to be scaned,not use  
*    @param[in]      u1EndChNum  the last channel number to be scaned,not use
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
*    @note    this interface is to scan all the channels in range 
**/

DTVAPI MRESULT TvnCtrl_Scan(HTVNCTRL hTvnCtrl, E_MSDK_BRDCST_TYPE tScanType, BOOL fgFullScan, UINT8 u1BeginChNum, UINT8 u1EndChNum);

/**
*    @brief This method begin to search specified region of the channel
*
*    @param[in]      hTvnCtrl  tvn info handle
*    @param[in]      tScanType  broad cast type
*    @param[in]      pu1ChNum  the channel numbers that want to be scaned
*    @param[in]      u1ChCnt  the count of the input channel numbers 
*
*    @return    MRESULT  define in msdk_error.h
*
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
*    @note    this interface is to scan the specified region of channels that inputed 
**/

DTVAPI MRESULT TvnCtrl_RegionScan(HTVNCTRL hTvnCtrl, E_MSDK_BRDCST_TYPE tScanType, UINT8 *pu1ChNum, UINT8 u1ChCnt);

/**
*    @brief This method stop scanning
*
*    @param[in]      hTvnCtrl  tvn info handle
*
*    @return    MRESULT  define in msdk_error.h
*
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/

DTVAPI MRESULT TvnCtrl_CancelScan(HTVNCTRL hTvnCtrl);

/**
*    @brief This method lock the frequency,prepare to play
*
*    @param[in]      hTvnCtrl  tvn info handle
*    @param[in]      hServiceList  the channel list that has been scaned
*    @param[in]      i4Idx  the index of the chosen channel in the channel list
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
*    @note    after select service ,the application will play the stream next.
**/

DTVAPI MRESULT TvnCtrl_ReScan(HTVNCTRL hTvnCtrl,GINT32 i4ChNum);
/**
*    @brief This method get the current stream time 
*
*    @param[in]      hTvnCtrl  Tvn info handle
*    @param[out]    ptNow  current time
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
*    @note    Every 1 minute BSP will send a message to notify that time is changed.
*                 MSDK will send EVT_MSDK_DTV_TIME_CHANGE message to Application.
*                 Application get the message and call the Function
**/

DTVAPI MRESULT TvnCtrl_SelectService(HTVNCTRL hTvnCtrl, HSERVICELIST hServiceList, INT32 i4Idx);

/**
*    @brief This method stop selecting the service
*
*    @param[in]      hTvnCtrl  tvn info handle
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/

DTVAPI MRESULT TvnCtrl_CancelSelectService(HTVNCTRL hTvnCtrl);

/**
*    @brief This method start recording the stream 
*
*    @param[in]      hTvnCtrl  tvn info handle
*    @param[in]      pszRecName  the File Name of the recorded file 
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/

DTVAPI MRESULT TvnCtrl_StartRec(HTVNCTRL hTvnCtrl, const GTCHAR *pszRecName);

/**
*    @brief This method stop recording file 
*
*    @param[in]      hTvnCtrl  tvn info handle
*
*    @return    MRESULT  define in msdk_error.h    
*
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/

DTVAPI MRESULT TvnCtrl_StopRec(HTVNCTRL hTvnCtrl);

/**
*    @brief This method set frequency point list via country info. 
*
*    @param[in]      hTvnCtrl  tvn info handle
*    @param[in]      tCountry country info 
*
*    @return    MRESULT  define in msdk_error.h    
*
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/

DTVAPI MRESULT TvnCtrl_SetCountry(HTVNCTRL hTvnCtrl, E_MSDK_DTV_COUNTRY tCountry);

/**
*    @brief This method rescan the frequence that is playing
*
*    @param[in]      hTvnCtrl  tvn info handle
*    @param[in]      i4ChNum  the channel number of the service will be rescaned
*
*    @return    MRESULT  define in msdk_error.h    
*
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
*    @note    if the Psi or Si table is changed when playing the transport stream,ISDBT should call the function
**/

DTVAPI MRESULT TvnCtrl_GetTime(HTVNCTRL hTvnCtrl, MSDK_TIMEINFO_T* ptNow);

/**
*@}
*/
/**
* @addtogroup EPGInterfaceCtrl
* @{
*/

/**
*    @brief This method set  the epg infromation count of one page. 
*
*    @param[in]      hEpgCtrl  EPG info handle
*    @param[out]    pu4EpgCnt  the count of epg info
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     msdk_error.h
*    @note    the method must called before DTV_SetSourceStream.if not, the page count is default page count(6).
*
**/
DTVAPI MRESULT EpgCtrl_SetPageCnt(UINT32 u4PageCnt);

/**
*    @brief This method get the count of epg info 
*
*    @param[in]      hEpgCtrl  EPG info handle
*    @param[out]    pu4EpgCnt  the count of epg info
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT EpgCtrl_GetEvtCnt(HEPGCTRL hEpgCtrl, UINT32 *pu4EpgCnt);

/**
*    @brief This method get a epg info
*
*    @param[in]      hEpgCtrl  EPG info handle
*    @param[in]      i4Idx  the epg index that want to get 
*    @param[out]    prEpgInfo  the epg info 
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT EpgCtrl_GetEvtInfo(HEPGCTRL hEpgCtrl, INT32 i4Idx, MSDK_EPGINFO_T *prEpgInfo);

/**
*    @brief This method free the epg info that get before 
*
*    @param[in]      hEpgCtrl  EPG info handle
*    @param[out]    prEpgInfo  epg info 
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
*    @note    after getting epg info ,please free epg info next.
**/
DTVAPI MRESULT EpgCtrl_FreeEvtInfo(HEPGCTRL hEpgCtrl, MSDK_EPGINFO_T *prEpgInfo);

/**
*    @brief This method start to analytical EPG info
*
*    @param[in]      hEpgCtrl  EPG info handle
*    @param[in]      ptStart  the time of first epg info 
*    @param[in]      ptEnd  the time of last epg info
*
*    @return    MRESULT  define in msdk_error.h
*               
*
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT EpgCtrl_GotoPrevPg(HEPGCTRL hEpgCtrl, UINT8 u1FocusIdx);

/**
*    @brief This method get the epg info of next page 
*
*    @param[in]      hEpgCtrl  EPG info handle
*    @param[in]      u1FocusIdx not used
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT EpgCtrl_GotoNextPg(HEPGCTRL hEpgCtrl, UINT8 u1FocusIdx);

/**
*    @brief This method  get the epg info of next day 
*
*    @param[in]      hEpgCtrl  EPG info handle
*    @param[in]      u1FocusIdx not used
*
*    @return    MRESULT  define in msdk_error.h
*
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT EpgCtrl_GotoNextDay(HEPGCTRL hEpgCtrl, UINT8 u1FocusIdx);

/**
*    @brief This method get the epg info of previous day 
*
*    @param[in]      hEpgCtrl  EPG info handle
*    @param[in]      u1FocusIdx not used
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT EpgCtrl_GotoPrevDay(HEPGCTRL hEpgCtrl, UINT8 u1FocusIdx);

/**
*    @brief This method get epg info 
*
*    @param[in]      hEpgCtrl  EPG info handle
*    @param[out]     pbFirst   is the first page or not
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT EpgCtrl_IsFirstPage(HEPGCTRL hEpgCtrl, BOOL *pfgFirst);

/**
*    @brief This method get epg info 
*
*    @param[in]      hEpgCtrl  EPG info handle
*    @param[out]     pbLast   is the last page or not
* 
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT EpgCtrl_IsLastPage(HEPGCTRL hEpgCtrl, BOOL *pfgLast);

/**
*    @brief This method get epg info 
*
*    @param[in]      hEpgCtrl  EPG info handle
*    @param[out]     pbPrevDay  whether can be page to prev day
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT EpgCtrl_CheckPrevDay(HEPGCTRL hEpgCtrl,BOOL *pfgPrevDay);

/**
*    @brief This method get epg info 
*
*    @param[in]      hEpgCtrl  EPG info handle
*    @param[out]     pbPrevDay  whether can be page to next day
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT EpgCtrl_CheckNextDay(HEPGCTRL hEpgCtrl,BOOL *pfgNextDay);

/**
*@}
*/
/**
* @addtogroup ServiceDBCtrl
* @{
*/
/**
*    @brief This method create tv channel service list
*
*    @param[in]      hSvlDb  ServiceDateBase info handle
*    @param[in]      szSvlName  Service List info name
*    @param[out]    phServiceList  the service list handle
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
*    @note    If the servicelist named szSvlName has existed,it will only open the servicelist.
*                 if not,will created a new servicelist.
**/
DTVAPI MRESULT SvlDb_CreateTvSvl(HSVLDB  hSvlDb, const CHAR *szSvlName, HSERVICELIST *phServiceList);

/**
*    @brief This method get real servicelist handle
*
*    @param[in]      hSvlDb  ServiceDateBase info handle
*    @param[out]    phServiceList  the service list handle
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_GetRealSvl(HSVLDB  hSvlDb, HSERVICELIST *phServiceList);

/**
*    @brief This method save SVLDB to NOR Flash
*
*    @param[in]      hSvlDb  ServiceDateBase info handle
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_Save(HSVLDB  hSvlDb);

/**
*    @brief This method clean all the records in service list.
*
*    @param[in]      hServiceList   the service list handle
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_CleanSvl(HSERVICELIST hServiceList);

/**
*    @brief This method add record to service list
*
*    @param[in]      hServiceList   the service list handle
*    @param[in]      prSvcRec  the record info that prepare to save
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_AddSvc(HSERVICELIST hServiceList, const MSDK_SVCREC_T *prSvcRec);

/**
*    @brief This method get the freq of a record
*
*    @param[in]      hServiceList   the service list handle
*    @param[in]      ptSvcRec  the record that want to get freq
*    @param[out]      ptSvcRec  the freq of the record
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_GetFreq(HSERVICELIST hServiceList, const MSDK_SVCREC_T *ptSvcRec, UINT32* pu4Freq);

/**
*    @brief This method remove record from service list
*
*    @param[in]      hServiceList   the service list handle
*    @param[in]      i4Idx  the index of record that want to remove
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_RemoveSvcByIndex(HSERVICELIST hServiceList, INT32 i4Idx);

/**
*    @brief This sort the svldb by channel number
*
*    @param[in]      hServiceList   the service list handle
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_SortByChannelNum(HSERVICELIST hServiceList);

/**
*    @brief This method remove record from service list by freq
*
*    @param[in]      hServiceList   the service list handle
*    @param[in]      u4Freq  the freq of record that want to remove
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_RemoveSvcByFreq(HSERVICELIST hServiceList, UINT32 u4Freq);

/**
*    @brief This method update an exist record with an new record
*
*    @param[in]      hServiceList   the service list handle
*    @param[in]      i4Idx  the index of record that want to update
*    @param[in]      ptSvcRec  the new record
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_UpdateSvc(HSERVICELIST hServiceList, INT32 i4Idx, const MSDK_SVCREC_T *ptSvcRec);

/**
*    @brief This method get record from service list
*
*    @param[in]      hServiceList   the service list handle
*    @param[in]      i4Idx   the index of record that want to get
*    @param[out]    ptSvcRec  the record info
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_GetSvc(HSERVICELIST hServiceList, INT32 i4Idx, MSDK_SVCREC_T *ptSvcRec);

/**
*    @brief This method get the count of records in the servicelist
*
*    @param[in]      hServiceList   the service list handle
*    @param[out]    pu4SvlCount  the count of records in the servicelist
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_GetSvlCount(HSERVICELIST hServiceList, UINT32 *pu4SvlCount);

/**
*    @brief This method add a record to favorite list
*
*    @param[in]      hSvlDb  ServiceDateBase info handle
*    @param[in]      hServiceList   the service list handle
*    @param[in]      i4Idx  the index of the record want to save to favorite list
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_AddFavSvcByIdx(HSVLDB  hSvlDb, HSERVICELIST hServiceList, INT32 i4Idx);

/**
*    @brief This method add a record to favorite list
*
*    @param[in]      hSvlDb  ServiceDateBase info handle
*    @param[in]      ptSvcRec   the record info that want to save to favorite list
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_AddFavSvcByRec(HSVLDB  hSvlDb, const MSDK_SVCREC_T *ptSvcRec);

/**
*    @brief This method remove a record from favorite list
*
*    @param[in]      hSvlDb  ServiceDateBase info handle
*    @param[in]      i4Idx   the index of the record that want to remove
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_RemoveFavorite(HSVLDB  hSvlDb, INT32 i4Idx);

/**
*    @brief This method get the count of records in the favorite list
*
*    @param[in]      hSvlDb  ServiceDateBase info handle
*    @param[out]    pu4FavCount   the count of records in the favorite list
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_GetFavoriteCnt(HSVLDB  hSvlDb, UINT32 *pu4FavCount);

/**
*    @brief This method get a record info from the favorite list
*
*    @param[in]      hSvlDb  ServiceDateBase info handle
*    @param[in]      i4Idx  the index ot the record that want to get
*    @param[out]    ptSvcRec   the record info
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SvlDb_GetFavorite(HSVLDB  hSvlDb, INT32 i4Idx, MSDK_SVCREC_T *ptSvcRec);

/**
*@}
*/
/**
* @addtogroup SnapshotInterfaceCtrl
* @{
*/
/**
*    @brief This method save a snapshot picture
*
*    @param[in]      hEpgCtrl  EPG info handle
*    @param[in]      tPixelFmt  the format of the snapped picture
*    @param[in]      pszSnapshotName  the file name of the picture
*
*    @return    MRESULT  define in msdk_error.h
*               
*    @see     MediaGraph_QueryInterface()
*    @see     msdk_error.h
*
**/
DTVAPI MRESULT SnapshotCtl_PrintScreen(HSNAPSHOTCTRL hSnapshot, E_MSDK_PIXEL_FORMAT tPixelFmt, const GTCHAR *pszSnapshotName);
#endif 
/**
*@}
*/

#ifdef __cplusplus
}
#endif

#endif //_MSDK_DTV_H_