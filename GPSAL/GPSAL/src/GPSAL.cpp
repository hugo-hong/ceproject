/************************************************************************
Filename:gpsal.cpp

Description:Defines the entry point for the DLL application.

Data:2013/04/03

Author: mtk71195
************************************************************************/

#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>
#include "ecommon.h"
#include "serial.h"
#include "gpsparser.h"
#include "GPSAL.h"

#define GPS_ENTRY_KEY           _T("drivers\\builtin\\gps")
#define GPS_DEFAULT_COM_INDEX   1

#define GPS_LISTEN_TIMEOUT      1000//ms

enum EVENT_TYPE
{
    EVENT_NEW_DATA,
    EVENT_DEVICE_CHANGE,
};


typedef struct _GPSAL_ENTRY_DATA 
{    
    CRITICAL_SECTION gps_cs;
    HANDLE hNewDataEvent;
    HANDLE hDeviceStateEvent;
    HANDLE hNMEADataThread;
    DWORD dwNMEADataTID;
    DWORD dwTimerId;
    CSerial gpsSerial;       
    CGPSParser gpsParser;
    GPS_DEVICE_DATA gpsDeviceData;
    GPS_POS_DATA gpsPosData;
}GPSAL_ENTRY_DATA, *PGPSAL_ENTRY_DATA;

PGPSAL_ENTRY_DATA g_pEntryData = NULL;

char buf[1024] = {0};

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  dwReasonCall, 
                       LPVOID lpReserved
					 )
{
    switch (dwReasonCall)
    {
    case DLL_PROCESS_ATTACH:
        RETAILMSG(LOG_ON, (_T("[GPSAL]GPSAL DLL Attach\r\n")));
        RETAILMSG(1, (_T("\r\n\r\n ========================================================================\r\n")));
        RETAILMSG(1, (_T("[GPSAL]GPSAL DLL BuildData %s - %s \r\n\r\n"), TEXT(__DATE__), TEXT(__TIME__)));
        RETAILMSG(1, (_T(" ========================================================================\r\n\r\n"))); 
        break;

    case DLL_PROCESS_DETACH:
        RETAILMSG(LOG_ON, (_T("[GPSAL]GPSAL DLL Deatach\r\n")));
        break;
    }

    return TRUE;
}

__inline void GPSLock(void)
{
    ::EnterCriticalSection(&g_pEntryData->gps_cs);
}

__inline void GPSUnLock(void)
{
    ::LeaveCriticalSection(&g_pEntryData->gps_cs);
}

void NotifyCallerEvent(EVENT_TYPE eType)
{
    ASSERT(g_pEntryData != NULL);

    switch (eType)
    {
    case EVENT_NEW_DATA:
        if (g_pEntryData->hNewDataEvent != NULL)
        {
            ::SetEvent(g_pEntryData->hNewDataEvent);
        }
        break;

    case EVENT_DEVICE_CHANGE:
        if (g_pEntryData->hDeviceStateEvent != NULL)
        {
            ::SetEvent(g_pEntryData->hDeviceStateEvent);
        }
        break;
    }
}

void CALLBACK GPSListenTimer(HWND hwnd, 
                             UINT uMsg, 
                             UINT idEvent, 
                             DWORD dwTime 
                             )
{
    ASSERT(idEvent == g_pEntryData->dwTimerId);

    GPSLock();

    if (g_pEntryData->dwTimerId != 0)
    {
        ::KillTimer(hwnd, idEvent);
        g_pEntryData->dwTimerId = 0;
    }

    //update GPS service state
    if (SERVICE_STATE_OFF == g_pEntryData->gpsDeviceData.dwServiceState)
    {
        g_pEntryData->gpsDeviceData.dwServiceState = SERVICE_STATE_ON;
    }
    else if (SERVICE_STATE_STARTING_UP == g_pEntryData->gpsDeviceData.dwServiceState)
    {
        g_pEntryData->gpsDeviceData.dwServiceState = SERVICE_STATE_OFF;
    }

    //update gps device state
    if (SERVICE_STATE_OFF == g_pEntryData->gpsDeviceData.dwDeviceState)
    {
        g_pEntryData->gpsDeviceData.dwDeviceState = SERVICE_STATE_ON;
    }
    else if (SERVICE_STATE_STARTING_UP == g_pEntryData->gpsDeviceData.dwDeviceState)
    {
        g_pEntryData->gpsDeviceData.dwDeviceState = SERVICE_STATE_OFF;
    }   

    NotifyCallerEvent(EVENT_DEVICE_CHANGE); 

    GPSUnLock();
}


