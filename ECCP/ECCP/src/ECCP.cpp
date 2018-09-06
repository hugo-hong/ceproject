// Commprotocol.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>
#include "ECCP.h"
#include "eutil.h"
#include "ecommon.h"

CCP_CONTEXT g_ccpContext;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CALLBACK OnRxDataInd(void *pCaller, PBYTE pData, DWORD dwLen)
{
    CarCommProtocol *pThis = (CarCommProtocol *)pCaller;    

    assert(pThis != NULL && pData != NULL);        

    char *pBuf = new char[dwLen];

    if (NULL == pBuf)
    {
        RETAILMSG(1, (_T("[ECCP]OnRxDataInd:no memory!!!\r\n")));
        return;
    }
    else
    {
        memset(pBuf, 0, DEFAULT_BUF_LEN);
    }    

    memcpy(pBuf, pData, dwLen);
    
    //Notify data incoming
    ::PostThreadMessage(pThis->m_dwReceiveThreadId, UART_RX_DATA_MSG, (WPARAM)dwLen, (LPARAM)pBuf);
   
}

DWORD CALLBACK CCP_ReceiveDataInt(LPVOID lpPara)
{
    CarCommProtocol *pThis = (CarCommProtocol *)lpPara;

    assert(pThis != NULL); 

    ::SetThreadPriority(pThis->m_hReceiveThread, THREAD_PRIORITY_ABOVE_NORMAL);

    MSG msg = {0};
    DWORD dwResult = 0;
    int datalen = 0;
    char *pData = NULL;
    char buf[DEFAULT_BUF_LEN] = {0};

    while (::GetMessage(&msg, NULL, 0, 0))
    {
         if (UART_RX_DATA_MSG == msg.message)
         {
             LPVOID pLock = ENTER_LOCK(NULL);

             pData = (char *)msg.lParam;
             datalen = msg.wParam;

             if (NULL == pData)
             {
                 continue;
             }

             memset(buf, 0, DEFAULT_BUF_LEN);            
             memmove((char *)buf, pData, datalen);

             g_ccpContext.m_ccp.ParserData(buf, datalen);

             delete []pData;
             pData = NULL;

             EXIT_LOCK(pLock);
         }
    }

    return 0;
}

DWORD CALLBACK CCP_ProcessCmdInt(LPVOID lpPara)
{
    CarCommProtocol *pThis = (CarCommProtocol *)lpPara;

    assert(pThis != NULL); 
    
    DWORD dwResut = 0;
    PCCP_ENTRY_DATA_T pEntryData = (PCCP_ENTRY_DATA_T)g_ccpContext.m_pEntryData;     
    HANDLE hRxEvent = pEntryData->m_ahRxEvent[g_ccpContext.m_callerList.nCallerId];
    HANDLE hTxEvent = pEntryData->m_ahTxEvent[g_ccpContext.m_callerList.nCallerId];
    HANDLE ahEvent[] = {hRxEvent, hTxEvent, pThis->m_hExitProcessEvent};
    while (TRUE)
    {
        dwResut = ::WaitForMultipleObjects(DIM(ahEvent), ahEvent, FALSE, INFINITE);

        LPVOID pLock = ENTER_LOCK(CCP_LOCK_NAME);

        switch (dwResut)
        {
        case WAIT_OBJECT_0:
            {
                char cmd[DEFAULT_BUF_LEN] = {0};
                PFNOnReceiveCmdInd pfnReceiveCmdInd = g_ccpContext.m_callerList.pfnReceiveCmdInd;

                memcpy(cmd, pEntryData->m_rxData, pEntryData->m_dwRxDataLen);
                if (NULL != pfnReceiveCmdInd)
                {
                    pfnReceiveCmdInd(g_ccpContext.m_callerList.pCaller, (PBYTE)cmd, pEntryData->m_dwRxDataLen);
                }
            }
            break;

        case WAIT_OBJECT_0 + 1:
            {
                char cmd[DEFAULT_BUF_LEN] = {0};
                int cmdLen = 0;

                assert(MASTER == pThis->m_eRole);
                pThis->MakeCommand(pEntryData->m_txData, pEntryData->m_dwTxDataLen, cmd, cmdLen);
                pThis->m_serial.WritePort(cmd, cmdLen);
            }
            break;

        case WAIT_OBJECT_0 + 2:            
            goto EXIT;
            break;
        }        

        EXIT_LOCK(pLock);
    }

EXIT:
    return 0;
}

