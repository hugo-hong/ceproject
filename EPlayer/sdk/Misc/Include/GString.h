/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008 MediaTek Inc.
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
*
* Filename:
* ---------
*   GString.h 
*
* Project:
* --------
*   GPS Pilot
*
* Description:
* ------------
*   
*   
* Author:
* -------
*   
*
* Last changed:
* ------------- 
* $Author: zeng.zhang $ 
*
* $Modtime: $  
*
* $Revision: #1 $
****************************************************************************/

#ifndef GBASE_STRING_H
#define GBASE_STRING_H

#ifdef _WIN32
#pragma  warning(disable: 4267)
#endif

#include "GDef.h"
#include "GKal.h"
#include "GLog.h"
#include "GObject.h" //\\ JK,
#include <stdlib.h>

#ifndef ASSERT
#define ASSERT assert 
#endif

//\\ Jau :
#define        _wcscmpi    _wcsicmp

extern char* _strrev (char*);
extern int   _stricmp (const char*, const char*);
extern GWCHAR* _wcsrev (GWCHAR*);
extern int    _wcsicmp (const GWCHAR*, const GWCHAR*);

/**
*  Define Internal Memory Management Routine
*
*/
struct GMemData;

class GMemoryMgr : public GObject //\\ JK,
{
public:
    // Allocate/Free/Reallocate a new GMemData
    GMemData*    Allocate(const GUINT16 u2Chars, const GUINT16 u2CharSize);
    GVOID         Free(GMemData* pData);
    //GMemData*    Reallocate(GMemData* pData, const GUINT16 u2Chars, const GUINT16 u2CharSize);
};

/**
* Class GMemData contains actual string data and a reference count.
* ANSI/UNICODE version
*/
struct GMemData
{
    GINT32        i4Refs;     // reference count
    GUINT16      u2Length;
    GUINT16      u2Partition;
    GCHAR*       data() { return (GCHAR*)(this + 1); }
};

static GINT32 _rNilData[] = { -1, 0, 0, 0}; // 2007/03/15 Fixed a bug by Jun
static GMemData* _NilString = (GMemData *)&_rNilData;
static const GCHAR* _pNilData = (const GCHAR*)(((GBYTE*)&_rNilData) + sizeof(GMemData));

/**
* @addtogroup  gstringGroup GString
* @brief 
*
* @ingroup Utilities
*/
/*@{*/

///////////////////////////////////////////////////////////////////////
// Class - GStringA
//
//

class GStringA// : public GObject //\\ JK,
{
public:
    // Constructors
    GStringA(); 
    GStringA(const GStringA& stringSrc);
    GStringA(const GCHAR* lpszA);
    GStringA(GCHAR ch, GINT i4Repeat = 1);
    GStringA(const GWCHAR* lpszW);
    // Destructor
    ~GStringA();

    // Attributes & Operations
    GUINT16  GetLength() const { return GetData()->u2Length;      }
    GBOOL    IsEmpty()   const { return GetData()->u2Length == 0; }
    GUINT16  GetBytes()  const { return GetLength() * sizeof(GCHAR);  }
    GVOID    Empty();

    GCHAR    GetAt(GINT i4Index) const;       // Zero-based index
    GCHAR    operator[](GINT i4Index) const;
    GVOID    SetAt(GINT i4Index, GCHAR ch);

    operator GLPCSTR()    const { return m_pchData; }
    GCHAR*    c_str()        { return m_pchData; }

    // overloaded assignment
    //\\ JK, {
    GVOID *operator new(GUINT u4Size) { return (GAlloc(u4Size)); }
    GVOID  operator delete(GVOID *pvObj) { GFree(pvObj); }
    //\\ JK, }

    // overloaded assignment
    const GStringA& operator=(const GStringA& stringSrc);

    const GStringA& operator=(GCHAR ch);
    const GStringA& operator=(const GCHAR* lpszA);
    const GStringA& operator=(const GWCHAR* lpszW);

    // string concatenation
    const GStringA& operator+=(const GWCHAR* lpszW);
    const GStringA& operator+=(GCHAR ch);

    const GStringA& operator+=(const GCHAR* lpszA);

    // Global operations
    friend GStringA  operator+(const GStringA& string1, const GStringA& string2);
    friend GStringA  operator+(const GStringA& string, GCHAR ch);
    friend GStringA  operator+(const GStringA& string, GWCHAR ch);
    friend GStringA  operator+(GWCHAR ch, const GStringA& string);
    friend GStringA  operator+(GCHAR ch, const GStringA& string);

