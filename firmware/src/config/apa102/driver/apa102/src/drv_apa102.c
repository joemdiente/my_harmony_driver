//driver_xxx_definitions.h : contains data structures and definitions that will be needed by initialization.c file
    //This file along with the driver_xxx.h file gets included in the definitions.h file which in turn is included in the initialization.c file
//driver_xxx_local.h : contains data structures and definitions that are local to the driver
    //-->includes driver_xxx_definitions file as the driver_xxx_local may use some of the definitions defined in this file
//driver_xxx.h : contains data structures, definitions and function prototypes. This file will be included by application using this driver.
   //-->includes driver_xxx_definitions file as the driver_xxx_local may use some of the definitions defined in this file
//driver_xxx.c : contains driver implementation and public APIs.
  //-->includes driver_xxx.h file

//------------------------------------------------------------------------------------------------  

/* Debug */
#define DRV_APA102_DEBUG 

#include "driver/apa102/drv_apa102.h"
#include "driver/apa102/drv_apa102_local.h"
#include "osal/osal_impl_basic.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#ifdef DRV_APA102_DEBUG
#include "driver/apa102/drv_apa102_debug_helper.h"
#endif

/* Driver Instance */
#define DRV_APA102_INSTANCES_NUMBER            1

/* Part-Specific Info */
#define DRV_APA102_MIN_CLK  800000      //800KHz
#define DRV_APA102_MAX_CLK 1200000      //1.2MHz
DRV_APA102_PAYLOAD_START_FRAME sFrame = SFRAME_VAL; 
DRV_APA102_PAYLOAD_END_FRAME eFrame = EFRAME_VAL; 

DRV_APA102_OBJ gDrvAPA102Obj[DRV_APA102_INSTANCES_NUMBER];

// *****************************************************************************
// *****************************************************************************
// Section: DRV_APA102 Private Functions
// *****************************************************************************
// *****************************************************************************

/* 
 * APA102 Driver PLIBEventHandler
 * ::This is called whenever one SPI transfer is finished.
 */
static void DRV_APA102_PLIBEventHandler(uintptr_t contextHandle)
{
    DRV_APA102_OBJ* dObj     = (DRV_APA102_OBJ*) contextHandle;

    if(dObj->isRunning == true)
    {
        //One Instruction was executed
        dObj->currInstructionIndex++;
        if(dObj->currInstructionIndex >= dObj->totalInstruction && dObj->shouldRepeat == true)
        {
            //Reset Instruction Index
            dObj->currInstructionIndex = 0;
        }
        if(dObj->currInstructionIndex < dObj->totalInstruction)
        {
            //Update pointer payload based on current instruction index
            dObj->currPtrPayload = (DRV_APA102_PAYLOAD_PTR *)(
                dObj->drvBuffer +
                (dObj->sizePayload/4) * 
                dObj->currInstructionIndex);
            // Write to PLib
            dObj->plibInterface->write((uint8_t*)dObj->currPtrPayload, dObj->sizePayload);
        }
        else
        {
            //Stop
            dObj->currInstructionIndex = 0;
            dObj->isRunning = false;
            dObj->isBusy = false;
        }
    }
    

}
/* 
 * APA102 Update LEDs
 */