CarCommProtocol::CarCommProtocol()        
{         
    m_hReceiveThread = NULL;
    m_dwReceiveThreadId = 0;

    m_hProcessThread = NULL;
    m_hExitProcessEvent = NULL;
}

CarCommProtocol::~CarCommProtocol()
{    
}

void CarCommProtocol::PreInit(void)
{
    m_eRole = SLAVER;
    m_serial.CSerial::CSerial();
    m_loopQueue.CLoopQueue::CLoopQueue(DEFAULT_BUF_LEN);
}

BOOL CarCommProtocol::Init(void)
{  
    if (!m_serial.OpenPort(g_ccpContext.m_commDcb.portNo,
                          g_ccpContext.m_commDcb.baud,
                          g_ccpContext.m_commDcb.parity,
                          g_ccpContext.m_commDcb.databits,
                          g_ccpContext.m_commDcb.stopbits))
    {
        return FALSE;
    }
    else
    {
        m_serial.RegisterPort(this, OnRxDataInd);
        m_hReceiveThread = ::CreateThread(NULL, 0, CCP_ReceiveDataInt, this, 0, &m_dwReceiveThreadId);
    }

    m_eRole = MASTER;

    return TRUE;
}

BOOL CarCommProtocol::PostInit(void)
{
    PCCP_ENTRY_DATA_T pEntryData = (PCCP_ENTRY_DATA_T)g_ccpContext.m_pEntryData;

    assert(pEntryData != NULL);

    TCHAR szEvent[16] = {0};
    int nCallerId = g_ccpContext.m_callerList.nCallerId;

    _stprintf(szEvent, _T("Rx%d_Event"), nCallerId);
    pEntryData->m_ahRxEvent[nCallerId] = ::CreateEvent(NULL, FALSE, FALSE, szEvent);

    _stprintf(szEvent, _T("Tx%d_Event"), nCallerId);
    pEntryData->m_ahTxEvent[nCallerId] = ::CreateEvent(NULL, FALSE, FALSE, szEvent);
    
    if (NULL == pEntryData->m_ahRxEvent[nCallerId] ||
        NULL == pEntryData->m_ahTxEvent[nCallerId])
    {
        RETAILMSG(1, (_T("[ECCP]PostInit Error:create caller event err=%d \r\n"), GetLastError()));
        return FALSE;
    }

    m_hExitProcessEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hProcessThread = ::CreateThread(NULL, 0, CCP_ProcessCmdInt, this, 0, NULL);
    if (NULL == m_hExitProcessEvent || NULL == m_hProcessThread)
    {
        RETAILMSG(1, (_T("[ECCP] PostInit Error:create process thread err=%d \r\n"), GetLastError()));
        return FALSE;
    }

    return TRUE;    
}

void CarCommProtocol::DeInit(void)
{
    if (m_hExitProcessEvent != NULL)
    {
        ::SetEvent(m_hExitProcessEvent);
        ::WaitForSingleObject(m_hProcessThread, 500);
        ::CloseHandle(m_hProcessThread);
        ::CloseHandle(m_hExitProcessEvent);
    }

    if (m_hReceiveThread != NULL)
    {
        ::PostThreadMessage(m_dwReceiveThreadId, WM_QUIT, 0, 0);
        ::WaitForSingleObject(m_hReceiveThread, 500);
        ::CloseHandle(m_hReceiveThread);
    }

    DELETE_LOCK();
}

void CarCommProtocol::AddRef(void)
{
    PCCP_ENTRY_DATA_T pEntryData = (PCCP_ENTRY_DATA_T)g_ccpContext.m_pEntryData;   

    if (CCP_DATA_ZONE_TAG == pEntryData->m_dwTag)
    {
        InterlockedIncrement((long *)&pEntryData->m_dwRef);
    }    
}

void CarCommProtocol::DelRef(void)
{
    PCCP_ENTRY_DATA_T pEntryData = (PCCP_ENTRY_DATA_T)g_ccpContext.m_pEntryData;   

    if (CCP_DATA_ZONE_TAG == pEntryData->m_dwTag)
    {
        InterlockedDecrement((long *)&pEntryData->m_dwRef);
    }    
}