    // string comparison
    GINT     Compare(const GCHAR* lpszA)            const { return _cStrCmp(m_pchData, lpszA); }    // MBCS/Unicode aware
    GINT     CompareNoCase(const GCHAR* lpszA)    const { return _cStrCmpi(m_pchData, lpszA); }   // MBCS/Unicode aware

    // Upper/Lower/Reverse conversion
    GVOID    MakeUpper();
    GVOID    MakeLower();
    GVOID    MakeReverse();

    // trimming whitespace (either side)
    GVOID    TrimRight();   
    GVOID    TrimLeft();

    // format string 
    GBOOL    Format(const GCHAR* lpszFormat, ...);

    // replace occurrences of chOld with chNew
    GINT     Replace(GCHAR chOld, GCHAR chNew); 

    // remove occurrences of chRemove
    GINT     Remove(GCHAR chRemove);

    // insert character at zero-based index;
    GINT     Insert(GINT i4Index, GCHAR ch);   

    // insert substring at zero-based index; 
    GINT     Insert(GINT i4Index, const GCHAR* pstr);    

    // delete i4Count characters starting at zero-based index
    GINT     Delete(GINT i4Index, GINT i4Count = 1);    

    // searching (return starting index, or -1 if not found)
    GINT     ReverseFind(GCHAR ch) const;    
    GINT     Find(GCHAR ch, GINT i4Start) const;         // starting at index   

    // look for a specific sub-string
    GINT     Find(const GCHAR* lpszSub, GINT i4Start) const; 
    GINT     Find(const GCHAR* lpszSub) const        { return Find(lpszSub, 0); }

    GVOID    ReleaseBuffer(GINT u4NewLen = -1);
    GCHAR*   GetBuffer(GINT i4MinBufLen);

    // Implementation for future use
    GUINT16  GetPartitionSize() const  { return GetData()->u2Partition; }

protected:
    GCHAR*    m_pchData;   // pointer to actual memory
    static    GMemoryMgr        m_Mgr;

    // implementation helpers
    GMemData* GetData() const
    { 
        ASSERT(m_pchData != NULL); 
        return ((GMemData*)m_pchData) - 1; 
    }

    GVOID    InitNullString();

    GBOOL    AllocMemory(GINT nLen);
    GVOID    FreeMemory();
    static  GVOID  FreeMemory(GMemData* pData);

    GBOOL    CopyCore(GINT i4Src1Len, const GCHAR* lpszSrc1Data, GINT i4Src2Len, const GCHAR* lpszSrc2Data);
    GVOID    ConcateCore(GINT i4SrcLen, const GCHAR* lpszSrcData);
    GVOID    AppendCore(GINT i4SrcLen, const GCHAR* lpszSrcData);

    GVOID    CopyOnWrite();

    // Internal helper functions
    static GCHAR* _cStrChr(GCHAR* p, GCHAR ch)     { return strchr(p, ch); }

    static GCHAR* _cStrrChr(GCHAR* p, GCHAR ch)    { return strrchr(p, ch); }

    //\\ Jau
    //static GCHAR* _cStrRev(GCHAR* pStr)           { return _strrev(pStr); }

    //\\ Jau : GStrStr
    static GCHAR* _cStrStr(GCHAR* pStr, const GCHAR* pCharSet)
    { return GStrStrA(pStr, pCharSet); }

    static GINT  _cIsDigit(GCHAR ch)            
    { 
        return isdigit((GUCHAR)ch); 
    }

    static GINT  _cIsSpace(GCHAR ch)            
    {
        return isspace((GUCHAR)ch); 
    }

    static GINT  _cStrCmp(const GCHAR* pstrOne, const GCHAR* pstrOther)
    { return strcmp(pstrOne, pstrOther); }

    static GINT  _cStrCmpi(const GCHAR* pstrOne, const GCHAR* pstrOther)
    { return _stricmp(pstrOne, pstrOther); }

    static GINT  _cStrToInt(const GCHAR* pStr)
    { return  atoi(pStr); }

};

// Compare helper functions
inline GBOOL operator==(const GStringA& s1, const GStringA& s2)
{ return s1.Compare(s2) == 0; }

inline GBOOL operator==(const GStringA& s1, const GCHAR* s2)
{ return s1.Compare(s2) == 0; }

