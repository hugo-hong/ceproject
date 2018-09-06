/************************************************************************
Filename:serial.cpp

Description:CSerial implement

Data:2010/05/03

Author: Hugo
************************************************************************/

#include "stdafx.h"
#include "ecommon.h"
#include "serial.h"


//default CSerial constructor
CSerial::CSerial()
{
	m_hComm = INVALID_HANDLE_VALUE;
	m_hReadThread = NULL;
	m_hReadExitEvent = NULL;
	m_hWriteExitEvent = NULL;	
}

//default CSerial destructor
CSerial::~CSerial()
{
	ClosePort();
}

void CSerial::RegisterPort(void *pCaller, PFNOnSerialRead pfnOnserialRead)
{
    m_pCaller = pCaller;
    m_pfnOnSerialRead = pfnOnserialRead;
}


BOOL CSerial::OpenPort(UINT portNo,	            /*port number*/
						 UINT baudRate,			/*baudrate*/
						 UINT parity,			/*parity check bits*/
						 UINT dataBits,			/*data bits*/
						 UINT stopBits			/*stop bits*/
						 )
{
	DCB dcb = {0};
    TCHAR szPort[32] = {0};	
    BOOL bRet = 0;	
    
	// check if already opened
    if (m_hComm != INVALID_HANDLE_VALUE)
	{
		return TRUE;
	}
	
	//set serial port name
#if 0 //this can only open COM 0-9
    _stprintf(szPort, _T("COM%d:"), portNo);
#else//this can open COM more than 10 
    _stprintf(szPort, _T("\\$device\\COM%d"), portNo);
#endif    

	//open port
	m_hComm = CreateFile(
		szPort,
		GENERIC_READ | GENERIC_WRITE,	//read & wirte
		0,								//mono mode£¨share mode£©
		NULL,
		OPEN_EXISTING,					//open style£¨create style)
		0,
		NULL 
		);
	
	if (INVALID_HANDLE_VALUE == m_hComm)
	{		
        RETAILMSG(1, (_T("[CSerial]OpenPort:CreateFile error=%d\r\n"), GetLastError()));
		return FALSE;		
	}
	
	// set port properity	
	if (!GetCommState(m_hComm,&dcb))
	{		
		goto EXIT;
	}
	
	dcb.BaudRate = baudRate;			// baudrate 
	dcb.fBinary = TRUE;					// set binary mode,must TRUE	
	dcb.ByteSize = dataBits;			// data bits,4-8 
	dcb.Parity = parity;				// parity check bits
	dcb.StopBits = stopBits;			// stop bits 	
	if (!SetCommState(m_hComm, &dcb))
	{
		//TRACE(_T("SetCommState error"));		
		goto EXIT;
	}	
	
    //set port read & wirte timeout
	COMMTIMEOUTS CommTimeOuts;
	GetCommTimeouts (m_hComm, &CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout = MAXDWORD;  
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;  
	CommTimeOuts.ReadTotalTimeoutConstant = 0;    
	CommTimeOuts.WriteTotalTimeoutMultiplier = 10;  
	CommTimeOuts.WriteTotalTimeoutConstant = 1000;  	
	if(!SetCommTimeouts( m_hComm, &CommTimeOuts ))
	{
		//TRACE( _T("SetCommTimeouts error") );
		goto EXIT;
	}	
	
	//set port monotor events
	SetCommMask (m_hComm, EV_RXCHAR);
	
	//set rx & tx buffer
	SetupComm(m_hComm, MAX_RX_BUF_LEN, MAX_TX_BUF_LEN);
	
	//clear rx & tx buffer
	PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
	
	//create serial event
	if (NULL == m_hReadExitEvent)
	{
		m_hReadExitEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	}

	if (NULL == m_hWriteExitEvent)
	{
		m_hWriteExitEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	}
	
	
	//create serial read thread
    if (NULL == m_hReadThread || INVALID_HANDLE_VALUE == m_hReadThread)
    {
        m_hReadThread = CreateThread(NULL, 0, ReadThreadFunc, this, 0, &m_dwReadThreadID);
    }	

	//create serial write thread
    if (NULL == m_hWriteThread || INVALID_HANDLE_VALUE == m_hWriteThread)
    {
        m_hWriteThread = CreateThread(NULL, 0, WriteThreadFunc, this, 0, &m_dwWriteThreadID);
    }	
	
    //TRACE(_T("Open port success"));
	bRet = TRUE;

EXIT:
    if (!bRet)
    {
        ClosePort();
    }

	return bRet;
}


void CSerial::ClosePort(void)
{	
	//serial port have not open
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return;
	}
	
	//exit read thread
    if (m_hReadExitEvent != NULL)
    {
        SetEvent(m_hReadExitEvent);
    }	
	SetCommMask(m_hComm, 0);	
    if (m_hReadThread != NULL)
    {
        if (WAIT_TIMEOUT == WaitForSingleObject(m_hReadThread, 1000))
        {
            TerminateThread(m_hReadThread, -1);
        }

        //release thread handle
        CloseHandle(m_hReadThread);
        m_hReadThread = NULL;
        CloseHandle(m_hReadExitEvent);
        m_hReadExitEvent = NULL;
    }	
		
	//exit write thread
    if (m_hWriteExitEvent != NULL)
    {
        SetEvent(m_hWriteExitEvent);
    }
	if (m_hWriteThread != NULL)
	{
        if (WAIT_TIMEOUT == WaitForSingleObject(m_hWriteThread, 1000))
        {
            TerminateThread(m_hWriteThread, -1);
        }

        //release write handle
        CloseHandle(m_hWriteThread);
        m_hWriteThread = NULL;
        CloseHandle(m_hWriteExitEvent);
        m_hWriteExitEvent = NULL;
	}		
	
	//close serial port
	if (!CloseHandle (m_hComm))
	{
		m_hComm = INVALID_HANDLE_VALUE;		
	}

    m_pCaller = NULL;
    m_pfnOnSerialRead = NULL;
    m_hComm = INVALID_HANDLE_VALUE;	
}



