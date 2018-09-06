/***********************************************************************************************
Name: ecommon.h

Author: Hugo

Descriotion:this common include for applications

************************************************************************************************/
#ifndef _ECOMMON_H_
#define _ECOMMON_H_

//open/close debug message
#ifdef _DEBUG
#define LOG_ON								1
#else
#define LOG_ON								0
#endif

#define HD_SCREEN                           (800 * 480)
#define SD_SCREEN                           (480 * 272)

#define HD_ORG_WIDTH                        640
#define HD_ORG_HEIGHT                       385
#define SD_ORG_WIDTH                        300
#define SD_ORG_HEIGHT                       180

#define MAX_WINDOW_W_F                      800                
#define MAX_WINDOW_H_F                      480 
#define MAX_WINDOW_W_R                      720
#define MAX_WINDOW_H_R                      480

//define message type
enum CM_MSG
{
    //serial message
    UART_TX_DATA_MSG = WM_USER + 0x110,
    UART_RX_DATA_MSG,

    //GNSS message
    GNSS_DATA_MSG,
    GNSS_FIX_MSG,
    GNSS_CMD_MSG,

    //media message
    MEDIA_MSDK_MSG,
    MEDIA_AVIN_MSG,
    MEDIA_DVP_MSG,
    MEDIA_PLAYER_MSG,

    //BT message
    BT_MW_MSG,

    //APP message
    APP_RESIZE_MSG,

};

//define event type
enum CE_EVENT
{
    //touch event
    TOUCHPRESS_EVT = 0x100,
    TOUCHRELEASE_EVT,
    TOUCHREPEAT_EVT,
    TOUCHCLICK_EVT,
    TOUCHDBCLICK_EVT,
    TOUCHLONG_EVT,
    TOUCHMOVE_EVT,

    //meida event
    MEDIA_MSDK_EVT,
    MEDIA_AVIN_EVT,
    MEDIA_DVP_EVT,
};

//define media notify
enum
{
    MEDIA_SINK_NULL,
    MEDIA_SINK_FORNT, // ��ǰ��ǰ�Ų���
    MEDIA_SINK_REAR,  // ��ǰ�ں��Ų���
    MEDIA_SINK_FORNT_AND_REAR, // ��ǰ��ǰ���Ų���
};
enum
{
    MEDIA_SRC_NULL, // ��ǰû�в����κ�source
    MEDIA_SRC_DVP, // ��ǰ�ڲ���DVP
    MEDIA_SRC_SD, // ��ǰ�ڲ���SD
    MEDIA_SRC_USB, // ��ǰ�ڲ���USB    
    MEDIA_SRC_AVIN1, // ��ǰ�ڲ���AVIN1
    MEDIA_SRC_AVIN2, // ��ǰ�ڲ���AVIN2
    MEDIA_SRC_AVIN3, // ��ǰ�ڲ���AVIN3
    MEDIA_SRC_AVIN4, // ��ǰ�ڲ���AVIN4
    MEDIA_SRC_AVIN5, // ��ǰ�ڲ���AVIN5
    MEDIA_SRC_A2DP, // ��ǰ�ڲ���A2DP
    MEDIA_SRC_ISDB,
    MEDIA_SRC_DTV,
    MEDIA_SRC_DISK = MEDIA_SRC_USB,
};

//define message wparam
enum
{
    //MEDIA_PLAYER_MSG WPARAM 
    PLAYER_PLAYBACK_POSITION,
};

//define message lparam
typedef struct
{
    DWORD dwSize;//lpData size
    LPVOID lpData;//lpData pointer
}MSG_PARAM, *PMSG_PARAM;

//define APP main entry parameters
typedef struct
{
    HINSTANCE m_hInstance; //current instance
    HWND m_hMainWnd;      //main window
    DWORD m_dwWidthPixels; //screen width pixels
    DWORD m_dwHeightPixels; //screen height pixels
    DWORD m_dwPixelsBpp; //screen bpp
    DWORD m_dwOrgWidth; //org window width
    DWORD m_dwOrgHeight; //org window height
    BOOL m_fgFullSreen; //flag for APP full sreen
    LPVOID m_pParam; //instance main param
}INSTANCE_ENTRYDATA, *PINSTANCE_ENTRYDATA;


#endif //_ECOMMON_H_