bool _DRV_APA102_FormatAllFrames (const DRV_HANDLE handle)
{
    DRV_APA102_OBJ* dObj     = NULL;
    uint32_t i;
    
#ifdef DRV_APA102_DEBUG
    printf("INFO: %s\r\n",__func__);
#endif
    
    if (handle >= DRV_APA102_INSTANCES_NUMBER)
    {
        return NULL;
    }
    
    dObj = &gDrvAPA102Obj[handle];
    
    //Check if buffer was allocated
    if(dObj->isBufferAllocated != true)
    {
#ifdef DRV_APA102_DEBUG
        printf("ERROR: Buffer not allocated\r\n");
#endif
    }
    
    //Format All Frames to Match APA102 Protocol
    for (dObj->currInstructionIndex = 0; 
        dObj->currInstructionIndex < dObj->totalInstruction; 
        dObj->currInstructionIndex++)
    {
        //Update pointer payload based on current instruction index
        dObj->currPtrPayload = (DRV_APA102_PAYLOAD_PTR *)(
                dObj->drvBuffer +
                (dObj->sizePayload/4) * 
                dObj->currInstructionIndex);
    
        //Format the allocated memory; Ensures LED Frames are always formatted before proceeding with write
        dObj->currPtrPayload[0] = SFRAME_VAL;                                           //Start Frame 0s*32
        dObj->currPtrPayload[dObj->drvConf.ledMaxCnt + 1] = EFRAME_VAL;                //End Frame 1s*32 (Placed after LED Frames, +1)
    
        //Ensure all padding bits are 1s
        DRV_APA102_PAYLOAD_LED_FRAME* initLEDFrames = (DRV_APA102_PAYLOAD_LED_FRAME*)&dObj->currPtrPayload[1];
        for(i = 0; i < dObj->drvConf.ledMaxCnt; i++)
        {
            initLEDFrames[i].padBits = 0b111;
        }
    }
    
    //Reset currInstructionIndex
    dObj->currInstructionIndex = 0;
    
    return true;
}
// *****************************************************************************
// *****************************************************************************
// Section: DRV_APA102 Public Functions
// *****************************************************************************
// *****************************************************************************
/*
 * APA102 Driver Run Instruction
 */
bool DRV_APA102_Instruction_Stop(const DRV_HANDLE handle)
{
    DRV_APA102_OBJ* dObj     = NULL;
    
    if (handle >= DRV_APA102_INSTANCES_NUMBER)
    {
        return NULL;
    }
    
    dObj = &gDrvAPA102Obj[handle];
    dObj->currInstructionIndex = 0xFFFF;
    dObj->shouldRepeat = false;
    dObj->isRunning = false;
    dObj->isBusy = false;
    
    return true;
}
/*
 * APA102 Driver Run Instruction
 */
bool DRV_APA102_Instruction_Run(const DRV_HANDLE handle, uint16_t cntInstruction, DRV_APA102_INSTRUCTION_FREQUENCY freq)
{
    DRV_APA102_OBJ* dObj     = NULL;
    
    if (handle >= DRV_APA102_INSTANCES_NUMBER)
    {
        return NULL;
    }
    
    dObj = &gDrvAPA102Obj[handle];
    
    if(dObj->isRunning == true)
    {
        printf("ERROR: Driver is Running. Initiate Stop()\r\n");
        return false;
    }
    if(dObj->isBusy == true || dObj->plibInterface->IsBusy())
    {
        printf("ERROR: Driver and PLIB is Busy\r\n");
        return false;
    }
    
    if(cntInstruction == 0)
    {
        printf("ERROR: Number of Instructions Not Specified\r\n");
        return false;
    }
    
    //Initialize memory
    dObj->currInstructionIndex = 0;
    dObj->totalInstruction = cntInstruction;
    dObj->shouldRepeat = freq;
    dObj->isBusy = true;
    dObj->isRunning = true;

    _DRV_APA102_FormatAllFrames(handle); 
    
    /* Write to PLib */
    if(dObj->plibInterface->write((uint8_t*)dObj->currPtrPayload, dObj->sizePayload) != true)
    {
#ifdef DRV_APA102_DEBUG
        printf("ERROR: plibInterface Failed!\r\n");
#endif
        return SYS_MODULE_OBJ_INVALID;
    }
            
    return true;
}
/*
 * APA102 Driver Initialize All LEDs to 0
 */
