#pragma once


#define DIM(a)		sizeof(a)/sizeof(a[0])
#define INI_RETURN_ERR 0xFFFF

class RWIni
{
    public:
        RWIni(void);
        ~RWIni(void);

        /******************************************************************************
        Prototype        int RWIni::InitStorage(TCHAR *szFileName)
        Description      Init ini file path
        Input            TCHAR *szFileName                           Ini file name
        Output
        Return           0                                           Fail
                         Other                                       Success
        Date             2010-5-31 11:49:09
        ******************************************************************************/
        int InitStorage(TCHAR* szFileName);
        int InitRemoteStorage(TCHAR *szFileName);
        /******************************************************************************
        Prototype        UINT RWIni::GetIniString(TCHAR* szTypeName, TCHAR* szKeyName, TCHAR* szReturnedString)
        Description      Get ini value
        Input            TCHAR* szTypeName                           Section name
        Input            TCHAR* szKeyName                            Key name
        Output           TCHAR* szReturnedString                     Value
        Return           UINT                                        String length
                         INI_RETURN_ERR                              Fail
        Date             2010-5-31 11:44:32
        ******************************************************************************/
        UINT RWIni::GetIniString(TCHAR* szTypeName, TCHAR* szKeyName, TCHAR* szReturnedString);

        /******************************************************************************
        Prototype        BOOL RWIni::SetIniString(CHAR* szTypeName, TCHAR* szKeyName, TCHAR* szString)
        Description      Set ini value

        Input            TCHAR* szTypeName                           Section name
        Input            TCHAR* szKeyName                            Key name, if key name is NULL, the section'll be deleted
        Input            TCHAR* szString                             Value, if Value is NULL, the key'll be deleted
        Output
        Return           1                                           Success
                         0                                           Fail
        Date             2010-5-31 11:36:21
        ******************************************************************************/
        BOOL RWIni::SetIniString(TCHAR* szTypeName, TCHAR* szKeyName, TCHAR* szString);

    private:
        TCHAR szFileNameAndPath[MAX_PATH];
        int GetWorkPath(TCHAR *szPath, TCHAR *szFileName);

};