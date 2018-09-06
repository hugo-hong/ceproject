#include "stdafx.h"
#include "eutil.h"

BOOL Size2String(LONGLONG llBytes, LPTSTR pszSize, DWORD dwLen)
{
	double bytes = (double)llBytes;	
	LPCTSTR pszUnits[] = {_T("B"), _T("KB"), _T("MB"), _T("GB"), _T("TB")};
	int iUnits = DIM(pszUnits); 
	int index = 0;

    if (NULL == pszSize)
    {
        return FALSE;
    }

	// move from bytes to kb, to mb, to gb and so on diving by 1024
	while (bytes >= KB_SIZE && index < (iUnits - 1))
	{
		bytes /= KB_SIZE;
		index++;
	}

	_sntprintf(pszSize, dwLen, _T("%.2f  %s"), bytes, pszUnits[index]);

	return TRUE;
}

BOOL Tick2String(UINT64 u8Tick, LPTSTR pszTime, DWORD dwLen)
{
    DWORD dwTime = u8Tick / 1000;
    DWORD dwBytes = 0;  
    TIME_DATA time[] = {0, 0, 0};
    const char unit[] = {'S', 'M', 'H'};    
    int iUnits = DIM(unit); 
    int index = 0;

    if (NULL == pszTime)
    {
        return FALSE;
    }

    while ((dwTime >= TIME_SCALE) && (index < iUnits))
    {        
        time[index].time_sec.cUnit = unit[index];
        time[index].time_sec.u1Time = dwTime % TIME_SCALE;

        dwTime /= TIME_SCALE;
        index++;
    }

   _sntprintf(pszTime, dwLen, _T("%d : %d : %d"), 
       time[2].time_sec.u1Time, time[1].time_sec.u1Time, time[0].time_sec.u1Time);

    return TRUE;
}

LPWSTR MByteToWChar(LPCSTR pMStr)
{
	LPWSTR pWStr = NULL;
	int iLen = 0;

    if (NULL == pMStr)
    {
        return NULL;
    }

	iLen = MultiByteToWideChar(CP_ACP, 
							    0, 
							    pMStr,
							    -1, 
							    NULL, 
							    0);	

	pWStr = new WCHAR[iLen + 1];
	if (NULL == pMStr)
	{
		RETAILMSG(1, (_T("[Eutil]MByteToWChar:out of memory!!!\r\n")));
		return NULL;
	}
	else
	{
		memset(pWStr, 0, (iLen + 1) * sizeof(WCHAR));
	}

	MultiByteToWideChar(CP_ACP,
						0, 
						pMStr, 
						-1,
						pWStr, 
						iLen);

	return pWStr;
}

LPSTR WCharToMByte(LPCWSTR pWStr)
{
	LPSTR pMStr = NULL;
	int iLen = 0;

    if (NULL == pWStr)
    {
        return NULL;
    }

	iLen = WideCharToMultiByte(CP_ACP, 
                                0, 
                                pWStr, 
                                -1, 
                                NULL,
                                0, 
                                NULL,
                                NULL);
	pMStr = new char[iLen + 1];
	if (NULL == pMStr)
	{
		RETAILMSG(1, (_T("[Eutil]WCharToMByte:out of memory!!!\r\n")));
		return NULL;
	} 
	else
	{
		memset(pMStr, 0, iLen + 1);
	}

	WideCharToMultiByte(CP_ACP, 
                        0, 
                        pWStr, 
                        -1, 
                        pMStr,
                        iLen, 
                        NULL,
                        NULL);
	return pMStr;	
}

void DeleteBuffer(LPVOID pBuffer)
{
	if(pBuffer != NULL)
	{
		delete []pBuffer;
		pBuffer = NULL;
	}
}

UINT Write2File(LPBYTE pData, DWORD dwLen, LPCSTR szFile)
{
	FILE *pFile = NULL;	
	UINT nBytes = 0;

	if (NULL == pData || NULL == szFile)
	{
        RETAILMSG(1, (_T("[Eutil]Write2File:invalid parameters!!!\r\n")));
		return nBytes;
	}

	pFile = fopen(szFile, "a+b");
	if (NULL == pFile)
	{
		nBytes = -1;
	}
	else
	{
		nBytes = fwrite(pData, sizeof(BYTE), dwLen, pFile);		
		fclose(pFile);
	}

	return nBytes;
}

HANDLE Write2File(HANDLE hFile, LPBYTE pData, DWORD dwLen, LPCTSTR szFile)
{    
    if (NULL == pData || NULL == szFile)
    {
        RETAILMSG(1, (_T("[Eutil]Write2File:invalid parameters!!!\r\n")));
        return NULL;
    }

    if (NULL == hFile ||
        INVALID_HANDLE_VALUE == hFile)
    {
        hFile = ::CreateFile(szFile,            
            GENERIC_READ | GENERIC_WRITE,	//允许读和写
            0,								//独占方式（共享模式）
            NULL,
            OPEN_ALWAYS,					//打开,如果不存在就创建文件（创建方式）
            0,
            NULL 
            ); 
        if (INVALID_HANDLE_VALUE == hFile)
        {
            RETAILMSG(1, (_T("[Eutil]Write2File:create file err=%d\r\n"), GetLastError()));
            return NULL;
        }
    }
    else
    {
        ::SetEndOfFile(hFile);
    }
    
    //write data
    DWORD dwWritten = 0;
    DWORD dwBytes = 0;
    DWORD dwLength = dwLen;
    do 
    {
        ::WriteFile(hFile, pData + dwWritten, dwLength, &dwBytes, NULL);
        dwWritten += dwBytes;
        dwLength -= dwBytes;
    } while(dwLen != dwWritten);

    return hFile;
}

BOOL GetCurrentDirectory(LPTSTR szPath, DWORD dwLen)
{
    if (NULL == szPath || 0 == dwLen)
    {
        RETAILMSG(1, (_T("[Eutil]GetCurrentDirectory:invalid parameter!!!\r\n")));
        return FALSE;
    }   

    TCHAR szTemp[MAX_PATH] = {0};
    tstring strPath = _T("");
    tstring::size_type pos = 0;
    
    ::GetModuleFileName(NULL, szTemp, MAX_PATH);

    strPath.append(szTemp);

    pos = strPath.rfind(_T("\\")) + 1;
    
    if (pos > dwLen)
    {
         RETAILMSG(1, (_T("[Eutil]GetCurrentDirectory:dwLen=%d too short!!!\r\n"), dwLen));
         return FALSE;
    }
    else
    {
        _tcsncpy((TCHAR *)szPath, strPath.c_str(), pos);
    }    

    return TRUE;
}