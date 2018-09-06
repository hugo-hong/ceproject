/************************************************************************ 
 Filename:gnssparser.cpp

 Description: implementation of the CGNSSParser class.

 Data:2012/12/03

 Author: Hugo(mtk71195)
************************************************************************/

#include "stdafx.h"
#include "gnssparser.h"
#include <assert.h>


CGNSSParser::CGNSSParser()
{
    m_dwLastUpdateTime = 0;
    m_pLoopQueue = NULL;
    memset(&m_gnssPosData, 0, sizeof(GNSS_POS_DATA));
}

CGNSSParser::~CGNSSParser()
{
    DeInit();
}

BOOL CGNSSParser::Init(void)
{    
    m_pLoopQueue = new CLoopQueue(DEFAULT_BUF_LEN);
    if (NULL == m_pLoopQueue)
    {
        RETAILMSG(1, (_T("[CGNSSParser]Init:No enough memory!\r\n")));
        return FALSE;
    }   

    return TRUE;
}

void CGNSSParser::DeInit(void)
{
    if (m_pLoopQueue)
    {
        delete m_pLoopQueue;
        m_pLoopQueue = NULL;
    }    
}

DWORD CGNSSParser::ParseNMEA(char *pBuf, int length)
{    
    UINT realDataLen = 0, dataHeader = 0;
    char tempBuf[DEFAULT_BUF_LEN] = {0};
    char realData[DEFAULT_BUF_LEN] = {0};
    DWORD dwLastDataLen = 0;
    BOOL bFindFirstFlag = 0;
    DWORD dwResult = 0;

    if (NULL == pBuf || 0 == length)
    {
        RETAILMSG(1, (_T("[CGNSSParser]ParseNMEA:invalid parameters!!!\r\n")));
        return 0;
    }

    //use the temp buffer
    dwLastDataLen = m_pLoopQueue->GetDataLength();    
    dwLastDataLen = m_pLoopQueue->Read(tempBuf, dwLastDataLen);    
    if ((DEFAULT_BUF_LEN - dwLastDataLen) < length)
    {      
        DWORD dwDataLen = DEFAULT_BUF_LEN - dwLastDataLen;
        memmove(tempBuf + dwLastDataLen, pBuf, dwDataLen);
        m_pLoopQueue->Reset();
        m_pLoopQueue->Write(pBuf + dwDataLen, length - dwDataLen);
        length = DEFAULT_BUF_LEN;
    }
    else
    {
        memmove(tempBuf + dwLastDataLen, pBuf, length);
        length += dwLastDataLen;
        m_pLoopQueue->Reset();        
    }

    //invlaid update bits
    m_gnssPosData.dwUpdateFields = 0;

    //parse data
    for (int i = 0; i < length; )
    {
        //begain with '$', and end with"\r\n" as an sentence
        if (tempBuf[i] == '$')
        {
            bFindFirstFlag = TRUE;
            dataHeader = i;
        }
        else if (tempBuf[i] == '\r' && tempBuf[i + 1] == '\n')
        {
            if (bFindFirstFlag)
            {
                i += 2;
                memset(realData, 0, sizeof(realData));
                memmove(realData, &tempBuf[dataHeader], i - dataHeader);                
                dwResult = DispatchNMEA(realData, i - dataHeader);

                bFindFirstFlag = FALSE;

                continue;
            }
            else
            {
                //this sentence incompetele discard data
                i += 2;

                continue;
            }
        }

        i++;
    }

    if (bFindFirstFlag)
    {
        //save the remain data to backup buffer
        m_pLoopQueue->Write(&tempBuf[dataHeader], length - dataHeader);
    }

    if (m_gnssPosData.dwUpdateFields != 0)
    {
        m_dwLastUpdateTime = GetTickCount();
    }
   
    return dwResult;
}