bool DRV_APA102_InitializeLED(const DRV_HANDLE handle)
{
    DRV_APA102_OBJ* dObj     = NULL;
    
    if (handle >= DRV_APA102_INSTANCES_NUMBER)
    {
        return NULL;
    }
    
    dObj = &gDrvAPA102Obj[handle];
    
    //Initialize memory
    memset(dObj->drvBuffer, 0,dObj->sizePayload);
    
    DRV_APA102_Instruction_Run(handle, 1, 0);
            
    return true;
}
/*
 * 
 */
DRV_APA102_PAYLOAD_LED_FRAME* DRV_APA102_GetLedFramePtrAtIndex (const DRV_HANDLE handle, 
        DRV_APA102_LIST_INDEX index)
{
    DRV_APA102_OBJ* dObj     = NULL;
    
    if (handle >= DRV_APA102_INSTANCES_NUMBER)
    {
        return false;
    }
    
    dObj = &gDrvAPA102Obj[handle];
    
    //Get LED Frame Pointer Starting at Index 0
    return (DRV_APA102_PAYLOAD_LED_FRAME*) 
            (dObj->drvBuffer + 
            1 +                     //LED Frame is Index 1 of Payload
            (dObj->sizePayload/4) * index);
}
/* 
 * APA102 Dynamic Buffer Allocation
 * based on LED count and number of LED Instruction
 */
bool DRV_APA102_AllocateBuffer(const DRV_HANDLE handle)
{
    DRV_APA102_OBJ* dObj     = NULL;
    
    if (handle >= DRV_APA102_INSTANCES_NUMBER)
    {
        return false;
    }
    
    dObj = &gDrvAPA102Obj[handle];
    
    /* Memory Allocation */
    //Single Payload
    dObj->sizePayload = (DRV_APA102_PAYLOAD_SIZE)( 
            (sizeof(DRV_APA102_PAYLOAD_LED_FRAME)*(dObj->drvConf.ledMaxCnt)) 
            + sizeof(DRV_APA102_PAYLOAD_START_FRAME) 
            + sizeof(DRV_APA102_PAYLOAD_END_FRAME)
            );
    
    //List of Instructions
    dObj->sizeBuffer = dObj->sizePayload * dObj->drvConf.instructionMaxCnt;
    
    //Allocate memory for payload only
    dObj->drvBuffer = OSAL_Malloc((size_t)dObj->sizeBuffer);
    
    if(dObj->drvBuffer != NULL)
    {
        dObj->isBufferAllocated = true;
#ifdef DRV_APA102_DEBUG
        printf("INFO: Buffer addr: %p\r\n", (void*)dObj->drvBuffer);
#endif
        memset(dObj->drvBuffer,0,dObj->sizeBuffer);
        return true;
    }        
    
    return false;
}
/* 
 * APA102 Driver Configuration Setup 
 */
bool DRV_APA102_ConfigSet(const DRV_HANDLE handle, DRV_APA102_CONFIG_PARAMS* config)
{
    DRV_APA102_OBJ* dObj     = NULL;
    
    if (handle >= DRV_APA102_INSTANCES_NUMBER)
    {
        return SYS_MODULE_OBJ_INVALID;
    }
    
    dObj = &gDrvAPA102Obj[handle];
    
    /* Verify 'config' if valid. */
    
    //Check LED Count
    if (config->ledMaxCnt <= 0)
    {
        return SYS_MODULE_OBJ_INVALID;
    }
    
    //Check Number of Instruction
    if (config->instructionMaxCnt <= 0)
    {
        return SYS_MODULE_OBJ_INVALID;
    }
    
    //Check Clock Frequency
    if (config->transferParams.clkFreq >= DRV_APA102_MAX_CLK ||
            config->transferParams.clkFreq <= DRV_APA102_MIN_CLK)
    {
        return SYS_MODULE_OBJ_INVALID;
    }
    
    /* Unsure */
    if (config->drvOper == DRV_APA102_LED_STRIP)
    {
    }
    else
    {
        return SYS_MODULE_OBJ_INVALID;
    }
    
    //Copy new config to drvConf
    dObj->drvConf = *config;
    
    /* Setup SPI Transfer Parameters (Hardcoded; This is APA102's SPI Mode) */
    dObj->drvConf.transferParams.clkPhase = DRV_APA102_CLOCK_PHASE_VALID_TRAILING_EDGE;
    dObj->drvConf.transferParams.clkPol = DRV_APA102_CLOCK_POLARITY_IDLE_HIGH;
    dObj->drvConf.transferParams.dataBits = DRV_APA102_DATA_BITS_8;
    
    dObj->plibInterface->transferSetup(&dObj->drvConf.transferParams,0);
    dObj->isSetupChanged = true;
    
    return true;
}
/*
 * APA102 Driver Event Handler Setup
 */
