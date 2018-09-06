/************************************************************************
Filename:regedit.h

Description:CEReg class implement

Data:2011/06/11

Author: Hugo
************************************************************************/

#include "stdafx.h"
#include "regedit.h"

CEReg::CEReg()
{
	m_hKey = NULL;	
	m_dwLastError = ERROR_SUCCESS;
}

CEReg::CEReg( HKEY hKey, LPCTSTR lpszSubKey )
{
	m_hKey = NULL;
	Open( hKey, lpszSubKey );
}

CEReg::~CEReg()
{
	Close();
}

BOOL CEReg::Open( HKEY hKey, LPCTSTR lpszSubKey )
{
	Close();

	if (::RegOpenKeyEx( hKey, lpszSubKey, 0, KEY_ALL_ACCESS, &m_hKey) != ERROR_SUCCESS)
	{
		m_hKey = NULL;
        m_dwLastError = ::GetLastError();
		return( FALSE );
	}

	return( TRUE );
}

BOOL CEReg::Close(void)
{
	BOOL bRet = TRUE;

	if(m_hKey == NULL)
		return FALSE;

	bRet = (::RegCloseKey(m_hKey) == ERROR_SUCCESS);
	m_hKey = NULL;

	return bRet;
}

BOOL CEReg::IsOpen( void )
{
	return( m_hKey != NULL );
}

BOOL CEReg::Read(LPCTSTR lpszValueName, LPVOID lpDataBuffer, DWORD dwLen)
{
	if( m_hKey == NULL )
		return FALSE;
	
	BOOL bRet = (::RegQueryValueEx( m_hKey,
                    lpszValueName,
                    NULL,
                    NULL,
                    (unsigned char *) lpDataBuffer, 
                    &dwLen) == ERROR_SUCCESS);

    m_dwLastError = ::GetLastError();

	return bRet;
}

BOOL CEReg::ReadDWORD( LPCTSTR lpszValueName, DWORD *pdwData)
{
	if( m_hKey == NULL )
		return FALSE;

	return Read(lpszValueName, pdwData, sizeof(DWORD));
}

BOOL CEReg::ReadString(LPCTSTR lpszValueName, LPVOID lpDataBuffer, DWORD dwLen)
{
	if(m_hKey == NULL)
		return FALSE;

	memset(lpDataBuffer, 0, dwLen);
	return Read(lpszValueName, lpDataBuffer, dwLen);

}

BOOL CEReg::Write(LPCTSTR lpszValueName, LPVOID lpData, DWORD dwType, DWORD dwLen)
{
	if( m_hKey == NULL )
		return( FALSE );
	
	BOOL bRet = (::RegSetValueEx(m_hKey, 
                    lpszValueName,
                    0,
                    dwType,
                    (unsigned char *) lpData,
                    dwLen) == ERROR_SUCCESS);

    m_dwLastError = ::GetLastError();

	return bRet;
}


BOOL CEReg::WriteDWORD(LPCTSTR lpszValueName, DWORD dwData)
{
	if( m_hKey == NULL )
		return FALSE;
	
	return Write( lpszValueName, &dwData, REG_DWORD, sizeof( DWORD ));	
}

BOOL CEReg::WriteString(LPCTSTR lpszValueName, LPVOID lpData)
{
	if( m_hKey == NULL )
		return FALSE;
	
	return Write(lpszValueName, lpData, REG_SZ, (_tcslen((const TCHAR *) lpData) + 1) * sizeof(TCHAR));
	
}

BOOL CEReg::WriteMultiString(LPCTSTR lpszValueName, LPVOID lpData, DWORD dwLen)
{
	if( m_hKey == NULL )
		return FALSE;
	
	return Write(lpszValueName, lpData, REG_MULTI_SZ, dwLen);	
}


BOOL CEReg::CreateKey(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR lpszClass)
{
	HKEY hKeyCreate;
	DWORD dwDisposition;	

	BOOL bRet = (::RegCreateKeyEx(hKey, 
                    lpszSubKey,
                    0,
                    (TCHAR *)lpszClass,
                    REG_OPTION_NON_VOLATILE,
                    KEY_ALL_ACCESS,
                    NULL, 
                    &hKeyCreate,
                    &dwDisposition) == ERROR_SUCCESS);
	if (bRet) 
	{
		::RegCloseKey(hKeyCreate);
	}
		
    m_dwLastError = ::GetLastError();

	return bRet;
}

BOOL CEReg::DeleteKey( HKEY hKey, LPCTSTR lpszSubKey)
{
	BOOL bRet;
	DWORD dwLastError;

	bRet = (::RegDeleteKey( hKey, lpszSubKey) == ERROR_SUCCESS);
    dwLastError = ::GetLastError();

	return bRet;

}

DWORD CEReg::GetLastError()
{
	return m_dwLastError;
}
