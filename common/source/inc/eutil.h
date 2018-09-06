/***********************************************************************************************
Name: eutil.h

Author: Hugo

Descriotion:decrelation common function

************************************************************************************************/
#ifndef _EUTIL_H_
#define _EUTIL_H_
#include <windows.h>
#include <string>

using namespace std;

#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif

/////////////////////////////////////////////////////////////////////
//macro define
#define SECTOR_SIZE			    (512)
#define KB_SIZE					(1024)
#define MB_SIZE					(KB_SIZE * 1024)
#define GB_SIZE					(MB_SIZE * 1024)
#define TB_SIZE					(GB_SIZE * 1024)

#define TIME_SCALE              (60)

#define  DIM(a)                 (sizeof(a) / sizeof(a[0]))

#define GET_BYTE(data, val, off) \
	(val = *(((PBYTE)data) + (off)))

#define GET_BYTS(data, val, len, off) \
	memcpy((PBYTE)(val), (PBYTE)(data) + (off), (len))

#define GET_WORD(data, val, off) \
	(val = *(((PBYTE)data) + (off)) | \
	*(((PBYTE)data) + (off) + 1) << 8)

#define GET_DWORD(data, val, off) \
	(val = *(((PBYTE)data) + (off)) | \
	*(((PBYTE)data) + (off) + 1) << 8 | \
	*(((PBYTE)data) + (off) + 2) << 16 | \
	*(((PBYTE)data) + (off) + 3) << 24 )

#define GET_DWORD64(data, val, off) \
    (val = *(((PBYTE)data) + (off)) | \
    *(((PBYTE)data) + (off) + 1) << 8 | \
    *(((PBYTE)data) + (off) + 2) << 16 | \
    *(((PBYTE)data) + (off) + 3) << 24 | \
    *(((PBYTE)data) + (off) + 4) << 32 | \
    *(((PBYTE)data) + (off) + 5) << 40 | \
    *(((PBYTE)data) + (off) + 6) << 48 | \
    *(((PBYTE)data) + (off) + 7) << 56 )
   
struct  Time_Section
{
    unsigned char u1Time;
    unsigned char cUnit;
};

typedef union
{
    Time_Section time_sec;
    unsigned short u2Time;
}TIME_DATA;



//define lock
static CRITICAL_SECTION *g_pCS = NULL;
class CELock
{
public:
    CELock(void)
    {
        m_hMutex = NULL;
        if (NULL == g_pCS)
        {
            g_pCS = new CRITICAL_SECTION;
            ASSERT(g_pCS != NULL);
            ::InitializeCriticalSection(g_pCS);
        }           
    }

    CELock(LPCTSTR lpLockName)
    {        
        m_hMutex = NULL;
        m_hMutex = ::CreateMutex(NULL, FALSE, lpLockName);
        ASSERT(m_hMutex != NULL);
    }

    ~CELock()
    {
        UnLock();
        if (m_hMutex != NULL)
        {
            ::CloseHandle(m_hMutex);
            m_hMutex = NULL;
        }       
    }


protected:
    void Lock(void)
    {
        if (m_hMutex != NULL)
        {
            ::WaitForSingleObject(m_hMutex, INFINITE);
        }
        else if (g_pCS != NULL)
        {
            ::EnterCriticalSection(g_pCS);
        }
        
    }

    void UnLock(void)
    {
        if (m_hMutex != NULL)
        {
            ::ReleaseMutex(m_hMutex);
        }
        else if (g_pCS != NULL)
        {
            ::LeaveCriticalSection(g_pCS);
        }        
    }

public:
    static LPVOID EnterLock(LPCTSTR szLock)
    {
        if (NULL == szLock)
        {
            CELock lock;

            lock.Lock();
            
            return (LPVOID)&lock;

        }
        else
        {
            CELock lock(szLock);

            lock.Lock();

            return (LPVOID)&lock;
        }              
    }

    static void ExitLock(LPVOID pLock)
    {
        ASSERT(pLock != NULL);

        ((CELock *)pLock)->UnLock();
    }

    static void DeleteLock(void)
    {
        if (g_pCS != NULL)
        {
            ::DeleteCriticalSection(g_pCS);
            delete g_pCS;
            g_pCS = NULL;
        }
    }

private:
    HANDLE m_hMutex;    
};

#define ENTER_LOCK             CELock::EnterLock

#define EXIT_LOCK              CELock::ExitLock

#define DELETE_LOCK            CELock::DeleteLock



/////////////////////////////////////////////////////////////////////////
//function decrelation

/***************************************************************
	Name:Size2String
	Para:llBytes,the bytes size 
			 pszSize, the buffer pointer to the size unit 
			 dwLen, the max chars in size unit
	Return: TRUE, success; FALSE, fail
	Func:covert the bytes size to proper size unit	
****************************************************************/
BOOL Size2String(LONGLONG llBytes, LPTSTR pszSize, DWORD dwLen);

/***************************************************************
    Name:Tick2String
    Para:u8Tick,the time tick, microsecond 
        pszTime, the buffer pointer to the time 
        dwLen, the max chars in the buffer
    Return: TRUE, success; FALSE, fail
    Func:covert the time microsecond to time xx : xx : xx	
****************************************************************/
BOOL Tick2String(UINT64 u8Tick, LPTSTR pszTime, DWORD dwLen);

/***************************************************************
	Name:MByteToWChar
	Para:pMStr, the ANSI string to be convert
	Return: return the UNICODE pointer
	Func:covert the ANSI to UNICODE
	Note:when used the UNICODE string,must delete it
****************************************************************/
LPWSTR MByteToWChar(LPCSTR pMStr);

/***************************************************************
	Name:WCharToMByte
	Para:pWStr, the UNICODE string to be convert
	Return: return the ANSI string pointer
	Func:covert the UNICODE to ANSI
	Note:when used the ANSI string,must delete it
****************************************************************/
LPSTR WCharToMByte(LPCWSTR pWStr);

/***************************************************************
	Name:DeleteBuffer
	Para:pBuffer, the buffer pointer to be release
	Return: no return
	Func:delete the buffer by new
	Note:when used the buffer you new,must delete it
****************************************************************/
void DeleteBuffer(LPVOID pBuffer);

/***************************************************************
	Name:Write2File(C/C++ Lib)
	Para:pData, the buffer pointer to the data
			 dwLen, the buffer length in bytes
			 szFile, the file name to write
	Return: return the bytes to wirte to file
	Func:write the data to a sepeical file	
****************************************************************/
UINT Write2File(LPBYTE pData, DWORD dwLen, LPCSTR szFile);


/***************************************************************
    Name:Write2File (WINAPI)
    Para: hFile, the file handle
    pData, the buffer pointer to the data
    dwLen, the buffer length in bytes
    szFile, the file name to write
    Return: return the bytes to wirte to file
    Func:write the data to a sepeical file	
****************************************************************/
HANDLE Write2File(HANDLE hFile, LPBYTE pData, DWORD dwLen, LPCTSTR szFile);

/***************************************************************
    Name:GetCurrentDirectory 
    Para: szPath, the buffer to save the file directory
    dwLen:buffer length, in chars
    Return: TRUE:success, FALSE:failed
    Func:get current file directory
****************************************************************/
BOOL GetCurrentDirectory(LPTSTR szPath, DWORD dwLen);


#endif//_EUTIL_H_