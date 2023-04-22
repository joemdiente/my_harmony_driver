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

// *****************************************************************************
// *****************************************************************************
// Section: DRV_APA102 Public Functions
// *****************************************************************************
// *****************************************************************************
//Driver Handling and Configuration
SYS_MODULE_OBJ DRV_APA102_Initialize(const SYS_MODULE_INDEX drvIndex, const SYS_MODULE_INIT* const init);
DRV_HANDLE DRV_APA102_Open(const SYS_MODULE_INDEX drvIndex, const DRV_IO_INTENT ioIntent);
void DRV_APA102_Close(const DRV_HANDLE handle);
void DRV_APA102_EventHandlerSet(const DRV_HANDLE handle, const DRV_APA102_APP_CALLBACK callback, const uintptr_t context);
bool DRV_APA102_ConfigSet(const DRV_HANDLE handle, DRV_APA102_CONFIG_PARAMS* config);

//Driver Memory Allocation and Manipulation APIs
bool DRV_APA102_AllocateBuffer(const DRV_HANDLE handle);
DRV_APA102_PAYLOAD_LED_FRAME* DRV_APA102_GetLedFramePtrAtIndex (const DRV_HANDLE handle, DRV_APA102_LIST_INDEX index);

//Driver Write APIs
bool DRV_APA102_InitializeLED(const DRV_HANDLE handle);
bool DRV_APA102_Instruction_Run(const DRV_HANDLE handle, uint16_t cntInstruction, DRV_APA102_INSTRUCTION_FREQUENCY freq);
bool DRV_APA102_Instruction_Stop(const DRV_HANDLE handle);
#endif /* _DRV_APA102_H */

/* *****************************************************************************
 End of File
 */