DWORD CGNSSParser::DispatchNMEA(char *pBuf, int length)
{
    DWORD dwResult = ERROR_SUCCESS;
   
    if (NULL == pBuf || 0 == length)
    {
        return ERROR_INVALID_PARAMETER;
    }

    if (!NMEA_DoCheck(pBuf, length))
    {
        RETAILMSG(1, (_T("[CGNSSParser]DispatchNMEA:invalid nmea sentence!!!\r\n")));
        return ERROR_INVALID_PARAMETER;
    }

    //dispatch nmea sentence
    if (0 == strncmp(pBuf, "$GPGGA", strlen("$GPGGA")))
    {
        NMEA_GGAParser(pBuf, length);
    }
    else if (0 == strncmp(pBuf, "$GPGSA", strlen("$GPGSA")))
    {
        NMEA_GSAParser(pBuf, length);
    }   
    else if (0 == strncmp(pBuf, "$GPGSV", strlen("$GPGSV")))
    {
        NMEA_GSVParser(pBuf, length);
    }
    else if (0 == strncmp(pBuf, "$GPRMC", strlen("$GPRMC")))
    {
        NMEA_RMCParser(pBuf, length);
    }
    else if (0 == strncmp(pBuf, "$GPVTG", strlen("$GPVTG")))
    {
        NMEA_VTGParser(pBuf, length);
    }
    else if (0 == strncmp(pBuf, "$GPGLL", strlen("$GPGLL")))
    {
        NMEA_GLLParser(pBuf, length);
    }  
    else if (0 == strncmp(pBuf, "$GNGSA", strlen("$GNGSA")))
    {
        NMEA_GNGSAParser(pBuf, length);
    }  
    else if (0 == strncmp(pBuf, "$GNRMC", strlen("$GNRMC")))
    {
        NMEA_GNRMCParser(pBuf, length);
    }  
    else if (0 == strncmp(pBuf, "$GLGSV", strlen("$GLGSV")))
    {
        NMEA_GLGSVParser(pBuf, length);
    }  
    else if (0 == strncmp(pBuf, "$BDGSA", strlen("$BDGSA")))
    {
        NMEA_BDGSAParser(pBuf, length);
    }
    else if (0 == strncmp(pBuf, "$BDGSV", strlen("$BDGSV")))
    {
        NMEA_BDGSVParser(pBuf, length);
    }

    return dwResult;
}

BOOL CGNSSParser::NMEA_DoCheck(char *pSentence, int length)
{
    unsigned char sum1 = 0;
    unsigned char sum2 = 0;
    BOOL fgRet = 0;
    int i = 0;
    
    for (i = 1; i < (length - 2); i++)
    {
        if (pSentence[i] != '*')
        {
            sum1 ^= pSentence[i];
        }
        else
        {
            //'*'后2位十六进制数字是checksum
            i++;
            char temp[4] = {0};
            temp[0] = pSentence[i++];
            temp[1] = pSentence[i++];
            sscanf(temp, "%x", &sum2);
            break;
        }
    }

    //check sentence if exist "\r\n"
    if (pSentence[i] != '\r' || pSentence[++i] != '\n')
    {
        fgRet = 0;
    }
    else 
    {
        fgRet = (sum1 == sum2);
    }

    return fgRet;
}

char* CGNSSParser::NMEA_FetchSeg(char *pSrc, const char *pSeg)
{
    if (NULL == pSrc)
    {
        return NULL;
    }

    if (NULL == pSeg)
    {
        return pSrc;
    }
    
    char *pDst = NULL;

    pDst = strstr(pSrc, pSeg);
    if (NULL == pDst)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_FetchSeg:NMEA end!\r\n")));
        return NULL;
    }
    else
    {
        pDst++;
    }

    return pDst;

}


BOOL CGNSSParser::NMEA_FetchField(char *pSrc, int inBufLen, char *pDes, int outBufLen)
{
    if (NULL == pSrc || NULL == pDes)
    {
        return FALSE;
    }

    if (inBufLen > MAX_NMEA_LEN || inBufLen == 0 ||
        outBufLen > MAX_FIELD_LEN || outBufLen == 0)
    {
        return FALSE;
    }

    char *pEnd = NULL;

    pEnd = strstr(pSrc, ",");
    if (NULL == pEnd)
    {
        pEnd = strstr(pSrc, "*");
    }

    int len = pEnd - pSrc;
    if (len > MAX_FIELD_LEN - 1)
    {       
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_FetchField:NMEA error\r\n")));
        return FALSE;        
    }
    else
    {
        memset(pDes, 0, outBufLen);
        strncpy(pDes, pSrc, len);        
    }

    return TRUE;
}

double CGNSSParser::NMEA_FetchDegree(double degValue)
{
    double dbDeg = 0.0;
    int intPart = 0;
    double fraPart = 0.0;

    degValue = fabs(degValue);

    // Lat/long coordinates are in form ddmm.mmmm - we need to shift
    // dd.mmmmm so we divide by 100.
    degValue /= 100;

    intPart = (int)degValue;
    fraPart = ((degValue - intPart) * 100.0) / 60.0;

    dbDeg = (double)(intPart) + fraPart;

    return dbDeg;
}

