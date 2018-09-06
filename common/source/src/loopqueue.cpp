/*******************************************************************************
*
* Filename:
* ---------
* Loopqueue.cpp
*
*
* Description:
* ------------
* This file implement the loop queue 
*
* Author:
* -------
* Hugo(mtk71195)
*
*------------------------------------------------------------------------------
* $Revision: #3 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/
#include <windows.h>
#include "stdafx.h"
#include "loopqueue.h"

CLoopQueue::CLoopQueue(void)
{
    m_pBuf = NULL;
}
CLoopQueue::CLoopQueue(DWORD dwSize, BOOL bOverwrite)
{
	m_pBuf = (char *)malloc(dwSize);
	
	m_dwSize = dwSize;
	m_dwReadPointer = m_dwWritePointer = 0;
	m_bOverflow = FALSE;
	m_bOverWrite = bOverwrite;
	m_bEmpty = TRUE;   
	
	InitializeCriticalSection(&m_cs);
}

CLoopQueue::~CLoopQueue()
{
	if (m_pBuf != NULL)
	{
		free(m_pBuf);
	}

	DeleteCriticalSection(&m_cs);
}

DWORD CLoopQueue::GetFreeSpace(void)
{
	DWORD dwFree;	

	if (m_bEmpty)
	{
		dwFree = m_dwSize;
	}
	else if (m_dwReadPointer < m_dwWritePointer)
	{
		dwFree = m_dwSize - m_dwWritePointer + m_dwReadPointer;
	}
	else
	{
		dwFree = m_dwReadPointer - m_dwWritePointer;
	}	

	return dwFree;
}


/****************export function to caller*************************/

void CLoopQueue::Reset(void)
{
	EnterCriticalSection(&m_cs);

	m_dwReadPointer = m_dwWritePointer = 0;
	m_bOverflow = FALSE;	
	m_bEmpty = TRUE;

	LeaveCriticalSection(&m_cs);
}

BOOL CLoopQueue::IsEmpty(void)
{
	BOOL bRet = 0;

	EnterCriticalSection(&m_cs);

	bRet = m_bEmpty;

	LeaveCriticalSection(&m_cs);

	return bRet;
}

BOOL CLoopQueue::IsOverflow(void)
{
	BOOL bRet;

	EnterCriticalSection(&m_cs);

	bRet = m_bOverflow;

	LeaveCriticalSection(&m_cs);

	return bRet;
}

void CLoopQueue::ClearOverflow(void)
{
	EnterCriticalSection(&m_cs);

	m_bOverflow = FALSE;

	LeaveCriticalSection(&m_cs);
}


DWORD CLoopQueue::GetDataLength(void)
{
	DWORD dwRet = 0;

	EnterCriticalSection(&m_cs);
	
	dwRet = m_dwSize - GetFreeSpace();

	LeaveCriticalSection(&m_cs);

	return dwRet;
}

BOOL CLoopQueue::IsFull(void)
{
	BOOL bRet;

	EnterCriticalSection(&m_cs);

	bRet = (0 == GetFreeSpace());

	LeaveCriticalSection(&m_cs);

	return bRet;
}

DWORD CLoopQueue::Read(char *pBuf, DWORD dwLen)
{
	DWORD dwTop = 0;
	DWORD dwRead = 0;	    

    if (NULL == pBuf || 0 == dwLen)
    {
        return 0;
    }

    if (m_bEmpty || NULL == m_pBuf)
    {
        return 0;
    }

	EnterCriticalSection(&m_cs);

	dwRead = min(dwLen, m_dwSize - GetFreeSpace());
	
	dwTop = min(dwRead, (m_dwSize - m_dwReadPointer));  

	memcpy(pBuf, (m_pBuf + m_dwReadPointer), dwTop);
	if (dwRead > dwTop)
	{
		memcpy((pBuf + dwTop), m_pBuf, (dwRead - dwTop));
	}

	// update read pointer
	m_dwReadPointer += dwRead;
	if (m_dwReadPointer >= m_dwSize)
	{
		m_dwReadPointer -= m_dwSize;
	}

	if (m_dwReadPointer == m_dwWritePointer)
	{
		m_bEmpty = TRUE;
	}

	LeaveCriticalSection(&m_cs);
	
	return dwRead;
}

DWORD CLoopQueue::Write(char *pBuf, DWORD dwLen)
{
	DWORD dwWrite, dwTop, dwFree, bOverflow;

	bOverflow = FALSE;

    if (NULL == pBuf || 0 == dwLen)
    {
        return 0;
    }

    if (NULL == m_pBuf)
    {
        return 0;
    }

	// if the write data is more than the buffer size
	// we only write the the latest data into the buffer
	// and discard useless data from the head of input buffer
	if (dwLen > m_dwSize)
	{
		pBuf += (dwLen - m_dwSize);
		dwWrite = m_dwSize;
	}
	else
	{
		dwWrite = dwLen;
	}

	EnterCriticalSection(&m_cs);

	dwFree = GetFreeSpace();
	if ((dwWrite > dwFree) && (m_bOverWrite))
	{
		bOverflow = m_bOverflow = TRUE;		
	}
	else if ((dwWrite > dwFree) && (!m_bOverWrite))
	{
		//discard some data
		pBuf += (dwWrite - dwFree);
		dwWrite = dwFree;
	}

	dwTop = min(dwWrite, (m_dwSize - m_dwWritePointer));
	memcpy((m_pBuf + m_dwWritePointer), pBuf, dwTop);
	if (dwWrite > dwTop)
	{
		memcpy(m_pBuf, pBuf + dwTop, (dwWrite - dwTop));
	}

	// update write pointer
	m_dwWritePointer += dwWrite;
	if (m_dwWritePointer >= m_dwSize)
	{
		m_dwWritePointer -= m_dwSize;
	}

	m_bEmpty = FALSE;

	if (bOverflow)
	{
		//update read pointer
		m_dwReadPointer = m_dwWritePointer;
	}

	LeaveCriticalSection(&m_cs);
	
	return dwWrite;	
}