inline GBOOL operator==(const GCHAR* s1, const GStringA& s2)
{ return s2.Compare(s1) == 0; }

inline GBOOL operator!=(const GStringA& s1, const GStringA& s2)
{ return s1.Compare(s2) != 0; }

inline GBOOL operator!=(const GStringA& s1, const GCHAR* s2)
{ return s1.Compare(s2) != 0; }

inline GBOOL operator!=(const GCHAR* s1, const GStringA& s2)
{ return s2.Compare(s1) != 0; }

inline GBOOL operator<(const GStringA& s1, const GStringA& s2)
{ return s1.Compare(s2) < 0; }

inline GBOOL operator<(const GStringA& s1, const GCHAR* s2)
{ return s1.Compare(s2) < 0; }

inline GBOOL operator<(const GCHAR* s1, const GStringA& s2)
{ return s2.Compare(s1) > 0; }

inline GBOOL operator>(const GStringA& s1, const GStringA& s2)
{ return s1.Compare(s2) > 0; }

inline GBOOL operator>(const GStringA& s1, const GCHAR* s2)
{ return s1.Compare(s2) > 0; }

inline GBOOL operator>(const GCHAR* s1, const GStringA& s2)
{ return s2.Compare(s1) < 0; }

inline GBOOL operator<=(const GStringA& s1, const GStringA& s2)
{ return s1.Compare(s2) <= 0; }

inline GBOOL operator<=(const GStringA& s1, const GCHAR* s2)
{ return s1.Compare(s2) <= 0; }

inline GBOOL operator<=(const GCHAR* s1, const GStringA& s2)
{ return s2.Compare(s1) >= 0; }

inline GBOOL operator>=(const GStringA& s1, const GStringA& s2)
{ return s1.Compare(s2) >= 0; }

inline GBOOL operator>=(const GStringA& s1, const GCHAR* s2)
{ return s1.Compare(s2) >= 0; }

inline GBOOL operator>=(const GCHAR* s1, const GStringA& s2)
{ return s2.Compare(s1) <= 0; }

// friend functions
inline GStringA operator+(const GStringA& string1, const GStringA& string2)
{
    GStringA s;
    s.CopyCore(string1.GetData()->u2Length, string1.m_pchData, string2.GetData()->u2Length, string2.m_pchData);
    return s;
}

inline GStringA operator+(const GStringA& string1, GCHAR ch)
{
    GStringA s;
    s.CopyCore(string1.GetData()->u2Length, string1.m_pchData, 1, &ch);
    return s;
}

inline GStringA operator+(GCHAR ch, const GStringA& string1)
{
    GStringA s;
    s.CopyCore(1, &ch, string1.GetData()->u2Length, string1.m_pchData);
    return s;
}


///////////////////////////////////////////////////////////////////////
// Class GStringW
//
//

class GStringW
{
public:
    // Constructors
    GStringW(); 
    GStringW(const GStringW& stringSrc);
    GStringW(const GCHAR* lpszA);
    GStringW(GWCHAR ch, GINT i4Repeat = 1);
    GStringW(const GWCHAR* lpszW);

    ~GStringW();

    // Attributes & Operations
    GUINT16  GetLength() const { return (GUINT16)GStrLenW(m_pchData);      }
    GBOOL    IsEmpty()   const { return m_pchData == NULL; }
    GUINT16  GetBytes()  const { return GetLength() * sizeof(GWCHAR); }
    GVOID    Empty();

    GWCHAR   GetAt(GINT i4Index) const;      // Zero-based index
    GWCHAR   operator[](GINT i4Index) const;
    GVOID    SetAt(GINT i4Index, GWCHAR ch);

    operator GLPCWSTR() const    { return m_pchData; }
    GWCHAR*    c_str()        { return m_pchData; }

    // overloaded assignment
    //\\ JK, {
    GVOID *operator new(GUINT u4Size) { return (GAlloc(u4Size)); }
    GVOID  operator delete(GVOID *pvObj) { GFree(pvObj); }
    //\\ JK, }
    
    const GStringW& operator=(const GStringW& stringSrc);

    const GStringW& operator=(GWCHAR ch);
    const GStringW& operator=(const GCHAR* lpszA);
    const GStringW& operator=(const GWCHAR* lpszW);

    // string concatenation
    const GStringW& operator+=(const GWCHAR* lpszW);
    const GStringW& operator+=(GWCHAR ch);

