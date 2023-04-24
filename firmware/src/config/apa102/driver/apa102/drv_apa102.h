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

/*
 * Driver Handling and Configuration
 */ 
SYS_MODULE_OBJ DRV_APA102_Initialize(const SYS_MODULE_INDEX drvIndex, const SYS_MODULE_INIT* const init);
DRV_HANDLE DRV_APA102_Open(const SYS_MODULE_INDEX drvIndex, const DRV_IO_INTENT ioIntent);
void DRV_APA102_Close(const DRV_HANDLE handle);
void DRV_APA102_EventHandlerSet(const DRV_HANDLE handle, const DRV_APA102_APP_CALLBACK callback, const uintptr_t context);
bool DRV_APA102_ConfigSet(const DRV_HANDLE handle, DRV_APA102_CONFIG_PARAMS* config);

/* 
 * Driver Memory Allocation and Manipulation APIs
 */
bool DRV_APA102_AllocateBuffer(const DRV_HANDLE handle);
DRV_APA102_LED_FRAME* DRV_APA102_GetLedFramePtrAtIndex (const DRV_HANDLE handle, DRV_APA102_LIST_INDEX index);

//Driver LED Instructions APIs
/* 
 * APA102 Driver Instruction Initialize LED
 * -Initializes all LEDs (Brightness, Red, Green, Blue) 
 */
bool DRV_APA102_Instruction_InitializeLED(const DRV_HANDLE handle);
/* 
 * APA102 Driver Instructions Operation Setup
 * -Configure Instruction Operation
 * --Run repeatedly or run once
 * --Number of instructions to run
 */
bool DRV_APA102_Instruction_OperSetup(const DRV_HANDLE handle, DRV_APA102_INSTRUCTION_CONFIG* instruction_config);
/* 
 * APA102 Driver Instructions Manual Update
 * -Allows user to manually trigger send instructions to the LEDs in series
 * --Automatically points to the next instruction in each calls.
 */
bool DRV_APA102_Instruction_ManualUpdate(const DRV_HANDLE handle);
/*
 * APA102 Driver Instruction Start
 * -Signals driver that instructions are already setup and can be run.
 * --Prevents modification of instruction list
 */
bool DRV_APA102_Instruction_Start(const DRV_HANDLE handle);
/* 
 * APA102 Driver Instruction Stop
 * -Signals driver that instructions are not ready and the list can be modified.
 * --Invoking trigger API results in error.
 */
bool DRV_APA102_Instruction_Stop(const DRV_HANDLE handle);
#endif /* _DRV_APA102_H */

/* *****************************************************************************
 End of File
 */
