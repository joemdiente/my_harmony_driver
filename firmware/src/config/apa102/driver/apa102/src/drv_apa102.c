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
#include "system/console/sys_console.h"

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

/* APA102 Internal Functions */
static void DRV_APA102_PLIBEventHandler(uintptr_t contextHandle)
{
    SYS_CONSOLE_MESSAGE("PlibEventHandler\r\n");
    
#ifdef DRV_APA102_DEBUG
    SYS_CONSOLE_PRINT("WARNING: Unimplemented %s\r\n",__func__);
#endif
//    DRV_APA102_OBJ* dObj = (DRV_APA102_OBJ *)contextHandle;
//    DRV_APA102_ERROR error;
//    
//    error = dObj->plibInterface->errorGet();
//    
//    if (dObj->wrInProgress == true && error == DRV_APA102_ERROR_NONE)
//    {    
//        dObj->wrInProgress = false;
//        dObj->eepChkBusyStatus = true;
//        
//        // Initiate checking of EEPROM's write busy status
//        dObj->wrBuffer[0] = 0xFF;
//        
//        dObj->plibInterface->write(dObj->configParams.eepromAddr, (void *)dObj->wrBuffer, 1);
//    }
//    else if (dObj->eepChkBusyStatus == true)
//    {
//        if (error == DRV_APA102_ERROR_NACK)
//        {
//            // EEPROM is still busy with the internal write. Keep checking for the EEPROM write busy status.
//            dObj->wrBuffer[0] = 0xFF;
//
//            dObj->plibInterface->write(dObj->configParams.eepromAddr, (void *)dObj->wrBuffer, 1);
//        }
//        else
//        {
//            // EEPROM internal write is completed.
//            dObj->eepChkBusyStatus = false;
//        }
//    }
//    
//    if (dObj->wrInProgress == false && dObj->eepChkBusyStatus == false)
//    {
//        if(error != DRV_APA102_ERROR_NONE)
//        {
//            dObj->event = DRV_APA102_EVENT_ERROR;
//        }        
//            
//        dObj->isBusy = false;
//        
//        if(dObj->callback != NULL)
//        {
//            dObj->callback(dObj->event, dObj->context);
//        }
//    }
}
//==========================TODO ABOVE==================//

//{  
//    /* Initialize LEDs */
//    if(dObj->plibInterface->write((uint8_t*)dObj->ptrPayload, dObj->sizePayload) != true)
//    {
//#ifdef DRV_APA102_DEBUG
//        SYS_CONSOLE_MESSAGE("ERROR: plibInterface Write Failed.\r\n");
//#endif
//        return SYS_MODULE_OBJ_INVALID;
//    }
//    else
//    {
//#ifdef DRV_APA102_DEBUG
//        SYS_CONSOLE_MESSAGE("INFO: plibInterface Write Success.\r\n");
//#endif
//    }
//    
//    //TODO: No PLIBEventHandler
////    dObj->plibInterface->callbackRegister(DRV_APA102_PLIBEventHandler, (uintptr_t)dObj);
//}
/* APA102 Public APIs */
/*
 * APA102 Driver Configuration
 */