    const GStringW& operator+=(const GCHAR* lpszA);

    // Concatenation
    // NOTE: "operator+" is done as friend functions for simplicity

    friend GStringW  operator+(const GStringW& string1, const GStringW& string2);
    friend GStringW  operator+(const GStringW& string, GCHAR ch);
    friend GStringW  operator+(const GStringW& string, GWCHAR ch);
    friend GStringW  operator+(GWCHAR ch, const GStringW& string);
    friend GStringW  operator+(GCHAR ch, const GStringW& string);

    // string comparison
    GINT     Compare(const GWCHAR* lpszW)   const { return _cStrCmp(m_pchData, lpszW); }    // MBCS/Unicode aware
    GINT     CompareNoCase(const GWCHAR* lpszW) const { return _cStrCmpi(m_pchData, lpszW); }   // MBCS/Unicode aware

    // Upper/Lower/Reverse conversion
    GVOID    MakeUpper();
    GVOID    MakeLower();
    GVOID    MakeReverse();

    // trimming whitespace (either side)
    GVOID    TrimRight();   
    GVOID    TrimLeft();

    // replace occurrences of chOld with chNew
    GINT     Replace(GWCHAR chOld, GWCHAR chNew); 

    // remove occurrences of chRemove
    GINT     Remove(GWCHAR chRemove);

    // insert character at zero-based index;
    GINT     Insert(GINT i4Index, GWCHAR ch);   

    // insert substring at zero-based index; 
    GINT     Insert(GINT i4Index, const GWCHAR* pstr);    

    // delete i4Count characters starting at zero-based index
    GINT     Delete(GINT i4Index, GINT i4Count = 1);    

    // format string 
    GBOOL    Format(const GWCHAR* lpszFormat, ...);

    // searching (return starting index, or -1 if not found)
    // look for a single character match
    GINT     ReverseFind(GWCHAR ch) const;    
    GINT     Find(GWCHAR ch, GINT i4Start) const;

    // look for a specific sub-string
    GINT     Find(const GWCHAR* lpszSub, GINT i4Start) const; 
    GINT     Find(const GWCHAR* lpszSub) const   { return Find(lpszSub, 0); }

    // Access to string implementation buffer as "C" character array
    GWCHAR*  GetBuffer(GINT i4MinBufLen);
    GVOID    ReleaseBuffer(GINT i4NewLen = -1);

    // Implementation for future use
    GUINT16    GetPartitionSize() const  { return GetData()->u2Partition; }


protected:
    GWCHAR*    m_pchData;   // pointer to actual memory
    static    GMemoryMgr        m_Mgr;

    // implementation helpers
    GMemData* GetData() const
    { 
        ASSERT(m_pchData != NULL); 
        return ((GMemData*)m_pchData) - 1; 
    }

    GVOID    InitNullString();

    GBOOL    AllocMemory(GINT nLen);
    GVOID    FreeMemory();
    static  GVOID FreeMemory(GMemData* pData);  

    GBOOL    CopyCore(GINT i4Src1Len, const GWCHAR* lpszSrc1Data, GINT i4Src2Len, const GWCHAR* lpszSrc2Data);
    GVOID    ConcateCore(GINT i4SrcLen, const GWCHAR* lpszSrcData);
    GVOID    AppendCore(GINT i4SrcLen, const GWCHAR* lpszSrcData);

    GVOID    CopyOnWrite(); 

    static GWCHAR*  _cStrChr(GWCHAR* p, GWCHAR ch)
    //\\ Jau :
    //{ return wcschr(p, ch); }
    { return GStrChrW((const GWCHAR*)p, ch); }

    static GWCHAR*  _cStrrChr(GWCHAR* p, GWCHAR ch)
    //\\ Jau :
    //{ return wcsrchr(p, ch); }
    { return GStrrChrW((const GWCHAR*)p, ch); }

    //\\ Jau
    //static GWCHAR*  _cStrRev(GWCHAR* pStr)
    //{ return _wcsrev(pStr); }

    static GWCHAR*  _cStrStr(GWCHAR* pStr, const GWCHAR* pCharSet)
    //\\ Jau :
    //{ return wcsstr(pStr, pCharSet); }
    { return GStrStrW((const GWCHAR*)pStr, pCharSet); }

    static GINT  _cIsDigit(GWCHAR ch)
    { 
        return iswdigit(ch); 
    }

