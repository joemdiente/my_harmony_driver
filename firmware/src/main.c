/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"
#include "driver/apa102/drv_apa102.h"


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

//Driver Handle
DRV_HANDLE rgbHandle;
//Transfer Setup
DRV_APA102_CONFIG_PARAMS confParam;
DRV_APA102_PAYLOAD_LED_FRAMES* rgbData;
//Instances
#define DRV_APA102_INSTANCE_0          0

volatile bool isWriteToLEDDone = false;
volatile bool WriteLEDError = true;
uint8_t wrBuffer[2];
uint8_t rdBuffer[2];

void drvAPA102EventHandler(DRV_APA102_EVENT event, uintptr_t context)
{
    SYS_CONSOLE_MESSAGE("EventHandler Called!\r\n");
    switch(event)
    {
        case DRV_APA102_EVENT_NONE:
            break;
        case DRV_APA102_EVENT_WRITE_DONE:
            isWriteToLEDDone = true;
            break;
        case DRV_APA102_EVENT_WRITE_ERROR:
            WriteLEDError = true;
            break;
    }
}

int main ( void )
{    
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    rgbHandle = DRV_APA102_Open(DRV_APA102_INSTANCE_0, 0);
    DRV_APA102_EventHandlerSet(rgbHandle, drvAPA102EventHandler, 0);
    
    confParam.ledCnt = 3;   
    confParam.transferParams.clkFreq = 1000000;
    confParam.drvOper = DRV_APA102_LED_STRIP;
    if(DRV_APA102_ConfigSet(rgbHandle,&confParam) != 0)
    {
        SYS_CONSOLE_MESSAGE("Driver Configuration Setup Failed\r\n");
        DRV_APA102_Close(rgbHandle);
    };
    
    rgbData = DRV_APA102_AllocateBuffer(rgbHandle);

    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