bool DRV_APA102_UpdateStrip (const DRV_HANDLE handle)
{
    DRV_APA102_OBJ* dObj     = NULL;

#ifdef DRV_APA102_DEBUG
    SYS_CONSOLE_PRINT("INFO: %s\r\n",__func__);
#endif
    
    if (handle >= DRV_APA102_INSTANCES_NUMBER)
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("ERROR: index >= instance number\r\n");
#endif
        return NULL;
    }
    
    dObj = &gDrvAPA102Obj[handle];
    
    /* Initialize LEDs */
    if(dObj->plibInterface->write((uint8_t*)dObj->ptrPayload, dObj->sizePayload) != true)
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("ERROR: plibInterface Write Failed.\r\n");
#endif
        return SYS_MODULE_OBJ_INVALID;
    }
    else
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("INFO: plibInterface Write Success.\r\n");
#endif
    }
    return true;
}
DRV_APA102_PAYLOAD_LED_FRAMES* DRV_APA102_AllocateBuffer(const DRV_HANDLE handle)
{
    DRV_APA102_OBJ* dObj     = NULL;

#ifdef DRV_APA102_DEBUG
    SYS_CONSOLE_PRINT("INFO: %s\r\n",__func__);
#endif
    
    if (handle >= DRV_APA102_INSTANCES_NUMBER)
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("ERROR: index >= instance number\r\n");
#endif
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

    //Initialize memory
    memset(dObj->ptrPayload,0,dObj->sizePayload);
    
    //Format the allocated memory and pass pointer to application
    dObj->ptrPayload[0] = SFRAME_VAL;                                       //Start Frame 0s*32
    dObj->ptrPayload[dObj->drvConf.ledCnt] = EFRAME_VAL;                    //End Frame 1s*32 (Last Index is NULL so use it as End Frame)

    return (DRV_APA102_PAYLOAD_LED_FRAMES*)&dObj->ptrPayload[1];
}

bool DRV_APA102_ConfigSet(const DRV_HANDLE handle, DRV_APA102_CONFIG_PARAMS* config)
{
    DRV_APA102_OBJ* dObj     = NULL;
    
#ifdef DRV_APA102_DEBUG
    SYS_CONSOLE_PRINT("INFO: %s\r\n",__func__);
#endif
    
    if (handle >= DRV_APA102_INSTANCES_NUMBER)
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("ERROR: index >= instance number\r\n");
#endif
        return SYS_MODULE_OBJ_INVALID;
    }
    
    dObj = &gDrvAPA102Obj[handle];
    
    /* Verify 'config' if valid. */
    
    //Check LED Count
    if (config->ledCnt <= 0)
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_PRINT("ERROR: No. of LEDs: %u is less than 1.\r\n",dObj->drvConf.ledCnt);
#endif
        return SYS_MODULE_OBJ_INVALID;
    }
    else
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_PRINT("INFO: No. of LEDs: %u\r\n",dObj->drvConf.ledCnt);
#endif 
    }
    //Check Clock Frequency
    if (config->transferParams.clkFreq >= DRV_APA102_MAX_CLK ||
            config->transferParams.clkFreq <= DRV_APA102_MIN_CLK)
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("ERROR: Clock speed not in specs\r\n");
        SYS_CONSOLE_PRINT("INFO: CLK: %lu Hz\r\n",dObj->drvConf.transferParams.clkFreq);
#endif
        return SYS_MODULE_OBJ_INVALID;
    }
    
    /* Unsure */
    if (config->drvOper == DRV_APA102_LED_STRIP)
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("INFO: Mode is LED Strip\r\n");
#endif
    }
    else
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("ERROR: Unknown/Unimplemented Driver Operation\r\n");
#endif
        return SYS_MODULE_OBJ_INVALID;
    }
    
    //Copy config to drvConf
    dObj->drvConf = *config;
    /* Setup SPI Transfer Parameters (Hardcoded; This is APA102's SPI Mode) */
    dObj->drvConf.transferParams.clkPhase = DRV_APA102_CLOCK_PHASE_VALID_TRAILING_EDGE;
    dObj->drvConf.transferParams.clkPol = DRV_APA102_CLOCK_POLARITY_IDLE_HIGH;
    dObj->drvConf.transferParams.dataBits = DRV_APA102_DATA_BITS_8;
//    
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
//    dObj->plibInterface->callbackRegister(DRV_APA102_PLIBEventHandler, (uintptr_t)dObj);
    /* Initialize Driver Variables  */
    dObj->isBusy = false;
    dObj->nClients = 0;
    dObj->isInitialized = true;

    return (SYS_MODULE_OBJ)drvIndex;
}
/* *****************************************************************************
 End of File
 */