void CALLBACK OnSerialRead(void *pCaller, PBYTE pData, DWORD dwLen)
{
    if (NULL == pCaller ||
        NULL == pData ||
        0 == dwLen)
    {
        return;
    }

    ASSERT(pCaller == g_pEntryData);

    PBYTE pBuf = NULL;    

    pBuf = new BYTE[dwLen];
    if (NULL == pBuf)
    {
        RETAILMSG(LOG_ON, (_T("[GPSAL]OnSerialRead:no memory!!!\r\n")));
        return;
    }
    else
    {
        memcpy(pBuf, pData, dwLen);
    }

    ::PostThreadMessage(g_pEntryData->dwNMEADataTID, GPS_DATA_MSG, (WPARAM)dwLen, (LPARAM)pBuf);

    GPSLock();

    //new data come
    if (g_pEntryData->dwTimerId != 0)
    {
        ::KillTimer(NULL, g_pEntryData->dwTimerId);
        g_pEntryData->dwTimerId = 0;
    }

    g_pEntryData->gpsDeviceData.dwServiceState = SERVICE_STATE_STARTING_UP;
    g_pEntryData->gpsDeviceData.dwDeviceState = SERVICE_STATE_STARTING_UP;

    GPSUnLock();
   
}

DWORD CALLBACK GPSDataInd(LPVOID lpPara)
{
    PGPSAL_ENTRY_DATA pEntryData = (PGPSAL_ENTRY_DATA)lpPara;

    if (NULL == pEntryData ||
        pEntryData != g_pEntryData)
    {
        return 0;
    }
    else
    {
        ::SetThreadPriority(pEntryData->hNMEADataThread, THREAD_PRIORITY_ABOVE_NORMAL);
    }    

    RETAILMSG(1, (_T("[GPSAL]GPSDataInd: run\r\n")));

    MSG msg = {0};
    DWORD dwResult = 0;
    int datalen = 0;
    char *pData = NULL;
    char buf[DEFAULT_BUF_LEN] = {0};

    while (::GetMessage(&msg, NULL, 0, 0))
    {
        if (GPS_DATA_MSG == msg.message)
        {
            pData = (char *)msg.lParam;
            datalen = (int)msg.wParam;

            if (NULL == pData)
            {
                continue;
            }

            GPSLock();

            memset(buf, 0, DEFAULT_BUF_LEN);            
            memmove((char *)buf, pData, datalen);	
            memset(&pEntryData->gpsPosData, 0, sizeof(GPS_POS_DATA));
            dwResult = pEntryData->gpsParser.ParseNMEA((char *)buf, datalen);  
            pEntryData->gpsParser.UpdateSatelliteInfo();
            NotifyCallerEvent(EVENT_NEW_DATA);        

            delete []pData;
            pData = NULL;

            //set timer to listen GPS device state
            if (g_pEntryData->dwTimerId != 0)
            {
                ::KillTimer(NULL, g_pEntryData->dwTimerId);
            }
            pEntryData->dwTimerId = ::SetTimer(NULL, 0, GPS_LISTEN_TIMEOUT, GPSListenTimer);

            GPSUnLock();
        }
    }

    RETAILMSG(1, (_T("[GPSAL]GPSDataInd: exit\r\n")));

    return 0;
}