BYTE CarCommProtocol::CalcChecksum(char *pCmd, int length)
{
    if (NULL == pCmd || 0 == length)
    {
        RETAILMSG(1, (_T("[ECCP] CalcChecksum Error:invalid param\r\n")));
        return FALSE;
    }

    int i = 0;
    int dataLen = pCmd[2];
    unsigned char ucSum = 0;

    for (i = 0; i < dataLen; i++)
    {
        ucSum ^= pCmd[3 + i];
    }

    return ucSum;
}

BOOL CarCommProtocol::DoChecksum(char *pCmd, int length)
{
    if (NULL == pCmd || 0 == length)
    {
        RETAILMSG(1, (_T("[ECCP] DoChecksum Error:invalid param\r\n")));
        return FALSE;
    }

    return (CalcChecksum(pCmd, length) == pCmd[length-1-2]);
}

DWORD CarCommProtocol::ParserData(char *pBuf, int length)
{
    UINT realDataLen = 0, dataHeader = 0;
    char tempBuf[DEFAULT_BUF_LEN] = {0};
    char realData[DEFAULT_BUF_LEN] = {0};
    DWORD dwLastDataLen = 0;
    BOOL bFindFirstFlag = 0;
    DWORD dwResult = 0;

    if (NULL == pBuf || 0 == length)
    {
        RETAILMSG(1, (_T("[CarCommProtocol]ProcessData:invalid parameters!!!\r\n")));
        return 0;
    }

    //use the temp buffer
    dwLastDataLen = m_loopQueue.GetDataLength();    
    dwLastDataLen = m_loopQueue.Read(tempBuf, dwLastDataLen);    
    if ((DEFAULT_BUF_LEN - dwLastDataLen) < length)
    {      
        DWORD dwDataLen = DEFAULT_BUF_LEN - dwLastDataLen;
        memmove(tempBuf + dwLastDataLen, pBuf, dwDataLen);
        m_loopQueue.Reset();
        m_loopQueue.Write(pBuf + dwDataLen, length - dwDataLen);
        length = DEFAULT_BUF_LEN;
    }
    else
    {
        memmove(tempBuf + dwLastDataLen, pBuf, length);
        length += dwLastDataLen;
        m_loopQueue.Reset();        
    }

    //parse data
    for (int i = 0; i < length; )
    {
        //begain with '0xA5 0x5A', and end with"0x5A 0xA5" as an command          
        if (tempBuf[i] == CMD_HEAD_TAG0 && tempBuf[i + 1] == CMD_HEAD_TAG1)
        {
            bFindFirstFlag = TRUE;
            dataHeader = i;
        }
        else if (tempBuf[i] == CMD_TAIL_TAG0 && tempBuf[i + 1] == CMD_TAIL_TAG1)
        {
            if (bFindFirstFlag)
            {
                i += 2;
                memset(realData, 0, sizeof(realData));
                memmove(realData, &tempBuf[dataHeader], i - dataHeader);                
                dwResult = DispatchData(realData, i - dataHeader);

                bFindFirstFlag = FALSE;

                continue;
            }
            else
            {
                //this sentence incompetele, discard data
                i += 2;

                continue;
            }
        }

        i++;
    }

    if (bFindFirstFlag)
    {
        //save the remain data to backup buffer
        m_loopQueue.Write(&tempBuf[dataHeader], length - dataHeader);
    }

    return dwResult;
}

