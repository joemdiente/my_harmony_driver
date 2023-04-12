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

#ifndef _DRV_APA102_H    /* Guard against multiple inclusion */
#define _DRV_APA102_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include "drv_apa102_definitions.h"
#include "system/system_module.h"

//Public API interface, function declarations
SYS_MODULE_OBJ DRV_APA102_Initialize( const SYS_MODULE_INDEX drvIndex, const SYS_MODULE_INIT* const init, DRV_APA102_CONFIG_PARAMS*);

DRV_HANDLE DRV_APA102_Open(const SYS_MODULE_INDEX drvIndex, const DRV_IO_INTENT ioIntent);

void DRV_APA102_Close(const DRV_HANDLE handle);

uint8_t DRV_APA102_WriteToLed(const DRV_HANDLE handle, uint8_t* ledNum, DRV_APA102_RGB_DATA* rgbVal);

//bool DRV_APA102_WriteArray(const DRV_HANDLE handle, DRV_APA102_RGB_ARRAY* rgbArr);

bool DRV_APA102_TransferSetup(const DRV_HANDLE handle, DRV_APA102_CONFIG_PARAMS* configParams);

#endif /* _DRV_APA102_H */

/* *****************************************************************************
 End of File
 */
