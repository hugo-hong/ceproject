#ifndef _GFILELIST_H_
#define _GFILELIST_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "GErr.h"

/**
*  @defgroup AVSwitch  AVSwitch
*
*  @brief The module for AV switch between front and rear,
*  @{
*/
typedef enum
{
    MISC_SOURCE_NONE,
	MISC_SOURCE_DVD,
	MISC_SOURCE_FILE,
    MISC_SOURCE_STRM_AV1,
	MISC_SOURCE_STRM_AV2,
	MISC_SOURCE_STRM_AV3,
	MISC_SOURCE_STRM_AV4,
	MISC_SOURCE_STRM_AV5
}E_MISC_SOURCE_TYPE;

typedef enum
{
	MISC_SINK_NONE,
    MISC_SINK_FRONT,
    MISC_SINK_REAR,
    MISC_SINK_FRONT_REAR,
}E_MISC_SINK_TYPE;


/**
* @brief  The method initializes avswitch module 
*
* @return GRESULT define in GDef.h
**/	
GRESULT GAVSwi_Init();

/**
* @brief  The method uninitializes avswitch module 
*
* @return GRESULT define in GDef.h
**/	
GRESULT GAVSwi_Uninit();

/**
* @brief  The method responsibles for open display,the method  be called before play.
*
* @param[in] eSoType open source type,that is file dvd avin and so on.
* @param[in] eSiType  display target to front or rear ,what's more,display on both sides
* 
* @return GRESULT define in GDef.h
**/	
GRESULT GAVSwi_Open(E_MISC_SOURCE_TYPE eSoType,E_MISC_SINK_TYPE eSiType);

/**
* @brief  The method responsibles for release resource,the method  be called after stop.
*
* @return GRESULT define in GDef.h
**/	
GRESULT GAVSwi_Close();


/**
* @brief  The method responsibles for change display target while playing.
*
* @param[in] eSoType open source type,that is file dvd avin and so on.
* @param[in] eSiType  display target to front or rear ,what's more,display on both sides
* 
* @return GRESULT define in GDef.h
**/
GRESULT GAVSwi_SinkSwitch(E_MISC_SOURCE_TYPE eSoType,E_MISC_SINK_TYPE eSiType);


/**
* @brief  The method responsibles specify display rectangle.
*
* @param[in] eSiType  display target to front or rear ,what's more,display on both sides
* @param[in] prRect  display target rectangle.
*
* @return GRESULT define in GDef.h
**/
GRESULT GAVSwi_SetSinkinfo(E_MISC_SINK_TYPE eSiType,RECT *prRect);

/**
*@}
*/

#ifdef __cplusplus
}
#endif

#endif