DWORD CarCommProtocol::DispatchData(char *pBuf, int length)
{
    DWORD dwResult = ERROR_SUCCESS;

    if (NULL == pBuf || 0 == length)
    {
        return ERROR_INVALID_PARAMETER;
    }

    if (!DoChecksum(pBuf, length))
    {
        RETAILMSG(1, (_T("[CarCommProtocol]ParserData:invalid command!!!\r\n")));
    }    

    DWORD dwCmdLen = pBuf[2];
    DWORD dwCmdId = pBuf[3];
    PCCP_ENTRY_DATA_T pEntryData = (PCCP_ENTRY_DATA_T)g_ccpContext.m_pEntryData; 
    char *pDest = pEntryData->m_rxData;
    LPVOID pLock = ENTER_LOCK(CCP_LOCK_NAME);

    memset(pDest, 0, DEFAULT_BUF_LEN);
    pEntryData->m_dwRxDataLen = dwCmdLen - 1;
    memcpy(pDest, &pBuf[4], pEntryData->m_dwRxDataLen);    
    switch (dwCmdId)
    {
    case CMD_RADIO:
        if (NULL != pEntryData->m_ahRxEvent[RADIO])
        {
            ::SetEvent(pEntryData->m_ahRxEvent[RADIO]);
        }
        else
        {
            goto DEFAULT_CMD;
        }        
        break;

    case CMD_BACKCAR:
        if (NULL != pEntryData->m_ahRxEvent[BACKCAR])
        {
            ::SetEvent(pEntryData->m_ahRxEvent[BACKCAR]);
        }
        else
        {
            goto DEFAULT_CMD;
        }        
        break;

    case CMD_DISK:
        if (NULL != pEntryData->m_ahRxEvent[EPLAYER])
        {
            ::SetEvent(pEntryData->m_ahRxEvent[EPLAYER]);
        }
        else
        {
            goto DEFAULT_CMD;
        }        
        break;

    case CMD_IPOD:
        if (NULL != pEntryData->m_ahRxEvent[IPOD])
        {
            ::SetEvent(pEntryData->m_ahRxEvent[IPOD]);
        }
        else
        {
            goto DEFAULT_CMD;
        }        
        break;

    case CMD_TV:
        if (NULL != pEntryData->m_ahRxEvent[TV])
        {
            ::SetEvent(pEntryData->m_ahRxEvent[TV]);
        }
        else
        {
            goto DEFAULT_CMD;
        }        
        break;

DEFAULT_CMD:
    default:
        //home command
        assert(pEntryData->m_ahRxEvent[HOME] != NULL);
        ::SetEvent(pEntryData->m_ahRxEvent[HOME]);
        break;
    }

    EXIT_LOCK(pLock);

    return dwResult;
}

