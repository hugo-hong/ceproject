/************************************************************************
    Filename:gpsal.h

    Description:head file of the GPS Adaptation layer.

    Data:2013/04/03

    Author: mtk71195
************************************************************************/

#ifndef __GPSAL_H__
#define __GPSAL_H__
#include <utility>

using namespace std;

#define GPSAL_API __declspec(dllexport)

#if 0

#define DEFAULT_BUF_LEN             (1024)

#define MAX_FIELD_LEN               (32)

#define MAX_NMEA_LEN                (100)

#define MAX_GPS_SV_NUM              (32)

#define MAX_GPS_SV_VIEW             (12)

#define GPS_VERSION_1               1

// GPS_VALID_XXX bit flags in GPS_POSITION structure are valid.
#define GPS_VALID_UTC_TIME                                 0x00000001
#define GPS_VALID_LATITUDE                                 0x00000002
#define GPS_VALID_LONGITUDE                                0x00000004
#define GPS_VALID_SPEED                                    0x00000008
#define GPS_VALID_HEADING                                  0x00000010
#define GPS_VALID_MAGNETIC_VARIATION                       0x00000020
#define GPS_VALID_ALTITUDE_WRT_SEA_LEVEL                   0x00000040
#define GPS_VALID_ALTITUDE_WRT_ELLIPSOID                   0x00000080
#define GPS_VALID_POSITION_DILUTION_OF_PRECISION           0x00000100
#define GPS_VALID_HORIZONTAL_DILUTION_OF_PRECISION         0x00000200
#define GPS_VALID_VERTICAL_DILUTION_OF_PRECISION           0x00000400
#define GPS_VALID_SATELLITE_COUNT                          0x00000800
#define GPS_VALID_SATELLITES_USED_PRNS                     0x00001000
#define GPS_VALID_SATELLITES_IN_VIEW                       0x00002000
#define GPS_VALID_SATELLITES_IN_VIEW_PRNS                  0x00004000
#define GPS_VALID_SATELLITES_IN_VIEW_ELEVATION             0x00008000
#define GPS_VALID_SATELLITES_IN_VIEW_AZIMUTH               0x00010000
#define GPS_VALID_SATELLITES_IN_VIEW_SIGNAL_TO_NOISE_RATIO 0x00020000

typedef enum
{
    GPS_FIX_TYPE_INVALID = 1,
    GPS_FIX_TYPE_2D = 2,
    GPS_FIX_TYPE_3D = 3
} GPS_FIX_TYPE;

typedef enum
{
    GPS_FIX_Q_INVALID = 0,//没有定位
    GPS_FIX_Q_GPS = 1, //非差分定位
    GPS_FIX_Q_DGPS = 2,//差分定位
    GPS_FIX_Q_EST = 6 //估算
}GPS_FIX_QUALITY;

typedef enum
{
    DGPS_MODE_NONE = 0,
    DGPS_MODE_RTCM = 1,
    DGPS_MODE_SBAS = 2,
    DGPS_MODE_AUTO = 3
} DGPS_MODE;

typedef struct
{
    UINT16    year;           /* years since 1900 */
    UINT8     month;          /* 0-11 */
    UINT8     mday;           /* 1-31 */
    UINT8     hour;           /* 0-23 */
    UINT8     min;            /* 0-59 */
    UINT8     sec;            /* 0-59 */
    UINT8     pad1;
    UINT16    msec;           /* 0-999 */
    UINT16    pad2;
} UTC_TIME;

typedef struct
{    
    int prn; //PRN ID
    int elevation;//仰角
    int azim; //方位角
    int snr; //SNR
    int fix;
}SV_IN_VIEW;

typedef struct _GPS_POS_DATA
{
    DWORD dwVersion;//Current version of GPSAL client is using.
    DWORD dwSize; //sizeof(_GPS_POS_DATA)

    DWORD dwUpdateFields;//The fields have been update
    DWORD dwValidFields;//Valid fields are specified in dwValidFields, based on GPS_VALID_XXX flags.

    UTC_TIME UTC_Time;//UTC

    UINT8 uFixQuality;//GPS_FIX_QUALITY
    UINT8 uFixType; //GPS_FIX_TYPE

    double dbLatitude; //Degrees latitude.  North is positive
    double dbLongtitude;//Degrees longitude.East is positive   
    double dbAltitude;//Altitude, above sea leave,meter

    float flGSpeed; //2D ground speed(m/s)
    float flVSpeed; //vertical speed(m/s)
    float flHeading;//track angle (deg)  True North=0

    float flPDOP;//Position Dilution Of Precision
    float flHDOP;//Horizontal Dilution Of Precision
    float flVDOP;//Horizontal Dilution Of Precision


    UINT8 sv_used_num; //GPGGA
    UINT8 sv_in_view_num; //GPGSV
    UINT8 sv_used_prn[MAX_GPS_SV_VIEW];
    SV_IN_VIEW sv_in_view[MAX_GPS_SV_VIEW];
}GPS_POS_DATA, *PGPS_POS_DATA;

#endif

//
//	Service states
//
#define SERVICE_STATE_OFF           0
#define SERVICE_STATE_ON            1
#define SERVICE_STATE_STARTING_UP   2
#define SERVICE_STATE_SHUTTING_DOWN 3
#define SERVICE_STATE_UNLOADING     4
#define SERVICE_STATE_UNINITIALIZED 5
#define SERVICE_STATE_UNKNOWN       0xffffffff

typedef struct _GPS_DEVICE_DATA
{
    DWORD dwVersion;
    DWORD dwSize;
    DWORD dwServiceState;
    DWORD dwDeviceState;

}GPS_DEVICE_DATA, *PGPS_DEVICE_DATA;

#ifdef __cplusplus
extern "C" {
#endif

GPSAL_API   HANDLE GPS_OpenDevice(HANDLE hNewLocationData,
                                HANDLE   hDeviceStateChange,
                                const TCHAR *szDeviceName,
                                DWORD    dwFlags
                                );

GPSAL_API    DWORD GPS_CloseDevice(HANDLE hGPSDevice);

GPSAL_API    DWORD GPS_GetPosition(HANDLE hGPSDevice,
                                GPS_POS_DATA *pGPSPosition,
                                DWORD         dwMaximumAge,
                                DWORD         dwFlags
                                );

GPSAL_API   DWORD GPS_GetDeviceState(GPS_DEVICE_DATA *pGPSDevice);


#ifdef __cplusplus
}
#endif

#endif//__GPSAL_H__