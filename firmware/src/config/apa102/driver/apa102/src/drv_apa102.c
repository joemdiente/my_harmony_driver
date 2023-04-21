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

#ifdef DRV_APA102_DEBUG
    printf("INFO: %s\r\n",__func__);
#endif
    if(dObj->plibInterface->IsBusy() == true)
    {
        dObj->wrInProgress = true;
    }
    else
    {
        dObj->isBusy = false;
        dObj->wrInProgress = false;
    }

#ifdef DRV_APA102_DEBUG
    printf("WARNING: Unimplemented %s\r\n",__func__);
#endif
}
/* 
 * APA102 Update LEDs
 */
bool _DRV_APA102_UpdateLEDs (const DRV_HANDLE handle)
{
    DRV_APA102_OBJ* dObj     = NULL;

#ifdef DRV_APA102_DEBUG
    printf("INFO: %s\r\n",__func__);
#endif
    
    if (handle >= DRV_APA102_INSTANCES_NUMBER)
    {
#ifdef DRV_APA102_DEBUG
        printf("ERROR: index >= instance number\r\n");
#endif
        return NULL;
    }
    
    dObj = &gDrvAPA102Obj[handle];
    
    //Check if Driver is Busy
    if(dObj->isBusy)
    {
#ifdef DRV_APA102_DEBUG
        printf("ERROR: Driver is Busy. Write Failed\r\n");
#endif
        return false;
    }
    dObj->isBusy = true;
    
    //Check if buffer was allocated
    if(dObj->isBufferAllocated != true)
    {
#ifdef DRV_APA102_DEBUG
        printf("ERROR: Buffer not allocated\r\n");
#endif
    }
    
    //Format the allocated memory; Ensures LED Frames are always formatted before proceeding with write
    dObj->ptrPayload[0] = SFRAME_VAL;                                       //Start Frame 0s*32
    dObj->ptrPayload[dObj->drvConf.ledCnt] = EFRAME_VAL;                    //End Frame 1s*32 (Last Index is NULL so use it as End Frame)
    
    //Ensure all padding bits are 1s
    uint32_t i;
    DRV_APA102_PAYLOAD_LED_FRAMES* initLEDFrames = (DRV_APA102_PAYLOAD_LED_FRAMES*)&dObj->ptrPayload[1];
    for(i = 0; i < dObj->drvConf.ledCnt; i++)
    {
        initLEDFrames[i].padBits = 0b111;
    }
    
    /* Write to PLib */
    if(dObj->plibInterface->write((uint8_t*)dObj->ptrPayload, dObj->sizePayload) != true)
    {
#ifdef DRV_APA102_DEBUG
        printf("ERROR: plibInterface Write Failed.\r\n");
#endif
        return SYS_MODULE_OBJ_INVALID;
    }
    else
    {
#ifdef DRV_APA102_DEBUG
        printf("INFO: plibInterface Write Success.\r\n");
#endif
    }
    return true;
}
// *****************************************************************************
// *****************************************************************************
// Section: DRV_APA102 Public Functions
// *****************************************************************************
// *****************************************************************************
bool DRV_APA102_InitializeLED(const DRV_HANDLE handle)
{
    DRV_APA102_OBJ* dObj     = NULL;

#ifdef DRV_APA102_DEBUG
    printf("INFO: %s\r\n",__func__);
#endif
    
    if (handle >= DRV_APA102_INSTANCES_NUMBER)
    {
#ifdef DRV_APA102_DEBUG
        printf("ERROR: index >= instance number\r\n");
#endif
        return NULL;
    }
    
    dObj = &gDrvAPA102Obj[handle];
    
    //Initialize memory
    memset(dObj->ptrPayload,0,dObj->sizePayload);
    
    _DRV_APA102_UpdateLEDs(handle);

    return true;
}

/* 
 * APA102 Dynamic Buffer Allocation
 * based on LED count 
 */
DRV_APA102_PAYLOAD_LED_FRAMES* DRV_APA102_AllocateBuffer(const DRV_HANDLE handle)
{
    DRV_APA102_OBJ* dObj     = NULL;
    
    if (handle >= DRV_APA102_INSTANCES_NUMBER)
    {
        return (DRV_APA102_PAYLOAD_LED_FRAMES*)NULL;
    }
    
    dObj = &gDrvAPA102Obj[handle];
    
    /* Memory Allocation */
    //Calculate needed memory
    dObj->sizePayload = (DRV_APA102_PAYLOAD_SIZE)( 
            (sizeof(DRV_APA102_PAYLOAD_LED_FRAMES)*(dObj->drvConf.ledCnt - 1)) 
            + sizeof(DRV_APA102_PAYLOAD_START_FRAME) 
            + sizeof(DRV_APA102_PAYLOAD_END_FRAME)
            );

    //Allocate memory
    dObj->ptrPayload = OSAL_Malloc((size_t)dObj->sizePayload);

    if(dObj->ptrPayload != NULL)
    {
        dObj->isBufferAllocated = true;
    }
    
    return (DRV_APA102_PAYLOAD_LED_FRAMES*)&dObj->ptrPayload[1];
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
    if (config->ledCnt <= 0)
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
    
    return false;
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
    dObj->isInitialized = true;
    dObj->isBufferAllocated = false;
    
    return (SYS_MODULE_OBJ)drvIndex;
}
/* *****************************************************************************
 End of File
 */
