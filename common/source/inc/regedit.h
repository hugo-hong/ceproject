/************************************************************************
Filename:regedit.h

Description:CEReg class declartion

Data:2011/06/11

Author: Hugo
************************************************************************/
#ifndef __REGEDIT_H__
#define __REGEDIT_H__


class CEReg
{

public:
	CEReg();
	CEReg( HKEY, LPCTSTR   );
	~CEReg();

	BOOL Open( HKEY, LPCTSTR );
	BOOL Close( void );
	BOOL IsOpen( void );

	BOOL ReadDWORD(LPCTSTR lpszValueName, DWORD *pdwData);
	BOOL ReadString(LPCTSTR lpszValueName, LPVOID lpDataBuffer, DWORD dwLen);
	
	BOOL WriteDWORD(LPCTSTR lpszValueName, DWORD dwData);
	BOOL WriteString(LPCTSTR lpszValueName, LPVOID lpData);	
	BOOL WriteMultiString(LPCTSTR lpszValueName, LPVOID lpData, DWORD dwLen);	

	BOOL CreateKey(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR lpszClass);
	BOOL DeleteKey( HKEY hKey, LPCTSTR lpszSubKey);

	DWORD GetLastError();
	
protected:
	HKEY m_hKey;
	DWORD m_dwLastError;	

	BOOL Read(LPCTSTR lpszValueName, LPVOID lpDataBuffer, DWORD dwLen);
	BOOL Write(LPCTSTR lpszValueName, LPVOID lpData, DWORD dwType, DWORD dwLen);

};

#endif //__REGEDIT_H__