    static GINT  _cIsSpace(GWCHAR ch)    
    { 
        return iswspace(ch); 
    }

    static GINT  _cStrCmp(const GWCHAR* pstrOne, const GWCHAR* pstrOther)
    //\\ Jau :
    //{ return wcscmp(pstrOne, pstrOther); }
    { return GStrCmpW(pstrOne, pstrOther); }

    static GINT  _cStrCmpi(const GWCHAR* pstrOne, const GWCHAR* pstrOther)
    { return _wcsicmp(pstrOne, pstrOther); }

    static GINT  _cStrToInt(const GWCHAR* pStr)
    //\\ Jau :
    //{ return _wtoi(pStr); }
    { return GStr2IntW(pStr); }


};

// Compare helpers
inline GBOOL  operator==(const GStringW& s1, const GStringW& s2)
{ return s1.Compare(s2) == 0; }

inline GBOOL  operator==(const GStringW& s1, const GWCHAR* s2)
{ return s1.Compare(s2) == 0; }

inline GBOOL  operator==(const GWCHAR* s1, const GStringW& s2)
{ return s2.Compare(s1) == 0; }

inline GBOOL  operator!=(const GStringW& s1, const GStringW& s2)
{ return s1.Compare(s2) != 0; }

inline GBOOL  operator!=(const GStringW& s1, const GWCHAR* s2)
{ return s1.Compare(s2) != 0; }

inline GBOOL  operator!=(const GWCHAR* s1, const GStringW& s2)
{ return s2.Compare(s1) != 0; }

inline GBOOL  operator<(const GStringW& s1, const GStringW& s2)
{ return s1.Compare(s2) < 0; }

inline GBOOL  operator<(const GStringW& s1, const GWCHAR* s2)
{ return s1.Compare(s2) < 0; }

inline GBOOL  operator<(const GWCHAR* s1, const GStringW& s2)
{ return s2.Compare(s1) > 0; }

inline GBOOL  operator>(const GStringW& s1, const GStringW& s2)
{ return s1.Compare(s2) > 0; }

inline GBOOL  operator>(const GStringW& s1, const GWCHAR* s2)
{ return s1.Compare(s2) > 0; }

inline GBOOL  operator>(const GWCHAR* s1, const GStringW& s2)
{ return s2.Compare(s1) < 0; }

inline GBOOL  operator<=(const GStringW& s1, const GStringW& s2)
{ return s1.Compare(s2) <= 0; }

inline GBOOL  operator<=(const GStringW& s1, const GWCHAR* s2)
{ return s1.Compare(s2) <= 0; }

inline GBOOL  operator<=(const GWCHAR* s1, const GStringW& s2)
{ return s2.Compare(s1) >= 0; }

inline GBOOL  operator>=(const GStringW& s1, const GStringW& s2)
{ return s1.Compare(s2) >= 0; }

inline GBOOL  operator>=(const GStringW& s1, const GWCHAR* s2)
{ return s1.Compare(s2) >= 0; }

inline GBOOL  operator>=(const GWCHAR* s1, const GStringW& s2)
{ return s2.Compare(s1) <= 0; }

// friend functions
inline GStringW operator+(const GStringW& string1, const GStringW& string2)
{
    GStringW s;
    s.CopyCore(string1.GetData()->u2Length, string1.m_pchData, string2.GetData()->u2Length, string2.m_pchData);
    return s;
}

inline GStringW operator+(const GStringW& string1, GCHAR ch)
{
    GStringW s;
    GWCHAR wch(ch);
    s.CopyCore(string1.GetData()->u2Length, string1.m_pchData, 1, &wch);
    return s;
}

inline GStringW operator+(const GStringW& string1, GWCHAR ch)
{
    GStringW s;
    s.CopyCore(string1.GetData()->u2Length, string1.m_pchData, 1, &ch);
    return s;
}

inline GStringW operator+(GWCHAR ch, const GStringW& string1)
{
    GStringW s;
    s.CopyCore(1, &ch, string1.GetData()->u2Length, string1.m_pchData);
    return s;
}

inline GStringW operator+(GCHAR ch, const GStringW& string1)
{
    GStringW s;
    GWCHAR wch(ch);
    s.CopyCore(1, &wch, string1.GetData()->u2Length, string1.m_pchData);
    return s;
}

// Default Macro
#ifdef  _UNICODE 
#define  GString        GStringW
#else
#define  GString        GStringA
#endif


#endif  // GBASE_STRING_H