void CGNSSParser::NMEA_GGAParser(char *pSentence, int length)
{    
    //Global Positioning System Fix Data(GGA) GPS定位信息
    //eg.$GPGGA,144437.000,2446.367638,N,12101.356226,E,1,9,0.95,155.696,M,15.057,M,,*58
    if (NULL == pSentence)
    {
        return;
    }

    char field[MAX_FIELD_LEN] = {0};    
    char *pNext = NULL;     

    //get the first field(hhmmss.ms)
    pNext = NMEA_FetchSeg(pSentence);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get time error\r\n")));
        return;
    }    
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        char item[MAX_FIELD_LEN] = {0};
        int len = 0;

        //get hour                       
        memcpy(item, field, 2);
        m_gnssPosData.UTC_Time.hour = atoi(item);
        len += 2;

        //get minue
        memcpy(item, &field[len], 2);
        m_gnssPosData.UTC_Time.min = atoi(item);
        len += 2;

        //get second
        memcpy(item, &field[len], 2);
        m_gnssPosData.UTC_Time.sec = atoi(item);
        len += 2;

        if (field[len] == '.')
        {
            len++;

            //get msecond
            memcpy(item, &field[len], strlen(field) -len);
            m_gnssPosData.UTC_Time.msec = atoi(item);
        } 

        //update UTC ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_UTC;
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get dbLatitude error\r\n")));
        return;
    }      
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //get the Latitude field   
        double dbLatitude = atof(field);
        m_gnssPosData.dbLatitude = NMEA_FetchDegree(dbLatitude);

        //update Latitude ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_LATITUDE;
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get lat direction error\r\n")));
        return;
    }          
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //get the N or S  
        if (field[0] == 'S')
        {
            m_gnssPosData.dbLatitude = -m_gnssPosData.dbLatitude;
        }
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get dbLongtitude error\r\n")));
        return;
    }         
    if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
       //get the Longtitude   
       double dbLongtitude = atof(field);
       m_gnssPosData.dbLongtitude = NMEA_FetchDegree(dbLongtitude);

       //update Longtitude ok
       m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_LONGITUDE;
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get dbLongtitude dirction error\r\n")));
        return;
    }        
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //get the E or W  
        if (field[0] == 'W')
        {
            m_gnssPosData.dbLongtitude = -m_gnssPosData.dbLongtitude;
        }
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get fix quality error\r\n")));
        return;
    }        
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //get the position typde  
        m_gnssPosData.uFixQuality = atoi(field);
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get sv used error\r\n")));
        return;
    }      
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //get the satelite in used number  
        m_gnssPosData.sv_used_num = atoi(field);

        //update satelite in used number 
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_SV_USED_NUM;
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get flHDOP error\r\n")));
        return;
    }       
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //get the HDOP  
        m_gnssPosData.flHDOP = (float)atof(field);

        //update HDOP ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_HDOP;
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get sea leavel error\r\n")));
        return;
    }        
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //get the altitude sea level 
        m_gnssPosData.dbAltitude = atof(field);

        //update altitude ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_ALTITUDE;
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:ERR NMEA sentence error\r\n")));
        return;
    }   

    //ignore UNIT

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get Altitude error\r\n")));
        return;
    }        
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
       //altitude earth   
       m_gnssPosData.dbAltitude += atof(field);
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get unit error\r\n")));
        return;
    }        
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //check unit 
        if (0 == strcmp(field, "KM"))
        {
            m_gnssPosData.dbAltitude *= 1000.0;
        }        
    }    
}

void CGNSSParser::NMEA_GSAParser(char *pSentence, int length)
{    
    //GPS DOP and Active Statellites(GSA)
    //eg.$GPGSA,A,3,03,19,27,23,13,16,15,11,07,,,,1.63,0.95,1.32*03

    if (NULL == pSentence)
    {
        return;
    }

    char field[MAX_FIELD_LEN] = {0};    
    char *pNext = NULL;   

    //GPS MODE A/M     
    pNext = NMEA_FetchSeg(pSentence);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GSAParser:get mode error\r\n")));
        return;
    }     
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        if (field[0] != 'A' && field[0] != 'M')
        {
            RETAILMSG(1, (_T("[CGNSSParser]NMEA_GSAParser:gps mode error\r\n")));
            return;
        }
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get fix type error\r\n")));
        return;
    }
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //GPS fixed type  
        m_gnssPosData.uFixType = atoi(field);
    }    

    //fetch prn
    memset(m_gnssPosData.sv_used_prn, 0, sizeof(m_gnssPosData.sv_used_prn));
    int sv_used_cnt = 0;    
    for (int i = 0; i < MAX_GPS_SV_VIEW; i++)
    {        
        //move to next seg
        pNext = NMEA_FetchSeg(pNext);
        if (NULL == pNext)
        {
            RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get prn error\r\n")));
            return;
        }  
        else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
        {
            //get used prn
            if (strlen(field) > 0)
            {
                m_gnssPosData.sv_used_prn[sv_used_cnt++] = (UINT8)atoi(field);
            }            
        }        
    }
    //update sv used prn 
    m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_SV_USED_PRN;

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get flPDOP error\r\n")));
        return;
    }        
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //PDOP 
        m_gnssPosData.flPDOP = (float)atof(field);

        //update PDOP ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_PDOP;
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get flHDOP error\r\n")));
        return;
    }        
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //flHDOP 
        m_gnssPosData.flHDOP = (float)atof(field);

        //update HDOP ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_HDOP;
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get flVDOP error\r\n")));
        return;
    }          
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //VDOP 
        m_gnssPosData.flVDOP = (float)atof(field);

        //update VDOP ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_VDOP;
    }
}