void CarCommProtocol::MakeCommand(char *pData, int length, char *pCmd, int& nCmdLen)
{
    if (NULL == pData || 0 == length || NULL == pCmd)
    {
        RETAILMSG(1, (_T("[CarCommProtocol]MakeCommand:invalid command!!!\r\n")));
        return;
    }

    int i = 0;
   
    //cmd head
    pCmd[i++] = CMD_HEAD_TAG0;
    pCmd[i++] = CMD_HEAD_TAG1;

    //cmd length
    pCmd[i++] = length + 1;

    //cmd ID
    pCmd[i++] = g_ccpContext.m_callerList.nCmdId;

    //cmd data
    memcpy(&pCmd[i], pData, length);

    //cmd checksum
    i += length;     
    pCmd[i++] = CalcChecksum(pCmd, pCmd[2]);   

    //cmd tail
    pCmd[i++] = CMD_TAIL_TAG0;
    pCmd[i++] = CMD_TAIL_TAG1;

    //whole cmd length
    nCmdLen = i;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCP_PreInit(void)
{
    RETAILMSG(1, (_T("\r\n\r\n ========================================================================\r\n")));
    RETAILMSG(1, (_T("[ECCP]ECCP DLL BuildData %s - %s \r\n\r\n"), TEXT(__DATE__), TEXT(__TIME__)));
    RETAILMSG(1, (_T(" ========================================================================\r\n\r\n"))); 

    memset(&g_ccpContext, 0, sizeof(CCP_CONTEXT));

    g_ccpContext.m_hFileMap = ::CreateFileMapping(INVALID_HANDLE_VALUE, 
        NULL, 
        PAGE_READWRITE, 
        0,
        sizeof(CCP_ENTRY_DATA_T),
        CCP_FILE_MAP);
    if (NULL == g_ccpContext.m_hFileMap)
    {
        RETAILMSG(1, (_T("[ECCP] Error:create file map err=%d\r\n"), GetLastError()));
        return;
    }

    g_ccpContext.m_pEntryData = ::MapViewOfFile(g_ccpContext.m_hFileMap,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        0);
    if (NULL == g_ccpContext.m_pEntryData)
    {
        RETAILMSG(1, (_T("[ECCP] Error:map file pointer err=%d\r\n"), GetLastError()));
        return;
    }

    g_ccpContext.m_ccp.PreInit();
}

BOOL CCP_Entry(UINT portNo,	            /*port number*/
               UINT baudRate,			/*baudrate*/
               UINT parity,			    /*parity check bits*/
               UINT dataBits,			/*data bits*/
               UINT stopBits			/*stop bits*/
               )
{
    if (NULL == g_ccpContext.m_hFileMap ||
        NULL == g_ccpContext.m_pEntryData)
    {
        goto EXCEPTION_EXIT;
    }

    //init paramer
    g_ccpContext.m_commDcb.portNo = portNo;
    g_ccpContext.m_commDcb.baud = baudRate;
    g_ccpContext.m_commDcb.parity = parity;
    g_ccpContext.m_commDcb.databits = dataBits;
    g_ccpContext.m_commDcb.stopbits = stopBits;    

    PCCP_ENTRY_DATA_T pEntryData = (PCCP_ENTRY_DATA_T)g_ccpContext.m_pEntryData;
    LPVOID pLock = ENTER_LOCK(CCP_LOCK_NAME);

    if (CCP_DATA_ZONE_TAG != pEntryData->m_dwTag)
    {
        //This is the MASTER, need do init
        pEntryData->m_dwTag = CCP_DATA_ZONE_TAG;
        g_ccpContext.m_ccp.Init();       
    }
  
    if (!g_ccpContext.m_ccp.PostInit())
    {
        goto EXCEPTION_EXIT;
    }

     g_ccpContext.m_ccp.AddRef();

    EXIT_LOCK(pLock);

    return TRUE;

EXCEPTION_EXIT:
    CCP_Exit();
    return FALSE;
}

void CCP_Exit(void)
{
    LPVOID pLock = ENTER_LOCK(CCP_LOCK_NAME);
    PCCP_ENTRY_DATA_T pEntryData = (PCCP_ENTRY_DATA_T)g_ccpContext.m_pEntryData;
    int nCallerId = g_ccpContext.m_callerList.nCallerId;

    if (pEntryData->m_ahRxEvent[nCallerId])
    {
        ::CloseHandle(pEntryData->m_ahRxEvent[nCallerId]);
    }

    if (pEntryData->m_ahTxEvent[nCallerId])
    {
        ::CloseHandle(pEntryData->m_ahTxEvent[nCallerId]);
    }

    g_ccpContext.m_ccp.DeInit();

    g_ccpContext.m_ccp.DelRef();

    UnmapViewOfFile(pEntryData);
    pEntryData = NULL;
    if (g_ccpContext.m_hFileMap)
    {
        ::CloseHandle(g_ccpContext.m_hFileMap);
    }
   
    EXIT_LOCK(pLock);        
}

BOOL CCP_RegisterCaller(int nCallerId, LPVOID pCaller, PFNOnReceiveCmdInd pfnReceiveCmdInd)
{
    if (!Is_Valid_ID(nCallerId) ||
        NULL == pCaller ||
        NULL == pfnReceiveCmdInd)
    {
        RETAILMSG(1, (_T("[ECCP]CCP_RegisterCaller Error:param error!\r\n")));
        return FALSE;
    }

    g_ccpContext.m_callerList.nCallerId = nCallerId;
    g_ccpContext.m_callerList.nCmdId = CMD_UNKNOWN;
    g_ccpContext.m_callerList.pCaller = pCaller;
    g_ccpContext.m_callerList.pfnReceiveCmdInd = pfnReceiveCmdInd;
    g_ccpContext.m_callerList.pNext = NULL; 

    return TRUE;
}

BOOL CCP_SendCommand(int nCmdId, UINT32 u4Param1, UINT32 u4Param2)
{
    DWORD dwLen = u4Param1;
    PBYTE pCmd = (PBYTE)u4Param2;

    if ( 0 == dwLen || NULL == pCmd)
    {
        RETAILMSG(1, (_T("[ECCP]CCP_SendCommand Error:param error!\r\n")));
        return FALSE;
    }

    PCCP_ENTRY_DATA_T pEntryData = (PCCP_ENTRY_DATA_T)g_ccpContext.m_pEntryData;
    LPVOID pLock = ENTER_LOCK(CCP_LOCK_NAME);
    char *pDest = (char *)pEntryData->m_txData;
    int nCallerId = g_ccpContext.m_callerList.nCallerId;

    g_ccpContext.m_callerList.nCmdId = nCmdId;
    pEntryData->m_dwTxDataLen = dwLen;    
    memcpy(pDest, pCmd, dwLen);
    ::SetEvent(pEntryData->m_ahTxEvent[MASTER]);

    EXIT_LOCK(pLock);

    return TRUE;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved
                      )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        RETAILMSG(LOG_ON, (_T("[ECCP]ECCP DLL Attach\r\n")));
        CCP_PreInit();
        break;

    case DLL_PROCESS_DETACH:
        RETAILMSG(LOG_ON, (_T("[ECCP]ECCP DLL Deatach\r\n")));
        CCP_Exit();
        break;
    }
    return TRUE;
}