DWORD OpenGPSPort(void)
{
    DWORD dwError = NO_ERROR;
    HKEY hKey = NULL;
    DWORD dwComIndex = GPS_DEFAULT_COM_INDEX;

    if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, GPS_ENTRY_KEY, 0, KEY_READ, &hKey)) 
    {
        DWORD dwEntryType;
        DWORD dwSize;

        dwSize = sizeof(dwComIndex);
        if ((ERROR_SUCCESS == RegQueryValueEx(hKey, _T("CommIndex"), NULL, &dwEntryType, (LPBYTE)&dwComIndex, &dwSize)))
        {
            //new GPS driver architecture
            RETAILMSG(1, (_T("[GPSAL]OpenGPSPort:new GPS driver architecture\r\n")));
        }
        else
        {
            //old GPS driver architecture
            RETAILMSG(1, (_T("[GPSAL]OpenGPSPort:old GPS driver architecture\r\n")));
            dwError = RegQueryValueEx(hKey, _T("Index"), NULL, &dwEntryType, (LPBYTE)&dwComIndex, &dwSize);
            if (ERROR_SUCCESS != dwError)
            {               
                goto EXIT;
            }
        }

        RegCloseKey(hKey);
        hKey = NULL;
    }

    //g_pEntryData->pSerial->RegisterPort(g_pEntryData, OnSerialRead);
    //if (!g_pEntryData->pSerial->OpenPort(dwComIndex))
    g_pEntryData->gpsSerial.RegisterPort(g_pEntryData, OnSerialRead);
    if (!g_pEntryData->gpsSerial.OpenPort(dwComIndex))    
    {
        dwError = ERROR_IO_INCOMPLETE;
        goto EXIT;
    }

EXIT:
    if (hKey != NULL)
    {
        RegCloseKey(hKey);
    }
    return dwError;
}

HANDLE GPS_OpenDevice(HANDLE hNewLocationData,
                    HANDLE   hDeviceStateChange,
                    const TCHAR *szDeviceName,
                    DWORD    dwFlags
                    )
{
    HANDLE hGPSDevice = NULL;
    DWORD dwError = NO_ERROR; 
    PGPSAL_ENTRY_DATA pGPSEntryData = NULL;

    if (g_pEntryData != NULL)
    {
        RETAILMSG(1, (_T("[GPSAL]GPS_OpenDevice:already opened!\r\n")));
        return (HANDLE)g_pEntryData;
    }

    pGPSEntryData = (PGPSAL_ENTRY_DATA)LocalAlloc(LPTR, sizeof(GPSAL_ENTRY_DATA));
    if (NULL == pGPSEntryData)
    {
        RETAILMSG(1, (_T("[GPSAL]GPS_OpenDevice:alloc memory fail!\r\n")));
        dwError = ERROR_NOT_ENOUGH_MEMORY;
        goto EXIT;
    }
    else
    {
        RETAILMSG(1, (_T("[GPSAL]GPS_OpenDevice:alloc memory size=%d\r\n"), LocalSize((HLOCAL)pGPSEntryData)));
        g_pEntryData = pGPSEntryData;        
    }

    pGPSEntryData->hNewDataEvent = hNewLocationData;
    pGPSEntryData->hDeviceStateEvent = hDeviceStateChange; 

    ::InitializeCriticalSection(&pGPSEntryData->gps_cs);
    
    GPSLock();

    pGPSEntryData->hNMEADataThread = ::CreateThread(NULL,
                                        0,
                                        GPSDataInd,
                                        pGPSEntryData,
                                        0,
                                        &pGPSEntryData->dwNMEADataTID);
    if (NULL == pGPSEntryData->hNMEADataThread)
    {
        dwError = GetLastError();
        RETAILMSG(1, (_T("[GPSAL]GPS_OpenDevice:create thread error=%d!\r\n"), dwError));
        goto EXIT;
    }

    //create gps parser       
    pGPSEntryData->gpsParser.CGPSParser::CGPSParser();
    if (!pGPSEntryData->gpsParser.Init())
    {
        RETAILMSG(1, (_T("[GPSAL]GPS_OpenDevice:new gps parser fail!\r\n")));
        dwError = ERROR_NOT_ENOUGH_MEMORY;
        goto EXIT;
    }

    //open gps serial port
    //open GPS hardware serial port    
    pGPSEntryData->gpsSerial.CSerial::CSerial();   
    dwError = OpenGPSPort();
    if (dwError != NO_ERROR)
    {            
        RETAILMSG(1, (_T("[GPSAL]GPS_OpenDevice:OpenGPSPort error=%d!\r\n"), dwError));
        goto EXIT;
    }

    pGPSEntryData->gpsDeviceData.dwServiceState = SERVICE_STATE_OFF;
    pGPSEntryData->gpsDeviceData.dwDeviceState = SERVICE_STATE_OFF;

    //create gps listen state timer
    pGPSEntryData->dwTimerId = ::SetTimer(NULL, 0, 100, GPSListenTimer);

    hGPSDevice = (HANDLE)pGPSEntryData;

EXIT:
    GPSUnLock();

    if (dwError != ERROR_SUCCESS)
    {
        GPS_CloseDevice((HANDLE)pGPSEntryData);
    }

    ::SetLastError(dwError);

    return hGPSDevice;
}

