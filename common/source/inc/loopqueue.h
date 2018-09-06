/*******************************************************************************
*
* Filename:
* ---------
* loopqueue.h
*
* Description:
* ------------
* This file define loop queue class
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
#ifndef _LOOPQUEUE_H_
#define _LOOPQUEUE_H_

class CLoopQueue
{
public:
    CLoopQueue(void);
	CLoopQueue(DWORD dwSize, BOOL bOverwrite = TRUE);
	~CLoopQueue();

	DWORD Read(char *pBuf, DWORD dwLen);
	DWORD Write(char *pBuf, DWORD dwLen);
	void Reset(void);
	BOOL IsEmpty(void);
	BOOL IsFull(void);
	BOOL IsOverflow(void);
	void ClearOverflow(void);
    DWORD GetDataLength(void);

protected:
	DWORD GetFreeSpace(void);

private:
	DWORD	m_dwReadPointer;	// read pointer start from 0
	DWORD	m_dwWritePointer;	// write pointer start from 0
	DWORD	m_dwSize;			// size of circular buffer
	BOOL	m_bEmpty;			// tag of whether the buffer is empty
	BOOL	m_bOverflow;		// tag of whether the buffer is overflow
	BOOL	m_bOverWrite;		// tag of whether overwrite on overflow is allowed
	CRITICAL_SECTION m_cs;		// critical section to protect data access
	char *m_pBuf;			// data storage
};



#endif //_LOOPQUEUE_H_