BOOL CSerial::WritePort(const char *buf, DWORD dwLen)
{
	char *pBuf = NULL;
	
	pBuf = new char[dwLen + 1];
	if (!pBuf)
	{
		return FALSE;
	}
    else
    {
        memset(pBuf, 0, dwLen + 1);
    }

	memmove(pBuf, buf, dwLen);

	//send date to write thread message que
	return PostThreadMessage(m_dwWriteThreadID,UART_TX_DATA_MSG,
		WPARAM(dwLen), LPARAM(pBuf));	
}


BOOL CSerial::SetTimeouts(COMMTIMEOUTS CommTimeOuts)
{
	if (INVALID_HANDLE_VALUE == m_hComm)
	{
		//TRACE(_T("SetTimeouts error: serial port have not open!\r\n"));
		return FALSE;
	}

	return SetCommTimeouts(m_hComm,&CommTimeOuts);
}



DWORD CSerial::ReadThreadFunc(LPVOID lpParam)
{
	CSerial *pSerial = (CSerial *)lpParam;
	
	DWORD dwEventMask;
	BYTE buf[MAX_RX_BUF_LEN] = {0};
	DWORD dwBytesRead;
	DWORD dwReadLen;
	
	DWORD dwErrorFlag;
	COMSTAT	comState;
	DWORD dwRet = 0;	

    if (NULL == pSerial)
    {
        return 0;
    }

    SetThreadPriority(pSerial->m_hReadThread, THREAD_PRIORITY_HIGHEST);
	
	//clear buffer
	PurgeComm(pSerial->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR );
	
    //set interest event
	SetCommMask (pSerial->m_hComm, EV_RXCHAR | EV_CTS | EV_DSR );
	while (TRUE)
	{   	
		if (WaitCommEvent(pSerial->m_hComm,&dwEventMask,0))
		{			
			SetCommMask (pSerial->m_hComm, EV_RXCHAR | EV_CTS | EV_DSR);
			
			if (dwEventMask & EV_RXCHAR) 
			{
				ClearCommError(pSerial->m_hComm, &dwErrorFlag, &comState);
				if (comState.cbInQue <= 0)
				{
					continue;
				}

				//read data
				dwReadLen = MAX_RX_BUF_LEN;//min(comState.cbInQue, MAX_RX_BUF_LEN);
				memset(buf, 0, sizeof(buf));				
				if (ReadFile(pSerial->m_hComm, buf, dwReadLen, &dwBytesRead, NULL) &&
					dwBytesRead > 0)
				{
					//dispath data
                    if (pSerial->m_pfnOnSerialRead != NULL)
                    {
                        pSerial->m_pfnOnSerialRead(pSerial->m_pCaller, buf, dwBytesRead);
                    }					
				}
				
			}		
			
		}

		if (WAIT_OBJECT_0 == WaitForSingleObject(pSerial->m_hReadExitEvent, 100))
		{
			CloseHandle(pSerial->m_hReadExitEvent);
			break;
		}	
	}
	
	return 0;
}


DWORD CSerial::WriteThreadFunc(LPVOID lpParam)
{
	CSerial *pSerial = (CSerial *)lpParam;
	MSG msg;
	DWORD dwWriteLen;
	char *pBuf = NULL;

	while (TRUE)
	{
		//peek thread message
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.hwnd != 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;
			}

			if (UART_TX_DATA_MSG == msg.message)
			{
				pBuf = (char *)msg.lParam;
				dwWriteLen = msg.wParam;

				//write data
				DWORD dwWritten = 0;
				DWORD dwBytes = 0;
				DWORD dwLen = dwWriteLen;
				do 
				{
					WriteFile(pSerial->m_hComm, pBuf + dwWritten, dwLen, &dwBytes, NULL);
					dwWritten += dwBytes;
					dwLen -= dwBytes;
				} while (dwWriteLen != dwWritten);

				//release buffer
				delete []pBuf;
				
			}
		}

		if (WAIT_OBJECT_0 == WaitForSingleObject(pSerial->m_hWriteExitEvent, 100))
		{
			CloseHandle(pSerial->m_hWriteExitEvent);
			break;
		}	
	}

	return 0;
}