DWORD GPS_CloseDevice(HANDLE hGPSDevice)
{
    DWORD dwError = NO_ERROR; 
    PGPSAL_ENTRY_DATA pGPSEntryData = (PGPSAL_ENTRY_DATA)hGPSDevice;    

    if (NULL == pGPSEntryData ||
        g_pEntryData != pGPSEntryData)
    {
        dwError = ERROR_INVALID_PARAMETER;
        goto EXIT;
    }    

    GPSLock();

    //close serial port
    g_pEntryData->gpsSerial.ClosePort();
    g_pEntryData->gpsSerial.~CSerial();   

    //exit thread
    if (g_pEntryData->hNMEADataThread && 
        g_pEntryData->dwNMEADataTID != 0)
    {
        ::PostThreadMessage(g_pEntryData->dwNMEADataTID, WM_QUIT, 0, 0);
        ::CloseHandle(g_pEntryData->hNMEADataThread);
        g_pEntryData->hNMEADataThread = NULL;
        g_pEntryData->dwNMEADataTID = 0;
    }

    //delete gps parser  
    g_pEntryData->gpsParser.DeInit();
    g_pEntryData->gpsParser.~CGPSParser();

    GPSUnLock();

    ::DeleteCriticalSection(&g_pEntryData->gps_cs); 

    if (g_pEntryData)
    {
        LocalFree(g_pEntryData);
        g_pEntryData = NULL;
    }

EXIT:    
    ::SetLastError(dwError);
    g_pEntryData = NULL;
    return dwError;
}

DWORD GPS_GetPosition(HANDLE        hGPSDevice,
                     GPS_POS_DATA *pGPSPosition,
                     DWORD         dwMaximumAge,
                     DWORD         dwFlags
                     )
{
    DWORD dwError = NO_ERROR; 
    PGPSAL_ENTRY_DATA pGPSEntryData = (PGPSAL_ENTRY_DATA)hGPSDevice;

    GPSLock();

    if (NULL == pGPSEntryData ||
        g_pEntryData != pGPSEntryData)
    {
        dwError = ERROR_INVALID_PARAMETER;
        goto EXIT;
    }

    if (pGPSPosition->dwVersion != GPS_VERSION_1 ||
        pGPSPosition->dwSize != sizeof(GPS_POS_DATA))
    {
        dwError = ERROR_INVALID_PARAMETER;
        goto EXIT;
    }   

    g_pEntryData->gpsParser.FillGPSPosition(pGPSPosition, dwMaximumAge);

EXIT:
    GPSUnLock();

    ::SetLastError(dwError);
    
    return dwError;
}

DWORD GPS_GetDeviceState(GPS_DEVICE_DATA *pGPSDevice)
{
    DWORD dwError = NO_ERROR;

    GPSLock();

    if (NULL == pGPSDevice)
    {
        dwError = ERROR_INVALID_PARAMETER;
        goto EXIT;
    }

    if (pGPSDevice->dwVersion != GPS_VERSION_1 ||
        pGPSDevice->dwSize != sizeof(GPS_POS_DATA))
    {
        dwError = ERROR_INVALID_PARAMETER;
        goto EXIT;
    }   

    pGPSDevice->dwServiceState = g_pEntryData->gpsDeviceData.dwServiceState;
    pGPSDevice->dwDeviceState = g_pEntryData->gpsDeviceData.dwDeviceState;   

EXIT:
    GPSUnLock();

    ::SetLastError(dwError);

    return dwError;
}