void CGNSSParser::NMEA_GSVParser(char *pSentence, int length)
{    
    //GPS Statellites in View(GSV)
    //eg.$GPGSV,3,1,09,03,63,020,43,19,76,257,37,27,14,320,30,23,39,228,37*79
    //eg.$GPGSV,3,2,09,13,38,274,38,16,31,058,37,15,16,055,34,11,16,192,32*76
    //eg.$GPGSV,3,3,09,07,15,043,26*40

    if (NULL == pSentence)
    {
        return;
    }

    char field[MAX_FIELD_LEN] = {0};    
    char *pNext = NULL;      

    //get sentence number and index
    int num = 0, index = 0;      
    pNext = NMEA_FetchSeg(pSentence);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GSVParser:get num error\r\n")));
        return;
    }    
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        num = atoi(field);
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GSVParser: get index error\r\n")));
        return;
    } 
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        index = atoi(field);
    }   

    if (1 == index)
    {
        //update sv informtion
        memset(m_gnssPosData.sv_in_view, 0, sizeof(m_gnssPosData.sv_in_view));
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GSVParser:get sv num error\r\n")));
        return;
    }
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        m_gnssPosData.sv_in_view_num = atoi(field);

        //update satelite in view number 
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_SV_VIEW_NUM;
    }

    if (0 == m_gnssPosData.sv_in_view_num)
    {
        return;
    }

    //calc array index
    int svIndex = (index - 1) * 4;

    //get Statellites information
    for (int i = 0; i < 4; )
    {
        //move to next seg
        pNext = NMEA_FetchSeg(pNext);
        if (NULL == pNext)
        {
            RETAILMSG(1, (_T("[CGNSSParser]NMEA_GSVParser:get prn error\r\n")));
            return;
        }
        else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
        {
            if (strlen(field) > 0)
            {
                m_gnssPosData.sv_in_view[svIndex + i].prn = atoi(field);
            }            
        }

        //move to next seg
        pNext = NMEA_FetchSeg(pNext);
        if (NULL == pNext)
        {
            RETAILMSG(1, (_T("[CGNSSParser]NMEA_GSVParser:get elev error\r\n")));
            return;
        }
        else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
        {
            if (strlen(field) > 0)
            {
                m_gnssPosData.sv_in_view[svIndex + i].elevation = atoi(field);
            }            
        }

        //move to next seg
        pNext = NMEA_FetchSeg(pNext);
        if (NULL == pNext)
        {
            RETAILMSG(1, (_T("[CGNSSParser]NMEA_GSVParser:get azim error\r\n")));
            return;
        }
        else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
        {
            if (strlen(field) > 0)
            {
                m_gnssPosData.sv_in_view[svIndex + i].azim = atoi(field);
            }            
        }

        //move to next seg
        pNext = NMEA_FetchSeg(pNext);
        if (NULL == pNext)
        {
            RETAILMSG(1, (_T("[CGNSSParser]NMEA_GSVParser:get snr error\r\n")));
            return;
        }
        else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
        {
            if (strlen(field) > 0 && field[0] != '*')
            {
                m_gnssPosData.sv_in_view[svIndex + i].snr = atoi(field);
            }  
            else
            {
                m_gnssPosData.sv_in_view[svIndex + i].snr = 0;
            }
        }

        if (svIndex + (++i) == m_gnssPosData.sv_in_view_num)
        {
            break;
        }
    }

    //update Statellites information
    m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_SV_VIEW_INFO;
}

