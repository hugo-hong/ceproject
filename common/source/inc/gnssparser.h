/************************************************************************
Filename:gnssParser.h

Description: head file of the CGNSSParser class.

Data:2012/12/03

Author: Hugo(mtk71195)
************************************************************************/
#ifndef _GNSSPARSER_H_
#define _GNSSPARSER_H_

#include <windows.h>
#include <utility>
#include <map>
#include "loopqueue.h"

using namespace std;

#define DEFAULT_BUF_LEN             (1024)

#define MAX_FIELD_LEN               (32)

#define MAX_NMEA_LEN                (100)

#define MAX_GPS_SV_NUM              (32)

#define MAX_GPS_SV_VIEW             (12)

#define MAX_GNSS_SV_VIEW            (24)

#define GNSS_VERSION_1               1

// GPS_VALID_XXX bit flags in GPS_POSITION structure are valid.
#define GNSS_VALID_UTC_TIME                                 0x00000001
#define GNSS_VALID_LATITUDE                                 0x00000002
#define GNSS_VALID_LONGITUDE                                0x00000004
#define GNSS_VALID_SPEED                                    0x00000008
#define GNSS_VALID_HEADING                                  0x00000010
#define GNSS_VALID_MAGNETIC_VARIATION                       0x00000020
#define GNSS_VALID_ALTITUDE_WRT_SEA_LEVEL                   0x00000040
#define GNSS_VALID_ALTITUDE_WRT_ELLIPSOID                   0x00000080
#define GNSS_VALID_POSITION_DILUTION_OF_PRECISION           0x00000100
#define GNSS_VALID_HORIZONTAL_DILUTION_OF_PRECISION         0x00000200
#define GNSS_VALID_VERTICAL_DILUTION_OF_PRECISION           0x00000400
#define GNSS_VALID_SATELLITE_COUNT                          0x00000800
#define GNSS_VALID_SATELLITES_USED_PRNS                     0x00001000
#define GNSS_VALID_SATELLITES_IN_VIEW                       0x00002000
#define GNSS_VALID_SATELLITES_IN_VIEW_PRNS                  0x00004000
#define GNSS_VALID_SATELLITES_IN_VIEW_ELEVATION             0x00008000
#define GNSS_VALID_SATELLITES_IN_VIEW_AZIMUTH               0x00010000
#define GNSS_VALID_SATELLITES_IN_VIEW_SIGNAL_TO_NOISE_RATIO 0x00020000

//GPS_UPDATA_XXX_BIT
#define GNSS_UPDATE_UTC                                      (1 << 0)
#define GNSS_UPDATE_LATITUDE                                 (1 << 1)
#define GNSS_UPDATE_LONGITUDE                                (1 << 2)
#define GNSS_UPDATE_ALTITUDE                                 (1 << 3)
#define GNSS_UPDATE_GSPEED                                   (1 << 4)
#define GNSS_UPDATE_VSPEED                                   (1 << 5)
#define GNSS_UPDATE_HEADING                                  (1 << 6)
#define GNSS_UPDATE_PDOP                                     (1 <<  7)
#define GNSS_UPDATE_HDOP                                     (1 <<  8)
#define GNSS_UPDATE_VDOP                                     (1 <<  9)
#define GNSS_UPDATE_SV_USED_NUM                              (1 <<  10)
#define GNSS_UPDATE_SV_VIEW_NUM                              (1 <<  11)
#define GNSS_UPDATE_SV_USED_PRN                              (1 <<  12)
#define GNSS_UPDATE_SV_VIEW_INFO                             (1 <<  13)

typedef enum
{
    GNSS_FIX_TYPE_INVALID = 1,
    GNSS_FIX_TYPE_2D = 2,
    GNSS_FIX_TYPE_3D = 3
} GNSS_FIX_TYPE;

typedef enum
{
    GNSS_FIX_Q_INVALID = 0,//没有定位
    GNSS_FIX_Q_GPS = 1, //非差分定位
    GNSS_FIX_Q_DGPS = 2,//差分定位
    GNSS_FIX_Q_EST = 6 //估算
}GNSS_FIX_QUALITY;

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


typedef struct _GNSS_POS_DATA
{
    DWORD dwVersion;//Current version of GPSAL client is using.
    DWORD dwSize; //sizeof(_GPS_POS_DATA)

    DWORD dwUpdateFields;//The fields have been update
    DWORD dwValidFields;//Valid fields are specified in dwValidFields, based on GPS_VALID_XXX flags.

    UTC_TIME UTC_Time;//UTC

    UINT8 uFixQuality;//GNSS_FIX_QUALITY
    UINT8 uFixType; //GNSS_FIX_TYPE

    double dbLatitude; //Degrees latitude.  North is positive
    double dbLongtitude;//Degrees longitude.East is positive   
    double dbAltitude;//Altitude, above sea leave,meter

    float flGSpeed; //2D ground speed(m/s)
    float flVSpeed; //vertical speed(m/s)
    float flHeading;//track angle (deg)  True North=0

    float flPDOP;//Position Dilution Of Precision
    float flHDOP;//Horizontal Dilution Of Precision
    float flVDOP;//Vertical Dilution Of Precision


    UINT8 sv_used_num; //GPGGA
    UINT8 sv_in_view_num; //GPGSV
    UINT8 sv_used_prn[MAX_GNSS_SV_VIEW];
    SV_IN_VIEW sv_in_view[MAX_GNSS_SV_VIEW];
}GNSS_POS_DATA, *PGNSS_POS_DATA;


class CGNSSParser
{
public:
    CGNSSParser();
    virtual ~CGNSSParser();

    BOOL Init(void);
    void DeInit(void);    
    
    DWORD ParseNMEA(char *pBuf, int length);
    void UpdateSatelliteInfo(void);
    void FillGNSSPosition(GNSS_POS_DATA *pPos, DWORD dwMaximumAge);

protected:    
    DWORD DispatchNMEA(char *pBuf, int dwLen);

    BOOL NMEA_DoCheck(char *pSentence, int length);
    char* NMEA_FetchSeg(char *pSrc, const char *pSeg = ",");
    BOOL NMEA_FetchField(char *pSrc, int inBufLen, char *pDes, int outBufLen);
    double NMEA_FetchDegree(double degValue);

    void NMEA_GGAParser(char *pSentence, int length);
    void NMEA_GSAParser(char *pSentence, int length);
    void NMEA_GSVParser(char *pSentence, int length);
    void NMEA_VTGParser(char *pSentence, int length);
    void NMEA_GLLParser(char *pSentence, int length);
    void NMEA_RMCParser(char *pSentence, int length);

    //GNSS support
    void NMEA_GNSSGSAParser(char *pSentence, int length);
    void NMEA_GNSSGSVParser(char *pSentence, int length);    

    //Glonass support
    void NMEA_GNGSAParser(char *pSentence, int length);    
    void NMEA_GLGSVParser(char *pSentence, int length);
    void NMEA_GNRMCParser(char *pSentence, int length);

    //Beidou support
    void NMEA_BDGSAParser(char *pSentence, int length);
    void NMEA_BDGSVParser(char *pSentence, int length);    

    BOOL IsRealTimeData(DWORD dwMaximumAge);
    

private:    
    DWORD m_dwLastUpdateTime;

    CLoopQueue *m_pLoopQueue;
    GNSS_POS_DATA m_gnssPosData;
};



#endif//_GNSSPARSER_H_