void DRV_APA102_EventHandlerSet(
    const DRV_HANDLE handle,
    const DRV_APA102_APP_CALLBACK callback,
    const uintptr_t context
)
{
    DRV_APA102_OBJ* dObj     = NULL;
    
    if (handle >= DRV_APA102_INSTANCES_NUMBER)
    {
        return;
    }
    
    dObj = &gDrvAPA102Obj[handle];
    
    /* Configure the Event Handler and callback function */
    dObj->callback = callback;
    dObj->event = DRV_APA102_EVENT_NONE;
    dObj->context = context;
}
/*
 * APA102 Driver Close
 */
void DRV_APA102_Close(const DRV_HANDLE handle)
{
    if (handle >= DRV_APA102_INSTANCES_NUMBER)
    {
        return;
    }
    
    if (gDrvAPA102Obj[handle].isBusy == true)
    {
        return;
    }

    gDrvAPA102Obj[handle].nClients = 0;
}
/*
 * APA102 Driver Open
 */
DRV_HANDLE DRV_APA102_Open(
    const SYS_MODULE_INDEX drvIndex,
    const DRV_IO_INTENT ioIntent
)
{
    DRV_APA102_OBJ* dObj = NULL;

    /* Validate the request */
    if(drvIndex >= DRV_APA102_INSTANCES_NUMBER)
    {
        return DRV_HANDLE_INVALID;
    }
    
    dObj = &gDrvAPA102Obj[drvIndex];

    /* Driver must be initialized before it can be opened by a client. Also, it must not already be in use (opened state). */
    if(dObj->isInitialized == false || dObj->nClients != 0)
    {
        return DRV_HANDLE_INVALID;
    }
    else
    {
        dObj->nClients = 1;
    }
    
    return drvIndex;
}

/*
 * APA102 Driver Initialize
 */
SYS_MODULE_OBJ DRV_APA102_Initialize(
    const SYS_MODULE_INDEX drvIndex,
    const SYS_MODULE_INIT* const init
)
{
    DRV_APA102_OBJ* dObj     = NULL;
    DRV_APA102_INIT* APA102Init = (DRV_APA102_INIT*)init;

    /* Validate the request */
    if(drvIndex >= DRV_APA102_INSTANCES_NUMBER)
    {
        return SYS_MODULE_OBJ_INVALID;
    }
    else
    {
        dObj = &gDrvAPA102Obj[drvIndex];
    }

    /* Is the driver instance already initialized? */
    if(dObj->isInitialized == true)
    {
        return SYS_MODULE_OBJ_INVALID;
    }    
    
    /* Initialize pLibInterface */
    dObj->plibInterface = APA102Init->plibInterface;
    dObj->plibInterface->callbackRegister(DRV_APA102_PLIBEventHandler, (uintptr_t)dObj);
    
    /* Initialize Driver Variables  */
    dObj->isBusy = false;
    dObj->nClients = 0;
    
    dObj->isBufferAllocated = false;
    dObj->isRunning = false;
    
    dObj->isInitialized = true;
    return (SYS_MODULE_OBJ)drvIndex;
}
/* *****************************************************************************
 End of File
 */