void CGNSSParser::NMEA_RMCParser(char *pSentence, int length)
{    
    //Recommended Minimum Specific GPS/TRANSIT Data (RMC)推荐最小定位信息
    //eg.$GPRMC,073446.000,A,2446.3752,N,12101.3708,E,0.002,22.08,121006,,,A*6C

    if (NULL == pSentence)
    {
        return;
    }

    char field[MAX_FIELD_LEN] = {0};    
    char *pNext = NULL; 

    //get the first field(hhmmss.ms)  
    pNext = NMEA_FetchSeg(pSentence);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_RMCParser:get time error\r\n")));
        return;
    }    
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        char item[MAX_FIELD_LEN] = {0};
        int len = 0;

        //get hour                       
        memcpy(item, field, 2);
        m_gnssPosData.UTC_Time.hour = atoi(item);
        len += 2;

        //get minue
        memcpy(item, &field[len], 2);
        m_gnssPosData.UTC_Time.min = atoi(item);
        len += 2;

        //get second
        memcpy(item, &field[len], 2);
        m_gnssPosData.UTC_Time.sec = atoi(item);
        len += 2;

        if (field[len] == '.')
        {
            len++;

            //get msecond
            memcpy(item, &field[len], strlen(field) -len);
            m_gnssPosData.UTC_Time.msec = atoi(item);
        } 

        //update UTC ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_UTC;
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GSVParser:get fix mode error\r\n")));
        return;
    }
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        if (field[0] == 'A')
        {
            if (m_gnssPosData.uFixType == 0)
            {
                m_gnssPosData.uFixType = GNSS_FIX_TYPE_2D;//assume
            }

            if (m_gnssPosData.uFixQuality == 0)
            {
                m_gnssPosData.uFixQuality = GNSS_FIX_Q_INVALID;
            }
        }
        else
        {
            m_gnssPosData.uFixType = GNSS_FIX_TYPE_INVALID;
            m_gnssPosData.uFixQuality = GNSS_FIX_Q_INVALID;
        }
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GSVParser:get dbLatitude error\r\n")));
        return;
    }
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        double dbLatitude = atof(field);
        m_gnssPosData.dbLatitude = NMEA_FetchDegree(dbLatitude);

        //update Latitude ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_LATITUDE;
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get lat direction error\r\n")));
        return;
    }         
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //get the N or S  
        if (field[0] == 'S')
        {
            m_gnssPosData.dbLatitude = -m_gnssPosData.dbLatitude;
        }
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GSVParser:get dbLongtitude error\r\n")));
        return;
    }
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        double dbLongtitude = atof(field);
        m_gnssPosData.dbLongtitude = NMEA_FetchDegree(dbLongtitude);

        //update Longtitude ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_LONGITUDE;
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get longti direction error\r\n")));
        return;
    }         
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //get the E or W  
        if (field[0] == 'W')
        {
            m_gnssPosData.dbLongtitude = -m_gnssPosData.dbLongtitude;
        }
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get speed error\r\n")));
        return;
    }         
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //get the ground speed
        m_gnssPosData.flGSpeed = (float)atof(field);

        //update ground speed ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_GSPEED;
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get track error\r\n")));
        return;
    }         
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //get the track heading
        m_gnssPosData.flHeading = (float)atof(field);

        //update track heading ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_HEADING;
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get date error\r\n")));
        return;
    }         
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //get date ddmmyy(日月年)
        char item[MAX_FIELD_LEN] = {0};
        int len = 0;

        //get date                       
        memcpy(item, field, 2);
        m_gnssPosData.UTC_Time.mday = atoi(item);
        len += 2;

        //get month
        memcpy(item, &field[len], 2);
        m_gnssPosData.UTC_Time.month = atoi(item);
        len += 2;

        //get year
        memcpy(item, &field[len], 2);
        m_gnssPosData.UTC_Time.year = atoi(item) + 2000;
        len += 2;

        //update UTC ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_UTC;
    }

    //the rest of data don't interest

}

void CGNSSParser::NMEA_VTGParser(char *pSentence, int length)
{    
    //Track Made Good and Ground Speed(VTG) 地面速度信息
    //eg.$GPVTG,159.16,T,,M,0.013,N,0.023,K,A*34

}

void CGNSSParser::NMEA_GLLParser(char *pSentence, int length)
{    
    //Geographic Position(GLL)定位地理信息
    //eg.$GPGLL,2446.367638,N,12101.356226,E,144437.000,A,A*56

}

