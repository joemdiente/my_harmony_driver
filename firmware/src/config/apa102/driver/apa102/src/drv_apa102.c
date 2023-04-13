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

//#ifdef DRV_APA102_DEBUG
#include "driver/apa102/drv_apa102_debug_helper.h"
//#endif

/* Driver Instance */
#define DRV_APA102_INSTANCES_NUMBER            1

/* Part-Specific Info */
#define DRV_APA102_MIN_CLK  800000
#define DRV_APA102_MAX_CLK 1200000

DRV_APA102_OBJ gDrvRGBLedObj[DRV_APA102_INSTANCES_NUMBER];

size_t DRV_APA102_Format_Payload (DRV_APA102_OBJ* dObj)
{
    /* Build Payload [32 bits/frame]
    * Start Frame                                  End Frame
    *   [0*32]   -   [LED 1]   -   [LED n + 1]   -   [1*32]
    * LED Frame
    *   [111 - Global(5b) - Blue(8b) - Green(8b) - Red(8b)] 
    * 
    * ::Global is brightness setting
    * Brightness   -   Driving Current
    * 00000        -       0/31       
    * 00001        -       1/31
    * .....        -       ....
    * 11111        -       31/31  
    * ::Blue/Green/Red are LEDs
    * Data         -   Duty Cycle
    * 00000000     -     0/256
    * 00000001     -     1/256
    * ........     -     .....
    * 11111111     -     255/256
    */
//    uint32_t startFrame = 0x00000000;
//    uint32_t endFrame = 0xFFFFFFFF;

    return (size_t)NULL;
} 

SYS_MODULE_OBJ DRV_APA102_Initialize(
    const SYS_MODULE_INDEX drvIndex,
    const SYS_MODULE_INIT* const init,
    DRV_APA102_CONFIG_PARAMS* config
)
{
    DRV_APA102_OBJ* dObj     = NULL;
    DRV_APA102_INIT* RGBLedInit = (DRV_APA102_INIT*)init;
    uint16_t i = 0;
    
    /* Validate the request */
    if(drvIndex >= DRV_APA102_INSTANCES_NUMBER)
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("ERROR: index >= instance number\r\n");
#endif
        return SYS_MODULE_OBJ_INVALID;
    }
    else
    {
        dObj = &gDrvRGBLedObj[drvIndex];
    }

    /* Is the driver instance already initialized? */
    if(dObj->isInitialized == true)
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("ERROR: Driver was re-initialized\r\n");
#endif
        return SYS_MODULE_OBJ_INVALID;
    }    
    
    //Get Configuration Parameters
    {
        dObj->confParam = *config;
    }
    
    /* Does the CLK within Device Specifications? */
    if (dObj->confParam.transferParams.clkFreq >= DRV_APA102_MAX_CLK ||
            dObj->confParam.transferParams.clkFreq <= DRV_APA102_MIN_CLK)
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("ERROR: Clock speed not in specs\r\n");
        SYS_CONSOLE_PRINT("INFO: CLK: %lu Hz\r\n",dObj->confParam.transferParams.clkFreq);
#endif
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Check Driver Operation */
    if (dObj->confParam.drvOper == DRV_APA102_LED_STRIP)
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("INFO: Mode is LED Strip\r\n");
#endif
        //Compute for RGBData + Start and End Frame
        dObj->sizeRgbData = sizeof(DRV_APA102_LED_FRAMES)*(dObj->confParam.ledCnt); //Size of rgbData (32bits * number of LEDs)
        dObj->sizePayload = dObj->sizeRgbData + 4*2;                                //Size of Start (32bits) and End Frame (32bits)

        //Allocate memory for payload 
        dObj->payload = OSAL_Malloc(dObj->sizePayload);      
        
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("INFO: Memory Allocated\r\n");
        SYS_CONSOLE_PRINT("INFO: Payload Size: %u\r\n",dObj->sizePayload);
#endif
        
        //Initialize RGBData
        for(i = 0; i < dObj->confParam.ledCnt; i++)
        {
            dObj->rgbData[i].padBits = 0b111;
            dObj->rgbData[i].ledBrightness = 0;
            dObj->rgbData[i].red = 0;
            dObj->rgbData[i].green = 0;
            dObj->rgbData[i].blue = 0;
        }
    }
    //Add here future implementation
    else
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("ERROR: Unknown/Unimplemented Driver Operation\r\n");
#endif  
        return SYS_MODULE_OBJ_INVALID;
    }
     
    /* Get plibInterface */
    dObj->plibInterface = RGBLedInit->plibInterface;
    dObj->confParam.transferParams.clkPhase = DRV_APA102_CLOCK_PHASE_VALID_TRAILING_EDGE;
    dObj->confParam.transferParams.clkPol = DRV_APA102_CLOCK_POLARITY_IDLE_HIGH;
    dObj->confParam.transferParams.dataBits = DRV_APA102_DATA_BITS_8;
    
    /* Setup Transfer */
    if(dObj->plibInterface->transferSetup(&dObj->confParam.transferParams, 0) != true) 
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("ERROR: plibInterface TransferSetup Failed.\r\n");   
#endif
        return SYS_MODULE_OBJ_INVALID;
    }
    /* Initialize all LEDS */
//#ifdef DRV_APA102_DEBUG
//    DRV_APA102_DEBUG_FILL_RGBDATA(dObj);
//#endif   
    //Format Payload
//    dObj->payload->startFrame = 0x00000000;
//    dObj->payload->ledFrames = dObj->rgbData;
//    dObj->payload->endFrame = 0xFFFFFFFF;
    
//    dObj->sizePayload = DRV_APA102_Format_Payload(dObj);
            
    //Send Payload
//    if(dObj->plibInterface->write((uint8_t*)dObj->payload, dObj->sizePayload))
    {
//#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("ERROR: plibInterface Write Failed.\r\n");
//#endif
//        return SYS_MODULE_OBJ_INVALID;
    }
    
    
    //TODO: No PLIBEventHandler
//    dObj->plibInterface->callbackRegister(DRV_APA102_PLIBEventHandler, (uintptr_t)dObj);
    
    /* Post-Initialization */
    dObj->isBusy = false;
    dObj->nClients = 0;
    dObj->isInitialized = true;

#ifdef DRV_APA102_DEBUG
    SYS_CONSOLE_MESSAGE("SUCCESS: An instance was initialized\r\n");
#endif
    return (SYS_MODULE_OBJ)drvIndex;
}

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
    
    dObj = &gDrvRGBLedObj[drvIndex];

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
/* *****************************************************************************
 End of File
 */
