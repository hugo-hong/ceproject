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
*   GObject.h 
*
* Project:
* --------
*   GPS Pilot
*
* Description:
* ------------
*   This file is the implementation of the generic object class.
*   
* Author:
* -------
*   mcn03046 jiechen@mtk.com.cn
*
* Last changed:
* ------------- 
* $Author: zeng.zhang $ 
*
* $Modtime: $  
*
* $Revision: #1 $
****************************************************************************/

#ifndef _GOBJECT_H_
#define _GOBJECT_H_

#include "GDef.h"
#include "GKal.h"

#define MEMBER_CALLBACK(pfn)  static_cast <PFN_MEMBERCALLBACK> (pfn)

class GObject;
class GCmdSource;

/**
 * The callback function prototype and the function is the member function of
 * the generic object. 
 */
typedef GBOOL (GObject::*PFN_MEMBERCALLBACK)(GCmdSource *pobjSender,
                                            GUINT32      u4Event, 
                                            GUINT32      u4Param1,
                                            GUINT32      u4Param2);

/**
 * The function prototype and the function is the normal function.
 */
typedef GBOOL (*PFN_NORMALCALLBACK)(GCmdSource *pobjSender,
                                   GUINT32      u4Event, 
                                   GUINT32      u4Param1,
                                   GUINT32      u4Param2);

/**
 * The GObject class is the base class for the other object, for example
 * the GCmdSource, GWidget, GGisEngine class, and so on.
 */
class GAPI GObject
{
public:
    virtual ~GObject(GVOID) {};

    GVOID *operator new(GUINT u4Size) { return (GAlloc(u4Size)); }

    GVOID  operator delete(GVOID *pvObj) { GFree(pvObj); }
};


/**
* The GCmdSource class is the base class that the callback function can be registered into.
*/
class GAPI GCmdSource : public GObject
{
public:
    /**
    * Construct a GCmdSource object.
    *
    * @param NONE.
    */
    GCmdSource(GVOID);

    /**
    * Destruct a GCmdSource object.
    *
    * @param NONE.
    */
    virtual ~GCmdSource(GVOID);

    /**
    * Register a member callback function into the GCmdSource object.
    *
    * @param obj   -- Pointer to a GObject object that implement the callback
    *                 function.
    * @param pfnCb -- Function pointer to the callback function.
    *
    * @return If register the callback function successfully, the return value
    *         is TRUE, otherwise the return value is FALSE.
    */
    GBOOL RegCallback(GObject *obj, PFN_MEMBERCALLBACK pfnCb);

    /**
    * Register a normal callback function into the GCmdSource object.
    *
    * @param pfnCb -- Function pointer to the callback function.
    *
    * @return If register the callback function successfully, the return value
    *         is TRUE, otherwise the return value is FALSE.
    */
    GBOOL RegCallback(PFN_NORMALCALLBACK pfnCb);

    /**
    * Unregister a callback function from the GCmdSource object.
    *
    * @param obj   -- Pointer to a GObject object that implement the callback
    *                 function.
    * @param pfnCb -- Function pointer to the callback function.
    *
    * @return NONE.
    */
    GVOID UnregCallback(GObject *obj, PFN_MEMBERCALLBACK pfnCb);

    /**
    * Unregister a callback function from the GCmdSource object.
    *
    * @param pfnCb -- Function pointer to the callback function.
    *
    * @return NONE.
    */
    GVOID UnregCallback(PFN_NORMALCALLBACK pfnCb);

    /**
    * Unregister all callback functions from the GCmdSource object.
    *
    * @param NONE.
    *
    * @return NONE.
    */
    GVOID UnregAllCallback(GVOID);

protected:
    GBOOL InvokeCallback(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);

private:

    GVOID   *AllocNode(GVOID);
    GVOID    FreeNode(GVOID *);

    struct _CBNODE  *m_prLastCb;
    struct _CBCHUNK *m_prCbChunk;
};


GBOOL    GInitObject(GVOID);
GVOID    GDeinitObject(GVOID);


#endif // _GOBJECT_H_