void CGNSSParser::NMEA_GNSSGSAParser(char *pSentence, int length)
{
    if (NULL == pSentence)
    {
        return;
    }

    char field[MAX_FIELD_LEN] = {0};    
    char *pNext = NULL;   

    //GPS MODE A/M     
    pNext = NMEA_FetchSeg(pSentence);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GSAParser:get mode error\r\n")));
        return;
    }     
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        if (field[0] != 'A' && field[0] != 'M')
        {
            RETAILMSG(1, (_T("[CGNSSParser]NMEA_GSAParser:gps mode error\r\n")));
            return;
        }
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get fix type error\r\n")));
        return;
    }
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //GPS fixed type  
        m_gnssPosData.uFixType = atoi(field);
    }    

    //fetch prn
    int sv_used_cnt = 0;
    for (int i = 0; i < MAX_GNSS_SV_VIEW; i++)
    {
        if (m_gnssPosData.sv_used_prn[i] != 0)
        {
            sv_used_cnt++;
        }
        else
        {
            //move to next seg
            pNext = NMEA_FetchSeg(pNext);
            if (NULL == pNext)
            {
                RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get prn error\r\n")));
                return;
            }  
            else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
            {
                //get used prn
                if (strlen(field) > 0)
                {
                    m_gnssPosData.sv_used_prn[sv_used_cnt++] = (UINT8)atoi(field);
                }            
            }        
        }
    }    
    //update sv used prn 
    m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_SV_USED_PRN;

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get flPDOP error\r\n")));
        return;
    }        
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //PDOP 
        m_gnssPosData.flPDOP = (float)atof(field);

        //update PDOP ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_PDOP;
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get flHDOP error\r\n")));
        return;
    }        
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //flHDOP 
        m_gnssPosData.flHDOP = (float)atof(field);

        //update HDOP ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_HDOP;
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_GGAParser:get flVDOP error\r\n")));
        return;
    }          
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        //VDOP 
        m_gnssPosData.flVDOP = (float)atof(field);

        //update VDOP ok
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_VDOP;
    }
}

void CGNSSParser::NMEA_GNSSGSVParser(char *pSentence, int length)
{
    if (NULL == pSentence)
    {
        return;
    }

    char field[MAX_FIELD_LEN] = {0};    
    char *pNext = NULL;      

    //get sentence number and index
    int num = 0, index = 0;      
    pNext = NMEA_FetchSeg(pSentence);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_BDGSVParser:get num error\r\n")));
        return;
    }    
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        num = atoi(field);
    }

    //move to next seg
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_BDGSVParser: get index error\r\n")));
        return;
    } 
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        index = atoi(field);
    }   

    //update sv informtion
    int gps_sv_in_view = m_gnssPosData.sv_in_view_num;
    if (1 == index)
    {        
        memset(&m_gnssPosData.sv_in_view[gps_sv_in_view], 0, sizeof(SV_IN_VIEW)*(MAX_GNSS_SV_VIEW-gps_sv_in_view));
    }

    //move to next seg
    int gnss_sv_in_view = 0;
    pNext = NMEA_FetchSeg(pNext);
    if (NULL == pNext)
    {
        RETAILMSG(1, (_T("[CGNSSParser]NMEA_BDGSVParser:get sv num error\r\n")));
        return;
    }
    else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
    {
        gnss_sv_in_view = atoi(field);
        m_gnssPosData.sv_in_view_num += gnss_sv_in_view;

        //update satelite in view number 
        m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_SV_VIEW_NUM;
    }

    if (0 == gnss_sv_in_view)
    {
        return;
    }

    //calc array index
    int svIndex = (index - 1) * 4;
    svIndex += gps_sv_in_view;
    //get Statellites information
    for (int i = 0; i < 4; )
    {
        //move to next seg
        pNext = NMEA_FetchSeg(pNext);
        if (NULL == pNext)
        {
            RETAILMSG(1, (_T("[CGNSSParser]NMEA_BDGSVParser:get prn error\r\n")));
            return;
        }
        else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
        {
            if (strlen(field) > 0)
            {
                m_gnssPosData.sv_in_view[svIndex + i].prn = atoi(field);
            }            
        }

        //move to next seg
        pNext = NMEA_FetchSeg(pNext);
        if (NULL == pNext)
        {
            RETAILMSG(1, (_T("[CGNSSParser]NMEA_BDGSVParser:get elev error\r\n")));
            return;
        }
        else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
        {
            if (strlen(field) > 0)
            {
                m_gnssPosData.sv_in_view[svIndex + i].elevation = atoi(field);
            }            
        }

        //move to next seg
        pNext = NMEA_FetchSeg(pNext);
        if (NULL == pNext)
        {
            RETAILMSG(1, (_T("[CGNSSParser]NMEA_BDGSVParser:get azim error\r\n")));
            return;
        }
        else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
        {
            if (strlen(field) > 0)
            {
                m_gnssPosData.sv_in_view[svIndex + i].azim = atoi(field);
            }            
        }

        //move to next seg
        pNext = NMEA_FetchSeg(pNext);
        if (NULL == pNext)
        {
            RETAILMSG(1, (_T("[CGNSSParser]NMEA_BDGSVParser:get snr error\r\n")));
            return;
        }
        else if (NMEA_FetchField(pNext, strlen(pNext), field, MAX_FIELD_LEN))
        {
            if (strlen(field) > 0 && field[0] != '*')
            {
                m_gnssPosData.sv_in_view[svIndex + i].snr = atoi(field);
            }  
            else
            {
                m_gnssPosData.sv_in_view[svIndex + i].snr = 0;
            }
        }

        if (svIndex + (++i) == (gnss_sv_in_view + gps_sv_in_view))
        {
            break;
        }
    }

    //update Statellites information
    m_gnssPosData.dwUpdateFields |= GNSS_UPDATE_SV_VIEW_INFO;
}

