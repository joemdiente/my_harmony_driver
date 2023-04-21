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

/* Main Menu + Options */
const uint8_t main_menu[] = 
{
    "*** APA102 RPi HAT Single Client Demonstration ***\r\n"
    "Connect APA102 RPi HAT in RPi Connector \r\n"
    "\r\n"
    "1: Fading LEDs\r\n"
    "2: Blink LEDs every 1 sec\r\n"
    "3: Running LEDs\r\n"    
    "Press any key to clear screen and print menu\r\n\r\n"
};
typedef enum
{
    CMD_LED_DEMO_FADE = '1',
    CMD_LED_DEMO_BLINK = '2',
    CMD_LED_DEMO_RUN = '3',
}CMD;
char cmd;

void drvAPA102EventHandler(DRV_APA102_EVENT event, uintptr_t context)
{
    printf("EventHandler Called!\r\n");
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
    
    confParam.ledCnt = 4;   
    confParam.transferParams.clkFreq = 1000000;
    confParam.drvOper = DRV_APA102_LED_STRIP;
    if(DRV_APA102_ConfigSet(rgbHandle,&confParam) != 0)
    {
        printf("Driver Configuration Setup Failed\r\n");
        DRV_APA102_Close(rgbHandle);
    };

    rgbData = DRV_APA102_AllocateBuffer(rgbHandle);
    rgbData[0].ledBrightness = 0;
    rgbData[1].ledBrightness = 0;
    rgbData[2].ledBrightness = 0;
    rgbData[3].ledBrightness = 0;
    DRV_APA102_UpdateLEDs (rgbHandle);

    while ( true )
    {
        printf ("\33[H\33[2J");
        printf("%s", main_menu);
        scanf("%c", &cmd);
        
        switch(cmd)
        {
            case CMD_LED_DEMO_FADE:
                printf ("\33[H\33[2J");
                printf("Fading LED Demo\r\n");
                printf("Press Any Key to Exit\r\n");
                DRV_APA102_UpdateLEDs (rgbHandle);
                scanf("%c", &cmd);
                
                break;

            case CMD_LED_DEMO_BLINK:
                printf ("\33[H\33[2J");
                printf("Blink LEDs every 1 Sec. Demo\r\n");
                printf("Press Any Key to Exit\r\n");
                scanf("%c", &cmd);
                break;

            case CMD_LED_DEMO_RUN:
                printf ("\33[H\33[2J");
                printf("Running LED Demo\r\n");
                printf("Press Any Key to Exit\r\n");
                scanf("%c", &cmd);
                break;
                
            default:    
                break;
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

