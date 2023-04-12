/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _DRV_APA102_LOCAL_H    /* Guard against multiple inclusion */
#define _DRV_APA102_LOCAL_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include "drv_apa102_definitions.h"
#include <stdbool.h>

typedef struct
{
    bool isInitialized;
    bool isBusy;    
    bool wrInProgress;
    uint8_t nClients;
    uint16_t ledMaxCnt;
    const DRV_APA102_PLIB_INTERFACE* plibInterface;    
    DRV_APA102_APP_CALLBACK callback; 
    uintptr_t context;
    DRV_APA102_EVENT event;
    bool isSetupChanged;
    DRV_APA102_RGB_DATA* rgbData;
}DRV_APA102_OBJ;

#endif /* _DRV_APA102_LOCAL_H */

/* *****************************************************************************
 End of File
 */