void CGNSSParser::NMEA_GNGSAParser(char *pSentence, int length)
{
    NMEA_GNSSGSAParser(pSentence, length);
}

void CGNSSParser::NMEA_GLGSVParser(char *pSentence, int length)
{
    NMEA_GNSSGSVParser(pSentence, length);
}

void CGNSSParser::NMEA_GNRMCParser(char *pSentence, int length)
{
    NMEA_RMCParser(pSentence, length);
}

void CGNSSParser::NMEA_BDGSAParser(char *pSentence, int length)
{
    NMEA_GNSSGSAParser(pSentence, length);
}

void CGNSSParser::NMEA_BDGSVParser(char *pSentence, int length)
{
    NMEA_GNSSGSVParser(pSentence, length);
}


void CGNSSParser::UpdateSatelliteInfo(void)
{
    int i = 0;
    int j = 0;
    int k = 0;

    if (m_gnssPosData.sv_in_view_num == 0 ||
        m_gnssPosData.sv_used_num == 0)
    {
        return;
    }

    for (i = 0; i < m_gnssPosData.sv_in_view_num; i++)
    {
        for (j = 0; j < m_gnssPosData.sv_used_num; j++)
        {
            if (m_gnssPosData.sv_in_view[i].prn == m_gnssPosData.sv_used_prn[j])
            {
                m_gnssPosData.sv_in_view[i].fix = TRUE;
                k++;
                if (k == m_gnssPosData.sv_used_num)
                {
                    return;
                }
            }
        }
    }
}

