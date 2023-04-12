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
#define DRV_APA102_MIN_CLK  8000000
#define DRV_APA102_MAX_CLK 12000000



DRV_APA102_OBJ gDrvRGBLedObj[DRV_APA102_INSTANCES_NUMBER];

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
    
    /* Store LED Count */
    dObj->ledMaxCnt = config->ledCnt;
    
    /* Does the CLK within Device Specifications? */
    if (config->transferParams.clockSpeed >= DRV_APA102_MAX_CLK ||
            config->transferParams.clockSpeed <= DRV_APA102_MIN_CLK)
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("ERROR: Clock speed not in specs\r\n");
        SYS_CONSOLE_PRINT("INFO: CLK: %lu Hz\r\n",config->transferParams.clockSpeed);
#endif
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Check Driver Operation */
    if (config->drvOper == DRV_APA102_LED_STRIP)
    {
#ifdef DRV_APA102_DEBUG
        SYS_CONSOLE_MESSAGE("INFO: Mode is LED Strip\r\n");
#endif
        //Allocate memory for RGBData
        dObj->rgbData = OSAL_Malloc(
                sizeof(DRV_APA102_RGB_DATA)*(dObj->ledMaxCnt) - 1);
        
        //Initialize RGBData
        for(i = 0; i < dObj->ledMaxCnt; i++)
        {
            dObj->rgbData[i].rgbArray.red = 0;
            dObj->rgbData[i].rgbArray.green = 0;
            dObj->rgbData[i].rgbArray.blue = 0;
        }
    }
    //Add here future implementation
    else
    {
        SYS_CONSOLE_MESSAGE("ERROR: Unknown/Unimplemented Driver Operation\r\n");
    }
     
    /* Get plibInterface */
    dObj->plibInterface = RGBLedInit->plibInterface;
    
    /* Initialize all LEDS */
    
    /* Post-Initialization */
    //TODO: No PLIBEventHandler
//    dObj->plibInterface->callbackRegister(DRV_APA102_PLIBEventHandler, (uintptr_t)dObj);
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
