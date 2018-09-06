/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*****************************************************************************/

/*****************************************************************************
*
* Filename:
* ---------
*   $Workfile:  $ 
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
*   mcn03046 mcn05013
*
* Last changed:
* ------------- 
* $Author: zeng.zhang $ 
*
* $Modtime: $  
*
* $Revision: #1 $
****************************************************************************/


#ifndef _GSTRINGDATA_H_
#define _GSTRINGDATA_H_

#include "GKal.h"
#include "GResource.h"

struct GStringData
{
public:
    GStringData() : m_pString(NULL)
    {
        fgType = NODATA;

    }

    GStringData(GTCHAR *szStr) : m_pString(NULL)
    {
        fgType = POINTERDATA;
        Data.szStr = szStr;
    }

    GStringData(GINT16 i2StrID) : m_pString(NULL)
    {
        fgType = IDDATA;
        Data.i2StrID = i2StrID;
    }
    
    ~GStringData()
    {
        if (fgType == IDDATA)
        {
            if (m_pString != NULL)
            {
                GDestroyAppString(m_pString);
            }            
        }
    }

    GVOID SetData(GStringData *pstrText)
    {
        if (!pstrText)
        {
            return;
        }
        fgType = pstrText->fgType;
        Data = pstrText->Data;
    }

    GVOID SetData(GTCHAR *szStr)
    {
        fgType = POINTERDATA;
        Data.szStr = szStr;
    }

    GVOID SetData(GINT16 i2StrID)
    {
        fgType = IDDATA;
        Data.i2StrID = i2StrID;
    }

    GBOOL IsContainData()
    {
        return(!(fgType == NODATA));
    }

    GINT32 GetLength(GVOID)
    {
        if (fgType == POINTERDATA)
        {
            return (GStrLen(Data.szStr));
        }
        else
        {
            GTCHAR *szTmp;
            GINT32 i4Len;

            szTmp = (GTCHAR*)GLoadAppString(Data.i2StrID);
            i4Len = GStrLen(szTmp);
            GDestroyAppString(szTmp);
            return (i4Len);            
        }
    }
    
    GBOOL operator ==(const GStringData szOp)
    {
        if (fgType == szOp.fgType && 
            ((fgType == POINTERDATA && (!GStrCmp(Data.szStr, szOp.Data.szStr))) ||
            (fgType == IDDATA && Data.i2StrID == szOp.Data.i2StrID)) )
        {
            return (TRUE);
        }
        else
        {
            return (FALSE);
        }
    }

    const GStringData& operator = (GStringData &szOp)
    {
        fgType = szOp.fgType;

        if(szOp.fgType == POINTERDATA)
        {
            Data.szStr = szOp.Data.szStr;
        }
        else
        {
            Data.i2StrID = szOp.Data.i2StrID;
        }

        return (*this);
    }
    
    GTCHAR *GetData()
    {
        if (fgType == NODATA)
        {
            return(NULL);
        }
        else if (fgType == POINTERDATA)
        {
            return(Data.szStr);
        }
        else
        {
            if (m_pString != NULL)
            {
                GDestroyAppString(m_pString);
                m_pString = NULL;
            }   
            m_pString = ((GTCHAR *)GLoadAppString(Data.i2StrID));
            return m_pString;
        }
    }

    enum
    {
        POINTERDATA,
        IDDATA,
        NODATA
    } fgType;

    union
    {
        GTCHAR *szStr;
        GINT16 i2StrID;
    } Data;

    GVOID *operator new(GUINT u4Size) { return (GAlloc(u4Size)); }

    GVOID  operator delete(GVOID *pvObj) { GFree(pvObj); }

    private:
    GTCHAR*      m_pString;
};

#endif //_GSTRINGDATA_H_