void CGNSSParser::FillGNSSPosition(GNSS_POS_DATA *pPos, DWORD dwMaximumAge)
{   
    if (NULL == pPos)
    {
        return;
    }    

    assert(pPos->dwVersion == GNSS_VERSION_1);
    assert(pPos->dwSize == sizeof(GNSS_POS_DATA));

    if (!IsRealTimeData(dwMaximumAge))
    {
        return;
    }

    DEBUGMSG(0, (_T("[CGNSSParser]FillGPSPosition:dwUpdateFields=%d\r\n"), m_gnssPosData.dwUpdateFields));

    memset(pPos, 0, sizeof(GNSS_POS_DATA));

    pPos->dwVersion = GNSS_VERSION_1;
    pPos->dwSize = sizeof(GNSS_POS_DATA);

    if (m_gnssPosData.dwUpdateFields & GNSS_UPDATE_UTC)
    {
        pPos->dwUpdateFields |= GNSS_UPDATE_UTC;
        pPos->dwValidFields |= GNSS_VALID_UTC_TIME;
        pPos->UTC_Time = m_gnssPosData.UTC_Time;
    }

    if (m_gnssPosData.dwUpdateFields & GNSS_UPDATE_LATITUDE)
    {
        pPos->dwUpdateFields |= GNSS_UPDATE_LATITUDE;
        pPos->dwValidFields |= GNSS_VALID_LATITUDE;
        pPos->dbLatitude = m_gnssPosData.dbLatitude;
    }

    if (m_gnssPosData.dwUpdateFields & GNSS_UPDATE_LONGITUDE)
    {
        pPos->dwUpdateFields |= GNSS_UPDATE_LONGITUDE;
        pPos->dwValidFields |= GNSS_VALID_LONGITUDE;
        pPos->dbLongtitude = m_gnssPosData.dbLongtitude;
    }

    if (m_gnssPosData.dwUpdateFields & GNSS_UPDATE_ALTITUDE)
    {
        pPos->dwUpdateFields |= GNSS_UPDATE_ALTITUDE;
        pPos->dwValidFields |= GNSS_VALID_ALTITUDE_WRT_SEA_LEVEL;
        pPos->dwValidFields |= GNSS_VALID_ALTITUDE_WRT_ELLIPSOID;
        pPos->dbAltitude = m_gnssPosData.dbAltitude;
    }

    if (m_gnssPosData.dwUpdateFields & GNSS_UPDATE_GSPEED)
    {
        pPos->dwUpdateFields |= GNSS_UPDATE_GSPEED;
        pPos->dwValidFields |= GNSS_VALID_SPEED;
        pPos->flGSpeed = m_gnssPosData.flGSpeed;
    }

    if (m_gnssPosData.dwUpdateFields & GNSS_UPDATE_VSPEED)
    {
        pPos->dwUpdateFields |= GNSS_UPDATE_VSPEED;
        //pPos->dwValidFields |= GNSS_UPDATE_VSPEED;
        pPos->flVSpeed = m_gnssPosData.flVSpeed;
    }

    if (m_gnssPosData.dwUpdateFields & GNSS_UPDATE_HEADING)
    {
        pPos->dwUpdateFields |= GNSS_UPDATE_HEADING;
        pPos->dwValidFields |= GNSS_VALID_HEADING;
        pPos->flHeading = m_gnssPosData.flHeading;
    }

    if (m_gnssPosData.dwUpdateFields & GNSS_UPDATE_PDOP)
    {
        pPos->dwUpdateFields |= GNSS_UPDATE_PDOP;
        pPos->dwValidFields |= GNSS_VALID_POSITION_DILUTION_OF_PRECISION;
        pPos->flPDOP = m_gnssPosData.flPDOP;
    }

    if (m_gnssPosData.dwUpdateFields & GNSS_UPDATE_HDOP)
    {
        pPos->dwUpdateFields |= GNSS_UPDATE_HDOP;
        pPos->dwValidFields |= GNSS_VALID_HORIZONTAL_DILUTION_OF_PRECISION;
        pPos->flHDOP = m_gnssPosData.flHDOP;
    }

    if (m_gnssPosData.dwUpdateFields & GNSS_UPDATE_VDOP)
    {
        pPos->dwUpdateFields |= GNSS_UPDATE_VDOP;
        pPos->dwValidFields |= GNSS_VALID_VERTICAL_DILUTION_OF_PRECISION;
        pPos->flVDOP = m_gnssPosData.flVDOP;
    }

    if (m_gnssPosData.dwUpdateFields & GNSS_UPDATE_SV_USED_NUM)
    {
        pPos->dwUpdateFields |= GNSS_UPDATE_SV_USED_NUM;
        pPos->dwValidFields |= GNSS_VALID_SATELLITES_USED_PRNS;
        pPos->sv_used_num = m_gnssPosData.sv_used_num;
    }

    if (m_gnssPosData.dwUpdateFields & GNSS_UPDATE_SV_VIEW_NUM)
    {
        pPos->dwUpdateFields |= GNSS_UPDATE_SV_VIEW_NUM;
        pPos->dwValidFields |= GNSS_VALID_SATELLITE_COUNT;
        pPos->sv_in_view_num = m_gnssPosData.sv_in_view_num;
    }

    if (m_gnssPosData.dwUpdateFields & GNSS_UPDATE_SV_USED_PRN)
    {
        pPos->dwUpdateFields |= GNSS_UPDATE_SV_USED_PRN;
        pPos->dwValidFields |= GNSS_VALID_SATELLITES_USED_PRNS;
        memcpy(pPos->sv_used_prn, m_gnssPosData.sv_used_prn, sizeof(pPos->sv_used_prn));        
    }

    if (m_gnssPosData.dwUpdateFields & GNSS_UPDATE_SV_VIEW_INFO)
    {
        pPos->dwUpdateFields |= GNSS_UPDATE_SV_VIEW_INFO;
        pPos->dwValidFields |= GNSS_VALID_SATELLITES_IN_VIEW;
        memcpy(pPos->sv_in_view, m_gnssPosData.sv_in_view, sizeof(pPos->sv_in_view));        
    }

    pPos->uFixType = m_gnssPosData.uFixType;
    pPos->uFixQuality = m_gnssPosData.uFixQuality;
}

BOOL CGNSSParser::IsRealTimeData(DWORD dwMaximumAge)
{
    DWORD dwCurTime = GetTickCount();
    DWORD dwStamp = dwCurTime - m_dwLastUpdateTime;

    return (dwStamp <= dwMaximumAge);
}